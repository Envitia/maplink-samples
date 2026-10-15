// Frigate.cpp: implementation of the CFrigate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Frigate.h"
#include "DOFrigate.h"
#include "TrackManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFrigate::CFrigate()
{
  double vel, alt ;
  CTrackManager::initialiseTrack( FRIGATE, vel, alt ) ;
  m_velocity = vel ;
  m_altitude = alt ;

  updateCourse() ;
}

CFrigate::~CFrigate()
{

}

CFrigate::CFrigate( const CTrack & other ) : CTrack( other )
{

}

TSLDisplayObject * CFrigate::instantiateDO( TSLDisplayType key, int dsID ) const
{
  return new CDOFrigate( ) ;
}

TRACK_TYPE CFrigate::trackType() const
{
  return FRIGATE ;
}
