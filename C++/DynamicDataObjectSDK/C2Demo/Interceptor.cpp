// Interceptor.cpp: implementation of the CInterceptor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Interceptor.h"
#include "DOInterceptor.h"
#include "TrackManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInterceptor::CInterceptor()
{
  double vel, alt ;
  CTrackManager::initialiseTrack( INTERCEPTOR, vel, alt ) ;
  m_velocity = vel ;
  m_altitude = alt ;

  updateCourse() ;
}

CInterceptor::~CInterceptor()
{

}

CInterceptor::CInterceptor( const CTrack & other ) : CTrack( other )
{

}

TSLDisplayObject * CInterceptor::instantiateDO( TSLDisplayType key, int dsID ) const
{
  return new CDOInterceptor( ) ;
}

TRACK_TYPE CInterceptor::trackType() const
{
  return INTERCEPTOR ;
}
