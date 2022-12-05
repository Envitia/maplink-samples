/****************************************************************************
  Copyright (c) 2017 by Envitia Group PLC.
 ****************************************************************************/

#include "wmsservicedimensionsmodel.h"
#include "wmsservicedimensioninfomodel.h"

#include "MapLink.h"
#include <sstream>

namespace Services
{

  static const QVariant g_headerNames[] = { QVariant( "Units" ),
    QVariant( "Unit symbol" ),
    QVariant( "Service uses nearest available value" ),
    QVariant( "Valid ranges" )  };
  static const size_t g_numRows = sizeof( g_headerNames ) / sizeof( QVariant );

  WMSServiceDimensionInfoModel::WMSServiceDimensionInfoModel()
    : m_selectedDimension( 0 )
      , m_dimensionsModel( NULL )
  {
  }

  WMSServiceDimensionInfoModel::~WMSServiceDimensionInfoModel()
  {
  }

  void WMSServiceDimensionInfoModel::setSelectedDimension( const QModelIndex &dimension )
  {
    // Tell the model and any attached views that the data has changed and needs reloading
    beginResetModel();
    if( dimension.isValid() )
    {
      m_dimensionsModel = reinterpret_cast< const WMSServiceDimensionsModel* >( dimension.model() );
      m_selectedDimension = dimension.row();
    }
    else
    {
      m_dimensionsModel = NULL;
      m_selectedDimension = -1;
    }
    endResetModel();
  }

  int WMSServiceDimensionInfoModel::rowCount(const QModelIndex& /*parent*/) const
  {
    return m_dimensionsModel && m_selectedDimension >= 0 ? g_numRows : 0;
  }

  int WMSServiceDimensionInfoModel::columnCount ( const QModelIndex& /*parent*/ ) const
  {
    return 2;
  }

  QVariant WMSServiceDimensionInfoModel::data(const QModelIndex &index, int role) const
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
      const std::pair< int, TSLWMSServiceLayer* > &dimensionInfo = m_dimensionsModel->getDimensionInfo( m_selectedDimension );

      // The second column is the value
      const TSLWMSServiceLayer *sourceLayer = dimensionInfo.second;
      const TSLWMSServiceLayerDimension *dimension = sourceLayer->getDimensionAt( dimensionInfo.first );

      switch( index.row() )
      {
        case 0:
          return dimension->units() ? QVariant( dimension->units() ) : QVariant();

        case 1:
          return dimension->unitSymbol() ? QVariant( dimension->unitSymbol() ) : QVariant();

        case 2:
          return dimension->usesNearestValue() ? QVariant( tr( "Yes" ) ) : QVariant( tr( "No" ) );

        case 3:
          {
            int numRanges = dimension->noOfRanges();
            if( numRanges == 0 )
            {
              return QVariant( tr( "The service does not specify any valid ranges for this dimension" ) );
            }

            std::ostringstream stream;
            for( int i = 0; i < numRanges; ++i )
            {
              const char *minimum = NULL, *maximum = NULL, *resolution = NULL;
              dimension->getRange( i, &minimum, &maximum, &resolution );
              if( minimum && maximum && resolution )
              {
                stream <<  minimum << "-" << maximum << " at " << resolution << " intervals" << std::endl;
              }
            }

            return QVariant( QString::fromUtf8( stream.str().c_str() ) );
          }

        default:
          break;
      }
    }

    return QVariant();
  }

  QVariant WMSServiceDimensionInfoModel::headerData(int /*section*/, Qt::Orientation /*orientation*/, int /*role*/) const
  {
    return QVariant();
  }

};
