/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include <algorithm>
#include <QMenu>
#include <QContextMenuEvent>

#include "servicetreeview.h"
#include "editdimensiondialog.h"
#include "mainwindow.h"
#include "layerpropertiesdialog.h"
#include "services/servicelistmodel.h"
#include "services/servicelist.h"

using namespace Services;

static bool caseInsensitiveLess( const char &lhs, const char &rhs )
{
  return toupper( lhs ) < toupper( rhs );
}

static bool caseInsenstitiveSort( const std::string &lhs, const std::string &rhs )
{
  // string::compare is case sensitive, so provide a case insensitive function to compare each character
  return std::lexicographical_compare( lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
                                       caseInsensitiveLess );
}

ServiceTreeView::ServiceTreeView( QWidget *parent )
  : QTreeView( parent )
{

}

ServiceTreeView::~ServiceTreeView()
{
}

void ServiceTreeView::contextMenuEvent(QContextMenuEvent * event)
{
  // Work out which item we are generating the context menu for
  QModelIndex contextMenuItem = indexAt( event->pos() );

  ServiceListModel *serviceModel = reinterpret_cast< ServiceListModel* >( model() );
  if( serviceModel )
  {
    switch( serviceModel->getNodeTypeForIndex( contextMenuItem ) )
    {
    case ServiceListModel::NodeTypeService:
      showContextMenuForService( serviceModel, contextMenuItem, event->globalPos() );
      event->accept();
      return;

    case ServiceListModel::NodeTypeLayer:
      showContextMenuForLayer( serviceModel, contextMenuItem, event->globalPos() );
      event->accept();
      return;

    default:
      break;
    }
  }

  // If we haven't handled the event already, ignore it as it doesn't apply
  event->ignore();
}

void ServiceTreeView::dragMoveEvent(QDragMoveEvent * event)
{
  QModelIndex dropIndex( indexAt( event->pos() ) );
  if( model()->canDropMimeData( event->mimeData(), event->dropAction(), dropIndex.row(), dropIndex.column(), dropIndex.parent() ) )
  {
    QTreeView::dragMoveEvent( event );
  }
  else
  {
    event->ignore();
  }
}

void ServiceTreeView::removeItem()
{
  // Remove the currently selected service - this effectively deletes the service and its enclosed data layer
  QObject *activatedItem = sender();
  if( activatedItem )
  {
    QAction *sourceAction = qobject_cast< QAction* >( activatedItem );
    if( sourceAction )
    {
      ServiceListModel *serviceModel = reinterpret_cast< ServiceListModel* >( model() );
      serviceModel->removeItem( sourceAction->data() );
    }
  }
}

void ServiceTreeView::showLayerOnService()
{
  // Tell the service to make the layer the user selected from the menu visible
  QObject *activatedItem = sender();
  if( activatedItem )
  {
    QAction *sourceAction = qobject_cast< QAction* >( activatedItem );
    if( sourceAction )
    {
      ServiceListModel *serviceModel = reinterpret_cast< ServiceListModel* >( model() );
      // The name of the layer to show is contained in the text of the action that triggered this slot
      serviceModel->setLayerVisibility( sourceAction->data(), sourceAction->text(), true );
    }
  }
}

void ServiceTreeView::changeDimensionValue()
{
  QObject *activatedItem = sender();
  if( activatedItem )
  {
    QAction *sourceAction = qobject_cast< QAction* >( activatedItem );
    if( sourceAction )
    {
      ServiceListModel *serviceModel = reinterpret_cast< ServiceListModel* >( model() );
      const char *units = serviceModel->getLayerDimensionUnits( sourceAction->data(), sourceAction->text() );
      Service *service = serviceModel->getService( sourceAction->data() );
      if( !service )
      {
        // TODO error
      }

      EditDimensionDialog *dimensionsEdit = new EditDimensionDialog( serviceModel->serviceList(), service, sourceAction->text(), units, 
                                                                     MainWindow::mainWindowInstance() );
      dimensionsEdit->setAttribute( Qt::WA_DeleteOnClose );
      dimensionsEdit->setModal( true );
      dimensionsEdit->show();
    }
  }
}

void ServiceTreeView::changeStyleValue()
{
  QObject *activatedItem = sender();
  if( activatedItem )
  {
    QAction *sourceAction = qobject_cast< QAction* >( activatedItem );
    if( sourceAction )
    {
      ServiceListModel *serviceModel = reinterpret_cast< ServiceListModel* >( model() );
      Service *service = serviceModel->getService( sourceAction->data() );
      if( !service )
      {
        // TODO error
      }

      serviceModel->setLayerStyle( sourceAction->data(), sourceAction->text() );
    }
  }
}

void ServiceTreeView::zoomToSelection()
{
  QObject *activatedItem = sender();
  if( activatedItem )
  {
    QAction *sourceAction = qobject_cast< QAction* >( activatedItem );
    if( sourceAction )
    {
      ServiceListModel *serviceModel = reinterpret_cast< ServiceListModel* >( model() );

      TSLTMC x1, y1, x2, y2;
      if( serviceModel->getItemTMCExtent( sourceAction->data(), x1, y1, x2, y2 ) )
      {
        serviceModel->serviceList()->setViewedExtent( x1, y1, x2, y2 );
      }
    }
  }
}

void ServiceTreeView::setServiceProperties()
{
  QObject *activatedItem = sender();
  if( activatedItem )
  {
    QAction *sourceAction = qobject_cast< QAction* >( activatedItem );
    if( sourceAction )
    {
      ServiceListModel *serviceModel = reinterpret_cast< ServiceListModel* >( model() );
      Service *service = serviceModel->getService( sourceAction->data() );
      if( !service )
      {
        // TODO error
      }

      LayerPropertiesDialog *propertiesDialog = new LayerPropertiesDialog( serviceModel->serviceList(), service,
                                                                           MainWindow::mainWindowInstance() );
      propertiesDialog->setAttribute( Qt::WA_DeleteOnClose );
      propertiesDialog->setModal( true );
      propertiesDialog->show();
    }
  }
}

void ServiceTreeView::showContextMenuForService( ServiceListModel *model, const QModelIndex &serviceIndex, const QPoint &menuLocation )
{
  QMenu *contextMenu = new QMenu( this );
  contextMenu->setAttribute( Qt::WA_DeleteOnClose );

  QAction *removeAction = contextMenu->addAction( "Remove service", this, SLOT(removeItem()) );
  // Use the 'user role' to store some model-specific data on the action that we can pass back to the model
  // in the slot so the model knows which item to act on
  QVariant nodeData( model->data( serviceIndex, Qt::UserRole ) );
  removeAction->setData( nodeData );

  // Add a sub-menu showing the layers that could be shown but currently aren't by this service
  std::vector< std::string > layerDisplayNames;
  model->getPotentialLayersForDisplay( serviceIndex, layerDisplayNames );

  if( !layerDisplayNames.empty() )
  {
    // Sort the layer names so they're in alpabetical order, rather than whatever order the service returned them in
    sort( layerDisplayNames.begin(), layerDisplayNames.end(), caseInsenstitiveSort );

    QMenu *layerSubMenu = contextMenu->addMenu( "Show layer" );

    size_t numInvisibleLayers = layerDisplayNames.size();
    for( size_t i = 0; i < numInvisibleLayers; ++i )
    {
      QAction *layerAddAction = layerSubMenu->addAction( layerDisplayNames[i].c_str(), this, SLOT(showLayerOnService()) );
      layerAddAction->setData( nodeData );
    }
  }

  QAction *zoomAction = contextMenu->addAction( "Zoom to", this, SLOT(zoomToSelection()) );
  zoomAction->setData( nodeData );

  QAction *propertiesAction = contextMenu->addAction( "Properties", this, SLOT(setServiceProperties()) );
  propertiesAction->setData( nodeData );

  contextMenu->popup( menuLocation );
}

void ServiceTreeView::showContextMenuForLayer( ServiceListModel *model, const QModelIndex &layerIndex, const QPoint &menuLocation )
{
  QMenu *contextMenu = new QMenu( this );
  contextMenu->setAttribute( Qt::WA_DeleteOnClose );

  QVariant nodeData( model->data( layerIndex, Qt::UserRole ) );

  // Add a submenu offering the ability to change the value of each of the layer's dimensions
  std::vector< std::string > layerDimensions;
  model->getLayerDimensionNames( layerIndex, layerDimensions );
  if( !layerDimensions.empty() )
  {
    QMenu *dimensionSubMenu = contextMenu->addMenu( "Change Dimension Value" );
    size_t numDimensions = layerDimensions.size();

    for( size_t i = 0; i < numDimensions; ++i )
    {
      QAction *dimensionAction = dimensionSubMenu->addAction( layerDimensions[i].c_str(), this, SLOT(changeDimensionValue()) );
      dimensionAction->setData( nodeData );
    }
  }

  // Offer a submenu offering the ability to change the current style of the layer
  std::vector< std::string > layerStyles;
  model->getLayerStyleNames( layerIndex, layerStyles );
  if( !layerStyles.empty() )
  {
    QMenu *styleSubMenu = contextMenu->addMenu( "Change Style" );
    size_t numStyles = layerStyles.size();

    for( size_t i = 0; i < numStyles; ++i )
    {
      QAction *styleAction = styleSubMenu->addAction( layerStyles[i].c_str(), this, SLOT(changeStyleValue()) );
      styleAction->setData( nodeData );
    }
  }

  QAction *removeAction = contextMenu->addAction( "Hide layer", this, SLOT(removeItem()) );
  removeAction->setData( nodeData );

  QAction *zoomAction = contextMenu->addAction( "Zoom to", this, SLOT(zoomToSelection()) );
  zoomAction->setData( nodeData );

  contextMenu->popup( menuLocation );
}
