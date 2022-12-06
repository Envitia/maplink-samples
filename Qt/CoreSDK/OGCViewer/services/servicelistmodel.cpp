/****************************************************************************
  Copyright (c) 2017 by Envitia Group PLC.
 ****************************************************************************/

#include <QStringList>
#include <QMimeData>
#include <QMenu>
#include <cassert>

#include "servicelistmodel.h"
#include "servicelist.h"

#include "MapLink.h"

namespace Services
{

  static const QString g_nodeMimeType( "application/vnd.envitia.ogcviewer.serviceitem" );

  ServiceListModel::ServiceListNode::ServiceListNode()
    : m_parent( NULL )
      , m_index( 0 )
      , m_dataType( NodeTypeRoot )
      , m_data( NULL )
  {
  }

  ServiceListModel::ServiceListNode::~ServiceListNode()
  {
    size_t numChildNodes = m_children.size();
    for( size_t i = 0; i < numChildNodes; ++i )
    {
      delete m_children[i];
    }

    if( m_dataType == NodeTypeLayer )
    {
      // A layer node owns the data pointer, and so must free it
      Service::ServiceLayer *layerInfo = reinterpret_cast< Service::ServiceLayer* >( m_data );
      delete layerInfo;
    }
  }

  ServiceListModel::ServiceListModel( ServiceList *services )
    : m_services( services )
      , m_rootNode( new ServiceListNode() )
      , m_serviceIcon( ":/icons/images/server.png" )
      , m_layersIcon( ":/icons/images/layers.png" )
  {
  }

  ServiceListModel::~ServiceListModel()
  {
    delete m_rootNode;
  }

  void ServiceListModel::addService( Service *service )
  {
    // New services are always added as the first item off the root node as they will appear on top of
    // any current services, and should therefore be at the top of the list
    beginInsertRows( QModelIndex(), 0, 0 );

    ServiceListNode *newServiceNode = new ServiceListNode();
    newServiceNode->m_parent = m_rootNode;
    newServiceNode->m_index = m_rootNode->m_children.size();
    newServiceNode->m_dataType = NodeTypeService;
    newServiceNode->m_data = service;

    // Add one child node for each layer visible from the service, sorted by the order they will appear in
    // (so the item drawn last is on top)
    std::vector< Service::ServiceLayer* > layers;
    service->getVisibleLayers( layers );
    newServiceNode->m_children.reserve( layers.size() );
    std::vector< Service::ServiceLayer* >::reverse_iterator layerIt( layers.rbegin() );
    std::vector< Service::ServiceLayer* >::reverse_iterator layerItE( layers.rend() );
    for( int index = 0; layerIt != layerItE; ++layerIt, ++index )
    {
      ServiceListNode *layerNode = new ServiceListNode();
      layerNode->m_parent = newServiceNode;
      layerNode->m_index = index;
      layerNode->m_dataType = NodeTypeLayer;
      layerNode->m_data = *layerIt;
      newServiceNode->m_children.push_back( layerNode );
    }

    m_rootNode->m_children.insert( m_rootNode->m_children.begin(), newServiceNode );

    updateChildNodeIndices( m_rootNode );

    endInsertRows();
  }

  bool ServiceListModel::getItemTMCExtent( const QModelIndex &index, TSLTMC &x1, TSLTMC &y1, TSLTMC &x2, TSLTMC &y2 ) const
  {
    if( !index.isValid() )
    {
      return false;
    }

    ServiceListNode *node = reinterpret_cast< ServiceListNode* >( index.internalPointer() );
    if( node->m_dataType == NodeTypeService )
    {
      // The extent of a service node is the sum extent of all currently visible layers. This is
      // also the extent of the data layer used to communicate with the service
      Service *service = reinterpret_cast< Service* >( node->m_data );
      return service->dataLayer()->getTMCExtent( &x1, &y1, &x2, &y2 );
    }
    else if( node->m_dataType == NodeTypeLayer )
    {
      Service::ServiceLayer *layerInfo = reinterpret_cast< Service::ServiceLayer* >( node->m_data );

      assert( node->m_parent && node->m_parent->m_dataType == NodeTypeService );

      return layerInfo->getTMCExtent( x1, y1, x2, y2 );
    }

    // The root item is selected, this has no extent
    return false;
  }

  bool ServiceListModel::getItemTMCExtent( const QVariant &index, TSLTMC &x1, TSLTMC &y1, TSLTMC &x2, TSLTMC &y2 ) const
  {
    if( !index.isValid() )
    {
      return false;
    }

    ServiceListNode *node = reinterpret_cast< ServiceListNode* >( index.value<void*>() );
    return getItemTMCExtent( createIndex( node->m_index, 0, node ), x1, y1, x2, y2 );
  }

  ServiceListModel::ServiceNodeType ServiceListModel::getNodeTypeForIndex( const QModelIndex &index ) const
  {
    if( !index.isValid() )
    {
      return NodeTypeInvalid;
    }

    ServiceListNode *node = reinterpret_cast< ServiceListNode* >( index.internalPointer() );
    return node->m_dataType;
  }

  void ServiceListModel::getPotentialLayersForDisplay(const QModelIndex &index, std::vector< std::string > &layerDisplayNames ) const
  {
    if( !index.isValid() )
    {
      return;
    }

    ServiceListNode *node = reinterpret_cast< ServiceListNode* >( index.internalPointer() );
    if( node->m_dataType != NodeTypeService )
    {
      // This function only makes sense for service nodes
      return;
    }

    Service *service = reinterpret_cast< Service* >( node->m_data );
    service->getPotentialLayersForDisplay( layerDisplayNames );
  }

  void ServiceListModel::getLayerDimensionNames( const QModelIndex &index, std::vector< std::string > &dimensionNames ) const
  {
    if( !index.isValid() )
    {
      return;
    }

    ServiceListNode *node = reinterpret_cast< ServiceListNode* >( index.internalPointer() );
    if( node->m_dataType != NodeTypeLayer )
    {
      // This function only makes sense for layer nodes
      return;
    }

    Service::ServiceLayer *layerInfo = reinterpret_cast< Service::ServiceLayer* >( node->m_data );
    layerInfo->getDimensionNames( dimensionNames );
  }

  void ServiceListModel::getLayerStyleNames( const QModelIndex &index, std::vector< std::string > &styleNames ) const
  {
    if( !index.isValid() )
    {
      return;
    }

    ServiceListNode *node = reinterpret_cast< ServiceListNode* >( index.internalPointer() );
    if( node->m_dataType != NodeTypeLayer )
    {
      // This function only makes sense for layer nodes
      return;
    }

    Service::ServiceLayer *layerInfo = reinterpret_cast< Service::ServiceLayer* >( node->m_data );
    layerInfo->getStyleNames( styleNames );
  }

  void ServiceListModel::removeItem( const QVariant &nodeVariant )
  {
    if( !nodeVariant.isValid() )
    {
      return;
    }

    ServiceListNode *node = reinterpret_cast< ServiceListNode* >( nodeVariant.value<void*>() );

    QModelIndex nodeIndex( createIndex( node->m_index, 0, node ) );

    beginRemoveRows( nodeIndex.parent(), node->m_index, node->m_index );

    // Remove the moved node from its parent
    node->m_parent->m_children.erase( node->m_parent->m_children.begin() + node->m_index );

    // Update the indices of the subsequent nodes in the source parent to account for the removed node
    updateChildNodeIndices( node->m_parent );

    if( node->m_dataType == NodeTypeService )
    {
      Service *service = reinterpret_cast< Service* >( node->m_data );
      m_services->removeService( service );
    }
    else if( node->m_dataType == NodeTypeLayer )
    {
      Service *service = reinterpret_cast< Service* >( node->m_parent->m_data );
      Service::ServiceLayer *layerInfo = reinterpret_cast< Service::ServiceLayer* >( node->m_data );
      service->setLayerVisibility( layerInfo, false );

      // Changing the layer visibility requires any attached drawing surfaces to be redrawn
      m_services->redrawAttachedSurface();
    }

    // Delete the node
    delete node;

    endRemoveRows();
  }

  const char* ServiceListModel::getLayerDimensionUnits( const QVariant &nodeVariant, const QString &name ) const
  {
    if( !nodeVariant.isValid() )
    {
      return NULL;
    }

    ServiceListNode *node = reinterpret_cast< ServiceListNode* >( nodeVariant.value<void*>() );
    if( node->m_dataType != NodeTypeLayer )
    {
      // This function only makes sense for layer nodes
      return NULL;
    }

    Service::ServiceLayer *layerInfo = reinterpret_cast< Service::ServiceLayer* >( node->m_data );
    return layerInfo->getDimensionUnits( name.toUtf8().constData() );
  }

  void ServiceListModel::setLayerVisibility( const QVariant &nodeVariant, const QString &layerName, bool visible )
  {
    if( !nodeVariant.isValid() )
    {
      return;
    }

    ServiceListNode *node = reinterpret_cast< ServiceListNode* >( nodeVariant.value<void*>() );
    if( node->m_dataType != NodeTypeService )
    {
      // This operation is only valid for service nodes
      return;
    }

    if( !visible )
    {
      // Making a layer invisible is the same as calling removeItem for that layer, so do that
      removeItem( nodeVariant );
      return;
    }

    Service *service = reinterpret_cast< Service* >( node->m_data );
    void *layerToken = service->setLayerVisibility( layerName.toUtf8(), visible );
    if( !layerToken )
    {
      // The layer was not found, so the operation failed
      return;
    }

    // Add a new node for the layer since it's not currently in the model. The layer
    // should be added to the front of the layer list so that it appears on top of
    // any current layers
    QModelIndex serviceNodeIndex( createIndex( node->m_index, 0, node ) );

    beginInsertRows( serviceNodeIndex, 0, 0 );

    ServiceListNode *layerNode = new ServiceListNode();
    layerNode->m_parent = node;
    layerNode->m_index = 0;
    layerNode->m_dataType = NodeTypeLayer;
    layerNode->m_data = service->getLayerForToken( layerToken );

    node->m_children.insert( node->m_children.begin(), layerNode );

    updateChildNodeIndices( node );

    endInsertRows();
    // Changing the layer visibility requires a redraw
    m_services->redrawAttachedSurface();
  }

  void ServiceListModel::setLayerStyle( const QVariant &nodeVariant, const QString &styleName )
  {
    if( !nodeVariant.isValid() )
    {
      return;
    }

    ServiceListNode *node = reinterpret_cast< ServiceListNode* >( nodeVariant.value<void*>() );
    if( node->m_dataType != NodeTypeLayer )
    {
      // This operation is only valid for layer nodes
      return;
    }

    Service::ServiceLayer *layerInfo = reinterpret_cast< Service::ServiceLayer* >( node->m_data );
    layerInfo->setStyle( styleName.toUtf8() );

    // Changing the layer style requires a redraw
    m_services->redrawAttachedSurface();
  }

  Service* ServiceListModel::getService( const QVariant &nodeVariant )
  {
    if( !nodeVariant.isValid() )
    {
      return NULL;
    }

    ServiceListNode *node = reinterpret_cast< ServiceListNode* >( nodeVariant.value<void*>() );
    switch( node->m_dataType )
    {
      case NodeTypeService:
        return reinterpret_cast< Service* >( node->m_data );

      case NodeTypeLayer:
        return reinterpret_cast< Service* >( node->m_parent->m_data );

      default:
        return NULL;
    }
  }

  Qt::ItemFlags ServiceListModel::flags( const QModelIndex &index ) const
  {
    if( !index.isValid() )
    {
      return QAbstractItemModel::flags(index) | Qt::ItemIsDropEnabled;
    }

    Qt::ItemFlags baseFlags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    ServiceListNode *node = reinterpret_cast< ServiceListNode* >( index.internalPointer() );
    if( node->m_dataType == NodeTypeService )
    {
      // Service nodes must be both drag and drop enabled so that they are valid targets for layer drag operations
      baseFlags |= Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
    }
    else if( node->m_dataType == NodeTypeLayer )
    {
      // Allow drag operations for layer nodes so that they can be reordered in the tree
      baseFlags |= Qt::ItemIsDragEnabled | Qt::ItemNeverHasChildren;
    }

    return baseFlags;
  }

  QVariant ServiceListModel::data( const QModelIndex &index, int role ) const
  {
    if( !m_services || !index.isValid() )
    {
      return QVariant();
    }

    ServiceListNode *node = reinterpret_cast< ServiceListNode* >( index.internalPointer() );

    switch( role )
    {
      case Qt::DisplayRole:
        {
          if( node->m_dataType == NodeTypeService )
          {
            Service *service = reinterpret_cast< Service* >( node->m_data );
            const char *serviceDisplayName = service->getServiceDisplayName();
            if( serviceDisplayName )
            {
              return QVariant( serviceDisplayName );
            }
          }
          else if( node->m_dataType == NodeTypeLayer )
          {
            Service::ServiceLayer *layerInfo = reinterpret_cast< Service::ServiceLayer* >( node->m_data );
            const char *layerDisplayName = layerInfo->displayName();
            if( layerDisplayName )
            {
              return QVariant( layerDisplayName );
            }
          }
          return QVariant( "<Unnamed>" );
        }

      case Qt::UserRole:
        {
          // For this model, the user role is used by attached views to record which node actions should
          // occur in in subsequent slots.
          // To do this we just encode the node pointer into the variant.
          QVariant userRoleVariant;
          userRoleVariant.setValue( index.internalPointer() );
          return QVariant( userRoleVariant );
        }

      case Qt::DecorationRole:
        {
          if( node->m_dataType == NodeTypeService )
          {
            return QVariant( m_serviceIcon );
          }
          else if( node->m_dataType == NodeTypeLayer )
          {
            return QVariant( m_layersIcon );
          }

          return QVariant();
        }

      default:
        break;
    }

    return QVariant();
  }

  QVariant ServiceListModel::headerData( int /*section*/, Qt::Orientation /*orientation*/, int /*role*/ ) const
  {
    return QVariant();
  }

  int ServiceListModel::rowCount( const QModelIndex &parent ) const
  {
    if( !m_services || parent.column() > 0 )
    {
      return 0;
    }

    if( !parent.isValid() )
    {
      return m_rootNode->m_children.size();
    }


    ServiceListNode *node = reinterpret_cast< ServiceListNode* >( parent.internalPointer() );
    return node->m_children.size();
  }

  int ServiceListModel::columnCount( const QModelIndex& /*parent*/ ) const
  {
    return 1;
  }

  QModelIndex ServiceListModel::index( int row, int column, const QModelIndex &parent ) const
  {
    if( !m_services || !hasIndex( row, column, parent ) )
    {
      return QModelIndex();
    }

    ServiceListNode *node = parent.isValid() ? reinterpret_cast< ServiceListNode* >( parent.internalPointer() ) : m_rootNode;
    if( node->m_children.size() <= row )
    {
      return QModelIndex();
    }

    return createIndex( row, column, node->m_children[row] );
  }

  QModelIndex ServiceListModel::parent ( const QModelIndex &index ) const
  {
    if( !index.isValid() || !m_services )
    {
      return QModelIndex();
    }

    ServiceListNode *node = reinterpret_cast< ServiceListNode* >( index.internalPointer() );
    if( !node->m_parent || node->m_parent == m_rootNode )
    {
      // Do not return a model index for the root node, see the Qt documentation on QAbstractItemModel
      return QModelIndex();
    }

    return createIndex( node->m_parent->m_index, 0, node->m_parent );
  }

  QStringList ServiceListModel::mimeTypes() const
  {
    QStringList supportedDragTypes;
    supportedDragTypes << g_nodeMimeType;
    return supportedDragTypes;
  }

  QMimeData* ServiceListModel::mimeData(const QModelIndexList &indexes) const
  {
    QMimeData *data = new QMimeData();
    QByteArray encodedData;
    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    foreach (const QModelIndex &index, indexes)
    {
      if (index.isValid())
      {
        qintptr nodePtr = (qintptr)index.internalPointer();
        stream << nodePtr;
      }
    }

    data->setData( g_nodeMimeType, encodedData );

    return data;
  }

  bool ServiceListModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
  {
    if( action != Qt::MoveAction )
    {
      // We only allow move actions
      return false;
    }

    if( !data->hasFormat( g_nodeMimeType ) || column > 0 )
    {
      return false;
    }

    qintptr nodePtr = 0;
    QByteArray encodedData = data->data( g_nodeMimeType );
    QDataStream stream( &encodedData, QIODevice::ReadOnly );
    stream >> nodePtr;

    ServiceListNode *node = reinterpret_cast< ServiceListNode* >( nodePtr );
    if( node->m_dataType == NodeTypeService )
    {
      // Service nodes can only be dropped onto the root node or another service node
      ServiceListNode *destinationParentNode = NULL;
      QModelIndex destinationParent = parent;
      int moveTargetRow = row;

      if( parent.isValid() )
      {
        destinationParentNode = reinterpret_cast< ServiceListNode* >( parent.internalPointer() );
        if( destinationParentNode->m_dataType == NodeTypeService )
        {
          // Otherwise, do not allow the drop - we don't want to embed a service inside the layer list of a different service
          return false;
        }
      }
      else
      {
        // An invalid parent index means we are dropping directly onto the root node
        destinationParentNode = m_rootNode;
      }

      if( moveTargetRow < 0 )
      {
        // This means that the drop was not on a specific node - the item should be moved to the end of the list
        moveTargetRow = destinationParentNode->m_children.size();
      }

      QModelIndex sourceParent = createIndex( node->m_parent->m_index, 0, node->m_parent );

      int originalLayerIndex = node->m_index;
      if( !beginMoveRows( QModelIndex(), node->m_index, node->m_index, destinationParent, moveTargetRow ) )
      {
        return false;
      }

      // Remove the moved node from its parent
      node->m_parent->m_children.erase( node->m_parent->m_children.begin() + node->m_index );

      // Update the indices of the subsequent nodes in the source parent to account for the moved node
      updateChildNodeIndices( node->m_parent );

      // And insert it at the designated place in the destination parent
      bool moveInFrontOfTargetLayer = true;
      if( moveTargetRow > originalLayerIndex )
      {
        // The item is being moved towards the end of the list, in this case moveTargetRow is one greater
        // than the index it will be inserted into
        --moveTargetRow;
        moveInFrontOfTargetLayer = false;
      }

      if( moveTargetRow < destinationParentNode->m_children.size() )
      {
        destinationParentNode->m_children.insert( destinationParentNode->m_children.begin() + moveTargetRow, node );
        node->m_index = moveTargetRow;
      }
      else
      {
        // Item is being inserted at the end
        destinationParentNode->m_children.push_back( node );
        node->m_index = destinationParentNode->m_children.size() - 1;
      }

      // Update the row indices of all subsequent nodes to account for the new node we inserted
      updateChildNodeIndices( destinationParentNode );

      endMoveRows();

      // Tell the service list to replicate the order change in any drawing surfaces that have the data
      // layers for the affected service in.
      // The order in the drawing surface is the reverse of what's in our model, as it uses draw ordering.
      // Therefore reverse the index we have for the node to get the index in the surface for the corresponding
      // layer.
      m_services->updateLayerDrawOrder( destinationParentNode->m_children.size() - originalLayerIndex - 1,
          destinationParentNode->m_children.size() - node->m_index - 1, moveInFrontOfTargetLayer );

      return true;
    }
    else if( node->m_dataType == NodeTypeLayer )
    {
      // Layer nodes can only be dropped within the same parent service node
      if( !parent.isValid() )
      {
        return false;
      }

      ServiceListNode *destinationParentNode = destinationParentNode = reinterpret_cast< ServiceListNode* >( parent.internalPointer() );
      if( destinationParentNode->m_dataType != NodeTypeService ||
          destinationParentNode != node->m_parent )
      {
        // The parent node of a layer must be the same service node that the layer is currently on
        return false;
      }

      int moveTargetRow = row;
      if( row < 0 )
      {
        moveTargetRow = 0;
      }
      if( node->m_index == moveTargetRow )
      {
        // This would not move the node, so don't allow the drop
        return false;
      }

      QModelIndex sourceParent = createIndex( node->m_parent->m_index, 0, node->m_parent );

      int originalLayerIndex = node->m_index;
      if( !beginMoveRows( parent, node->m_index, node->m_index, parent, row ) )
      {
        // Invalid move operation
        return false;
      }

      // Remove the moved node from its parent
      node->m_parent->m_children.erase( node->m_parent->m_children.begin() + node->m_index );

      // Update the indices of the subsequent nodes in the source parent to account for the moved node
      updateChildNodeIndices( node->m_parent );

      // And insert it at the designated place in the destination parent
      bool moveInFrontOfTargetLayer = false;
      if( moveTargetRow > originalLayerIndex )
      {
        // The item is being moved towards the end of the list, in this case moveTargetRow is one greater
        // than the index it will be inserted into
        --moveTargetRow;
        moveInFrontOfTargetLayer = true;
      }

      if( moveTargetRow < destinationParentNode->m_children.size() )
      {
        destinationParentNode->m_children.insert( destinationParentNode->m_children.begin() + moveTargetRow, node );
        node->m_index = moveTargetRow;
      }
      else
      {
        // Item is being inserted at the end
        destinationParentNode->m_children.push_back( node );
        node->m_index = destinationParentNode->m_children.size() - 1;
      }

      // Update the row indices of all subsequent nodes to account for the new node we inserted
      updateChildNodeIndices( destinationParentNode );

      endMoveRows();

      // Tell the service list to replicate the order change in any drawing surfaces that have the data
      // layers for the affected service in.
      // Layer orders are the reverse of the order in the model (they are in draw order), so convert the indices
      // when updating the visibility order for the layer
      Service *service = reinterpret_cast< Service* >( destinationParentNode->m_data );
      service->updateLayerVisibilityOrder( destinationParentNode->m_children.size() - originalLayerIndex - 1,
          destinationParentNode->m_children.size() - node->m_index - 1 );

      // Request that the drawing surface containing the data layers is redrawn
      m_services->redrawAttachedSurface();

      return true;
    }

    return false;
  }

  bool ServiceListModel::canDropMimeData(const QMimeData *data, Qt::DropAction action, int /*row*/, int column, const QModelIndex &parent) const
  {
    if( action != Qt::MoveAction )
    {
      // We only allow move actions
      return false;
    }

    if( !data->hasFormat( g_nodeMimeType ) || column > 0 )
    {
      return false;
    }

    qintptr nodePtr = 0;
    QByteArray encodedData = data->data( g_nodeMimeType );
    QDataStream stream( &encodedData, QIODevice::ReadOnly );
    stream >> nodePtr;

    ServiceListNode *node = reinterpret_cast< ServiceListNode* >( nodePtr );
    if( node->m_dataType == NodeTypeService )
    {
      // Service nodes can only be dropped onto the root node or another service node
      ServiceListNode *destinationParentNode = NULL;
      QModelIndex destinationParent( parent );

      if( parent.isValid() )
      {
        destinationParentNode = reinterpret_cast< ServiceListNode* >( parent.internalPointer() );
        if( destinationParentNode->m_dataType == NodeTypeService )
        {
          // Otherwise, do not allow the drop - we don't want to embed a service inside the layer list of a different service
          return false;
        }
      }

      // This is a valid drop location
      return true;
    }
    else if( node->m_dataType == NodeTypeLayer )
    {
      // Layer nodes can only be dropped within the same parent service node
      if( !parent.isValid() )
      {
        return false;
      }

      ServiceListNode *destinationParentNode = destinationParentNode = reinterpret_cast< ServiceListNode* >( parent.internalPointer() );
      if( destinationParentNode->m_dataType != NodeTypeService ||
          destinationParentNode != node->m_parent )
      {
        // The parent node of a layer must be the same service node that the layer is currently on
        return false;
      }

      // This is a valid drop location

      return true;
    }

    return false;
  }

  Qt::DropActions ServiceListModel::supportedDropActions() const
  {
    return Qt::MoveAction;
  }

  void ServiceListModel::updateChildNodeIndices( ServiceListNode *node )
  {
    for( size_t i = 0; i < node->m_children.size(); ++i )
    {
      node->m_children[i]->m_index = i;
    }
  }

};
