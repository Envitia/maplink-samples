/****************************************************************************
  Copyright (c) 2017 by Envitia Group PLC.
 ****************************************************************************/

#include "wmsservicelayerstylesmodel.h"
#include "wmsservicelayermodel.h"
#include <sstream>

#include "MapLink.h"

namespace Services
{
  WMSServiceLayerStylesModel::WMSServiceLayerStylesModel()
    : m_layer( NULL )
  {
  }

  WMSServiceLayerStylesModel::~WMSServiceLayerStylesModel()
  {
  }

  int WMSServiceLayerStylesModel::rowCount(const QModelIndex& /*parent*/) const
  {
    return m_layer ? m_layer->noOfStyles() : 0;
  }

  int WMSServiceLayerStylesModel::columnCount ( const QModelIndex& /*parent*/ ) const
  {
    return m_layer ? 1 : 0;
  }

  QVariant WMSServiceLayerStylesModel::data(const QModelIndex &index, int role) const
  {
    if( !m_layer )
    {
      return QVariant();
    }

    if( !index.isValid() || index.row() >= m_layer->noOfStyles() || 
        (role != Qt::DisplayRole && role != Qt::EditRole) )
    {
      return QVariant();
    }

    return QVariant( QString::fromUtf8( m_layer->getStyleAt( index.row() ) ) );
  }

  QVariant WMSServiceLayerStylesModel::headerData(int /*section*/, Qt::Orientation /*orientation*/, int /*role*/) const
  {
    return QVariant();
  }

  void WMSServiceLayerStylesModel::setSelectedLayer( const QModelIndex &layer )
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

  const char* WMSServiceLayerStylesModel::currentStyle() const
  {
    if( !m_layer )
    {
      return NULL;
    }

    return m_layer->getStyleValue();
  }

  void WMSServiceLayerStylesModel::setStyle( const char *name )
  {
    if( !m_layer )
    {
      return;
    }

    m_layer->setStyleValue( name );
  }
};
