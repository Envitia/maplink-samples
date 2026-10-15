// TSLViewModeGrab.cpp: implementation of the TSLViewModeGrab class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "C2PerformanceDemo.h"
#include "TSLViewModeGrab.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TSLViewModeGrab::TSLViewModeGrab( TSLViewModeInterface * iViewMode, unsigned long modeID )
: TSLViewMode( iViewMode, modeID ), m_pressed( false ), m_lastX( 0 ), m_lastY( 0 ),
m_lastDUX( 0 ), m_lastDUY( 0 )
{

}

TSLViewModeGrab::~TSLViewModeGrab()
{

}

bool TSLViewModeGrab::OnLButtonDown( long x, long y, bool shift, bool control )
{
  // Remember the starting point for the first grab....

  // Store the device units for the last grab
  m_lastDUX = x ;
  m_lastDUY = y ;

  // Convert x & y into User Units 
  m_drawingSurface->DUToUU( x, y, &m_lastX, &m_lastY ) ;

  m_pressed = true ;

  // Set the id so that the cursor will change
  m_id = ID_TOOLS_GRABBING ;

  return true ;
}

bool TSLViewModeGrab::OnMouseMove( long x, long y, bool shift, bool control )
{
  // Check that we are either in the process of grabbing or that the mouse has moved
  if ( !m_pressed || (x == m_lastDUX && y == m_lastDUY) )
    return false ;

  m_lastDUX = x ;
  m_lastDUY = y ;

  double thisX = 0.0 ;
  double thisY = 0.0 ;

  // convert the DUs to UUs
  m_drawingSurface->DUToUU( x, y, &thisX, &thisY ) ;

  // calculate the offset
  double uu_offset_x = m_lastX - thisX ;
  double uu_offset_y = m_lastY - thisY ;
  
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

  // Attempt to pan, without auto_redraw enabled.
  // Request redraw only if pan is successful
  m_drawingSurface->pan( uu_x, uu_y, false ) ;

  m_drawingSurface->DUToUU( x, y, &m_lastX, &m_lastY ) ;

  return true ;

}

bool TSLViewModeGrab::OnLButtonUp( long x, long y, bool shift, bool control )
{

  m_pressed = false ;

  // Set the id back to normal ( for the cursor )
  m_id = ID_TOOLS_GRAB ;

  return true ;

}