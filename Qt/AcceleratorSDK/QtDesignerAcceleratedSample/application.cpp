/****************************************************************************
                Copyright (c) 2008-2017 by Envitia Group PLC.
****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <QtGui>
#include <QMessageBox>

#include "application.h"
#include "maplinkwidget.h"
#include "AcceleratedSampleCustomDataLayer.h"

// Interaction mode IDs
#define ID_TOOLS_ZOOM                   32777
#define ID_TOOLS_PAN                    32778
#define ID_TOOLS_GRAB                   32784

// The name of our map layer. This is used when adding the data layer
// to the drawing surface and used to reference the data layer from the
// drawing surface
const char * Application::m_mapLayerName = "map" ;


void Application::redrawCallback(void *arg)
{
  // Callback received to notify us that the screen needs up-dating.
  // This will only occur if the tile drawing queue becomes empty.
  Application *view = (Application*)arg;

  // The callback will be in the context of the background thread
  // so we must not call back into MapLink.
  if (view->m_widget != NULL)
  {
    MapLinkWidget *widget = (MapLinkWidget*)view->m_widget;

    // Call a Qt signal.
    widget->changed();
  }
}


Application::Application(QGLWidget *widget, QLabel *statusLabel) :
  m_mapDataLayer(NULL),
  m_stdDataLayer(NULL),
  m_overlayType(ID_OVERLAYS_NONE),
  m_mapLoaded(false),
  m_drawingSurface(NULL),
#ifdef Q_WS_X11
  m_display(NULL),
  m_drawable(0),
  m_screen(NULL),
  m_colourmap(0),
  m_visual(NULL),
  m_context(0),
#else
  m_window(NULL),
  m_hglrc(NULL),
#endif
  m_modeManager(NULL),
  m_widget(widget),
  m_statusLabel(statusLabel),
  m_customDataLayer(NULL),
  m_customClient(NULL),
  m_mtRenderControl(NULL),
  m_blockingRenderControl(NULL),
  m_configuration(NULL),
  m_width(1),
  m_height(1),
  m_initialUpdate(true)
{
  //qWarning("Constructor start");
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Clear the error stack so that we can get the errors that occurred here.
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  TSLErrorStack::clear( ) ;

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
  TSLDrawingSurface::loadStandardConfig( );

  // Check for any errors that have occurred, and display them

  const char *msg = TSLErrorStack::errorString("Error") ;
  if ( msg )
  {
    // If we have any errors during initialisation, display the message
    // and exit.
    qWarning( msg ) ;
    exit( 0 ) ;
  }

  // Allocate the Configuration object
  m_configuration = new TSLAcceleratorConfiguration;
  //qWarning("Constructor end");
}


Application::~Application()
{
  // Clean up by destroying the map and pathlist
  if ( m_mapDataLayer )
  {
    m_mapDataLayer->destroy() ;
    m_mapDataLayer = NULL ;
  }
  if ( m_stdDataLayer )
  {
    m_stdDataLayer->destroy();
    m_stdDataLayer = NULL;
  }
  if (m_customDataLayer)
  {
    m_customDataLayer->destroy();
    m_customDataLayer = NULL;
  }
  if (m_customClient)
  {
    delete m_customClient;
    m_customClient = NULL;
  }

  // Clean up Accelerator Configuration object
  if (m_configuration)
  {
    delete m_configuration;
    m_configuration = NULL;
  }

  // Clean up Interaction mode manager
  if ( m_modeManager )
  {
    delete m_modeManager ;
    m_modeManager = 0 ;
  }

  // restart the stopped thread so we can do a clean shutdown.
  if (m_mtRenderControl)
  {
    m_mtRenderControl->resumeThread();
  }
  m_mtRenderControl = NULL; // owned by the Surface.
  m_blockingRenderControl = NULL;

  if (m_drawingSurface)
  {
    delete m_drawingSurface;
    m_drawingSurface = NULL;
  }
  TSLDrawingSurface::cleanup() ;
}


bool Application::loadMap(const char *mapFilename)
{
  // Clear the error stack, load the map then check for errors.
  TSLErrorStack::clear() ;

  if (mapFilename && mapFilename[0] != '\0')
  {
    if (m_mtRenderControl)
      m_mtRenderControl->stopThread();

    // load the map
    if ( !m_mapDataLayer->loadData( mapFilename ) )
    {
      QString message( "Could not load map data " + QString::fromUtf8( mapFilename ) );
      QMessageBox::critical( NULL, "Failed to load map", message );
      return false;
    }

    createGeometry(m_stdDataLayer);

    if (m_mtRenderControl)
      m_mtRenderControl->resumeThread();

    // Display any errors that have occurred
    const char *msg = TSLErrorStack::errorString( "Loading map\n" ) ;
    if ( msg )
    {
      qWarning(msg);
      TSLErrorStack::clear();
      return false;
    }
  }

  return true;
}

bool Application::addToSurface()
{
  if (!m_stdDataLayer || !m_mapDataLayer)
  {
    if (m_mapDataLayer == NULL)
    {
      m_mapDataLayer = new TSLMapDataLayer();
    }
    if (m_stdDataLayer == NULL)
    {
      m_stdDataLayer = new TSLStandardDataLayer();
    }
  }
  if (!m_customDataLayer && ! m_customClient)
  {
    // Create a custom datalayer and client.
    m_customDataLayer = new TSLAcceleratedCustomDataLayer;
    m_customClient = new AcceleratedSampleCustomDataLayer;
    m_customDataLayer->setClientCustomDataLayer((TSLAcceleratedClientCustomDataLayer*)m_customClient);
  }

  if (!m_drawingSurface)
  {
    return false;
  }

  if (m_mtRenderControl)
    m_mtRenderControl->stopThread();

  // Remove the layers
  m_drawingSurface->removeDataLayer(m_mapLayerName);
  m_drawingSurface->removeDataLayer("overlay");
  m_drawingSurface->removeDataLayer("custom");

  // Add the Map Data-layer to the drawing surface.
  //
  // Note: any number of datalayers can be added to a surface (each has it's own name)
  m_drawingSurface->addDataLayer( m_mapDataLayer, m_mapLayerName ) ;

  // Tell the surface to render this data layer into the background tiles.
  //
  // Note:
  //  1. Layers will only be added to the background renderer if
  //     the TSLPropertyBuffered flag is set to true.
  //
  //  2. Dynamic Data Object layers and Custom Data layers not designed for this
  //     surface will defeat the object of the Accelerated Surface by forcing
  //     a redraw every frame. These layers will need to be reimplemented for
  //     this surface.
  //
  //     Please see the documentation for class TSLAcceleratedSurface for
  //     additional information.
  //
  m_drawingSurface->setDataLayerProps(m_mapLayerName, TSLPropertyBuffered, true);

  // If we are displaying vector maps we may wish to enable the following
  // option.
  //
  // This option extends the view extent for Symbols and Text in the map layer
  // by the percentage specified. This is sometimes necessary to ensure that
  // text is drawn across the background generated map tiles.
  //
  // The value should be changed depending on the size of the text and symbols.
  //
  // If the value is too large performance will be affected.
  //
  m_drawingSurface->setDataLayerProps(m_mapLayerName, TSLPropertyLoadedSymbolsAndTextViewExpansion, 10);

  // Add the Standard DataLayer to the surface.
  //
  // If we double buffer the layer it will added to the background renderer.
  //
  // If we do not double buffer layer it will be rendered in the foreground
  // surface.
  //
  // All double buffered layers (except custom accelerated) will be drawn at
  // one time.
  //
  // Single buffered layers are drawin in order they were added (or subsequently
  // re-ordered). If a single buffered layer appears between double buffered layers
  // the single buffered layer is drawn after the double buffered layers.
  //
  // Single buffered layers before double buffered layers are drawn before. Normal
  // drawing surfaces (X11 & NT) draw double buffered layers first.
  //
  // The buffered layers are drawn as texture tiles using DirectX.
  //
  // Custom Accelerated layers can be drawn using either GDI (TMF Geometry) or OpenGL.
  //
  // Try to avoid too many switches between GDI and OpenGL as this will impact performance.
  //
  //
  // Overlay layer - mainly static data - order of adding is important
  m_drawingSurface->addDataLayer( m_stdDataLayer, "overlay" ) ;
  m_drawingSurface->setDataLayerProps( "overlay", TSLPropertyVisible, true); // visible by default.

  // Add the custom layer to the specified drawing surface.
  //
  // This type of layer will never be added to the background renderer.
  //
  m_drawingSurface->addDataLayer( m_customDataLayer, "custom" ) ;

  if (m_mtRenderControl)
    m_mtRenderControl->resumeThread();

  return true;
}


void Application::OnInitialUpdate()
{
  if (!m_initialUpdate && m_drawingSurface)
  {
    return ;  // allready created all that we need
  }
  // The first time ever, there will be no drawing surface.
  // If we have no drawing surface, create it, otherwise remove the old layer.
  if ( !m_drawingSurface )
  {
    // Create a double buffered drawing surface, and an interaction interface
    // to control it.
    // Set up the initial window extents.

#ifdef Q_WS_X11
    // Assume that we have called QtGLWidget::makeCurrent();
    m_context = glXGetCurrentContext();

    // Create the Accelerated Surface object
    m_drawingSurface = new TSLX11GLAcceleratedSurface( m_display, m_screen, m_drawable, false,
                                                       m_visual, m_colourmap, &m_context );
#else
    // Assume that we have called QtGLWidget::makeCurrent();
    m_hglrc = (void*)wglGetCurrentContext(); // store in a void * pointer so we do not expose windows type in the header
    m_drawingSurface = new TSLWGLAcceleratedSurface( (void *)m_window, false, (HGLRC*)&m_hglrc);
#endif

    // Enable dynamic arc map support.
    m_drawingSurface->setOption( TSLOptionDynamicArcSupportEnabled, true ) ;

    // Enable Double Buffering for Surface.
    m_drawingSurface->setOption( TSLOptionDoubleBuffered, false ) ;

    // Create a multi-threaded render control.
    //
    // We are requesting that the texture map tiles are loaded onto the
    // card in the background thread. You may wish to experiament with this setting.
    //
    // We also want a callback to occur once all the tiles have been loaded so that
    // we can redraw the screen.
    //
    // You may also wish to use a TSLAcceleratedBlockingRenderControl. This particular
    // render control blocks while each tile is rendered rather then use a thread
    // based approach.

    // Add the render control to the surface. This will create an Accelerated
    // Renderer to draw the map tiles.
    //

    // The first parameter askes the renderer to try and load the texture map tile in the
    // background thread.
    //
    // The second parameter asks the renderer to try and create a display list containing
    // the texture map tile.
    //
    // Peformance will be dependent on your graphics card and TSLAcceleratorConfiguration
    // setup. Please try varying these settings and the tile configuration to obtain
    // the best performance possible for you hardware setup.
#define MT_RENDERER
#ifdef MT_RENDERER
    m_mtRenderControl = new TSLAcceleratedMTRenderControl(false, false, Application::redrawCallback, this);
    m_drawingSurface->addRenderControl(m_mtRenderControl);
#else
    m_blockingRenderControl = new TSLAcceleratedBlockingRenderControl(redrawCallback, this);
    m_drawingSurface->addRenderControl(m_blockingRenderControl);
#endif
    // If we are using the Multi-threaded render control then zoom can either
    // occur as blocking or non-blocking.
    //
    // If you require non-blocking zoom then you will need to either provide
    // a redraw and possibly a tile callback method to ensure that the display
    // is updated when the tiles have been rendered.
    //
    // By default zooming blocks.
    m_drawingSurface->setOption(TSLOptionAcceleratorZoomAsynchronous, false);

    ////////////////////////////////////////////////////////////////////////
    // Reconfigure the number of tiles and tile size. This should be done before
    // any layers are added. If you do change the settings you should cause the
    // display to be redrawn.
    //
    // The values defined here are the defaults.
    //
    // We may wish to change the defaults to tune for the displayed window size
    // and the graphics card.
    const int tilePixelSize = 512;
    m_configuration->tileSize(tilePixelSize);  // size of tile in pixels (32, 64, 128, 256, 512, 1024 etc...)

    // If we are rotating the surface we need to consider how may tiles we
    // may possibly need to draw the bounding box around the circle scribed by the
    // view area being rotated.
    //
    // In addition panning requirements may also affect the number of tiles
    // required.
    //
    // The following settings are hints that control the maximum number of tiles
    // that will be held before old tiles are removed.
    //
    // However if the values are too small to be able to draw the rotated surface
    // the maximum will be increased temporarily. This may affect the pan performance
    // of a rotated surface as tiles just outside the required extent will be removed
    // from the tile cache too early.
    //
    // Calling TSLAcceleratedSurface::getConfiguration will return the number of tiles
    // currently being used if this is greater then the number requested.
    //
    m_configuration->numberTilesX(10);   // number of tiles in the X axis
    m_configuration->numberTilesY(10);   // number of tiles in the Y axis

    // Quality Settings
    m_configuration->textureFilterNearest(false);// GL_NEAREST (occasionally faster on old hardware, but you may
                                       // lose fine detail) or GL_LINEAR (sometimes slower, but
                                       // display is usually better).
    m_configuration->textureBorder(false);// If we are using GL_LINEAR, some graphics cards display
                                       // a thin tile boundary. By setting this to true we add a
                                       // small boundary to the texture, which lets the tile
                                       // boundaries blend, eliminating the thin boundary effect.
    m_configuration->dynamicArcTolerance(10.0); // If we have a dynamic arc map the x-axis extent
                                       // changes as you move north and south. This causes the
                                       // cached map tiles to be thrown away too often.
                                       // The tolerance setting allows us to specify how much
                                       // change in the x-axis extent we want to tolerate before
                                       // redrawing the map tiles.
    m_configuration->viewExpansion(12.5); // This allows us to configure the pre-loading of map
                                       // tiles, by specifying a percentage of the current extent
                                       // to look ahead by.
                                       // The value used will be dependent on the max' pan rate
                                       // you wish to support and the effect on performance by
                                       // rendering more tiles (adjust numberTiles* as well).

    // The MapLink Pro Accelerator Surface can do the swapping if necessary.
    // For this demo we are turning on the Qt swapping and turning off the
    // MapLink Pro swapping.
    // OGLWidget::setAutoBufferSwap( true );
    m_configuration->swapBuffers(!ML_QT_BUFFER_SWAP);

    //
    // Note: There is a trade off between tile size and speed of display of the tiles.
    //
    //       If you make the tile size too small then the time taken to display all the
    //       tiles becomes greater and will probably impact upon the frame rate you can
    //       achieve.
    //
    //       The value used should be tuned for your map, drawing area and graphics card.
    //
    //       The drawing of the tiles occurs in the background thread, so the number of tiles
    //       generated is less important. It is the actual time to display the tiles once
    //       they have been generated that needs to be considered.
    //
    m_drawingSurface->setConfiguration(*m_configuration);

    m_drawingSurface->setOption( TSLOptionDynamicArcSupportEnabled, true ) ; // could do this based on map
    m_drawingSurface->wndResize( 0, 0, m_width, m_height, false, TSLResizeActionMaintainTopLeft ) ;

    // Now create and initialse the mode manager and modes
#ifdef Q_WS_X11
    m_modeManager = new TSLInteractionModeManagerX11( this, m_drawingSurface, m_display, m_screen, m_drawable, m_colourmap) ;
#else
    m_modeManager = new TSLInteractionModeManagerNT( this, m_drawingSurface, (HWND)m_window ) ;
#endif

    m_modeManager->addMode( new TSLInteractionModeZoom( ID_TOOLS_ZOOM ), true ) ;
    m_modeManager->addMode( new TSLInteractionModePan( ID_TOOLS_PAN ), false ) ;
    m_modeManager->addMode( new TSLInteractionModeGrab( ID_TOOLS_GRAB ), false ) ;
    // The following mode will not work with the Accelerator.
    //m_modeManager->addMode( new TSLInteractionModeMagnify( ID_TOOLS_MAGNIFY ), false ) ;
  }

  // If the document loaded successfully, add it to the drawing surface
  m_mapLoaded = addToSurface( );

  // and reset the current view to display the entire map.
  m_drawingSurface->reset( ) ;

  // Display any errors that have occurred
  const char * msg = TSLErrorStack::errorString( "Cannot initialise view\n" ) ;
  if (msg)
  {
    qWarning(msg);
    TSLErrorStack::clear();
  }

  m_initialUpdate = false;

  // Reset the mode manager.
  m_modeManager->resetViews( ) ;
  m_modeManager->resetMode( TSLButtonNone, 0, 0 ) ;
}

void Application::reset()
{
  if (m_drawingSurface)
    m_drawingSurface->reset( ) ;
}

void Application::OnDraw()
{
  if ( m_drawingSurface )
  {
    TSLDeviceUnits x1, y1, x2, y2 ;
    m_drawingSurface->getDUExtent( &x1, &y1, &x2, &y2 ) ;
    m_drawingSurface->drawDU( x1, y1, x2, y2, !ML_QT_CLEAR_OGL ) ;

    // Don't forget to draw any echo rectangle that may be active.
    if ( m_modeManager )
    {
      // The rubber rectangle uses GDI/Xlib calls which will not
      // work if the main application swaps the buffers.
      //
      // The magnifier will not work at all.
      m_modeManager->onDraw( x1, y1, x2, y2 ) ;
    }
  }
  const char * msg = TSLErrorStack::errorString("Error") ;
  if ( msg )
  {
    // If we have any errors during initialisation, display the message
    // and exit.
    qWarning(msg);
    TSLErrorStack::clear();
  }
}

void Application::redraw()
{
  // need to redraw
  OnDraw();
}

void Application::OnSize(int cx, int cy)
{
  m_height = cy;
  m_width = cx;

  if ( m_drawingSurface )
  {
    // Inform the drawing surface of the new window size,
    // attempting to keep the top left corner the same.
    // Do not ask for an automatic redraw since we will get an OnDraw call
    m_drawingSurface->wndResize( 0, 0, cx, cy, false, TSLResizeActionMaintainTopLeft ) ;
  }
  if ( m_modeManager )
  {
    m_modeManager->onSize( cx, cy ) ;
  }
}

////////////////////////////////////////////////////////////////////////
bool Application::OnMouseMove(unsigned int buttonPressed, bool shiftPressed, bool controlPressed, int mx, int my)
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_modeManager )
  {
    bool result = m_modeManager->onMouseMove( (TSLButtonType)buttonPressed, mx, my, shiftPressed, controlPressed );

    // Update the cursor position on the status bar.
    if (m_statusLabel)
    {
      double lat, lon;
      const TSLCoordinateSystem* coordSystem = m_mapDataLayer->queryCoordinateSystem();

      const char *name = "";
      if (coordSystem)
        name = coordSystem->projectionClassName();

      bool result = m_drawingSurface->DUToLatLong( mx, my, &lat, &lon ) ;
      char temp[1024];
      if (result)
      {
        if (strlen(name) == 0)
          sprintf(temp, "Lat = %02.2f Lon = %03.2f", lat, lon);
        else
          sprintf(temp, "Lat = %02.2f Lon = %03.2f Projection : %s", lat, lon, name);
      }
      else
      {
        if (strlen(name) == 0)
          sprintf(temp, "Projection problem");
        else
          sprintf(temp, "Projection problem. Projection is %s", name);
      }
      QString text(temp);
      m_statusLabel->setText(text);
    }
    // Request a redraw if the interaction hander requires it
    return result;
  }
  return false;
}

bool Application::OnLButtonDown(bool shiftPressed, bool controlPressed, int X, int Y)
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_modeManager )
  {
    if ( m_modeManager->onLButtonDown( X, Y, shiftPressed, controlPressed) )
    {
      // Request a redraw if the interaction hander requires it
      return true;
    }
  }
  return false;
}

bool Application::OnMButtonDown(bool shiftPressed, bool controlPressed, int X, int Y)
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_modeManager )
  {
    if ( m_modeManager->onMButtonDown( X, Y, shiftPressed, controlPressed) )
    {
      // Request a redraw if the interaction hander requires it
      return true;
    }
  }
  return false;
}

bool Application::OnRButtonDown(bool shiftPressed, bool controlPressed, int mx, int my)
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_modeManager )
  {
    if ( m_modeManager->onRButtonDown( mx, my, shiftPressed, controlPressed) )
    {
      // Request a redraw if the interaction hander requires it
      return true;
    }
  }
  return false;
}

bool Application::OnLButtonUp(bool shiftPressed, bool controlPressed, int mx, int my)
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_modeManager )
  {
    if ( m_modeManager->onLButtonUp( mx, my, shiftPressed, controlPressed) )
    {
      // Request a redraw if the interaction hander requires it
      return true;
    }
  }
  return false;
}

bool Application::OnMButtonUp(bool shiftPressed, bool controlPressed, int mx, int my)
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_modeManager )
  {
    if ( m_modeManager->onMButtonUp( mx, my, shiftPressed, controlPressed) )
    {
      // Request a redraw if the interaction hander requires it
      return true;
    }
  }
  return false;
}

bool Application::OnRButtonUp(bool shiftPressed, bool controlPressed, int mx, int my)
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_modeManager )
  {
    if ( m_modeManager->onRButtonUp( mx, my, shiftPressed, controlPressed) )
    {
      // Request a redraw if the interaction hander requires it
      return true;
    }
  }
  return false;
}

bool Application::OnKeyPress(bool, bool, int keySym)
{
  switch (toupper(keySym))
  {
    case 'N':
      m_overlayType = ID_OVERLAYS_NONE;
      break;
    case 'L':
      m_overlayType = ID_OVERLAYS_LINE;
      break;
    case 'P':
      m_overlayType = ID_OVERLAYS_POLYGON;
      break;
    case 'T':
      m_overlayType = ID_OVERLAYS_TEXT;
      break;
    case 'S':
      m_overlayType = ID_OVERLAYS_SYMBOL;
      break;
    case 'F':
      m_overlayType = ID_OVERLAYS_FEATURE;
      break;
  }
  return true;
}

bool Application::OnMouseWheel(bool, bool, short zDelta, int X, int Y)
{
  if ( m_modeManager )
  {
    if ( m_modeManager->onMouseWheel( zDelta, X, Y ) )
    {
      // Request a redraw if the interaction hander requires it
      return true ;
    }
  }
  return false;
}

///////////////////////////////////////////////////////////////////////////

void Application::OnToolsPan()
{
  if ( m_modeManager )
  {
    m_modeManager->setCurrentMode( ID_TOOLS_PAN ) ;
  }
}

void Application::OnToolsZoom()
{
  if ( m_modeManager )
  {
    m_modeManager->setCurrentMode( ID_TOOLS_ZOOM ) ;
  }
}

void Application::OnToolsGrab()
{
  if ( m_modeManager )
  {
    m_modeManager->setCurrentMode( ID_TOOLS_GRAB ) ;
  }
}

//void Application::OnToolsMagnify()
//{
  //if ( m_modeManager )
  //{
  //  m_modeManager->setCurrentMode( ID_TOOLS_MAGNIFY ) ;
  //}
//}

bool Application::OnToolsZoomin()
{
  if ( m_modeManager )
  {
    if ( m_modeManager->zoomIn( 30 ) )
      return true ;
  }
  return false;
}

bool Application::OnToolsZoomout()
{
  if ( m_modeManager )
  {
    if ( m_modeManager->zoomOut( 30 ) )
      return true ;
  }
  return false;
}

bool Application::OnToolsReset()
{
  if ( m_modeManager )
  {
    if ( m_modeManager->resetToFullExtent( ) )
      return true ;
  }
  return false;
}

void Application::resetMode(TSLInteractionMode *, TSLButtonType, TSLDeviceUnits, TSLDeviceUnits)
{
}

void Application::viewChanged(TSLDrawingSurface*)
{
}

bool Application::OnSavedviewsGoto1()
{
  if ( m_modeManager )
  {
    if ( m_modeManager->savedViewGoto( 0 ) )
      return true ;
  }
  return false;
}

bool Application::OnSavedviewsGoto2()
{
  if ( m_modeManager )
  {
    if ( m_modeManager->savedViewGoto( 1 ) )
      return true ;
  }
  return false;
}

bool Application::OnSavedviewsGoto3()
{
  if ( m_modeManager )
  {
    if ( m_modeManager->savedViewGoto( 2 ) )
      return true ;
  }
  return false;
}

void Application::OnSavedviewsReset()
{
  if ( m_modeManager )
    m_modeManager->savedViewReset( ) ;
}

bool Application::OnSavedviewsSet1()
{
  if ( m_modeManager )
  {
    m_modeManager->savedViewSetToCurrent( 0 ) ;
    return m_modeManager->savedViewValid(0);
  }
  return false;
}

bool Application::OnSavedviewsSet2()
{
  if ( m_modeManager )
  {
    m_modeManager->savedViewSetToCurrent( 1 ) ;
    return m_modeManager->savedViewValid(1);
  }
  return false;
}

bool Application::OnSavedviewsSet3()
{
  if ( m_modeManager )
  {
    m_modeManager->savedViewSetToCurrent( 2 ) ;
    return m_modeManager->savedViewValid(2);
  }
  return false;
}

bool Application::OnViewstackNextview()
{
  if ( m_modeManager )
  {
    if ( m_modeManager->viewStackGotoNext( ) )
      return true ;
  }
  return false;
}

bool Application::OnViewstackPreviousview()
{
  if ( m_modeManager )
  {
    if ( m_modeManager->viewStackGotoPrevious( ) )
      return true ;
  }
  return false;
}


///////////////////////////////////////////////////////////////////////////
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Sets the Map background colour by querying the Map Datalayer
// for the colour and either clearing the draw surface background
// colour or setting it to the colour specified in the datalayer.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Application::setMapBackgroundColour()
{
  // Set the Map background colour.
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Query the colour from tha Map datalayer
  int backgroundColour = m_mapDataLayer->getBackgroundColour();

  // If there is no colour then clear the colour set in the
  // drawing surface or we will keep the old colour (the
  // default colour is white).
  //
  // If there is a colour set it.
  //
  // If we have multiple map data layers attached to the drawing
  // surface we would need to decide at application level
  // what colour to use.
  //
  // When we originally attach a datalayer the drawing surface
  // sets the background colour using the colour in the datalayer
  // however on subsequent load's the background colour is not
  // read, as there is a knock on effect depending on which
  // drawing surfaces a layer is attached to and the order
  // and number of other attached layers.
  //
  if (backgroundColour == -1)
  {
    m_drawingSurface->clearBackgroundColour();
  }
  else
  {
    m_drawingSurface->setBackgroundColour(backgroundColour);
  }
}

#ifdef WINNT
void Application::drawingInfo(WId window)
{
  m_window = window;
}

#else
void Application::drawingInfo(Drawable drawable, Display *display, Screen *screen, Colormap colourmap, Visual *visual)
{
  m_display = display;
  m_drawable = drawable;
  m_screen = screen;
  m_colourmap = colourmap;
  m_visual = visual;
}
#endif


//////////////////////////////////////////////////////////
bool Application::createOverlay( TSLTMC x, TSLTMC y, TSLDrawingSurface * ds )
{
  switch ( m_overlayType )
  {
    case ID_OVERLAYS_LINE :    return createPolyline( x, y, ds ) ;
    case ID_OVERLAYS_POLYGON : return createPolygon( x, y, ds ) ;
    case ID_OVERLAYS_TEXT :    return createText( x, y, ds ) ;
    case ID_OVERLAYS_SYMBOL :  return createSymbol( x, y, ds ) ;
    case ID_OVERLAYS_FEATURE : return createFeature( x, y, ds ) ;
  }
  return false ;
}

bool Application::createText( TSLTMC x, TSLTMC y, TSLDrawingSurface * )
{
  // Before Editing a layer which is in the background thread stop
  // the background thread.
  if (m_mtRenderControl)
    m_mtRenderControl->stopThread();

  TSLEntitySet * es = m_stdDataLayer->entitySet() ;
  TSLText * txt = es->createText( 0, x, y, "Hello World" ) ;
  if ( !txt )
  {
    if (m_mtRenderControl)
      m_mtRenderControl->resumeThread();
    return false ;
  }

  int black = TSLComposeRGB( 0, 0, 0 ) ;

  // Set the rendering of the text to be Vector Text 25 pixels high
  txt->setRendering( TSLRenderingAttributeTextFont, 56 ) ;
  txt->setRendering( TSLRenderingAttributeTextColour, black ) ;
  txt->setRendering( TSLRenderingAttributeTextSizeFactor, 25.0 ) ;
  txt->setRendering( TSLRenderingAttributeTextSizeFactorUnits,
                     TSLDimensionUnitsPixels ) ;

  // Tell the layer that its contents have changed
  m_stdDataLayer->notifyChanged( true ) ;

  if (m_mtRenderControl)
    m_mtRenderControl->resumeThread();

  return true ;
}

bool Application::createPolygon( TSLTMC x, TSLTMC y, TSLDrawingSurface *ds )
{
  if (m_mtRenderControl)
    m_mtRenderControl->stopThread();

  TSLEntitySet * es = m_stdDataLayer->entitySet() ;

  // Create a coordinate list forming a triangle around the position
  // Use the Drawing Surafce to calculate the coordinates
  // We will make our triangle 25 pixels either side of the position
  // Note that the pixels are at the current zoom factor - the polygon
  // is always completely scalable
  TSLCoordSet * coords = new TSLCoordSet() ;
  if ( !coords )
  {
    if (m_mtRenderControl)
      m_mtRenderControl->resumeThread();
    return false ;
  }

  double tmcPerDUX, tmcPerDUY;
  ds->TMCperDU( tmcPerDUX, tmcPerDUY );
  coords->add( x - 25 * tmcPerDUX, y - 25 * tmcPerDUY ) ;
  coords->add( x + 25 * tmcPerDUX, y - 25 * tmcPerDUY ) ;
  coords->add( x,                  y + 25 * tmcPerDUY ) ;

  // Hand ownership of the coordset to the polygon
  TSLPolygon * poly = es->createPolygon( 0, coords, true ) ;
  if ( !poly )
  {
    if (m_mtRenderControl)
      m_mtRenderControl->resumeThread();
    return false ;
  }

  int yellow = TSLComposeRGB( 255, 255, 0 ) ;
  int black  = TSLComposeRGB( 0, 0, 0 ) ;
  poly->setRendering( TSLRenderingAttributeFillStyle, 1 ) ;
  poly->setRendering( TSLRenderingAttributeFillColour, yellow ) ;
  poly->setRendering( TSLRenderingAttributeEdgeStyle, 1 ) ;
  poly->setRendering( TSLRenderingAttributeEdgeColour, black ) ;
  poly->setRendering( TSLRenderingAttributeEdgeThickness, 1.0 ) ;

  // Tell the layer that its contents have changed
  m_stdDataLayer->notifyChanged( true ) ;

  if (m_mtRenderControl)
    m_mtRenderControl->resumeThread();

  return true ;
}

bool Application::createPolyline( TSLTMC x, TSLTMC y, TSLDrawingSurface * ds )
{
  if (m_mtRenderControl)
    m_mtRenderControl->stopThread();

  TSLEntitySet * es = m_stdDataLayer->entitySet() ;

  // Create a coordinate list forming a triangle around the position
  // Use the Drawing Surafce to calculate the coordinates
  TSLCoordSet * coords = new TSLCoordSet() ;
  if ( !coords )
  {
    if (m_mtRenderControl)
      m_mtRenderControl->resumeThread();
    return false ;
  }

  // Make a triangle, 1km either side of the specified position
  double tmcPerMU = ds->TMCperMU() ;
  coords->add( x - 1000 * tmcPerMU, y + 1000 * tmcPerMU ) ;
  coords->add( x,                   y - 1000 * tmcPerMU ) ;
  coords->add( x + 1000 * tmcPerMU, y + 1000 * tmcPerMU ) ;

  // Hand ownership of the coordset to the polygon
  TSLPolyline * poly = es->createPolyline( 0, coords, true ) ;
  if ( !poly )
  {
    if (m_mtRenderControl)
      m_mtRenderControl->resumeThread();
    return false ;
  }

  // Use MapUnit thickness so the line thickness scales as we zoom in/out
  // Set it to 20m. Complex line styles - like style 48 have thickness
  // clamping applied automatically to avoid performance
  // or aesthetic problems
  int yellow = TSLComposeRGB( 255, 255, 0 ) ;
  poly->setRendering( TSLRenderingAttributeEdgeStyle, 48 ) ;
  poly->setRendering( TSLRenderingAttributeEdgeColour, yellow ) ;
  poly->setRendering( TSLRenderingAttributeEdgeThickness, 20.0 ) ;
  poly->setRendering( TSLRenderingAttributeEdgeThicknessUnits,
                      TSLDimensionUnitsMapUnits) ;

  // Tell the layer that its contents have changed
  m_stdDataLayer->notifyChanged( true ) ;

  if (m_mtRenderControl)
    m_mtRenderControl->resumeThread();

  return true ;
}

bool Application::createSymbol( TSLTMC x, TSLTMC y, TSLDrawingSurface * )
{
  if (m_mtRenderControl)
    m_mtRenderControl->stopThread();

  TSLEntitySet * es = m_stdDataLayer->entitySet() ;

  TSLSymbol * symbol = es->createSymbol( 0, x, y ) ;
  if ( !symbol )
  {
    if (m_mtRenderControl)
      m_mtRenderControl->resumeThread();
    return false ;
  }

  // Create a green star, 1000m high. This looks sensible on the Dorset map!
  int green = TSLComposeRGB( 0, 255, 0 ) ;
  symbol->setRendering( TSLRenderingAttributeSymbolStyle, 14 ) ;
  symbol->setRendering( TSLRenderingAttributeSymbolColour, green ) ;
  symbol->setRendering( TSLRenderingAttributeSymbolSizeFactor, 1000.0 ) ;
  symbol->setRendering( TSLRenderingAttributeSymbolSizeFactorUnits,
                        TSLDimensionUnitsMapUnits ) ;


  int black = TSLComposeRGB( 0, 0, 0 );

  // Make up a feature name and numeric ID
  m_stdDataLayer->addFeatureRendering( "Airport", 123 ) ;

  // Associate some rendering with the new feature, use ID for efficiency
  m_stdDataLayer->setFeatureRendering( 0, 123, TSLRenderingAttributeSymbolStyle, 6003 ) ;
  m_stdDataLayer->setFeatureRendering( 0, 123, TSLRenderingAttributeSymbolColour, black ) ;
  m_stdDataLayer->setFeatureRendering( 0, 123, TSLRenderingAttributeSymbolSizeFactor, 40.0 ) ;
  m_stdDataLayer->setFeatureRendering( 0, 123, TSLRenderingAttributeSymbolSizeFactorUnits, TSLDimensionUnitsPixels ) ;

  // Tell the layer that its contents have changed
  m_stdDataLayer->notifyChanged( true ) ;

  if (m_mtRenderControl)
    m_mtRenderControl->resumeThread();

  return true ;
}

bool Application::createFeature( TSLTMC x, TSLTMC y, TSLDrawingSurface * )
{
  if (m_mtRenderControl)
    m_mtRenderControl->stopThread();

  TSLEntitySet * es = m_stdDataLayer->entitySet() ;

  // 123 is the numeric feature code we assigned on the Data Layer
  TSLSymbol * symbol = es->createSymbol( 123, x, y ) ;

  if ( !symbol )
  {
    if (m_mtRenderControl)
      m_mtRenderControl->resumeThread();
    return false ;
  }

  // No need to configure any rendering, MapLink will look it up
  // from the Data Layer at display time.

  // Tell the layer that its contents have changed
  m_stdDataLayer->notifyChanged( true ) ;

  if (m_mtRenderControl)
    m_mtRenderControl->resumeThread();

  return true ;
}


void Application::createGeometry(TSLStandardDataLayer *layer)
{
  TSLTMC x1, y1, x2, y2;
  m_mapDataLayer->getTMCExtent( &x1, &y1, &x2, &y2 );
  TSLEnvelope mapExtent( x1, y1, x2, y2); // required by Green Hills V3.5 compiler

  // Get the top level Geometry Object (Basically a Group).
  TSLEntitySet* parent = layer->entitySet();

  // Clear the current contents out.
  parent->clear();

  // We are going to create 10 different primitives in a strip across
  // the middle of the map, so work out the extents now....
  TSLEnvelope   extents[10];

  // The top and bottom are 10% of the map height, centred around the middle of the map
  TSLTMC  top = mapExtent.centre().y() + (TSLTMC)( (double)mapExtent.height() * 0.05 );
  TSLTMC  bottom = mapExtent.centre().y() - (TSLTMC)( (double)mapExtent.height() * 0.05 );

  TSLTMC  x = mapExtent.xMin();
  TSLTMC  inc = mapExtent.width() / 11;

  for ( int i = 0; i < 10; i++ )
  {
    extents[i].corners( x, bottom, x + inc, top );
    x += inc;
  }
#define ARCID 1
#define BPID  2
#define ELLID 3
#define POLYID 4
#define RECTID 5
#define SCALESYMID 6
#define NONSCALESYMID 7
#define SCALETEXTID 8
#define NONSCALETEXTID 9
#define POLYLINEID 10

  // Now start creating the objects and setting thier drawing attributes
  // ALSO see GeometryCreation sample for NT.

  // Arc

  TSLArc * arc;
  arc = parent->createArc( ARCID, 0.1 * M_PI/180.0 , 359.1 * M_PI/180.0, extents[0].centre().x(), extents[0].centre().y(), extents[0].width() );

  arc->setRendering( TSLRenderingAttributeEdgeColour, 1 );
  arc->setRendering( TSLRenderingAttributeEdgeStyle , 1 );
  arc->setRendering( TSLRenderingAttributeEdgeThickness, 2 );

  arc = parent->createArc( ARCID, 359.8 * M_PI/180.0, 359.999 * M_PI/180.0, extents[0].centre().x()+300, extents[0].centre().y(), extents[0].width() );

  arc->setRendering( TSLRenderingAttributeEdgeColour, 1 );
  arc->setRendering( TSLRenderingAttributeEdgeStyle , 1 );
  arc->setRendering( TSLRenderingAttributeEdgeThickness, 2 );

  double startAngle = 10;
  double endAngle = 55;
  for (int i = 0; i < 10; ++i)
  {
    arc = parent->createArc( ARCID, startAngle * M_PI/180.0, endAngle * M_PI/180.0,
               extents[0].centre().x(),
               (extents[0].centre().y() / 2) + (extents[0].width() / 4) * i,
               extents[0].width(), extents[0].width() );

    arc->setRendering( TSLRenderingAttributeEdgeColour, 1);
    arc->setRendering( TSLRenderingAttributeEdgeStyle , 1 );
    arc->setRendering( TSLRenderingAttributeEdgeThickness, 2 );

    startAngle += 10;
    endAngle += 15;
  }


  startAngle = 0;
  endAngle = 45;
  for (int i = 0; i < 10; ++i)
  {
    arc = parent->createArc( ARCID, startAngle * M_PI/180.0, endAngle * M_PI/180.0,
               extents[0].centre().x(),
               (extents[0].centre().y() / 2) - (extents[0].width() / 4) * i,
               extents[0].width(), extents[0].width() / 2. );

    arc->setRendering( TSLRenderingAttributeEdgeColour, 1 * i);
    arc->setRendering( TSLRenderingAttributeEdgeStyle , 1 );
    arc->setRendering( TSLRenderingAttributeEdgeThickness, 2 );

    startAngle += 10;
    endAngle += 15;
  }

  // Bordered Polygon
  TSLCoordSet*  coords = new TSLCoordSet;
  coords->add( extents[1].bottomLeft().x(), extents[1].bottomLeft().y() );
  coords->add( extents[1].bottomLeft().x(), extents[1].topRight().y() );
  coords->add( extents[1].topRight().x(), extents[1].topRight().y() );
  coords->add( extents[1].topRight().x(), extents[1].bottomLeft().y() );
  TSLBorderedPolygon *borderedPolygon = parent->createBorderedPolygon( BPID, coords, extents[1].width() / 10, true );

  borderedPolygon->setRendering( TSLRenderingAttributeFillColour, 3 ) ;
  borderedPolygon->setRendering( TSLRenderingAttributeFillStyle,  35 ) ;
  borderedPolygon->setRendering( TSLRenderingAttributeExteriorEdgeStyle, 1 ) ;
  borderedPolygon->setRendering( TSLRenderingAttributeExteriorEdgeColour, 1 ) ;
  borderedPolygon->setRendering( TSLRenderingAttributeExteriorEdgeThickness, 2 ) ;
  borderedPolygon->setRendering( TSLRenderingAttributeBorderColour, 2 ) ;

  // Ellipse
  TSLEllipse *ellipse = parent->createEllipse( ELLID, extents[2].centre().x(), extents[2].centre().y(), extents[2].width() / 2, extents[2].width() / 4 );

  ellipse->setRendering( TSLRenderingAttributeEdgeColour, 1 ) ;
  ellipse->setRendering( TSLRenderingAttributeEdgeStyle,  1 ) ;
  ellipse->setRendering( TSLRenderingAttributeEdgeThickness, 2 ) ;
  ellipse->setRendering( TSLRenderingAttributeFillColour, 106 ) ;
  ellipse->setRendering( TSLRenderingAttributeFillStyle,  1 ) ;


  for (int i = 0; i < 10; ++i)
  {
    ellipse = parent->createEllipse( ELLID, extents[2].centre().x(),
                  extents[2].centre().y() + (extents[2].width() / 4) * i,
                  extents[2].width() / 8, extents[2].width() / 8, (10.0 * i) / M_PI * 180.0  );

    ellipse->setRendering( TSLRenderingAttributeEdgeColour, 1 ) ;
    ellipse->setRendering( TSLRenderingAttributeEdgeStyle,  1 ) ;
    ellipse->setRendering( TSLRenderingAttributeEdgeThickness, 2 ) ;
    ellipse->setRendering( TSLRenderingAttributeFillColour, 106 ) ;
    ellipse->setRendering( TSLRenderingAttributeFillStyle,  1 ) ;
  }

  for (int i = 0; i < 10; ++i)
  {
    ellipse = parent->createEllipse( ELLID, extents[2].centre().x(),
                  extents[2].centre().y() - (extents[2].width() / 4) * i,
                  extents[2].width() / 7, extents[2].width() / 8, (10.0 * i) / M_PI * 180.0  );

    ellipse->setRendering( TSLRenderingAttributeEdgeColour, 1 ) ;
    ellipse->setRendering( TSLRenderingAttributeEdgeStyle,  1 ) ;
    ellipse->setRendering( TSLRenderingAttributeEdgeThickness, 2 ) ;
    ellipse->setRendering( TSLRenderingAttributeFillColour, 106 ) ;
    ellipse->setRendering( TSLRenderingAttributeFillStyle,  1 ) ;
  }

  for (int i = 0; i < 10; ++i)
  {
    ellipse = parent->createEllipse( ELLID, extents[2].centre().x() + (extents[2].width() / 4),
                  extents[2].centre().y() + (extents[2].width() / 4) * i,
                  extents[2].width() / 8, extents[2].width() / 8 );

    ellipse->setRendering( TSLRenderingAttributeEdgeColour, 1 ) ;
    ellipse->setRendering( TSLRenderingAttributeEdgeStyle,  1 ) ;
    ellipse->setRendering( TSLRenderingAttributeEdgeThickness, 2 ) ;
    ellipse->setRendering( TSLRenderingAttributeFillColour, 106 ) ;
    ellipse->setRendering( TSLRenderingAttributeFillStyle,  1 ) ;
  }


  // Polygon
  coords = new TSLCoordSet;
  coords->add( extents[3].bottomLeft().x(), extents[3].bottomLeft().y() );
  coords->add( extents[3].bottomLeft().x(), extents[3].topRight().y() );
  coords->add( extents[3].topRight().x(), extents[3].topRight().y() );
  coords->add( extents[3].topRight().x(), extents[3].bottomLeft().y() );
  TSLPolygon * poly = parent->createPolygon( POLYID, coords, true );
  poly->setRendering( TSLRenderingAttributeFillColour, 181 ) ;
  poly->setRendering( TSLRenderingAttributeFillStyle,  36 ) ;
  poly->setRendering( TSLRenderingAttributeExteriorEdgeStyle, 1 ) ;
  poly->setRendering( TSLRenderingAttributeExteriorEdgeColour, 1 ) ;
  poly->setRendering( TSLRenderingAttributeExteriorEdgeThickness, 2 ) ;

  // Rectangle
  TSLRectangle *rect = parent->createRectangle( RECTID, extents[4].bottomLeft(), extents[4].topRight() );
  rect->setRendering( TSLRenderingAttributeFillColour, 181 ) ;
  rect->setRendering( TSLRenderingAttributeFillStyle,  503 ) ;
  rect->setRendering( TSLRenderingAttributeExteriorEdgeStyle, 1 ) ;
  rect->setRendering( TSLRenderingAttributeExteriorEdgeColour, 1 ) ;
  rect->setRendering( TSLRenderingAttributeExteriorEdgeThickness, 2 ) ;


  // Symbols
  TSLSymbol * symbol = parent->createSymbol( SCALESYMID, extents[5].centre().x(), extents[5].centre().y(), extents[5].height() / 5 );
  symbol->setRendering( TSLRenderingAttributeSymbolColour, 66 ) ;
  symbol->setRendering( TSLRenderingAttributeSymbolStyle,  6001 ) ;

  symbol = parent->createSymbol( NONSCALESYMID, extents[6].centre().x(), extents[6].centre().y(), 20, 45.0 / M_PI * 180.0 );
  symbol->setRendering( TSLRenderingAttributeSymbolColour, 66 ) ;
  symbol->setRendering( TSLRenderingAttributeSymbolStyle,  6001 ) ;
  symbol->setRendering( TSLRenderingAttributeSymbolSizeFactorUnits, TSLDimensionUnitsPixels ) ;
  symbol->setRendering( TSLRenderingAttributeSymbolSizeFactor, 40 ) ; // height is in pixels
  symbol->setRendering( TSLRenderingAttributeSymbolRotatable,  TSLSymbolRotationEnabled ) ;


  // Text
  // Note: The effect of scaling non-vector text is highly dependent on the
  //       font's installed and the font specification in tslfonts.dat.
  //
  //       Only vector font's can be rotated on X11.
  //
  TSLText * text = parent->createText( SCALETEXTID, extents[7].centre().x(), extents[7].centre().y(), "Scale-Text", extents[7].height() / 5 );
  text->setRendering( TSLRenderingAttributeTextColour, 1 ) ;
  text->setRendering( TSLRenderingAttributeTextFont, 1 ) ;

  //
  // A non scalable text is defined as index 57 in the file tslfonts.dat.
  //
  // In this case it is user11x19, but could be a fully specified
  // font (for example -adobe-courier-bold-o-normal--12-120-75-75-m-70-hp-roman8)
  //
  // Another method for setting a non scaleable font is to
  // set Text units to Pixels and set a size factor as shown below.
  //
  // Note: Font selection in X is very dependent on the available
  //       fonts. If a particular font size is not available X
  //       tends to provide a substitute.
  //
  //       Additionally not all fonts are available on all systems.
  //       You may need to substitute fonts in tslfonts.dat for
  //       ones available to you or add additional fonts to your
  //       system.
  //
  text = parent->createText( NONSCALETEXTID, extents[8].centre().x(), extents[8].centre().y(), "Fixed-Text", 19 );
  text->setRendering( TSLRenderingAttributeTextColour, 1 ) ;
  text->setRendering( TSLRenderingAttributeTextFont, 2 ) ;
  text->setRendering( TSLRenderingAttributeTextSizeFactorUnits, TSLDimensionUnitsPixels ) ;
  text->setRendering( TSLRenderingAttributeTextSizeFactor, 19 ) ;

  // PolyLine

  for (int i = 0; i < 13; ++i)
  {
    coords = new TSLCoordSet;
    coords->add( extents[9].bottomLeft().x(), (extents[9].bottomLeft().y()) + (extents[9].width() / 7) * i );
    coords->add( extents[9].topRight().x(), extents[9].topRight().y() + (extents[9].width() / 7) * i);
    coords->add( extents[9].bottomLeft().x(), extents[9].topRight().y() + (extents[9].width() / 7) * i);

    TSLPolyline * polyline = parent->createPolyline( POLYLINEID, coords, true );
    polyline->setRendering( TSLRenderingAttributeEdgeStyle, i ) ;
    polyline->setRendering( TSLRenderingAttributeEdgeColour, i * 8 ) ;
    polyline->setRendering( TSLRenderingAttributeEdgeThickness, 1 ) ;
  }
}
