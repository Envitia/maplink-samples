/****************************************************************************
                Copyright (c) 2008-2017 by Envitia Group PLC.
****************************************************************************/

#ifndef WIN32
# include <QX11Info>
#endif
#include <QEvent>
#include <QMouseEvent>

#include "maplinkwidget.h"
#include "application.h"

#include <iostream>
using namespace std;

// These are sometimes #defined by X headers to different values than the ones used by Qt's QEvent class
#ifdef KeyPress
# undef KeyPress
#endif
#ifdef KeyRelease
# undef KeyRelease
#endif

MapLinkWidget::MapLinkWidget( QWidget * parent, const QGLWidget * shareWidget, Qt::WindowFlags f)
: QGLWidget(parent, shareWidget, f)
, m_application(NULL)
{
  // These may not be necessary for the OpenGL QGLWidget.
  // Try commenting out on each platform required - reports are inconsistent as
  // to the effect of not calling any of the following.
  setAttribute(Qt::WA_NoBackground, true);
  setAttribute(Qt::WA_NoSystemBackground, true);
  setAttribute(Qt::WA_PaintOnScreen, true);
  setAttribute(Qt::WA_OpaquePaintEvent);
  setAttribute(Qt::WA_InputMethodEnabled, true);

  // Turn on/off Qt buffer swapping.
  //
  // The MapLink Pro Accelerator Surface can do the swapping if necessary.
  //
  // For this demo we are turning on the Qt swapping and turning off the
  // MapLink Pro swapping. This is controlled by the macro ML_QT_BUFFER_SWAP
  setAutoBufferSwap( ML_QT_BUFFER_SWAP );

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

  // Background clear colour - should match the map background colour
  // see Application::setMapBackgroundColour
  qtWhite = QColor::fromRgb(255, 255, 255);

  // Allows the MapLink redraw callback to trigger a re-draw in a
  // thread safe manner.
  //
  // Qt::AutoConnection - If in the same thread we will be called directly.
  // If in different threads the request will be queued (thread safe).
  //
  // Ref: http://doc.qt.nokia.com/4.6/qt.html#ConnectionType-enum
  connect(this, SIGNAL(changed()), this, SLOT(updateGL()), Qt::AutoConnection);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Map to load.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::loadMap(const QString &filename)
{
  m_filename = filename; // store the filename - the application may not be ready.
  if (m_application)
  {
    m_application->loadMap(filename.toUtf8()); // load the map.
    m_application->reset(); // display full map.
    updateGL(); // cause a redraw - new map!
  }
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

  TSLButtonType button = TSLButtonNone ;
  Qt::MouseButton buttonQt = event->button();
  if ( buttonQt & Qt::LeftButton )
  {
     button = TSLButtonLeft ;
  }
  else if ( buttonQt & Qt::MidButton )
  {
    button = TSLButtonCentre ;
  }
  else if ( buttonQt & Qt::RightButton )
  {
    button = TSLButtonRight ;
  }

  if (m_application->OnMouseMove(button, shiftPressed, controlPressed, event->x(),  event->y()))
    updateGL(); // update the display if a change occured
}

void MapLinkWidget::mousePressEvent( QMouseEvent *event )
{
  //qWarning("mousePress");
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
    updateGL(); // update the display if a change occured
}

void MapLinkWidget::mouseReleaseEvent( QMouseEvent *event )
{
  //qWarning("mouseRelease");
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
    updateGL(); // update the display if a change occured
}

void MapLinkWidget::wheelEvent( QWheelEvent *event )
{
  Qt::KeyboardModifiers modifiers = event->modifiers();
  bool shiftPressed = ( modifiers & Qt::ShiftModifier ) ;
  bool controlPressed = ( modifiers & Qt::ControlModifier ) ;
  int x = event->x() ;
  int y = event->y() ;
  if (m_application->OnMouseWheel(shiftPressed, controlPressed, event->delta(), x, y))
    updateGL(); // update the display if a change occured
}

void MapLinkWidget::keyPressEvent( QKeyEvent *event )
{
  //qWarning("keypress");

  Qt::KeyboardModifiers modifiers = event->modifiers();
  bool shiftPressed = ( modifiers & Qt::ShiftModifier ) ;
  bool controlPressed = ( modifiers & Qt::ControlModifier ) ;
  if (m_application->OnKeyPress(shiftPressed, controlPressed, event->key()))
    updateGL(); // update the display if a change occured
}

void MapLinkWidget::keyReleaseEvent( QKeyEvent * )
{
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Qt calls this to initalise OpenGL
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::initializeGL()
{
  // Default clear colour.
  qglClearColor(qtWhite);

  // Initialise my application..
  if (m_application == NULL)
    create();
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
  // Clear the OpenGL colour buffer - if MapLink is not doing so.
  // See the use of the macro ML_QT_CLEAR_OGL.
  if (ML_QT_CLEAR_OGL)
    glClear(GL_COLOR_BUFFER_BIT);

  // OnDraw causes the MapLink Drawing Surface to draw.
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
    return ; // Already created.

  try
  {
    m_application = new Application(this, m_statusLabel);

    // Pass the map in.
    m_application->loadMap(m_filename.toUtf8());

    makeCurrent(); // need to call this so that we can query the under-lying OpenGL context

    // Platform Specific Setup.
#ifdef Q_WS_X11
    // Extract the X11 information.
    QX11Info x11info = this->x11Info();
    Display *display = x11info.display();
    int screenNum = x11info.screen();
    Visual *visual = (Visual *)x11info.visual();
    Qt::HANDLE colourmap = x11info.colormap();
    Qt::HANDLE drawable = handle();
    Screen *screen = ScreenOfDisplay(display, screenNum);

    // pass to the application as we will need for the Drawing Surface
    m_application->drawingInfo(drawable, display, screen, colourmap, visual);
#else
    // Attaching to the window is much more efficent.
    WId hWnd = winId();

    // pass to the application as we will need for the Drawing Surface
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
  else if (event->type() == QEvent::KeyPress)
  {
    QKeyEvent *ke = static_cast<QKeyEvent *>(event);
    keyPressEvent( ke );
    return false;
  }
  else if (event->type() == QEvent::KeyRelease)
  {
    QKeyEvent *ke = static_cast<QKeyEvent *>(event);
    keyReleaseEvent( ke );
    return false;
  }
  else if (event->type() == QEvent::MouseButtonPress)
  {
    QMouseEvent *ke = static_cast<QMouseEvent *>(event);
    mousePressEvent( ke );
    return false;
  }
  else if (event->type() == QEvent::MouseButtonRelease)
  {
    QMouseEvent *ke = static_cast<QMouseEvent *>(event);
    mouseReleaseEvent( ke );
    return false;
  }
  else if (event->type() == QEvent::MouseMove)
  {
    QMouseEvent *ke = static_cast<QMouseEvent *>(event);
    mouseMoveEvent( ke );
    return false;
  }
  else if (event->type() == QEvent::Wheel)
  {
    QWheelEvent *ke = static_cast<QWheelEvent *>(event);
    wheelEvent( ke );
    return false;
  }
  else if (event->type() == QEvent::Resize)
  {
    QResizeEvent *re = static_cast<QResizeEvent *>(event);

    setGeometry(0, 0, re->size().width(), re->size().height());
    return true;
  }
  else if (event->type() == QEvent::Close)
  {
    QCloseEvent *ce = static_cast<QCloseEvent *>(event);
    closeEvent( ce );
  }
  return false;
}

///////////////////////////////////////////
void MapLinkWidget::reset()
{
  if (m_application)
    m_application->reset();
}
void MapLinkWidget::zoomInOnce()
{
  if (m_application)
  {
    if (m_application->OnToolsZoomin())
      update();
  }
}

void MapLinkWidget::zoomOutOnce()
{
  if (m_application)
  {
    if (m_application->OnToolsZoomout())
      update();
  }
}

void MapLinkWidget::pan()
{
  if (m_application)
    m_application->OnToolsPan();
}

void MapLinkWidget::grab()
{
  if (m_application)
    m_application->OnToolsGrab();
}

void MapLinkWidget::zoom()
{
  if (m_application)
    m_application->OnToolsZoom();
}

void MapLinkWidget::previousView()
{
  if (m_application)
  {
    if (m_application->OnViewstackPreviousview())
      update();
  }
}

void MapLinkWidget::nextView()
{
  if (m_application)
  {
    if (m_application->OnViewstackNextview())
      update();
  }
}

void MapLinkWidget::viewOne()
{
  if (m_application)
  {
    if (m_application->OnSavedviewsGoto1())
      update();
  }
}

void MapLinkWidget::viewTwo()
{
  if (m_application)
  {
    if (m_application->OnSavedviewsGoto2())
     update();
  }
}

void MapLinkWidget::viewThree()
{
  if (m_application)
  {
    if (m_application->OnSavedviewsGoto3())
      update();
  }
}

void MapLinkWidget::resetViews()
{
  if (m_application)
  {
    m_application->OnSavedviewsReset();
  }
}

bool MapLinkWidget::setViewOne()
{
  if (m_application)
  {
    return m_application->OnSavedviewsSet1();
  }
  return false;
}

bool MapLinkWidget::setViewTwo()
{
  if (m_application)
  {
    return m_application->OnSavedviewsSet2();
  }
  return false;
}

bool MapLinkWidget::setViewThree()
{
  if (m_application)
  {
    return m_application->OnSavedviewsSet3();
  }
  return false;
}

void MapLinkWidget::statusLabel(QLabel *label)
{
  m_statusLabel = label;
  if (m_application)
    m_application->statusLabel(label);
}
