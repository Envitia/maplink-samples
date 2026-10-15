//	********************************************************
//	*           Source Control Information
//	********************************************************
//	$Archive:$
//	$Author$
//	$Date$
//	$Revision$
//	********************************************************

#include "StdAfx.h"
#include "tsl3dviewmodecamera.h"
#include "camera.h"


TSL3DViewModeCamera::TSL3DViewModeCamera( TSL3DViewModeInterface * iViewMode, unsigned long modeID ) 
  : TSL3DViewMode ( iViewMode, modeID ),
    m_originX     ( 0 ),
    m_originY     ( 0 ), 
    m_active      ( false )
{
}

TSL3DViewModeCamera::~TSL3DViewModeCamera()
{
}

void TSL3DViewModeCamera::orientation(double x, double y, double z, double angle)
{
  // Northings are given by the y-component.
  // Eastings are given by the x-component.
  m_drawingSurface->camera()->orientation( y, x, z, angle );
}

void TSL3DViewModeCamera::fov(double angle)
{
  m_drawingSurface->camera()->angleOfView( angle );
}


bool TSL3DViewModeCamera::OnLButtonDown(long x, long y, bool shift, bool control)
{
  m_originX = x ;
  m_originY = y ;
  m_active  = true ;

  m_interface->captureMouse() ;

  return false ;
}

bool TSL3DViewModeCamera::OnMouseMove(long x, long y, bool shift, bool control)
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
    }
  }

  longitude = newLongitude;

  // Remember the new origin
  m_originX = x ;
  m_originY = y ;

  // Update the screen
  m_drawingSurface->camera()->moveTo( latitude, longitude, altitude, TSL3DCameraMoveActionNone ) ;

  return true ;
}

bool TSL3DViewModeCamera::OnLButtonUp(long x, long y, bool shift, bool control)
{
  m_interface->releaseMouse() ;
  m_active = false ;

  return false ;
}

