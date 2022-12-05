/****************************************************************************
                Copyright (c) 2008-2017 by Envitia Group PLC.
****************************************************************************/

#include "maplinkwidget.h"
#include "application.h"
#ifndef WIN32
# include <QX11Info>
#endif

#include <iostream>
using namespace std;


MapLinkWidget::MapLinkWidget( const QGLFormat &format, QWidget * parent, const QGLWidget * shareWidget, Qt::WindowFlags f )
: QGLWidget(format, parent, shareWidget, f)
, m_application(NULL)
{
  // These may not be necessary for the OpenGL QGLWidget.
  setAttribute(Qt::WA_NoBackground, true);
  setAttribute(Qt::WA_NoSystemBackground, true);
  setAttribute(Qt::WA_PaintOnScreen, true);
  setAttribute(Qt::WA_OpaquePaintEvent);

  // Turn on/off Qt buffer swapping.
  //
  // The MapLink Pro Accelerator Surface can do the swapping if necessary.
  //
  // For this demo we are turning on the Qt swapping and turning off the
  // MapLink Pro swapping. This is controlled by the macro ML_QT_BUFFER_SWAP
  setAutoBufferSwap( ML_QT_BUFFER_SWAP );

  //
  setMouseTracking( true );
  // set the focus policy so we can get keyboard events
  setFocusPolicy(Qt::WheelFocus);

  // Background clear colour - should match the map background colour
  // see Application::setMapBackgroundColour
  qtWhite = QColor::fromRgb(255, 255, 255);

  // Allows the MapLink redraw callback to trigger a re-draw in a
  // thread safe manner.
  connect(this, SIGNAL(changed()), this, SLOT(updateGL()));
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Exe closing... Cleaup
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool MapLinkWidget::close( )
{
  bool result = QWidget::close();
  delete m_application;
  m_application = NULL;
  return result;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Mouse & Keyboard handling
//
// We only update the display if MapLink tells us too.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::mouseMoveEvent( QMouseEvent *event )
{
  Qt::KeyboardModifiers modifiers = event->modifiers();
  bool shiftPressed = ( modifiers & Qt::ShiftModifier ) ;
  bool controlPressed = ( modifiers & Qt::ControlModifier ) ;

  if (m_application->OnMouseMove(event->button(), shiftPressed, controlPressed, event->x(),  event->y()))
    updateGL();
}

void MapLinkWidget::mousePressEvent( QMouseEvent *event )
{
  Qt::KeyboardModifiers modifiers = event->modifiers();
  bool shiftPressed = ( modifiers & Qt::ShiftModifier ) ;
  bool controlPressed = ( modifiers & Qt::ControlModifier ) ;
  int x = event->x() ;
  int y = event->y() ;
  Qt::MouseButton button = event->button();

  bool redraw = false;
  if ( button & Qt::LeftButton )
  {
    redraw = m_application->OnLButtonDown(shiftPressed, controlPressed, x, y);
  }
  else if ( button & Qt::MidButton )
  {
    redraw = m_application->OnMButtonDown(shiftPressed, controlPressed, x, y);
  }
  else if ( button & Qt::RightButton )
  {
    redraw = m_application->OnRButtonDown(shiftPressed, controlPressed, x, y);
  }
  if (redraw)
    updateGL();
}

void MapLinkWidget::mouseReleaseEvent( QMouseEvent *event )
{
  Qt::KeyboardModifiers modifiers = event->modifiers();
  bool shiftPressed = ( modifiers & Qt::ShiftModifier ) ;
  bool controlPressed = ( modifiers & Qt::ControlModifier ) ;
  int x = event->x() ;
  int y = event->y() ;
  Qt::MouseButton button = event->button();

  bool redraw = false;
  if ( button & Qt::LeftButton )
  {
    redraw = m_application->OnLButtonUp(shiftPressed, controlPressed, x, y);
  }
  else if ( button & Qt::MidButton )
  {
    redraw = m_application->OnMButtonUp(shiftPressed, controlPressed, x, y);
  }
  else if ( button & Qt::RightButton )
  {
    redraw = m_application->OnRButtonUp(shiftPressed, controlPressed, x, y);
  }
  if (redraw)
    updateGL();
}

void MapLinkWidget::wheelEvent( QWheelEvent *event )
{
  Qt::KeyboardModifiers modifiers = event->modifiers();
  bool shiftPressed = ( modifiers & Qt::ShiftModifier ) ;
  bool controlPressed = ( modifiers & Qt::ControlModifier ) ;
  int x = event->x() ;
  int y = event->y() ;
  if (m_application->OnMouseWheel(shiftPressed, controlPressed, event->delta(), x, y))
    updateGL();
}

void MapLinkWidget::keyPressEvent( QKeyEvent *event )
{
  Qt::KeyboardModifiers modifiers = event->modifiers();
  bool shiftPressed = ( modifiers & Qt::ShiftModifier ) ;
  bool controlPressed = ( modifiers & Qt::ControlModifier ) ;
  if (m_application->OnKeyPress(shiftPressed, controlPressed, event->key()))
    updateGL();
}

void MapLinkWidget::keyReleaseEvent( QKeyEvent * )
{
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Qt calls this to initalise OpenGL
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::initializeGL()
{
  qglClearColor(qtWhite);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// resize - informs the drawing surface of any change in size of the window
// Relys on the ResizeAction to maintain the view of the map sensibly
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::resizeGL(int w, int h)
{
  if (m_application)
  {
    m_application->OnSize(w, h);
    m_application->OnInitialUpdate();
  }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Qt calls this to draw OpenGL
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::paintGL()
{
  // Initialise my application..
  if (m_application == NULL)
    create();

  // Clear the OpenGL colour buffer - if MapLink is not doing so.
  // See the use of the macro ML_QT_CLEAR_OGL.
  if (ML_QT_CLEAR_OGL)
    glClear(GL_COLOR_BUFFER_BIT);

  m_application->OnDraw( );

  // Either MapLink or Qt needs to swap the OpenGL MapLink.
  // See the use of ML_QT_BUFFER_SWAP.
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Called on first draw to create the Application class instance
// and to pass the platform specific needed by MapLink Accelerator
// SDK Drawing Surface.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void MapLinkWidget::create()
{
  if (m_application)
    return ;

  try
  {
    m_application = new Application(this);

    // Pass the map in.
    m_application->loadMap(m_filename.toUtf8());

    // Platform Specific Setup.
#ifdef Q_WS_X11
    QX11Info x11info = this->x11Info();
    Display *display = x11info.display(); 
    int screenNum = x11info.screen(); 
    Visual *visual = (Visual *)x11info.visual(); 
    Qt::HANDLE colourmap = x11info.colormap(); 
    Qt::HANDLE drawable = handle();
    Screen *screen = ScreenOfDisplay(display, screenNum);

    makeCurrent(); // need to call this so that we can query the under-lying OpenGL context
    // pass to the application as we will need for the Drawing Surface
    m_application->drawingInfo(drawable, display, screen, colourmap, visual);
#else
    // Attaching to the window is much more efficent.
    WId hWnd = winId();

    makeCurrent(); // need to call this so that we can query the under-lying OpenGL context
    m_application->drawingInfo(hWnd);
#endif

    // Set initial size of the area to draw to.
    m_application->OnSize(width(), height());

    // Initial size and creation of the drawing surface. This will 
    // exit immediatly if all has been setup correctly.
    m_application->OnInitialUpdate();
  }
  catch (...)
  {
    qWarning("Caught exception");
  }
}


