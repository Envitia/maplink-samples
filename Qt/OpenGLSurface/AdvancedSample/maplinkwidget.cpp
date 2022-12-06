/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>

#if (QT_VERSION >= 0x50000)
#include <QGuiApplication>
#endif
#include <QMessageBox>

#include "application.h"
#include "maplinkwidget.h"
#include "layermanager.h"

#ifdef HAVE_DIRECT_IMPORT_SDK
# include "ui/layertreeview/directimportscalebandtreeitem.h"
# include "ui/layertreeview/directimportdatasettreeitem.h"
#endif

#include <sstream>
#include <iostream>
using namespace std;

#if (QT_VERSION >= 0x50000) 
//
// Qt5.0 has dropped winID() and x11Info()
//
// A different interface was reinstated in Qt 5.1.
//
// The location of the file in the include directory is difficult to
// find on Windows so we pick it up directly from the build area.
//
// To use Qt on Desktop Windows you are going to have to build from source
// in anycase as the default and currently only distribution is for ANGLE on
// Windows.
# ifdef WIN32
#  include <qpa/qplatformnativeinterface.h>
#  include <qwindow.h>

static QWindow* windowForWidget( const QWidget* widget )
{
  QWindow* window = widget->windowHandle();
  if( window )
    return window;
  const QWidget* nativeParent = widget->nativeParentWidget();
  if( nativeParent )
    return nativeParent->windowHandle();
  return NULL;
}

WId getHWNDForWidget( const QWidget* widget )
{
  QWindow* window = ::windowForWidget( widget );
  if( window && window->handle() )
  {
    QPlatformNativeInterface* iface = QGuiApplication::platformNativeInterface();
    return (WId)( iface->nativeResourceForWindow( QByteArrayLiteral( "handle" ), window ) );
  }
  return 0;
}
# else
#  include <qpa/qplatformnativeinterface.h>
# endif
#else
# ifdef WIN32
WId getHWNDForWidget( const QWidget* widget )
{
  WId hWnd = widget->winId();
  return hWnd;
}
# endif
#endif

// These are defined by X11 which interfere with the Qt definitions
#ifdef KeyPress
#  undef KeyPress
#endif
#ifdef KeyRelease
# undef KeyRelease
#endif

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Constructor
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MapLinkWidget::MapLinkWidget( QWidget * parent, const QGLWidget * shareWidget, Qt::WindowFlags f )
  : QGLWidget( parent, shareWidget, f )
  , m_application( new Application( parent ) )
  , m_dataLayerDialog( new DataLayerDialog( parent ) )
  , m_tmfLayerDialog( new TMFLayerDialog( parent ) )
  // , m_progressDialog( new QProgressDialog( parent ) )
  , m_viewUpdater( new QTimer( parent ) )
  , m_pauseUpdate( false )
  , m_lastWheelEventTime( QTime::currentTime() )
  , m_initialise(true)
{
  // Setup progress dialog
  //m_progressDialog->setWindowModality( Qt::WindowModal );
  //m_progressDialog->setRange( 0, 100 );
  //m_progressDialog->setCancelButton( NULL );

  // Turn on/off Qt buffer swapping.
  // The MapLink Pro OpenGL drawing surface can do the swapping if desired.
  // For this demo we are turning on the Qt swapping and turning off the
  // drawing surface swapping. This is controlled by the macro ML_QT_BUFFER_SWAP
  setAutoBufferSwap( ML_QT_BUFFER_SWAP );

  // Turn off Qt colour buffer clearing.
  // The MapLink Pro OpenGL drawing surface will normally clear the colour buffer.
  // When a map with a background colour is loaded this will clear the colour buffer to the map's
  // background colour automatically.
  setAutoFillBackground( false );

  if( parent )
  {
    // If this Widget is not a child of a MainWindow then we need
    // to configure the parent so that we get Keyboard and Mouse
    // Events.
    //
    // In Qt Designer do not set the Focus and Mouse Tracking.
    // If you do delete the entries from the 'ui' file.
    //
    parent->installEventFilter( this );
    // See below.
    parent->setFocusPolicy( Qt::WheelFocus );
    parent->setMouseTracking( true );
  }

  m_layerManager = m_application->layerManager();

  // Strong Focus and Wheel Mouse.
  setFocusPolicy( Qt::WheelFocus );

  // Set the mouse tracking in the designer.
  setMouseTracking( true );

  // Set functions for clicking the 'Ok' button when a form has been completed.
  connect( m_dataLayerDialog->getButtonBox(), SIGNAL( accepted() ), this, SLOT( dataDialogOKButton() ) );
  connect( m_tmfLayerDialog->getButtonBox(), SIGNAL( accepted() ), this, SLOT( tmfDialogOKButton() ) );

  // Connect browse button in dialog form to a function
  connect( m_tmfLayerDialog->getBrowseButton(), SIGNAL( clicked() ), this, SLOT( tmfBrowseButton() ) );

  // Frame update events
  connect( &m_viewUpdater, SIGNAL( timeout() ), this, SLOT( idle() ) );
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Destructor
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MapLinkWidget::~MapLinkWidget()
{
  // Clean up
  delete m_application;
  //delete m_progressDialog;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Loading Maps
//
// This function is only used by .map files entered into the command line.
// It is used to configure the projection feature of this sample.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool MapLinkWidget::loadMap( const char *mapToLoad, bool useSharedCache )
{
  bool result( m_layerManager->loadMap( mapToLoad, drawingSurface(), useSharedCache ) );

  if (result && m_initialise)
  {
    m_initialise = false;
    m_layerManager->configureDrawingSurface( drawingSurface(), m_application->getWaypointMode() );
    getTreeModel()->refreshFromSurface( drawingSurface(), "ProjectionSample" );

    m_layerManager->configureEntityLayer( drawingSurface() );
    m_layerManager->configureTracksLayer( drawingSurface() );
    getTreeModel()->refreshFromSurface( drawingSurface(), "Background" );

    drawingSurface()->bringToFront( "EntityLayer" );
    drawingSurface()->bringToFront( "TracksLayer" );
    drawingSurface()->bringToFront( "track" );
    drawingSurface()->bringToFront( "fps" );

    resetView();
  }
  m_viewUpdater.start( 0 );

  return result;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Display Zoom Scale & Range
//
// This function is ran when the Maplink widget interacts with mouse 
// events. It requests a recalculation of the Zoom Scale and Range and
// then replaces the old values displayed on the status bar widgets
// with the new calculations.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::displayZoomScaleAndRange()
{
  int zoomScale = 0;
  double xViewRange = 0.0, yViewRange = 0.0;

  calculateZoomScaleAndRange( zoomScale, xViewRange, yViewRange );

  m_zoomScaleComboBox->setCurrentIndex( -1 );
  m_zoomScaleComboBox->setCurrentText( QString( "1:%1" ).arg( zoomScale, 9 ) );

  m_viewRangeComboBox->setCurrentIndex( -1 );
  m_viewRangeComboBox->setCurrentText( QString( "%1 NM" ).arg( (int)yViewRange, 9 ) );
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Zoom Scale & Range calculation
//
// This function is used to calculate the Zoom Scale and Range of
// the map data currently displayed from the drawing surface. The result
// of this is used in the status bar widgets and is ran every time the
// widget interacts with mouse events.
//
// The calculations make a number of assumptions which means that the
// range and scale are very much an estimate.
//
// To obtain a more accurate horizontal and vertical range and scale
// you would need to take one or more of the approaches below:
//
// 1. Use Lat/Long conversions
// 2. Great Circle distance
// 3. Vincenty calculations (see TSLCoordinateConverter).
//
// You are trading accuracy, complexity of implementation and performance
// for each approach.
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::calculateZoomScaleAndRange( int& zoomScale, double& xViewRange, double& yViewRange )
{
  // Ensure we have a drawing surface before proceeding.
  TSLDrawingSurface* ds( drawingSurface() );
  if( ds == NULL )
  {
    return;
  }

  const TSLDataLayer* csLayer( ds->getCoordinateProvidingLayer() );
  const TSLCoordinateSystem *cs = csLayer ? csLayer->queryCoordinateSystem() : NULL;

  if( !cs )
  {
    zoomScale = 0;
    xViewRange = 0.0;
    yViewRange = 0.0;
    return;
  }

  // Get the displayed extent in Map Units
  //
  // It is assumed that the Map Units (MU) are meters.
  // Map Units are essentially scaled TMCs and are thus also a rectilinear
  // coordinate system. This introduces errors with simple distance measures
  // as each projection preserves different properties such as area, distance
  // angles etc...
  double x1, y1, x2, y2;
  ds->getMUExtent( &x1, &y1, &x2, &y2 );

  // Size of display area in Map Units
  double extentHeight = ( y2 > y1 ) ? ( y2 - y1 ) : ( y1 - y2 );
  double extentWidth = ( x2 > x1 ) ? ( x2 - x1 ) : ( x1 - x2 );

  xViewRange = extentWidth;
  yViewRange = extentHeight;


  // To calculate the scale of the map we need:
  // - The extent of the displayed map data (in meters)
  // - The physical dimensions of the display area used by the drawing surface (in meters)
  //
  // Note that only the vertical component of the extent is used in this calculation. This corresponds to
  // the calculation used for the direct import layer's scale bands.

  // Query the physical dimensions of the display
  // These may have been determined automatically, however applications must ensure these values are correct.
  // Some platform configurations and displays may produce incorrect values, or none at all.
  // The method of determining these values varies based on the platform.
  int horizontalMM, verticalMM, horizontalPixels, verticalPixels;
  ds->getDeviceCapabilities( horizontalMM, verticalMM, horizontalPixels, verticalPixels );

  if( verticalMM != 0 && verticalPixels != 0 )
  {
    // The device capabilities may apply to the entire display, or just to the current application's window.
    // Because of this we must work out the size of a single pixel
    double physicalHeightOfPixel( (verticalMM / 1000.0) / verticalPixels );

    // Then calculate the physical size of the surface's DU extent
    TSLDeviceUnits dux1, duy1, dux2, duy2;
    ds->getDUExtent( &dux1, &duy1, &dux2, &duy2 );

    TSLDeviceUnits duHeight( (duy2 > duy1) ? (duy2 - duy1) : (duy1 - duy2) );
    double physicalViewHeight( duHeight * physicalHeightOfPixel );

    // And the currently displayed map scale
    zoomScale = 1.0 / (physicalViewHeight / yViewRange);
  }
  else
  {
    zoomScale = 0.0;
  }

  // Convert the range to Nautical Miles.
  // 1degree is 60NM
  // 60NM at the equator is 111200m.
  // As you go north or south the x-axis shrinks while the y-axis does not.
  xViewRange *= 0.000539957;
  yViewRange *= 0.000539957;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Changes the current view scale to be the specified scale.
//
// This is very much an approximation and assumes a rectalinear
// projection.
//
// The scale calculated is an approximation (see calculateZoomScaleAndRange).
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::setViewScale( double scaleFactor )
{
  // Ensure we have a drawing surface before proceeding.
  if( !drawingSurface() )
  {
    return;
  }
  // Calculate the correct UU extent for the drawing surface in order to make
  // the view the requested scale without changing the viewing position.
  double tmcPerMU = drawingSurface()->TMCperMU();
  double tmcPerDUX, tmcPerDUY;
  drawingSurface()->TMCperDU( tmcPerDUX, tmcPerDUY );

  double duPerMUX = tmcPerMU / tmcPerDUX;
  double duPerMUY = tmcPerMU / tmcPerDUY;

  // Ask the drawing surface for the physical size of the display
  int displayHorizontalMM, displayVerticalMM, width, height;
  drawingSurface()->getDeviceCapabilities( displayHorizontalMM, displayVerticalMM, width, height );
  double pixelsPerMeterX = width / ( displayHorizontalMM / 1000.0 );
  double pixelsPerMeterY = height / ( displayVerticalMM / 1000.0 );

  // Scale the currently displayed extent to match the requested scale
  double kX = duPerMUX / ( pixelsPerMeterX * scaleFactor );
  double kY = duPerMUY / ( pixelsPerMeterY * scaleFactor );

  TSLEnvelope extent;
  drawingSurface()->getTMCExtent( extent );
  extent.scale( kX, kY );

  // Set the view of the drawing surface to match the requested scale
  double uux1, uuy1, uux2, uuy2;
  drawingSurface()->TMCToUU( extent.bottomLeft().x(), extent.bottomLeft().y(), &uux1, &uuy1 );
  drawingSurface()->TMCToUU( extent.topRight().x(), extent.topRight().y(), &uux2, &uuy2 );
  drawingSurface()->resize( uux1, uuy1, uux2, uuy2, false, true );

  update();
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Changes the current view range to be the specified range.
//
// This is very much an approximation and assumes a rectalinear
// projection.
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::setViewRange( double range )
{
  // Ensure we have a drawing surface before proceeding.
  if( !drawingSurface() )
  {
    return;
  }
  double uuX1, uuY1, uuX2, uuY2;

  drawingSurface()->getUUExtent( &uuX1, &uuY1, &uuX2, &uuY2 );

  double centreX = uuX1 + ( ( uuX2 - uuX1 ) / 2.0 );
  double centreY = uuY1 + ( ( uuY2 - uuY1 ) / 2.0 );

  // calculate new extent to be viewed, in UU
  double border = ( range * 1852 ) * 0.5 * drawingSurface()->userUnits();
  double viewMinx = centreX - border;
  double viewMaxx = centreX + border;
  double viewMiny = centreY - border;
  double viewMaxy = centreY + border;

  drawingSurface()->resize( viewMinx, viewMiny, viewMaxx, viewMaxy, false, true );

  update();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Events forwarded from the main window. These are all forwarded on to
// the appliction, and we will issue a redraw request if the application asks
// us to.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::resetView()
{
  m_application->resetView();
  update(); // The viewing extent has changed, so a redraw is always required
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Zoom In
//
// Allows the user to click the widget to zoom in.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::zoomInOnce()
{
  if( m_application->zoomIn() )
  {
    // We were asked to redraw the display
    update();
  }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Zoom Out
//
// Allows the user to click the widget to zoom out.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::zoomOutOnce()
{
  if( m_application->zoomOut() )
  {
    // We were asked to redraw the display
    update();
  }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Pan Mode
//
// Allows the user to click the widget to pan to that position as the
// center of the view.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::activatePanMode() const
{
  // Tell the application to activate the pan interaction mode
  m_application->activatePanMode();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Grab Mode
//
// Allows the user to click and drag the widget to change the position
// of the view.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::activateGrabMode() const
{
  // Tell the application to activate the grab interaction mode
  m_application->activateGrabMode();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Zoom mode
//
// Allows the user to click and drag the widget to zoom to the selected
// extent.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::activateZoomMode() const
{
  // Tell the application to activate the zoom interaction mode
  m_application->activateZoomMode();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Set To Waypoint Mode
//
// Allows the user to click the widget to redirect the projection
// object to that location.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::activateWaypointMode() const
{
  // Tell the application to activate the waypoint interaction mode
  m_application->activateWaypointMode();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Set To Track Select Mode
//
// Allows the user to click the widget to view details about a 
// tracked object.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::activateTrackSelectMode() const
{
  // Tell the application to activate the waypoint interaction mode
  m_application->activateTrackSelectMode();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Move Layer Position
//
// Changes the layer position of data in the drawing surface.
// This is caused by dragging and dropping layers between each other
// on the LayerTreeView.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::moveLayerToIndex( const char* moveLayer, const char* targetLayer, int row )
{
  m_application->moveLayerToIndex( moveLayer, targetLayer, row );
  update();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Edit Tree Model
//
// Used to manually edit an option on the LayerTreeView. This
// currently only supports editing the transparency and visibility
// of a layer
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::editTreeModel( const QModelIndex &index, long value )
{
  m_application->editTreeModel( index, value );
  update();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Remove Layer
//
// Requests to the drawing surface in application to remove the
// selected layer in the LayerTreeView.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::removeTreeLayer( std::string layerName, std::string treeAttribute )
{
  m_application->removeTreeLayer( layerName, treeAttribute );
  update();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Map / KML Configuration Dialog
//
// Configures and displays the dialog box form when the user opens
// some TMF data.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::handleDataDialog( const std::string& filename )
{
  std::string ext = filename;
  ext = ext.substr( ext.find_last_of( "." ) );

  std::stringstream ss;

  if( ext == ".map" || ext == ".mpc" )
  {
    ss << "Map_Layer_" << m_layerManager->getLayerCount( TSLDataLayerTypeStaticMapDataLayer ) << '\0';
    m_dataLayerDialog->setDataLabel( "Map Data" );
  }
  else if( ext == ".kml" || ext == ".kmz" )
  {
    ss << "KML_Layer_" << m_layerManager->getLayerCount( TSLDataLayerTypeKMLDataLayer ) << '\0';
    m_dataLayerDialog->setDataLabel( "KML Data" );
  }

  std::string name = ss.str();

  m_dataLayerDialog->setLayerNameBox( name.c_str() );

  m_dataLayerDialog->show();

  m_selectedFile = filename;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TMF Configuration Dialog
//
// Configures and displays the dialog box form when the user opens
// some TMF data.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::handleTMFDialog( const std::string& filename )
{
  std::stringstream ss;
  ss << "TMF_Layer_" << m_layerManager->getLayerCount( TSLDataLayerTypeStandardDataLayer ) << '\0';
  std::string name = ss.str();

  m_tmfLayerDialog->setLayerNameBox( name.c_str() );
  m_tmfLayerDialog->setCoordinateSystemBox( "British National Grid" );
  m_tmfLayerDialog->setStylingText( "TMF Style Sheet" );

  m_tmfLayerDialog->show();

  m_selectedFile = filename;
}

#ifdef HAVE_DIRECT_IMPORT_SDK
void MapLinkWidget::moveDirectImportDataSetToIndex( const char* layerName, const char* scaleBandName, int rowFrom, int rowTo )
{
  m_application->moveDirectImportDataSetToIndex( layerName, scaleBandName, rowFrom, rowTo );
  update();
}

void MapLinkWidget::removeDirectImportDataSet( const char* layerName, const char* scaleBandName, int row )
{
  m_application->removeDirectImportDataSet( layerName, scaleBandName, row );
  update();
}

void MapLinkWidget::beginDirectImportLoad()
{
  // Pause the display while the wizard is open
  if( m_application )
  {
    m_viewUpdater.stop();
  }
}


void MapLinkWidget::endDirectImportLoad( const std::string& layerName )
{
  // Unpause the display, and update the various ui elements for the new
  // layer/surface state
  if( m_application )
  {
    // If the layer hasn't been added to the surface yet then do so
    if( m_application->checkDuplicateLayer( layerName ) == false )
    {
      // Add the layer to the drawing surface and refresh the tree view
      m_layerManager->configureDataLayer( drawingSurface(), layerName );
      getTreeModel()->refreshFromSurface( drawingSurface(), "DirectImport" );
    }

    m_viewUpdater.start( 0 );
  }
}
#endif

//void MapLinkWidget::handleProgressDialog( std::string text )
//{
//  pauseUpdate( true );
//  m_progressDialog->setValue( 0 );
//  m_progressDialog->setLabelText( text.c_str() );
//  m_progressDialog->show();
//}

//void MapLinkWidget::closeProgressDialog()
//{
//  m_progressDialog->setValue( 100 );
//  pauseUpdate( false );
//}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Update Pause
//
// Stops the GUI from being redrawn so that the progress dialog can 
// display properly. (TODO: Is this obsolete?)
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::pauseUpdate( bool val )
{
  m_pauseUpdate = val;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Load Geometry Shape 
//
// From the multiple button functions found in 'mainwindow.cpp', they
// all call this function to 'load in' the type of shape to draw when
// the widget is next clicked.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::selectEntity( std::string entity )
{
  m_selectedEntity = entity;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Attribute Tree Reference
//
// Passes a pointer to the Attribute Tree so that other objects can
// read/write from/to it.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::setAttributeTree( AttributeTreeWidget* atw ) const
{
  m_application->setAttributeTree( atw );
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Tree Model Reference
//
// Retrieves a pointer to the Tree Model so that other objects can
// read/write from/to it.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
TreeModel* MapLinkWidget::getTreeModel()
{
  return m_application->getTreeModel();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Layer Manager Reference
//
// Retrieves a pointer to the LayerManager so that other objects can
// read/write from/to it.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LayerManager* MapLinkWidget::layerManager()
{
  return m_application->layerManager();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Drawing Surface Retrieval 
//
// Passes a pointer to the drawing surface so that another application
// can read/write data from/to it.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#ifdef _MSC_VER
TSLWGLSurface* MapLinkWidget::drawingSurface() const
{
  return m_application->getDrawingSurface();
}
#else
TSLGLXSurface* MapLinkWidget::drawingSurface() const
{
  return m_application->getDrawingSurface();
}
#endif

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Zoom To Layer Extent
//
// This function is the result of clicking the 'Zoom To Extent' button
// on the 'LayerViewTree'. It asks the application to zoom to the
// extent of the selected layer and then proceeds to update some
// status bar widgets and the drawing surface widget.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::zoomToLayerExtent( std::string layerName )
{
  m_application->zoomToLayerExtent( layerName );
  update();
}

#ifdef HAVE_DIRECT_IMPORT_SDK
void MapLinkWidget::zoomToLayerDataSetExtent( DirectImportDataSetTreeItem* dataSet )
{
  m_application->zoomToLayerDataSetExtent( dataSet );
  update();
}

void MapLinkWidget::zoomToLayerScaleBandExtent( DirectImportScaleBandTreeItem* scaleBand )
{
  m_application->zoomToLayerScaleBandExtent( scaleBand );
  update();
}
#endif

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Selected Attribute Data
//
// Sends the selected attribute data currently displayed in the 
// AttributeTreeWidget to Application.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::selectedAttributeData( std::string layerName, std::string layerType ) const
{
  m_application->selectedAttributeData( layerName, layerType );
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Map / KML Data Handling
//
// This function is called when the 'OK' button has been clicked when
// KML or Map data has been opened. It configures the data and adds it to the
// drawing surface and layer tree window.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::dataDialogOKButton()
{
  if( m_application )
  {
    m_viewUpdater.stop();
    std::string layerName = (const char*)(m_dataLayerDialog->getLayerNameBox().toUtf8());
    bool useSharedCache = m_dataLayerDialog->getUseSharedCache();
    int cacheSize = m_dataLayerDialog->getCacheSizeBox();
    std::string bytes = (const char*)(m_dataLayerDialog->getByteSizeBox().toUtf8());

    if( bytes == "MB" )
    {
      cacheSize *= 1024;
    }
    else if (bytes == "GB")
    {
      cacheSize *= 1024 * 1024;
    }
    std::string fileType = m_selectedFile.substr( m_selectedFile.find_last_of( "." ) );

    if (useSharedCache)
    {
      m_layerManager->setUsedCacheSize(cacheSize);
    }
    
    if( fileType == ".map" || fileType == ".mpc" )
    {
      getTreeModel()->refreshFromSurface( drawingSurface(), "Map" );

      // Handle the 2 background maps
      // If 2 maps have already been loaded this will fail
      // and fall through to the generic layer loading below.
      if (loadMap(m_selectedFile.c_str(), useSharedCache))
      {
        return ;
      }
    }

	// if there is no projection map loaded, reject the data.
	bool isProjectedMapsLoaded = m_layerManager->projectionMapsLoaded();
	if (!isProjectedMapsLoaded)
	{
		// add meaningful message for the user.
		QMessageBox box;
		box.setWindowTitle("Failed to load data");
		box.setText(
			"Loading an unprojected data is not permitted without initial realtime projected map.\n"
			"Example of realtime projected maps are shipped as part of MapLink installation: \n"
			"- <Maplink_Installations>\Maps\NaturalEarthRasterRealtime\n"
			"- <Maplink_Installations>\Maps\NaturalEarthBasicRealtime\n\n"
			"For further information about Realtime Projection, please read MapLink Pro Developer's Guide."
		);
		box.exec();
		return;
	}

    if( !m_application->checkDuplicateLayer( layerName ) )
    {
      // Creates Data Layer
      if( m_layerManager->loadLayer( m_selectedFile.c_str(), layerName.c_str(), cacheSize, useSharedCache ) )
      {
        if( !m_layerManager->setProjection( m_layerManager->getCurrentProjection(), drawingSurface() ))
		{
		  // if the map is not runtime projectable, remove it as it does miss up the display. 
		  m_layerManager->removeLayer( layerName );

		  // add meaningful message for the user.
		  QMessageBox box;
		  box.setWindowTitle("Failed to load data");
		  box.setText(
			  "Loading an unprojected data is not permitted.\n"
			  "For further information about Realtime Projection, please read MapLink Pro Developer's Guide."
		  );
		  box.exec();
		  return;
		}

        std::string fileType = m_selectedFile.substr( m_selectedFile.find_last_of( "." ) );

        // Adds Data Layer to the Drawing Surface & refreshes View Tree.
        m_layerManager->configureDataLayer( drawingSurface(), layerName );

        if( fileType == ".map" || fileType == ".mpc" )
        {
          getTreeModel()->refreshFromSurface( drawingSurface(), "Map" );
        }
        else if( fileType == ".kml" || fileType == ".kmz" )
        {
          getTreeModel()->refreshFromSurface( drawingSurface(), "KML" );
          selectedAttributeData( layerName, "KML" );
        }

        m_viewUpdater.start( 0 );
      }
    }
    else
    {
      QMessageBox box;
      box.setText( "Layer name is already being used." );
      box.exec();
    }
  }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TMF Data handling
//
// This function is called when the 'OK' button has been clicked when
// TMF data has been opened. It configures the data and adds it to the
// drawing surface and layer tree window. (Currently not functional, TODO)
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::tmfDialogOKButton()
{
  if( m_application )
  {
    std::string layerName = (const char*)(m_tmfLayerDialog->getLayerNameBox().toUtf8());

    if( !m_application->checkDuplicateLayer( layerName ) )
    {
      //m_application->loadLayer( layerName, m_selectedFile.toUtf8(), 0, m_attributeTree );
    }
    else
    {
      QMessageBox box;
      box.setText( "Layer name is already being used." );
      box.exec();
    }
    m_viewUpdater.start( 0 );
  }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TMF Dialog Browse Button
//
// Allows user to browse the file system for a style sheet file for
// the selected TMF data.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::tmfBrowseButton()
{
  //TODO: File dialog, browse to style sheet.
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Timeout Trigger
//
// Updates the surface frequently so that the realtime reprojection
// can perform at a good framerate
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::idle()
{
  if( !m_pauseUpdate )
  {
    m_layerManager->updateLayers();
    update();
  }
}

void MapLinkWidget::saveToTMF( const char* filename )
{
  if ( m_application )
    m_application->saveToTMF( filename );
}

void MapLinkWidget::loadFromTMF( const char* filename )
{
  if ( m_application )
    m_application->loadFromTMF( filename );
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Qt calls this to initalise OpenGL
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::initializeGL()
{
  if( !m_application )
  {
    return;
    //m_application = new Application( parentWidget() );
  }

  // Platform Specific Setup.
#ifdef X11_BUILD
# if QT_VERSION >= 0x50100
  // Extract the X11 information - QX11Info was removed in Qt5
  QPlatformNativeInterface *native = QGuiApplication::platformNativeInterface();
  Display *display = static_cast<Display*>( native->nativeResourceForWindow( "display", NULL ) );
  Screen *screen = DefaultScreenOfDisplay( display );
  m_application->drawingInfo( display, screen );
# else
  // Qt 5.1 introduced a different version of QX11Info for accessing widget native handles
  int screenNum = DefaultScreen( QX11Info::display() );
  Screen *screen = ScreenOfDisplay( QX11Info::display(), screenNum );

  // pass to the application as we will need for the Drawing Surface
  m_application->drawingInfo( QX11Info::display(), screen );
# endif
#else
  // The MapLink OpenGL drawing surface needs to know the window handle to attach to - 
  // query this from Qt
  WId hWnd = getHWNDForWidget( this );

  // Pass the handle to the application so it can be used by the drawing surface
  m_application->drawingInfo( hWnd );
#endif

  // Tell the application to create the drawing surface and attach it to this widget
  m_application->create();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// resize - informs the drawing surface of any change in size of the window
// Relys on the ResizeAction to maintain the view of the map sensibly
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::resizeGL( int w, int h )
{
  m_application->resize( w, h );
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Qt calls this to draw OpenGL
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::paintGL()
{
  // redraw causes the MapLink drawing surface to draw.
  m_application->redraw();

  displayZoomScaleAndRange();

  // Either MapLink or Qt needs to swap the OpenGL MapLink.
  // See the use of ML_QT_BUFFER_SWAP.
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Mouse & Keyboard handling
//
// We only update the display if MapLink tells us to.
// This function also deals with the majority of status bar widget
// calculations.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::mouseMoveEvent( QMouseEvent *event )
{
  Qt::KeyboardModifiers modifiers = event->modifiers();
  bool shiftPressed = modifiers & Qt::ShiftModifier;
  bool controlPressed = modifiers & Qt::ControlModifier;

  // The MapLink application class doesn't use Qt - convert the mouse button types
  // to the MapLink types
  TSLButtonType button = TSLButtonNone;
  Qt::MouseButton buttonQt = event->button();
  switch( buttonQt )
  {
  case Qt::LeftButton:
    button = TSLButtonLeft;
    break;

  case Qt::MidButton:
    button = TSLButtonCentre;
    break;

  case Qt::RightButton:
    button = TSLButtonRight;
    break;

  default:
    break;
  }

  // Forward the event onto the application
  if( m_application->mouseMoveEvent( button, shiftPressed, controlPressed, event->x(), event->y() ) )
  {
    update(); // We were asked to redraw the display
  }

  // Display the current cursor position in both Map Units and lat/lon in the status bar widgets we were given
  TSLTMC cursorTMCX = 0.0, cursorTMCY = 0.0;
#ifdef _MSC_VER
  TSLWGLSurface* surface = m_application->getDrawingSurface();
#else
  TSLGLXSurface* surface = m_application->getDrawingSurface();
#endif

  if( m_statusBarTMCPosition && surface && surface->getNumDataLayers() > 0 &&
    surface->DUToTMC( event->x(), event->y(), &cursorTMCX, &cursorTMCY ) )
  {
    QString label = QString( tr( "TMC X = %1  Y = %2" ) ).arg( (long)cursorTMCX, 11 ).arg( (long)cursorTMCY, 11 );
    m_statusBarTMCPosition->setText( label );
  }
  else
  {
    m_statusBarTMCPosition->setText( "TMC X = <invalid>  Y = <invalid>" );
  }

  double cursorMUX = 0.0, cursorMUY = 0.0;
  if( m_statusBarMUPosition && surface && surface->getNumDataLayers() > 0 &&
    surface->DUToMU( event->x(), event->y(), &cursorMUX, &cursorMUY ) )
  {

    QString label = QString( tr( "Map Units X = %1  Y = %2" ) ).arg( cursorMUX, 14, 'f', 4 ).arg( cursorMUY, 14, 'f', 4 );
    m_statusBarMUPosition->setText( label );
  }
  else
  {
    m_statusBarMUPosition->setText( "Map Units X = <invalid>  Y = <invalid>" );
  }

  double cursorLat = 0.0, cursorLon = 0.0;
  if( m_statusBarLatLonPosition && surface && surface->getNumDataLayers() > 0 &&
    surface->DUToLatLong( event->x(), event->y(), &cursorLat, &cursorLon ) )
  {
    QString label = QString( "Latitude = %1  Longitude = %2" ).arg( cursorLat, 10, 'f', 6 ).arg( cursorLon, 11, 'f', 6 );
    m_statusBarLatLonPosition->setText( label );
  }
  else
  {
    m_statusBarLatLonPosition->setText( "Latitude = <invalid>  Longitude = <invalid>" );
  }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Mouse Button Press Handling
//
// We only update the display if MapLink tells us to.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::mousePressEvent( QMouseEvent *event )
{
  Qt::KeyboardModifiers modifiers = event->modifiers();
  bool shiftPressed = modifiers & Qt::ShiftModifier;
  bool controlPressed = modifiers & Qt::ControlModifier;
  int x = event->x();
  int y = event->y();
  Qt::MouseButton button = event->button();

  // Forward the event onto the application
  bool redraw = false;
  switch( button )
  {
  case Qt::LeftButton:
    redraw = m_application->onLButtonDown( shiftPressed, controlPressed, x, y, m_selectedEntity );
    break;

  case Qt::MidButton:
    redraw = m_application->onMButtonDown( shiftPressed, controlPressed, x, y );
    break;

  case Qt::RightButton:
    redraw = m_application->onRButtonDown( shiftPressed, controlPressed, x, y );
    break;

  default:
    break;
  }

  if( redraw )
  {
    update(); // We were asked to redraw the display
  }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Mouse Button Release Handling
//
// We only update the display if MapLink tells us to.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::mouseReleaseEvent( QMouseEvent *event )
{
  Qt::KeyboardModifiers modifiers = event->modifiers();
  bool shiftPressed = modifiers & Qt::ShiftModifier;
  bool controlPressed = modifiers & Qt::ControlModifier;
  int x = event->x();
  int y = event->y();
  Qt::MouseButton button = event->button();

  // Forward the event onto the application
  bool redraw = false;
  switch( button )
  {
  case Qt::LeftButton:
    redraw = m_application->onLButtonUp( shiftPressed, controlPressed, x, y );
    break;

  case Qt::MidButton:
    redraw = m_application->onMButtonUp( shiftPressed, controlPressed, x, y );
    break;

  case Qt::RightButton:
    redraw = m_application->onRButtonUp( shiftPressed, controlPressed, x, y );
    break;

  default:
    break;
  }
  if( redraw )
  {
    update(); // We were asked to redraw the display
  }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Mouse Wheel Scroll Handling
//
// We only update the display if MapLink tells us to.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::wheelEvent( QWheelEvent *event )
{
  int delta( event->delta() );

  int timeDelta( m_lastWheelEventTime.elapsed() );
  if( timeDelta > 0 && timeDelta < 25 )
  {
    return;
  }
  m_lastWheelEventTime.restart();

  Qt::KeyboardModifiers modifiers = event->modifiers();
  bool shiftPressed = modifiers & Qt::ShiftModifier;
  bool controlPressed = modifiers & Qt::ControlModifier;
  int x = event->x();
  int y = event->y();

  // Forward the event onto the application
  if( m_application->onMouseWheel( shiftPressed, controlPressed, event->delta(), x, y ) )
  {
    update(); // We were asked to redraw the display
  }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Key Press Handling
//
// We only update the display if MapLink tells us to.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::keyPressEvent( QKeyEvent *event )
{
  Qt::KeyboardModifiers modifiers = event->modifiers();
  bool shiftPressed = modifiers & Qt::ShiftModifier;
  bool controlPressed = modifiers & Qt::ControlModifier;

  // Forward the event onto the application
  if( m_application->onKeyPress( shiftPressed, controlPressed, event->key() ) )
  {
    update(); // We were asked to redraw the display
  }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Event filter - We require this when not directly attached to a
// MainWindow so that we can receive Keyboard, Mouse and Resize
// events.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool MapLinkWidget::eventFilter( QObject *obj, QEvent *event )
{
  if( obj != parent() )
  {
    return false;
  }

  switch( event->type() )
  {
  case QEvent::KeyPress:
  {
    QKeyEvent *ke = static_cast<QKeyEvent *>( event );
    keyPressEvent( ke );
    return false;
  }
  case QEvent::KeyRelease:
  {
    QKeyEvent *ke = static_cast<QKeyEvent *>( event );
    keyReleaseEvent( ke );
    return false;
  }
  case QEvent::MouseButtonPress:
  {
    QMouseEvent *ke = static_cast<QMouseEvent *>( event );
    mousePressEvent( ke );
    return false;
  }
  case QEvent::MouseButtonRelease:
  {
    QMouseEvent *ke = static_cast<QMouseEvent *>( event );
    mouseReleaseEvent( ke );
    return false;
  }
  case QEvent::MouseMove:
  {
    QMouseEvent *ke = static_cast<QMouseEvent *>( event );
    mouseMoveEvent( ke );
    return false;
  }
  case QEvent::Wheel:
  {
    QWheelEvent *ke = static_cast<QWheelEvent *>( event );
    wheelEvent( ke );
    return false;
  }
  case QEvent::Resize:
  {
    QResizeEvent *re = static_cast<QResizeEvent *>( event );

    setGeometry( 0, 0, re->size().width(), re->size().height() );
    return true;
  }
  case QEvent::Close:
  {
    QCloseEvent *ce = static_cast<QCloseEvent *>( event );
    closeEvent( ce );
  }
  default:
    return false;
  }
}

// Direct Import Layer Callbacks
#ifdef HAVE_DIRECT_IMPORT_SDK
void MapLinkWidget::onDeviceCapabilitiesRequired( TSLDeviceCapabilities& capabilities )
{
  const TSLDrawingSurface* ds( drawingSurface() );
  ds->getDeviceCapabilities( capabilities );
}

unsigned int MapLinkWidget::onChoiceOfDrivers( const char* data, const TSLvector<const char*>* drivers )
{
  // This sample may be asked to load any data type.
  // Applications may need to implement this callback in order to select a particular driver.
  // Returning 0 will simply use the first driver which supports the data set.
  return 0;
}

const TSLCoordinateSystem* MapLinkWidget::onNoCoordinateSystem( const TSLDirectImportDataSet* dataSet, TSLDirectImportDriver* driver )
{
  // This implementation is quite simplistic, and will
  // not be suitable for some data types.
  // 
  // Applications should implement this callback in a better manner 
  // for their required data types.

  // Simply check the extent and assume lat/lon if it's less than 360 x 180
  const TSLMUExtent& muExtent( dataSet->extent() );

  if( muExtent == TSLMUExtent() )
  { 
    // Driver could not calculate an extent from the data
    return NULL;
  }

  double xDiff( fabs(muExtent.maxX() - muExtent.minX()) );
  double yDiff( fabs(muExtent.maxY() - muExtent.minY()) );
  // Some data is in lat/lon, yet has an extent just outside the world
  if( xDiff <= 365.0 && yDiff <= 185.0 )
  {
    return TSLCoordinateSystem::findByEPSG( 4326 );
  }
  return NULL;
}

TSLMUExtent MapLinkWidget::onNoExtent( const TSLDirectImportDataSet* dataSet, TSLDirectImportDriver* driver )
{
  // This sample may be asked to load any data type.
  // Applications may need to implement this callback if loading data which doesn't define a spatial extent.
  // Returning a default extent at this point will cause the load to fail.
  return TSLMUExtent();
}

//! This callback will be made when the direct import layer needs to be redrawn
//!
//! Data has either finished processing for display, or a redraw is required to schedule 
//! tiles.
void MapLinkWidget::requestRedraw()
{
  // Request an update/redraw of the application
  // This callback must use a signal/slot to request a redraw
  // as this callback may be made from any thread.
  update();
}

// The following callbacks are for information purposes, in situations
// where an application needs to display a progress bar during data loading.
//
// If no progress bar functionality is implemented data will appear in the 
// direct import layer once it has been loaded.
void MapLinkWidget::onDataSetLoadScheduled( const TSLDirectImportDataSet* dataSet, unsigned int numProcessingTotal )
{
  std::cout << "MapLinkWidget::onDataSetLoadScheduled: " << dataSet->name() << ": " << numProcessingTotal << " data sets being processed" << std::endl;
}

void MapLinkWidget::onDataSetLoadCancelled( const TSLDirectImportDataSet* dataSet, unsigned int numProcessingTotal )
{
  std::cout << "MapLinkWidget::onDataSetLoadCancelled: " << dataSet->name() << ": " << numProcessingTotal << " data sets being processed" << std::endl;
}

void MapLinkWidget::onDataSetLoadComplete( const TSLDirectImportDataSet* dataSet, unsigned int numProcessingTotal )
{
  std::cout << "MapLinkWidget::onDataSetLoadComplete: " << dataSet->name() << ": " << numProcessingTotal << " data sets being processed" << std::endl;
}

void MapLinkWidget::onTileLoadScheduled( const TSLDirectImportDataSet* dataSet, unsigned int numScheduled, unsigned int numProcessing, unsigned int numProcessingTotal )
{
  std::cout << "MapLinkWidget::onTileLoadScheduled: " << dataSet->name() << ": " << numScheduled << " tiles scheduled: " << numProcessing << " tiles processing for dataset: " << numProcessingTotal << " tiles processing in total" << std::endl;
}

void MapLinkWidget::onTileLoadCancelled( const TSLDirectImportDataSet* dataSet, unsigned int numProcessing, unsigned int numProcessingTotal )
{
  std::cout << "MapLinkWidget::onTileLoadCancelled: " << dataSet->name() << ": " << numProcessing << " tiles processing for dataset: " << numProcessingTotal << " tiles processing in total" << std::endl;
}

void MapLinkWidget::onTileLoadFailed( const TSLDirectImportDataSet* dataSet, unsigned int numProcessing, unsigned int numProcessingTotal )
{
  std::cout << "MapLinkWidget::onTileLoadFailed: " << dataSet->name() << ": " << numProcessing << " tiles processing for dataset: " << numProcessingTotal << " tiles processing in total" << std::endl;
}

void MapLinkWidget::onTileLoadComplete( const TSLDirectImportDataSet* dataSet, unsigned int numProcessing, unsigned int numProcessingTotal )
{
  std::cout << "MapLinkWidget::onTileLoadComplete: " << dataSet->name() << ": " << numProcessing << " tiles processing for dataset: " << numProcessingTotal << " tiles processing in total" << std::endl;

  if( numProcessing == 0 && m_layerManager )
  {
    // If a data set has completely finished loading the direct import layer
    // may contain new features.
    // Reload the direct import layer into the declutter panel to show these.
    // Note that this will make use of a signal/slot to ensure thread safety.
    // Callbacks from the TSLDirectImportDataLayer may happen in any thread, including the foreground/UI thread.
    QString layerName( m_layerManager->getDirectImportName().c_str() );
    emit updateDeclutterList( layerName );
  }
}

#endif
