/****************************************************************************
                Copyright (c) 2008-2017 by Envitia Group PLC.
****************************************************************************/

#include <QtGui>
#include <QMessageBox>
#include "maplinkwidget.h"
#include "application.h"
#ifndef WIN32
# include <QApplication>
# include <QDesktopWidget>
# include <QX11Info>
#endif

#include <iostream>
using namespace std;

// ************************************************************
// The MapLink widget class extends the QT OpenGL widget and provides
// the drawing area for the MapLink drawing surface.
//
// This class bridges the Application class, which contains the code
// dealing with the MapLink API and the QT user interface objects.
// ************************************************************


MapLinkWidget::MapLinkWidget(QWidget *parent)
  : QGLWidget(parent)
  , m_application(NULL)
{
  // Set the focus policy so we can get mouse wheel events
  setFocusPolicy(Qt::WheelFocus);

  // Tell Qt that we will draw all pixels in the widget when we draw. This means Qt
  // does not have to erase the background first.
  setAttribute(Qt::WA_OpaquePaintEvent);
}

MapLinkWidget::~MapLinkWidget()
{
}

bool MapLinkWidget::close( )
{
  // Clean up the widget
  bool result = QGLWidget::close();
  delete m_application;
  m_application = NULL;
  return result;
}

void MapLinkWidget::initializeGL()
{
  // This function is called when the application is started. It provides the necessary information
  // to the Application class to enable it to create a MapLink drawing surface
  if (m_application)
    return ;

  m_application = new Application();
#ifndef WIN32
  // The method to access the widget's native X11 resources differs depending on the version of Qt being used.
# if QT_VERSION < 0x50000
  // Qt 4.x 
  QX11Info x11info = this->x11Info();
  Display *display = x11info.display(); 
  int screenNum = x11info.screen(); 
  Visual *visual = (Visual *)x11info.visual(); 
  Screen *screen = ScreenOfDisplay(display, screenNum);
  Qt::HANDLE colourmap = x11info.colormap();
# elif QT_VERSION >= 0x50100
  // Qt 5.1 or later
  Display *display = QX11Info::display();
  QDesktopWidget *desktop = QApplication::desktop();
  int screenNum = desktop->screenNumber( this );
  Screen *screen = ScreenOfDisplay( display, screenNum );
  Visual *visual = DefaultVisual( display, screenNum );
  Colormap colourmap = DefaultColormap( display, screenNum );
# else
#  error "This sample does not currently support building with Qt 5.0.
# endif

  // Extract the GLXContext used by the QT widget as we want it to be used by the drawing surface in
  // preference to it creating its own.
  GLXContext activeContext = glXGetCurrentContext();
  GLXDrawable activeDrawable = glXGetCurrentDrawable();

  // Pass to the application so it can create the drawing surface
  m_application->drawingInfo( activeDrawable, display, screen, activeContext, colourmap, visual );
#else
  WId hWnd = winId();

  // Extract the OpenGL conext used by the QT widget as we want it to be used by the drawing surface in
  // preference to it creating its own.
  HGLRC activeContext = wglGetCurrentContext();

  // Pass to the application so it can create the drawing surface
  m_application->drawingInfo( activeContext, hWnd );
#endif

  // Initialise the application so it is ready for drawing
  m_application->onSize(width(), height());

  // Pass to the application the function to invoke when the window needs to be redrawn when new imagery
  // is available. The application knows nothing about the Qt widget it is contained in so it is unable
  // to issue repaint events directly.
  m_application->onInitialUpdate( repaintFunc, this );

  // Check for any errors that have occurred, and display them
  const char * msg = TSLErrorStack::errorString() ;
  if ( msg )
  {
    // If we have any errors during initialisation, display the message.
    QMessageBox::information(this, tr("Initialisation Error"),
                               tr( msg ), QMessageBox::Cancel);
  }

}

void MapLinkWidget::paintGL()
{
  if( m_application )
  {
    // A redraw was requested, tell the application to draw to the back buffer.
    // Qt handles swapping the display buffers for us.
    m_application->onDraw();
  }
}

void MapLinkWidget::repaintFunc( void *arg, int )
{
  // This function is invoked by the MapLink drawing surface when new imagery is ready to
  // be drawn. This call happens in a background thread so we cannot call paintGL directly.
  // Instead, tell Qt to create a paint event for the window. This will cause a draw to 
  // occur in the main thread.
  MapLinkWidget *widget = (MapLinkWidget*)arg;
  widget->update();
}

void MapLinkWidget::resizeGL( int width, int height )
{
  if (m_application)
  {
    // The window has been resized, forward this event on to the application so that
    // the MapLink drawing surface knows the correct window size.
    m_application->onSize( width, height );

    // Ensure a redraw of the window occurs at it's new size.
    update();
  }
}

void MapLinkWidget::mouseMoveEvent( QMouseEvent *event )
{
  // The mouse has moved over the window. Convert the Qt event information into
  // the types used by the MapLink 3D interaction modes and forward the event
  // on to the application.

  Qt::KeyboardModifiers modifiers = event->modifiers();
  bool shiftPressed = ( modifiers & Qt::ShiftModifier ) ;
  bool controlPressed = ( modifiers & Qt::ControlModifier ) ;

  TSLButtonType buttonPressed = TSLButtonNone;
  switch( event->buttons() )
  {
    case Qt::LeftButton:
      buttonPressed = TSLButtonLeft;
      break;

    case Qt::RightButton:
      buttonPressed = TSLButtonRight;
      break;

    case Qt::MidButton:
      buttonPressed = TSLButtonCentre;
      break;
 
    default:
      break;
  }
  
  if( m_application->onMouseMove( buttonPressed, shiftPressed, controlPressed, event->pos().x(), event->pos().y() ) )
  {
    // The above function returns true if the active interaction mode requires the window to be redrawn.
    // If this happened, tell Qt to issue a repaint event. 
    update();
  }
}

void MapLinkWidget::mousePressEvent( QMouseEvent *event )
{
  // One of the mouse buttons was pressed over the window. Convert the Qt event information into
  // the types used by the MapLink 3D interaction modes and forward the event
  // on to the application.

  Qt::KeyboardModifiers modifiers = event->modifiers();
  bool shiftPressed = ( modifiers & Qt::ShiftModifier ) ;
  bool controlPressed = ( modifiers & Qt::ControlModifier ) ;
  int x = event->x() ;
  int y = event->y() ;
  Qt::MouseButton button = event->button();

  bool doRedraw = false;
  if ( button == Qt::LeftButton )
  {
    doRedraw = m_application->onLButtonDown(shiftPressed, controlPressed, x, y);
  }
  else if ( button == Qt::MidButton )
  {
    doRedraw = m_application->onMButtonDown(shiftPressed, controlPressed, x, y);
  }
  else if ( button == Qt::RightButton )
  {
    doRedraw = m_application->onRButtonDown(shiftPressed, controlPressed, x, y);
  }

  // All of the above functions return true if the active interaction mode requires a redraw.
  // If this happened, tell Qt to issue a repaint event. 
  if( doRedraw )
  {
    update();
  }
}

void MapLinkWidget::mouseReleaseEvent( QMouseEvent *event )
{
  // One of the mouse buttons was released over the window. Convert the Qt event information into
  // the types used by the MapLink 3D interaction modes and forward the event
  // on to the application.

  Qt::KeyboardModifiers modifiers = event->modifiers();
  bool shiftPressed = ( modifiers & Qt::ShiftModifier ) ;
  bool controlPressed = ( modifiers & Qt::ControlModifier ) ;
  int x = event->x() ;
  int y = event->y() ;
  Qt::MouseButton button = event->button();

  bool doRedraw = false;
  if ( button == Qt::LeftButton )
  {
    doRedraw = m_application->onLButtonUp(shiftPressed, controlPressed, x, y);
  }
  else if ( button == Qt::MidButton )
  {
    doRedraw = m_application->onMButtonUp(shiftPressed, controlPressed, x, y);
  }
  else if ( button == Qt::RightButton )
  {
    doRedraw = m_application->onRButtonUp(shiftPressed, controlPressed, x, y);
  }

  // All of the above functions return true if the active interaction mode requires a redraw.
  // If this happened, tell Qt to issue a repaint event.
  if( doRedraw )
  {
    update();
  }
}

void MapLinkWidget::wheelEvent( QWheelEvent *event )
{
  // The mouse wheel was used over the window. Convert the Qt event information into
  // the types used by the MapLink 3D interaction modes and forward the event
  // on to the application.

  Qt::KeyboardModifiers modifiers = event->modifiers();
  int x = event->x() ;
  int y = event->y() ;
  if( m_application->onMouseWheel(event->delta(), x, y ) )
  {
    // The above function returns true if the active interaction mode requires the window to be redrawn.
    // If this happened, tell Qt to issue a repaint event. 
    update();
  }
}

bool MapLinkWidget::loadMap( const char *filename )
{
  // This function is invoked by the main window class when the user wants to load a map.
  // The actual loading of the map is handled by the application, but we are responsible for
  // ensuring the view is updated once this is done.

  if( m_application )
  {
    bool success = m_application->loadMap( filename );
    if( success )
    {
      // The map loaded successfully, cause a redraw to occur so that the new map can be seen
      update();
    }
    
    return success;
  }

  return false;
}

bool MapLinkWidget::loadTerrain( const char *filename )
{
  // This function is invoked by the main window class when the user wants to load a terrain database.
  // The actual loading of the database is handled by the application, but we are responsible for
  // ensuring the view is updated once this is done.

  if( m_application )
  {
    bool success = m_application->loadTerrain( filename );
    if( success )
    {
      // The database loaded successfully, cause a redraw to occur so it can be seen
      update();
    }
    
    return success;
  }

  return false;
}

void MapLinkWidget::zoomIn()
{
  // The zoom in toolbar button was used, forward this call to the application in
  // order to update the camera position.
  if( m_application && m_application->zoomIn() )
  {
    // The above function returns true if the active interaction mode requires the window to be redrawn.
    // If this happened, tell Qt to issue a repaint event. 
    update();
  }
}

void MapLinkWidget::zoomOut()
{
  // The zoom out toolbar button was used, forward this call to the application in
  // order to update the camera position.
  if( m_application && m_application->zoomOut() )
  {
    // The above function returns true if the active interaction mode requires the window to be redrawn.
    // If this happened, tell Qt to issue a repaint event. 
    update();
  }
}

void MapLinkWidget::resetView()
{
  // The reset view toolbar button was used, forward this call to the application in
  // order to update the camera position.
  if( m_application && m_application->resetView() )
  {
    // The above function returns true if the active interaction mode requires the window to be redrawn.
    // If this happened, tell Qt to issue a repaint event. 
    update();
  }
}

void MapLinkWidget::activateEyePointInteractionMode()
{
  // The user is changing the active interaction mode, simply forward this call to the application
  if( m_application )
  {
    m_application->activateEyePointInteractionMode();
  }
}

void MapLinkWidget::activateWorldInteractionMode()
{
  // The user is changing the active interaction mode, simply forward this call to the application
  if( m_application )
  {
    m_application->activateWorldInteractionMode();
  }
}

const char* MapLinkWidget::statusBarText() const
{
  // This method is called by the main window to retrieve the text to display on the status bar.
  // Ask the application for the text for the active interaction mode.
  if( m_application )
  {
    return m_application->interactionModePrompt();
  }
  
  return NULL;
}

void MapLinkWidget::setWireframeMode( bool wireframe )
{
  // The user is changing the wireframe rendering state, forward this on to the application.
  if( m_application )
  {
    m_application->setWireframeMode( wireframe );

    // Changing the wireframe state will always cause the window to need redrawing, so tell
    // Qt to issue a repaint event.
    update();
  }
}

void MapLinkWidget::setTerrainExaggeration( bool exaggerate )
{
  // The user is changing whether terrain is drawn exaggerated or not. Forward this on to the
  // application.
  if( m_application )
  {
    m_application->setTerrainExaggeration( exaggerate );

    // Changing the exaggeration state will always cause the window to need redrawing, so tell
    // Qt to issue a repaint event.
    update();
  }
}

void MapLinkWidget::setCameraAltitudeLimit( bool limit )
{
  // The user is changing whether the camera is allowed to move below the terrain or not.
  // Forward this on to the application.
  if( m_application )
  {
    m_application->setCameraAltitudeLimit( limit );

    // Changing this setting doesn't modify the view, so no repaint is required here.
  }
}

