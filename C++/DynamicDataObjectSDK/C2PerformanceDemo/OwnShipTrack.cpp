// OwnShipTrack.cpp: implementation of the COwnShipTrack class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OwnShipTrack.h"
#include "trackmanager.h"
#include "DOOwnShip.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


COwnShipTrack::COwnShipTrack()
{
  // Aircraft carrier
  m_velocity = CTrackManager::random( 0, 30 ) ; 
  m_altitude = 0.0;

  m_hostility = FRIENDLY ;

  updateCourse() ;
}

COwnShipTrack::~COwnShipTrack()
{

}

TSLDisplayObject* COwnShipTrack::instantiateDO( TSLDisplayType key, int dsID ) const
{
  return new CDOOwnShip( ) ;
}


void COwnShipTrack::translate( TSLDataLayer * pLayer ) 
{
  // If the track is not being updated by the user or needs moving ....
  if ( m_dx || m_dy )
  {
    TSLDynamicDataObject::translate( m_dx, m_dy, true ) ;
    
    long x_tmc = position().x() ;
    long y_tmc = position().y() ;
    
    if ( !m_historyCounter-- )
    {
      m_historyCounter = 10;
      m_historyPoints.push_back( make_pair( x_tmc, y_tmc ) );
      while ( (int)m_historyPoints.size() > CTrackManager::numberHistoryPoints() )
	m_historyPoints.erase( m_historyPoints.begin() );
    }

    // Constantly increase the heading, the Own Ship cruises around in a circle
    m_heading += 0.5 / 180.0 * PI ;
    if ( m_heading >= 2.0 * PI )
    {
      m_heading = 0.0 ;
    }
    
    updateCourse() ;

    // If selected convert the tmc position to lat long and then
    // to a string format to be displayed on the tote
    if ( CTrackManager::selectedTrack() == this )
    {
      pLayer->TMCToLatLong( x_tmc, y_tmc, &m_latitude, &m_longitude ) ;
    }
  }

}

TRACK_TYPE COwnShipTrack::trackType() const
{
  return OWN_SHIP ;
}
