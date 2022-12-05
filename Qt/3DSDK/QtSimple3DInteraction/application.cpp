/****************************************************************************
                Copyright (c) 2008-2017 by Envitia Group PLC.
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <QtGui>

#ifdef WIN32
# include <windows.h>
#endif

#include <GL/gl.h>
#ifndef WIN32
# include <GL/glx.h>
#endif

#include "application.h"

//***************************************************************************
// The Application class handles all interactions with the MapLink API. It
// knows nothing about the windowing system being used, that is handled by
// the MapLink widget class.
//***************************************************************************


// The names of our data layers. These are used when adding the data layers
// to the drawing surface and used to reference the data layers from the 
// drawing surface
const char* Application::m_mapLayerName = "map";
const char* Application::m_terrainLayerName = "terrain";

// The IDs assigned to the two interaction modes used in the sample. These are
// passed to the interaction mode manager in order to activate the corresponding
// mode. The numbers are application defined.
#define TRACKBALL_WORLD_ID 1
#define TRACKBALL_EYEPOINT_ID 2

Application::Application() : 
  m_mapDataLayer(NULL), 
  m_terrainDataLayer(NULL),
  m_drawingSurface(NULL),
#ifndef WIN32
  m_display(NULL),
  m_drawable(0),
  m_context(NULL),
  m_screen(NULL),
  m_colourmap(0),
  m_visual(NULL),
#else
  m_context(0),
  m_hwnd(0),
#endif
  m_modeManager(NULL),
  m_initialUpdate(true)
{
  // Clear the error stack so that we can get the errors that occurred here.
  TSLErrorStack::clear( ) ;

  // Initialise the drawing surface data files.
  // This call uses the TSLUtilityFunctions::getMapLinkHome method to determine
  // where MapLink is currently installed.  It then proceeds to load the
  // following files : tsllinestyles.dat, tslfillstyles.dat, tslfonts.dat, tslsymbols.dat
  // and tslcolours.dat
  // When deploying your application, pass in a full path to the directory containing
  // these files.
  TSLDrawingSurface::loadStandardConfig( );
  TSL3DDrawingSurface::loadStandardConfig();

  // In order to draw 3D draped polygons and polylines created using the
  // TSL3DInterpolationGrid interpolation type the contents of tsltransforms.dat (containing
  // the coordinate systems) must be loaded.
  // When deploying your application, pass in a full path to the directory containing
  // this file.
  TSLCoordinateSystem::loadCoordinateSystems();

  // Applications should ensure that no errors occurred during the above calls. In this sample
  // this is done inside the initialisation of the MapLink widget.
  // Details of any errors can be retrieved using the following code:
  /*const char * msg = TSLErrorStack::errorString() ;
  if ( msg )
  {
    // Display error message here
  }*/
}


Application::~Application()
{
  // Clean up all allocated objects
  if ( m_mapDataLayer )
  {
    m_mapDataLayer->destroy() ;
    m_mapDataLayer = NULL;
  }
  if ( m_terrainDataLayer )
  {
    m_terrainDataLayer->destroy();
    m_terrainDataLayer = NULL;
  }
  if( m_modeManager )
  {
    delete m_modeManager;
    m_modeManager = 0;
  }
  if (m_drawingSurface)
  {
    delete m_drawingSurface;
    m_drawingSurface = 0;
  }
}


bool Application::loadMap( const char *mapFilename)
{
  // Clear the error stack, load the map then check for errors.
  TSLErrorStack::clear() ;

  // Create a map data layer if we haven't already
  if (m_mapDataLayer == NULL)
  {
    m_mapDataLayer = new TSLMapDataLayer();

    // Add the new layer to the drawing surface so it will be used
    m_drawingSurface->addDataLayer( m_mapDataLayer, m_mapLayerName );
  }
  
  // Load the map
  if ( !m_mapDataLayer->loadData( mapFilename ) )
  {
    return false;
  }

  m_mapDataLayer->notifyChanged();

  // Check for errors
  if( TSLErrorStack::size() > 0 )
  {
    // There is one or more errors on the error stack as a result of the previous
    // operations, this means that the map likely did not load correctly.
    return false;
  }

  resetView();

  return true;
}

bool Application::loadTerrain( const char *terrainFilename )
{
  // Clear the error stack, load the map then check for errors.
  TSLErrorStack::clear() ;

  // Create the terrain layer on first use
  if (m_terrainDataLayer == NULL)
  {
    m_terrainDataLayer = new TSL3DTerrainDataLayer();

    // Add the new layer to the drawing surface so it will be used
    m_drawingSurface->addDataLayer( m_terrainDataLayer, m_terrainLayerName );
  }
  
  // Load the terrain database
  if( !m_terrainDataLayer->loadData( terrainFilename ) )
  {
    return false;
  }

  m_terrainDataLayer->notifyChanged();

  // Check for errors
  if( TSLErrorStack::size() > 0 )
  {
    // There is one or more errors on the error stack as a result of the previous
    // operations, this means that the map likely did not load correctly.
    return false;
  }

  return true;
}

void Application::onInitialUpdate( TSL3DRenderingCallback renderingCallback, void *arg ) 
{
  if (!m_initialUpdate)
  {
    // This function has already been called
    return;
  }

  if ( !m_drawingSurface ) 
  {
    // Create a 3D drawing surface. We are creating the drawing surface using an existing OpenGL context
    // supplied by the MapLink widget, instead of allowing the drawing surface to create it's own.
    //
    // Note that we tell the drawing surface that we will handle buffer swaps ourselves - in this sample
    // this is done for us by the QGLWidget so allowing the drawing surface to perform buffer swaps 
    // would cause it to occur twice per frame.

#ifndef WIN32
    // We always pass in the visual and screen retrieved from Qt along with the context here. This ensures
    // the information is available if it cannot be queried from the supplied GLX context.
    //
    // If we were not using an existing OpenGL context, we would need to ensure that we created the window
    // using an appropriate visual. The visual used determines whether double buffering and a depth buffer
    // are available so using an unsuitable visual can lead to flickers or incorrect rendering.
    //
    // A suitable visual can be found using the following code:
    //    int visualAttrs[] = { GLX_RGBA,
    //                          GLX_DOUBLEBUFFER, True,
    //                          GLX_RED_SIZE, 8,
    //                          GLX_GREEN_SIZE, 8,
    //                          GLX_BLUE_SIZE, 8,
    //                          GLX_DEPTH_SIZE, 16,
    //                          None };
    //
    //    XVisualInfo *visual = glXChooseVisual( display, DefaultScreen( display ), visualAttrs );

    m_drawingSurface = new TSL3DX11GLSurface( m_display, m_drawable, m_context, true, m_visual, m_screen );
#else
    m_drawingSurface = new TSL3DWinGLSurface( (HWND)m_hwnd, false, m_context, true );
#endif

    // Enable dynamic arc map support. If a map is loaded that is not in a dynamic arc coordinate system
    // this option has no effect.
    m_drawingSurface->setOption( TSLOptionDynamicArcSupportEnabled, true );

    // Inform the drawing surface of the size of the window
    m_drawingSurface->wndResize( 0, 0, m_width, m_height ) ;

    // Configure a set of default colours to use for rendering.
    static const int skyColourIndex( TSLComposeRGB( 0, 0, 128 ) );
    static int const wireframeColourIndex( TSLComposeRGB( 255, 0, 0 )  );
    static int const solidColourIndex( TSLComposeRGB( 51, 153, 255 ) ) ;

    // Set the background colour of the drawing surface to be the 'sky' colour
    m_drawingSurface->setBackgroundColour( skyColourIndex );

    // Determine the path to the default earth image.
    m_earthTexture = TSLUtilityFunctions::getMapLinkHome();
    m_earthTexture += "/config/earth.png";

    // Tell the surface the location of the earth image to use. Also tell it the colours to use when rendering in wireframe
    // mode, and the background colour of imagery to use if the earth image cannot be used.
    m_drawingSurface->setTerrainRendering( wireframeColourIndex, solidColourIndex, m_earthTexture.c_str() );

    // Reduce the swimming of map textures displayed in the distance.
    m_drawingSurface->setAnisotropicFilter(true);
    m_drawingSurface->setAnisotropicFilterLevel(4.0);

    // Now create and initialise the mode manager and modes. The IDs used when creating the interaction modes
    // are used to activate specific interaction modes through the manager.
#ifndef WIN32
    m_modeManager = new TSL3DInteractionModeManagerX11( this, m_drawingSurface, m_display, m_screen, m_drawable, m_colourmap ) ;
#else
    m_modeManager = new TSL3DInteractionModeManagerNT( this, m_drawingSurface, (HWND)m_hwnd ) ;
#endif
    m_modeManager->addMode( new TSL3DInteractionModeTrackballWorld( TRACKBALL_WORLD_ID, 30 ), false ) ;
    m_modeManager->addMode( new TSL3DInteractionModeTrackballEyepoint( TRACKBALL_EYEPOINT_ID, 30 ), true ) ;
  }

  // Set the initial camera position to have a reasonable view of the earth
  double defaultLatitude = 50.0;
  double defaultLongitude = 0.0;
  double defaultAltitude = 8000000.0;

  m_drawingSurface->camera()->reset();
  m_drawingSurface->camera()->moveTo( defaultLatitude, defaultLongitude, defaultAltitude, TSL3DCameraMoveActionNone ) ;
  m_drawingSurface->camera()->lookAt( defaultLatitude, 0.0, 0.0, false ) ;

  // Tell the drawing surface which function to invoke when new imagery is available. This function should
  // cause a redraw to occur.
  m_drawingSurface->setRenderingCallback( renderingCallback, arg ) ;

  m_modeManager->resetViews( ) ;
  m_modeManager->resetMode( TSLButtonNone, 0, 0 ) ;

  // Store the current view as the default. This view will be used when resetToDefaultView is called
  // on the mode manager.
  m_modeManager->setDefaultView( ) ;

  // Make the eyepoint mode active by default
  m_modeManager->setCurrentMode( TRACKBALL_EYEPOINT_ID );

  m_initialUpdate = false;
}

void Application::reset()
{
  if( m_modeManager )
  {
    // Tell the interaction mode manager to move the camera back to the default view
    // that we set inside onInitialUpdate().
    m_modeManager->resetToDefaultView();
  }
}

void Application::onDraw()
{
  if( m_drawingSurface )
  {
    // Always draw the entire window - the contents of the back buffer are undefined
    // following a buffer swap so the entire window needs to be redrawn.
    m_drawingSurface->drawDU( 0, 0, m_width, m_height, true );
  }
}

void Application::redraw()
{
  // We need to redraw the entire window - this is what's done normally by onDraw().
  onDraw();
}

void Application::onSize(int cx, int cy)
{
  // The window size is changing - record the new extent as it is needed
  // when we redraw.
  m_height = cy;
  m_width = cx;

  if ( m_drawingSurface )
  {
    // Inform the drawing surface of the new window size
    m_drawingSurface->wndResize( 0, 0, cx, cy ) ;
  }
}

bool Application::onMouseMove(TSLButtonType buttonPressed, bool shiftPressed, bool controlPressed, int mx, int my)
{
  // Ask the interaction mode manager to do the appropriate action for this event based on the currently
  // active interaction mode.
  if ( m_modeManager && m_modeManager->onMouseMove( buttonPressed, mx, my, shiftPressed, controlPressed ) )
  {
    // Request a redraw if the interaction hander requires it
    return true;
  }

  return false;
}

bool Application::onLButtonDown(bool shiftPressed, bool controlPressed, int X, int Y)
{
  // Ask the interaction mode manager to do the appropriate action for this event based on the currently
  // active interaction mode.
  if ( m_modeManager && m_modeManager->onLButtonDown( X, Y, shiftPressed, controlPressed ) )
  {
    // Request a redraw if the interaction hander requires it
    return true;
  }

  return false;
}

bool Application::onMButtonDown(bool shiftPressed, bool controlPressed, int X, int Y)
{
  // Ask the interaction mode manager to do the appropriate action for this event based on the currently
  // active interaction mode.
  if ( m_modeManager && m_modeManager->onMButtonDown( X, Y, shiftPressed, controlPressed ) )
  {
    // Request a redraw if the interaction hander requires it
    return true;
  }

  return false;

}

bool Application::onRButtonDown(bool shiftPressed, bool controlPressed, int mx, int my) 
{
  // Ask the interaction mode manager to do the appropriate action for this event based on the currently
  // active interaction mode.
  if ( m_modeManager && m_modeManager->onRButtonDown( mx, my, shiftPressed, controlPressed ) )
  {
    // Request a redraw if the interaction hander requires it
    return true;
  }

  return false;

}

bool Application::onLButtonUp(bool shiftPressed, bool controlPressed, int mx, int my)
{
  // Ask the interaction mode manager to do the appropriate action for this event based on the currently
  // active interaction mode.
  if ( m_modeManager && m_modeManager->onLButtonUp( mx, my, shiftPressed, controlPressed ) )
  {
    // Request a redraw if the interaction hander requires it
    return true;
  }

  return false;

}

bool Application::onMButtonUp(bool shiftPressed, bool controlPressed, int mx, int my)
{
  // Ask the interaction mode manager to do the appropriate action for this event based on the currently
  // active interaction mode.
  if ( m_modeManager && m_modeManager->onMButtonUp( mx, my, shiftPressed, controlPressed ) )
  {
    // Request a redraw if the interaction hander requires it
    return true;
  }

  return false;
}

bool Application::onRButtonUp(bool shiftPressed, bool controlPressed, int X, int Y)
{
  // Ask the interaction mode manager to do the appropriate action for this event based on the currently
  // active interaction mode.
  if ( m_modeManager && m_modeManager->onRButtonUp( X, Y, shiftPressed, controlPressed ) )
  {
    // Request a redraw if the interaction hander requires it
    return true;
  }

  return false;

}

bool Application::onMouseWheel(short zDelta, int X, int Y)
{
  // Ask the interaction mode manager to do the appropriate action for this event based on the currently
  // active interaction mode.
  if ( m_modeManager && m_modeManager->onMouseWheel( zDelta, X, Y ) )
  {
    // Request a redraw if the interaction hander requires it
    return true;
  }

  return false;
}

void Application::resetMode( TSL3DInteractionMode*, TSLButtonType, TSLDeviceUnits, TSLDeviceUnits )
{
  // Do nothing
}

void Application::viewChanged( TSL3DDrawingSurface* )
{
  // Do nothing
}

bool Application::zoomIn()
{
  // Ask the interaction mode manager to do the appropriate action for this event based on the currently
  // active interaction mode.
  if( m_modeManager )
  {
    // The return value from the mode manager indicates if a redraw is required
    return m_modeManager->zoomIn( 30 );
  }

  return false;
}

bool Application::zoomOut()
{
  // Ask the interaction mode manager to do the appropriate action for this event based on the currently
  // active interaction mode.
  if( m_modeManager )
  {
    // The return value from the mode manager indicates if a redraw is required
    return m_modeManager->zoomOut( 30 );
  }

  return false;
}

bool Application::resetView()
{
  if( m_modeManager )
  {
    // Ask the interaction mode manager to return the camera to the position and orientation
    // that we defined as the default inside onInitialUpdate(). The return value from the mode 
    // manager indicates if a redraw is required.
    return m_modeManager->resetToDefaultView();
  }

  return false;
}

void Application::activateEyePointInteractionMode()
{
  if( m_modeManager )
  {
    // Activate the interaction mode we want based on the ID we used when adding
    // the interaction mode to the mode manager initially inside onInitialUpdate().
    m_modeManager->setCurrentMode( TRACKBALL_EYEPOINT_ID );
  }
}

void Application::activateWorldInteractionMode()
{
  if( m_modeManager )
  {
    // Activate the interaction mode we want based on the ID we used when adding
    // the interaction mode to the mode manager initially inside onInitialUpdate().
    m_modeManager->setCurrentMode( TRACKBALL_WORLD_ID );
  }
}

const char* Application::interactionModePrompt() const
{
  if( m_modeManager )
  {
    // Return the status prompt for the currently active interaction mode.
    return m_modeManager->queryPrompt();
  }

  return NULL;
}

void Application::setWireframeMode( bool wireframe )
{
  if( m_drawingSurface )
  {
    // Set the wireframe rendering mode on the drawing surface to what was requested
    m_drawingSurface->set3DOption( TSL3DOptionWireframeMode, wireframe );
  }
}

void Application::setTerrainExaggeration( bool exaggerate )
{
  if( m_drawingSurface )
  {
    if( exaggerate )
    {
      // Make terrain features appear 10 times bigger than they really are
      m_drawingSurface->exaggerateTerrain( 10.0, 10.0 );
    }
    else
    {
      // Reset the appearance of terrain back to normal
      m_drawingSurface->exaggerateTerrain( 1.0, 1.0 );
    }
  }
}

void Application::setCameraAltitudeLimit( bool limit )
{
  if( m_drawingSurface )
  {
    // Tell the drawing surface whether or not it should stop the camera from moving to less than
    // 100 meters above the surface of the terrain.
    m_drawingSurface->camera()->limitHeightAboveEarth( limit, 100.0 );
  }
}

#ifndef WIN32
void Application::drawingInfo(GLXDrawable drawable, Display *display, Screen *screen, GLXContext context, Colormap colourmap, Visual *visual)
{
  // Store information from the MapLink widget necessary to create a drawing surface.
  m_display = display;
  m_drawable = drawable;
  m_screen = screen;
  m_context = context;
  m_colourmap = colourmap;
  m_visual = visual;
}
#else
void Application::drawingInfo( HGLRC context, WId hwnd )
{
  // Store information from the MapLink widget necessary to create a drawing surface.
  m_context = context;
  m_hwnd = hwnd;
}
#endif

