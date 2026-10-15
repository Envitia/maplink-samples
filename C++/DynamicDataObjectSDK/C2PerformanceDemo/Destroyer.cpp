// Destroyer.cpp: implementation of the CDestroyer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Destroyer.h"
#include "DODestroyer.h"
#include "TrackManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDestroyer::CDestroyer()
{
  double vel, alt ;
  CTrackManager::initialiseTrack( DESTROYER, vel, alt ) ;
  m_velocity = vel ;
  m_altitude = alt ;

  updateCourse() ;
}

CDestroyer::~CDestroyer()
{

}

CDestroyer::CDestroyer( const CTrack & other ) : CTrack( other )
{

}

TSLDisplayObject * CDestroyer::instantiateDO( TSLDisplayType key, int dsID ) const
{
  return new CDODestroyer( ) ;
}

TRACK_TYPE CDestroyer::trackType() const
{
  return DESTROYER ;
}
