/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "scalebandstablemodel.h"

#include <cctype>
#include <functional>

#include <QCheckBox>
#include <QAbstractTableModel>
#include <QtMath>

#include <iostream>
using namespace std;


ScaleBandsTableModel::ScaleBandsTableModel(QObject *parent)
  : QAbstractTableModel( parent )
  , m_layer( NULL )
{
}

ScaleBandsTableModel::~ScaleBandsTableModel()
{

}

void ScaleBandsTableModel::dataLayer( TSLDirectImportDataLayer* layer )
{
  m_layer = layer;

  unsigned int numScaleBands = m_layer ? m_layer->numScaleBands() : 0;
  if( m_layer && numScaleBands != 0 )
  {
    for( unsigned int i(0); i < m_layer->numScaleBands(); ++i )
    {
      TSLDirectImportScaleBand* band( m_layer->getScaleBand(i) );
      const char* name( band->name() );
      ScaleBandTableItem item;
      item.m_name = name ? name : "";
      item.m_minScale = band->minScale();

      if( item.m_minScale == 0.0 )
      {
        item.m_minScaleStr = "1:Infinity";
      }
      else
      {
        item.m_minScaleStr = "1:";
        item.m_minScaleStr += QString::number( (int)( 1.0 / item.m_minScale ) );
      }
      item.m_tilesX = band->tilesX();
      item.m_tilesY = band->tilesY();
      item.m_automaticTiling = band->automaticTiling();
      item.m_polarTiling = band->tilingMethod() == TSLDirectImportScaleBand::TilingMethodPolarOptimised;
      m_data.push_back( item );
    }
  }
  else
  {
    if (m_data.empty())
    {
      // The TSLDirectImportDataLayer requires at least one scale band to be created
      ScaleBandTableItem item;
      item.m_name = "Default";
      item.m_minScale = 0.0;
      item.m_minScaleStr = "1:Infinity";
      item.m_tilesX = 0;
      item.m_tilesY = 0;
      item.m_automaticTiling = true;
      item.m_polarTiling = false;
      m_data.push_back( item );
    }
  }
}

bool ScaleBandsTableModel::scaleBandsValid() const
{
  if( m_data.empty() )
  {
    return false;
  }

  // Each scale band must be valid, and in the correct order
  const ScaleBandTableItem* prevItem( NULL );
  std::vector<ScaleBandTableItem>::const_iterator it( m_data.begin() );
  std::vector<ScaleBandTableItem>::const_iterator itE( m_data.end() );
  for( int index = 0; it != itE; ++it, ++index )
  {
    const ScaleBandTableItem* item( &(*it) );
    if( item->m_name.isEmpty() )
    {
      std::cout << "name is empty " << index << std::endl;
      return false;
    }
    if( std::count_if( m_data.begin(), m_data.end(), ScaleBandTableItemNamePredicate(item->m_name) ) > 1 )
    {
      std::cout << "name is a problem " << index << std::endl;
      return false;
    }

    if( prevItem )
    {
      //std::cout << "scale values " << index << " " <<  prevItem->m_minScale << " " << item->m_minScale << std::endl;

      if( prevItem->m_minScale > item->m_minScale )
      {
        std::cout << "scale value issue " << index << " " <<  prevItem->m_minScale << " " << item->m_minScale << std::endl;
        return false;
      }
    }
    prevItem = item;
  }
  return true;
}

bool ScaleBandsTableModel::saveToLayer()
{
  if( !scaleBandsValid() )
  {
    return false;
  }
  
  // Note: This function assumes the data layer is clean, and hasn't already got non-default
  // scale bands.
  std::vector<ScaleBandTableItem>::const_iterator it( m_data.begin() );
  std::vector<ScaleBandTableItem>::const_iterator itE( m_data.end() );
  for( ; it != itE; ++it )
  {
    const ScaleBandTableItem& item( *it );
    unsigned int tilesX(0);
    unsigned int tilesY(0);
    if( item.m_automaticTiling == false )
    {
      tilesX = item.m_tilesX;
      tilesY = item.m_tilesY;
    }
    TSLDirectImportScaleBand* scaleBand = m_layer->addScaleBand(item.m_minScale, item.m_name.toUtf8(), tilesX, tilesY);
    if( !scaleBand)
    {
      return false;
    }
    if (item.m_polarTiling)
    {
      scaleBand->tilingMethod(TSLDirectImportScaleBand::TilingMethodPolarOptimised);
    }
  }

  return true;
}

int ScaleBandsTableModel::rowCount(const QModelIndex& parent) const
{
  return m_data.size();
}

int ScaleBandsTableModel::columnCount(const QModelIndex &parent) const
{
  return ColumnMax;
}

QVariant ScaleBandsTableModel::data(const QModelIndex &index, int role) const
{
  if( m_layer == NULL )
  {
    return QVariant();
  }
  unsigned int i( index.row() );
  if( i >= m_data.size() )
  {
    return QVariant();
  }
  const ScaleBandTableItem& band( m_data[i] );

  if( role == Qt::DisplayRole )
  {
    switch( index.column() )
    {
      case ColumnName:
        return band.m_name;
      case ColumnMinScale:
        return band.m_minScaleStr;
      case ColumnAutomaticTiling:
      {
        // Cell is handled via checkstate
        return QVariant();
      }
      case ColumnPolarTiling:
      {
        // Cell is handled via checkstate
        return QVariant();
      }
      case ColumnTilesX:
        return band.m_tilesX;
      case ColumnTilesY:
        return band.m_tilesY;
    }
  }
  if( role == Qt::CheckStateRole )
  {
    if( index.column() == ColumnAutomaticTiling )
    {
      return band.m_automaticTiling ? Qt::Checked : Qt::Unchecked;
    }
    if (index.column() == ColumnPolarTiling)
    {
      return band.m_polarTiling ? Qt::Checked : Qt::Unchecked;
    }
  }

  return QVariant();
}

QVariant ScaleBandsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if( role == Qt::DisplayRole )
  {
    if( orientation == Qt::Horizontal )
    {
      switch( section )
      {
        case ColumnName:
          return QString("Name");
        case ColumnMinScale:
          return QString("Min Scale");
        case ColumnAutomaticTiling:
          return QString("Auto Tiling");
        case ColumnPolarTiling:
          return QString("Polar Tile\nOptimisation");
        case ColumnTilesX:
          return QString("Tiles X");
        case ColumnTilesY:
          return QString("Tiles Y");
      }
    }
  }
  return QVariant();
}

bool ScaleBandsTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  if( m_layer == NULL )
  {
    return false;
  }
  if( !value.isValid() )
  {
    return false;
  }

  unsigned int i( index.row() );
  if( i >= m_data.size() )
  {
    return false;
  }
  ScaleBandTableItem& band( m_data[i] );

  // Index bounds to flag as changed
  QModelIndex topLeft( index );
  QModelIndex bottomRight( index );

  if( role == Qt::EditRole )
  {
    // Leave the existing data if a new value isn't entered
    if( value.toString().isEmpty() )
    {
      return false;
    }
    // Update the relevant value
    switch( index.column() )
    {
      case ColumnName:
        band.m_name = value.toString();
        break;
      case ColumnMinScale:
      {
        QString qScaleStr( value.toString() );
        // Scale string in the form 1:X or 1:XK
        band.m_minScaleStr = qScaleStr;
        std::string scaleStr( band.m_minScaleStr.toUtf8() );
        std::transform( scaleStr.begin(), scaleStr.end(), scaleStr.begin(), std::ptr_fun<int,int>(std::toupper) );
        size_t firstNonSpace( scaleStr.find_first_not_of(' '));
        if( firstNonSpace != std::string::npos )
        {
          scaleStr.erase(0, firstNonSpace);
        }
        size_t lastNonSpace( scaleStr.find_last_not_of(' '));
        if( lastNonSpace != std::string::npos )
        {
          if( lastNonSpace < scaleStr.size() )
          {
            scaleStr.erase( lastNonSpace + 1, std::string::npos );
          }
        }
        if( scaleStr.empty() )
        {
          return false;
        }
        unsigned int mult( 1 );
        char lastChar( scaleStr[scaleStr.size() - 1] );
        if( lastChar == 'K' )
        {
          mult = 1000;
          scaleStr.resize( scaleStr.size() - 1 );
        }
        else if( lastChar == 'M' )
        {
          mult = 1000000;
          scaleStr.resize( scaleStr.size() - 1 );
        }

        size_t colonPos( scaleStr.find(":") );
        if( colonPos != std::string::npos && colonPos < scaleStr.size() )
        {
          scaleStr = scaleStr.substr( colonPos + 1, std::string::npos );
        }
        // Assume the number they typed in meant 1 / num

        qScaleStr = scaleStr.c_str();
        unsigned int scaleNum( qScaleStr.toInt() );
        scaleNum *= mult;
        band.m_minScale = scaleNum ? (1.0 / (double)scaleNum) : 0.0;
        if( band.m_minScale == 0.0 )
        {
          band.m_minScaleStr = "1:Infinity";
        }
        else
        {
          band.m_minScaleStr = "1:";
          double r( 1.0 / band.m_minScale );
          QString suffix;
          if( r > 1000.0 )
          {
            r /= 1000.0;
            suffix = "K";
          }
          if( r > 1000.0 )
          {
            r /= 1000.0;
            suffix = "M";
          }
          band.m_minScaleStr += QString::number( r, 'f', 1 );
          band.m_minScaleStr += suffix;
        }

        break;
      }
      case ColumnAutomaticTiling:
        // This cell is handled via the checkstate
        break;
      case ColumnPolarTiling:
        // This cell is handled via the checkstate
        break;
      case ColumnTilesX:
        band.m_tilesX = value.toInt();
        break;
      case ColumnTilesY:
        band.m_tilesY = value.toInt();
        break;
    }
  }
  if( role == Qt::CheckStateRole )
  {
    if( index.column() == ColumnAutomaticTiling )
    {
      if( value == Qt::Checked )
      {
        band.m_automaticTiling = true;
        band.m_tilesX = 0;
        band.m_tilesY = 0;
      }
      else
      {
        band.m_automaticTiling = false;
      }
      // Changing this column will change the flags of the tilesX and tilesY columns
      bottomRight = createIndex( index.row(), (int)ColumnTilesY );
    }
    if (index.column() == ColumnPolarTiling)
    {
      if (value == Qt::Checked)
      {
        band.m_polarTiling = true;
      }
      else
      {
        band.m_polarTiling = false;
      }
    }
  }

  emit dataChanged( topLeft, bottomRight );
  return true;
}

Qt::ItemFlags ScaleBandsTableModel::flags(const QModelIndex& index ) const
{
  unsigned int i( index.row() );
  if( i >= m_data.size() )
  {
    return false;
  }
  const ScaleBandTableItem& band( m_data[i] );

  Qt::ItemFlags result;

  switch( index.column() )
  {
    case ColumnAutomaticTiling:
      result = Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable;
      break;
    case ColumnPolarTiling:
      result = Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable;
      break;
    case ColumnTilesX:
    case ColumnTilesY:
      result = Qt::ItemIsEditable | Qt::ItemIsSelectable;
      if( band.m_automaticTiling == false )
      {
        result |= Qt::ItemIsEnabled;
      }
      break;
    default:
      result = Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled;
      break;
  }
  return result;
}


bool ScaleBandsTableModel::insertRows(int row, int count, const QModelIndex & parent)
{
  if( row > m_data.size() )
  {
    return false;
  }
  if( parent != QModelIndex() )
  {
    return false;
  }
  beginInsertRows( parent, row, row + count - 1 );

  // Simple implementation, just add a default item
  // based on the previous one
  for( int i(0); i < count; ++i, ++row )
  {
    const ScaleBandTableItem* prevItem( NULL );
    if( row > 0 )
    {
      prevItem = &(m_data[row - 1]);
    }

    ScaleBandTableItem item;
    item.m_name = "ScaleBand_" + QString::number(row);
    if( prevItem )
    {
      item.m_minScale = prevItem->m_minScale;
      item.m_minScaleStr = prevItem->m_minScaleStr;
    }
    else
    {
      item.m_minScale = 0.0;
      item.m_minScaleStr = "1:Infinity";
    }
    item.m_tilesX = 0;
    item.m_tilesY = 0;
    item.m_automaticTiling = true;
    item.m_polarTiling = false;

    if( row == m_data.size() )
    {
      m_data.push_back( item );
    }
    else
    {
      m_data.insert( m_data.begin() + row, item );
    }
  }

  endInsertRows();
  return true;
}

bool ScaleBandsTableModel::removeRows(int row, int count, const QModelIndex & parent)
{
  if( row >= m_data.size() )
  {
    return false;
  }
  if( parent != QModelIndex() )
  {
    return false;
  }
  beginRemoveRows(parent, row, row + count -1);

  m_data.erase( m_data.begin() + row, m_data.begin() + row + count );

  endRemoveRows();
  return true;
}









