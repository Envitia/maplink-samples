/****************************************************************************
  Copyright (c) 2017 by Envitia Group PLC.
 ****************************************************************************/

#include "wmtsservicedimensionsmodel.h"
#include "wmtsservicedimensioninfomodel.h"

#include "MapLink.h"
#include "MapLinkWMTSDataLayer.h"
#include <sstream>

namespace Services
{

  static const QVariant g_headerNames[] = { QVariant( "Units of measure" ),
    QVariant( "Unit symbol" ) };
  static const size_t g_numRows = sizeof( g_headerNames ) / sizeof( QVariant );

  WMTSServiceDimensionInfoModel::WMTSServiceDimensionInfoModel()
    : m_selectedDimension( 0 )
      , m_dimensionsModel( NULL )
  {
  }

  WMTSServiceDimensionInfoModel::~WMTSServiceDimensionInfoModel()
  {
  }

  void WMTSServiceDimensionInfoModel::setSelectedDimension( const QModelIndex &dimension )
  {
    // Tell the model and any attached views that the data has changed and needs reloading
    beginResetModel();
    if( dimension.isValid() )
    {
      m_dimensionsModel = reinterpret_cast< const WMTSServiceDimensionsModel* >( dimension.model() );
      m_selectedDimension = dimension.row();
    }
    else
    {
      m_dimensionsModel = NULL;
      m_selectedDimension = -1;
    }
    endResetModel();
  }

  int WMTSServiceDimensionInfoModel::rowCount(const QModelIndex& /*parent*/) const
  {
    return m_dimensionsModel && m_selectedDimension >= 0 ? g_numRows : 0;
  }

  int WMTSServiceDimensionInfoModel::columnCount ( const QModelIndex& /*parent*/ ) const
  {
    return 2;
  }

  QVariant WMTSServiceDimensionInfoModel::data(const QModelIndex &index, int role) const
  {
    if( !m_dimensionsModel || !index.isValid() || role != Qt::DisplayRole || index.row() >= g_numRows )
    {
      return QVariant();
    }

    if( index.column() == 0 )
    {
      // The first column is the field title
      return g_headerNames[index.row()];
    }
    else if( index.column() == 1 )
    {
      const std::pair< int, const TSLWMTSServiceLayer* > &dimensionInfo = m_dimensionsModel->getDimensionInfo( m_selectedDimension );

      // The second column is the value
      const TSLWMTSServiceLayer *sourceLayer = dimensionInfo.second;
      const TSLWMTSServiceDimension *dimension = sourceLayer->getDimensionAt( dimensionInfo.first );

      switch( index.row() )
      {
        case 0:
          return dimension->uom() ? QVariant( dimension->uom() ) : QVariant();

        case 1:
          return dimension->unitSymbol() ? QVariant( dimension->unitSymbol() ) : QVariant();

        default:
          break;
      }
    }

    return QVariant();
  }

  QVariant WMTSServiceDimensionInfoModel::headerData(int /*section*/, Qt::Orientation /*orientation*/, int /*role*/) const
  {
    return QVariant();
  }
};
