/****************************************************************************
Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include <iostream>

#include <QtGui>
#include <QMainWindow>
#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QSpinBox>
#include <QActionGroup>
#include <QLabel>
#include <QProgressDialog>
#include <QStatusBar>
#include <QTimer>

#include "ui/toolbarspeedcontrol.h"
#include "ui/tracks/trackhostilitydelegate.h"
#include "ui/tracks/tracknumbers.h"

#include "ui/layertreeview/treemodel.h"
#include "ui/layertreeview/treeitem.h"

#include "ui/cachesizedialog.h"
#include "ui/urldialog.h"

#include "mainwindow.h"
#include "layermanager.h"
#include "tracks/trackmanager.h"

#include "MapLinkDrawing.h"
#include "application.h"

#ifdef HAVE_DIRECT_IMPORT_SDK
# include "MapLinkDirectImport.h"
# include "ui/directimportwizard/wizardpageenum.h"
# include "ui/directimportwizard/directimportwizard.h"
# include "ui/layertreeview/directimportscalebandtreeitem.h"
# include "ui/layertreeview/directimportdatasettreeitem.h"
#endif

#include <string>
using namespace std;

// This class is the main window of the application. It receives events from the user and
// passes them to the widget containing the drawing surface

// Constructor
MainWindow::MainWindow( QWidget *parent )
  : QMainWindow( parent )
#ifdef HAVE_DIRECT_IMPORT_SDK
  , m_directImportWizard( NULL )
#endif
  , m_tmcCursorPosition( new QLabel() )
  , m_mapUnitCursorPosition( new QLabel() )
  , m_latLonCursorPosition( new QLabel() )
  , m_scaleBox( new QComboBox() )
  , m_rangeBox( new QComboBox() )
  , m_projectionToolbar( new QToolBar( this ) )
  , m_trackToolbar( new QToolBar( this ) )
  , m_entitiesToolbar( new QToolBar( this ) )
  , m_geodeticToolbar( new QToolBar( this ) )
  , m_simulationToolbar( new QToolBar( this ) )
  , m_zoomRangeToolbar( new QToolBar( this ) )
  , m_cacheSizeDialog( new CacheSizeDialog( this ) )
  , m_urlDialog( new URLDialog( this ) )
  , m_toolbarSpeedControl( NULL )
  , m_hostilityDelegate( NULL )
{
  // Construct the window
  setupUi( this );

  // Add widgets to the status bar at the bottom of the application.
  statusBar()->addWidget( m_tmcCursorPosition );
  statusBar()->addWidget( m_mapUnitCursorPosition );
  statusBar()->addWidget( m_latLonCursorPosition );

  // Inform the drawing surface widget of the status bar labels it should update with the current
  // cursor position
  mapLinkWidget->setStatusBarTMCWidget( m_tmcCursorPosition );
  mapLinkWidget->setStatusBarMUWidget( m_mapUnitCursorPosition );
  mapLinkWidget->setStatusBarlatLonWidget( m_latLonCursorPosition );

  // Inform the drawing surface widget of the combo boxes in the toolbar so they can
  // update them with the current zoom levels and ranges.
  mapLinkWidget->setZoomScaleComboBox( m_scaleBox );
  mapLinkWidget->setViewRangeComboBox( m_rangeBox );

  // Set the minimum width of the status bar widgets so that they don't overlap each other.
  m_tmcCursorPosition->setMinimumWidth( 260 );
  m_mapUnitCursorPosition->setMinimumWidth( 335 );
  m_latLonCursorPosition->setMinimumWidth( 300 );

  // Connect the actions for the toolbars and menus to the slots that deal with them
  connect( action_Open, SIGNAL( triggered() ), this, SLOT( loadLayerDialog() ) );
  connect( actionOpen_URL, SIGNAL( triggered() ), this, SLOT( loadURLDialog() ) );
  connect( actionExit, SIGNAL( triggered() ), this, SLOT( exit() ) );

  connect( actionReset, SIGNAL( triggered() ), this, SLOT( resetView() ) );
  connect( actionZoom_In, SIGNAL( triggered() ), this, SLOT( zoomInOnce() ) );
  connect( actionZoom_Out, SIGNAL( triggered() ), this, SLOT( zoomOutOnce() ) );

  connect( actionZoom_Mode, SIGNAL( triggered() ), this, SLOT( activateZoomMode() ) );
  connect( actionPan_Mode, SIGNAL( triggered() ), this, SLOT( activatePanMode() ) );
  connect( actionGrab_Mode, SIGNAL( triggered() ), this, SLOT( activateGrabMode() ) );
  connect( actionChange_Destination_Waypoint, SIGNAL( triggered() ), this, SLOT( activateWaypointMode() ) );
  connect( actionSelectTrack, SIGNAL( triggered() ), this, SLOT( activateTrackSelectMode() ) );

  // Projection type menu items
  connect( actionStereographic_WGS84, SIGNAL( triggered() ), this, SLOT( setProjectionStereographicWGS84() ) );
  connect( actionGnomic_Spherical_Earth, SIGNAL( triggered() ), this, SLOT( setProjectionGnomicSphericalEarth() ) );
  connect( actionTransverse_Mercator_WGS84, SIGNAL( triggered() ), this, SLOT( setProjectionTransverseMercatorWGS84() ) );
  connect( actionMercator, SIGNAL( triggered() ), this, SLOT( setProjectionMercator() ) );
  connect( actionLock_Projection_Origin, SIGNAL( triggered() ), this, SLOT( lockProjectionOrigin() ) );

  // Time acceleration menu items
  connect( actionReal_Time, SIGNAL( triggered() ), this, SLOT( setTimeAcceleration1x() ) );
  connect( action10x, SIGNAL( triggered() ), this, SLOT( setTimeAcceleration10x() ) );
  connect( action100x, SIGNAL( triggered() ), this, SLOT( setTimeAcceleration100x() ) );
  connect( action1_000x, SIGNAL( triggered() ), this, SLOT( setTimeAcceleration1000x() ) );
  connect( action10_000x, SIGNAL( triggered() ), this, SLOT( setTimeAcceleration10000x() ) );

  // Tracks
  connect( actionPinTrack, SIGNAL( triggered() ), this, SLOT( pinSelectedTrack() ) );
  connect( actionEnableTrackMotion, SIGNAL( triggered() ), this, SLOT( toggleTrackMotion() ) );
  connect( actionFollowTrack, SIGNAL( toggled( bool ) ), this, SLOT( toggleFollowTrack( bool ) ) );
  connect( actionViewOrientation, SIGNAL( toggled( bool ) ), this, SLOT( toggleViewOrientation( bool ) ) );
  connect( actionNumberOfTracks, SIGNAL( triggered() ), this, SLOT( changeTrackNumbers() ) );
  connect( actionAnnotationNone, SIGNAL( triggered() ), this, SLOT( setNoTrackAnnotation() ) );
  connect( actionAnnotationLow, SIGNAL( triggered() ), this, SLOT( setLowTrackAnnotation() ) );
  connect( actionAnnotationMedium, SIGNAL( triggered() ), this, SLOT( setMediumTrackAnnotation() ) );
  connect( actionAnnotationHigh, SIGNAL( triggered() ), this, SLOT( setHighTrackAnnotation() ) );
  connect( actionAPP6A, SIGNAL( triggered() ), this, SLOT( setSymbolTypeAPP6A() ) );
  connect( action2525B, SIGNAL( triggered() ), this, SLOT( setSymbolType2525B() ) );

  // Help/about menu items
  connect( actionAbout, SIGNAL( triggered() ), this, SLOT( showAboutBox() ) );
  connect( actionDocumentation, SIGNAL( triggered() ), this, SLOT( openDocumentation() ) );

  // Window docking menu items
  connect( actionAttribute_Dock, SIGNAL( triggered() ), this, SLOT( toggleAttributeDock() ) );
  connect( actionLayer_Dock, SIGNAL( triggered() ), this, SLOT( toggleLayerDock() ) );
  connect( actionTrack_Dock, SIGNAL( triggered() ), this, SLOT( toggleTrackDock() ) );

  // Geometry menu items
  connect( actionLine, SIGNAL( triggered() ), this, SLOT( selectPolyline() ) );
  connect( actionFont, SIGNAL( triggered() ), this, SLOT( selectText() ) );
  connect( actionPolygon, SIGNAL( triggered() ), this, SLOT( selectPolygon() ) );
  connect( actionVectorSymbol, SIGNAL( triggered() ), this, SLOT( selectVectorSymbol() ) );
  connect( actionRasterSymbol, SIGNAL( triggered() ), this, SLOT( selectRasterSymbol() ) );
  connect( actionArc, SIGNAL( triggered() ), this, SLOT( selectArc() ) );
  connect( actionEllipse, SIGNAL( triggered() ), this, SLOT( selectEllipse() ) );
  connect( actionSaveToTMF, SIGNAL( triggered() ), this, SLOT( saveToTMF() ) );
  connect( actionLoadFromTMF, SIGNAL( triggered() ), this, SLOT( loadFromTMF() ) );

  // Geodetic geometry menu items
  connect( actionGeoLine, SIGNAL( triggered() ), this, SLOT( selectGeoPolyline() ) );
  connect( actionGeoFont, SIGNAL( triggered() ), this, SLOT( selectGeoText() ) );
  connect( actionGeoPolygon, SIGNAL( triggered() ), this, SLOT( selectGeoPolygon() ) );
  connect( actionGeoVectorSymbol, SIGNAL( triggered() ), this, SLOT( selectGeoVectorSymbol() ) );
  connect( actionGeoRasterSymbol, SIGNAL( triggered() ), this, SLOT( selectGeoRasterSymbol() ) );
  connect( actionGeoArc, SIGNAL( triggered() ), this, SLOT( selectGeoArc() ) );
  connect( actionGeoEllipse, SIGNAL( triggered() ), this, SLOT( selectGeoEllipse() ) );

  // Create a group of actions for the interaction mode buttons and menus so that
  // the active interaction mode is reflected in the toolbar and menu display
  m_interactionModesGroup = new QActionGroup( mainToolBar );
  m_interactionModesGroup->addAction( actionZoom_Mode );
  m_interactionModesGroup->addAction( actionPan_Mode );
  m_interactionModesGroup->addAction( actionGrab_Mode );
  m_interactionModesGroup->addAction( actionChange_Destination_Waypoint );
  m_interactionModesGroup->addAction( actionSelectTrack );

  // Create a group of actions for the symbol annotation menu items so that
  // only one is shown checked at a time
  m_symbolAnnotationGroup = new QActionGroup( menuTracks );
  m_symbolAnnotationGroup->addAction( actionAnnotationNone );
  m_symbolAnnotationGroup->addAction( actionAnnotationLow );
  m_symbolAnnotationGroup->addAction( actionAnnotationMedium );
  m_symbolAnnotationGroup->addAction( actionAnnotationHigh );

  // Create an action group for the symbology type selector
  m_symbolTypeGroup = new QActionGroup( menuSymbology_Type );
  m_symbolTypeGroup->addAction( actionAPP6A );
  m_symbolTypeGroup->addAction( action2525B );

  // Create a group of actions for the time acceleration menu entries
  m_timeAccelerationGroup = new QActionGroup( mainToolBar );
  m_timeAccelerationGroup->addAction( actionReal_Time );
  m_timeAccelerationGroup->addAction( action10x );
  m_timeAccelerationGroup->addAction( action100x );
  m_timeAccelerationGroup->addAction( action1_000x );
  m_timeAccelerationGroup->addAction( action10_000x );

  // Create a group of actions for the projection type menu entries
  m_projectionGroup = new QActionGroup( mainToolBar );
  m_projectionGroup->addAction( actionStereographic_WGS84 );
  m_projectionGroup->addAction( actionGnomic_Spherical_Earth );
  m_projectionGroup->addAction( actionTransverse_Mercator_WGS84 );
  m_projectionGroup->addAction( actionMercator );

  // Create a group of actions for the entities & geodetic entities
  m_entityGroup = new QActionGroup( mainToolBar );
  m_entityGroup->addAction( actionLine );
  m_entityGroup->addAction( actionFont );
  m_entityGroup->addAction( actionPolygon );
  m_entityGroup->addAction( actionVectorSymbol );
  m_entityGroup->addAction( actionRasterSymbol );
  m_entityGroup->addAction( actionArc );
  m_entityGroup->addAction( actionEllipse );

  // Geodetic entities
  m_entityGroup->addAction( actionGeoLine );
  m_entityGroup->addAction( actionGeoFont );
  m_entityGroup->addAction( actionGeoPolygon );
  m_entityGroup->addAction( actionGeoVectorSymbol );
  m_entityGroup->addAction( actionGeoRasterSymbol );
  m_entityGroup->addAction( actionGeoArc );
  m_entityGroup->addAction( actionGeoEllipse );

  // Pass references 
  layerTreeView->setModel( mapLinkWidget->getTreeModel() );
  mapLinkWidget->setAttributeTree( attributeTreeWidget );

  // Various connections to other classes
  connect( mapLinkWidget->getTreeModel(), &TreeModel::moveLayerToIndex, this, &MainWindow::moveLayerToIndex );
  connect( mapLinkWidget->getTreeModel(), &TreeModel::modelModified, this, &MainWindow::editTreeModel );
#ifdef HAVE_DIRECT_IMPORT_SDK
  connect( mapLinkWidget->getTreeModel(), &TreeModel::moveDirectImportDataSetToIndex, this, &MainWindow::moveDirectImportDataSetToIndex );
  connect( layerTreeView, &LayerTreeView::removeDirectImportDataSet, this, &MainWindow::removeDirectImportDataSet );
  connect( layerTreeView, &LayerTreeView::editDirectImportDataSet, this, &MainWindow::editDirectImportDataSet );
  connect( layerTreeView, &LayerTreeView::zoomToLayerDataSetExtent, mapLinkWidget, &MapLinkWidget::zoomToLayerDataSetExtent );
  connect( layerTreeView, &LayerTreeView::zoomToLayerScaleBandExtent, mapLinkWidget, &MapLinkWidget::zoomToLayerScaleBandExtent );
#endif
  connect( layerTreeView, &LayerTreeView::modifyData, this, &MainWindow::editTreeModel );
  connect( layerTreeView, &LayerTreeView::removeTreeLayer, this, &MainWindow::removeTreeLayer );
  connect( layerTreeView, &LayerTreeView::selectedAttributeData, this, &MainWindow::selectedAttributeData );
  connect( layerTreeView, &LayerTreeView::zoomToLayerExtent, mapLinkWidget, &MapLinkWidget::zoomToLayerExtent );
  connect( layerTreeView, &LayerTreeView::addLayer, this, &MainWindow::loadLayerDialog );
  connect( zoomToExtent, SIGNAL( clicked() ), layerTreeView, SLOT( zoomToExtentClicked() ) );
  connect( layerTreeView, SIGNAL( editCacheSize( std::string ) ), this, SLOT( cacheSizeDialog( std::string ) ) );
  connect( layerDockWidget, &QDockWidget::visibilityChanged, this, &MainWindow::setLayerMenuChecked );
  connect( attributeDockWidget, &QDockWidget::visibilityChanged, this, &MainWindow::setAttributeMenuChecked );
  connect( infoDisplayDock, &QDockWidget::visibilityChanged, this, &MainWindow::setTracksMenuChecked );
  connect( this, &MainWindow::attributeDatalayer, mapLinkWidget, &MapLinkWidget::selectedAttributeData );
  connect( m_cacheSizeDialog->getButtonBox(), SIGNAL( accepted() ), this, SLOT( cacheSizeDialogOKButton() ) );
  connect( m_urlDialog->getButtonBox(), SIGNAL( accepted() ), this, SLOT( urlDialogOKButton() ) );

  // Grab a reference to the layer manager and set the time acceleration of re-projection
  m_layerManager = mapLinkWidget->layerManager();
  if (m_layerManager)
  {
    m_layerManager->setTimeAccelerationFactor(1.0);
  }

  lockProjectionOrigin();

  // Create combo boxes to be able to select the scale and range of the view
  QStringList sl;
  sl << "1:10000000"
    << "1: 5000000"
    << "1: 2000000"
    << "1: 1000000"
    << "1:  500000"
    << "1:  250000"
    << "1:  100000"
    << "1:   75000"
    << "1:   50000"
    << "1:   25000"
    << "1:   20000"
    << "1:   15000"
    << "1:   10000"
    << "1:    5000"
    << "1:    2000"
    << "1:    1000";

  m_scaleBox->addItems( sl );
  m_scaleBox->setEditable( true );
  m_scaleBox->setMinimumWidth( 150 );

  sl.clear();

  sl << "2048 NM"
    << "1024 NM"
    << "512 NM"
    << "50 NM"
    << "10 NM"
    << "5 NM"
    << "1 NM";

  m_rangeBox->addItems( sl );
  m_rangeBox->setEditable( true );
  m_rangeBox->setMinimumWidth( 160 );

  QLabel* comboName1 = new QLabel();
  comboName1->setText( " Scale:  " );

  QLabel* comboName2 = new QLabel();
  comboName2->setText( " Range:  " );

  // Add combobox widgets to the toolbar
  m_zoomRangeToolbar->setVisible( actionZoomRangeToolbar->isChecked() );
  m_zoomRangeToolbar->addWidget( comboName1 );
  m_zoomRangeToolbar->addWidget( m_scaleBox );
  m_zoomRangeToolbar->addSeparator();
  m_zoomRangeToolbar->addWidget( comboName2 );
  m_zoomRangeToolbar->addWidget( m_rangeBox );

  // Make connections to the toolbar combo boxes
  connect( m_scaleBox, SIGNAL( currentIndexChanged( int ) ), this, SLOT( zoomScale( int ) ) );
  connect( m_rangeBox, SIGNAL( currentIndexChanged( int ) ), this, SLOT( nauticalRange( int ) ) );

  // Projection Toolbar
  m_projectionToolbar->setVisible( actionProjectionToolbar->isChecked() );
  m_projectionToolbar->addAction( actionChange_Destination_Waypoint );
  m_projectionToolbar->addSeparator();
  m_projectionToolbar->addAction( actionLock_Projection_Origin );

  // Track Toolbar
  m_trackToolbar->setVisible( actionTrackToolbar->isChecked() );
  m_trackToolbar->addAction( actionSelectTrack );
  m_trackToolbar->addSeparator();
  m_trackToolbar->addAction( actionViewOrientation );
  m_trackToolbar->addAction( actionPinTrack );
  m_trackToolbar->addAction( actionFollowTrack );
  actionFollowTrack->setEnabled( false );
  actionPinTrack->setEnabled( false );
  actionViewOrientation->setEnabled( false );

  // Entities Toolbar
  m_entitiesToolbar->setVisible( actionEntitiesToolbar->isChecked() );
  m_entitiesToolbar->addAction( actionLine );
  m_entitiesToolbar->addAction( actionFont );
  m_entitiesToolbar->addAction( actionPolygon );
  m_entitiesToolbar->addAction( actionVectorSymbol );
  m_entitiesToolbar->addAction( actionRasterSymbol );
  m_entitiesToolbar->addAction( actionArc );
  m_entitiesToolbar->addAction( actionEllipse );

  // Geodetic Entities Toolbar
  m_geodeticToolbar->setVisible( actionGeodeticToolbar->isChecked() );
  m_geodeticToolbar->addAction( actionGeoLine );
  m_geodeticToolbar->addAction( actionGeoFont );
  m_geodeticToolbar->addAction( actionGeoPolygon );
  m_geodeticToolbar->addAction( actionGeoVectorSymbol );
  m_geodeticToolbar->addAction( actionGeoRasterSymbol );
  m_geodeticToolbar->addAction( actionGeoArc );
  m_geodeticToolbar->addAction( actionGeoEllipse );

  // Tracks Simulation Toolbar
  m_simulationToolbar->setVisible( actionSimulationToolbar->isChecked() );
  m_simulationToolbar->addAction( actionEnableTrackMotion );

  // Add the widget that allows controlling the simulation speed to the toolbar
  m_toolbarSpeedControl = new ToolbarSpeedControl( m_simulationToolbar );
  m_simulationToolbar->addWidget( m_toolbarSpeedControl );

  // Add toolbars to window
  addToolBar( m_projectionToolbar );
  addToolBar( m_entitiesToolbar );
  addToolBar( m_geodeticToolbar );
  addToolBar( m_trackToolbar );
  addToolBar( m_zoomRangeToolbar );
  addToolBarBreak( Qt::TopToolBarArea );
  addToolBar( m_simulationToolbar );

  // Connect menu items to toolbars
  connect( actionMainToolbar, SIGNAL( triggered() ), this, SLOT( setMainToolbar() ) );
  connect( actionProjectionToolbar, SIGNAL( triggered() ), this, SLOT( setProjectionToolbar() ) );
  connect( actionTrackToolbar, SIGNAL( triggered() ), this, SLOT( setTrackToolbar() ) );
  connect( actionEntitiesToolbar, SIGNAL( triggered() ), this, SLOT( setEntitiesToolbar() ) );
  connect( actionGeodeticToolbar, SIGNAL( triggered() ), this, SLOT( setGeodeticToolbar() ) );
  connect( actionZoomRangeToolbar, SIGNAL( triggered() ), this, SLOT( setZoomRangeToolbar() ) );
  connect( actionSimulationToolbar, SIGNAL( triggered() ), this, SLOT( setSimulationToolbar() ) );

  TrackManager::instance().setTrackSelectionCallbacks( this );

  // Set up the models for the controls in the dock widget. These handle mapping MapLink objects
  // onto the UI for display.
  if (m_layerManager)
  {
    m_layerManager->declutterModel().setParent(declutterTree);
    declutterTree->setModel(&m_layerManager->declutterModel());
    m_layerManager->declutterModel().setUpdateView(mapLinkWidget);
    connect(mapLinkWidget, SIGNAL(updateDeclutterList(const QString&)), &m_layerManager->declutterModel(), SLOT(updateLayerFeatures(const QString&)));
  }

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

  //Set the default projection
  if (m_layerManager)
  {
    m_layerManager->setCurrentProjection(LayerManager::Stereographic);
  }

  // Enable drag-and-drop
  setAcceptDrops( true );

  // Maximize the window on start-up
  showMaximized();
}

// Destructor
MainWindow::~MainWindow()
{
  // Clean up
  TrackManager::instance().stopTrackUpdates();

  if (m_layerManager)
  {
    m_layerManager->declutterModel().setParent(NULL);
  }
  declutterTree->setModel( NULL );
  TrackManager::instance().trackInfoModel().setParent( NULL );
  trackInfoTable->setModel( NULL );
  TrackManager::instance().pinnedTrackModel().setParent( NULL );
  pinnedTrackDisplay->setModel( NULL );

  delete m_interactionModesGroup;
  delete m_mapUnitCursorPosition;
  delete m_latLonCursorPosition;
  delete m_timeAccelerationGroup;
  delete m_projectionGroup;
  delete m_entityGroup;
  delete m_symbolAnnotationGroup;
  delete m_symbolTypeGroup;
  delete m_toolbarSpeedControl;
  delete m_hostilityDelegate;
  delete m_trackToolbar;
}

// Load Map
//
// This function is used exclusively by the command line entered
// maps.
bool MainWindow::loadMap( const char *mapToLoad ) const
{
  if (mapLinkWidget && mapLinkWidget->loadMap(mapToLoad, false))
  {
    // Remove any existing declutter settings
    mapLinkWidget->drawingSurface()->clearAllDeclutterData();

    // By default we create 100 tracks of 50 possible types
    //TrackManager::instance().createTracks( (quint32)TrackManager::instance().numRequestedTracks(), (quint32)TrackManager::instance().numRequestedTrackTypes() );
    return true;
  }
  return false;
}

// Exit
//
// Closes the application.
void MainWindow::exit()
{
  close();
}

// Set Layer Menu Checked
//
// This is called when the user closes the 'Layer Tree View'
// docked widget by clicking the 'x' button. Doing so will update
// the menu item in 'Window' to be unchecked.
void MainWindow::setLayerMenuChecked() const
{
  actionLayer_Dock->setChecked( layerDockWidget->isVisible() );
}

// Set Attribute Menu Checked
//
// This is called when the user closes the 'Attribute Window'
// docked widget by clicking the 'x' button. Doing so will update
// the menu item in 'Window' to be unchecked.
void MainWindow::setAttributeMenuChecked() const
{
  actionAttribute_Dock->setChecked( attributeDockWidget->isVisible() );
}

// Set Tracks Menu Checked
//
// This is called when the user closes the 'Track/View Controls'
// docked widget by clicking the 'x' button. Doing so will update
// the menu item in 'Window' to be unchecked.
void MainWindow::setTracksMenuChecked() const
{
  actionTrack_Dock->setChecked( infoDisplayDock->isVisible() );
}

#ifdef _MSC_VER
const QFileDialog::Options FileDialogFlags = 0;
#else
const QFileDialog::Options FileDialogFlags = QFileDialog::DontUseNativeDialog;
#endif

const QString KEY_PREV_OPENED_LAYER = "PreviouslyLoadedLayerFile";

// Load Layer Dialog
//
// This is called when the user clicks the 'Open' button and is used
// to add a data layer to the drawing surface. There is a variety
// of different types of data that can be opened so there are
// a few different paths that can be taken depending on the file
// extension.
void MainWindow::loadLayerDialog()
{
  // Show a file open dialog to let the user choose the map to load - 
  // MapLink Pro maps either have a '.map' or '.mpc' file ending.

  QSettings settings;

  QString qFileName = QFileDialog::getOpenFileName( this, tr( "Load Supported Data" ), settings.value( KEY_PREV_OPENED_LAYER ).toString(),
    tr( "Any Supported Data (*);;Map Files (*.map *.mpc);;KML Files (*.kml *.kmz)" ), 0, FileDialogFlags );

  if ( !qFileName.isEmpty() )
  {
    settings.setValue( KEY_PREV_OPENED_LAYER, QDir().absoluteFilePath( qFileName ) );
    loadLayerFromPath( qFileName );
  }
}

void MainWindow::loadLayerFromPath( const QString& qFileName )
{
  if( !qFileName.isEmpty() )
  {
    std::string fileName = (const char*)(qFileName.toUtf8());

    std::string ext = "";
    int position = fileName.find_last_of( "." );

    if( position != std::string::npos )
    {
      ext = fileName.substr( position );
    }

    if( !(ext == ".map" || ext == ".mpc") && checkBackgroundMapIsLoaded() == false )
    {
      return;
    }

    if( ext == ".map" || ext == ".mpc" )
    {
      mapLinkWidget->handleDataDialog( fileName );
      return;
    }

    if( ext == ".tmf" )
    {
      mapLinkWidget->handleTMFDialog( fileName );
      return;
    }

    if( ext == ".kml" || ext == ".kmz" )
    {
#ifndef HAVE_DIRECT_IMPORT_SDK  
      mapLinkWidget->handleDataDialog( fileName );
      return;
#else
      QMessageBox msgBox;
      msgBox.setText("KML Data may be loaded via multiple data layer types.\n The KML DataLayer displays data based on the KML specification, with support for both KML and KMZ formats.\nThe Direct Import layer will read the data as generic vector data which may be styled via an SLD. Support for KML and KMZ formats depends on the available Direct Import Drivers.");
      msgBox.addButton("Use Direct Import Data Layer", QMessageBox::NoRole);
      QAbstractButton* buttonKMLLayer( msgBox.addButton("Use KML Data Layer", QMessageBox::YesRole) );
      msgBox.exec();
      if( msgBox.clickedButton() == buttonKMLLayer )
      {
        mapLinkWidget->handleDataDialog( fileName );
        return;
      }
      // Fall through to Direct Import Layer
#endif
    }

#ifdef HAVE_DIRECT_IMPORT_SDK
    openDirectImport( fileName ); 
#endif

  }
}

// URL Dialog Ok Button
//
// Reads the information selected by the user in the 'URL Dialog'
// and creates a layer using direct import.
void MainWindow::urlDialogOKButton()
{
#ifdef HAVE_DIRECT_IMPORT_SDK
  std::string fileName = (const char*)(m_urlDialog->getURLTextbox().toUtf8());
  openDirectImport( fileName );
#endif
}

// Open Direct Import
//
// Initialises the direct import wizard and then displays it.
void MainWindow::openDirectImport( const std::string& fileName )
{
# ifdef HAVE_DIRECT_IMPORT_SDK
  if( !m_directImportWizard )
  {
    const TSLDataLayer* coordinateProvidingLayer( mapLinkWidget->drawingSurface()->getCoordinateProvidingLayer() );
    const TSLCoordinateSystem* mapCoordinateSystem( NULL );
    if( coordinateProvidingLayer )
    {
      // Unless specified by the wizard the direct import layer should display data
      // in the same coordinate system as the loaded map
      switch( coordinateProvidingLayer->layerType() )
      {
        case TSLDataLayerTypeMapDataLayer:
          mapCoordinateSystem = (reinterpret_cast<const TSLMapDataLayer*>(coordinateProvidingLayer))->queryMapCoordinateSystem();
          break;
        case TSLDataLayerTypeStaticMapDataLayer:
          mapCoordinateSystem = (reinterpret_cast<const TSLStaticMapDataLayer*>(coordinateProvidingLayer))->queryMapCoordinateSystem();
          break;
        default:
          mapCoordinateSystem = coordinateProvidingLayer->queryCoordinateSystem();
          break;
      }
    }

    // Create the wizard
    // This will walk through the process of setting up the direct import
    // layer, performing basic analysis of the data and commiting to load the data into
    // the sample
    // Operations on the layer itself are done via the LayerManager
    // The MapLinkWidget provides an implementation of TSLDirectImportDataLayerCallbacks.
    m_directImportWizard = new DirectImportWizard
    (
      m_layerManager,
      mapLinkWidget,
      mapCoordinateSystem,
      fileName,
      this
    );

    mapLinkWidget->beginDirectImportLoad();

    m_directImportWizard->show();
    connect( m_directImportWizard->button( QWizard::FinishButton ), SIGNAL( clicked() ), this, SLOT( finishedDirectImportWizard() ) );
    connect( m_directImportWizard->button( QWizard::CancelButton ), SIGNAL( clicked() ), this, SLOT( cancelDirectImportWizard() ) );
    connect( m_directImportWizard, SIGNAL( rejected() ), this, SLOT( cancelDirectImportWizard() ) );
  }
# endif
}

void MainWindow::loadURLDialog()
{
  m_urlDialog->show();
}

// Selected Attribute Data
//
// Sends the layer details of the selected attribute data the user
// wants to be displayed in the 'Attribute Tree Widget'
void MainWindow::selectedAttributeData( const std::string& layerName, const std::string& layerType )
{
  attributeDatalayer( layerName, layerType );
}

#ifdef HAVE_DIRECT_IMPORT_SDK
void MainWindow::moveDirectImportDataSetToIndex( const char* layerName, const char* scaleBandName, int rowFrom, int rowTo )
{
  mapLinkWidget->moveDirectImportDataSetToIndex( layerName, scaleBandName, rowFrom, rowTo );
}

void MainWindow::removeDirectImportDataSet( const char* layerName, const char* scaleBandName, int row )
{
  mapLinkWidget->removeDirectImportDataSet( layerName, scaleBandName, row );
}

void MainWindow::editDirectImportDataSet( const char* layerName, const char* scaleBandName, int row )
{
  // Any prior instance of the wizard will have an invalid state
  if( m_directImportWizard )
  {
    delete m_directImportWizard;
  }

  TSLDirectImportDataSet* dataSet( m_layerManager->getDirectImportDataSet( layerName, scaleBandName, row ) );

  // Create the wizard, providing the data set to edit
  m_directImportWizard = new DirectImportWizard
  (
    m_layerManager,
    mapLinkWidget,
    NULL, // No coordinate system is required in this case
    dataSet,
    this
  );

  mapLinkWidget->beginDirectImportLoad();

  m_directImportWizard->show();
  connect( m_directImportWizard->button( QWizard::FinishButton ), SIGNAL( clicked() ), this, SLOT( finishedDirectImportWizard() ) );
  connect( m_directImportWizard->button( QWizard::CancelButton ), SIGNAL( clicked() ), this, SLOT( cancelDirectImportWizard() ) );
  connect( m_directImportWizard, SIGNAL( rejected() ), this, SLOT( cancelDirectImportWizard() ) );
}

void MainWindow::cancelDirectImportWizard()
{
  mapLinkWidget->endDirectImportLoad( m_layerManager->getDirectImportName() );
  delete m_directImportWizard;
  m_directImportWizard = NULL;
}

void MainWindow::finishedDirectImportWizard()
{
  mapLinkWidget->endDirectImportLoad( m_layerManager->getDirectImportName() );

  delete m_directImportWizard;
  m_directImportWizard = NULL;

  mapLinkWidget->getTreeModel()->refreshFromSurface( mapLinkWidget->drawingSurface() );
}
#endif

// Show About Box
//
// Displays copyright information about the sample.
void MainWindow::showAboutBox()
{
  // Display an about box
  QMessageBox::about( this, tr( "MapLink Pro Advanced Sample" ),
    tr( "<img src=\":/images/envitia.png\"/>"
        "<p>Copyright &copy; 1998-2017 Envitia Group PLC. All rights reserved.</p>"
#ifdef WIN32
        "<p align=center style=\"color:#909090\">Portions developed using LEADTOOLS<br>"
        "Copyright &copy; 1991-2009 LEAD Technologies, Inc. ALL RIGHTS RESERVED</p>"
#endif
        "<p>Please see the MapLink Pro 3rd Party Licence Document for a list of all libraries used.</p>"
    ) );
}

// Move Layer To Index
//
// Triggered when the layers in the 'Layer Tree View' are re-ordered
// and need to be re-ordered in the drawing surface.
void MainWindow::moveLayerToIndex( const char* moveLayer, const char* targetLayer, int row ) const
{
  mapLinkWidget->moveLayerToIndex( moveLayer, targetLayer, row );
}

// Edit Tree Model
//
// Triggered when either the transparency or visibility of a 
// layer in the 'Layer Tree View' is modified.
void MainWindow::editTreeModel( const QModelIndex &index, long value ) const
{
  mapLinkWidget->editTreeModel( index, value );
}

// Remove Tree Layer
//
// Removes the selected data layer from the drawing surface.
void MainWindow::removeTreeLayer( const std::string& layerName, const std::string& treeAttribute ) const
{
  mapLinkWidget->removeTreeLayer( layerName, treeAttribute );
}

// Reset View
//
// Drawing surface focuses all of it's content in the centre of
// the widget.
void MainWindow::resetView() const
{
  // Tell the widget to reset the viewing area to its maximum extent
  mapLinkWidget->resetView();
}

// Zoom In Once
//
// Zooms in the drawing surface by a set increment.
void MainWindow::zoomInOnce() const
{
  // Tell the widget to zoom in by a fixed percentage
  mapLinkWidget->zoomInOnce();
}

// Zoom Out Once
//
// Zooms out the drawing surface by a set increment.
void MainWindow::zoomOutOnce() const
{
  // Tell the widget to zoom out by a fixed percentage
  mapLinkWidget->zoomOutOnce();
}

// Toggle Layer Dock
//
// Toggles the 'Layer Tree View' docked widget's visibility.
void MainWindow::toggleLayerDock() const
{
  if( actionLayer_Dock->isChecked() )
  {
    layerDockWidget->show();
  }
  else
  {
    layerDockWidget->hide();
  }
}

// Toggle Attribute Dock
//
// Toggles the 'Attribute Window' docked widget's visibility.
void MainWindow::toggleAttributeDock() const
{
  if( actionAttribute_Dock->isChecked() )
  {
    attributeDockWidget->show();
  }
  else
  {
    attributeDockWidget->hide();
  }
}

// Toggle Track Dock
//
// Toggles the 'Track/View Control' docked widget's visibility.
void MainWindow::toggleTrackDock() const
{
  if( actionTrack_Dock->isChecked() )
  {
    infoDisplayDock->show();
  }
  else
  {
    infoDisplayDock->hide();
  }
}

// Set To Pan Mode
//
// Allows the user to click the widget which will focus the drawing
// surface with that point in the centre.
void MainWindow::activatePanMode() const
{
  // Tell the widget to activate the pan interaction mode
  mapLinkWidget->activatePanMode();
}

// Set To Grab Mode
//
// Allows the user to click and drag the widget to move the
// drawing surface around.
void MainWindow::activateGrabMode() const
{
  // Tell the widget to activate the grab interaction mode
  mapLinkWidget->activateGrabMode();
}

// Set To Zoom Mode
//
// Allows the user to click and drag the widget to zoom to the 
// selected region.
void MainWindow::activateZoomMode() const
{
  // Tell the widget to activate the zoom interaction mode
  mapLinkWidget->activateZoomMode();
}

// Set To Waypoint Select Mode
//
// Allows the user to click the widget to redirect the projection
// object.
void MainWindow::activateWaypointMode() const
{
  // Tell the widget to activate the waypoint interaction mode
  mapLinkWidget->activateWaypointMode();
}

// Set To Track Select Mode
//
// Allows the user to click the widget to view details about a 
// tracked object.
void MainWindow::activateTrackSelectMode() const
{
  // Tell the application to activate the waypoint interaction mode
  mapLinkWidget->activateTrackSelectMode();
}

// Nautical Range
//
// This method is called when the value in the 'Range' combo box
// has changed and sends the new value to the MapLink Widget where
// the drawing surface will zoom to that particular range.
void MainWindow::nauticalRange( int index )
{
  std::string str = (const char*)(m_rangeBox->currentText().toUtf8());

  int range = atoi( str.c_str() );
  mapLinkWidget->setViewRange( range );
}

// Zoom Scale
//
// This method is called when the value in the 'Scale' combo box
// has changed and sends the new value to the MapLink Widget where
// the drawing surface will zoom to that particular scale.
void MainWindow::zoomScale( int index )
{
  std::string str = (const char*)(m_scaleBox->currentText().toUtf8());
  size_t pos = str.find( ":" );        // position of ":" in str
  string str2 = str.substr( pos + 1 );  // get from after ":" to the end

  int m_scale = atoi( str2.c_str() );

  mapLinkWidget->setViewScale( 1.0 / (double)m_scale );
}

// Cache Size Dialog
//
// Right-clicking a layer in the tree view and selecting 'Edit Cache
// Size' will initiate this method. Doing so will create a dialog
// box which allows the user to change the cache size.
void MainWindow::cacheSizeDialog( const std::string& name )
{
  if (!m_layerManager || !m_cacheSizeDialog)
  {
    return;
  }
  int currentCache = m_layerManager->getCacheSize( name );
  m_cacheSizeDialog->setupDialog( name, currentCache );
  m_cacheSizeDialog->show();
}

// Cache Size Dialog Ok Button
//
// Reads the information selected by the user in the 'Cache
// Size Dialog' and edits the cache size for the layer.
void MainWindow::cacheSizeDialogOKButton()
{
  if (m_layerManager)
  {
    m_layerManager->editCacheSize(m_cacheSizeDialog->getLayerName(), m_cacheSizeDialog->cacheSizeInKB());
  }
}

// Toggle Track Motion
//
// Turns the track movement off/on.
void MainWindow::toggleTrackMotion()
{
  if (!m_layerManager || !mapLinkWidget)
  {
    return;
  }

  if( !actionEnableTrackMotion->isChecked() )
  {
    // We are enabling track motion. Use the special case for a QTimer with a timeout
    // value of 0 to cause the connected slot to activate whenever the application's event queue is empty.
    // This causes the application to update as fast as possible without stopping the application widgets
    // from functioning.

    // Prevent the user from reloading a map while tracks are being updated
    // action_Open->setEnabled( false );
    m_layerManager->setFramerateLayerVisibility( mapLinkWidget->drawingSurface(), true );
    TrackManager::instance().startTrackUpdates();
    m_toolbarSpeedControl->setEnabled( true );
  }
  else
  {
    // We are disabling track motion
    m_layerManager->setFramerateLayerVisibility( mapLinkWidget->drawingSurface(), false );
    TrackManager::instance().stopTrackUpdates();
    // action_Open->setEnabled( true );
    mapLinkWidget->update();
    m_toolbarSpeedControl->setEnabled( false );
  }
}

// Toggle Follow Track
//
// Sets the currently selected track to be the central focus
// of the drawing surface.
void MainWindow::toggleFollowTrack( bool followTrack )
{
  TrackManager::instance().enableTrackFollow( followTrack );
  mapLinkWidget->update();
}

// Toggle View Orientation
//
// Track option - Redraws the drawing surface with the direction
// the track is facing towards the north.
void MainWindow::toggleViewOrientation( bool trackHeadingUp )
{
  TrackManager::instance().enableTrackUpOrientation( trackHeadingUp );
  mapLinkWidget->update();
}

// Change Track Numbers
//
// Shows a dialog box that allows the user to change the number
// of tracks displayed, and the number of different types of
// symbols used.
void MainWindow::changeTrackNumbers()
{
  if (!m_layerManager || !checkBackgroundMapIsLoaded())
  {
    return;
  }
  
  if( !mapLinkWidget->drawingSurface()->getDataLayer( "TracksLayer" ) )
  {
    m_layerManager->configureTracksLayer( mapLinkWidget->drawingSurface() );
    mapLinkWidget->getTreeModel()->refreshFromSurface( mapLinkWidget->drawingSurface(), "Background" );

    mapLinkWidget->drawingSurface()->bringToFront( "EntityLayer" );
  }

  // Show the dialog to let the user change how many tracks there are
  TrackNumbers *trackNumberDialog = new TrackNumbers( this );
  trackNumberDialog->setAttribute( Qt::WA_DeleteOnClose );
  trackNumberDialog->setModal( true );
  trackNumberDialog->show();
}

// Set No Track Annotation
//
// No details about each individual track on shown on the drawing
// surface.
void MainWindow::setNoTrackAnnotation()
{
  TrackManager::instance().setTrackAnnotationLevel( AnnotationNone );
}

// Set Low Track Annotation
//
// A small amount of details about each individual track is shown 
// on the drawing surface.
void MainWindow::setLowTrackAnnotation()
{
  TrackManager::instance().setTrackAnnotationLevel( AnnotationLow );
}

// Set Medium Track Annotation
//
// A medium amount of details about each individual track is shown 
// on the drawing surface.
void MainWindow::setMediumTrackAnnotation()
{
  TrackManager::instance().setTrackAnnotationLevel( AnnotationMedium );
}

// Set High Track Annotation
//
// A large amount of details about each individual track is shown 
// on the drawing surface.
void MainWindow::setHighTrackAnnotation()
{
  TrackManager::instance().setTrackAnnotationLevel( AnnotationHigh );
}

// Track Selection Status Changed
//
// Called when a track is selected or when no tracks are selected.
// Enables/disables toolbar controls for tracks depending on
// if a track is selected, and data about the selected track
// can be viewed in the 'Track/View Control' widget's 'Selected Track'
// tab.
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
  mapLinkWidget->update();

  // Refresh the contents of the track information table with the new selection
  TrackManager::instance().trackInfoModel().reloadData();
  trackInfoTable->update();
}

// Pin Selected Track
//
// Adds details about the currently selected track to the list of
// 'pinned' tracks on the Track/View Control widget. 
//
// The direction, velocity and altitude of each pinned 
// track are constantly updated.
void MainWindow::pinSelectedTrack()
{
  TrackManager::instance().pinnedTrackModel().pinSelectedTrack();
}

// Set Symbol Type APP6A
//
// Sets the symbol type for the tracks to APP6A.
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
    QString configLocation( QString::fromUtf8( maplHome ) );
    configLocation += "/config/app6aConfig.csv";
    TrackManager::instance().loadSymbolConfig( configLocation );
  }
}

// Set Symbol Type 2525B
//
// Sets the symbol type for the tracks to 2525B.
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
    QString configLocation( QString::fromUtf8( maplHome ) );
    configLocation += "/config/2525bConfig.csv";
    TrackManager::instance().loadSymbolConfig( configLocation );
  }
}

// Lock Projection Origin
//
// This halts the origin of the projection to stop it from updating.
// The projection object will continue to move.
void MainWindow::lockProjectionOrigin() const
{
  if (m_layerManager)
  {
    m_layerManager->lockProjectionOrigin(actionLock_Projection_Origin->isChecked());
  }
}

// Time Acceleration 1x
//
// This modifies the movement speed of the projection object to the
// selected value.
void MainWindow::setTimeAcceleration1x() const
{
  if (m_layerManager)
  {
    m_layerManager->setTimeAccelerationFactor(1.0);
  }
}

// Time Acceleration 10x
//
// This modifies the movement speed of the projection object to the
// selected value.
void MainWindow::setTimeAcceleration10x() const
{
  if (m_layerManager)
  {
    m_layerManager->setTimeAccelerationFactor(10.0);
  }
}

// Time Acceleration 100x
//
// This modifies the movement speed of the projection object to the
// selected value.
void MainWindow::setTimeAcceleration100x() const
{
  if (m_layerManager)
  {
    m_layerManager->setTimeAccelerationFactor(100.0);
  }
}

// Time Acceleration 1,000x
//
// This modifies the movement speed of the projection object to the
// selected value.
void MainWindow::setTimeAcceleration1000x() const
{
  if (m_layerManager)
  {
    m_layerManager->setTimeAccelerationFactor(1000.0);
  }
}

// Time Acceleration 10,000x
//
// This modifies the movement speed of the projection object to the
// selected value.
void MainWindow::setTimeAcceleration10000x() const
{
  if (m_layerManager)
  {
    m_layerManager->setTimeAccelerationFactor(10000.0);
  }
}

// Select Stereographic WGS84 Projection
//
// This allows for the drawing surface to be displayed with the
// selected projection.
void MainWindow::setProjectionStereographicWGS84() const
{
  if (m_layerManager)
  {
    m_layerManager->setProjection(LayerManager::Stereographic, mapLinkWidget->drawingSurface());
  }
}

// Select Gnomic Spherical Earth Projection
//
// This allows for the drawing surface to be displayed with the
// selected projection.
void MainWindow::setProjectionGnomicSphericalEarth() const
{
  if (m_layerManager)
  {
    m_layerManager->setProjection(LayerManager::GnomicSphericalEarth, mapLinkWidget->drawingSurface());
  }
}

// Select Transverse Mercator WGS84 Projection
//
// This allows for the drawing surface to be displayed with the
// selected projection.
void MainWindow::setProjectionTransverseMercatorWGS84() const
{
  if (m_layerManager)
  {
    m_layerManager->setProjection(LayerManager::TransverseMercatorUSGS, mapLinkWidget->drawingSurface());
  }
}

// Select Mercator Projection
//
// This allows for the drawing surface to be displayed with the
// selected projection.
void MainWindow::setProjectionMercator() const
{
  if (m_layerManager)
  {
    m_layerManager->setProjection(LayerManager::Mercator, mapLinkWidget->drawingSurface());
  }
}

// Select Polyline
//
// This allows for the selected entity to be drawn onto
// the drawing surface.
void MainWindow::selectPolyline() const
{
  mapLinkWidget->selectEntity( "Polyline" );
}

// Select Polygon
//
// This allows for the selected entity to be drawn onto
// the drawing surface.
void MainWindow::selectPolygon() const
{
  mapLinkWidget->selectEntity( "Polygon" );
}

// Select Text
//
// This allows for the selected entity to be drawn onto
// the drawing surface.
void MainWindow::selectText() const
{
  mapLinkWidget->selectEntity( "Text" );
}

// Select Vector Symbol
//
// This allows for the selected entity to be drawn onto
// the drawing surface.
void MainWindow::selectVectorSymbol() const
{
  mapLinkWidget->selectEntity( "VectorSymbol" );
}

// Select Raster Symbol
//
// This allows for the selected entity to be drawn onto
// the drawing surface.
void MainWindow::selectRasterSymbol() const
{
  mapLinkWidget->selectEntity( "RasterSymbol" );
}

// Select Arc
//
// This allows for the selected entity to be drawn onto
// the drawing surface.
void MainWindow::selectArc() const
{
  mapLinkWidget->selectEntity( "Arc" );
}

// Select Ellipse
//
// This allows for the selected entity to be drawn onto
// the drawing surface.
void MainWindow::selectEllipse() const
{
  mapLinkWidget->selectEntity( "Ellipse" );
}

// Select Geodetic Polyline
//
// This allows for the selected entity to be drawn onto
// the drawing surface.
void MainWindow::selectGeoPolyline() const
{
  mapLinkWidget->selectEntity( "GeoPolyline" );
}

// Select Geodetic Polygon
//
// This allows for the selected entity to be drawn onto
// the drawing surface.
void MainWindow::selectGeoPolygon() const
{
  mapLinkWidget->selectEntity( "GeoPolygon" );
}

// Select Geodetic Text
//
// This allows for the selected entity to be drawn onto
// the drawing surface.
void MainWindow::selectGeoText() const
{
  mapLinkWidget->selectEntity( "GeoText" );
}

// Select Geodetic Vector Symbol
//
// This allows for the selected entity to be drawn onto
// the drawing surface.
void MainWindow::selectGeoVectorSymbol() const
{
  mapLinkWidget->selectEntity( "GeoVectorSymbol" );
}

// Select Geodetic Raster Symbol
//
// This allows for the selected entity to be drawn onto
// the drawing surface.
void MainWindow::selectGeoRasterSymbol() const
{
  mapLinkWidget->selectEntity( "GeoRasterSymbol" );
}

// Select Geodetic Arc
//
// This allows for the selected entity to be drawn onto
// the drawing surface.
void MainWindow::selectGeoArc() const
{
  mapLinkWidget->selectEntity( "GeoArc" );
}

// Select Geodetic Ellipse
//
// This allows for the selected entity to be drawn onto
// the drawing surface.
void MainWindow::selectGeoEllipse() const
{
  mapLinkWidget->selectEntity( "GeoEllipse" );
}

const QString KEY_PREV_SELECTED_TMF_FILE = "PreviouslyLoadedOrSavedTMFFile";

void MainWindow::saveToTMF()
{
  // Show a file save dialog.

  QSettings settings;

  QString qFileName = QFileDialog::getSaveFileName( this, tr( "Save As TMF File" ), settings.value( KEY_PREV_SELECTED_TMF_FILE ).toString(),
    tr( "TMF Files (*.tmf);;All Files (*)" ), 0, FileDialogFlags );

  if ( !qFileName.isEmpty() )
  {
    settings.setValue( KEY_PREV_SELECTED_TMF_FILE, QDir().absoluteFilePath( qFileName ) );
    mapLinkWidget->saveToTMF( (const char*)qFileName.toUtf8() );
  }
}

void MainWindow::loadFromTMF()
{
  // Show a file open dialog.

  QSettings settings;

  QString qFileName = QFileDialog::getOpenFileName( this, tr( "Load TMF File" ), settings.value( KEY_PREV_SELECTED_TMF_FILE ).toString(),
    tr( "TMF Files (*.tmf);;All Files (*)" ), 0, FileDialogFlags );

  if ( !qFileName.isEmpty() )
  {
    settings.setValue( KEY_PREV_SELECTED_TMF_FILE, QDir().absoluteFilePath( qFileName ) );
    mapLinkWidget->loadFromTMF( (const char*)qFileName.toUtf8() );
  }
}

void MainWindow::dragEnterEvent( QDragEnterEvent * event )
{
  const QMimeData* mimeData = event->mimeData();
  if ( mimeData->hasUrls() )
    event->acceptProposedAction();
}

void MainWindow::dragMoveEvent( QDragMoveEvent * event )
{
  const QMimeData* mimeData = event->mimeData();
  if ( mimeData->hasUrls() )
    event->acceptProposedAction();
}

void MainWindow::dropEvent( QDropEvent * event )
{
  const QMimeData* mimeData = event->mimeData();
  if ( mimeData->hasUrls() )
  {
    QList<QUrl> urlList = mimeData->urls();
    if ( urlList.size() > 0 )
    {
      loadLayerFromPath( urlList[0].toLocalFile() );
    }
  }
}

// Set Menu Toolbar
//
// Hides/Shows the 'Main' toolbar when checked/unchecked in the 
// Window->Toolbar menu.
void MainWindow::setMainToolbar()
{
  mainToolBar->setVisible( actionMainToolbar->isChecked() );
}

// Set Projection Toolbar
//
// Hides/Shows the 'Projection' toolbar when checked/unchecked in the 
// Window->Toolbar menu.
void MainWindow::setProjectionToolbar()
{
  m_projectionToolbar->setVisible( actionProjectionToolbar->isChecked() );
}

// Set Entities Toolbar
//
// Hides/Shows the 'Entities' toolbar when checked/unchecked in the 
// Window->Toolbar menu.
void MainWindow::setEntitiesToolbar()
{
  m_entitiesToolbar->setVisible( actionEntitiesToolbar->isChecked() );
}

// Set Track Toolbar
//
// Hides/Shows the 'Track' toolbar when checked/unchecked in the 
// Window->Toolbar menu.
void MainWindow::setTrackToolbar()
{
  m_trackToolbar->setVisible( actionTrackToolbar->isChecked() );
}

// Set Geodetic Toolbar
//
// Hides/Shows the 'Geodetic Entities' toolbar when checked/unchecked 
// in the Window->Toolbar menu.
void MainWindow::setGeodeticToolbar()
{
  m_geodeticToolbar->setVisible( actionGeodeticToolbar->isChecked() );
}

// Set Zoom Range Toolbar
//
// Hides/Shows the 'Zoom/Scale Range' toolbar when checked/unchecked in the 
// Window->Toolbar menu.
void MainWindow::setZoomRangeToolbar()
{
  m_zoomRangeToolbar->setVisible( actionZoomRangeToolbar->isChecked() );
}

// Set Simulation Toolbar
//
// Hides/Shows the 'Track Simulation' toolbar when checked/unchecked in the 
// Window->Toolbar menu.
void MainWindow::setSimulationToolbar()
{
  m_simulationToolbar->setVisible( actionSimulationToolbar->isChecked() );
}

bool MainWindow::checkBackgroundMapIsLoaded()
{
  if (m_layerManager && !m_layerManager->backgroundMapLoaded())
  {
    QMessageBox::critical( this, "No Background Map Loaded",
        "No Background Map has been loaded. This is required to provide a coordinate providing layer for realtime reprojection, track simulation, and KML/Direct Import layers.\nPlease load a MapLink Map file (.map|.mpc)" );
    return false;
  }
  return true;
}
