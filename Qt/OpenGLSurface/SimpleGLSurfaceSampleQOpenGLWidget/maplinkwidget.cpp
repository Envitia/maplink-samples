/****************************************************************************
                Copyright (c) 2013-2017 by Envitia Group PLC.
****************************************************************************/

#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>

#if (QT_VERSION >= 0x50000)
#include <QGuiApplication>
#endif
#include <QMessageBox>

#include <QGLWidget>

#include "MapLink.h"
#include "MapLinkOpenGLSurface.h"

#include "application.h"
#include "maplinkwidget.h"

#include <iostream>
using namespace std;

// These are defined by X11 which interfere with the Qt definitions
#ifdef KeyPress
#  undef KeyPress
#endif
#ifdef KeyRelease
# undef KeyRelease
#endif

MapLinkWidget::MapLinkWidget( QWidget * parent, Qt::WindowFlags f )
  : QOpenGLWidget(parent, f)
  , m_application( new Application( parent ) )
{
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

  // Set the mouse tracking in the designer.
  setMouseTracking( true );
}

MapLinkWidget::~MapLinkWidget()
{
  // Clean up
  delete m_application;
}

// Map to load.
void MapLinkWidget::loadFile( const char *filename)
{
  if( m_application )
  {
    m_application->loadFile( filename ); // Load the map/file
    m_application->resetView(); // Reset the view to show the entire extent of the new map/file
    update(); // Cause a redraw so the new map/file can be seen
  }
}

// Mouse, Keyboard and event handling
//
// We only update the display if MapLink tells us too.
void MapLinkWidget::mouseMoveEvent( QMouseEvent *event )
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
  if( m_application->mouseMoveEvent(button, shiftPressed, controlPressed, event->x(),  event->y()) )
  {
    update(); // We were asked to redraw the display
  }
}

void MapLinkWidget::mousePressEvent( QMouseEvent *event )
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
     redraw = m_application->onLButtonDown( shiftPressed, controlPressed, x, y );
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

void MapLinkWidget::mouseReleaseEvent( QMouseEvent *event )
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

void MapLinkWidget::wheelEvent( QWheelEvent *event )
{
  Qt::KeyboardModifiers modifiers = event->modifiers();
  bool shiftPressed = modifiers & Qt::ShiftModifier;
  bool controlPressed = modifiers & Qt::ControlModifier;
  int x = event->x() ;
  int y = event->y() ;

  // Forward the event onto the application
  if( m_application->onMouseWheel(shiftPressed, controlPressed, event->delta(), x, y) )
  {
    update(); // We were asked to redraw the display
  }
}

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

void MapLinkWidget::redrawRequired (const TSLEnvelope&, unsigned int)
{
  // Trigger a redraw
  update();
}

// Qt calls this to initalise OpenGL
void MapLinkWidget::initializeGL()
{
  if( !m_application )
  {
    m_application = new Application( parentWidget() );
  }

  // OpenGL Initialisation
  // - The widget's context is current, can setup resources and state here
  // - But the widget's framebuffer is not valid yet - No draw calls can be made until paintGL
  // - The context will remain valid unless the widget is moved to a new parent
  m_application->drawingInfo( context() );

  // Tell the application to create the drawing surface and attach it to this widget
  // If the surface is already attached this will be a no-op
  m_application->create( this );
}

// resize - informs the drawing surface of any change in size of the window
// Relys on the ResizeAction to maintain the view of the map sensibly
void MapLinkWidget::resizeGL(int w, int h)
{
  // The widget has been resized, and the MapLink Surface/OpenGL resources must be updated to match
  // - The widget's context is current
  // - The widget's framebuffer is bound
  
  // When a resize is performed the widget's framebuffer will be recreated
  // It is important that MapLink is updated to reference the new buffer.
  // This may be done here, or before rendering in paintGL
  m_application->resize( w, h );
}

// Qt calls this to draw OpenGL
void MapLinkWidget::paintGL()
{
  // A request from Qt to render a frame
  // - The widget's context is current
  // - The widget's framebuffer is bound
  // - glViewport has been called
  // - No other state has been changed, no buffers have been cleared
 
  // Tell MapLink to render a frame to the widget's framebuffer
  // (In the older QGLWidget this was not necesarry as MapLink rendered
  //  directly to the windowing system. With the QOpenGLWidget we are
  //  rendering to an object within Qt's scene graph, which will then
  //  be presented to the windowing system after Qt's composition tasks)
  m_application->redraw(defaultFramebufferObject());
}

// Event filter - We require this when not directly attached to a
// MainWindow so that we can receive Keyboard, Mouse and Resize
// events.
bool MapLinkWidget::eventFilter(QObject *obj, QEvent *event)
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

///////////////////////////////////////////
// Events forwarded from the main window. These are all forwarded on to
// the appliction, and we will issue a redraw request if the application asks
// us to.
void MapLinkWidget::resetView()
{
  m_application->resetView();
  update(); // The viewing extent has changed, so a redraw is always required
}

void MapLinkWidget::zoomInOnce()
{
  if( m_application->zoomIn() )
  {
    // We were asked to redraw the display
    update();
  }
}

void MapLinkWidget::zoomOutOnce()
{
  if( m_application->zoomOut() )
  {
    // We were asked to redraw the display
    update();
  }
}

void MapLinkWidget::activatePanMode()
{
  // Tell the application to activate the pan interaction mode
  m_application->activatePanMode();
}

void MapLinkWidget::activateGrabMode()
{
  // Tell the application to activate the grab interaction mode
  m_application->activateGrabMode();
}

void MapLinkWidget::activateZoomMode()
{
  // Tell the application to activate the zoom interaction mode
  m_application->activateZoomMode();
}

void MapLinkWidget::enableBufferedLayerTiling( bool enable )
{
  // Tell the application to enable/disable buffered layer tiling
  m_application->enableBufferedLayerTiling( enable );
}
