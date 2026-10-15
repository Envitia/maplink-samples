// AirCraftCarrier.cpp: implementation of the CAirCraftCarrier class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AirCraftCarrier.h"
#include "DOAirCraftCarrier.h"
#include "TrackManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAircraftCarrier::CAircraftCarrier() : CTrack( )
{
  double vel, alt ;
  CTrackManager::initialiseTrack( AIRCRAFT_CARRIER, vel, alt ) ;
  m_velocity = vel ;
  m_altitude = alt ;

  updateCourse() ;
}

CAircraftCarrier::~CAircraftCarrier()
{

}

CAircraftCarrier::CAircraftCarrier( const CTrack & other ) : CTrack( other )
{

}

TSLDisplayObject * CAircraftCarrier::instantiateDO( TSLDisplayType key, int dsID ) const
{
  return new CDOAircraftCarrier( ) ;
}

TRACK_TYPE CAircraftCarrier::trackType() const
{
  return AIRCRAFT_CARRIER ;
}
