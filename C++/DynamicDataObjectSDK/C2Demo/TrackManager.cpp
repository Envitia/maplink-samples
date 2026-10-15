// TrackManager.cpp: implementation of the CTrackManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TrackManager.h"
#include "MainFrm.h"
#include "C2DemoDoc.h"

#include "DOAircraftCarrier.h"
#include "DOBomber.h"
#include "DODestroyer.h"
#include "DOFrigate.h"
#include "DOGeneralAir.h"
#include "DOHelicopter.h"
#include "DOInterceptor.h"
#include "DOOwnShip.h"
#include "DORangeRing.h"
#include "DOSubmarine.h"
#include "DOUnknown.h"

#include "OwnShipTrack.h"
#include "AircraftCarrier.h"
#include "Bomber.h"
#include "Destroyer.h"
#include "Frigate.h"
#include "GeneralAir.h"
#include "Helicopter.h"
#include "Interceptor.h"
#include "Submarine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// Map related variables
long CTrackManager::m_tmcPerMetre	      = 1 ;
TSLEnvelope CTrackManager::m_extent ;

// Track related variables
bool CTrackManager::m_displayPlatformCodes    = true ;
bool CTrackManager::m_displayTrackNums	      = true ;
bool CTrackManager::m_displayVelocityVectors  = true ;
bool CTrackManager::m_displayRangeRings	      = true ;
bool CTrackManager::m_displayHistoryPoints    = true ;
bool CTrackManager::m_displayHostileTracks    = true ;
bool CTrackManager::m_displayFriendlyTracks   = true ;
bool CTrackManager::m_displayNeutralTracks    = true ;
bool CTrackManager::m_displayUnknownTracks    = true ;

unsigned int CTrackManager::m_numHistoryPoints	      = 10 ;

bool CTrackManager::m_northUp		      = true ;
bool CTrackManager::m_trackHeadingUp	      = false ;
bool CTrackManager::m_followSelectedTrack     = false ;

int  CTrackManager::m_trackNumber	      = 0 ;

COwnShipTrack * CTrackManager::m_ownship      = NULL ;
CTrack * CTrackManager::m_selectedTrack	      = NULL ;
CTrack * CTrackManager::m_dragTrack	      = NULL ;


void CTrackManager::setTMCExtent( long x1, long y1, long x2, long y2 )
{
  m_extent.corners( x1, y1, x2, y2 ) ;
}

//////////////////////////////////////////
// Track Management functions
//////////////////////////////////////////

// Get intialisation values for a track depending on the type of track
void CTrackManager::initialiseTrack( TRACK_TYPE type, double& velocity, double& altitude )
{
  switch( type ) 
  {
  case AIRCRAFT_CARRIER :
    velocity = random( 0 , 30 ) ;
    altitude = 0 ;
    break ;
  case BOMBER :
    velocity = random( 200, 800 ) ;
    altitude = random( 10000, 30000 ) ;
    break ;
  case DESTROYER :
    velocity = random( 0, 35 ) ;
    altitude = 0 ;
    break ;
  case FRIGATE :
    velocity = random( 0, 35 ) ;
    altitude = 0 ;
    break ;
  case GENERAL_AIR :
    velocity = random( 200, 800 ) ;
    altitude = random( 10000, 30000 ) ;
    break ;
  case HELICOPTER :
    velocity = random( 20, 200 ) ;
    altitude = random( 500, 5000 ) ;
    break ;
  case INTERCEPTOR :
    velocity = random( 200, 800 ) ;
    altitude = random( 10000, 30000 ) ;
    break ;
  case SUBMARINE :
    velocity = random( 0, 20 ) ;
    altitude = random( -2000, 0 ) ;
    break ;
  case UNKNOWN_TRACK :
    velocity = random( 0, 300 ) ;
    altitude = random( -2000, 15000 ) ;
    break ;
  }    
}

// When a track's type has changed it calls this function to 
// get a new display object
CDOTrack * CTrackManager::getDisplayObject( TRACK_TYPE type ) 
{
  switch( type )
  {
  case AIRCRAFT_CARRIER :
    return new CDOAircraftCarrier() ;
  case BOMBER :
    return new CDOBomber() ;
  case DESTROYER :
    return new CDODestroyer() ;
  case FRIGATE :
    return new CDOFrigate() ;
  case GENERAL_AIR :
    return new CDOGeneralAir() ;
  case HELICOPTER :
    return new CDOHelicopter() ;
  case INTERCEPTOR :
    return new CDOInterceptor() ;
  case SUBMARINE :
    return new CDOSubmarine() ;
  case UNKNOWN_TRACK :
    return new CDOUnknown() ;
  }

  return NULL ;

}

void CTrackManager::displayTrackNumbers( bool display )
{
  m_displayTrackNums = display ;
}

bool CTrackManager::displayTrackNumbers()
{
  return m_displayTrackNums ;
}

void CTrackManager::displayPlatformCodes( bool display )
{
  m_displayPlatformCodes = display ;
}
bool CTrackManager::displayPlatformCodes()
{
  return m_displayPlatformCodes ;
}

void CTrackManager::displayVelocityVectors( bool display )
{
  m_displayVelocityVectors = display ;
}

bool CTrackManager::displayVelocityVectors()
{
  return m_displayVelocityVectors ;
}

void CTrackManager::displayHostileTracks( bool display )
{
  m_displayHostileTracks = display ;
}

bool CTrackManager::displayHostileTracks()
{
  return m_displayHostileTracks ;
}

void CTrackManager::displayFriendlyTracks( bool display )
{
  m_displayFriendlyTracks = display ;
}

bool CTrackManager::displayFriendlyTracks()
{
  return m_displayFriendlyTracks ;
}

void CTrackManager::displayNeutralTracks( bool display )
{
  m_displayNeutralTracks = display ;
}

bool CTrackManager::displayNeutralTracks()
{
  return m_displayNeutralTracks ;
}

void CTrackManager::displayUnknownTracks( bool display )
{
  m_displayUnknownTracks = display ;
}

bool CTrackManager::displayUnknownTracks()
{
  return m_displayUnknownTracks ;
}

bool CTrackManager::displayTrackType( HOSTILE_TYPE hostility )
{
  switch( hostility )
  {
  case HOSTILE :
    return m_displayHostileTracks ;
    break ;
  case FRIENDLY :
    return m_displayFriendlyTracks ;
    break ;
  case NEUTRAL :
    return m_displayNeutralTracks ;
    break ;
  case UNKNOWN :
    return m_displayUnknownTracks ;
    break ;
  }

  return false ;
}

void CTrackManager::displayRangeRings( bool display )
{
  m_displayRangeRings = display ;
}

bool CTrackManager::displayRangeRings()
{
  return m_displayRangeRings ;
}

void CTrackManager::displayHistoryPoints( bool display )
{
  m_displayHistoryPoints = display ;
}

bool CTrackManager::displayHistoryPoints()
{
  return m_displayHistoryPoints ;
}

void CTrackManager::numberHistoryPoints( unsigned int points )
{
  m_numHistoryPoints = points ;
}

unsigned int CTrackManager::numberHistoryPoints()
{
  return m_numHistoryPoints ;
}

void CTrackManager::ownShip( COwnShipTrack * ship )
{
  m_ownship = ship ;
}

COwnShipTrack * CTrackManager::ownShip()
{
  return m_ownship ;
}

void CTrackManager::selectedTrack( CTrack * track )
{
  m_selectedTrack = track ;
}

CTrack * CTrackManager::selectedTrack()
{
  return m_selectedTrack ;
}

void CTrackManager::dragTrack( CTrack * track )
{
  m_dragTrack = track ;
}

CTrack * CTrackManager::dragTrack()
{
  return m_dragTrack ;
}


void CTrackManager::northUp( bool apply )
{
  m_northUp = apply ;
  m_trackHeadingUp = !apply ;
}

bool CTrackManager::northUp()
{
  return m_northUp ;
}

void CTrackManager::trackHeadingUp( bool apply )
{
  m_trackHeadingUp = apply ;
  m_northUp = !apply ;
}

bool CTrackManager::trackHeadingUp()
{
  return m_trackHeadingUp ;
}

void CTrackManager::followSelectedTrack( bool apply )
{
  m_followSelectedTrack = apply ;
}

bool CTrackManager::followSelectedTrack()
{
  return m_followSelectedTrack ;
}

bool CTrackManager::ZoomTo( TSLDrawingSurface * pSurface, double range )
{
  if ( !CTrackManager::ownShip() )
    return false ;
 
  // Work out the size of the drawing surface....
  TSLDeviceUnits x1, y1, x2, y2 ;
  pSurface->getDUExtent( &x1 , &y1, &x2, &y2 ) ;
  
  double width = x2 - x1 ;
  double height = y2 - y1 ;

  // Find the position of the own ship track
  TSLCoord coord = (CTrackManager::ownShip())->position() ;

  // Convert the position from TMCs to User Units
  double cx, cy ;
  pSurface->TMCToUU( coord.x(), coord.y(), &cx, &cy ) ;

  // Work out the new size for the drawing surface
  double uux1, uux2, uuy1, uuy2 ;
  if ( width < height )
  {
    uux1 = cx - range * 1000 ;
    uux2 = cx + range * 1000 ;
    uuy1 = cy - range * height / width * 1000 ;
    uuy2 = cy + range * height / width * 1000 ;
  }
  else
  {
    uuy1 = cy - range * 1000 ;
    uuy2 = cy + range * 1000 ;
    uux1 = cx - range * width / height * 1000 ;
    uux2 = cx + range * width / height * 1000 ;
  }
  
  pSurface->resize( uux1, uuy1, uux2, uuy2, false ) ;

  return true ;
}

// Generate a new track ID
int CTrackManager::getNewTrackID()
{
  ++m_trackNumber ;
  return m_trackNumber ;
}


// Generate a random integer
int CTrackManager::random( int min, int max )	  
{
  double rval = (double) rand() / (double) RAND_MAX ;
    return round( rval * ((max - min) + 0.99999) + min - 0.49999 );
}

// Round a double to the nearest integer
long CTrackManager::round( double val )
{
  return (val < 0.0) ? (long)(val - 0.5) : (long)(val + 0.5) ;
}

// Convert the latitude position into a degrees, minutes, seconds format
void CTrackManager::latitudeAsString( double latitude, char * buffer )
{
  char	  NS  = latitude < 0.0 ? 'S' : 'N';

  double  lat = fabs( latitude );
  
  int	  deg = static_cast<int>( lat );
  
  int	  min = static_cast<int>( (60.0 * lat) - (60.0 * deg) );
  
  double  sec = ( 3600.0 * lat ) - ( 3600.0 * deg ) - ( 60.0 * min );
  
  sprintf( buffer, "%3.2ld%\xba%3.2ld' %05.02lf\" %c", deg, min, sec, NS ) ;

}

// Convert the longitude position into a degrees, minutes, seconds format
void CTrackManager::longitudeAsString( double longitude, char * buffer )
{
  char	  EW  = longitude < 0.0 ? 'W' : 'E';
  
  double  lng = fabs( longitude );
  
  int	  deg = static_cast<int>(lng);
  
  int	  min = static_cast<int>( (60.0 * lng) - (60.0 * deg) );
  
  double  sec = ( 3600.0 * lng ) - ( 3600.0 * deg ) - ( 60.0 * min );

  sprintf( buffer, "%3.2ld%\xba%3.2ld' %05.02lf\" %c", deg, min, sec, EW ) ;

}

// Retrieve track hostility and type information and format into a string
void CTrackManager::platformDescription( CTrack * pTrack, char * buffer )
{
  switch( (int)pTrack->hostility() )
  {
  case HOSTILE:
    buffer[HOSTILITY] = 'H' ;
    break ;
  case FRIENDLY:
    buffer[HOSTILITY] = 'F' ;
    break ;
  case NEUTRAL:
    buffer[HOSTILITY] = 'N' ;
    break ;
  case UNKNOWN:
    buffer[HOSTILITY] = 'U' ;
    break ;
  }

  switch( (int)pTrack->trackType() )
  {
  case AIRCRAFT_CARRIER:
    buffer[TYPE] = 'R' ;
    break ;
  case BOMBER:
    buffer[TYPE] = 'B' ;
    break ;
  case DESTROYER:
    buffer[TYPE] = 'D' ;
    break ;
  case FRIGATE:
    buffer[TYPE] = 'F' ;
    break ;
  case GENERAL_AIR:
    buffer[TYPE] = 'A' ;
    break ;
  case HELICOPTER:
    buffer[TYPE] = 'Y' ;
    break ;
  case INTERCEPTOR:
    buffer[TYPE] = 'I' ;
    break ;
  case SUBMARINE:
    buffer[TYPE] = 'S' ;
    break ;
  case UNKNOWN_TRACK:
    buffer[TYPE] = 'U' ;
    break ;
  }

  buffer[PLATFORM_DESCRIPTION - 1] = '\0' ;

}

void CTrackManager::redesignateSelectedTrack( TRACK_TYPE newTrackType )
{
  // Retrieve a pointer to the object data layer
  CC2DemoDoc * pDoc = (CC2DemoDoc *)((CMainFrame *)AfxGetMainWnd())->GetActiveDocument() ;
  TSLObjectDataLayer * pLayer = pDoc->getObjectDataLayer() ;

  // Remove the track from the object data layer
  pLayer->removeDDO( m_selectedTrack ) ;

  CTrack * newTrack = 0 ;

  switch( newTrackType )
  {
    case AIRCRAFT_CARRIER :
      newTrack = new CAircraftCarrier( *m_selectedTrack ) ;
      break ;   
    case BOMBER :
      newTrack = new CBomber( *m_selectedTrack ) ;
      break ;   
    case DESTROYER :
      newTrack = new CDestroyer( *m_selectedTrack ) ;
      break ;   
    case FRIGATE :
      newTrack = new CFrigate( *m_selectedTrack ) ;
      break ;   
    case GENERAL_AIR :
      newTrack = new CGeneralAir( *m_selectedTrack ) ;
      break ;   
    case HELICOPTER :
      newTrack = new CHelicopter( *m_selectedTrack ) ;
      break ;   
    case INTERCEPTOR :
      newTrack = new CInterceptor( *m_selectedTrack ) ;
      break ;   
    case SUBMARINE :
      newTrack = new CSubmarine( *m_selectedTrack ) ;
      break ;   
  }

  if ( newTrack )
  {
    delete m_selectedTrack ;
    m_selectedTrack = newTrack ;
  }
  // Then add it back to the object data layer
  pLayer->addDDO( m_selectedTrack ) ;
  
}

