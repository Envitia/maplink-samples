/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include <stdint.h>

#ifdef _MSC_VER
# define NOMINMAX
# include <Windows.h>
#endif

#include <GL/gl.h>

#include "maplinkwidget.h"
#include "MapLink.h"
#include "MapLinkDrawing.h"
#include "TrackCustomDataLayer.h"
#include "Util.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <cstdlib>
#include <fstream>
#include <algorithm>

using namespace std;


static const int numWaypoints = 100;
static const double maxDistanceMeters = 111319.9 * 70.0; // Maximum distance between waypoints in meters, equal to 70 degrees at equator
static const double minDistanceMeters = 111319.9 * 25.0; // Minimum distance between waypoints in meters


static double baseLineAngle( const TSLCoord &lower, const TSLCoord &upper )
{
  double dy = (Double)upper.y() - (Double)lower.y();
  double dx = (Double)upper.x() - (Double)lower.x();
  double lengthSq = dx*dx + dy*dy;
  double length = sqrt( lengthSq );

  dx -= 1.0;								//generates an horizontal line 
  Double horizLengthSq = dx*dx + dy*dy;

  if( length == 0 )
    return 0;

  //Trigonometric formula:
  //A,B,C are the vertices of a triangle, a is the angle <(AB),(AC)>, then
  //sqrt(BC) = sqrt(AC) + sqrt(AB) - 2*(AB)(BC)cos(a)  
  double val = ( lengthSq + 1.0 - horizLengthSq ) / ( 2.0*length );

  // Handle rounding 
  if( val > 1.0 )
    val = 1.0;
  else if( val < -1.0 )
    val = -1.0;

  return acos( val );
}

TrackCustomDataLayer::TrackWaypoint::TrackWaypoint()
  : m_lat( 0.0 )
  , m_lon( 0.0 )
  , m_addedByUser( false )
{
}

TrackCustomDataLayer::TrackWaypoint::TrackWaypoint( double lat, double lon, bool addedByUser )
  : m_lat( lat )
  , m_lon( lon )
  , m_addedByUser( addedByUser )
{
}

TrackCustomDataLayer::TrackCustomDataLayer()
  : m_trackLatPos( 0.0 )
  , m_trackLonPos( 0.0 )
  , m_trackSpeed( 248.055556 ) // Crusing speed of a 747 in meters/second
  , m_trackTurnRate( 360.0 / 60.0 ) // Number of degrees per second the track is allowed to turn (defines turning circle)
  , m_distanceToNextWaypoint( 0.0 )
  , m_waypointAzimuth( 0.0 )
  , m_showTrackInfo( true )
  , m_trackInfoTextColour( TSLComposeRGB( 255, 255, 255 ) )
  , m_trackInfoTextStyle( 1 )
  , m_trackInfoTextSize( 16.0 )
  , m_trackColour( TSLComposeRGB( 255, 0, 0 ) )
  , m_trackStyle( 6001 )
  , m_trackSize( 50.0 )
  , m_destinationMarkerColour( TSLComposeRGB( 32, 226, 215 ) )
  , m_destinationMarkerStyle( 10 )
  , m_destinationMarkerSize( 25.0 )
{
  // Pick a random starting point - use noddy random seed initialisation
  time_t timer;
  srand( time( &timer ) );

  TrackWaypoint startPoint;
  startPoint.m_lon = 0.0;//(360.0 * (rand() / (double)RAND_MAX)) - 180.0;
  startPoint.m_lat = 52.0;//(180.0 * (rand() / (double)RAND_MAX)) - 90.0;
  // Vincenty doesn't seem to clamp to +-180/+-360, so we have to do that ourselves
  Util::normaliseLatLon( startPoint.m_lat, startPoint.m_lon );
  m_waypoints.push_back( startPoint );

  double currentPositionLon = startPoint.m_lon;
  double currentPositionLat = startPoint.m_lat;

  for( int i = 1; i < numWaypoints; ++i )
  {
    // Travel a random distance up to maxDistanceMeters in a random direction
    double randomAzimuth = 360.0 * ( rand() / (double)RAND_MAX );
    double randomDistance = std::max( maxDistanceMeters * ( rand() / (double)RAND_MAX ), minDistanceMeters );

    TrackWaypoint nextPoint;
    TSLCoordinateConverter::vincentyDirect( currentPositionLat, currentPositionLon, randomAzimuth,
      randomDistance, nextPoint.m_lat, nextPoint.m_lon );
    Util::normaliseLatLon( nextPoint.m_lat, nextPoint.m_lon );
    m_waypoints.push_back( nextPoint );

    currentPositionLon = nextPoint.m_lon;
    currentPositionLat = nextPoint.m_lat;
  }

  // Set the track to start at the first waypoint
  m_currentWaypoint = m_waypoints.begin();
  m_trackLatPos = m_currentWaypoint->m_lat;
  m_trackLonPos = m_currentWaypoint->m_lon;

  ++m_currentWaypoint;

  // Determine the bearing and distance to the next waypoint
  m_distanceToNextWaypoint = TSLCoordinateConverter::vincentyInverse( m_trackLatPos, m_trackLonPos,
    m_currentWaypoint->m_lat, m_currentWaypoint->m_lon,
    m_waypointAzimuth );

  // Load the rendering information from the ini file if present
  TSLProfileHelper::setDefaultSection( "rendering" );
  char tempBuf[64] = { '\0' };

  // Track text information
  if( TSLProfileHelper::lookupProfile( "trackinfotextcolour", tempBuf, sizeof( tempBuf ) ) )
  {
    unsigned int r = 255, g = 255, b = 255;
    sscanf( tempBuf, "%u,%u,%u", &r, &g, &b );
    m_trackInfoTextColour = TSLComposeRGB( r, g, b );
  }
  else
  {
    unsigned int r = 255, g = 255, b = 255;
    m_trackInfoTextColour = TSLComposeRGB( r, g, b );
  }
  TSLProfileHelper::lookupProfile( "trackinfotextsize", &m_trackInfoTextSize, 16.0 );
  TSLProfileHelper::lookupProfile( "trackinfotextstyle", &m_trackInfoTextStyle, 1 );

  // Track
  if( TSLProfileHelper::lookupProfile( "trackcolour", tempBuf, sizeof( tempBuf ) ) )
  {
    unsigned int r = 255, g = 0, b = 0;
    sscanf( tempBuf, "%u,%u,%u", &r, &g, &b );
    m_trackColour = TSLComposeRGB( r, g, b );
  }
  else
  {
    unsigned int r = 255, g = 0, b = 0;
    m_trackColour = TSLComposeRGB( r, g, b );
  }
  TSLProfileHelper::lookupProfile( "tracksize", &m_trackSize, 50.0 );
  TSLProfileHelper::lookupProfile( "trackstyle", &m_trackStyle, 6001 );

  // Destination marker
  if( TSLProfileHelper::lookupProfile( "destinationmarkercolour", tempBuf, sizeof( tempBuf ) ) )
  {
    unsigned int r = 32, g = 226, b = 215;
    sscanf( tempBuf, "%u,%u,%u", &r, &g, &b );
    m_destinationMarkerColour = TSLComposeRGB( r, g, b );
  }
  else
  {
    unsigned int r = 32, g = 226, b = 215;
    m_destinationMarkerColour = TSLComposeRGB( r, g, b );
  }
  TSLProfileHelper::lookupProfile( "destinationmarkersize", &m_destinationMarkerSize, 25.0 );
  TSLProfileHelper::lookupProfile( "destinationmarkerstyle", &m_destinationMarkerStyle, 10 );
}

TrackCustomDataLayer::~TrackCustomDataLayer()
{
}

bool TrackCustomDataLayer::drawLayer( TSLRenderingInterface *renderingInterface, const TSLEnvelope* extent, TSLCustomDataLayerHandler& layerHandler )
{
  const TSLDrawingSurface *surface = layerHandler.drawingSurface();

  // Show a marker for the current waypoint destination
  TSLCoord markerPos;
  if( surface->latLongToTMC( m_currentWaypoint->m_lat, m_currentWaypoint->m_lon, &markerPos.m_x, &markerPos.m_y, true ) )
  {
    renderingInterface->setupSymbolAttributes( m_destinationMarkerStyle, m_destinationMarkerColour, m_destinationMarkerSize,
      TSLDimensionUnitsPixels );
    renderingInterface->drawSymbol( markerPos );
  }

  TSLCoord trackPos;
  if( surface->latLongToTMC( m_trackLatPos, m_trackLonPos, &trackPos.m_x, &trackPos.m_y, true ) )
  {
    // Determine the rotation of the symbol so that it points in the direction it's moving - as it flies
    // great circles this may not always be pointing exactly at the destination marker
    double directionLat, directionLon;
    TSLCoordinateConverter::vincentyDirect( m_trackLatPos, m_trackLonPos, m_waypointAzimuth,
      1000.0, directionLat, directionLon );
    TSLCoord directionPos;
    if( !surface->latLongToTMC( directionLat, directionLon, &directionPos.m_x, &directionPos.m_y, true ) )
    {
      directionPos = markerPos;
    }

    double angleToDestination = baseLineAngle( trackPos, directionPos );
    // baseLineAngle assumes trackPos.y < markerPos.y, so handle the case where it doesn't
    if( directionPos.y() < trackPos.y() )
    {
      angleToDestination = ( 2.0 * M_PI ) - angleToDestination;
    }

    // By default the plane symbol faces left, so the additional -PI is necessary to get rotations from horizontal, which is what
    // baseLineAngle returns
    renderingInterface->setupSymbolAttributes( m_trackStyle, m_trackColour, m_trackSize, TSLDimensionUnitsPixels, 1, 2000,
      -M_PI + angleToDestination );
    renderingInterface->drawSymbol( trackPos );

    if( m_showTrackInfo )
    {
      // Draw some text to say where the plane is and how far it has to go
      char planePosStr[64];
      TSL_SNPRINTF( planePosStr, sizeof( planePosStr ), "Lat %.2f  Lon %.2f", m_trackLatPos, m_trackLonPos );
      renderingInterface->setupTextAttributes( m_trackInfoTextStyle, m_trackInfoTextColour, m_trackInfoTextSize, TSLDimensionUnitsPixels,
        1, 2000, 0.0, -m_trackSize / 2.0, TSLDimensionUnitsPixels,
        0.0, TSLHorizontalAlignmentCentre, TSLVerticalAlignmentFullTop,
        TSLTextBackgroundModeHalo, 0, TSLComposeRGB( 0, 0, 0 ), TSLComposeRGB(0, 0, 0), TSLTextRotationDisabled);
      renderingInterface->drawText( trackPos, planePosStr );

      char remainingDistanceStr[64];
      TSL_SNPRINTF( remainingDistanceStr, sizeof( remainingDistanceStr ), " %.0f meters to destination", m_distanceToNextWaypoint );
      renderingInterface->setupTextAttributes( m_trackInfoTextStyle, m_trackInfoTextColour, m_trackInfoTextSize, TSLDimensionUnitsPixels,
        1, 2000, 0.0, -m_trackSize / 2.0 - m_trackInfoTextSize,
        TSLDimensionUnitsPixels, 0.0, TSLHorizontalAlignmentCentre, TSLVerticalAlignmentFullTop,
        TSLTextBackgroundModeHalo, 0, TSLComposeRGB( 0, 0, 0 ), TSLComposeRGB(0, 0, 0), TSLTextRotationDisabled);
      renderingInterface->drawText( trackPos, remainingDistanceStr );
    }
  }

  return true;
}

void TrackCustomDataLayer::updatePositions( double secsSinceLastFrame, double timeMultiplier )
{
  // See how far we got based on how long since the last frame
  double distanceCovered = m_trackSpeed * secsSinceLastFrame * timeMultiplier;

  TSLCoordinateConverter::vincentyDirect( m_trackLatPos, m_trackLonPos, m_waypointAzimuth,
    distanceCovered, m_trackLatPos, m_trackLonPos );
  // Vincenty doesn't seem to clamp to +-180/+-360, so we have to do that ourselves
  Util::normaliseLatLon( m_trackLatPos, m_trackLonPos );

  m_distanceToNextWaypoint -= distanceCovered;

  //double oldAzimuth = m_waypointAzimuth;

  if( m_distanceToNextWaypoint <= 0.0 )
  {
    // Move to the next waypoint
    if( m_currentWaypoint->m_addedByUser )
    {
      // This was a user-added waypoint, these are only travelled to once so delete it now we're there
      m_currentWaypoint = m_waypoints.erase( m_currentWaypoint );
    }
    else
    {
      ++m_currentWaypoint;
    }

    if( m_currentWaypoint == m_waypoints.end() )
    {
      // Looped through the entire waypoint list, start from the beginning
      m_currentWaypoint = m_waypoints.begin();
    }

    m_distanceToNextWaypoint = TSLCoordinateConverter::vincentyInverse( m_trackLatPos, m_trackLonPos,
      m_currentWaypoint->m_lat, m_currentWaypoint->m_lon,
      m_waypointAzimuth );
  }
  else
  {
    // Determine the new bearing to where we want to go
    m_distanceToNextWaypoint = TSLCoordinateConverter::vincentyInverse( m_trackLatPos, m_trackLonPos,
      m_currentWaypoint->m_lat, m_currentWaypoint->m_lon,
      m_waypointAzimuth );
  }

  // Turn rate clamping - disabled since it makes the track flicker oddly at high time multipliers
  /*if( fabs( oldAzimuth - m_waypointAzimuth ) > (m_trackTurnRate * secsSinceLastFrame * timeMultiplier) )
  {
    // Clamp the azimuth to the maximum allowed turn rate
    m_waypointAzimuth = oldAzimuth - (Util::sign( oldAzimuth - m_waypointAzimuth ) * m_trackTurnRate * secsSinceLastFrame * timeMultiplier);
  }*/
}

void TrackCustomDataLayer::addWaypoint( double lat, double lon )
{
  // Note: This is only invoked from the message pump thread, which is also the draw thread. It's therefore
  // safe to modify the waypoint list without having to do any locking.
  TrackWaypoint newUserPoint( lat, lon, true );

  // Make the new waypoint the current target waypoint
  m_currentWaypoint = m_waypoints.insert( m_currentWaypoint, newUserPoint );


  // Don't directly update our current heading, let the track turn normally
  double tempAzimuth;
  m_distanceToNextWaypoint = TSLCoordinateConverter::vincentyInverse( m_trackLatPos, m_trackLonPos,
    m_currentWaypoint->m_lat, m_currentWaypoint->m_lon,
    tempAzimuth );
}

bool TrackCustomDataLayer::loadWaypointList( const string &filename )
{
#if 0
  ifstream inputStream( filename );
  if( !inputStream )
  {
    return false;
  }

  // Don't immediately trash the current waypoint list in case the new list isn't valid
  list< TrackWaypoint > newWaypoints;

  while( !inputStream.eof() )
  {
    string currentLine;
    std::getline( inputStream, currentLine, '\n' );
    if( currentLine.empty() || currentLine[0] == ';' )
    {
      // Lines that start with a semicolon are comments
      continue;
    }

    // Should just have a comma seperated lat/lon position
    double lat = 0.0, lon = 0.0;
    if( sscanf( currentLine.c_str(), "%lf,%lf", &lat, &lon ) == 2 &&
      lat >= -90.0 && lat <= 90.0 && lon >= -180.0 && lon <= 180.0 )
    {
      newWaypoints.push_back( TrackWaypoint( lat, lon ) );
    }
  }

  if( newWaypoints.size() >= 2 )
  {
    // This list is valid - set the track to be at the first point
    m_waypoints = newWaypoints;
    m_currentWaypoint = m_waypoints.begin();
    m_trackLatPos = m_currentWaypoint->m_lat;
    m_trackLonPos = m_currentWaypoint->m_lon;

    ++m_currentWaypoint;
    m_distanceToNextWaypoint = TSLCoordinateConverter::vincentyInverse( m_trackLatPos, m_trackLonPos,
      m_currentWaypoint->m_lat, m_currentWaypoint->m_lon,
      m_waypointAzimuth );
    return true;
  }
#endif
  return false;
}
