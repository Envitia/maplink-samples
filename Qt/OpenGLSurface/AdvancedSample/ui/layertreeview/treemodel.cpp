/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "treeitem.h"
#include "treemodel.h"
#include "layertreeview.h"

#include <QtWidgets>
#include <sstream>
#include "MapLink.h"

static const QString g_nodeMimeType( "application/vnd.envitia.complexsample.treeitem" );

TreeModel::TreeModel( const QString &data, QObject *parent )
  : QAbstractItemModel( parent )
{

  createRootItem( "" );

}

TreeModel::~TreeModel()
{
  delete m_rootItem;
}

void TreeModel::createRootItem( const QString &data )
{
  QList<QVariant> rootData;
  rootData << "Title" << "Summary";
  m_rootItem = new TreeItem( rootData );
  setupModelData( data.split( QString( "\n" ) ), m_rootItem );
}

int TreeModel::columnCount( const QModelIndex &parent ) const
{
  if( parent.isValid() )
    return static_cast<TreeItem*>( parent.internalPointer() )->columnCount();
  else
    return m_rootItem->columnCount();
}

QVariant TreeModel::data( const QModelIndex &index, int role ) const
{
  if (!index.isValid())
  {
    return QVariant();
  }

  TreeItem *item = static_cast<TreeItem*>( index.internalPointer() );
  if (!item)
  {
    return QVariant();
  }

  if( (item->flags() & Qt::ItemIsUserCheckable) && index.column() == 1 && role == Qt::CheckStateRole && item->parentItem() != m_rootItem )
  {
    if( item->checked() )
    {
      return Qt::Checked;
    }
    else
    {
      return Qt::Unchecked;
    }
  }

  if( role != Qt::DisplayRole )
    return QVariant();

  return item->data( index.column() );
}

Qt::ItemFlags TreeModel::flags( const QModelIndex &index ) const
{
  if( !index.isValid() )
  {
    return QAbstractItemModel::flags( index ) | Qt::ItemIsDropEnabled;
  }

  Qt::ItemFlags itemFlags = QAbstractItemModel::flags( index ) | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
  if (TreeItem *treeItem = static_cast<TreeItem*>(index.internalPointer()))
  {
    itemFlags |= treeItem->flags();
  }

  return itemFlags;
}

QVariant TreeModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
  if( m_rootItem && orientation == Qt::Horizontal && role == Qt::DisplayRole )
  {
    return m_rootItem->data( section );
  }

  return QVariant();
}

QModelIndex TreeModel::index( int row, int column, const QModelIndex &parent ) const
{
  if( !hasIndex( row, column, parent ) )
    return QModelIndex();

  TreeItem *parentItem(NULL);

  if( !parent.isValid() )
  {
    parentItem = m_rootItem;
  }
  else
  {
    parentItem = static_cast<TreeItem*>( parent.internalPointer() );
  }

  if (parentItem)
  {
    TreeItem *childItem = parentItem->child(row);
    if (childItem)
    {
      return createIndex(row, column, childItem);
    }
  }
 
  return QModelIndex();
}

QModelIndex TreeModel::parent( const QModelIndex &index ) const
{
  if( !index.isValid() )
  {
    return QModelIndex();
  }

  TreeItem *childItem = static_cast<TreeItem*>( index.internalPointer() );
  TreeItem *parentItem = childItem ? childItem->parentItem() : NULL;

  if( !parentItem || parentItem == m_rootItem )
  {
    return QModelIndex();
  }

  return createIndex( parentItem->row(), 0, parentItem );
}

int TreeModel::rowCount( const QModelIndex &parent ) const
{
  if( parent.column() > 0 )
  {
    return 0;
  }

  TreeItem *parentItem(NULL);

  if( !parent.isValid() )
  {
    parentItem = m_rootItem;
  }
  else
  {
    parentItem = static_cast<TreeItem*>( parent.internalPointer() );
  }

  return parentItem ? parentItem->childCount() : 0;
}

void TreeModel::setupModelData( const QStringList &lines, TreeItem *parent )
{
  QList<TreeItem*> parents;
  QList<int> indentations;
  parents << parent;
  indentations << 0;

  int number = 0;

  while( number < lines.count() )
  {
    int position = 0;
    while( position < lines[number].length() )
    {
      if( lines[number].at( position ) != ' ' )
      {
        break;
      }
      position++;
    }

    QString lineData = lines[number].mid( position ).trimmed();

    if( !lineData.isEmpty() )
    {
      // Read the column data from the rest of the line.
      QStringList columnStrings = lineData.split( "\t", QString::SkipEmptyParts );
      QList<QVariant> columnData;
      for( int column = 0; column < columnStrings.count(); ++column )
      {
        columnData << columnStrings[column];
      }

      if( position > indentations.last() )
      {
        // The last child of the current parent is now the new parent
        // unless the current parent has no children.

        if( parents.last()->childCount() > 0 )
        {
          parents << parents.last()->child( parents.last()->childCount() - 1 );
          indentations << position;
        }
      }
      else
      {
        while( position < indentations.last() && parents.count() > 0 )
        {
          parents.pop_back();
          indentations.pop_back();
        }
      }

      // Append a new item to the current parent's list of children.
      parents.last()->appendChild( new TreeItem( columnData, parents.last() ) );
    }

    ++number;
  }
}

void TreeModel::refreshFromSurface( TSLDrawingSurface* drawingSurface, QString newLayerType )
{
  if (!drawingSurface)
  {
    return;
  }

  // Before deleting the root tree item, we must make a note of the data layer 
  // types stored on the tree with their corresponding names.
  std::vector< QString > names;
  std::vector< QString > types;

  int numChildren = m_rootItem->childCount();

  for( int i = 0; i < numChildren; ++i )
  {
    names.push_back( m_rootItem->child( i )->getName() );
    types.push_back( m_rootItem->child( i )->getType() );
  }

  // Delete the root item and create a new one.
  beginResetModel();

  m_drawingSurface = drawingSurface;
  delete m_rootItem;
  m_rootItem = NULL;
  createRootItem( "" );

  endResetModel();


  int numDataLayers = drawingSurface->getNumDataLayers();


  for( int i = 0; i < numDataLayers; ++i )
  {
    const char* layerName = NULL;

    drawingSurface->getDataLayerInfo( i, NULL, &layerName );
    QString qName = layerName;

    QString layerType = newLayerType;

    for( int j = 0; j < numChildren; ++j )
    {
      std::string val = (const char*)(names[j].toUtf8());
      if( names[j] == qName )
      {
        layerType = types[j].toUtf8();
        break;
      }
    }

    TreeItem* newItem( new TreeItem( m_rootItem, drawingSurface, i, layerType ) );
    m_rootItem->appendChild( newItem );
  }
}

Qt::DropActions TreeModel::supportedDragActions() const
{
  return Qt::MoveAction;
}

Qt::DropActions TreeModel::supportedDropActions() const
{
  return Qt::MoveAction;
}

QStringList TreeModel::mimeTypes() const
{
  QStringList supportedDragTypes;
  supportedDragTypes << g_nodeMimeType;
  return supportedDragTypes;
}

bool TreeModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
  TreeItem *childItem = static_cast<TreeItem*>( index.internalPointer() );

  if( childItem && childItem->data( 0 ) == "Transparency" )
  {
    bool ok = false;
    int intValue = value.toInt( &ok );

    if ( ok && intValue >= 0 && intValue <= 255 )
    {
      childItem->setData( 1, value );
      emit modelModified( index, value.toInt() );
      return true;
    }
  }

  return false;
}

QMimeData* TreeModel::mimeData( const QModelIndexList &indexes ) const
{
  if( indexes.empty() )
  {
    return NULL;
  }

  QMimeData* data = new QMimeData();
  QByteArray encodedData;
  QDataStream stream( &encodedData, QIODevice::WriteOnly );

  foreach( const QModelIndex &index, indexes )
  {
    if( index.isValid() )
    {
      qintptr nodePtr = (qintptr)&index;
      stream << nodePtr;
    }
  }

  data->setData( g_nodeMimeType, encodedData );

  return data;
}

bool TreeModel::canDropMimeData( const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent ) const
{
  if( action != Qt::MoveAction )
  {
    return false;
  }

  if( !data || !data->hasFormat(g_nodeMimeType) )
  {
    return false;
  }

  qintptr nodePtr = 0;
  QByteArray encodedData = data->data( g_nodeMimeType );
  QDataStream stream( &encodedData, QIODevice::ReadOnly );
  stream >> nodePtr;

  QModelIndex* draggedItemIndex = reinterpret_cast<QModelIndex*>( nodePtr );
  if (!draggedItemIndex)
  {
    return false;
  }

  TreeItem* draggedItem = static_cast<TreeItem*>( draggedItemIndex->internalPointer() );
  if (!draggedItem)
  {
    return false;
  }

  TreeItem::TreeItemType draggedItemType( draggedItem->type() );
  if( draggedItemType == TreeItem::TreeItemTypeDefault )
  {
    // DataLayer's may only be dropped under the root item
    if( parent.isValid() )
    {
      return false;
    }
    return true;
  }
#ifdef HAVE_DIRECT_IMPORT_SDK
  else if( draggedItemType == TreeItem::TreeItemTypeDirectImportDataSet )
  {
    TreeItem* droppedParentItem( static_cast<TreeItem*>( parent.internalPointer() ) );
    if( !droppedParentItem || droppedParentItem->type() != TreeItem::TreeItemTypeDirectImportScaleBand )
    {
      return false;
    }
    // Datasets can't be moved between scale bands
    if( droppedParentItem != draggedItem->parentItem() )
    {
      return false;
    }
    return true;
  }
#endif
  return false;
}

bool TreeModel::dropMimeData( const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent )
{
  if( action != Qt::MoveAction )
  {
    // We only allow move actions
    return false;
  }

  if( !data || !data->hasFormat( g_nodeMimeType ) )
  {
    return false;
  }

  qintptr nodePtr = 0;
  QByteArray encodedData = data->data( g_nodeMimeType );
  QDataStream stream( &encodedData, QIODevice::ReadOnly );
  stream >> nodePtr;

  QModelIndex* draggedItemIndex = reinterpret_cast<QModelIndex*>( nodePtr );
  if (!draggedItemIndex)
  {
    return false;
  }

  TreeItem* draggedItem = static_cast<TreeItem*>( draggedItemIndex->internalPointer() );
  if (!draggedItem)
  {
    return false;
  }

  TreeItem::TreeItemType draggedItemType( draggedItem->type() );
  if( draggedItemType == TreeItem::TreeItemTypeDefault )
  {
    QString layerName = draggedItem->getName();
    QString targetName;

    if( row == 0 )
    {
      targetName = draggedItemIndex->model()->data( draggedItemIndex->model()->index( 0, 0 ), Qt::DisplayRole ).toString();
    }
    else
    {
      targetName = draggedItemIndex->model()->data( draggedItemIndex->model()->index( row - 1, 0 ), Qt::DisplayRole ).toString();
    }

    // Ask the application to move the layer, and refresh this
    emit moveLayerToIndex( layerName.toUtf8().constData(), targetName.toUtf8().constData(), row );
    return true;
  }
#ifdef HAVE_DIRECT_IMPORT_SDK
  else if( draggedItemType == TreeItem::TreeItemTypeDirectImportDataSet )
  {
    TreeItem* droppedParentItem( static_cast<TreeItem*>( parent.internalPointer() ) );
    if( !droppedParentItem || droppedParentItem->type() != TreeItem::TreeItemTypeDirectImportScaleBand )
    {
      return false;
    }
    // Datasets can't be moved between scale bands
    if( droppedParentItem != draggedItem->parentItem() )
    {
      return false;
    }

    // Find the name of the direct import layer
    TreeItem* directImportLayerItem( droppedParentItem->parentItem() );
    if( !directImportLayerItem )
    {
      return false;
    }


    emit moveDirectImportDataSetToIndex( directImportLayerItem->getName().toUtf8().constData(), droppedParentItem->getName().toUtf8().constData(), draggedItemIndex->row(), row );
    return true;
  }
#endif

  return false;
}

TreeItem* TreeModel::getRootItem()
{
  return m_rootItem;
}
