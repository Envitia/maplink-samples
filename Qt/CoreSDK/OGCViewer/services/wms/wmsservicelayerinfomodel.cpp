/****************************************************************************
  Copyright (c) 2017 by Envitia Group PLC.
 ****************************************************************************/

#include "wmsservicelayerinfomodel.h"
#include "wmsservicelayermodel.h"
#include <sstream>

#include "MapLink.h"

namespace Services
{

  static const QVariant g_rowTitles[] = { QVariant( "Abstract" ),
    QVariant( "Identifiers" ),
    QVariant( "Keywords" ),
    QVariant( "Name" ),
    QVariant( "Title" ) };

  static const size_t g_numRows = sizeof( g_rowTitles ) / sizeof( QVariant );

  WMSServiceLayerInfoModel::WMSServiceLayerInfoModel()
    : m_layer( NULL )
  {
  }

  WMSServiceLayerInfoModel::~WMSServiceLayerInfoModel()
  {
  }

  int WMSServiceLayerInfoModel::rowCount(const QModelIndex& /*parent*/) const
  {
    return m_layer ? g_numRows : 0;
  }

  int WMSServiceLayerInfoModel::columnCount ( const QModelIndex& /*parent*/ ) const
  {
    return m_layer ? 2 : 0;
  }

  QVariant WMSServiceLayerInfoModel::data(const QModelIndex &index, int role) const
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
        return QVariant( m_layer->abstract() );

      case 1:
        {
          // Extract all the identifiers from the layer and return them as a comma seperated string
          std::ostringstream identifiersStream;
          int numIdentifiers = m_layer->noOfIdentifiers();
          for( int i = 0; i < numIdentifiers; ++i )
          {
            const char *identifier = NULL;
            m_layer->getIdentifierAt(i, &identifier);
            identifiersStream << identifier;

            if( i < numIdentifiers-1 )
            {
              identifiersStream << ", ";
            }
          }

          return QVariant( QString::fromUtf8( identifiersStream.str().c_str() ) );
        }

      case 2:
        {
          // Extract all the keywords from the layer and return them as a comma seperated string
          std::ostringstream keywordsStream;
          int numKeywords = m_layer->noOfKeywords();
          for( int i = 0; i < numKeywords; ++i )
          {
            keywordsStream << m_layer->getKeywordAt( i );

            if( i < numKeywords-1 )
            {
              keywordsStream << ", ";
            }
          }

          return QVariant( QString::fromUtf8( keywordsStream.str().c_str() ) );
        }

      case 3:
        return QVariant( QString::fromUtf8( m_layer->name() ) );

      case 4:
        return QVariant( QString::fromUtf8( m_layer->title() ) );

      default:
        return QVariant();
    }
  }

  QVariant WMSServiceLayerInfoModel::headerData(int /*section*/, Qt::Orientation /*orientation*/, int /*role*/) const
  {
    return QVariant();
  }

  void WMSServiceLayerInfoModel::setSelectedLayer( const QModelIndex &layer )
  {
    // Tell the model and any attached views that the data has changed and needs reloading
    if( layer.isValid() )
    {
      beginResetModel();

      WMSServiceLayerModel::WMSLayerNodeInfo *layerNode = reinterpret_cast< WMSServiceLayerModel::WMSLayerNodeInfo* >( layer.internalPointer() );
      m_layer = layerNode->m_layer;

      endResetModel();
    }
  }
};
