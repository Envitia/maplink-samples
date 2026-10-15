// GeneralAir.cpp: implementation of the CGeneralAir class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GeneralAir.h"
#include "DOGeneralAir.h"
#include "TrackManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGeneralAir::CGeneralAir()
{
  double vel, alt ;
  CTrackManager::initialiseTrack( GENERAL_AIR, vel, alt ) ;
  m_velocity = vel ;
  m_altitude = alt ;

  updateCourse() ;
}

CGeneralAir::~CGeneralAir()
{

}

CGeneralAir::CGeneralAir( const CTrack & other ) : CTrack( other )
{

}

TSLDisplayObject * CGeneralAir::instantiateDO( TSLDisplayType key, int dsID ) const
{
  return new CDOGeneralAir( ) ;
}

TRACK_TYPE CGeneralAir::trackType() const
{
  return GENERAL_AIR ;
}
