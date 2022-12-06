/****************************************************************************
  Copyright (c) 2017 by Envitia Group PLC.
 ****************************************************************************/

#include "wmtsservicedimensionsmodel.h"
#include <QAbstractItemView>
#include <QComboBox>

#include "MapLink.h"
#include "MapLinkWMTSDataLayer.h"
#include "tslplatformhelper.h"

namespace Services
{
  WMTSServiceDimensionsModel::WMTSServiceDimensionsModel( TSLWMTSServiceInfo *serviceInfo )
    : m_serviceInfo( serviceInfo )
  {
    // Build up a list of dimensions that apply to the visible layers and store
    // accessors to them so we don't have to traverse the layer tree each time
    if( serviceInfo )
    {
      extractVisibleLayerDimensions( serviceInfo );

      // Remove any duplicate entries
      std::unique( m_dimensions.begin(), m_dimensions.end() );
    }
  }

  WMTSServiceDimensionsModel::~WMTSServiceDimensionsModel()
  {
  }

  bool WMTSServiceDimensionsModel::configurationValid() const
  {
    // All fields are considered to be complete if each dimension returns a value from getDimensionValue(),
    // whether this is the service-defined default value or a user-specified value
    size_t numDimensions = m_dimensions.size();
    for( size_t i = 0; i < numDimensions; ++i )
    {
      const TSLWMTSServiceDimension *dimension = m_dimensions[i].second->getDimensionAt( m_dimensions[i].first );
      if( !m_dimensions[i].second->getDimensionValue( dimension->identifier() ) )
      {
        return false;
      }
    }

    return true;
  }

  void WMTSServiceDimensionsModel::setDelegates( QAbstractItemView *view )
  {
    size_t numDimensions = m_dimensions.size();
    for( size_t i = 0; i < numDimensions; ++i )
    {
      const TSLWMTSServiceDimension *dimension = m_dimensions[i].second->getDimensionAt( m_dimensions[i].first );
      if( dimension->uom() && TSLPlatformHelper::stricmp( dimension->uom(), "ISO8601" ) == 0 )
      {
        // Use the date editor for modifying dates in IS8601 format
        view->setItemDelegateForRow( i, &m_dateDelegate );
      }
      else if( dimension->numValues() > 0 )
      {
        // Otherwise, if the service provides some fixed values, use a combobox instead
        view->setItemDelegateForRow( i, &m_prescribedValueDelegate );
      }
    }
  }

  bool WMTSServiceDimensionsModel::getPossibleValues( const QModelIndex &index, std::vector< const char* > &values ) const
  {
    if( !index.isValid() )
    {
      return false;
    }

    const std::pair< int, const TSLWMTSServiceLayer* > &dimensionPair = m_dimensions[index.row()];
    const TSLWMTSServiceLayer *sourceLayer = dimensionPair.second;

    const TSLWMTSServiceDimension *dimension = sourceLayer->getDimensionAt( dimensionPair.first );
    int numPossibleValues = dimension->numValues();
    values.reserve( numPossibleValues );
    for( int i = 0; i < numPossibleValues; ++i )
    {
      values.push_back( dimension->getValueAt(i) );
    }

    return true;
  }

  QModelIndex WMTSServiceDimensionsModel::findDimensionByName( const QString &dimensionName ) const
  {
    size_t numDimensions = m_dimensions.size();
    for( size_t i = 0; i < numDimensions; ++i )
    {
      const TSLWMTSServiceDimension *dimension = m_dimensions[i].second->getDimensionAt( m_dimensions[i].first );
      if( dimensionName.compare( QString::fromUtf8( dimension->identifier() ), Qt::CaseInsensitive ) == 0 )
      {
        // Return the value column rather than the row column
        return createIndex( i, 1 );
      }
    }

    return QModelIndex();
  }

  Qt::ItemFlags WMTSServiceDimensionsModel::flags(const QModelIndex &index) const
  {
    if( index.isValid() && index.column() == 1 )
    {
      // Make the value column editable
      return Service::ServiceDimensionsModel::flags( index ) | Qt::ItemIsEditable;
    }

    return Service::ServiceDimensionsModel::flags( index );
  }

  int WMTSServiceDimensionsModel::rowCount(const QModelIndex& /*parent*/) const
  {
    return m_dimensions.size();
  }

  int WMTSServiceDimensionsModel::columnCount ( const QModelIndex& /*parent*/ ) const
  {
    return m_serviceInfo ? 2 : 0;
  }

  QVariant WMTSServiceDimensionsModel::data(const QModelIndex &index, int role) const
  {
    if( !m_serviceInfo )
    {
      return QVariant();
    }

    if( !index.isValid() || index.row() >= m_dimensions.size() || (role != Qt::DisplayRole && role != Qt::EditRole) )
    {
      return QVariant();
    }

    const std::pair< int, const TSLWMTSServiceLayer* > &dimension = m_dimensions[index.row()];
    const TSLWMTSServiceLayer *sourceLayer = dimension.second;

    if( index.column() == 0 )
    {
      // The first column is the dimension's name
      return QVariant( QString::fromUtf8( sourceLayer->getDimensionAt( dimension.first )->identifier() ) );
    }
    else if( index.column() == 1 )
    {
      // The second column is the value
      const char *value = sourceLayer->getDimensionValue( sourceLayer->getDimensionAt( dimension.first )->identifier() );
      if( value )
      {
        return QVariant( QString::fromUtf8( value ) );
      }
    }

    return QVariant();
  }

  QVariant WMTSServiceDimensionsModel::headerData(int section, Qt::Orientation orientation, int role) const
  {
    if( role != Qt::DisplayRole || orientation != Qt::Horizontal )
    {
      return QVariant();
    }

    if( section == 0 )
    {
      return QVariant( tr("Dimension Name") );
    }
    else if( section == 1 )
    {
      return QVariant( tr("Current Value") );
    }

    return QVariant();
  }

  bool WMTSServiceDimensionsModel::setData(const QModelIndex &index, const QVariant &value, int role)
  {
    if( !index.isValid() || role != Qt::EditRole || index.column() != 1 )
    {
      // Only the 2nd column is editable, reject edits to anything else
      return false;
    }

    std::pair< int, TSLWMTSServiceLayer* > &dimension = m_dimensions[index.row()];
    const TSLWMTSServiceLayer *sourceLayer = dimension.second;
    if( dimension.second->setDimensionValue( sourceLayer->getDimensionAt( dimension.first )->identifier(),
          value.toString().toUtf8() ) )
    {
      emit dataChanged( index, index );
    }

    return false;
  }

  void WMTSServiceDimensionsModel::extractVisibleLayerDimensions( TSLWMTSServiceInfo *serviceInfo )
  {
    int numLayers = serviceInfo->numLayers();
    for( int i = 0; i < numLayers; ++i )
    {
      TSLWMTSServiceLayer *layer = serviceInfo->getLayerAt(i);
      if( layer->getVisibility() )
      {
        int numLayerDimensions = layer->numDimensions();
        for( int i = 0; i < numLayerDimensions; ++i )
        {
          m_dimensions.push_back( std::make_pair( i, layer ) );
        }
      }
    }
  }
};
