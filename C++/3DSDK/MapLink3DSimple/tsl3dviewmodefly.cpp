//	********************************************************
//	*           Source Control Information
//	********************************************************
//	$Archive: /Development/Samples/NT/mfc/tsl3dviewmodefly.cpp $
//	$Author$
//	$Date$
//	$Revision$
//	********************************************************

#include "StdAfx.h"
#include "tsl3dviewmodefly.h"


TSL3DViewModeFly::TSL3DViewModeFly(TSL3DViewModeInterface* iViewMode, unsigned long modeID)
  : TSL3DViewMode ( iViewMode, modeID ),
    m_originX     ( 0 ),
    m_originY     ( 0 ),
    m_active      ( false )
{
}

TSL3DViewModeFly::~TSL3DViewModeFly()
{
}

bool TSL3DViewModeFly::OnLButtonDown(long x, long y, bool shift, bool control)
{
  m_originX = x ;
  m_originY = y ;
  m_interface->captureMouse() ;
  m_active = true ;

  return false ;
}

bool TSL3DViewModeFly::OnMouseMove(long x, long y, bool shift, bool control)
{
  long dy = y - m_originY ;
  long dx = x - m_originX ;

  if ( !m_active || (dy == 0 && dx == 0) )
  {
    return false;
  }

  double latitude, longitude, altitude;
  m_drawingSurface->camera()->position( &latitude, &longitude, &altitude );

  static const double MIN_ALTITUDE = 100.0;

  if ( altitude < MIN_ALTITUDE )
  {
    altitude = MIN_ALTITUDE;
  }

  static const double SCALING_FACTOR = 0.02;

  double delta = dy*altitude*SCALING_FACTOR;

  double forward  = control ? 0.0 : -delta;
  double upwards  = control ? -delta : 0.0;
  double sideways = dx*altitude*SCALING_FACTOR;

  static TSL3DCameraMoveAction action = TSL3DCameraMoveActionMaintainGroundOrientation;

  bool maintainAltitude = !control;

  m_drawingSurface->camera()->translate( forward, sideways, upwards, action, maintainAltitude );

  m_originX = x;
  m_originY = y;

  return true;
}

bool TSL3DViewModeFly::OnLButtonUp(long x, long y, bool shift, bool control)
{
  m_interface->releaseMouse() ;
  m_active = false ;
  return false ;
}

