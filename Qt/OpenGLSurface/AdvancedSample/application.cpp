/****************************************************************************
                  Copyright (c) 2016-2017 by Envitia Group PLC.
****************************************************************************/

#include <QtGui>
#include <QMessageBox>

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#include "application.h"
#include "maplinkwidget.h"
#include "MapLinkIMode.h"
#include "layermanager.h"
#include "AddWaypointInteractionMode.h"
#include "ui/tracks/trackselectionmode.h"
#include "tracks/trackmanager.h"
#include "Util.h"

#ifdef HAVE_DIRECT_IMPORT_SDK
# include "ui/layertreeview/directimportscalebandtreeitem.h"
# include "ui/layertreeview/directimportdatasettreeitem.h"
#endif

#include "tslkmldatalayer.h"

#include "ui/attributetreewidget/attributetreewidget.h"

#include <iostream>

// Interaction mode IDs - these can be any numbers
#define ID_TOOLS_ZOOM                   1
#define ID_TOOLS_PAN                    2
#define ID_TOOLS_GRAB                   3
#define ID_TOOLS_WAYPOINT               4
#define ID_TOOLS_TRACK_SELECT           5

// Controls how far the drawing surface rotates in one key press - this value is in radians
static const double rotationIncrement = M_PI / 360.0;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Constructor
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Application::Application( QWidget *parent )
  : m_drawingSurface( NULL )
#ifdef _MSC_VER
  , m_window( NULL )
#else
  , m_display( NULL )
  , m_screen( NULL )
#endif
  , m_modeManager( NULL )
  , m_widgetWidth( 0 )
  , m_widgetHeight( 0 )
  , m_surfaceRotation( 0.0 )
  , m_parentWidget( parent )
  , m_attributeTree( NULL )
  , m_treeModel( NULL )
  , m_selectedAttributeLayer( "" )
{
  // Clear the error stack so that we can get the errors that occurred here.
  TSLErrorStack::clear();

  // Initialise the drawing surface data files.
  //
  // This call uses the TSLUtilityFunctions::getMapLinkHome method to determine
  // where MapLink is currently installed.  It then proceeds to load the
  // following files : tsllinestyles.dat, tslfillstyles.dat, tslfonts.dat, tslsymbols.dat
  // and tslcolours.dat
  //
  // When deploying your application, pass in a full path to the directory containing
  // these files.
  bool res = TSLDrawingSurface::loadStandardConfig();

  // For AP66A/2525B symbols to display correctly it is also necessary to load the APP6A symbol file
  if( !TSLDrawingSurface::setupSymbols( "tslsymbolsAPP6A.dat" ) )
  {
    res = false;
  }

  // When deploying pass the full path to tsltransforms.dat
  TSLCoordinateSystem::loadCoordinateSystems();

  // Log the contents of the error stack to the terminal
  const char* errorMessage( TSLErrorStack::errorString() );
  if( errorMessage )
  {
    std::cerr << "Errors during MapLink initialisation: " << errorMessage << std::endl;
    TSLErrorStack::clear();
  }

  if( !res )
  {
    QMessageBox::critical( m_parentWidget, "MapLink Initialisation Failed", 
        "Failed to load the MapLink standard configuration files. Please see the MapLink Pro Developer's Guide" );
    m_parentWidget->close();
  }

  m_layerManager = new LayerManager();

  m_treeModel = new TreeModel( "" );
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Destructor
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Application::~Application()
{
  // Clean up by destroying the objects we created
  if( m_treeModel )
  {
    delete m_treeModel;
    m_treeModel = NULL;
  }

  if( m_modeManager )
  {
    delete m_modeManager;
    m_modeManager = NULL;
  }

  if( m_layerManager )
  {
    delete m_layerManager;
    m_layerManager = NULL;
  }

  if( m_drawingSurface )
  {
    delete m_drawingSurface;
    m_drawingSurface = NULL;
  }

  // Beyond this point MapLink will no longer be used - clear up all static data.
  // Once this is done no MapLink functions or classes can be used.
  TSLDrawingSurface::cleanup( true );
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Create
//
// This function initialises the drawing surface.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Application::create()
{
  if( m_drawingSurface )
  {
    return;  // We have already been called before!
  }

  // Tell the drawing surface whether it will need to perform buffer swaps, or whether
  // it is handled externally. See the constructor of MapLinkWidget.
  TSLOpenGLSurfaceCreationParameters creationOptions;
  creationOptions.swapBuffersManually( ML_QT_BUFFER_SWAP );

#ifdef _MSC_VER
  HGLRC context = wglGetCurrentContext();
  m_drawingSurface = new TSLWGLSurface( (HWND)m_window, false, context, creationOptions );
#else
  // Get the active OpenGL context to attach the drawing surface to
  GLXContext context = glXGetCurrentContext();
  GLXDrawable drawable = glXGetCurrentDrawable();

  // Create the Accelerated Surface object
  m_drawingSurface = new TSLGLXSurface( m_display, m_screen, drawable, context, creationOptions );
#endif

  if( !m_drawingSurface->context() )
  {
    // The drawing surface failed to attach to the context - show the error from the error stack.
    // This means the drawing surface cannot be used, so exit the sample
    const char *msg = TSLErrorStack::errorString();
    if( msg )
    {
      QMessageBox::critical( m_parentWidget, "Failed to attach drawing surface", msg );
    }
    else
    {
      QMessageBox::critical( m_parentWidget, "Failed to attach drawing surface", "Unknown error" );
    }
    m_parentWidget->close();
    return;
  }

  // Set the physical capabilities of the display
  // This is an optional step, and these values may be determined automatically by the drawing surface. 
  // Applications should verify that m_drawingSurface->getDeviceCapabilities returns the correct values for their
  // display, and override if necessary.
  //
  // These values must be correct in order to calculate map scales accurately.
  //m_drawingSurface->setDeviceCapabilities( 344, 194, 1920, 1080 );

  // Enable dynamic arc map support.
  m_drawingSurface->setOption( TSLOptionDynamicArcSupportEnabled, true );

  // We cannot call wndResize on the drawing surface yet as we don't know the size of the widget

  // Now create and initialise the mode manager and modes
  m_modeManager = new TSLInteractionModeManagerGeneric( this, m_drawingSurface );

  // Add the three interaction mode types to the manager - the zoom mode is the default
  m_modeManager->addMode( new TSLInteractionModeZoom( ID_TOOLS_ZOOM ), true );
  m_modeManager->addMode( new TSLInteractionModePan( ID_TOOLS_PAN ), false );
  m_modeManager->addMode( new TSLInteractionModeGrab( ID_TOOLS_GRAB ), false );

  m_waypointMode = new AddWaypointInteractionMode( ID_TOOLS_WAYPOINT );
  m_modeManager->addMode( m_waypointMode, false );

  m_modeManager->addMode( new TrackSelectionMode( ID_TOOLS_TRACK_SELECT ), false );

  m_modeManager->setCurrentMode( ID_TOOLS_GRAB );

  // Display any errors that have occurred
  const char *errorMsg = TSLErrorStack::errorString();
  if( errorMsg )
  {
    QMessageBox::warning( m_parentWidget, "Cannot initialise view", errorMsg );
    TSLErrorStack::clear();
  }

}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Resize
//
// Readjusts the drawing surface when the application window
// size has been modified.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Application::resize( int width, int height )
{
  if( m_drawingSurface )
  {
    // Inform the drawing surface of the new window size,
    // attempting to keep the top left corner the same.
    // Do not ask for an automatic redraw since we will get a call to redraw() to do so
    m_drawingSurface->wndResize( 0, 0, width, height, false, TSLResizeActionMaintainTopLeft );
  }
  if( m_modeManager )
  {
    m_modeManager->onSize( width, height );
  }
  m_widgetWidth = width;
  m_widgetHeight = height;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Redraw
//
// Requests an update to the drawing surface and refreshes the
// Attribute Tree.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Application::redraw()
{
  if( m_drawingSurface )
  {
    TrackManager::instance().preDraw( m_drawingSurface );

    // Draw the map to the widget
    m_drawingSurface->drawDU( 0, 0, m_widgetWidth, m_widgetHeight, true );

    // Don't forget to draw any echo rectangle that may be active.
    if( m_modeManager )
    {
      m_modeManager->onDraw( 0, 0, m_widgetWidth, m_widgetHeight );
    }

    TrackManager::instance().postDraw( m_drawingSurface );

    reloadAttributeTree();
  }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Mouse Move Event
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool Application::mouseMoveEvent( unsigned int buttonPressed, bool shiftPressed, bool controlPressed, int mx, int my )
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if( m_modeManager )
  {
    // Request a redraw if the interaction hander requires it
    return m_modeManager->onMouseMove( (TSLButtonType)buttonPressed, mx, my, shiftPressed, controlPressed );
  }
  return false;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Left Mouse Button Pressed
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool Application::onLButtonDown( bool shiftPressed, bool controlPressed, int X, int Y, std::string entity )
{
  if( controlPressed && m_drawingSurface )
  {

    // Convert using Runtime Coordinate System from Device Units to TMC.
    TSLTMC x, y;
    double lat, lon;
    if (!m_drawingSurface->DUToTMC( X, Y, &x, &y ))
    {
      QMessageBox::information( NULL, "Selected position outside valid projection range",
        "The selected position is too far from the projection centre to be used as the center point for geometry", QMessageBox::Ok );
      return false;
    }

    // Convert using Runtime Coordinate System from TMC to latitude & longitude
    if (!m_drawingSurface->TMCToLatLong( x, y, &lat, &lon ))
    {
      QMessageBox::information( NULL, "Selected position outside valid projection range",
        "The selected position is too far from the projection centre to be used as the center point for geometry", QMessageBox::Ok );
      return false;
    }

    if( entity == "Polyline" )
    {
      createPolyline( lat, lon );
      return true;
    }
    else if( entity == "Polygon" )
    {
      createPolygon( lat, lon );
      return true;
    }
    else if( entity == "Text" )
    {
      createText( lat, lon );
      return true;
    }
    else if( entity == "VectorSymbol" )
    {
      createVectorSymbol( lat, lon );
      return true;
    }
    else if( entity == "RasterSymbol" )
    {
      createRasterSymbol( lat, lon );
      return true;
    }
    else if( entity == "Arc" )
    {
      createArc( lat, lon );
      return true;
    }
    else if( entity == "Ellipse" )
    {
      createEllipse( lat, lon );
      return true;
    }
    else if( entity == "GeoPolyline" )
    {
      createGeoPolyline( lat, lon );
      return true;
    }
    else if( entity == "GeoPolygon" )
    {
      createGeoPolygon( lat, lon );
      return true;
    }
    else if( entity == "GeoText" )
    {
      createGeoText( lat, lon );
      return true;
    }
    else if( entity == "GeoVectorSymbol" )
    {
      createGeoVectorSymbol( lat, lon );
      return true;
    }
    else if( entity == "GeoRasterSymbol" )
    {
      createGeoRasterSymbol( lat, lon );
      return true;
    }
    else if( entity == "GeoArc" )
    {
      createGeoArc( lat, lon );
      return true;
    }
    else if( entity == "GeoEllipse" )
    {
      createGeoEllipse( lat, lon );
      return true;
    }
  }

  // If the user is in the middle of an interaction, pass the event onto the handler
  if( m_modeManager )
  {

    // Request a redraw if the interaction hander requires it
    return m_modeManager->onLButtonDown( X, Y, shiftPressed, controlPressed );
  }
  return false;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Middle Mouse Button Pressed
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool Application::onMButtonDown( bool shiftPressed, bool controlPressed, int X, int Y )
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if( m_modeManager )
  {
    // Request a redraw if the interaction hander requires it
    return m_modeManager->onMButtonDown( X, Y, shiftPressed, controlPressed );
  }
  return false;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Right Mouse Button Pressed
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool Application::onRButtonDown( bool shiftPressed, bool controlPressed, int mx, int my )
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if( m_modeManager )
  {
    // Request a redraw if the interaction hander requires it
    return m_modeManager->onRButtonDown( mx, my, shiftPressed, controlPressed );
  }
  return false;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Left Mouse Button Released
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool Application::onLButtonUp( bool shiftPressed, bool controlPressed, int mx, int my )
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if( m_modeManager )
  {
    // Request a redraw if the interaction hander requires it
    return m_modeManager->onLButtonUp( mx, my, shiftPressed, controlPressed );
  }
  return false;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Middle Mouse Button Released
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool Application::onMButtonUp( bool shiftPressed, bool controlPressed, int mx, int my )
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if( m_modeManager )
  {
    // Request a redraw if the interaction hander requires it
    return m_modeManager->onMButtonUp( mx, my, shiftPressed, controlPressed );
  }
  return false;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Right Mouse Button Released
//
// This function allows the user to pick data from the MapLink Widget
// when they right-click on it. Data in the general region of the click
// will be displayed on the AttributeTree.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool Application::onRButtonUp( bool /*shiftPressed*/, bool /*controlPressed*/, int mx, int my )
{
  //
  // Picks the closest entity and displays its parent if it is a base geometry element.
  //
  // In the case of this sample, the right mouse button is unused
  // - mapped to perform a pick operation on the kml layer and 
  //   display the results in the attribute widget.
  if( m_attributeTree && m_drawingSurface )
  {
    m_attributeTree->clear();

    for( int i = 0; i < 4; i++ ) // Cycle through all layers and pick data from them all.
    {
      TSLPickResultSet* pickResults = m_drawingSurface->pick( "kml", mx, my, 0, 1 );
      if (!pickResults)
        continue;

      for( int j = 0; j < pickResults->numResults(); ++j )
      {
        const TSLPickResult* pickResult = pickResults->getResult( j );
        if( pickResult && pickResult->queryType() == TSLPickCustom )
        {
          TSLPickResultCustom* pickResultCustom = (TSLPickResultCustom*)pickResult;
          TSLKMLPickResult* kmlPickResult = (TSLKMLPickResult*)pickResultCustom->getClientCustomPickResult();
          if (!kmlPickResult)
            continue;

          const TSLEntity* entity = kmlPickResult->entity();
          if (!entity)
            continue;

          // Entities created from kml geometry objects, such as lineString or Point
          // don't contain any attributes, as they are children of place marks
          // This displays the parent entitySet, which should be a place mark containing
          // all of the relevant attribute information.
          if( TSLEntitySet::isEntitySet( entity ) )
          {
            unsigned int numAdded(0);
            m_attributeTree->addEntitySet( (TSLEntitySet*)entity, numAdded );
          }
          else
          {
            if( entity->parent() )
            {
              unsigned int numAdded(0);
              m_attributeTree->addEntitySet( entity->parent(), numAdded );
            }
          }
        }
      }
    }
  }
  return false;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Key Pressed
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool Application::onKeyPress( bool, bool, int keySym )
{
  // The left and right arrow keys allow the drawing surface to be rotated
  switch( keySym )
  {
  case Qt::Key_Left:
    m_surfaceRotation += rotationIncrement;
    if (m_drawingSurface)
    {
      m_drawingSurface->rotate(m_surfaceRotation);
    }
    return true;

  case Qt::Key_Right:
    m_surfaceRotation -= rotationIncrement;
    if (m_drawingSurface)
    {
      m_drawingSurface->rotate(m_surfaceRotation);
    }
    return true;

  default:
    break;
  }
  return false;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Mouse Wheel Scrolled
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool Application::onMouseWheel( bool, bool, short zDelta, int X, int Y )
{
  if( m_modeManager )
  {
    // Request a redraw if the interaction hander requires it
    return m_modeManager->onMouseWheel( zDelta, X, Y );
  }
  return false;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Zoom In
//
// Allows the user to click the widget to zoom in.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool Application::zoomIn()
{
  if (m_modeManager)
  {
    return m_modeManager->zoomIn(30);
  }
  return false;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Zoom Out
//
// Allows the user to click the widget to zoom out.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool Application::zoomOut()
{
  if (m_modeManager)
  {
    return m_modeManager->zoomOut(30);
  }
  return false;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Reset View
//
// Reset the view to the full extent of the map being loaded
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Application::resetView()
{
  if( m_drawingSurface )
  {
    // Reset the drawing surface rotation as well
    m_surfaceRotation = 0.0;
    m_drawingSurface->rotate( m_surfaceRotation );
    m_drawingSurface->reset( false );
  }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Pan Mode
//
// Allows the user to click the widget to pan to that position as the
// centre of the view.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Application::activatePanMode()
{
  if (m_modeManager)
  {
    // Tell the application to activate the pan interaction mode
    m_modeManager->setCurrentMode(ID_TOOLS_PAN);
  }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Grab Mode
//
// Allows the user to click and drag the widget to change the position
// of the view.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Application::activateGrabMode()
{
  if (m_modeManager)
  {
    // Tell the application to activate the grab interaction mode
    m_modeManager->setCurrentMode(ID_TOOLS_GRAB);
  }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Zoom mode
//
// Allows the user to click and drag the widget to zoom to the selected
// extent.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Application::activateZoomMode()
{
  if (m_modeManager)
  {
    // Tell the application to activate the zoom interaction mode
    m_modeManager->setCurrentMode(ID_TOOLS_ZOOM);
  }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Set To Waypoint Mode
//
// Allows the user to click the widget to redirect the projection
// object to that location.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Application::activateWaypointMode()
{
  if (m_modeManager)
  {
    // Tell the application to activate the zoom interaction mode
    m_modeManager->setCurrentMode(ID_TOOLS_WAYPOINT);
  }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Set To Track Select Mode
//
// Allows the user to click the widget to view details about a 
// tracked object.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Application::activateTrackSelectMode()
{
  // Activate the track selection interaction mode
  if( m_modeManager )
  {
    m_modeManager->setCurrentMode( ID_TOOLS_TRACK_SELECT ) ;
  }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Layer Manager Reference
//
// Retrieves a pointer to the LayerManager so that other objects can
// read/write from/to it.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LayerManager* Application::layerManager()
{
  return m_layerManager;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Zoom To Layer Extent
//
// This function gathers the information it requires from the
// Data Layer to be able to zoom to its extent.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Application::zoomToLayerExtent( const std::string& layerName )
{
  // Button does not work for these layers.
  if( layerName == "track" || layerName == "fps" || layerName == "EntityLayer" || layerName == "TracksLayer" )
  {
    return;
  }
  if (!m_drawingSurface)
  {
    return;
  }

  TSLDataLayer* datalayer = m_drawingSurface->getDataLayer( layerName.c_str() );
  if (!datalayer)
  {
    return;
  }
  
  // Get the TMC extent of the Layer
  TSLTMC x1, y1, x2, y2;
  datalayer->getTMCExtent( &x1, &y1, &x2, &y2 );

  TSLEnvelope extent(x1, y1, x2, y2);

  zoomToExtent( datalayer, extent );
}

#ifdef HAVE_DIRECT_IMPORT_SDK
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Zoom To Layer Data Set Extent
//
// This function gathers the information it requires from 
// the Direct Import Data Set to be able to zoom to its extent.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Application::zoomToLayerDataSetExtent( DirectImportDataSetTreeItem* dataSetTreeItem )
{
  QString parentName = dataSetTreeItem->parentItem()->parentItem()->data( 0 ).toString();
  TSLDataLayer* datalayer = m_drawingSurface->getDataLayer( parentName.toUtf8() );
  if (!datalayer)
  {
    return;
  }

  TSLDirectImportDataSet* dataSet = dataSetTreeItem->getDataSet();
  
  zoomToExtent( datalayer, dataSet->extentTMC() );
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Zoom To Layer Scale Band Extent
//
// This function gathers the information it requires from 
// the Direct Import Scale Band to be able to zoom to its extent. 
// This function also combines the extents of all the datasets 
// in the scale band.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Application::zoomToLayerScaleBandExtent( DirectImportScaleBandTreeItem* scaleBandTreeItem )
{
  QString parentName = scaleBandTreeItem->parentItem()->data( 0 ).toString();
  TSLDataLayer* datalayer = m_drawingSurface->getDataLayer( parentName.toUtf8() );
  if (!datalayer)
  {
    return;
  }

  TSLDirectImportScaleBand* scaleBand = scaleBandTreeItem->getScaleBand();

  TSLTMC x1, y1, x2, y2;
  unsigned int numDataSets = scaleBand->numDataSets();
  TSLDirectImportDataSet* dataSet;
  for(int i = 0; i < numDataSets; i++)
  {
    dataSet = scaleBand->getDataSet(i);
    const TSLEnvelope& env = dataSet->extentTMC();
    if( i != 0 )
    {
      if( env.xMin() < x1 )
      {
        x1 = env.xMin();
      }
      if( env.yMin() < y1 )
      {
        y1 = env.yMin();
      }
      if( env.xMax() > x2 )
      {
        x2 = env.xMax();
      }
      if( env.yMax() > y2 )
      {
        y2 = env.yMax();
      }
    }
    else
    {
      x1 = env.xMin();
      y1 = env.yMin();
      x2 = env.xMax();
      y2 = env.yMax();
    }
  }

  TSLEnvelope extent(x1, y1, x2, y2);
  
  zoomToExtent( datalayer, extent );
}
#endif

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Zoom To Extent
//
// This function is the result of clicking the 'Zoom To Extent' button
// on the 'LayerViewTree'. It asks the application to zoom to the
// extent of the selected layer and then proceeds to update some
// status bar widgets and the drawing surface widget.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Application::zoomToExtent( TSLDataLayer* datalayer, TSLEnvelope extent )
{
  // Convert to lat/lon so that we can work out the center of the data.
  double lat1, lon1, lat2, lon2;
  datalayer->TMCToLatLong( extent.xMin(), extent.yMin(), &lat1, &lon1 );
  datalayer->TMCToLatLong( extent.xMax(), extent.yMax(), &lat2, &lon2 );

  double mLat = lat1 + ((lat2 - lat1) / 2.0) ;
  double mLon = lon1 + ((lon2 - lon1) / 2.0 );

  // Work out the Range so that we can see all the data in the layer.
  // Range has to be in map units (MU) for the current projection the drawing surface is using.
  // NOTE: The range is specified in the Map Units of the Display - not necessarily the data.

  // range is in TMC for the layer not the display.
  int64_t rangeInTMC = abs(std::max((int64_t)extent.xMax() - (int64_t)extent.xMin(), (int64_t)extent.yMax() - (int64_t)extent.yMin())); // make sure we get the maximum range so we see all the data.

  // We have to convert the range defined in the layers TMC into a common set of units we
  // can use for the display.
  // The assumption is that the center of the map is at the equator and that there is no TMC offset.
  datalayer->TMCToLatLong(0, 0, &lat1, &lon1);
  datalayer->TMCToLatLong(0, rangeInTMC, &lat2, &lon2);

  // convert the lat/lon values into Map Unit of the display.
  double pos1X, pos1Y, pos2X, pos2Y;
  m_drawingSurface->latLongToMU(lat1, lat1, &pos1X, &pos1Y);
  m_drawingSurface->latLongToMU(lat2, lat2, &pos2X, &pos2Y);

  // calculate the maximum range using the display map units calculated on the previous couple of lines
  double range = std::max( pos2X - pos1X, pos2Y - pos1Y);

  m_drawingSurface->setViewedLatLongRange( mLat, mLon, range, true, true, false );
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Move Layer Position
//
// Changes the layer position of data in the drawing surface.
// This is caused by dragging and dropping layers between each other
// on the LayerTreeView.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Application::moveLayerToIndex( const char* moveLayer, const char* targetLayer, int row )
{
  if (!m_drawingSurface)
    return;

  if( row == 0 )
  {
    m_drawingSurface->sendToBackOf( moveLayer, targetLayer );
  }
  else
  {
    m_drawingSurface->bringInFrontof( moveLayer, targetLayer );
  }

  if (m_treeModel)
    m_treeModel->refreshFromSurface( m_drawingSurface );
}

#ifdef HAVE_DIRECT_IMPORT_SDK
void Application::moveDirectImportDataSetToIndex( const char* layerName, const char* scaleBandName, int rowFrom, int rowTo )
{
  if (m_layerManager && m_treeModel)
  {
    m_layerManager->moveDirectImportDataSetToIndex(layerName, scaleBandName, rowFrom, rowTo);
    m_treeModel->refreshFromSurface(m_drawingSurface);
  }
}

void Application::removeDirectImportDataSet( const char* layerName, const char* scaleBandName, int row )
{
  if (m_layerManager && m_treeModel)
  {
    m_layerManager->removeDirectImportDataSet(layerName, scaleBandName, row);
    m_treeModel->refreshFromSurface(m_drawingSurface);
  }
}
#endif

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Edit Tree Model
//
// Used to manually edit an option on the LayerTreeView. This
// currently only supports editing the transparency and visibility
// of a layer.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Application::editTreeModel( const QModelIndex &index, long value )
{
  // Retrieve data from the provided index of the LayerTreeView.
  TreeItem *item = static_cast<TreeItem*>( index.internalPointer() );
  if (!item)
  {
    return;
  }

  QString layerName = item->parentItem()->data( 0 ).toString();

  if( item->data( 0 ) == "Visible" )
  {
    item->toggleChecked(); // Toggles the checkbox
    if (m_drawingSurface)
      m_drawingSurface->setDataLayerProps( layerName.toUtf8(), TSLPropertyVisible, value );
  }
  else if( item->data( 0 ) == "Geometry Streaming" )
  {
    item->toggleChecked(); // Toggles the checkbox
    if (m_drawingSurface)
    {
      m_drawingSurface->setDataLayerProps( layerName.toUtf8(), TSLPropertyGeometryStreaming, value );

      TSLDataLayer* datalayer = m_drawingSurface->getDataLayer( layerName.toUtf8() );
      if ( datalayer )
      {
        datalayer->notifyChanged( true );
      }
    }
  }
  else if( item->data( 0 ) == "Transparency" )
  {
    if (m_drawingSurface)
      m_drawingSurface->setDataLayerProps( layerName.toUtf8(), TSLPropertyTransparency, value );
  }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Tree Model Reference
//
// Retrieves a pointer to the Tree Model so that other objects can
// read/write from/to it.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
TreeModel* Application::getTreeModel()
{
  return m_treeModel;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Attribute Tree Reference
//
// Passes a pointer to the Attribute Tree so that other objects can
// read/write from/to it.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Application::setAttributeTree( AttributeTreeWidget* atw )
{
  m_attributeTree = atw;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Set-up Attribute Tree
//
// Prepares the name and layer type of the layer that will have
// its data listed in the Attribute Tree.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Application::selectedAttributeData( const std::string& layerName, const std::string& layerType )
{
  if( m_attributeTree ) // Clears the previously loaded data from the tree.
  {
    m_attributeTree->clear();
    m_attributeTree->initialised(false);
  }

  m_selectedAttributeLayer = layerName;
  m_selectedAttributeType = layerType;
}

// Display Attribute Tree Data
//
// This function takes the layer name and type defined by the 
// 'selectedAttributeData' function and lists vector data from the layer
// in the attribute tree.
// This is not supported for all data layers
void Application::reloadAttributeTree()
{
  if( m_selectedAttributeLayer != "" )
  {
    std::string value = m_selectedAttributeLayer;
    if( m_attributeTree && !m_attributeTree->initialised() )
    {
      // Query the DataLayer from the drawing surface
      TSLDataLayer* dataLayer( m_drawingSurface->getDataLayer( m_selectedAttributeLayer.c_str() ) );
      if( !dataLayer )
      {
        return;
      }

      // KML Layers consist of multiple sub-datalayers
      // The first of these should be a TSLStandardDataLayer
      TSLDataLayerTypeEnum layerType( dataLayer->layerType() );
      if( layerType == TSLDataLayerTypeKMLDataLayer )
      {
        TSLKMLDataLayer* kmlLayer( reinterpret_cast<TSLKMLDataLayer*>( dataLayer ) );
        if( !kmlLayer )
        {
          return;
        }
        dataLayer = kmlLayer->getLayer(0);
        layerType = dataLayer->layerType();
        if( layerType != TSLDataLayerTypeStandardDataLayer )
        {
          return;
        }
      }

      // The root entity set can be queried from standard datalayers directly
      if( layerType == TSLDataLayerTypeStandardDataLayer )
      {
        TSLStandardDataLayer* standardLayer( reinterpret_cast<TSLStandardDataLayer*>( dataLayer ) );
        if( !standardLayer )
        {
          return;
        }
        const TSLEntitySet* set( standardLayer->entitySet() );
        if( set )
        {
          unsigned int numAdded;
          m_attributeTree->addEntitySet( set, numAdded );
        }
      }
      else
      {
        // Query from any datalayer using entity iterators
        // Note that this may take a long time, based on the amount of data in the layer
        // often it's a better idea to only query data in a reduced extent, or for the 
        // attribute widget to list a lot less information.
        // Entity iterators may not be supported on all layers.
        TSLEntityIterator* it( dataLayer->getEntityIterator( NULL ) );
        if( !it )
        {
          return;
        }
        m_attributeTree->addEntityIterator( it );
      }
      m_attributeTree->initialised(true);

    }
  }
  emit m_treeModel->closeProgressDialog();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Remove Layer
//
// This function takes the layer name and the second attribute from
// the LayerTreeView and attempts to remove the layer.
//
// The 'treeAttribute' variable will often contain the type of data
// that the layer has been given, such as 'Map', 'KML' or 'DirectImport'.
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Application::removeTreeLayer( const std::string& name, const std::string& treeAttribute )
{
  if (!m_drawingSurface || !m_layerManager || !m_treeModel)
  {
    return;
  }

  // Ensures that the only coordinate providing layer is not deleted when other layers are present.
  const TSLDataLayer* namedLayer( m_drawingSurface->getDataLayer( name.c_str() ) );
  if( !namedLayer )
  {
    return;
  }

  const TSLDataLayer* coordinateProvidingLayer( m_drawingSurface->getCoordinateProvidingLayer() );
  if( coordinateProvidingLayer && coordinateProvidingLayer == namedLayer )
  {
    if( m_drawingSurface->getNumDataLayers() > 1 )
    {
      // Don't remove layer
      QMessageBox box;
      box.setText( "Cannot delete last coordinate providing layer whilst it is being relied on by another layer." );
      box.exec();
      return;
    }
  }
  else if( treeAttribute == "ProjectionSample" ) // If part of the projection sample is deleted, all layers related to it are destroyed too.
  {
    int numLayers = m_drawingSurface->getNumDataLayers();

    TreeItem* root = m_treeModel->getRootItem();

    for( int i = 0; i < numLayers; ++i )
    {
      if( root && root->child( i )->data( 1 ).toString() == "ProjectionSample" )
      {
        m_drawingSurface->removeDataLayer( root->child( i )->data( 0 ).toString().toStdString().c_str() );
      }
    }
    m_treeModel->refreshFromSurface( m_drawingSurface );
    m_treeModel->layoutChanged();
    return;
  }
  else if( treeAttribute == "Background" )
  {
    m_drawingSurface->removeDataLayer( name.c_str() );
    if( name == "TracksLayer" )
    {
      TrackManager::instance().createTracks( 0, 0 );
    }
    m_treeModel->refreshFromSurface( m_drawingSurface );
    m_treeModel->layoutChanged();
    return;
  }

  m_drawingSurface->removeDataLayer( name.c_str() );
  m_layerManager->removeLayer( name );

  m_treeModel->refreshFromSurface( m_drawingSurface );
  m_treeModel->layoutChanged();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TSLInteractionModeRequest callback function
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Application::resetMode( TSLInteractionMode *, TSLButtonType, TSLDeviceUnits, TSLDeviceUnits )
{
  // Do nothing
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TSLInteractionModeRequest callback function
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Application::viewChanged( TSLDrawingSurface* )
{
  // Do nothing
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Duplicate Layer Check
//
// This function queries the drawing surface for a layer using
// the provided name, and returns true if the layer exists.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool Application::checkDuplicateLayer( const std::string& layerName ) const
{
  if (m_drawingSurface && m_drawingSurface->getDataLayer(layerName.c_str()))
  {
    return true;
  }
  return false;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Waypoint Mode
//
// Selects the 'Waypoint Mode' to be able to redirect the projection
// object.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
AddWaypointInteractionMode* Application::getWaypointMode() const
{
  return m_waypointMode;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Drawing Info
//
// Sets the properties of the monitor being used.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#ifdef _MSC_VER
void Application::drawingInfo( WId window )
{
  m_window = window;
}
#else
void Application::drawingInfo( Display *display, Screen *screen )
{
  m_display = display;
  m_screen = screen;
}
#endif


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Create Polyline
//
// Creates a polyline based on feature rendering from the 
// LayerManager class. It is created when the user clicks 
// on a point in the MapLink Widget.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool Application::createPolyline( double lat, double lon )
{
  TSLStandardDataLayer* dataLayer(NULL);
  TSLEntitySet* es(NULL);
  const TSLCoordinateSystem* cs(NULL);

  if (!getDataAndMapLayers(dataLayer, es, cs))
  {
    return false;
  }

  TSLTMC x, y, x1, y1;
  // we try and make the distance at least one degree
  if ((!cs->latLongToTMC( lat, lon, &x, &y ) || !cs->latLongToTMC( lat+1, lon+1, &x1, &y1 )) )
  {
    QMessageBox::information( NULL, "Unable to work out position of geometry correctly",
        "One of the calculated positions was invalid", QMessageBox::Ok );
    return false;
  }

  TSLTMC dist = fabs((double)(x1 - x)) * 2;

  // Create a coordinate list forming a triangle around the position
  // Use the Drawing Surface to calculate the coordinates
  TSLCoordSet* coords = new TSLCoordSet();
  if( !coords )
  {
    return false;
  }

  // Make a triangle, 1km either side of the specified position
  coords->add( x - dist, y + dist);
  coords->add( x, y - dist );
  coords->add( x + dist, y + dist );

  // Hand ownership of the coordset to the polygon
  TSLPolyline* poly = es->createPolyline( POLYLINE_FC, coords, true );
  if( !poly )
  {
    return false;
  }
  poly->name( "Polyline" );

  // Tell the layer that its contents have changed
  dataLayer->notifyChanged( true );

  return true;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Create Polygon
//
// Creates a polygon based on feature rendering from the 
// LayerManager class. It is created when the user clicks 
// on a point in the MapLink Widget.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool Application::createPolygon( double lat, double lon )
{
  TSLStandardDataLayer* dataLayer(NULL);
  TSLEntitySet* es(NULL);
  const TSLCoordinateSystem* cs(NULL);

  if (!getDataAndMapLayers(dataLayer, es, cs))
  {
    return false;
  }

  // Create a coordinate list forming a triangle around the position
  // Use the Drawing Surface to calculate the coordinates
  // We will make our triangles 25 pixels either side of the position
  // Note that the pixels are at the current zoom factor - the polygon
  // is always completely scalable

  TSLTMC x, y, x1, y1;
  // we try and make the distance at least one degree
  if ((!cs->latLongToTMC( lat, lon, &x, &y ) || !cs->latLongToTMC( lat+1, lon+1, &x1, &y1 )) )
  {
    QMessageBox::information( NULL, "Unable to work out position of geometry correctly",
        "One of the calculated positions was invalid", QMessageBox::Ok );
    return false;
  }

  TSLTMC dist = fabs((double)(x1 - x)) * 2;

  TSLCoordSet* coords = new TSLCoordSet();
  if( !coords )
  {
    return false;
  }
  coords->add( x - dist, y - dist );
  coords->add( x + dist, y - dist );
  coords->add( x, y + dist );

  // Hand ownership of the coordset to the polygon
  TSLPolygon * poly = es->createPolygon( POLYGON_FC, coords, true );

  if( !poly )
  {
    return false;
  }
  poly->name( "Polygon" );

  // Tell the layer that its contents have changed  
  dataLayer->notifyChanged( true );

  return true;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Create Text
//
// Creates text based on feature rendering from the 
// LayerManager class. It is created when the user clicks 
// on a point in the MapLink Widget.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool Application::createText( double lat, double lon )
{
  TSLStandardDataLayer* dataLayer(NULL);
  TSLEntitySet* es(NULL);
  const TSLCoordinateSystem* cs(NULL);

  if (!getDataAndMapLayers(dataLayer, es, cs))
  {
    return false;
  }

  TSLTMC x, y;

  // Convert the lat/lon to TMC using the coordinate providing layers original
  // Coordinate System (not the runtime one).
  if (!cs->latLongToTMC( lat, lon, &x, &y ))
  {
    QMessageBox::information( NULL, "Unable to work out position of geometry correctly",
        "One of the calculated positions was invalid", QMessageBox::Ok );
    return false;
  }
  
  TSLText* txt = es->createText( TEXT_FC, x, y, "Hello World" );

  if( !txt )
  {
    return false; // Return failure if text could not be created
  }
  txt->name( "Text" );

  // Tell the layer that its contents have changed
  dataLayer->notifyChanged( true );

  return true;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Create Vector Symbol
//
// Creates a vector symbol based on feature rendering from the 
// LayerManager class. It is created when the user clicks 
// on a point in the MapLink Widget.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool Application::createVectorSymbol( double lat, double lon )
{
  TSLStandardDataLayer* dataLayer(NULL);
  TSLEntitySet* es(NULL);
  const TSLCoordinateSystem* cs(NULL);

  if (!getDataAndMapLayers(dataLayer, es, cs))
  {
    return false;
  }

  TSLTMC x, y;

  // Convert the lat/lon to TMC using the coordinate providing layers original
  // Coordinate System (not the runtime one).
  if (!cs->latLongToTMC( lat, lon, &x, &y ))
  {
    QMessageBox::information( NULL, "Unable to work out position of geometry correctly",
        "One of the calculated positions was invalid", QMessageBox::Ok );
    return false;
  }

  TSLSymbol* symbol = es->createSymbol( VSYMBOL_FC, x, y );

  if( !symbol )
  {
    return false;
  }
  symbol->name( "Vector Symbol" );

  // Tell the layer that its contents have changed
  dataLayer->notifyChanged( true );

  return true;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Create Raster Symbol
//
// Creates a raster symbol based on feature rendering from the 
// LayerManager class. It is created when the user clicks 
// on a point in the MapLink Widget.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool Application::createRasterSymbol( double lat, double lon )
{
  TSLStandardDataLayer* dataLayer(NULL);
  TSLEntitySet* es(NULL);
  const TSLCoordinateSystem* cs(NULL);

  if (!getDataAndMapLayers(dataLayer, es, cs))
  {
    return false;
  }

  TSLTMC x, y;

  // Convert the lat/lon to TMC using the coordinate providing layers original
  // Coordinate System (not the runtime one).
  if (!cs->latLongToTMC( lat, lon, &x, &y ))
  {
    QMessageBox::information( NULL, "Unable to work out position of geometry correctly",
        "One of the calculated positions was invalid", QMessageBox::Ok );
    return false;
  }

  // 123 is the numeric feature code we assigned on the Data Layer
  TSLSymbol* symbol = es->createSymbol( 123, x, y );

  if( !symbol )
  {
    return false;
  }
  symbol->name( "Raster Symbol" );

  // No need to configure any rendering, MapLink will look it up from the Data Layer at display time.

  // Tell the layer that its contents have changed
  dataLayer->notifyChanged( true );

  return true;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Create Arc
//
// Creates an arc polyline based on feature rendering from the 
// LayerManager class. It is created when the user clicks 
// on a point in the MapLink Widget.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool Application::createArc( double lat, double lon )
{
  TSLStandardDataLayer* dataLayer(NULL);
  TSLEntitySet* es(NULL);
  const TSLCoordinateSystem* cs(NULL);

  if (!getDataAndMapLayers(dataLayer, es, cs))
  {
    return false;
  }
  
  TSLTMC x, y, x1, y1;
  // we try and make the distance at least one degree
  if ((!cs->latLongToTMC( lat, lon, &x, &y ) || !cs->latLongToTMC( lat+1, lon+1, &x1, &y1 )) )
  {
    QMessageBox::information( NULL, "Unable to work out position of geometry correctly",
        "One of the calculated positions was invalid", QMessageBox::Ok );
    return false;
  }

  TSLTMC rad = fabs((double)(x1 - x)) ;

  TSLArc* poly = es->createArc( ARC_FC, 0.785398, 3.75246, x - rad, y + rad, rad );
  if( !poly )
  {
    return false;
  }
  poly->name( "Arc" );

  // Tell the layer that its contents have changed
  dataLayer->notifyChanged( true );

  return true;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Create Ellipse
//
// Creates an ellipse polyline based on feature rendering from the 
// LayerManager class. It is created when the user clicks 
// on a point in the MapLink Widget.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool Application::createEllipse( double lat, double lon )
{
  TSLStandardDataLayer* dataLayer(NULL);
  TSLEntitySet* es(NULL);
  const TSLCoordinateSystem* cs(NULL);

  if (!getDataAndMapLayers(dataLayer, es, cs))
  {
    return false;
  }
  
  TSLTMC x, y, x1, y1;
  // we try and make the distance at least one degree
  if ((!cs->latLongToTMC( lat, lon, &x, &y ) || !cs->latLongToTMC( lat+1, lon+1, &x1, &y1 )) )
  {
    QMessageBox::information( NULL, "Unable to work out position of geometry correctly",
        "One of the calculated positions was invalid", QMessageBox::Ok );
    return false;
  }

  TSLTMC rad = fabs((double)(x1 - x));
  TSLEllipse* poly = es->createEllipse( ELLIPSE_FC, x, y, rad, rad );
  if( !poly )
  {
    return false;
  }
  poly->name( "Ellipse" );

  // Tell the layer that its contents have changed
  dataLayer->notifyChanged( true );

  return true;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Create Geodetic Polyline
//
// Creates a geodetic polyline based on feature rendering from the 
// LayerManager class. It is created when the user clicks 
// on a point in the MapLink Widget and moves relative to the projection.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool Application::createGeoPolyline( double lat, double lon )
{
  TSLStandardDataLayer* dataLayer(NULL);
  TSLEntitySet* es(NULL);
  const TSLCoordinateSystem* cs(NULL);

  if (!getDataAndMapLayers(dataLayer, es, cs))
  {
    return false;
  }

  // Create a coordinate list forming a triangle around the position
  // Use the Drawing Surface to calculate the coordinates

  TSLTMC x, y, x1, y1;
  // we try and make the distance at least one degree
  if ((!cs->latLongToTMC( lat, lon, &x, &y ) || !cs->latLongToTMC( lat+1, lon+1, &x1, &y1 )) )
  {
    QMessageBox::information( NULL, "Unable to work out position of geometry correctly",
        "One of the calculated positions was invalid", QMessageBox::Ok );
    return false;
  }

  TSLTMC dist = fabs((double)(x1 - x)) * 2;

  TSLCoordSet* coords = new TSLCoordSet();
  if( !coords )
  {
    return false;
  }
  // Make a triangle, 1km either side of the specified position
  coords->add( x - dist, y + dist );
  coords->add( x, y - dist );
  coords->add( x + dist, y + dist );

  //Hand ownership of the coordset to the polygon
  TSLGeodeticPolyline* poly = es->createGeodeticPolyline( POLYLINE_FC, coords, true );
  if( !poly )
  {
    return false;
  }
  poly->name( "Geodetic Polyline" );

  // Tell the layer that its contents have changed
  dataLayer->notifyChanged( true );

  return true;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Create Geodetic Polygon
//
// Creates a geodetic polygon based on feature rendering from the 
// LayerManager class. It is created when the user clicks 
// on a point in the MapLink Widget and moves relative to the projection.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool Application::createGeoPolygon( double lat, double lon )
{
  TSLStandardDataLayer* dataLayer(NULL);
  TSLEntitySet* es(NULL);
  const TSLCoordinateSystem* cs(NULL);

  if (!getDataAndMapLayers(dataLayer, es, cs))
  {
    return false;
  }

  // Create a coordinate list forming a triangle around the position.
  // Use the Drawing Surface to calculate the coordinates.
  // We will make our triangles about 300km across.

  double lats[3], lons[3];
  TSLCoordinateConverter::greatCircleDistancePoint( lat, lon, 0.0, 200000.0, lats[0], lons[0] );
  TSLCoordinateConverter::greatCircleDistancePoint( lat, lon, 120.0, 100000.0, lats[1], lons[1] );
  TSLCoordinateConverter::greatCircleDistancePoint( lat, lon, -120.0, 100000.0, lats[2], lons[2] );

  for ( int i = 0; i < sizeof( lats ) / sizeof( *lats ); i++ )
  {
    // clamp lat/lon to +-90/+-180 range
    Util::normaliseLatLon( lats[i], lons[i] );
  }

  TSLTMC x[3], y[3];
  if ( (!cs->latLongToTMC( lats[0], lons[0], &x[0], &y[0] )) ||
       (!cs->latLongToTMC( lats[1], lons[1], &x[1], &y[1] )) || 
       (!cs->latLongToTMC( lats[2], lons[2], &x[2], &y[2] )) )
  {
    QMessageBox::information( NULL, "Unable to work out position of geometry correctly",
      "One of the calculated positions was invalid", QMessageBox::Ok );
    return false;
  }
  
  TSLCoordSet* coords = new TSLCoordSet();
  if( !coords )
  {
    return false;
  }
  coords->add( x[0], y[0] );
  coords->add( x[1], y[1] );
  coords->add( x[2], y[2] );

  // Hand ownership of the coordset to the polygon
  TSLGeodeticPolygon * poly = es->createGeodeticPolygon( POLYGON_FC, coords, true );

  if( !poly )
  {
    return false;
  }
  poly->name( "Geodetic Polygon" );

  // Tell the layer that its contents have changed
  dataLayer->notifyChanged( true );

  return true;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Create Geodetic Text
//
// Creates geodetic text based on feature rendering from the 
// LayerManager class. It is created when the user clicks 
// on a point in the MapLink Widget and moves relative to the projection.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool Application::createGeoText( double lat, double lon )
{
  TSLStandardDataLayer* dataLayer(NULL);
  TSLEntitySet* es(NULL);
  const TSLCoordinateSystem* cs(NULL);

  if (!getDataAndMapLayers(dataLayer, es, cs))
  {
    return false;
  }
  
  TSLTMC x, y;
  if (!cs->latLongToTMC( lat, lon, &x, &y ))
  {
    QMessageBox::information( NULL, "Unable to work out position of geometry correctly",
        "One of the calculated positions was invalid", QMessageBox::Ok );
    return false;
  }

  TSLGeodeticText* txt = es->createGeodeticText( TEXT_FC, x, y, "Hello World" );

  if( !txt )
  {
    return false; // Return failure if text could not be created
  }
  txt->name( "Geodetic Text" );

  // Tell the layer that its contents have changed
  dataLayer->notifyChanged( true );

  return true;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Create Geodetic Vector Symbol
//
// Creates a geodetic vector symbol based on feature rendering from the 
// LayerManager class. It is created when the user clicks 
// on a point in the MapLink Widget and moves relative to the projection.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool Application::createGeoVectorSymbol( double lat, double lon )
{
  TSLStandardDataLayer* dataLayer(NULL);
  TSLEntitySet* es(NULL);
  const TSLCoordinateSystem* cs(NULL);

  if (!getDataAndMapLayers(dataLayer, es, cs))
  {
    return false;
  }
  
  TSLTMC x, y;
  if (!cs->latLongToTMC( lat, lon, &x, &y ))
  {
    QMessageBox::information( NULL, "Unable to work out position of geometry correctly",
        "One of the calculated positions was invalid", QMessageBox::Ok );
    return false;
  }

  TSLGeodeticSymbol* symbol = es->createGeodeticSymbol( VSYMBOL_FC, x, y );

  if( !symbol )
  {
    return false;
  }
  symbol->name( "Geodetic Vector Symbol" );
  // Create a green star, 1,000,000m high.
  // This looks sensible on the Dorset map!
  TSLStyleID green = TSLDrawingSurface::getIDOfNearestColour( 0, 255, 0 );
  symbol->setRendering( TSLRenderingAttributeSymbolStyle, 14 );
  symbol->setRendering( TSLRenderingAttributeSymbolColour, green );
  symbol->setRendering( TSLRenderingAttributeSymbolSizeFactor, 30.0 );
  symbol->setRendering( TSLRenderingAttributeSymbolSizeFactorUnits, TSLDimensionUnitsPoints );

  // Tell the layer that its contents have changed
  dataLayer->notifyChanged( true );

  return true;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Create Geodetic Raster Symbol
//
// Creates a geodetic raster symbol based on feature rendering from the 
// LayerManager class. It is created when the user clicks 
// on a point in the MapLink Widget and moves relative to the projection.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool Application::createGeoRasterSymbol( double lat, double lon )
{
  TSLStandardDataLayer* dataLayer(NULL);
  TSLEntitySet* es(NULL);
  const TSLCoordinateSystem* cs(NULL);

  if (!getDataAndMapLayers(dataLayer, es, cs))
  {
    return false;
  }
  
  TSLTMC x, y;
  if (!cs->latLongToTMC( lat, lon, &x, &y ))
  {
    QMessageBox::information( NULL, "Unable to work out position of geometry correctly",
        "One of the calculated positions was invalid", QMessageBox::Ok );
    return false;
  }

  // 123 is the numeric feature code we assigned on the Data Layer
  TSLGeodeticSymbol* symbol = es->createGeodeticSymbol( 123, x, y );

  if( !symbol )
  {
    return false;
  }
  symbol->name( "Geodetic Raster Symbol" );

  // No need to configure any rendering, MapLink will look it up from the Data Layer at display time.

  // Tell the layer that its contents have changed
  dataLayer->notifyChanged( true );

  return true;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Create Geodetic Arc
//
// Creates a geodetic arc based on feature rendering from the 
// LayerManager class. It is created when the user clicks 
// on a point in the MapLink Widget and moves relative to the projection.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool Application::createGeoArc( double lat, double lon )
{
  TSLStandardDataLayer* dataLayer(NULL);
  TSLEntitySet* es(NULL);
  const TSLCoordinateSystem* cs(NULL);

  if (!getDataAndMapLayers(dataLayer, es, cs))
  {
    return false;
  }
  
  TSLTMC x, y, x1, y1;
  // we try and make the distance at least one degree
  if ((!cs->latLongToTMC( lat, lon, &x, &y ) || !cs->latLongToTMC( lat+1, lon+1, &x1, &y1 )) )
  {
    QMessageBox::information( NULL, "Unable to work out position of geometry correctly",
        "One of the calculated positions was invalid", QMessageBox::Ok );
    return false;
  }

  TSLTMC rad = fabs((double)(x1 - x)) / 50.;

  //Hand ownership of the coordset to the polygon
  TSLGeodeticArc* poly = es->createGeodeticArc( ARC_FC, 0.785398, 3.75246, x - rad, y + rad, rad );
  if( !poly )
  {
    return false;
  }

  poly->name( "Geodetic Arc" );

  // Tell the layer that its contents have changed
  dataLayer->notifyChanged( true );

  return true;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Create Geodetic Ellipse
//
// Creates a geodetic ellipse based on feature rendering from the 
// LayerManager class. It is created when the user clicks 
// on a point in the MapLink Widget and moves relative to the projection.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool Application::createGeoEllipse( double lat, double lon )
{
  TSLStandardDataLayer* dataLayer(NULL);
  TSLEntitySet* es(NULL);
  const TSLCoordinateSystem* cs(NULL);

  if (!getDataAndMapLayers(dataLayer, es, cs))
  {
    return false;
  }
  
  TSLTMC x, y, x1, y1;
  // we try and make the distance at least one degree
  if ((!cs->latLongToTMC( lat, lon, &x, &y ) || !cs->latLongToTMC( lat+1, lon+1, &x1, &y1 )) )
  {
    QMessageBox::information( NULL, "Unable to work out position of geometry correctly",
        "One of the calculated positions was invalid", QMessageBox::Ok );
    return false;
  }

  TSLTMC rad = fabs((double)(x1 - x)) / 50.0;
  
  //Hand ownership of the coordset to the polygon
  TSLGeodeticEllipse* poly = es->createGeodeticEllipse( ELLIPSE_FC, x, y, rad, rad);
  if( !poly )
  {
    return false;
  }
  poly->name( "Geodetic Ellipse" );

  // Tell the layer that its contents have changed
  dataLayer->notifyChanged( true );

  return true;
}

void Application::saveToTMF( const char* filename )
{
  TSLStandardDataLayer* dataLayer = reinterpret_cast<TSLStandardDataLayer*>(m_layerManager->getLayer( "EntityLayer" ));
  if (!dataLayer)
  {
    return;
  }
  TSLEntitySet* es = dataLayer->entitySet();
  if (!es)
  {
    return;
  }

  if ( !es->saveData( filename ) )
  {
    QMessageBox::information( NULL, "Unable to save file", QString( "Error occurred saving TMF file \"%1\"" ).arg( filename ), QMessageBox::Ok );
  }
}

void Application::loadFromTMF( const char* filename )
{
  TSLStandardDataLayer* dataLayer = reinterpret_cast<TSLStandardDataLayer*>(m_layerManager->getLayer( "EntityLayer" ));
  if (!dataLayer)
  {
    return;
  }
  TSLEntitySet* es = dataLayer->entitySet();
  if (!es)
  {
    return;
  }

  if ( !es->loadData( filename ) )
  {
    QMessageBox::information( NULL, "Unable to load file", QString( "Error occurred loading TMF file \"%1\"" ).arg( filename ), QMessageBox::Ok );
  }
  dataLayer->notifyChanged( true );
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Drawing Surface
//
// Returns a reference to the drawing surface so that another object
// may use it.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#ifdef _MSC_VER
TSLWGLSurface* Application::getDrawingSurface()
{
  return m_drawingSurface;
}
#else
TSLGLXSurface* Application::getDrawingSurface()
{
  return m_drawingSurface;
}
#endif

bool Application::getDataAndMapLayers(TSLStandardDataLayer*& dataLayer, TSLEntitySet*& entitySet, const TSLCoordinateSystem*& coordSys)
{
  if (!m_drawingSurface || !m_layerManager)
  {
    return false;
  }

  dataLayer = reinterpret_cast<TSLStandardDataLayer*>(m_layerManager->getLayer("EntityLayer"));
  if (!dataLayer)
  {
    return false;
  }
  
  TSLTMC x, y;
  
  // Convert the lat/lon to TMC using the coordinate providing layers original
  // Coordinate System (not the runtime one).
  TSLStaticMapDataLayer* map = reinterpret_cast<TSLStaticMapDataLayer*>(m_drawingSurface->getCoordinateProvidingLayer());
  if (!map)
  {
    return false;
  }
  coordSys = map->queryMapCoordinateSystem();
    
  entitySet = dataLayer->entitySet();
  if (!entitySet)
  {
    return false;
  }

  return true;
}
