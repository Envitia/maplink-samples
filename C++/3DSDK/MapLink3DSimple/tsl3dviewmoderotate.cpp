//	********************************************************
//	*           Source Control Information
//	********************************************************
//	$Archive: /Development/Samples/NT/mfc/tsl3dviewmoderotate.cpp $
//	$Author$
//	$Date$
//	$Revision$
//	********************************************************

#include "StdAfx.h"
#include "tsl3dviewmoderotate.h"
#include "camera.h"


TSL3DViewModeRotate::TSL3DViewModeRotate( TSL3DViewModeInterface * iViewMode, unsigned long modeID ) 
  : TSL3DViewMode ( iViewMode, modeID ),
    m_originX     ( 0 ),
    m_originY     ( 0 ), 
    m_active      ( false ),
    m_upsideDown  ( false )
{
}

TSL3DViewModeRotate::~TSL3DViewModeRotate()
{
}

bool TSL3DViewModeRotate::OnLButtonDown(long x, long y, bool shift, bool control)
{
  m_originX = x ;
  m_originY = y ;
  m_active  = true ;

  // Query whether we are upsideDown, in order to reverse the directions
  m_upsideDown = m_drawingSurface->camera()->upsideDown() ;

  m_interface->captureMouse() ;

  return false ;
}

bool TSL3DViewModeRotate::OnMouseMove(long x, long y, bool shift, bool control)
{
  long dx = m_originX - x ;
  long dy = m_originY - y;

  if ( !m_active || (dx == 0 && dy == 0) )
  {
    return false;
  }

  // Get the current lat/long/alt position
  double latitude, longitude, altitude ;
  m_drawingSurface->camera()->position( &latitude, &longitude, &altitude ) ;

  // Calculate an increment based on the altitude but don't allow it to go too fast
  double distance = altitude;

  static const double MAX_ALTITUDE( 5500000.0 );
  static const double SCALING_FACTOR( 1.0 / 900000.0 );

  if ( distance > MAX_ALTITUDE )
  {
    distance = MAX_ALTITUDE;
  }

  double xFactor = ( distance * SCALING_FACTOR ) / ( 60.0 * cos( degreesToRadians( latitude ) ) );
  double yFactor = -( distance * SCALING_FACTOR ) / 60.0;

  static const double MAX_FACTOR( 0.2 );

  if ( fabs(yFactor) > MAX_FACTOR )
  {
    yFactor = -MAX_FACTOR;
  }

  if ( xFactor > MAX_FACTOR )
  {
    xFactor = MAX_FACTOR;
  }
  
  // If we are upside down, then the directions need to be reversed
  if ( m_upsideDown )
  {
    dx = -dx;
    dy = -dy;
  }

  latitude += (double)dy * yFactor;
  double newLongitude = longitude;
  newLongitude += (double)dx * xFactor;
 
  // Deal with going over the north or south pole - flip the view
  // and mark the fact that we're now upside down
  if ( fabs( latitude ) > 90.0 )
  {
    int sgn = latitude > 90.0 ? -1 : 1;

    latitude = sgn*( fabs( latitude ) - 180.0 );

    if ( fabs( newLongitude - longitude ) < 1.0 )
    {
      // We are now travelling on the opposite meridian.
      newLongitude = longitude + 180.0;

      m_upsideDown = !m_upsideDown;
    }
  }

  longitude = newLongitude;

  // Remember the new origin
  m_originX = x ;
  m_originY = y ;

  // Update the screen

  m_drawingSurface->camera()->moveTo( latitude, longitude, altitude, TSL3DCameraMoveActionNone ) ;
  m_drawingSurface->camera()->lookAt( latitude, longitude + LOOK_AHEAD_OFFSET, 0.0, control ) ;

  return true ;
}

bool TSL3DViewModeRotate::OnLButtonUp(long x, long y, bool shift, bool control)
{
  m_interface->releaseMouse() ;
  m_active = false ;

  return false ;
}

