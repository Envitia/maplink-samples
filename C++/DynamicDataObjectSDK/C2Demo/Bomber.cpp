// Bomber.cpp: implementation of the CBomber class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Bomber.h"
#include "DOBomber.h"
#include "TrackManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBomber::CBomber()
{
  double vel, alt ;
  CTrackManager::initialiseTrack( BOMBER, vel, alt ) ;
  m_velocity = vel ;
  m_altitude = alt ;

  updateCourse() ;
}

CBomber::~CBomber()
{

}

CBomber::CBomber( const CTrack & other ) : CTrack( other )
{

}

TSLDisplayObject * CBomber::instantiateDO( TSLDisplayType key, int dsID ) const
{
  return new CDOBomber( ) ;
}

TRACK_TYPE CBomber::trackType() const
{
  return BOMBER ;
}
