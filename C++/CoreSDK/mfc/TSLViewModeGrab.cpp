#include "StdAfx.h"
#include "TSLViewModeGrab.h"

TSLViewModeGrab::TSLViewModeGrab(TSLViewModeInterface* iViewMode, unsigned long modeID)
  : TSLViewMode( iViewMode, modeID )
{
  m_x = 0 ;
  m_y = 0 ;
  m_pressed = false ;
}

bool TSLViewModeGrab::OnLButtonDown(long x, long y, bool shift, bool control)
{
  m_interface->captureMouse() ;

  // Store the location of the mouse click
  m_x = x ;
  m_y = y ;

  m_pressed = true ;

  return false ;	
}

bool TSLViewModeGrab::OnLButtonUp(long x, long y, bool shift, bool control)
{
  m_interface->releaseMouse() ;
  m_pressed = false ;

  return true ;
}

bool TSLViewModeGrab::OnMouseMove(long x, long y, bool shift, bool control)
{
  if ( !m_pressed )
    return false ;

  // Calculate offset between the last point and the new point
  long du_offset_x = m_x - x ;
  long du_offset_y = y - m_y ;

  // Convert the offset into user units

  // No DUPerUU() function so we will take the long way round.
  double tmcPerDUx = 0.0, tmcPerDUy = 0.0 ;
	m_drawingSurface->TMCperDU( tmcPerDUx, tmcPerDUy ) ;
  double uu_per_du = tmcPerDUx / m_drawingSurface->TMCperUU() ;

  double uu_offset_x = du_offset_x * uu_per_du ;
  double uu_offset_y = du_offset_y * uu_per_du ;

  // Get the current centre point of the drawing surface
  double uu_x = 0.0 ;
  double uu_y = 0.0 ;
  double uu_x1 = 0.0 ;
  double uu_y1 = 0.0 ;
  double uu_x2 = 0.0 ;
  double uu_y2 = 0.0 ;

  m_drawingSurface->getUUExtent( &uu_x1, &uu_y1, &uu_x2, &uu_y2 ) ;
  uu_x = ( uu_x1 + uu_x2 ) / 2 ;
  uu_y = ( uu_y1 + uu_y2 ) / 2 ;
  
  uu_x += uu_offset_x ;
  uu_y += uu_offset_y ;

  // Save the current point

  m_x = x ;
  m_y = y ;

  // Pan and redraw the drawing surface

  // Attempt to pan, without auto_redraw enabled.
  // Request redraw only if pan is successful
  return m_drawingSurface->pan( uu_x, uu_y, false ) ;
}

