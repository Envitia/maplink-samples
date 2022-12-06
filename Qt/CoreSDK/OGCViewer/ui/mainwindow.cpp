/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include <QtGui>
#include <QMainWindow>
#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>

#include "mainwindow.h"
#include "servicewizard.h"
#include "generaloptionsdialog.h"

#ifdef HAVE_QWEBVIEW
# include "helpdialog.h"
#endif

#include "services/servicelist.h"
#include "services/servicelistmodel.h"

#include "MapLinkDrawing.h"

#include <string>
using namespace std;
using namespace Services;

static QMainWindow *g_mainWindowInstance = NULL;

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , m_services( new ServiceList() )
#ifdef HAVE_QWEBVIEW  
  , m_helpDialog( NULL )
#endif
{
  // Construct the window
  setupUi(this);

  g_mainWindowInstance = this;

  // Connect the actions for the toolbars and menus to the slots that deal with them
  connect(actionResetView, SIGNAL(triggered()), maplinkSurface, SLOT(resetView()));
  connect(actionZoomIn, SIGNAL(triggered()), maplinkSurface, SLOT(zoomIn()));
  connect(actionZoomOut, SIGNAL(triggered()), maplinkSurface, SLOT(zoomOut()));
  connect(actionZoomMode, SIGNAL(triggered()), maplinkSurface, SLOT(activateZoomMode()));
  connect(actionPanMode, SIGNAL(triggered()), maplinkSurface, SLOT(activatePanMode()));
  connect(actionGrabMode, SIGNAL(triggered()), maplinkSurface, SLOT(activateGrabMode()));

  connect(actionAddService, SIGNAL(triggered()), this, SLOT(showServiceWizard()));
  connect(this, SIGNAL(signalSetLoadingAnimationState(bool)), this, SLOT(setLoadingAnimationState(bool)) );

  connect(zoomToButton, SIGNAL(clicked(bool)), this, SLOT(zoomViewToLayer()));

  connect(actionAbout, SIGNAL(triggered()), this, SLOT(showAboutBox()));
  connect(actionHelp, SIGNAL(triggered()), this, SLOT(showHelp()));
  connect(actionOptions, SIGNAL(triggered()), this, SLOT(showGeneralOptions()));
  connect(actionExit, SIGNAL(triggered()), this, SLOT(close()));

  connect(maplinkSurface, SIGNAL(mapDrawn()), this, SLOT(update()));

  // Create a group of actions for the interaction mode buttons and menus so that
  // the active interaction mode is reflected in the toolbar and menu display
  m_interactionModesGroup = new QActionGroup(toolBar);
  m_interactionModesGroup->addAction(actionZoomMode);
  m_interactionModesGroup->addAction(actionPanMode);
  m_interactionModesGroup->addAction(actionGrabMode);

  // Set up the status bar
  m_dataLoadingAnimation = new QMovie( ":/animations/images/splash_loader.gif" );
  m_dataLoadingLabel = new QLabel();
  m_dataLoadingLabel->setMovie( m_dataLoadingAnimation );
  m_dataLoadingLabel->setVisible( false );

  m_mapUnitCursorPosition = new QLabel();
  m_latLonCursorPosition = new QLabel();

  statusBar()->addPermanentWidget( m_dataLoadingLabel );
  statusBar()->addWidget( m_mapUnitCursorPosition );
  statusBar()->addWidget( m_latLonCursorPosition );

  // Inform the drawing surface widget of the status bar labels it should update with the current
  // cursor position
  maplinkSurface->setStatusBarMUWidget( m_mapUnitCursorPosition );
  maplinkSurface->setStatusBarlatLonWidget( m_latLonCursorPosition );

  // Tell the service list which drawing surface it should make the data layers visible in
  m_services->setDrawingSurface( maplinkSurface );

  // Ask the service list to forward load calls from the file loader to us so we can start and stop
  // the status bar animation
  m_services->setLoadCallbackForwards( &MainWindow::loadCallback, this, &MainWindow::allLoadedCallback, this );

  // Set the display model for the loaded services onto the dockable tree view
  loadedServicesTree->setModel( m_services->getDisplayModel() );

  readSettings();
}

MainWindow::~MainWindow()
{
  // Clean up
  delete m_interactionModesGroup;
  delete m_dataLoadingLabel;
  delete m_dataLoadingAnimation;
  delete m_mapUnitCursorPosition;
  delete m_latLonCursorPosition;
  delete m_services;
#ifdef HAVE_QWEBVIEW  
  delete m_helpDialog;
#endif
}

void MainWindow::showServiceWizard()
{
  TSLDrawingSurface *surface = maplinkSurface->drawingSurface();
  TSLDataLayer *coordinateProvidingLayer = NULL;
  if( surface )
  {
    coordinateProvidingLayer = surface->getCoordinateProvidingLayer();
  }

  // This will be deleted when the wizard is closed
  ServiceWizard *wizard = new ServiceWizard( m_services, coordinateProvidingLayer, this );
  wizard->setAttribute( Qt::WA_DeleteOnClose );
  wizard->setModal(true);
  wizard->show();
}

void MainWindow::setLoadingAnimationState( bool running )
{
  if( running && m_dataLoadingAnimation->state() != QMovie::Running )
  {
    m_dataLoadingLabel->setVisible( true );
    m_dataLoadingAnimation->start();
  }
  else if( !running && m_dataLoadingAnimation->state() != QMovie::NotRunning )
  {
    m_dataLoadingLabel->setVisible( false );
    m_dataLoadingAnimation->stop();
  }
}

void MainWindow::zoomViewToLayer()
{
  // Ask the model to give us the MU extent of the currently selected layer
  QModelIndex selectedTreeItem = loadedServicesTree->currentIndex();
  if( !selectedTreeItem.isValid() )
  {
    QMessageBox::information( this, tr("No service/layer selected"), tr("A layer or service must be selected first."),
                                QMessageBox::Ok );
    return;
  }

  ServiceListModel *serviceModel = reinterpret_cast< ServiceListModel* >( loadedServicesTree->model() );
  if( serviceModel )
  {
    TSLTMC x1 = 0.0, y1 = 0.0, x2 = 0.0, y2 = 0.0;
    if( serviceModel->getItemTMCExtent( selectedTreeItem, x1, y1, x2, y2 ) )
    {
      serviceModel->serviceList()->setViewedExtent( x1, y1, x2, y2 );
    }
    else
    {
      QMessageBox::information( this, tr("Cannot zoom to layer"), tr("Selected layer or service does not have any extent information."),
                                QMessageBox::Ok );
    }
  }
}

void MainWindow::closeEvent(QCloseEvent* /*event*/)
{
#ifdef HAVE_QWEBVIEW  
  if( m_helpDialog )
  {
    delete m_helpDialog; 
    m_helpDialog = NULL;
  }
#endif

  // Save persistent widget information
  writeSettings();
}

void MainWindow::showAboutBox()
{
    // Display an about box
  QMessageBox::about(this, tr("Envitia WMS/WMTS Viewer"),
                           tr("<img src=\":/icons/images/envitia.png\"/>"
                              "<p>Copyright &copy; 1998-2014 Envitia Group PLC. All rights reserved.</p>"
#ifndef HAVE_QWEBVIEW
                              "<p>Help for this sample may be found at [MapLink Installation]/Samples/Qt/OGCViewer/doc/index.html"
#endif
#ifdef WIN32
                              "<p align=center style=\"color:#909090\">Portions developed using LEADTOOLS<br>"
                              "Copyright &copy; 1991-2009 LEAD Technologies, Inc. ALL RIGHTS RESERVED</p>"
#endif
                              ));
}

void MainWindow::showGeneralOptions()
{
  GeneralOptionsDialog *optionsDialog = new GeneralOptionsDialog( m_services, this );
  optionsDialog->setAttribute( Qt::WA_DeleteOnClose );
  optionsDialog->show();
}

void MainWindow::showHelp()
{
#ifdef HAVE_QWEBVIEW  
  if( !m_helpDialog )
  {
    m_helpDialog = new HelpDialog( this );
  }
  m_helpDialog->show();
#endif
}

TSLLoaderCallbackReturn MainWindow::loadCallback( void* arg, const char* /*filename*/, TSLEnvelope extent, TSLLoaderStatus status, int percentDone )
{
  MainWindow *window = reinterpret_cast< MainWindow* >( arg );
  if( status == TSLLoadingOK && percentDone != 100 )
  {
    emit window->signalSetLoadingAnimationState( true );
  }
  return TSLContinue;
}

void MainWindow::allLoadedCallback( void *arg )
{
  MainWindow *window = reinterpret_cast< MainWindow* >( arg );
  emit window->signalSetLoadingAnimationState( false );
}

void MainWindow::writeSettings()
{
  QSettings settings;
  settings.setValue( "mainwindow/size", size() );
  settings.setValue( "mainwindow/pos", pos() );
}

void MainWindow::readSettings()
{
  QSettings settings;

  QVariant positionVariant = settings.value( "mainwindow/pos" );
  QVariant sizeVariant = settings.value( "mainwindow/size" );

  if( sizeVariant.isValid() )
  {
    resize( sizeVariant.toSize() );
  }
  if( positionVariant.isValid() )
  {
    move( positionVariant.toPoint() );
  }
}

QMainWindow* MainWindow::mainWindowInstance()
{
  return g_mainWindowInstance;
}
