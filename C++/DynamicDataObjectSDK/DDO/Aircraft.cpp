// Aircraft.cpp: implementation of the Aircraft class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Aircraft.h"
#include "AircraftDO.h"
#include "TMS747_DO.h"
#include "TMSAirbus_DO.h"
#include "TMSCessna_DO.h"

#include <cassert>
#include <cmath>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


double	      Aircraft::m_tmcPerMeter = 1;

const double KH_to_MS    =   1.0 / 3.6	  ;
const double MS_to_KH    =   3.6	  ;
const double DEG_to_RAD  =  M_PI / 180.0  ;
const double RAD_to_DEG  = 180.0 / M_PI	  ;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Aircraft::Aircraft(unsigned int cls, double departureLatitude, double departureLongitude, const TSLCoordinateSystem & coordinateSystem)
  : m_class	( cls ),
    m_velocity	( 0.0 ),
    m_heading	( 0.0 ),
    m_departureLocation(departureLatitude, departureLongitude),
    m_currentLocation(departureLatitude, departureLongitude), 
    m_coordinateSystem(&coordinateSystem)
{
  coordinateSystem.latLongToTMC(
    TSLCoordinateConverter::normaliseLatitude(m_currentLocation.first),
    TSLCoordinateConverter::normaliseLongitude(m_currentLocation.second),
    &m_currentLocationTmc.m_x, 
    &m_currentLocationTmc.m_y);
  move(m_currentLocationTmc, true);
}

Aircraft::~Aircraft()
{
}

TSLDisplayObject* Aircraft::instantiateDO(TSLDisplayType key, int dsID) const
{
  // Instantiate and return the appropriate DO, or return 0
  // Could use the dsID to identify a particular darwingSurface and use that
  // to instantiate a specific type of Display Object (eg. for main and overview screens)

  switch ( m_class )
  {
    case TMS_747:
      return new TMS747_DO( ) ;

    case TMS_Airbus:
      return new TMSAirbus_DO( ) ;

    case TMS_Cessna:
      return new TMSCessna_DO( ) ;
  }

  return 0 ;
}

double Aircraft::velocity() const
{
  return ( m_velocity*MS_to_KH );
}

void Aircraft::velocity(double vel)
{
  m_velocity = vel*KH_to_MS;
}

double Aircraft::heading() const
{
  return m_heading;
}

void Aircraft::heading(double hdng)
{
  m_heading = hdng;
}

const std::pair<double, double>& Aircraft::departure() const
{
  return m_departureLocation;
}

void Aircraft::destination(double latitude, double longitude)
{
  m_destinationLocation.first = latitude;
  m_destinationLocation.second = longitude;
}

const std::pair<double, double>& Aircraft::destination() const
{
  return m_destinationLocation;
}

const std::pair<double, double>& Aircraft::currentLocation() const
{
  return m_currentLocation;
}

const double M_PIBy2 = 0.5*M_PI;
const double M_PI_2  = 2.0*M_PI;
const double M_PIa2  = 0.99722*M_PI_2;

void Aircraft::update(const TSLCoordinateSystem& coordinateSystem, double rate)
{
  double dx = m_destinationLocation.second - m_currentLocation.second;
  double dy = m_destinationLocation.first - m_currentLocation.first;

  double togo = sqrt( dx*dx + dy*dy );

  bool atDestination = ( togo == 0.0 );

  if ( !atDestination )
  {
    if ( dx != 0.0 )
    {
      m_heading = atan2( dx, dy );
    }
    else
    {
      m_heading = ( dy > 0.0 ) ? 0.0 : M_PI;
    }
    
    double hdg = m_heading;
    while ( hdg < 0.0 )
    {
      hdg += M_PI_2;
    }

    while ( hdg > M_PIa2 )
    {
      hdg -= M_PI_2;
    }

    heading( hdg );

    double speed   = (rate > 0) ? m_velocity*rate : m_velocity;
    double travelled = speed*m_tmcPerMeter;

    if ( travelled > togo ) 
    {
      m_coordinateSystem->latLongToTMC(
        TSLCoordinateConverter::normaliseLatitude(m_destinationLocation.first),
        TSLCoordinateConverter::normaliseLongitude(m_destinationLocation.second),
        &m_currentLocationTmc.m_x, 
        &m_currentLocationTmc.m_y);
      move(m_currentLocationTmc, true);
      m_currentLocation = m_destinationLocation;
    }
    else 
    {
      double dw = travelled / togo;

      dx *= dw;
      dy *= dw;
      m_currentLocation.first += dy;
      m_currentLocation.second += dx;

      m_coordinateSystem->latLongToTMC(
        TSLCoordinateConverter::normaliseLatitude(m_currentLocation.first),
        TSLCoordinateConverter::normaliseLongitude(m_currentLocation.second),
        &m_currentLocationTmc.m_x,
        &m_currentLocationTmc.m_y);
      move(m_currentLocationTmc, true);
    }
  }
}

