/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "wmtsservicelayermodel.h"
#include "wmtsservice.h"
#include "MapLink.h"

namespace Services
{
  WMTSServiceLayerModel::WMTSLayerNodeInfo::WMTSLayerNodeInfo( TSLWMTSServiceLayer *layer, WMTSLayerNodeInfo *parent, int index )
    : m_layer( layer )
      , m_parent( parent )
      , m_layerIndex( index )
      , m_checked( Qt::Unchecked )
      , m_supportedCRS( false )
      , m_compatibleWithSelectedLayers( true )
  {
    if( layer )
    {
      int numMatrixSets = layer->numTileMatrixSets();
      for( int i = 0; i < numMatrixSets; ++i )
      {
        const TSLWMTSServiceTileMatrixSet *matrixSet = layer->getTileMatrixSetAt( i );
        if( TSLWMTSDataLayer::crsIsSupported( matrixSet->supportedCRS() ) )
        {
          m_supportedCRS = true;
          break;
        }
      }
    }
  }

  WMTSServiceLayerModel::WMTSLayerNodeInfo::~WMTSLayerNodeInfo()
  {
    size_t numChildren = m_children.size();
    for( size_t i = 0; i < numChildren; ++i )
    {
      delete m_children[i];
    }
  }

  WMTSServiceLayerModel::WMTSServiceLayerModel( WMTSService *service, TSLWMTSServiceInfo *serviceInfo )
    : ServiceLayerModel()
      , m_rootLayerNode( NULL )
      , m_service( service )
  {
    updateService( service, serviceInfo );
  }

  WMTSServiceLayerModel::~WMTSServiceLayerModel()
  {
    delete m_rootLayerNode;
  }

  Qt::ItemFlags WMTSServiceLayerModel::flags( const QModelIndex &index ) const
  {
    if( !index.isValid() )
    {
      return 0;
    }

    WMTSLayerNodeInfo *node = reinterpret_cast< WMTSLayerNodeInfo* >( index.internalPointer() );
    if( node->m_supportedCRS && node->m_compatibleWithSelectedLayers )
    {
      return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
    }
    else
    {
      // Prevent layers with only unsupported coordinate systems from being selected for viewing
      return Qt::ItemIsSelectable;
    }
  }

  QVariant WMTSServiceLayerModel::data( const QModelIndex &index, int role ) const
  {
    if( !m_rootLayerNode || !index.isValid() )
    {
      return QVariant();
    }

    WMTSLayerNodeInfo *node = reinterpret_cast< WMTSLayerNodeInfo* >( index.internalPointer() );

    if( !node->m_layer )
    {
      return QVariant();
    }

    switch( role )
    {
      case Qt::DisplayRole:
        {
          if( node->m_layer->identifier() )
          {
            return QVariant( QString::fromUtf8( node->m_layer->identifier() ) );
          }
          else
          {
            return QVariant( tr( "<Unnamed>" ) );
          }
        }

      case Qt::CheckStateRole:
        {
          return QVariant( node->m_checked );
        }

      case Qt::ToolTipRole:
        {
          if( !node->m_supportedCRS )
          {
            return QVariant( QString( tr( "This layer does not have any supported coordinate systems and so cannot be selected for display" ) ) );
          }
          else if( !node->m_compatibleWithSelectedLayers )
          {
            return QVariant( QString( tr( "This layer is not compatible with the other layers currently selected for display" ) ) );
          }

          return QVariant();
        }

      default:
        return QVariant();
    }
  }

  bool WMTSServiceLayerModel::setData ( const QModelIndex &index, const QVariant &value, int role )
  {
    if( !index.isValid() )
    {
      return false;
    }

    WMTSLayerNodeInfo *node = reinterpret_cast< WMTSLayerNodeInfo* >( index.internalPointer() );

    if( !node->m_layer )
    {
      return false;
    }

    switch( role )
    {
      case Qt::CheckStateRole:
        {
          node->m_checked = (Qt::CheckState)value.toInt();

          bool originalVisibility = node->m_layer->getVisibility();
          node->m_layer->setVisibility( value == Qt::Checked );

          bool newVisibility = node->m_layer->getVisibility();

          if( originalVisibility != newVisibility )
          {
            m_service->layerVisiblityChanged( node->m_layer );

            if( newVisibility )
            {
              m_visibleLayers.insert( node->m_layer );
            }
            else
            {
              m_visibleLayers.erase( node->m_layer );
            }

            emit dataChanged( index, index );
          }

          // If we have just enabled a new layer, update the enabled state of each layer based on whether it can
          // be enabled along with the layers already selected
          if( value == Qt::Checked )
          {
            updateLayerCRSCompatibility( m_rootLayerNode );
          }
          return true;
        }

      default:
        return false;
    }
  }

  QVariant WMTSServiceLayerModel::headerData( int /*section*/, Qt::Orientation /*orientation*/, int role ) const
  {
    if( role != Qt::DisplayRole )
    {
      return QVariant();
    }

    return QVariant( tr("Layer name") );
  }

  int WMTSServiceLayerModel::rowCount( const QModelIndex &parent ) const
  {
    if( !m_rootLayerNode || parent.column() > 0 )
    {
      return 0;
    }

    WMTSLayerNodeInfo *node = parent.isValid() ? reinterpret_cast< WMTSLayerNodeInfo* >( parent.internalPointer() ) : m_rootLayerNode;
    return node->m_children.size();
  }

  int WMTSServiceLayerModel::columnCount( const QModelIndex& /*parent*/ ) const
  {
    return 1;
  }

  QModelIndex WMTSServiceLayerModel::index( int row, int column, const QModelIndex &parent ) const
  {
    if( !m_rootLayerNode || !hasIndex( row, column, parent ) )
    {
      return QModelIndex();
    }

    WMTSLayerNodeInfo *node = parent.isValid() ? reinterpret_cast< WMTSLayerNodeInfo* >( parent.internalPointer() ) : m_rootLayerNode;
    if( node->m_children.size() <= row )
    {
      // Invalid row requested
      return QModelIndex();
    }

    return createIndex( row, column, node->m_children[row] );
  }

  QModelIndex WMTSServiceLayerModel::parent ( const QModelIndex &index ) const
  {
    if( !index.isValid() || !m_rootLayerNode )
    {
      return QModelIndex();
    }

    WMTSLayerNodeInfo *thisNode = reinterpret_cast< WMTSLayerNodeInfo* >( index.internalPointer() );
    if( !thisNode->m_parent || thisNode->m_parent == m_rootLayerNode )
    {
      // Do not return a model index for the root node, see the Qt documentation on QAbstractItemModel
      return QModelIndex();
    }

    return createIndex( thisNode->m_parent->m_layerIndex, 0, thisNode->m_parent );
  }

  void WMTSServiceLayerModel::updateService( WMTSService *service, TSLWMTSServiceInfo *serviceInfo )
  {
    // Tell the model and any attached views that the data has changed and needs reloading
    beginResetModel();

    delete m_rootLayerNode;
    m_service = service;

    // Create an empty root node to act as the top level container for the tree. This is so that
    // we get the root service layer shown in the treeview when visualising the model
    if( serviceInfo )
    {
      m_rootLayerNode = new WMTSLayerNodeInfo( NULL, NULL, 0 );

      int numLayers = serviceInfo->numLayers();
      for( int i = 0; i < numLayers; ++i )
      {
        m_rootLayerNode->m_children.push_back( new WMTSLayerNodeInfo( serviceInfo->getLayerAt(i), m_rootLayerNode, 0 ) );
      }
    }

    endResetModel();
  }

  bool WMTSServiceLayerModel::visibleLayerHasDimensions( WMTSLayerNodeInfo *node ) const
  {
    // Dimensions apply to any child layers as well as this layer,
    // so if any children are visibile then this counts for using any dimensions
    // defined on this layer
    bool layerHasDimensions = false;
    if( node->m_layer )
    {
      layerHasDimensions = node->m_layer->numDimensions() > 0;

      if( node->m_layer->getVisibility() && layerHasDimensions )
      {
        return true;
      }
    }

    size_t numChildren = node->m_children.size();

    if( layerHasDimensions )
    {
      for( size_t i = 0; i < numChildren; ++i )
      {
        if( anyChildrenVisible( node->m_children[i] ) )
        {
          return true;
        }
      }
    }

    for( size_t i = 0; i < numChildren; ++i )
    {
      if( visibleLayerHasDimensions( node->m_children[i] ) )
      {
        return true;
      }
    }

    return false;
  }

  bool WMTSServiceLayerModel::anyChildrenVisible( WMTSLayerNodeInfo *node ) const
  {
    if( node->m_layer && node->m_layer->getVisibility() )
    {
      return true;
    }

    size_t numChildren = node->m_children.size();
    for( size_t i = 0; i < numChildren; ++i )
    {
      if( anyChildrenVisible( node->m_children[i] ) )
      {
        return true;
      }
    }

    return false;
  }

  bool WMTSServiceLayerModel::configurationValid() const
  {
    if( !m_rootLayerNode )
    {
      return false;
    }

    // We are considered to be valid if at least one layer is visible
    return !m_visibleLayers.empty();
  }

  bool WMTSServiceLayerModel::selectionsHaveDimensions() const
  {
    if( !m_rootLayerNode )
    {
      return false;
    }

    return visibleLayerHasDimensions( m_rootLayerNode );
  }

  void WMTSServiceLayerModel::updateLayerCRSCompatibility( WMTSLayerNodeInfo *node )
  {
    // We only need to do this for layers that are not already visible,
    // layers that are already visible are known to be compatible
    if( node->m_layer && node->m_supportedCRS && !node->m_layer->getVisibility() )
    {
      bool wasCompatible = node->m_compatibleWithSelectedLayers;

      std::vector< const TSLWMTSServiceLayer* > visibleLayers( m_visibleLayers.begin(), m_visibleLayers.end() );
      visibleLayers.push_back( node->m_layer );

      node->m_compatibleWithSelectedLayers = TSLWMTSServiceLayer::layersHaveCompatibleCRS( &visibleLayers[0], visibleLayers.size() );

      if( node->m_compatibleWithSelectedLayers != wasCompatible )
      {
        // Only mark the node as changed if the state has actually changed
        QModelIndex nodeIndex( createIndex( node->m_layerIndex, 0, node ) );
        emit dataChanged( nodeIndex, nodeIndex );
      }
    }

    size_t numChildren = node->m_children.size();
    for( size_t i = 0; i < numChildren; ++i )
    {
      updateLayerCRSCompatibility( node->m_children[i] );
    }
  }
};
