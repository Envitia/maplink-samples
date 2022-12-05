/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "wmtsservicelayerinfomodel.h"
#include "wmtsservicelayermodel.h"
#include <sstream>

#include "MapLink.h"
#include "MapLinkWMTSDataLayer.h"

namespace Services
{

  static const QVariant g_rowTitles[] = { QVariant( "Abstract" ),
    QVariant( "Identifier" ),
    QVariant( "Titles" ) };

  static const size_t g_numRows = sizeof( g_rowTitles ) / sizeof( QVariant );

  WMTSServiceLayerInfoModel::WMTSServiceLayerInfoModel()
    : m_layer( NULL )
  {
  }

  WMTSServiceLayerInfoModel::~WMTSServiceLayerInfoModel()
  {
  }

  int WMTSServiceLayerInfoModel::rowCount(const QModelIndex& /*parent*/) const
  {
    return m_layer ? g_numRows : 0;
  }

  int WMTSServiceLayerInfoModel::columnCount ( const QModelIndex& /*parent*/ ) const
  {
    return m_layer ? 2 : 0;
  }

  QVariant WMTSServiceLayerInfoModel::data(const QModelIndex &index, int role) const
  {
    if( !m_layer )
    {
      return QVariant();
    }

    if( !index.isValid() || index.row() >= g_numRows || role != Qt::DisplayRole )
    {
      return QVariant();
    }

    if( index.column() == 0 )
    {
      return g_rowTitles[index.row()];
    }

    // Return the data for the requested row. The index here equates to the matching field
    // for g_rowTitles[index.row()]
    switch( index.row() )
    {
      case 0:
        {
          // Extract all the abstracts from the layer and return them as a comma seperated string
          std::ostringstream abstractsStream;
          int numAbstracts = m_layer->numAbstracts();
          for( int i = 0; i < numAbstracts; ++i )
          {
            const char *abstract = m_layer->getAbstractAt(i);
            abstractsStream << abstract;

            if( i < numAbstracts-1 )
            {
              abstractsStream << ", ";
            }
          }

          return QVariant( QString::fromUtf8( abstractsStream.str().c_str() ) );
        }

      case 1:
        {
          if( m_layer->identifier() )
          {
            return QVariant( QString::fromUtf8( m_layer->identifier() ) );
          }

          return QVariant();
        }

      case 2:
        {
          // Extract all the titles from the layer and return them as a comma seperated string
          std::ostringstream titleStream;
          int numTitles = m_layer->numTitles();
          for( int i = 0; i < numTitles; ++i )
          {
            titleStream << m_layer->getTitleAt(i);

            if( i < numTitles-1 )
            {
              titleStream << ", ";
            }
          }

          return QVariant( QString::fromUtf8( titleStream.str().c_str() ) );
        }

      default:
        return QVariant();
    }
  }

  QVariant WMTSServiceLayerInfoModel::headerData(int /*section*/, Qt::Orientation /*orientation*/, int /*role*/) const
  {
    return QVariant();
  }

  void WMTSServiceLayerInfoModel::setSelectedLayer( const QModelIndex &layer )
  {
    // Tell the model and any attached views that the data has changed and needs reloading
    if( layer.isValid() )
    {
      beginResetModel();

      WMTSServiceLayerModel::WMTSLayerNodeInfo *layerNode = reinterpret_cast< WMTSServiceLayerModel::WMTSLayerNodeInfo* >( layer.internalPointer() );
      m_layer = layerNode->m_layer;

      endResetModel();
    }
  }
};
