// Helicopter.cpp: implementation of the CHelicopter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Helicopter.h"
#include "DOHelicopter.h"
#include "TrackManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHelicopter::CHelicopter()
{
  double vel, alt ;
  CTrackManager::initialiseTrack( HELICOPTER, vel, alt ) ;
  m_velocity = vel ;
  m_altitude = alt ;

  updateCourse() ;
}

CHelicopter::~CHelicopter()
{

}

CHelicopter::CHelicopter( const CTrack & other ) : CTrack( other )
{

}

TSLDisplayObject * CHelicopter::instantiateDO( TSLDisplayType key, int dsID ) const
{
  return new CDOHelicopter( ) ;
}

TRACK_TYPE CHelicopter::trackType() const
{
  return HELICOPTER ;
}
