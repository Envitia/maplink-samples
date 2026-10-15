//	********************************************************
//	*           Source Control Information
//	********************************************************
//	$Archive: /Development/Samples/NT/mfc/tsl3dviewmodezoom.cpp $
//	$Author$
//	$Date$
//	$Revision$
//	********************************************************

#include "StdAfx.h"
#include "tsl3dviewmodezoom.h"
#include "camera.h"


TSL3DViewModeZoom::TSL3DViewModeZoom( TSL3DViewModeInterface * iViewMode, unsigned long modeID ) 
  : TSL3DViewMode ( iViewMode, modeID ),
    m_originX     ( 0 ),
    m_originY     ( 0 ),
    m_active      ( false )
{
}

TSL3DViewModeZoom::~TSL3DViewModeZoom()
{
}

bool TSL3DViewModeZoom::OnLButtonDown(long x, long y, bool shift, bool control)
{
  m_originX = x ;
  m_originY = y ;
  m_interface->captureMouse() ;
  m_active = true ;

  return false ;
}

bool TSL3DViewModeZoom::OnMouseMove(long x, long y, bool shift, bool control)
{
  long dx = m_originX - x ;

  if ( !m_active || (dx == 0) )
    return false ;

  double latitude, longitude, altitude ;

  m_drawingSurface->camera()->position( &latitude, &longitude, &altitude ) ;

  long   magnitude = abs( dx ) ;
  double sign      = dx / magnitude  ;

  for ( int i = 0 ; i < magnitude ; ++i )
    altitude += sign * altitude / 100.0 ;

  m_drawingSurface->camera()->moveTo( latitude, longitude, altitude, TSL3DCameraMoveActionNone ) ;
  m_drawingSurface->camera()->lookAt( latitude, longitude + LOOK_AHEAD_OFFSET, 0.0, control ) ;

  m_originX = x ;
  m_originY = y ;

  return true ;
}

bool TSL3DViewModeZoom::OnLButtonUp(long x, long y, bool shift, bool control)
{
  m_interface->releaseMouse() ;
  m_active = false ;
  return false ;
}

