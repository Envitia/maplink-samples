/****************************************************************************
                Copyright (c) 2013-2017 by Envitia Group PLC.
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <QtGui>
#include <QMessageBox>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>

#include "application.h"
#include "maplinkwidget.h"
#include "offscreenrender.h"


// Interaction mode IDs - these can be any numbers
#define ID_TOOLS_ZOOM                   1
#define ID_TOOLS_PAN                    2
#define ID_TOOLS_GRAB                   3

// The name of our map layer. This is used when adding the data layer
// to the drawing surface and used to reference the data layer from the
// drawing surface
const char * Application::m_mapLayerName = "map";
const char * Application::m_stdLayerName = "std";

// Controls how far the drawing surface rotates in one key press - this value is in radians
static const double rotationIncrement = M_PI / 360.0;



Application::Application( QWidget *parent )
  : m_mapDataLayer(NULL)
  , m_stdDataLayer(NULL)
  , m_drawingSurface(NULL)
  , m_offScreenHelper(NULL)
  , m_modeManager(NULL)
  , m_widgetWidth(0)
  , m_widgetHeight(0)
  , m_surfaceRotation(0.0)
  , m_parentWidget( parent )
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
  TSLDrawingSurface::loadStandardConfig();

  // Check for any errors that have occurred, and display them

  const char *errorMessage = TSLErrorStack::errorString("Error") ;
  if( errorMessage )
  {
    // If we have any errors during initialisation, display the message
    QMessageBox::critical( m_parentWidget, "Failed to load standard configuration data", errorMessage );
    m_parentWidget->close();
  }
}


Application::~Application()
{
  // Clean up by destroying the objects we created
  if( m_mapDataLayer )
  {
    m_mapDataLayer->destroy();
    m_mapDataLayer = NULL;
  }

  if( m_stdDataLayer )
  {
    m_stdDataLayer->destroy();
    m_stdDataLayer = NULL;
  }

  if( m_modeManager )
  {
    delete m_modeManager;
    m_modeManager = NULL;
  }

  // This class does not own these objects.
  m_offScreenHelper = NULL;
  m_drawingSurface = NULL;

  // Beyond this point MapLink will no longer be used - clear up all static data.
  // Once this is done no MapLink functions or classes can be used.
  TSLDrawingSurface::cleanup( true );
}


bool Application::loadFile(const char *fileFilename)
{
  if (!m_stdDataLayer || !m_mapDataLayer)
    return false;

  // Clear the error stack, load the map then check for errors.
  TSLErrorStack::clear() ;

  if( fileFilename )
  {
    if (strstr(fileFilename, ".tmf") != NULL)
    {
      // we are loading a TMF file rather than a map file
      if( !m_stdDataLayer->loadData( fileFilename ) )
      {
        QMessageBox::critical( m_parentWidget, "Failed to load TMF file", QString::fromUtf8( fileFilename ) );
        return false;
      }
      return true;
    }

    // load the map
    if( !m_mapDataLayer->loadData( fileFilename ) )
    {
      QMessageBox::critical( m_parentWidget, "Failed to load map", QString::fromUtf8( fileFilename ) );
      return false;
    }

    TSLStyleID backgroundColour = m_mapDataLayer->getBackgroundColour();
    if( backgroundColour != -1 )
    {
      m_drawingSurface->setBackgroundColour( backgroundColour );
    }

    if( m_modeManager )
    {
      // Loading a map invalidates any stored views in mode manager  - this sample
      // doesn't create any
      m_modeManager->resetViews();
    }

    // Display any errors that have occurred
    const char *msg = TSLErrorStack::errorString( "Loading map\n" ) ;
    if( msg )
    {
      QMessageBox::critical( m_parentWidget, "Failed to load map", msg );
      TSLErrorStack::clear();
      return false;
    }
  }

  return true;
}

void Application::create( TSLDrawingSurfaceDrawCallback *redrawCallback, TSLOpenGLSurface *surface, OffScreenHelper *helper)
{
  if( m_drawingSurface )
  {
    return;  // We have already been called before!
  }

  // Save the objects for use later. We don't own these.
  m_drawingSurface = surface;
  m_offScreenHelper = helper;

  // Enable dynamic arc map support.
  m_drawingSurface->setOption( TSLOptionDynamicArcSupportEnabled, true );

  //
  // With the Qt integration we are using here neither option makes sense.
  //
  // We are rendering to an offscreen texture and as such only need to update the maplink drawn
  // items on change.
  //
  // Using TSLOptionTileBufferedLayers and TSLOptionProgressiveTileZoom complicates the management
  // of the re-drawing of the texture.
  //
  // Limitation: MapLink has to create the Context at present for TSLOptionTileBufferedLayers to work.
  //
  m_drawingSurface->setOption( TSLOptionTileBufferedLayers, false );
  m_drawingSurface->setOption( TSLOptionProgressiveTileZoom, false );

  // Tell the drawing surface which callback it should invoke when the new tiles for buffered
  // layers are ready for drawing.
  m_drawingSurface->setRedrawCallback( redrawCallback );

  // We cannot call wndResize on the drawing surface yet as we don't know the size of the widget

  // Add a map data layer to the drawing surface
  m_mapDataLayer = new TSLMapDataLayer();
  // Set a cache size of 256Mb on the map layer to avoid reloading tiles from disk too frequently
  m_mapDataLayer->cacheSize( 256 * 1024 );
  m_drawingSurface->addDataLayer( m_mapDataLayer, m_mapLayerName );

  // Make the map layer buffered so it will be rendered asynchronously to tiles
  m_drawingSurface->setDataLayerProps( m_mapLayerName, TSLPropertyBuffered, 1 );

  // As we have buffered layer tiling enabled, turn on text and symbol view expansion to reduce
  // incidents of text and symbols being clipped at the tile boundaries
  // See above as to why this is commented out.
  // m_drawingSurface->setDataLayerProps( m_mapLayerName, TSLPropertyLoadedSymbolsAndTextViewExpansion, 20 );

  // Vector Overlay 
  // We do not push this layer into the buffered tiles.
  m_stdDataLayer = new TSLStandardDataLayer();
  m_drawingSurface->addDataLayer( m_stdDataLayer, m_stdLayerName );

  // Now create and initialse the mode manager and modes
  m_modeManager = new TSLInteractionModeManagerGeneric( this, m_drawingSurface );

  // Add the three interaction mode types to the manager - the zoom mode is the default
  m_modeManager->addMode( new TSLInteractionModeZoom( ID_TOOLS_ZOOM ), true ) ;
  m_modeManager->addMode( new TSLInteractionModePan( ID_TOOLS_PAN ), false ) ;
  m_modeManager->addMode( new TSLInteractionModeGrab( ID_TOOLS_GRAB ), false ) ;

  m_modeManager->setCurrentMode( ID_TOOLS_ZOOM );

  // Display any errors that have occurred
  const char *errorMsg = TSLErrorStack::errorString();
  if( errorMsg )
  {
    QMessageBox::warning( m_parentWidget, "Cannot initialise view", errorMsg );
    TSLErrorStack::clear();
  }
}

GLuint Application::draw()
{
  if( m_offScreenHelper )
  {
    // Draw the map to the widget - the mode manager will be called to draw any feedback
    return m_offScreenHelper->draw(m_modeManager);
  }
  return 0; // invalid texture id
}

void Application::resize(int width, int height)
{
  if (m_widgetWidth == width && m_widgetHeight == height)
    return ;

  if( m_offScreenHelper )
  {
    // Inform the drawing surface of the new window size,
    // attempting to keep the top left corner the same.
    // Do not ask for an automatic redraw since we will get a call to redraw() to do so
    m_offScreenHelper->wndResize( width, height, TSLResizeActionMaintainTopLeft ) ;
  }
  if( m_modeManager )
  {
    m_modeManager->onSize( width, height ) ;
  }
  m_widgetWidth = width;
  m_widgetHeight = height;
}

////////////////////////////////////////////////////////////////////////
bool Application::mouseMoveEvent(unsigned int buttonPressed, bool shiftPressed, bool controlPressed, int mx, int my)
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if( m_modeManager )
  {
    // Request a redraw if the interaction hander requires it
    return m_modeManager->onMouseMove( (TSLButtonType)buttonPressed, mx, my, shiftPressed, controlPressed );
  }
  return false;
}

bool Application::onLButtonDown(bool shiftPressed, bool controlPressed, int X, int Y)
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if( m_modeManager )
  {
    // Request a redraw if the interaction hander requires it
    return m_modeManager->onLButtonDown( X, Y, shiftPressed, controlPressed );
  }
  return false;
}

bool Application::onMButtonDown(bool shiftPressed, bool controlPressed, int X, int Y)
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if( m_modeManager )
  {
    // Request a redraw if the interaction hander requires it
    return m_modeManager->onMButtonDown( X, Y, shiftPressed, controlPressed );
  }
  return false;
}

bool Application::onRButtonDown(bool shiftPressed, bool controlPressed, int mx, int my)
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if( m_modeManager )
  {
    // Request a redraw if the interaction hander requires it
    return m_modeManager->onRButtonDown( mx, my, shiftPressed, controlPressed );
  }
  return false;
}

bool Application::onLButtonUp(bool shiftPressed, bool controlPressed, int mx, int my)
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if( m_modeManager )
  {
    // Request a redraw if the interaction hander requires it
    return m_modeManager->onLButtonUp( mx, my, shiftPressed, controlPressed );
  }
  return false;
}

bool Application::onMButtonUp(bool shiftPressed, bool controlPressed, int mx, int my)
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if( m_modeManager )
  {
    // Request a redraw if the interaction hander requires it
    return m_modeManager->onMButtonUp( mx, my, shiftPressed, controlPressed );
  }
  return false;
}

bool Application::onRButtonUp(bool shiftPressed, bool controlPressed, int mx, int my)
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if( m_modeManager )
  {
    // Request a redraw if the interaction hander requires it
    return m_modeManager->onRButtonUp( mx, my, shiftPressed, controlPressed );
  }
  return false;
}

bool Application::onKeyPress(bool, bool, int keySym)
{
  // The left and right arrow keys allow the drawing surface to be rotated
  switch( keySym )
  {
  case Qt::Key_Left:
    m_surfaceRotation += rotationIncrement;
    m_drawingSurface->rotate( m_surfaceRotation );
    return true;

  case Qt::Key_Right:
    m_surfaceRotation -= rotationIncrement;
    m_drawingSurface->rotate( m_surfaceRotation );
    return true;

  default:
    break;
  }
  return false;
}

bool Application::onMouseWheel(bool, bool, short zDelta, int X, int Y)
{
  if( m_modeManager )
  {
    // Request a redraw if the interaction hander requires it
    return m_modeManager->onMouseWheel( zDelta, X, Y );
  }
  return false;
}

///////////////////////////////////////////////////////////////////////////
// Event handler functions - these are invoked from the widget
///////////////////////////////////////////////////////////////////////////

void Application::activatePanMode()
{
  // Activate the pan interaction mode
  if( m_modeManager )
  {
    m_modeManager->setCurrentMode( ID_TOOLS_PAN ) ;
  }
}

void Application::activateZoomMode()
{
  // Activate the zoom interaction mode
  if( m_modeManager )
  {
    m_modeManager->setCurrentMode( ID_TOOLS_ZOOM ) ;
  }
}

void Application::activateGrabMode()
{
  // Activate the grab interaction mode
  if( m_modeManager )
  {
    m_modeManager->setCurrentMode( ID_TOOLS_GRAB ) ;
  }
}

bool Application::zoomIn()
{
  if( m_modeManager )
  {
    // Request a redraw if the interaction hander requires it
    return m_modeManager->zoomIn( 30 );
  }
  return false;
}

bool Application::zoomOut()
{
  if( m_modeManager )
  {
    // Request a redraw if the interaction hander requires it
    return m_modeManager->zoomOut( 30 );
  }
  return false;
}

void Application::resetView()
{
  // Reset the view to the full extent of the map being loaded
  if( m_drawingSurface )
  {
    // Reset the drawing surface rotation as well
    m_surfaceRotation = 0.0;
    m_drawingSurface->rotate( m_surfaceRotation );

    m_drawingSurface->reset( false );
  }
}

void Application::enableBufferedLayerTiling( bool enable )
{
  return; // This will not work at the moment - see comments above

  if( m_drawingSurface )
  {
    m_drawingSurface->setOption( TSLOptionTileBufferedLayers, enable );

    // Turn off symbol and text view expansion of we are no longer using buffered layer tiling
    m_drawingSurface->setDataLayerProps( m_mapLayerName, TSLPropertyLoadedSymbolsAndTextViewExpansion, enable ? 20 : 0 );
  }
}

///////////////////////////////////////////////////////////////////////////
// TSLInteractionModeRequest callback functions
///////////////////////////////////////////////////////////////////////////
void Application::resetMode(TSLInteractionMode *, TSLButtonType, TSLDeviceUnits, TSLDeviceUnits)
{
  // Do nothing
}

void Application::viewChanged(TSLDrawingSurface*)
{
  // Do nothing
}

