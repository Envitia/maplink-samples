// Submarine.cpp: implementation of the CSubmarine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Submarine.h"
#include "DOSubmarine.h"
#include "TrackManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSubmarine::CSubmarine()
{
  double vel, alt ;
  CTrackManager::initialiseTrack( SUBMARINE, vel, alt ) ;
  m_velocity = vel ;
  m_altitude = alt ;

  updateCourse() ;
}

CSubmarine::~CSubmarine()
{

}

CSubmarine::CSubmarine( const CTrack & other ) : CTrack( other )
{

}

TSLDisplayObject * CSubmarine::instantiateDO( TSLDisplayType key, int dsID ) const
{
  return new CDOSubmarine( ) ;
}

TRACK_TYPE CSubmarine::trackType() const
{
  return SUBMARINE ;
}
