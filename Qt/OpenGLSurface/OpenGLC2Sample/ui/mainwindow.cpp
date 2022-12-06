/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include <QtGui>
#include <QMainWindow>
#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QSpinBox>

#include "MapLink.h"
#include "MapLinkOpenGLSurface.h"
#include "tslapp6ahelper.h"

#undef None
#undef Status
#undef Bool

#include "mainwindow.h"
#include "toolbarspeedcontrol.h"
#include "trackhostilitydelegate.h"
#include "tracknumbers.h"
#include "layers/layermanager.h"
#include "tracks/trackmanager.h"

#include <string>
using namespace std;

// This class is the main window of the application. It receives events from the user and
// passes them to the widget containing the drawing surface

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , m_appRefresh(new QTimer(this))
  , m_toolbarSpeedControl(NULL)
  , m_hostilityDelegate(NULL)
{
  // Construct the window
  setupUi(this);

  // Connect the actions for the toolbars and menus to the slots that deal with them
  connect(actionLoadMap, SIGNAL(triggered()), this, SLOT(loadMap()));
  connect(actionResetView, SIGNAL(triggered()), maplinkSurface, SLOT(resetView()));
  connect(actionZoomMode, SIGNAL(triggered()), maplinkSurface, SLOT(activateZoomMode()));
  connect(actionPanMode, SIGNAL(triggered()), maplinkSurface, SLOT(activatePanMode()));
  connect(actionGrabMode, SIGNAL(triggered()), maplinkSurface, SLOT(activateGrabMode()));
  connect(actionSelectTrack, SIGNAL(triggered()), maplinkSurface, SLOT(activateTrackSelectMode()));
  connect(actionPinTrack, SIGNAL(triggered()), this, SLOT(pinSelectedTrack()));
  connect(actionEnableTrackMotion, SIGNAL(triggered()), this, SLOT(toggleTrackMotion()));
  connect(actionFollowTrack, SIGNAL(toggled(bool)), this, SLOT(toggleFollowTrack(bool)));
  connect(actionViewOrientation, SIGNAL(toggled(bool)), this, SLOT(toggleViewOrientation(bool)));
  connect(actionNumberOfTracks, SIGNAL(triggered()), this, SLOT(changeTrackNumbers()));
  connect(actionSymbolAnnotationNone, SIGNAL(triggered()), this, SLOT(setNoTrackAnnotation()));
  connect(actionSymbolAnnotationLow, SIGNAL(triggered()), this, SLOT(setLowTrackAnnotation()));
  connect(actionSymbolAnnotationMedium, SIGNAL(triggered()), this, SLOT(setMediumTrackAnnotation()));
  connect(actionSymbolAnnotationHigh, SIGNAL(triggered()), this, SLOT(setHighTrackAnnotation()));
  connect(actionSetScale500, SIGNAL(triggered()), this, SLOT(setViewScale500()));
  connect(actionSetScale1000, SIGNAL(triggered()), this, SLOT(setViewScale1000()));
  connect(actionSetScale10000, SIGNAL(triggered()), this, SLOT(setViewScale10000()));
  connect(actionSetScale50000, SIGNAL(triggered()), this, SLOT(setViewScale50000()));
  connect(actionSetScale250000, SIGNAL(triggered()), this, SLOT(setViewScale250000()));
  connect(actionEnableTiledBuffering, SIGNAL(toggled(bool)), this, SLOT(toggleTiledBuffering(bool)));
  connect(actionAPP6A, SIGNAL(triggered()), this, SLOT(setSymbolTypeAPP6A()));
  connect(action2525B, SIGNAL(triggered()), this, SLOT(setSymbolType2525B()));
  connect(m_appRefresh, SIGNAL(timeout()), maplinkSurface, SLOT(update()));
  connect(actionAbout, SIGNAL(triggered()), this, SLOT(showAboutBox()));
  connect(actionExit, SIGNAL(triggered()), this, SLOT(close()));

  // Create a group of actions for the interaction mode buttons and menus so that
  // the active interaction mode is reflected in the toolbar and menu display
  m_interactionModesGroup = new QActionGroup(toolBar);
  m_interactionModesGroup->addAction(actionZoomMode);
  m_interactionModesGroup->addAction(actionPanMode);
  m_interactionModesGroup->addAction(actionGrabMode);
  m_interactionModesGroup->addAction(actionSelectTrack);

  // Create a group of actions for the symbol annotation menu items so that
  // only one is shown checked at a time
  m_symbolAnnotationGroup = new QActionGroup(menuTracks);
  m_symbolAnnotationGroup->addAction(actionSymbolAnnotationNone);
  m_symbolAnnotationGroup->addAction(actionSymbolAnnotationLow);
  m_symbolAnnotationGroup->addAction(actionSymbolAnnotationMedium);
  m_symbolAnnotationGroup->addAction(actionSymbolAnnotationHigh);

  // Create an action group for the symbology type selector
  m_symbolTypeGroup = new QActionGroup( menuSymbologyType );
  m_symbolTypeGroup->addAction( actionAPP6A );
  m_symbolTypeGroup->addAction( action2525B );

  // Add the widget that allows controlling the simulation speed to the toolbar
  m_toolbarSpeedControl = new ToolbarSpeedControl(toolBar);
  toolBar->addWidget( m_toolbarSpeedControl );

  TrackManager::instance().setTrackSelectionCallbacks( this );

  // Set up the models for the controls in the dock widget. These handle mapping MapLink objects
  // onto the UI for display.
  LayerManager::instance().declutterModel().setParent( declutterTree );
  declutterTree->setModel( &LayerManager::instance().declutterModel() );
  LayerManager::instance().declutterModel().setUpdateView( maplinkSurface );

  TrackManager::instance().trackInfoModel().setParent( trackInfoTable );
  trackInfoTable->setModel( &TrackManager::instance().trackInfoModel() );

  TrackManager::instance().pinnedTrackModel().setParent( pinnedTrackDisplay );
  pinnedTrackDisplay->setModel( &TrackManager::instance().pinnedTrackModel() );

  m_hostilityDelegate = new TrackHostilityDelegate( trackInfoTable );
  trackInfoTable->setItemDelegateForRow( TrackInfoModel::TrackHostility, m_hostilityDelegate );

  // Make the information column fill as much of the available space in the table as possible
  trackInfoTable->horizontalHeader()->setSectionResizeMode( 0, QHeaderView::Stretch );
  trackInfoTable->horizontalHeader()->setSectionResizeMode( 1, QHeaderView::ResizeToContents );
  trackInfoTable->verticalHeader()->setSectionResizeMode( QHeaderView::ResizeToContents );

  pinnedTrackDisplay->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch );
  pinnedTrackDisplay->verticalHeader()->setSectionResizeMode( QHeaderView::ResizeToContents );
  pinnedTrackDisplay->installEventFilter( new PinnedTrackModel::DeleteKeyEvent() );

  // Maximize the window on startup
  showMaximized();

  // Disable tiled buffering by default
  // The ui/initial drawing surface setup enables this, so the
  // simplest way to disable it is to trigger the action
  // for the menu entry.
  actionEnableTiledBuffering->trigger();
}

MainWindow::~MainWindow()
{
  // Clean up
  TrackManager::instance().stopTrackUpdates();
  m_appRefresh->stop();

  LayerManager::instance().declutterModel().setParent( NULL );
  declutterTree->setModel( NULL );
  TrackManager::instance().trackInfoModel().setParent( NULL );
  trackInfoTable->setModel( NULL );
  TrackManager::instance().pinnedTrackModel().setParent( NULL );
  pinnedTrackDisplay->setModel( NULL );

  delete m_interactionModesGroup;
  delete m_symbolAnnotationGroup;
  delete m_symbolTypeGroup;
  delete m_appRefresh;
  delete m_toolbarSpeedControl;
  delete m_hostilityDelegate;
}

void MainWindow::loadMap( const char *mapToLoad )
{
  if( LayerManager::instance().loadMap( mapToLoad ) )
  {
    // Remove any existing declutter settings
    maplinkSurface->drawingSurface()->clearAllDeclutterData();

    // As we loaded a new map, we want to reset the surface when it is resized to ensure that the map's full extent is loaded
    maplinkSurface->resetOnResize( true );

    // By default we create 100 tracks of 50 possible types
    TrackManager::instance().createTracks( (quint32)TrackManager::instance().numRequestedTracks(), (quint32)TrackManager::instance().numRequestedTrackTypes() );

    // Enable the simulation controls now we have a loaded map
    toggleMapLoadedControls( true );
    toggleTrackControls( true );
  }
  else
  {
    // Failed to load the map
    TSLSimpleString errorMessage;
    TSLThreadedErrorStack::errorString( errorMessage, NULL, TSLErrorCategoryAll );

    QMessageBox::critical( this, "Failed to load map", QString::fromUtf8(errorMessage.c_str()) );
  }
}

void MainWindow::loadMap()
{
  // Show a file open dialog to let the user choose the map to load - 
  // MapLink Pro maps either have a '.map' or '.mpc' file ending.
  QString fileName = QFileDialog::getOpenFileName(this, tr("Load A Map"), QString(),
                                                  tr("All Map files (*.map *.mpc)"));

  if (!fileName.isEmpty())
  {
    // First clear out any existing tracks
    TrackManager::instance().createTracks( 0, 0 );
    LayerManager::instance().resetLayers( maplinkSurface->drawingSurface() );

    // Load the new map
    TSLThreadedErrorStack::clear();
    if( !LayerManager::instance().loadMap( fileName.toUtf8() ) )
    {
      // The map failed to load, display the error as logged by the error stack
      TSLSimpleString errorMessage;
      TSLThreadedErrorStack::errorString( errorMessage, NULL, TSLErrorCategoryAll );

      QMessageBox::critical( this, "Failed to load map", QString::fromUtf8(errorMessage.c_str()) );

      // As we now have no map loaded, disable the appropriate UI controls
      toggleMapLoadedControls( false );
      toggleTrackControls( false );

      return;
    }

    // By default we create 100 tracks of 50 possible types
    TrackManager::instance().createTracks( (quint32)TrackManager::instance().numRequestedTracks(), (quint32)TrackManager::instance().numRequestedTrackTypes() );
    
    // As we loaded a new map, we want to reset the view to cover its full extent
    actionResetView->activate( QAction::Trigger );

    // Enable the simulation controls now we have a loaded map
    toggleMapLoadedControls( true );
    toggleTrackControls( true );
  }
}

void MainWindow::toggleTrackMotion()
{
  if( actionEnableTrackMotion->isChecked() )
  {
    // We are enabling track motion. Use the special case for a QTimer with a timeout
    // value of 0 to cause the connected slot to activate whenever the application's event queue is empty.
    // This causes the application to update as fast as possible without stopping the application widgets
    // from functioning.

    // Prevent the user from reloading a map while tracks are being updated
    toggleTrackControls( false );
    actionLoadMap->setEnabled( false );
    LayerManager::instance().setFramerateLayerVisibility( maplinkSurface->drawingSurface(), true );
    TrackManager::instance().startTrackUpdates();
    m_appRefresh->start(0);
  }
  else
  {
    // We are disabling track motion
    LayerManager::instance().setFramerateLayerVisibility( maplinkSurface->drawingSurface(), false );
    TrackManager::instance().stopTrackUpdates();
    m_appRefresh->stop();
    toggleTrackControls( true );
    actionLoadMap->setEnabled( true );
    maplinkSurface->update();
  }
}

void MainWindow::toggleFollowTrack( bool followTrack )
{
  TrackManager::instance().enableTrackFollow( followTrack );
  maplinkSurface->update();
}

void MainWindow::toggleViewOrientation( bool trackHeadingUp )
{
  TrackManager::instance().enableTrackUpOrientation( trackHeadingUp );
  maplinkSurface->update();
}

void MainWindow::changeTrackNumbers()
{
  // Show the dialog to let the user change how many tracks there are
  TrackNumbers *trackNumberDialog = new TrackNumbers( this );
  trackNumberDialog->setAttribute( Qt::WA_DeleteOnClose );
  trackNumberDialog->setModal( true );
  trackNumberDialog->show();
}

void MainWindow::setNoTrackAnnotation()
{
  TrackManager::instance().setTrackAnnotationLevel( AnnotationNone );
}

void MainWindow::setLowTrackAnnotation()
{
  TrackManager::instance().setTrackAnnotationLevel( AnnotationLow );
}

void MainWindow::setMediumTrackAnnotation()
{
  TrackManager::instance().setTrackAnnotationLevel( AnnotationMedium );
}

void MainWindow::setHighTrackAnnotation()
{
  TrackManager::instance().setTrackAnnotationLevel( AnnotationHigh );
}

void MainWindow::showAboutBox()
{
  // Display an about box
  QMessageBox::about(this, tr("MapLink Pro OpenGL C2 Sample"),
                           tr("<img src=\":/general/images/envitia.png\"/>"
                              "<p>Copyright &copy; 1998-2014 Envitia Group PLC. All rights reserved.</p>"
#ifdef WIN32
                              "<p align=center style=\"color:#909090\">Portions developed using LEADTOOLS<br>"
                              "Copyright &copy; 1991-2009 LEAD Technologies, Inc. ALL RIGHTS RESERVED</p>"
#endif
                              ));
}


void MainWindow::trackSelectionStatusChanged( bool trackSelected )
{
  // If a track is selected, enable the toolbar controls that depend on it
  actionFollowTrack->setEnabled( trackSelected );
  actionPinTrack->setEnabled( trackSelected );
  actionViewOrientation->setEnabled( trackSelected );

  if( !trackSelected )
  {
    TrackManager::instance().enableTrackFollow( false );
    TrackManager::instance().enableTrackUpOrientation( false );
    actionFollowTrack->setChecked( false );
    actionViewOrientation->setChecked( false );
  }

  // Request a refresh of the display in case the simulation is paused. This is necessary
  // to show the selection box for the currently selected track
  maplinkSurface->update();

  // Refresh the contents of the track information table with the new selection
  TrackManager::instance().trackInfoModel().reloadData();
  trackInfoTable->update();
}

void MainWindow::pinSelectedTrack()
{
  TrackManager::instance().pinnedTrackModel().pinSelectedTrack();
}

void MainWindow::toggleTrackControls( bool enable )
{
  actionAPP6A->setEnabled( enable );
  action2525B->setEnabled( enable );
}

void MainWindow::toggleMapLoadedControls( bool enable )
{
  m_toolbarSpeedControl->setEnabled( enable );
  actionEnableTrackMotion->setEnabled( enable );
  actionResetView->setEnabled( enable );
  actionZoomMode->setEnabled( enable );
  actionPanMode->setEnabled( enable );
  actionGrabMode->setEnabled( enable );
  actionSelectTrack->setEnabled( enable );
  actionSetScale500->setEnabled( enable );
  actionSetScale1000->setEnabled( enable );
  actionSetScale10000->setEnabled( enable );
  actionSetScale50000->setEnabled( enable );
  actionSetScale250000->setEnabled( enable );
  actionSymbolAnnotationNone->setEnabled( enable );
  actionSymbolAnnotationLow->setEnabled( enable );
  actionSymbolAnnotationMedium->setEnabled( enable );
  actionSymbolAnnotationHigh->setEnabled( enable );
  actionNumberOfTracks->setEnabled( enable );
}

void MainWindow::setViewScale500()
{
  setViewScale( 1.0 / 500.0 );
}

void MainWindow::setViewScale1000()
{
  setViewScale( 1.0 / 1000.0 );
}

void MainWindow::setViewScale10000()
{
  setViewScale( 1.0 / 10000.0 );
}

void MainWindow::setViewScale50000()
{
  setViewScale( 1.0 / 50000.0 );
}

void MainWindow::setViewScale250000()
{
  setViewScale( 1.0 / 250000.0 );
}

void MainWindow::setViewScale( double scaleFactor )
{
  // Calculate the correct UU extent for the drawing surface in order to make
  // the view the requested scale without changing the viewing position.
  double tmcPerMU = maplinkSurface->drawingSurface()->TMCperMU();
  double tmcPerDUX, tmcPerDUY;
  maplinkSurface->drawingSurface()->TMCperDU( tmcPerDUX, tmcPerDUY );

  double duPerMUX = tmcPerMU / tmcPerDUX;
  double duPerMUY = tmcPerMU / tmcPerDUY;

  // Ask the drawing surface for the physical size of the display
  int displayHorizontalMM, displayVerticalMM, width, height;
  maplinkSurface->drawingSurface()->getDeviceCapabilities( displayHorizontalMM, displayVerticalMM, width, height );
  double pixelsPerMeterX = width / (displayHorizontalMM / 1000.0);
  double pixelsPerMeterY = height / (displayVerticalMM / 1000.0);

  // Scale the currently displayed extent to match the requested scale
  double kX = duPerMUX / (pixelsPerMeterX * scaleFactor);
  double kY = duPerMUY / (pixelsPerMeterY * scaleFactor);

  TSLEnvelope extent;
  maplinkSurface->drawingSurface()->getTMCExtent( extent );
  extent.scale(kX, kY);

  // Set the view of the drawing surface to match the requested scale
  double uux1, uuy1, uux2, uuy2;
  maplinkSurface->drawingSurface()->TMCToUU( extent.bottomLeft().x(), extent.bottomLeft().y(), &uux1, &uuy1 );
  maplinkSurface->drawingSurface()->TMCToUU( extent.topRight().x(), extent.topRight().y(), &uux2, &uuy2 );
  maplinkSurface->drawingSurface()->resize( uux1, uuy1, uux2, uuy2, false, true );
  maplinkSurface->update();
}

void MainWindow::toggleTiledBuffering( bool enable )
{
  // Enables or disables tiling of buffered layers for comparison
  maplinkSurface->drawingSurface()->setOption( TSLOptionTileBufferedLayers, enable );
  maplinkSurface->update();
}

void MainWindow::setSymbolTypeAPP6A()
{
  const char *maplHome = TSLUtilityFunctions::getMapLinkHome();
  if( !maplHome )
  {
    QMessageBox::critical( this, "Cannot load APP6A Symbol configuration file",
                           "This sample requires the MAPL_HOME environmental variable to be set" );
  }
  else
  {
    QString configLocation( QString::fromUtf8(maplHome) );
    configLocation += "/config/app6aConfig.csv";
    TrackManager::instance().loadSymbolConfig( configLocation );
  }
}

void MainWindow::setSymbolType2525B()
{
  const char *maplHome = TSLUtilityFunctions::getMapLinkHome();
  if( !maplHome )
  {
    QMessageBox::critical( this, "Cannot load 2525B Symbol configuration file",
                           "This sample requires the MAPL_HOME environmental variable to be set" );
  }
  else
  {
    QString configLocation( QString::fromUtf8(maplHome) );
    configLocation += "/config/2525bConfig.csv";
    TrackManager::instance().loadSymbolConfig( configLocation );
  }
}
