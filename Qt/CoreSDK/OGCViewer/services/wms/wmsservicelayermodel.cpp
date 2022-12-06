/****************************************************************************
  Copyright (c) 2017 by Envitia Group PLC.
 ****************************************************************************/

#include "wmsservicelayermodel.h"
#include "wmsservice.h"
#include "MapLink.h"

namespace Services
{
  WMSServiceLayerModel::WMSLayerNodeInfo::WMSLayerNodeInfo( TSLWMSServiceLayer *layer, WMSLayerNodeInfo *parent, int index )
    : m_layer( layer )
      , m_parent( parent )
      , m_layerIndex( index )
      , m_checked( Qt::Unchecked )
      , m_supportedCRS( false )
      , m_compatibleWithSelectedLayers( true )
  {
    if( layer )
    {
      int numCRSs = layer->noOfCRSs();
      for( int i = 0; i < numCRSs; ++i )
      {
        if( TSLWMSDataLayer::crsIsSupported( layer->getCRSAt(i) ) )
        {
          m_supportedCRS = true;
          break;
        }
      }

      int numChildren = layer->noOfSubLayers();
      m_children.reserve( numChildren );
      for( int i = 0; i < numChildren; ++i )
      {
        m_children.push_back( new WMSLayerNodeInfo( layer->getSubLayerAt(i), this, i ) );
      }
    }
  }

  WMSServiceLayerModel::WMSLayerNodeInfo::~WMSLayerNodeInfo()
  {
    size_t numChildren = m_children.size();
    for( size_t i = 0; i < numChildren; ++i )
    {
      delete m_children[i];
    }
  }

  WMSServiceLayerModel::WMSServiceLayerModel( WMSService *service, TSLWMSServiceLayer *rootLayer )
    : ServiceLayerModel()
      , m_rootLayerNode( NULL )
      , m_service( service )
  {
    updateRootLayer( service, rootLayer );
  }

  WMSServiceLayerModel::~WMSServiceLayerModel()
  {
    delete m_rootLayerNode;
  }

  Qt::ItemFlags WMSServiceLayerModel::flags( const QModelIndex &index ) const
  {
    if( !index.isValid() )
    {
      return 0;
    }

    WMSLayerNodeInfo *node = reinterpret_cast< WMSLayerNodeInfo* >( index.internalPointer() );
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

  QVariant WMSServiceLayerModel::data( const QModelIndex &index, int role ) const
  {
    if( !m_rootLayerNode || !index.isValid() )
    {
      return QVariant();
    }

    WMSLayerNodeInfo *node = reinterpret_cast< WMSLayerNodeInfo* >( index.internalPointer() );

    if( !node->m_layer )
    {
      return QVariant();
    }

    switch( role )
    {
      case Qt::DisplayRole:
        {
          if( node->m_layer->title() )
          {
            return QVariant( QString::fromUtf8( node->m_layer->title() ) );
          }
          else if( node->m_layer->name() )
          {
            return QVariant( QString::fromUtf8( node->m_layer->name() ) );
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

  bool WMSServiceLayerModel::setData ( const QModelIndex &index, const QVariant &value, int role )
  {
    if( !index.isValid() )
    {
      return false;
    }

    WMSLayerNodeInfo *node = reinterpret_cast< WMSLayerNodeInfo* >( index.internalPointer() );

    if( !node->m_layer )
    {
      return false;
    }

    switch( role )
    {
      case Qt::CheckStateRole:
        {
          updateChildVisibility( node, value == Qt::Checked );
          updateParentVisibility( node );

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

  QVariant WMSServiceLayerModel::headerData( int /*section*/, Qt::Orientation /*orientation*/, int role ) const
  {
    if( role != Qt::DisplayRole )
    {
      return QVariant();
    }

    return QVariant( tr("Layer name") );
  }

  int WMSServiceLayerModel::rowCount( const QModelIndex &parent ) const
  {
    if( !m_rootLayerNode || parent.column() > 0 )
    {
      return 0;
    }

    WMSLayerNodeInfo *node = parent.isValid() ? reinterpret_cast< WMSLayerNodeInfo* >( parent.internalPointer() ) : m_rootLayerNode;
    return node->m_children.size();
  }

  int WMSServiceLayerModel::columnCount( const QModelIndex& /*parent*/ ) const
  {
    return 1;
  }

  QModelIndex WMSServiceLayerModel::index( int row, int column, const QModelIndex &parent ) const
  {
    if( !m_rootLayerNode || !hasIndex( row, column, parent ) )
    {
      return QModelIndex();
    }

    WMSLayerNodeInfo *node = parent.isValid() ? reinterpret_cast< WMSLayerNodeInfo* >( parent.internalPointer() ) : m_rootLayerNode;
    if( node->m_children.size() <= row )
    {
      // Invalid row requested
      return QModelIndex();
    }

    return createIndex( row, column, node->m_children[row] );
  }

  QModelIndex WMSServiceLayerModel::parent ( const QModelIndex &index ) const
  {
    if( !index.isValid() || !m_rootLayerNode )
    {
      return QModelIndex();
    }

    WMSLayerNodeInfo *thisNode = reinterpret_cast< WMSLayerNodeInfo* >( index.internalPointer() );
    if( !thisNode->m_parent || thisNode->m_parent == m_rootLayerNode )
    {
      // Do not return a model index for the root node, see the Qt documentation on QAbstractItemModel
      return QModelIndex();
    }

    return createIndex( thisNode->m_parent->m_layerIndex, 0, thisNode->m_parent );
  }

  void WMSServiceLayerModel::updateRootLayer( WMSService *service, TSLWMSServiceLayer *rootLayer )
  {
    // Tell the model and any attached views that the data has changed and needs reloading
    beginResetModel();

    delete m_rootLayerNode;
    m_service = service;

    // Create an empty root node to act as the top level container for the tree. This is so that
    // we get the root service layer shown in the treeview when visualising the model
    if( rootLayer )
    {
      m_rootLayerNode = new WMSLayerNodeInfo( NULL, NULL, 0 );
      m_rootLayerNode->m_children.push_back( new WMSLayerNodeInfo( rootLayer, m_rootLayerNode, 0 ) );
    }

    endResetModel();
  }

  void WMSServiceLayerModel::updateChildVisibility( WMSLayerNodeInfo *node, bool visible )
  {
    node->m_checked = visible ? Qt::Checked : Qt::Unchecked;

    bool originalVisibility = node->m_layer->getVisibility();
    node->m_layer->setVisibility( visible );
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
    }

    size_t numChildren = node->m_children.size();
    for( size_t i = 0; i < numChildren; ++i )
    {
      updateChildVisibility( node->m_children[i], visible );
    }

    // Changing the visibility of a node can changes the visibility of all its children
    if( !node->m_children.empty() )
    {
      QModelIndex firstChild( createIndex( 0, 0, node->m_children.front() ) );
      QModelIndex lastChild( createIndex( node->m_children.size()-1, 0, node->m_children.back() ) );
      emit dataChanged( firstChild, lastChild );
    }

    // Also emit the changed note for the node itself
    QModelIndex nodeIndex( createIndex( node->m_layerIndex, 0, node ) );
    emit dataChanged( nodeIndex, nodeIndex );
  }

  void WMSServiceLayerModel::updateParentVisibility( WMSLayerNodeInfo *node )
  {
    // When a child layer's visibility is changed, the parent layers checkbox state is
    // set to the partially checked state if any of the child layers are visible
    size_t numChildren = node->m_children.size();
    size_t numVisibleChildren = 0;
    for( size_t i = 0; i < numChildren; ++i )
    {
      if( node->m_children[i]->m_checked == Qt::Checked || node->m_children[i]->m_checked == Qt::PartiallyChecked )
      {
        ++numVisibleChildren;
      }
    }

    if( numVisibleChildren == 0 && (!node->m_layer || !node->m_layer->getVisibility()) )
    {
      // No child layers are enabled and the layer itself is not enabled (e.g. if it has no children)
      node->m_checked = Qt::Unchecked;
    }
    else if( numVisibleChildren == node->m_children.size() )
    {
      // All child layers are enabled or the layer itself is enabled
      node->m_checked = Qt::Checked;
    }
    else
    {
      // Only some child layers are enabled, set the checkbox to the partial state
      node->m_checked = Qt::PartiallyChecked;
    }

    QModelIndex nodeIndex( createIndex( node->m_layerIndex, 0, node ) );
    emit dataChanged( nodeIndex, nodeIndex );

    if( node->m_parent )
    {
      updateParentVisibility( node->m_parent );
    }
  }

  bool WMSServiceLayerModel::visibleLayerHasDimensions( WMSLayerNodeInfo *node ) const
  {
    // Dimensions apply to any child layers as well as this layer,
    // so if any children are visibile then this counts for using any dimensions
    // defined on this layer
    bool layerHasDimensions = false;
    if( node->m_layer )
    {
      layerHasDimensions = node->m_layer->noOfDimensions() > 0;

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

  bool WMSServiceLayerModel::anyChildrenVisible( WMSLayerNodeInfo *node ) const
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

  bool WMSServiceLayerModel::configurationValid() const
  {
    if( !m_rootLayerNode )
    {
      return false;
    }

    // We are considered to be valid if at least one layer is visible
    return !m_visibleLayers.empty();
  }

  bool WMSServiceLayerModel::selectionsHaveDimensions() const
  {
    if( !m_rootLayerNode )
    {
      return false;
    }

    return visibleLayerHasDimensions( m_rootLayerNode );
  }

  void WMSServiceLayerModel::updateLayerCRSCompatibility( WMSLayerNodeInfo *node )
  {
    // We only need to do this for layers that are not already visible,
    // layers that are already visible are known to be compatible
    if( node->m_layer && node->m_supportedCRS && !node->m_layer->getVisibility() && node->m_layer->name() )
    {
      bool wasCompatible = node->m_compatibleWithSelectedLayers;

      std::vector< const TSLWMSServiceLayer* > visibleLayers( m_visibleLayers.begin(), m_visibleLayers.end() );
      visibleLayers.push_back( node->m_layer );

      node->m_compatibleWithSelectedLayers = TSLWMSServiceLayer::layersHaveCompatibleCRS( &visibleLayers[0], visibleLayers.size() );

      if( node->m_compatibleWithSelectedLayers != wasCompatible )
      {
        // Only mark the node as changed if the state has actually changed
        QModelIndex nodeIndex( createIndex( node->m_layerIndex, 0, node ) );
        emit dataChanged( nodeIndex, nodeIndex );
      }
    }

    // Propogate the call to the child layers
    size_t numChildren = node->m_children.size();
    for( size_t i = 0; i < numChildren; ++i )
    {
      updateLayerCRSCompatibility( node->m_children[i] );
    }
  }
}
