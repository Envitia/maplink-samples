/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QGuiApplication>
#include "maplinkwidget.h"
#include "application.h"

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
# include <qpa/qplatformnativeinterface.h>
# ifdef WIN32
#  include <qwindow.h>

static QWindow* windowForWidget(const QWidget* widget) 
{
    QWindow* window = widget->windowHandle();
    if (window)
        return window;
    const QWidget* nativeParent = widget->nativeParentWidget();
    if (nativeParent) 
        return nativeParent->windowHandle();
    return NULL; 
}

WId getHWNDForWidget(const QWidget* widget)
{
    QWindow* window = ::windowForWidget(widget);
    if (window && window->handle())
    {
        QPlatformNativeInterface* iface = QGuiApplication::platformNativeInterface();
        return (WId)(iface->nativeResourceForWindow(QByteArrayLiteral("handle"), window));
    }
    return 0; 
}
# endif
#else
# ifdef WIN32
WId getHWNDForWidget(const QWidget* widget)
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

MapLinkWidget::MapLinkWidget( QWidget * parent, const QGLWidget * shareWidget, Qt::WindowFlags f )
  : QGLWidget(parent, shareWidget, f)
  , m_application( new Application( parent ) )
{
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

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Map to load.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::loadMap( const char *filename)
{
  if( m_application )
  {
    m_application->loadMap( filename ); // Load the map
    m_application->resetView(); // Reset the view to show the entire extent of the new map
    update(); // Cause a redraw so the new map can be seen
  }
}

void MapLinkWidget::loadKML(const char* filename, AttributeTreeWidget* attributeTree)
{
  if( m_application )
  {
    m_application->loadKML( filename, attributeTree );
    update();
  }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Mouse & Keyboard handling
//
// We only update the display if MapLink tells us too.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Qt calls this to initalise OpenGL
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::initializeGL()
{
  if( !m_application )
  {
    m_application = new Application( parentWidget() );
  }

  // Platform Specific Setup.
#ifdef X11_BUILD
# if QT_VERSION >= 0x50100
  // Extract the X11 information - QX11Info was removed in Qt5
  QPlatformNativeInterface *native = QGuiApplication::platformNativeInterface();
  Display *display = static_cast<Display*>( native->nativeResourceForWindow("display", NULL) );
  Screen *screen = DefaultScreenOfDisplay(display);
  m_application->drawingInfo(display, screen);
#else
  // Qt 5.1 introduced a different version of QX11Info for accessing widget native handles
  int screenNum = DefaultScreen( QX11Info::display() ); 
  Screen *screen = ScreenOfDisplay( QX11Info::display(), screenNum );

  // pass to the application as we will need for the Drawing Surface
  m_application->drawingInfo( QX11Info::display(), screen );
#endif
#else
  // The MapLink OpenGL drawing surface needs to know the window handle to attach to - 
  // query this from Qt
  WId hWnd = getHWNDForWidget(this);

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
void MapLinkWidget::resizeGL(int w, int h)
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

  // Either MapLink or Qt needs to swap the OpenGL MapLink.
  // See the use of ML_QT_BUFFER_SWAP.
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Event filter - We require this when not directly attached to a
// MainWindow so that we can receive Keyboard, Mouse and Resize
// events.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
