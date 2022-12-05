/****************************************************************************
                Copyright (c) 2008-2017 by Envitia Group PLC.
****************************************************************************/

#include "maplinkwidget.h"
#include "application.h"
#ifndef WIN32
# include <QX11Info>
#endif

#include <iostream>
#include <QApplication>
#include <QDesktopWidget>
using namespace std;

MapLinkWidget::MapLinkWidget(QWidget *parent)//, Qt::WFlags flags)
  : QWidget(parent)
  , m_application(NULL)
{
  // This is required for Qt4 to stop the back ground being drawn and Qt
  // Double buffering. You also need to override paintEngine().
  //
  // Ref:
  // http://lists.trolltech.com/qt-interest/2006-02/thread00004-0.html
  //
  setAttribute( Qt::WA_NoBackground, true);
  setAttribute( Qt::WA_NoSystemBackground, true);
  // Possible issue with this for Qt4.1.0 and newer versions.
  //
  // See:
  //   http://www.trolltech.com/developer/task-tracker/index_html?id=106922&method=entry
  //   http://lists.trolltech.com/qt-interest/2006-05/thread00316-0.html
  //
  // Talk to Trolltech support about getting a fix if this proves to be a problem
  //
  // NOTE: I am not seeing this problem, probably because I'm doing things slightly
  //       differently from the example.
  setAttribute(Qt::WA_PaintOnScreen);
  setAttribute(Qt::WA_OpaquePaintEvent);
  setAttribute(Qt::WA_NativeWindow);

  // set the focus policy so we can get keyboard events
  setFocusPolicy(Qt::WheelFocus);
}


bool MapLinkWidget::close( )
{
  bool result = QWidget::close();
  delete m_application;
  m_application = NULL;
  return result;
}

void MapLinkWidget::create()
{
  if (m_application)
    return ;
  m_application = new Application();
#ifndef WIN32
  // The method to access the widget's native X11 resources differs depending on the version of Qt being used.
# if QT_VERSION < 0x50000
  QX11Info x11info = this->x11Info();
  Display *display = x11info.display(); 
  int screenNum = x11info.screen(); 
  Visual *visual = (Visual *)x11info.visual(); 
  Qt::HANDLE colourmap = x11info.colormap(); 
  Qt::HANDLE drawable = handle();
  Screen *screen = ScreenOfDisplay(display, screenNum);
# elif QT_VERSION >= 0x50100
  // Qt 5.1 or later
  Display *display = QX11Info::display();
  QDesktopWidget *desktop = QApplication::desktop();
  int screenNum = desktop->screenNumber( this );
  Screen *screen = ScreenOfDisplay( display, screenNum );
  Visual *visual = DefaultVisual( display, screenNum );
  Colormap colourmap = DefaultColormap( display, screenNum );
  WId drawable = winId();
# else
#  error "This sample does not currently support building with Qt 5.0.
# endif

  // pass to the application as we will need for the Drawing Surface
  m_application->drawingInfo(drawable, display, screen, colourmap, visual);
#else
  // Attaching to the window is much more efficent.
  WId hWnd = winId();
  m_application->drawingInfo(hWnd);
#endif
  m_application->OnSize(width(), height());
  m_application->OnInitialUpdate();
}

void MapLinkWidget::paintEvent ( QPaintEvent *rect )
{
  if (m_application == NULL)
    create();

  // Redraw the MapLink drawing surface
  m_application->redraw();

  // When using the X11/GDI drawing surfaces with Qt5
  // the application can reach a state where the MapLinkWidget
  // has received an update event but the MainWindow has not.
  // If the MapLinkWidget has been double buffered by Qt
  // this means the map update will not take effect until the
  // next event is received by the widget (key/mouse press).
  // Emit a signal to notify any classes that the map display
  // has been redrawn.
  //
  // This must not be done if the map is being rendered continuously
  // using Qt or on a regular interval with a timer.
  emit mapDrawn();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// resize - informs the drawing surface of any change in size of the window
// Relys on the ResizeAction to maintain the view of the map sensibly
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::resizeEvent( QResizeEvent *event )
{
  QWidget::resizeEvent(event);
  if (m_application)
  {
    m_application->OnSize(width(), height());
    m_application->OnInitialUpdate();
  }
}

void MapLinkWidget::mouseMoveEvent( QMouseEvent *event )
{
  Qt::KeyboardModifiers modifiers = event->modifiers();
  bool shiftPressed = ( modifiers & Qt::ShiftModifier ) ;
  bool controlPressed = ( modifiers & Qt::ControlModifier ) ;
  
  m_application->OnMouseMove(event->button(), shiftPressed, controlPressed, event->x(),  event->y());
}

void MapLinkWidget::mousePressEvent( QMouseEvent *event )
{
  Qt::KeyboardModifiers modifiers = event->modifiers();
  bool shiftPressed = ( modifiers & Qt::ShiftModifier ) ;
  bool controlPressed = ( modifiers & Qt::ControlModifier ) ;
  int x = event->x() ;
  int y = event->y() ;
  Qt::MouseButton button = event->button();

  // Forward the event onto the application
  bool redraw = false;
  switch( button )
  {
  case Qt::LeftButton:
     redraw = m_application->OnLButtonDown( shiftPressed, controlPressed, x, y );
     break;

  case Qt::MidButton:
    redraw = m_application->OnMButtonDown( shiftPressed, controlPressed, x, y );
    break;

  case Qt::RightButton:
    redraw = m_application->OnRButtonDown( shiftPressed, controlPressed, x, y );
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
  bool shiftPressed = ( modifiers & Qt::ShiftModifier ) ;
  bool controlPressed = ( modifiers & Qt::ControlModifier ) ;
  int x = event->x() ;
  int y = event->y() ;
  Qt::MouseButton button = event->button();

  // Forward the event onto the application
  bool redraw = false;
  switch( button )
  {
  case Qt::LeftButton:
     redraw = m_application->OnLButtonUp( shiftPressed, controlPressed, x, y );
     break;

  case Qt::MidButton:
    redraw = m_application->OnMButtonUp( shiftPressed, controlPressed, x, y );
    break;

  case Qt::RightButton:
    redraw = m_application->OnRButtonUp( shiftPressed, controlPressed, x, y );
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
  bool shiftPressed = ( modifiers & Qt::ShiftModifier ) ;
  bool controlPressed = ( modifiers & Qt::ControlModifier ) ;
  int x = event->x() ;
  int y = event->y() ;

  // Forward the event onto the application
  if( m_application->OnMouseWheel(shiftPressed, controlPressed, event->delta(), x, y) )
  {
    update(); // We were asked to redraw the display
  }
}

void MapLinkWidget::keyPressEvent( QKeyEvent *event )
{
  Qt::KeyboardModifiers modifiers = event->modifiers();
  bool shiftPressed = ( modifiers & Qt::ShiftModifier ) ;
  bool controlPressed = ( modifiers & Qt::ControlModifier ) ;

  // Forward the event onto the application
  if( m_application->OnKeyPress( shiftPressed, controlPressed, event->key() ) )
  {
    update(); // We were asked to redraw the display
  }
}

void MapLinkWidget::keyReleaseEvent( QKeyEvent * )
{
  // ignore
}


