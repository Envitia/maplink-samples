// Unknown.cpp: implementation of the CUnknown class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Unknown.h"
#include "DOUnknown.h"
#include "TrackManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUnknown::CUnknown()
{
  double vel, alt ;
  CTrackManager::initialiseTrack( UNKNOWN_TRACK, vel, alt ) ;
  m_velocity = vel ;
  m_altitude = alt ;

  updateCourse() ;
}

CUnknown::~CUnknown()
{

}

CUnknown::CUnknown( const CTrack & other ) : CTrack( other )
{

}

TSLDisplayObject * CUnknown::instantiateDO( TSLDisplayType key, int dsID ) const
{
  return new CDOUnknown( ) ;
}

TRACK_TYPE CUnknown::trackType() const
{
  return UNKNOWN_TRACK ;
}
