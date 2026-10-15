#include "stdafx.h"
#include "TSLViewModeZoom.h"

TSLViewModeZoom::TSLViewModeZoom( TSLViewModeInterface * iViewMode, 
				  unsigned long modeID   ) 
  : TSLViewMode( iViewMode, modeID )
{
  // Clear down the starting flags.
  m_startX = 0 ;
  m_startY = 0 ;
  m_pressed = false ;
}

TSLViewModeZoom::~TSLViewModeZoom()
{
  // If are in the middle of an operation, then remove any rubber rectangle,
  // and release the mouse capture.
  if ( m_pressed )
  {
    m_interface->clearEchoRectangle( ) ;
    m_interface->releaseMouse( ) ;
  }
}

bool TSLViewModeZoom::OnMouseMove(TSLViewModeButton button, long x, long y, bool shift, bool control)
{
  // Only do anything if we have pressed
  if ( !m_pressed )
    return false ;

  // Calculate delta offsets from the press event location
  long dx = x - m_startX ;
  long dy = y - m_startY ;

  if ( abs( dx ) < 3 && abs( dy ) < 3 )
  {
    // If we haven't moved more than 3 pixels, then assume its just mouse wobble.
    m_interface->clearEchoRectangle( ) ;
  }
  else
  {
    // We have moved more than 3 pixels from the start position.
    // Set the position of the echo rectangle. Automatically causes it to be drawn
    m_interface->setEchoRectangle( m_startX, m_startY, x, y ) ;
  }

  // We never want a redraw here
  return false ;
}

bool TSLViewModeZoom::OnLButtonDown(long x, long y, bool shift, bool control)
{
  // Remember where the event occurred but wait until the release
  // before doing anything.
  m_startX = x ;
  m_startY = y ;
  m_pressed = true ;

  // Capture the mouse so that we get the Button Up event even if the user leaves
  // the window.
  m_interface->captureMouse( ) ;
  return false ;
}

bool TSLViewModeZoom::OnLButtonUp(long x, long y, bool shift, bool control)
{
  // Now the user has released the button, we can deduce what they want to do.

  // Release the mouse capture.
  m_interface->releaseMouse( ) ;

  // Firstly clear the flags and any rectangle drawn.
  m_interface->clearEchoRectangle( ) ;
  m_pressed = false ;

  // Calculate the size of any rectangle dragged
  long dx = x - m_startX ;
  long dy = y - m_startY ;

  if ( abs( dx ) < 3 && abs( dy ) < 3 )
  {
    return false ;
  }

  // Here we have a rectangle to zoom to.
  double x1, y1 ;
  double x2, y2 ;

  // Calculate the corners in user units.  It doesn't matter which way
  // round they are, since the TSLDrawingSurface will sort them.
  m_drawingSurface->DUToUU( m_startX, m_startY, &x1, &y1 ) ;
  m_drawingSurface->DUToUU( x, y, &x2, &y2 ) ;

  // Zoom to the requested rectangle without auto redraw. 
  // The final argument requests that the TSLDrawingSurface maintains the existing 
  // aspect ratio, as determined by the window.
  // If the resize is successful, request a redraw.
  return m_drawingSurface->resize( x1, y1, x2, y2, false, true );
}

bool TSLViewModeZoom::OnRButtonUp(long x, long y, bool shift, bool control)
{
  // Reset the current view mode to edit
  m_interface->resetMode() ;
  return 0 ;
}

const char * TSLViewModeZoom::queryPrompt( )
{
  return "Left button drag zoom rectangle, Right button click to finish" ;
}

void TSLViewModeZoom::OnDraw( long x1, long y1, long x2, long y2 )
{
  m_interface->drawEchoRectangle( ) ;
}
