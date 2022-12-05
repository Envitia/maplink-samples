/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <QtGui>
#include <QMessageBox>

#include "application.h"
#include "maplinkwidget.h"

#include "tslkmldatalayer.h"

#include "attributetreewidget.h"

// Interaction mode IDs - these can be any numbers
#define ID_TOOLS_ZOOM                   1
#define ID_TOOLS_PAN                    2
#define ID_TOOLS_GRAB                   3

// The name of our map layer. This is used when adding the data layer
// to the drawing surface and used to reference the data layer from the
// drawing surface
const char * Application::m_mapLayerName = "map";

// Controls how far the drawing surface rotates in one key press - this value is in radians
static const double rotationIncrement = M_PI / 360.0;

Application::Application( QWidget *parent )
  : m_mapDataLayer(NULL)
  , m_drawingSurface(NULL)
#ifdef X11_BUILD
  , m_display(NULL)
  , m_screen(NULL)
#else
  , m_window(NULL)
#endif
  , m_modeManager(NULL)
  , m_widgetWidth(0)
  , m_widgetHeight(0)
  , m_surfaceRotation(0.0)
  , m_parentWidget( parent )
  , m_kmlLayer(NULL)
  , m_attributeTree(NULL)
{
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Clear the error stack so that we can get the errors that occurred here.
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  TSLErrorStack::clear();

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Initialise the drawing surface data files.
  //
  // This call uses the TSLUtilityFunctions::getMapLinkHome method to determine
  // where MapLink is currently installed.  It then proceeds to load the
  // following files : tsllinestyles.dat, tslfillstyles.dat, tslfonts.dat, tslsymbols.dat
  // and tslcolours.dat
  //
  // When deploying your application, pass in a full path to the directory containing
  // these files.
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  TSLDrawingSurface::loadStandardConfig();

  // Check for any errors that have occurred, and display them

  const char *errorMessage = TSLErrorStack::errorString("Error") ;
  if( errorMessage )
  {
    // If we have any errors during initialisation, display the message
    QMessageBox::critical( m_parentWidget, "Failed to load standard configuration data", errorMessage );
    m_parentWidget->close();
  }

  // When deploying pass the full path to tsltransforms.dat
  TSLCoordinateSystem::loadCoordinateSystems();
  errorMessage = TSLErrorStack::errorString("Error") ;
  if( errorMessage )
  {
    // If we have any errors during initialisation, display the message
    QMessageBox::critical( m_parentWidget, "Failed to load transforms configuration data", errorMessage );
    m_parentWidget->close();
  }

}


Application::~Application()
{
  // Clean up by destroying the objects we created
  if (m_kmlLayer)
  {
    m_kmlLayer->destroy();
  }

  if( m_mapDataLayer )
  {
    m_mapDataLayer->destroy();
    m_mapDataLayer = NULL;
  }

  if( m_modeManager )
  {
    delete m_modeManager;
    m_modeManager = NULL;
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


bool Application::loadMap(const char *mapFilename)
{
  // Clear the error stack, load the map then check for errors.
  TSLErrorStack::clear() ;

  if( mapFilename )
  {
    // load the map
    if( !m_mapDataLayer->loadData( mapFilename ) )
    {
      QMessageBox::critical( m_parentWidget, "Failed to load map", QString::fromUtf8( mapFilename ) );
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

    m_drawingSurface->setDataLayerProps( m_mapLayerName, TSLPropertyBuffered, true ) ;
  }

  return true;
}

bool Application::loadKML(const char* kmlFilename, AttributeTreeWidget* attributeTree)
{
  if (m_kmlLayer)
  {
    m_kmlLayer->destroy();
  }

  // Attribute tree passed in from main window
  if( attributeTree )
  {
    m_attributeTree = attributeTree;
  }

  if( m_attributeTree )
  {
    m_attributeTree->clear();
    m_attributeTree->m_initialised = false;
  }

  if( m_mapDataLayer && m_mapDataLayer->queryCoordinateSystem() )
  {
    m_kmlLayer = new TSLKMLDataLayer();
    if( !m_kmlLayer )
    {
      return false;
    }
    // If the coordinate system is set manually, data will be loaded 
    // immediately.
    //
    // Otherwise the data will be loaded on the first redraw.
    //
    // If you set a coordinate system then the KML data will not be reprojected if
    // the map coordinate system changes.
    //
    //m_kmlLayer->setCoordinateSystem( m_mapDataLayer->queryCoordinateSystem() );

    //
    // Set the location for the KML Cache if required.
    //
    //m_kmlLayer->setCacheDirectory( "CustomCacheFolder" );
    //m_kmlLayer->clearCache();

    if( !m_kmlLayer->loadData( kmlFilename ) )
    {
      QMessageBox::critical( m_parentWidget, "Failed to load kml file.", QString::fromUtf8( kmlFilename ) );
      return false;
    }
 
    m_drawingSurface->removeDataLayer("kml");
    m_drawingSurface->addDataLayer( m_kmlLayer, "kml");

    // Enable if there is a drawing issue with lots of transparent lines - this is a performance hit.
    // m_drawingSurface->setLayerTransparencyHint( "kml", TSLOpenGLTransparencyHintFlushOpaque );
    m_drawingSurface->setDataLayerProps( "kml", TSLPropertyVisible, true ) ;
    m_drawingSurface->setDataLayerProps( "kml", TSLPropertyDetect, true ) ;
    m_drawingSurface->setDataLayerProps( "kml", TSLPropertySelect, true ) ;
    m_drawingSurface->setDataLayerProps( "kml", TSLPropertyBuffered, true ) ;
    m_drawingSurface->bringToFront( "kml" );
    
    
    if( m_attributeTree && m_kmlLayer && !m_attributeTree->m_initialised)
    {
      // This can only be called after m_kmlLayer->loadData( filename ), 
      // if we previously set a coordinate system on the layer.
      //
      // If the layer has no coordinate system, data isn't loaded
      // until the first draw, so this will return NULL

      TSLDataLayer* layer = m_kmlLayer->getLayer(0);
      if( layer->layerType() == TSLDataLayerTypeStandardDataLayer )
      {
        TSLStandardDataLayer* standardLayer = (TSLStandardDataLayer*)layer;
        TSLEntitySet* es = standardLayer->entitySet();
        if( es->size() )
        {
          m_attributeTree->AddEntitySet( es );
          m_attributeTree->m_initialised = true;
          m_kmlLayer->notifyChanged();
        }
      }
    }
  }
  else
  {
    return false;
  }

  return true;
}

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

#ifdef X11_BUILD
  // Get the active OpenGL context to attach the drawing surface to
  GLXContext context = glXGetCurrentContext();
  GLXDrawable drawable = glXGetCurrentDrawable();

  // Create the Accelerated Surface object
  m_drawingSurface = new TSLGLXSurface( m_display, m_screen, drawable, context, creationOptions );
#else
  HGLRC context = wglGetCurrentContext();
  m_drawingSurface = new TSLWGLSurface( (HWND)m_window, false, context, creationOptions );
#endif

  if( !m_drawingSurface->context() )
  {
    // The drawing surface failed to attach to the context - show the error from the error stack.
    // This means the drawing surface cannot be used, so exit the sample
    const char *msg = TSLErrorStack::errorString() ;
    if ( msg )
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

  // Enable dynamic arc map support.
  m_drawingSurface->setOption( TSLOptionDynamicArcSupportEnabled, true );

  // We cannot call wndResize on the drawing surface yet as we don't know the size of the widget

  // Add a map data layer to the drawing surface
  m_mapDataLayer = new TSLMapDataLayer();
  // Set a cache size of 256Mb on the map layer to avoid reloading tiles from disk too frequently
  m_mapDataLayer->cacheSize( 256 * 1024 );
  m_drawingSurface->addDataLayer( m_mapDataLayer, m_mapLayerName );

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

void Application::redraw()
{
  if( m_drawingSurface )
  {
    // Draw the map to the widget
    m_drawingSurface->drawDU( 0, 0, m_widgetWidth, m_widgetHeight, true );

    // Don't forget to draw any echo rectangle that may be active.
    if ( m_modeManager )
    {
      m_modeManager->onDraw( 0, 0, m_widgetWidth, m_widgetHeight );
    }

    // We have to wait until we have loaded data into the layer
    // which may happen on first draw if a coordinate system was
    // not set on the KML data-layer.
    if( m_attributeTree && m_kmlLayer && !m_attributeTree->m_initialised)
    {
      // This can only be called after m_kmlLayer->loadData( filename ), 
      // if we previously set a coordinate system on the layer.
      //
      // If the layer has no coordinate system, data isn't loaded
      // until the first draw, so this will return NULL

      TSLDataLayer* layer = m_kmlLayer->getLayer(0); // First layer is the vector data-layer.
      if( layer->layerType() == TSLDataLayerTypeStandardDataLayer ) // Always check the layer type.
      {
        TSLStandardDataLayer* standardLayer = (TSLStandardDataLayer*)layer;
        TSLEntitySet* es = standardLayer->entitySet();
        if( es->size() )
        {
          m_attributeTree->AddEntitySet( es );
          m_attributeTree->m_initialised = true;
        }
      }
      m_kmlLayer->notifyChanged();
    }
  }
}

void Application::resize(int width, int height)
{
  if( m_drawingSurface )
  {
    // Inform the drawing surface of the new window size,
    // attempting to keep the top left corner the same.
    // Do not ask for an automatic redraw since we will get a call to redraw() to do so
    m_drawingSurface->wndResize( 0, 0, width, height, false, TSLResizeActionMaintainTopLeft ) ;
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
  //
  // Picks the closest entity and displays its parent if it is a base geometry element.
  //
  // In the case of this sample, the right mouse button is unused
  // - mapped to perform a pick operation on the kml layer and 
  //   display the results in the attribute widget.
  if( m_attributeTree )
  {
    TSLPickResultSet* pickResults = m_drawingSurface->pick( "kml", mx, my, 0, 1 );

    m_attributeTree->clear();

    for( int i(0); i < pickResults->numResults(); ++i )
    {
      const TSLPickResult* pickResult = pickResults->getResult(i);
      if( pickResult->queryType() == TSLPickCustom )
      {
        TSLPickResultCustom* pickResultCustom = (TSLPickResultCustom*)pickResult;
        TSLKMLPickResult* kmlPickResult = (TSLKMLPickResult*)pickResultCustom->getClientCustomPickResult();

        const TSLEntity* entity = kmlPickResult->entity();

        // Entities created from kml geometry objects, such as lineString or Point
        // don't contain any attributes, as they are children of placemarks
        // This displays the parent entitySet, which should be a placemark containing
        // all of the relevant attribute information.
        if( TSLEntitySet::isEntitySet(entity) )
        {
          m_attributeTree->AddEntitySet( (TSLEntitySet*)entity );
        }
        else
        {
          if( entity->parent() )
          {
            m_attributeTree->AddEntitySet( entity->parent() );
          }
        }
      }
    }
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

#ifdef WINNT
void Application::drawingInfo(WId window)
{
  m_window = window;
}

#else
void Application::drawingInfo(Display *display, Screen *screen)
{
  m_display = display;
  m_screen = screen;
}
#endif
