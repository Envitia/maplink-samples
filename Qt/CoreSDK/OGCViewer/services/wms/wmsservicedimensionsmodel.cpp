/****************************************************************************
  Copyright (c) 2017 by Envitia Group PLC.
 ****************************************************************************/

#include "wmsservicedimensionsmodel.h"
#include <QAbstractItemView>
#include <QComboBox>

#include "MapLink.h"
#include "tslplatformhelper.h"

namespace Services
{

  WMSServiceDimensionsModel::WMSServiceDimensionsModel( TSLWMSServiceLayer *rootLayer )
    : m_rootLayer( rootLayer )
  {
    // Build up a list of dimensions that apply to the visible layers and store
    // accessors to them so we don't have to traverse the layer tree each time
    if( rootLayer )
    {
      extractVisibleLayerDimensions( rootLayer );

      // Remove any duplicate entries
      std::unique( m_dimensions.begin(), m_dimensions.end() );
    }
  }

  WMSServiceDimensionsModel::~WMSServiceDimensionsModel()
  {
  }

  bool WMSServiceDimensionsModel::configurationValid() const
  {
    // All fields are considered to be complete if each dimension returns a value from getDimensionValue(),
    // whether this is the service-defined default value or a user-specified value
    size_t numDimensions = m_dimensions.size();
    for( size_t i = 0; i < numDimensions; ++i )
    {
      if( !m_dimensions[i].second->getDimensionValue( m_dimensions[i].first ) )
      {
        return false;
      }
    }

    return true;
  }

  void WMSServiceDimensionsModel::setDelegates( QAbstractItemView *view )
  {
    size_t numDimensions = m_dimensions.size();
    for( size_t i = 0; i < numDimensions; ++i )
    {
      const TSLWMSServiceLayerDimension *dimension = m_dimensions[i].second->getDimensionAt( m_dimensions[i].first );
      if( dimension->units() && TSLPlatformHelper::stricmp( dimension->units(), "ISO8601" ) == 0 )
      {
        // Use the date editor for modifying dates in IS8601 format
        view->setItemDelegateForRow( i, &m_dateDelegate );
      }
      else if( dimension->noOfPossibleValues() > 0 )
      {
        // Otherwise, if the service provides some fixed values, use a combobox instead
        view->setItemDelegateForRow( i, &m_prescribedValueDelegate );
      }
    }
  }

  bool WMSServiceDimensionsModel::getPossibleValues( const QModelIndex &index, std::vector< const char* > &values ) const
  {
    if( !index.isValid() )
    {
      return false;
    }

    const std::pair< int, TSLWMSServiceLayer* > &dimensionPair = m_dimensions[index.row()];
    TSLWMSServiceLayer *sourceLayer = dimensionPair.second;

    const TSLWMSServiceLayerDimension *dimension = sourceLayer->getDimensionAt( dimensionPair.first );
    int numPossibleValues = dimension->noOfPossibleValues();
    values.reserve( numPossibleValues );
    for( int i = 0; i < numPossibleValues; ++i )
    {
      values.push_back( dimension->getPossibleValue(i) );
    }

    return !dimension->usesNearestValue();
  }

  QModelIndex WMSServiceDimensionsModel::findDimensionByName( const QString &dimensionName ) const
  {
    size_t numDimensions = m_dimensions.size();
    for( size_t i = 0; i < numDimensions; ++i )
    {
      const TSLWMSServiceLayerDimension *dimension = m_dimensions[i].second->getDimensionAt( m_dimensions[i].first );
      if( dimensionName.compare( QString::fromUtf8( dimension->name() ), Qt::CaseInsensitive ) == 0 )
      {
        // Return the value column rather than the row column
        return createIndex( i, 1 );
      }
    }

    return QModelIndex();
  }

  Qt::ItemFlags WMSServiceDimensionsModel::flags(const QModelIndex &index) const
  {
    if( index.isValid() && index.column() == 1 )
    {
      // Make the value column editable
      return Service::ServiceDimensionsModel::flags( index ) | Qt::ItemIsEditable;
    }

    return Service::ServiceDimensionsModel::flags( index );
  }

  int WMSServiceDimensionsModel::rowCount(const QModelIndex& /*parent*/) const
  {
    return m_dimensions.size();
  }

  int WMSServiceDimensionsModel::columnCount ( const QModelIndex& /*parent*/ ) const
  {
    return m_rootLayer ? 2 : 0;
  }

  QVariant WMSServiceDimensionsModel::data(const QModelIndex &index, int role) const
  {
    if( !m_rootLayer )
    {
      return QVariant();
    }

    if( !index.isValid() || index.row() >= m_dimensions.size() || (role != Qt::DisplayRole && role != Qt::EditRole) )
    {
      return QVariant();
    }

    const std::pair< int, TSLWMSServiceLayer* > &dimension = m_dimensions[index.row()];
    const TSLWMSServiceLayer *sourceLayer = dimension.second;

    if( index.column() == 0 )
    {
      // The first column is the dimension's name
      return QVariant( QString::fromUtf8( sourceLayer->getDimensionAt( dimension.first )->name() ) );
    }
    else if( index.column() == 1 )
    {
      // The second column is the value
      const char *value = sourceLayer->getDimensionValue( dimension.first );
      if( value )
      {
        return QVariant( QString::fromUtf8( value ) );
      }
    }

    return QVariant();
  }

  QVariant WMSServiceDimensionsModel::headerData(int section, Qt::Orientation orientation, int role) const
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

  bool WMSServiceDimensionsModel::setData(const QModelIndex &index, const QVariant &value, int role)
  {
    if( !index.isValid() || role != Qt::EditRole || index.column() != 1 )
    {
      // Only the 2nd column is editable, reject edits to anything else
      return false;
    }

    std::pair< int, TSLWMSServiceLayer* > &dimension = m_dimensions[index.row()];
    if( dimension.second->setDimensionValue( dimension.first, value.toString().toUtf8() ) )
    {
      emit dataChanged( index, index );
    }

    return false;
  }

  void WMSServiceDimensionsModel::extractVisibleLayerDimensions( TSLWMSServiceLayer *layer )
  {
    bool derivedVisibility = false;
    bool layerIsVisible = layer->getVisibility( &derivedVisibility );
    if( layerIsVisible && !derivedVisibility )
    {
      int numLayerDimensions = layer->noOfDimensions();
      for( int i = 0; i < numLayerDimensions; ++i )
      {
        m_dimensions.push_back( std::make_pair( i, layer ) );
      }
    }

    int numChildLayers = layer->noOfSubLayers();
    for( int i = 0; i < numChildLayers; ++i )
    {
      extractVisibleLayerDimensions( layer->getSubLayerAt( i ) );
    }
  }

};
