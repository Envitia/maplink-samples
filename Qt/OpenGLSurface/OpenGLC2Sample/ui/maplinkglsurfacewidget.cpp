/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QGuiApplication>
#include <QMessageBox>
#ifndef WIN32
#include <QX11Info>
#endif

#include "maplinkglsurfacewidget.h"

#include <iostream>
using namespace std;

#include "MapLink.h"
#include "MapLinkOpenGLSurface.h"
#include "MapLinkIMode.h"

#undef KeyPress
#undef KeyRelease

#include "layers/layermanager.h"
#include "tracks/trackmanager.h"
#include "trackselectionmode.h"

// Interaction mode IDs - these can be any numbers
#define ZOOM_INTERACTION_MODE                   1
#define PAN_INTERACTION_MODE                    2
#define GRAB_INTERACTION_MODE                   3
#define TRACKSELECT_INTERACTION_MODE            4

MapLinkGLSurfaceWidget::MapLinkGLSurfaceWidget( QWidget * parent, const QGLWidget * shareWidget, Qt::WindowFlags f )
  : QGLWidget(parent, shareWidget, f)
  , m_drawingSurface(NULL)
  , m_modeManager(NULL)
  , m_widgetWidth(0)
  , m_widgetHeight(0)
  , m_resetOnResize(false)
{
  // Turn off Qt colour buffer clearing.
  // The MapLink Pro OpenGL drawing surface will normally clear the colour buffer.
  // When a map with a background colour is loaded this will clear the colour buffer to the map's
  // background colour automatically.
  setAutoFillBackground( false );

  if (parent)
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

  // Strong Focus and Wheel Mouse.
  setFocusPolicy(Qt::WheelFocus);

  // Enable mouse tracking
  setMouseTracking( true );
}

MapLinkGLSurfaceWidget::~MapLinkGLSurfaceWidget()
{
  // Clean up
  LayerManager::instance().detachLayersFromSurface( m_drawingSurface );

  delete m_drawingSurface;
  delete m_modeManager;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Qt calls this to initalise the widget
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkGLSurfaceWidget::initializeGL()
{
  // Platform-specific drawing surface creation
  TSLOpenGLSurfaceCreationParameters creationOptions;
  creationOptions.swapBuffersManually( true ); // Qt will automatically handle buffer swaps, so tell the drawing surface not to
  creationOptions.useVSync( false ); // Ensure vsync is disabled or application rendering speeds will be capped at the monitor's refresh rate

  // Create a MapLink OpenGL drawing surface using the context for the Qt widget
#ifdef WIN32
  HGLRC context = wglGetCurrentContext();
  m_drawingSurface = new TSLWGLSurface( (HWND)winId(), false, context, creationOptions );
#elif QT_VERSION >= 0x50100
  Display *display = QX11Info::display();
  WId wid = winId();
  
  XWindowAttributes attribs;
  XGetWindowAttributes( display, wid, &attribs );

  GLXContext context = glXGetCurrentContext();
  GLXDrawable drawable = glXGetCurrentDrawable();

  m_drawingSurface = new TSLGLXSurface( display, attribs.screen, drawable, context, creationOptions );
#else
# error "This sample currently doesn't build with < Qt5.1"
#endif

  if( !m_drawingSurface->context() )
  {
    // The drawing surface failed to attach to the context - show the error from the error stack.
    // This means the drawing surface cannot be used, so exit the sample
    const char *msg = TSLErrorStack::errorString();
    if ( msg )
    {
      QMessageBox::critical( parentWidget(), "Failed to attach drawing surface", msg );
    }
    else
    {
      QMessageBox::critical( parentWidget(), "Failed to attach drawing surface", "Unknown error" );
    }
    parentWidget()->close();
    return;
  }

  // Enable dynamic arc map support.
  m_drawingSurface->setOption( TSLOptionDynamicArcSupportEnabled, true );

  // Enable tiling of buffered layers by default for best performance. This can be turned on and off
  // at runtime with a menu option in this sample.
  m_drawingSurface->setOption( TSLOptionTileBufferedLayers, true );
  m_drawingSurface->setOption( TSLOptionProgressiveTileZoom, true );

  TSLDrawingSurfaceTiledBufferControl *controller = m_drawingSurface->getTiledBufferController();
  if( controller )
  {
    // Tell the surface to preload a border of two tiles around the current view to reduce the likelyhood
    // of tiles on the edge of the view not being ready during panning.
    controller->drawExtentExpansion( 2 );
  }

  // Tell the drawing surface which callback it should invoke when the new tiles for buffered
  // layers are ready for drawing.
  m_drawingSurface->setRedrawCallback( this );

  // Now create and initialse the mode manager and modes
  m_modeManager = new TSLInteractionModeManagerGeneric( this, m_drawingSurface );

  // Add the three standard interaction mode types to the manager - the grab mode is the default
  m_modeManager->addMode( new TSLInteractionModeZoom( ZOOM_INTERACTION_MODE ), false ) ;
  m_modeManager->addMode( new TSLInteractionModePan( PAN_INTERACTION_MODE ), false ) ;
  m_modeManager->addMode( new TSLInteractionModeGrab( GRAB_INTERACTION_MODE ), true ) ;
  m_modeManager->addMode( new TrackSelectionMode( TRACKSELECT_INTERACTION_MODE ), false ) ;

  m_modeManager->setCurrentMode( GRAB_INTERACTION_MODE );

  // Display any errors that have occurred
  const char *errorMsg = TSLErrorStack::errorString();
  if( errorMsg )
  {
    QMessageBox::warning( parentWidget(), "Cannot initialise view", errorMsg );
    TSLErrorStack::clear();
  }

  // Now the drawing surface is set up, add the data layers from the layer manager to it so they
  // will be visible
  LayerManager::instance().attachLayersToSurface( m_drawingSurface );

}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Resize - informs the drawing surface of any change in size of the widget
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkGLSurfaceWidget::resizeGL(int width, int height)
{
  QGLWidget::resize( width, height );

  if( m_drawingSurface )
  {
    // Inform the drawing surface of the new window size,
    // attempting to keep the top left corner the same.
    // Do not ask for an automatic redraw since we will get a call to paintGL() to do so
    m_drawingSurface->wndResize( 0, 0, width, height, false, TSLResizeActionMaintainTopLeft ) ;

    if( m_resetOnResize )
    {
      m_drawingSurface->reset(false);
    }
  }
  if( m_modeManager )
  {
    m_modeManager->onSize( width, height ) ;
  }

  m_widgetWidth = width;
  m_widgetHeight = height;

  update();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Qt calls this to draw the widget
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkGLSurfaceWidget::paintGL()
{
  if( m_drawingSurface )
  {
    TrackManager::instance().preDraw( m_drawingSurface );

    // Draw to the widget
    m_drawingSurface->drawDU( 0, 0, m_widgetWidth, m_widgetHeight, true );

    // Don't forget to draw any echo rectangle that may be active.
    if ( m_modeManager )
    {
      m_modeManager->onDraw( 0, 0, m_widgetWidth, m_widgetHeight );
    }

    TrackManager::instance().postDraw( m_drawingSurface );

    m_resetOnResize = false;
  }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Event filter - We require this when not directly attached to a
// MainWindow so that we can receive Keyboard, Mouse and Resize
// events.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool MapLinkGLSurfaceWidget::eventFilter(QObject *obj, QEvent *event)
{
  if (obj != parent())
  {
    return false;
  }

  switch( event->type() )
  {
  case QEvent::KeyPress:
    {
      QKeyEvent *ke = static_cast<QKeyEvent *>(event);
      keyPressEvent( ke );
      return false;
    }
  case QEvent::KeyRelease:
    {
      QKeyEvent *ke = static_cast<QKeyEvent *>(event);
      keyReleaseEvent( ke );
      return false;
    }
  case QEvent::MouseButtonPress:
    {
      QMouseEvent *ke = static_cast<QMouseEvent *>(event);
      mousePressEvent( ke );
      return false;
    }
  case QEvent::MouseButtonRelease:
    {
      QMouseEvent *ke = static_cast<QMouseEvent *>(event);
      mouseReleaseEvent( ke );
      return false;
    }
  case QEvent::MouseMove:
    {
      QMouseEvent *ke = static_cast<QMouseEvent *>(event);
      mouseMoveEvent( ke );
      return false;
    }
  case QEvent::Wheel:
    {
      QWheelEvent *ke = static_cast<QWheelEvent *>(event);
      wheelEvent( ke );
      return false;
    }
  case QEvent::Resize:
    {
      QResizeEvent *re = static_cast<QResizeEvent *>(event);

      setGeometry(0, 0, re->size().width(), re->size().height());
      return true;
    }
  case QEvent::Close:
    {
      QCloseEvent *ce = static_cast<QCloseEvent *>(event);
      closeEvent( ce );
    }
  default:
    return false;
  }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Redraw callback - this is called by MapLink when the widget needs to be
// redrawn
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkGLSurfaceWidget::redrawRequired (const TSLEnvelope&, unsigned int)
{
  // Ask Qt to perform a redraw
  update();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Mouse event handling - these forward the event to the interaction
// manager in order to update the display
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkGLSurfaceWidget::mouseMoveEvent( QMouseEvent *event )
{
  Qt::KeyboardModifiers modifiers = event->modifiers();
  bool shiftPressed = modifiers & Qt::ShiftModifier;
  bool controlPressed = modifiers & Qt::ControlModifier;

  // The MapLink application class doesn't use Qt - convert the mouse button types
  // to the MapLink types
  TSLButtonType button = TSLButtonNone ;
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
  if( m_modeManager->onMouseMove( button, event->x(), event->y(), shiftPressed, controlPressed ) )
  {
    update(); // We were asked to redraw the display
  }
}

void MapLinkGLSurfaceWidget::mousePressEvent( QMouseEvent *event )
{
  Qt::KeyboardModifiers modifiers = event->modifiers();
  bool shiftPressed = modifiers & Qt::ShiftModifier;
  bool controlPressed = modifiers & Qt::ControlModifier;
  int x = event->x() ;
  int y = event->y() ;
  Qt::MouseButton button = event->button();

  // Forward the event onto the application
  bool redraw = false;
  switch( button )
  {
  case Qt::LeftButton:
    redraw = m_modeManager->onLButtonDown( x, y, shiftPressed, controlPressed );
    break;

  case Qt::MidButton:
    redraw = m_modeManager->onMButtonDown( x, y, shiftPressed, controlPressed );
    break;

  case Qt::RightButton:
    redraw = m_modeManager->onRButtonDown( x, y, shiftPressed, controlPressed );
    break;

  default:
    break;
  }

  if( redraw )
  {
    update(); // We were asked to redraw the display
  }
}

void MapLinkGLSurfaceWidget::mouseReleaseEvent( QMouseEvent *event )
{
  Qt::KeyboardModifiers modifiers = event->modifiers();
  bool shiftPressed = modifiers & Qt::ShiftModifier;
  bool controlPressed = modifiers & Qt::ControlModifier;
  int x = event->x() ;
  int y = event->y() ;
  Qt::MouseButton button = event->button();

  // Forward the event onto the application
  bool redraw = false;
  switch( button )
  {
  case Qt::LeftButton:
    redraw = m_modeManager->onLButtonUp( x, y, shiftPressed, controlPressed );
    break;

  case Qt::MidButton:
    redraw = m_modeManager->onMButtonUp( x, y, shiftPressed, controlPressed );
    break;

  case Qt::RightButton:
    redraw = m_modeManager->onRButtonUp( x, y, shiftPressed, controlPressed );
    break;

  default:
    break;
  }

  if( redraw )
  {
    update(); // We were asked to redraw the display
  }
}

void MapLinkGLSurfaceWidget::wheelEvent( QWheelEvent *event )
{
  // Forward the event onto the application
  if( m_modeManager->onMouseWheel(event->delta(), event->x(), event->y()) )
  {
    update(); // We were asked to redraw the display
  }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Interaction mode events - these are triggered to change
// the current interaction mode or alter the current view
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkGLSurfaceWidget::resetView()
{
  if( m_drawingSurface )
  {
    // Don't auto-redraw, instead use Qt's update mechanism to issue a redraw request
    m_drawingSurface->reset( false );
    update();
  }
}

void MapLinkGLSurfaceWidget::zoomIn()
{
  if( m_modeManager && m_modeManager->zoomIn( 30 ) )
  {
    // Request a redraw if the interaction hander requires it
    update();
  }
}

void MapLinkGLSurfaceWidget::zoomOut()
{
  if( m_modeManager && m_modeManager->zoomOut( 30 ) )
  {
    // Request a redraw if the interaction hander requires it
    update();
  }
}

void MapLinkGLSurfaceWidget::activateZoomMode()
{
  // Activate the zoom interaction mode
  if( m_modeManager )
  {
    m_modeManager->setCurrentMode( ZOOM_INTERACTION_MODE ) ;
  }
}

void MapLinkGLSurfaceWidget::activatePanMode()
{
  // Activate the pan interaction mode
  if( m_modeManager )
  {
    m_modeManager->setCurrentMode( PAN_INTERACTION_MODE ) ;
  }
}

void MapLinkGLSurfaceWidget::activateGrabMode()
{
  // Activate the grab interaction mode
  if( m_modeManager )
  {
    m_modeManager->setCurrentMode( GRAB_INTERACTION_MODE ) ;
  }
}

void MapLinkGLSurfaceWidget::activateTrackSelectMode()
{
  // Activate the track selection interaction mode
  if( m_modeManager )
  {
    m_modeManager->setCurrentMode( TRACKSELECT_INTERACTION_MODE ) ;
  }
}

void MapLinkGLSurfaceWidget::resetMode(TSLInteractionMode* /*mode*/, TSLButtonType /*button*/, TSLDeviceUnits /*xDU*/, TSLDeviceUnits /*yDU*/)
{
  // Do nothing
}

void MapLinkGLSurfaceWidget::viewChanged(TSLDrawingSurface* /*drawingSurface*/)
{
  // Do nothing
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Accessor method for other parts of the application to access the MapLink drawing surface
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
TSLOpenGLSurface* MapLinkGLSurfaceWidget::drawingSurface()
{
  return m_drawingSurface;
}
