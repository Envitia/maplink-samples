// App6aAircraft.cpp: implementation of the App6aAircraft class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "app6addo.h"
#include "App6aAircraft.h"

#include <cassert>
#include <cmath>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


double	      App6aAircraft::m_tmcPerMeter = 1;

const double KH_to_MS    =   1.0 / 3.6	  ;
const double MS_to_KH    =   3.6	  ;
const double DEG_to_RAD  =  M_PI / 180.0  ;
const double RAD_to_DEG  = 180.0 / M_PI	  ;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

App6aAircraft::App6aAircraft(unsigned int cls)
  : m_class	( cls ),
    m_velocity	( 0.0 ),
    m_heading	( 0.0 )
{
}

App6aAircraft::~App6aAircraft()
{
}

TSLDisplayObject* App6aAircraft::instantiateDO(TSLDisplayType key, int dsID) const
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

double App6aAircraft::velocity() const
{
  return ( m_velocity*MS_to_KH );
}

void App6aAircraft::velocity(double vel)
{
  m_velocity = vel*KH_to_MS;
}

double App6aAircraft::heading() const
{
  return m_heading;
}

void App6aAircraft::heading(double hdng)
{
  m_heading = hdng;
}

const double M_PIBy2 = 0.5*M_PI;
const double M_PI_2  = 2.0*M_PI;
const double M_PIa2  = 0.99722*M_PI_2;

void App6aAircraft::update(double dt, double rate)
{
  double dx   = m_destination.x() - position().x();
  double dy   = m_destination.y() - position().y();

  double togo = sqrt( dx*dx + dy*dy );

  bool notAtDestination = ( togo != 0.0 );

  if ( notAtDestination )
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

    if ( travelled > togo ) {
      move( m_destination.x(), m_destination.y(), true );
    }
    else {
      double dw = travelled / togo;

      dx *= dw;
      dy *= dw;
      
      translate( dx, dy, true );
    }
  }
}

