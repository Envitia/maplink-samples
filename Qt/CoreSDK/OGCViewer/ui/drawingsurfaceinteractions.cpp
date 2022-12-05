/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include <QMouseEvent>
#include <QLabel>

#include "drawingsurfaceinteractions.h"

#include "MapLink.h"
#include "MapLinkDrawing.h"
#include "MapLinkIMode.h"

// Interaction mode IDs - these can be any numbers
#define ZOOM_INTERACTION_MODE                   1
#define PAN_INTERACTION_MODE                    2
#define GRAB_INTERACTION_MODE                   3

DrawingSurfaceInteractions::DrawingSurfaceInteractions( QWidget *parent )
  : DrawingSurfaceWidget( parent )
  , m_statusBarMUPosition( NULL )
  , m_statusBarLatLonPosition( NULL )
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

  // Enable mouse tracking
  setMouseTracking( true );

  // Create the interaction mode manager to handle map navigation
  m_modeManager = new TSLInteractionModeManagerGeneric( this, m_surface );

  // Add the three interaction mode types to the manager - the grab mode is the default
  m_modeManager->addMode( new TSLInteractionModeZoom( ZOOM_INTERACTION_MODE ), false ) ;
  m_modeManager->addMode( new TSLInteractionModePan( PAN_INTERACTION_MODE ), false ) ;
  m_modeManager->addMode( new TSLInteractionModeGrab( GRAB_INTERACTION_MODE ), true ) ;

  m_modeManager->setCurrentMode( GRAB_INTERACTION_MODE );
}

DrawingSurfaceInteractions::~DrawingSurfaceInteractions()
{
  delete m_modeManager;
}

// Interaction Mode request implementations.
void DrawingSurfaceInteractions::resetMode(TSLInteractionMode* /*mode*/, TSLButtonType /*button*/, TSLDeviceUnits /*xDU*/, TSLDeviceUnits /*yDU*/)
{
  // Do nothing
}

void DrawingSurfaceInteractions::viewChanged(TSLDrawingSurface* /*drawingSurface*/)
{
  // Do nothing
}

void DrawingSurfaceInteractions::zoomIn()
{
  if( m_modeManager && m_modeManager->zoomIn( 30 ) )
  {
    // Request a redraw if the interaction hander requires it
    update();
  }
}

void DrawingSurfaceInteractions::zoomOut()
{
  if( m_modeManager && m_modeManager->zoomOut( 30 ) )
  {
    // Request a redraw if the interaction hander requires it
    update();
  }
}

void DrawingSurfaceInteractions::activateZoomMode()
{
  // Activate the zoom interaction mode
  if( m_modeManager )
  {
    m_modeManager->setCurrentMode( ZOOM_INTERACTION_MODE ) ;
  }
}

void DrawingSurfaceInteractions::activatePanMode()
{
  // Activate the pan interaction mode
  if( m_modeManager )
  {
    m_modeManager->setCurrentMode( PAN_INTERACTION_MODE ) ;
  }
}

void DrawingSurfaceInteractions::activateGrabMode()
{
  // Activate the grab interaction mode
  if( m_modeManager )
  {
    m_modeManager->setCurrentMode( GRAB_INTERACTION_MODE ) ;
  }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Mouse & Keyboard handling
//
// We only update the display if MapLink tells us too.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void DrawingSurfaceInteractions::mouseMoveEvent( QMouseEvent *event )
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

  // Display the current cursor position in both Map Units and lat/lon in the status bar widgets we were given
  double cursorMUX = 0.0, cursorMUY = 0.0;
  if( m_statusBarMUPosition && m_surface && m_surface->getNumDataLayers() > 0 &&
      m_surface->DUToMU( event->x(), event->y(), &cursorMUX, &cursorMUY ) )
  {
    QString label = QString( tr("X = %1  Y = %2 (Map Units)") ).arg( cursorMUX, 10, 'f', 4 ).arg( cursorMUY, 10, 'f', 4 );
    m_statusBarMUPosition->setText( label );
  }
  else
  {
    m_statusBarMUPosition->setText( "X = <invalid>  Y = <invalid> (Map Units)" );
  }
  
  double cursorLat = 0.0, cursorLon = 0.0;
  if( m_statusBarLatLonPosition && m_surface && m_surface->getNumDataLayers() > 0 &&
      m_surface->DUToLatLong( event->x(), event->y(), &cursorLat, &cursorLon ) )
  {
    QString label = QString( "Latitude = %1  Longitude = %2" ).arg( cursorLat, 8, 'f', 6 ).arg( cursorLon, 8, 'f', 6 );
    m_statusBarLatLonPosition->setText( label );
  }
  else
  {
    m_statusBarLatLonPosition->setText( "Latitude = <invalid>  Longuitude = <invalid>" );
  }
}

void DrawingSurfaceInteractions::mousePressEvent( QMouseEvent *event )
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

void DrawingSurfaceInteractions::mouseReleaseEvent( QMouseEvent *event )
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

void DrawingSurfaceInteractions::wheelEvent( QWheelEvent *event )
{
  // Forward the event onto the application
  if( m_modeManager->onMouseWheel(event->delta(), event->x(), event->y()) )
  {
    update(); // We were asked to redraw the display
  }
}
