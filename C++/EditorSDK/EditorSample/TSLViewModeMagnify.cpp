//	********************************************************
//	*           Source Control Information
//	********************************************************
//	$Archive: /Development/Samples/NT/LandLinkSample/TSLViewModeMagnify.cpp $
//	$Author$
//	$Date$
//	$Revision$
//	********************************************************


#include "stdafx.h"
#include "TSLMagnifyingGlass.h"
#include "TSLViewModeMagnify.h"

TSLViewModeMagnify::TSLViewModeMagnify( TSLViewModeInterface* pInterface, unsigned long modeID )
  : TSLViewMode	    ( pInterface, modeID ),
    m_oldXpt	    ( 0 ),
    m_oldYpt	    ( 0 ),
    m_mouseX        ( 0 ),
    m_mouseY        ( 0 )
{
  TSLViewModeInterface* magInterface = pInterface->clone() ;

  m_magnifier = new TSLMagnifyingGlass( magInterface ) ;

  m_interface->bufferingOn();
}

TSLViewModeMagnify::~TSLViewModeMagnify()
{
  delete m_magnifier;
  m_interface->bufferingOff() ;
  m_magnifier = 0;
}

void TSLViewModeMagnify::OnSize(long cx, long cy)
{
  m_magnifier->reset();///
  m_interface->onSize( cx, cy );
  m_magnifier->onSize( cx, cy );
}

void TSLViewModeMagnify::OnDraw( long x1, long y1, long x2, long y2 )
{
  updateMagnifiedArea( m_mouseX, m_mouseY );
  m_interface->magnify( m_magnifier, false );
}

bool TSLViewModeMagnify::OnMouseMove(TSLViewModeButton button, long x, long y, bool shift, bool control)
{
  m_mouseX = x ;
  m_mouseY = y ;

  // Change the position of the magnifier.
  updateMagnifierExtent( x, y );

  // Change the zoom area of the magnifier.
  updateMagnifiedArea( x, y );

  // Co-ordinate the drawing from here.
  m_interface->magnify( m_magnifier, false );
  
  // The magnify() method always does the drawing so do not want a redraw here
  return false;
}

bool TSLViewModeMagnify::OnRButtonUp( long x, long y, bool shift, bool control )
{
  // Reset the current view mode to edit
  m_interface->resetMode() ;
  return true ;
}

void TSLViewModeMagnify::updateMagnifierExtent( long x, long y )
{
  //-------------------- Magnifier position & extent --------------------//
  //
  TSLDeviceUnits scrx1;
  TSLDeviceUnits scry1;
  TSLDeviceUnits scrx2;
  TSLDeviceUnits scry2;
  m_drawingSurface->getDUExtent( &scrx1, &scry1, &scrx2, &scry2 ) ;
  int half_side = 0.5*0.01*( (scrx2-scrx1) + (scry2-scry1) )*m_magnifier->size();
  
  // Work out offset for magnifier.
  int offset   = half_side*m_magnifier->offset();
  
  double dx    = x - m_oldXpt;
  double dy    = y - m_oldYpt;
  double dr    = sqrt( dx*dx + dy*dy );
  
  double cosT  = fabs( dx/dr );
  double sinT  = fabs( dy/dr );
  
  int ptX      = ( x > m_oldXpt ) ? x - offset*cosT : x + offset*cosT;
  int ptY      = ( y > m_oldYpt ) ? y - offset*sinT : y + offset*sinT;
  
  int leftX    = ptX - half_side;
  int rightX   = ptX + half_side;
  int bottomY  = ptY + half_side;
  int topY     = ptY - half_side;
  
  int dw = 2*half_side;
  
  if ( leftX < 0 )
  {
    leftX   = 0;
    rightX  = dw;
  }
  else if ( rightX > scrx2 )
  {
    rightX  = scrx2;
    leftX   = rightX - dw;
  }
  
  if ( topY < 0 )
  {
    topY    = 0;
    bottomY = dw;
  }
  else if ( bottomY > scry2 )
  {
    bottomY = scry2;
    topY    = bottomY - dw;
  }
  
  m_oldXpt = ( leftX + rightX ) / 2;
  m_oldYpt = ( bottomY + topY ) / 2;
  
  dx       = x - m_oldXpt;
  dy       = y - m_oldYpt;
  dr       = dx*dx + dy*dy;
  
  if ( dr < half_side*half_side ) {
    m_oldXpt = x;
    m_oldYpt = y;
  }
  
  m_magnifier->setWinExtent( leftX, topY, rightX, bottomY );
  //
  //-------------------- Magnifier position & extent --------------------//
}

void TSLViewModeMagnify::updateMagnifiedArea( long x, long y )
{
  //-------------------- Magnifier zoom area --------------------//
  //
  double uux1;
  double uux2;
  double uuy1;
  double uuy2;

  m_drawingSurface->getUUExtent( &uux1, &uuy1, &uux2, &uuy2 );

  double halfSideUU = 0.5*0.01*( (uux2-uux1) + (uuy2-uuy1) )*m_magnifier->size() / m_magnifier->magnification();
  
  double uuX;
  double uuY;
  m_drawingSurface->DUToUU( x, y, &uuX, &uuY ); 

  double zoom_uux1 = uuX - halfSideUU ;
  double zoom_uux2 = uuX + halfSideUU ;
  double zoom_uuy1 = uuY - halfSideUU ;
  double zoom_uuy2 = uuY + halfSideUU ;
  
  m_magnifier->setMagExtent( zoom_uux1, zoom_uuy1, zoom_uux2, zoom_uuy2 );
  //
  //-------------------- Magnifier zoom area --------------------//
}

