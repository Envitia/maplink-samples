/****************************************************************************
  Copyright (c) 2017 by Envitia Group PLC.
 ****************************************************************************/

#ifndef WMSSERVICELAYERMODEL_H
#define WMSSERVICELAYERMODEL_H

#include "services/service.h"
#include <map>
#include <vector>
#include <set>

class TSLWMSServiceLayer;

namespace Services
{
  class WMSService;

  // Implementation of a Qt model that displays a tree of all the layers available
  // on a WMS service.

  class WMSServiceLayerModel : public Service::ServiceLayerModel
  {
    Q_OBJECT
    public:
      // Internal utility class that provides state information about how to present
      // a specific layer from the WMS in the UI.
      class WMSLayerNodeInfo
      {
        public:
          WMSLayerNodeInfo( TSLWMSServiceLayer *layer, WMSLayerNodeInfo *parent, int index );
          ~WMSLayerNodeInfo();

          TSLWMSServiceLayer *m_layer;
          WMSLayerNodeInfo *m_parent;
          int m_layerIndex; // Index of layer in its parent
          Qt::CheckState m_checked;
          bool m_supportedCRS; // true if any coordinate systems in this layer are supported
          bool m_compatibleWithSelectedLayers; // true if this layer has a CRS compatible with layers already selected

          std::vector< WMSLayerNodeInfo* > m_children;
      };

      WMSServiceLayerModel( WMSService *service, TSLWMSServiceLayer *rootLayer );
      virtual ~WMSServiceLayerModel();

      virtual Qt::ItemFlags flags( const QModelIndex &index ) const;
      virtual QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;
      virtual bool setData ( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole );
      virtual QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
      virtual int rowCount( const QModelIndex &parent = QModelIndex() ) const;
      virtual int columnCount( const QModelIndex &parent = QModelIndex() ) const;

      virtual QModelIndex index( int row, int column, const QModelIndex &parent = QModelIndex() ) const;
      virtual QModelIndex parent ( const QModelIndex &index ) const;

      virtual bool configurationValid() const;
      virtual bool selectionsHaveDimensions() const;

      void updateRootLayer( WMSService *layer, TSLWMSServiceLayer *rootLayer );

    private:
      void updateChildVisibility( WMSLayerNodeInfo *node, bool visible );
      void updateParentVisibility( WMSLayerNodeInfo *node );
      bool visibleLayerHasDimensions( WMSLayerNodeInfo *node ) const;
      bool anyChildrenVisible( WMSLayerNodeInfo *node ) const;
      void updateLayerCRSCompatibility( WMSLayerNodeInfo *node );

      WMSLayerNodeInfo *m_rootLayerNode;
      WMSService *m_service;

      std::set< TSLWMSServiceLayer* > m_visibleLayers;
  };
};
#endif
