/****************************************************************************
                Copyright (c) 2013-2017 by Envitia Group PLC.
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <QtGui>
#include <QMessageBox>

#ifdef X11_BUILD
# include <QtPlatformHeaders/QGLXNativeContext>
# include <QX11Info>
#elif WINNT
# include <QtPlatformHeaders/QWGLNativeContext>
#endif

#include "application.h"
#include "maplinkwidget.h"

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

  if( m_drawingSurface )
  {
    delete m_drawingSurface;
    m_drawingSurface = NULL;
  }

  // Beyond this point MapLink will no longer be used - clear up all static data.
  // Once this is done no MapLink functions or classes can be used.
  TSLDrawingSurface::cleanup( true );
}


bool Application::loadFile(const char *fileFilename)
{
  if( !m_mapDataLayer ) return false;
  
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

QWindow* Application::windowForWidget(const QWidget* widget)
{
  QWindow* window = widget->windowHandle();
  if (window)
    return window;
  const QWidget* nativeParent = widget->nativeParentWidget();
  if (nativeParent)
    return nativeParent->windowHandle();
  return 0;
}

void Application::create( TSLDrawingSurfaceDrawCallback *redrawCallback )
{
  if( m_drawingSurface )
  {
    // We have already been called before
    return;
  }
  if( !m_context )
  {
    // We don't have an OpenGL context to attach to
    return;
  }

  // These options specify how the MapLink surface will act when attached
  // to the context
  // In this case we will render using Qt's context directly (Won't create a shared one),
  // and will perform buffer swaps within MapLink.
  TSLOpenGLSurfaceCreationParameters creationOptions;
  creationOptions.swapBuffersManually( false );

  // Retrieve the native OpenGL context type, as needed by the MapLink API
  QVariant nativeContextVariant = m_context->nativeHandle();
#ifdef X11_BUILD  
  if( nativeContextVariant.typeName() != std::string("QGLXNativeContext") ) {
	  // The context is not a GLX Context
	  return;
  }
  QGLXNativeContext* nativeContext = reinterpret_cast<QGLXNativeContext*>(nativeContextVariant.data());
  if( !nativeContext ) {
	  return;
  }
  GLXContext ctx = nativeContext->context();
  
  // Retrieve the underlying window system handles required by MapLink
  Display* display = nativeContext->display();
  if( !display ) {
	  display = QX11Info::display();
  }
  Window win = nativeContext->window();
  if( win == 0 ) {
	  win = m_parentWidget->winId();
  }
  
  XWindowAttributes attribs;
  XGetWindowAttributes( display, win, &attribs );
#elif WINNT
  if (nativeContextVariant.typeName() != std::string("QWGLNativeContext")) {
	  // Not a Windows OpenGL context
	  return;
  }
  QWGLNativeContext* nativeContext = reinterpret_cast<QWGLNativeContext*>(nativeContextVariant.data());
  if (!nativeContext) {
	  return;
  }
  HGLRC ctx = nativeContext->context();

  // Retrieve the underlying window system handles required by MapLink
  // When using QOpenGLWidget the MapLink surface must be attached using the HDC rather than the HWND
  HDC hdc = wglGetCurrentDC();
#else
# error "Sample not setup for this platform. Please implement retrieval of native OpenGL Context from QOpenGLContext"
#endif  

#ifdef X11_BUILD
  GLXDrawable drawable = glXGetCurrentDrawable();
  m_drawingSurface = new TSLGLXSurface( display, attribs.screen, drawable, ctx, creationOptions );
#else
  HGLRC context = wglGetCurrentContext();
  m_drawingSurface = new TSLWGLSurface( hdc, true, ctx, creationOptions );
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

  // Enable tiling of buffered layers by default for best performance. This can be turned on and off
  // at runtime with a menu option in this sample.
  m_drawingSurface->setOption( TSLOptionTileBufferedLayers, true );
  m_drawingSurface->setOption( TSLOptionProgressiveTileZoom, true );

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
  m_drawingSurface->setDataLayerProps( m_mapLayerName, TSLPropertyLoadedSymbolsAndTextViewExpansion, 20 );

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

void Application::redraw(GLuint fbo)
{
  if( m_drawingSurface )
  {
	// Set the render target for the frame
    m_drawingSurface->targetFrameBuffer(fbo);

    // Draw the map to the widget
    m_drawingSurface->drawDU( 0, 0, m_widgetWidth, m_widgetHeight, true );

    // Render any echo rectangle that may be active.
    if ( m_modeManager )
    {
      m_modeManager->onDraw( 0, 0, m_widgetWidth, m_widgetHeight );
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

void Application::drawingInfo( QOpenGLContext* context) {
	//m_display = display;
	//m_screen = screen;
	m_context = context;
}
