/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef WMTSSERVICELAYERMODEL_H
#define WMTSSERVICELAYERMODEL_H

#include "services/service.h"
#include <map>
#include <vector>
#include <set>

class TSLWMTSServiceLayer;
class TSLWMTSServiceInfo;

namespace Services
{
  class WMTSService;

  // Implementation of a Qt model that displays a tree of all the layers available
  // on a WMTS service.

  class WMTSServiceLayerModel : public Service::ServiceLayerModel
  {
    Q_OBJECT
    public:
      class WMTSLayerNodeInfo
      {
        public:
          WMTSLayerNodeInfo( TSLWMTSServiceLayer *layer, WMTSLayerNodeInfo *parent, int index );
          ~WMTSLayerNodeInfo();

          TSLWMTSServiceLayer *m_layer;
          WMTSLayerNodeInfo *m_parent;
          int m_layerIndex; // Index of layer in its parent
          Qt::CheckState m_checked;
          bool m_supportedCRS; // true if any coordinate systems in this layer are supported
          bool m_compatibleWithSelectedLayers; // true if this layer has a CRS compatible with layers already selected

          std::vector< WMTSLayerNodeInfo* > m_children;
      };

      WMTSServiceLayerModel( WMTSService *service, TSLWMTSServiceInfo *serviceInfo );
      virtual ~WMTSServiceLayerModel();

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

      //signals:
      //void signalUpdateService( WMTSService *layer, const TSLWMTSServiceInfo *serviceInfo );

      //private slots:
      void updateService( WMTSService *layer, TSLWMTSServiceInfo *serviceInfo );

    private:
      bool visibleLayerHasDimensions( WMTSLayerNodeInfo *node ) const;
      bool anyChildrenVisible( WMTSLayerNodeInfo *node ) const;
      void updateLayerCRSCompatibility( WMTSLayerNodeInfo *node );

      WMTSLayerNodeInfo *m_rootLayerNode;
      WMTSService *m_service;

      std::set< const TSLWMTSServiceLayer* > m_visibleLayers;
  };
};
#endif
