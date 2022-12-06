/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include <algorithm>
#include <iostream>

#include <QMenu>
#include <QContextMenuEvent>
#include <QListWidget>

#include "treemodel.h"
#include "treeitem.h"
#ifdef HAVE_DIRECT_IMPORT_SDK
# include "directimportscalebandtreeitem.h"
# include "directimportdatasettreeitem.h"
#endif

#include "layertreeview.h"
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Constructor
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LayerTreeView::LayerTreeView( QWidget *parent )
  : QTreeView( parent )
#ifdef HAVE_DIRECT_IMPORT_SDK
  , m_dataSetSelected(NULL)
  , m_scaleBandSelected(NULL)
  , m_dataSetToEdit( NULL )
#endif
{
  connect( this, SIGNAL( clicked( const QModelIndex& ) ), this, SLOT( itemClicked( const QModelIndex& ) ) );
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Destructor
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LayerTreeView::~LayerTreeView()
{
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Clear Selected Layer
//
// Clears the name of the currently selected layer.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void LayerTreeView::clearSelectedLayer()
{
  m_selectedLayer.clear();
#ifdef HAVE_DIRECT_IMPORT_SDK
  m_dataSetSelected = NULL;
  m_scaleBandSelected = NULL;
#endif
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Remove Item
//
// This function is called when the user clicks on 'Remove Item' in the
// context menu. ( see LayerTreeView::showContextMenuForLayer )
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void LayerTreeView::removeItem()
{
  if( m_selectedLayer != "" )
  {
    removeTreeLayer( m_selectedLayer, m_selectedType );
    clearSelectedLayer();
    selectedAttributeData( "", "" );
  }
}

#ifdef HAVE_DIRECT_IMPORT_SDK
void LayerTreeView::removeDirectImportData()
{
  if( !m_dataSetToEdit )
  {
    return;
  }

  // Fire signal with layer name, scale band name and dataset index
  TreeItem* scaleBandItem( m_dataSetToEdit->parentItem() );
  if( !scaleBandItem )
  {
    return;
  }
  TreeItem* directImportLayerItem( scaleBandItem->parentItem() );
  if( !directImportLayerItem )
  {
    return;
  }

  emit removeDirectImportDataSet( directImportLayerItem->getName().toUtf8().constData(), scaleBandItem->getName().toUtf8().constData(), m_dataSetToEdit->row() );

  m_dataSetToEdit = NULL;
}


void LayerTreeView::editDirectImportData()
{
  if( !m_dataSetToEdit )
  {
    return;
  }

  // Fire signal with layer name, scale band name and dataset index
  TreeItem* scaleBandItem( m_dataSetToEdit->parentItem() );
  if( !scaleBandItem )
  {
    return;
  }
  TreeItem* directImportLayerItem( scaleBandItem->parentItem() );
  if( !directImportLayerItem )
  {
    return;
  }

  emit editDirectImportDataSet( directImportLayerItem->getName().toUtf8().constData(), scaleBandItem->getName().toUtf8().constData(), m_dataSetToEdit->row() );

  m_dataSetToEdit = NULL;
}

#endif

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Zoom To Extent
//
// The 'Zoom To Extent' button has been clicked. This function
// passes the information to MapLinkWidget and allows the currently
// selected layer to have it's full extent displayed on the MapLink
// Widget.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void LayerTreeView::zoomToExtentClicked()
{
  if( m_selectedLayer != "" )
  {
    zoomToLayerExtent( m_selectedLayer );
  }
#ifdef HAVE_DIRECT_IMPORT_SDK
  else if( m_dataSetSelected != NULL )
  {
    zoomToLayerDataSetExtent( m_dataSetSelected );
  }
  else if( m_scaleBandSelected != NULL )
  {
    zoomToLayerScaleBandExtent( m_scaleBandSelected );
  }
#endif

}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Load Layer
//
// This function is called when the user clicks on 'Add Layer' in the
// context menu. ( see LayerTreeView::showContextMenuForLayer )
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void LayerTreeView::loadLayer()
{
  addLayer();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Load Attribute Data
//
// This function is called when the user clicks on 'Load Attribute Data' 
// in the context menu. ( see LayerTreeView::showContextMenuForLayer )
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void LayerTreeView::loadAttributeData()
{
  selectedAttributeData( m_selectedLayer, m_selectedType );
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Edit Cache Size
//
// This function is called when the user clicks on 'Edit Cache Size' 
// in the context menu. ( see LayerTreeView::showContextMenuForLayer )
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void LayerTreeView::editCacheSize()
{
  editCacheSize( m_selectedLayer );
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Item Clicked
//
// Activates when an item on the tree view is clicked. This function
// is only concerned with toggling the check state of the 'Visible'
// checkbox of a layer.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void LayerTreeView::itemClicked( const QModelIndex& index )
{
  if( index.model()->data( index, Qt::DisplayRole ) == "" )
  {
    bool checkState = index.model()->data( index, Qt::CheckStateRole ).toBool();

    modifyData( index, !checkState );
    dataChanged( index, index );
  }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Selection Changed
//
// This function is called when a different layer has been 
// selected on the LayerTreeView. 
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void LayerTreeView::selectionChanged( const QItemSelection &selected, const QItemSelection &deselected )
{
  QModelIndexList indexList = selected.indexes();

  if ( indexList.size() > 0 )   // size will usually be 2, because there are 2 columns
  {
    // Updates the TreeView selection
    dataChanged( indexList.first(), indexList.last() );

    const QModelIndex& current = indexList.first();

#ifdef HAVE_DIRECT_IMPORT_SDK
    m_dataSetSelected = NULL;
    m_scaleBandSelected = NULL;
#endif
    m_selectedLayer.clear();
    m_selectedType.clear();

    TreeItem* treeItem( reinterpret_cast<TreeItem*>(current.internalPointer()) );
    if ( !treeItem )
    {
      return;
    }

    TreeItem::TreeItemType itemType( treeItem->type() );

    if ( itemType == TreeItem::TreeItemTypeDefault && current.parent() == model()->index( -1, -1 ) )
    {
      m_selectedLayer = (const char*)(model()->data( model()->index( current.row(), 0 ), Qt::DisplayRole ).toString().toUtf8());
      m_selectedType = (const char*)(model()->data( model()->index( current.row(), 1 ), Qt::DisplayRole ).toString().toUtf8());
    }
#ifdef HAVE_DIRECT_IMPORT_SDK
    else if ( itemType == TreeItem::TreeItemTypeDirectImportDataSet )
    {
      m_dataSetSelected = dynamic_cast<DirectImportDataSetTreeItem*>(treeItem);
    }
    else if ( itemType == TreeItem::TreeItemTypeDirectImportScaleBand )
    {
      m_scaleBandSelected = dynamic_cast<DirectImportScaleBandTreeItem*>(treeItem);
    }
#endif
  }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Drag Move Event
//
// Keeps hold of an item while it is being dragged.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void LayerTreeView::dragMoveEvent( QDragMoveEvent * event )
{
  if (!event)
  {
    return;
  }

  QModelIndex dropIndex( indexAt( event->pos() ) );

  if( model()->canDropMimeData( event->mimeData(), event->dropAction(), dropIndex.row(), dropIndex.column(), dropIndex.parent() ) )
  {
    QTreeView::dragMoveEvent( event );
  }
  else
  {
    event->ignore();
  }

  clearSelectedLayer();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Context Menu Event
//
// Initialises the context menu when the user right-click's on the
// widget, and sends it to 'showContextMenuForLayer' to work out
// the options available.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void LayerTreeView::contextMenuEvent( QContextMenuEvent* event )
{
  if (!event)
  {
    return;
  }

  // Work out which item we are generating the context menu for
  QModelIndex contextMenuItem = indexAt( event->pos() );
  TreeModel *treeModel = reinterpret_cast<TreeModel*>( model() );
  showContextMenu( treeModel, contextMenuItem, event->globalPos() );
  event->accept();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Context Menu
//
// Right-clicking the LayerTreeView docked widget will activate this
// function. Doing so will display a context menu with different
// options depending on where the user performed this action.
//
// This menu can be used for adding a new layer, removing a layer,
// editing the cache size of a layer, and displaying detailed
// information about the layer in the AttributeTree.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void LayerTreeView::showContextMenu( TreeModel *model, const QModelIndex &layerIndex, const QPoint &menuLocation )
{
  if( layerIndex.row() == -1 ) // Occurs when the user right-clicks the widget away from any data displayed.
  {
    QMenu *contextMenu = new QMenu( this );
    contextMenu->setAttribute( Qt::WA_DeleteOnClose );

    QVariant nodeData( model->data( layerIndex, Qt::UserRole ) );

    QAction *removeAction = contextMenu->addAction( "Add Layer...", this, SLOT( loadLayer() ) );
    removeAction->setData( nodeData );

    contextMenu->popup( menuLocation );
  }
  else // Occurs when the user right-clicks an item in the widget.
  {
    TreeItem* treeItem( reinterpret_cast<TreeItem*>( layerIndex.internalPointer() ) );
    if( !treeItem )
    {
      return;
    }

    TreeItem::TreeItemType itemType( treeItem->type() );
    if( itemType == TreeItem::TreeItemTypeDefault && layerIndex.parent() == model->index( -1, -1 ) )
    {
      // It's a DataLayer
      QMenu *contextMenu = new QMenu( this );
      contextMenu->setAttribute( Qt::WA_DeleteOnClose );

      QVariant nodeData( model->data( layerIndex, Qt::UserRole ) );

      QAction *removeAction = contextMenu->addAction( "Remove Layer", this, SLOT( removeItem() ) );
      removeAction->setData( nodeData );

      contextMenu->addSeparator();

      QAction *loadAttributeAction = contextMenu->addAction( "Display Attribute Data", this, SLOT( loadAttributeData() ) );
      loadAttributeAction->setData( nodeData );

      QAction *zoomToAction = contextMenu->addAction( "Zoom to Extent", this, SLOT( zoomToExtentClicked() ) );

      QAction *editCache = contextMenu->addAction( "Edit Cache Size...", this, SLOT( editCacheSize() ) );
      editCache->setData( nodeData );

      contextMenu->popup( menuLocation );
    }
#ifdef HAVE_DIRECT_IMPORT_SDK
    else if( itemType == TreeItem::TreeItemTypeDirectImportDataSet )
    {
      QMenu* contextMenu = new QMenu( this );
      contextMenu->setAttribute( Qt::WA_DeleteOnClose );

      contextMenu->addAction( "Remove Dataset", this, SLOT( removeDirectImportData() ) );

      contextMenu->addSeparator();

      contextMenu->addAction( "Zoom to Extent", this, SLOT( zoomToExtentClicked() ) );

      contextMenu->addAction( "Edit Dataset Settings...", this, SLOT( editDirectImportData() ) );

      m_dataSetToEdit = dynamic_cast<DirectImportDataSetTreeItem*>( treeItem );

      contextMenu->popup( menuLocation );
    }
#endif
  }
}
