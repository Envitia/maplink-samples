// Track.cpp: implementation of the CTrack class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Track.h"
#include "TrackManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTrack::CTrack()
{
  // Initialise members
  m_altitude = 0 ;
  m_velocity = 0 ;
  m_velocityMPS = 0 ;
  m_latitude = 0 ; 
  m_longitude = 0 ;
  m_dx = 0 ;
  m_dy = 0 ;
  m_historyCounter = 10 ;

  // Randomly generate values for the member variables
  m_heading = CTrackManager::random( 0, 359) * PI / 180.0 ;
  m_targetHeading = m_heading + CTrackManager::random( -90, 90) * PI / 180.0 ;
  
  // Set the ID
  m_trackNumber = CTrackManager::getNewTrackID() ;

  char label[200] ; label[0] = '\0' ;
  sprintf( label, "%d", m_trackNumber ) ;

  m_trackLabel = label;
  m_pTrackLabel = m_trackLabel.c_str();
  m_labelLen = m_trackLabel.size();

}

CTrack::CTrack( const CTrack & other ) :
  TSLDynamicDataObject( ), // Don't copy over everything, just initialise it
  m_altitude( other.m_altitude ),
  m_dx( other.m_dx ),
  m_dy( other.m_dy ),
  m_heading( other.m_heading ),
  m_historyCounter( other.m_historyCounter ),
  m_historyPoints( other.m_historyPoints ),
  m_hostility( other.m_hostility ),
  m_latitude( other.m_latitude ),
  m_longitude( other.m_longitude ),
  m_targetHeading( other.m_targetHeading ),
  m_trackNumber( other.m_trackNumber ),
  m_velocity( other.m_velocity ),
  m_velocityMPS( other.m_velocityMPS ),
  m_velocityVectorX( other.m_velocityVectorX ),
  m_velocityVectorY( other.m_velocityVectorY ),
  m_trackLabel( other.m_trackLabel ),
  m_labelLen( other.m_labelLen )
{
  position( other.position() ) ;
  setExtent( other.getExtent() ) ;
  m_pTrackLabel = m_trackLabel.c_str();
}

CTrack::~CTrack()
{
}

// Move the track to its next position, then work out whether the heading needs changing,
// update the history 
void CTrack::translate( TSLMapDataLayer * pLayer )
{  
  // If the track is not being updated by the user or needs moving ....
  if ( m_dx || m_dy )
  {
    TSLDynamicDataObject::translate( m_dx, m_dy, true ) ;

    // Check to see if we need to change the heading 
    // since we are going outside the edge of the map
    long x_tmc = position().x() ;
    long y_tmc = position().y() ;
    
    bool changeHeading = false ;

    if ( x_tmc < CTrackManager::getTMCExtent().xMin() || x_tmc > CTrackManager::getTMCExtent().xMax() )
    {
      m_dx = -m_dx ;
      changeHeading = true ;
    }

    if ( y_tmc < CTrackManager::getTMCExtent().yMin() || y_tmc > CTrackManager::getTMCExtent().yMax() )
    {
      m_dy = -m_dy ;
      changeHeading = true ;
    }

    if ( !m_historyCounter-- )
    {
      m_historyCounter = 10;
      m_historyPoints.push_back( make_pair( x_tmc, y_tmc ) );
      while ( m_historyPoints.size() > CTrackManager::numberHistoryPoints() )
	m_historyPoints.erase( m_historyPoints.begin() );
    }

    if ( changeHeading )
    {
      if ( m_dx != 0.0 )
	m_heading = atan2( (double)m_dx, (double)m_dy ) ;
      else
	m_heading = (m_dy > 0.0) ? 0.0 : PI ;
      
      updateCourse() ;
      m_targetHeading = m_heading + (double)CTrackManager::random( -90, 90 ) * PI / 180.0 ;
    }
    else if ( fabs( m_heading - m_targetHeading ) <= 0.1 )
    {
      // Set new target
      m_targetHeading = m_heading + (double)CTrackManager::random( -90, 90 ) * PI / 180.0 ;
      updateCourse();
    }
    else
    {
      if ( m_targetHeading - m_heading < 0.0 )
	m_heading -= ((double)CTrackManager::random( 0, 25 ) / 10.0) * PI / 180.0 ;
      else
	m_heading += ((double)CTrackManager::random( 0, 25 ) / 10.0) * PI / 180.0 ;
      updateCourse();
    }

    // If selected convert the tmc position to lat long and then
    // to a string format to be displayed on the tote
    if ( CTrackManager::selectedTrack() == this )
    {
      pLayer->TMCToLatLong( x_tmc, y_tmc, &m_latitude, &m_longitude ) ;
    }
  }

}

// Update the heading and velocity vector variables
void CTrack::updateCourse()
{
  while ( m_heading < 0.0 )
    m_heading += PI * 2 ;
  while ( m_heading > PI * 2 )
    m_heading -= PI * 2 ;

      // Convert knots velocity into metres per second
  double magnitude ;

  if ( m_velocity <= 50 )
    magnitude = VELOCITY_SLOW ;
  else if ( m_velocity <= 300 )
    magnitude = VELOCITY_MEDIUM ;
  else
    magnitude = VELOCITY_FAST ;

  m_velocityMPS = m_velocity * 0.5144444444 ; // 1852 / (60 * 60)
  double sh	= sin( m_heading ) ;
  double ch	= cos( m_heading ) ;

  double tmcPerMetre = CTrackManager::tmcPerMetre() ;

  m_dx = tmcPerMetre * ( sh * m_velocityMPS ) ;
  m_dy = tmcPerMetre * ( ch * m_velocityMPS ) ;

  m_velocityVectorX = magnitude * sh ;
  m_velocityVectorY = magnitude * -ch ;
    
}

void CTrack::velocityVectorData( long& x, long& y )   
{
  x = m_velocityVectorX ;
  y = m_velocityVectorY ;
}

const vector< pair< long, long > >& CTrack::historyPoints()
{
  return m_historyPoints ;
}


void CTrack::heading( double heading )
{
  m_heading = heading ;
}

void CTrack::velocity( double velocity )
{
  m_velocity = velocity ;
}

void CTrack::altitude( double altitude )
{
  m_altitude = altitude ;
}

void CTrack::hostility( HOSTILE_TYPE hostility )
{
  m_hostility = hostility ;
}

void CTrack::latitude( char * buffer ) 
{
  CTrackManager::latitudeAsString( m_latitude, buffer ) ;
}

void CTrack::longitude( char * buffer ) 
{
  CTrackManager::longitudeAsString( m_longitude, buffer ) ;
}
