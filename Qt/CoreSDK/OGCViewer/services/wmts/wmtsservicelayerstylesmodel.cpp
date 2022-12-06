/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "wmtsservicelayerstylesmodel.h"
#include "wmtsservicelayermodel.h"
#include <sstream>

#include "MapLink.h"
#include "MapLinkWMTSDataLayer.h"

namespace Services
{
  WMTSServiceLayerStylesModel::WMTSServiceLayerStylesModel()
    : m_layer( NULL )
  {
  }

  WMTSServiceLayerStylesModel::~WMTSServiceLayerStylesModel()
  {
  }

  int WMTSServiceLayerStylesModel::rowCount(const QModelIndex& /*parent*/) const
  {
    return m_layer ? m_layer->numStyles() : 0;
  }

  int WMTSServiceLayerStylesModel::columnCount ( const QModelIndex& /*parent*/ ) const
  {
    return m_layer ? 1 : 0;
  }

  QVariant WMTSServiceLayerStylesModel::data(const QModelIndex &index, int role) const
  {
    if( !m_layer )
    {
      return QVariant();
    }

    if( !index.isValid() || index.row() >= m_layer->numStyles() || 
        (role != Qt::DisplayRole && role != Qt::EditRole) )
    {
      return QVariant();
    }

    const TSLWMTSServiceStyle *style = m_layer->getStyleAt( index.row() );
    return QVariant( QString::fromUtf8( style->identifier() ) );
  }

  QVariant WMTSServiceLayerStylesModel::headerData(int /*section*/, Qt::Orientation /*orientation*/, int /*role*/) const
  {
    return QVariant();
  }

  void WMTSServiceLayerStylesModel::setSelectedLayer( const QModelIndex &layer )
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

  const char* WMTSServiceLayerStylesModel::currentStyle() const
  {
    if( !m_layer )
    {
      return NULL;
    }

    return m_layer->getStyleValue();
  }

  void WMTSServiceLayerStylesModel::setStyle( const char *name )
  {
    if( !m_layer )
    {
      return;
    }

    m_layer->setStyleValue( name );
  }
};
