#include "track.h"
#include <cstdlib>
#include <cmath>
#include "MapLink.h"
#include "tslapp6ahelper.h"

#ifdef _MSC_VER
# define snprintf _snprintf
#endif

double Track::m_minTargetDistance = 100.0; // Tracks must move at least 100m before turning
double Track::m_maxTargetDistance = 10000.0; // Tracks cannot move more than 10,000m before turning
double Track::m_maxHeadingDelta = 1.0; // Tracks cannot turn more than 1 degree at a time
double Track::m_historyDropDistance = 500.0; // Tracks record their positions once every 500m
size_t Track::m_maxHistoryPoints = 10; // The number of historical positions for a track to remember

Track::DisplayInfo::DisplayInfo()
  : m_x( 0 )
  , m_y( 0 )
  , m_lat( 0.0 )
  , m_lon( 0.0 )
  , m_symbolKey( 0 )
  , m_size( 0 )
  , m_heading( 0.0 )
  , m_speed( 0.0 )
  , m_altitude( 0.0 )
  , m_sinDisplayHeading( 0.0 )
  , m_cosDisplayHeading( 0.0 )
  , m_hostility( TSLAPP6ASymbol::HostilityNone )
  , m_speedLabel( NULL )
  , m_positionLabel( NULL )
{
}

Track::DisplayInfo::DisplayInfo( const DisplayInfo &rhs )
  : m_x( rhs.m_x )
  , m_y( rhs.m_y )
  , m_lat( rhs.m_lat )
  , m_lon( rhs.m_lon )
  , m_symbolKey( rhs.m_symbolKey )
  , m_size( rhs.m_size )
  , m_heading( rhs.m_heading )
  , m_speed( rhs.m_speed )
  , m_altitude( rhs.m_altitude )
  , m_sinDisplayHeading( rhs.m_sinDisplayHeading )
  , m_cosDisplayHeading( rhs.m_cosDisplayHeading )
  , m_hostility( rhs.m_hostility )
  , m_speedLabel( NULL )
  , m_positionLabel( NULL )
{
  if( rhs.m_speedLabel )
  {
    m_speedLabel = reinterpret_cast<TSLText*>( rhs.m_speedLabel->clone() );
  }
  if( rhs.m_positionLabel )
  {
    m_positionLabel = reinterpret_cast<TSLText*>( rhs.m_positionLabel->clone() );
  }
}

Track::DisplayInfo::~DisplayInfo()
{
  if( m_speedLabel )
  {
    m_speedLabel->destroy();
  }
  if( m_positionLabel )
  {
    m_positionLabel->destroy();
  }
}

Track::Track( const TSLAPP6ASymbol &type, TSLAPP6AHelper *helper )
  : m_type( type )
  , m_speed( 0.0 )
  , m_heading( ( rand() / (double)RAND_MAX ) * 360.0 )
  , m_displayHeading( 0.0 )
  , m_altitude( 0.0 )
  , m_targetDistance( 0.0 )
  , m_distanceFromLastHistoryPoint( 0.0 )
  , m_currentHistoryIndex( 0 )
  , m_speedLabel( NULL )
  , m_positionLabel( NULL )
  , m_x( 0 )
  , m_y( 0 )
  , m_lat( 0.0 )
  , m_lon( 0.0 )
{
  // Choose sensible speed and altitudes based on what type of track this is
  switch( m_type.type() )
  {
  case TSLAPP6ASymbol::TypeAirSpace:
    m_speed = 250.0 + ( -50.0 + ( rand() / (double)RAND_MAX ) * 100.0 );
    m_altitude = 3000.0 + ( rand() / (double)RAND_MAX ) * 2000.0; // Variable altitudes from 10,000 feet to 16,000 feet
    break;

  case TSLAPP6ASymbol::TypeSeaSurface:
    m_speed = 15.4 + ( -10.0 + ( rand() / (double)RAND_MAX ) * 20.0 ); // Approximately 30 knots base
    break;

  case TSLAPP6ASymbol::TypeUnit:
    m_speed = 26.82 + ( -20.0 + ( rand() / (double)RAND_MAX ) * 40.0 ); // Approximately 60mph base 
    break;

  case TSLAPP6ASymbol::TypeSpecialOperations:
    m_speed = 1.4 + ( -1.0 + ( rand() / (double)RAND_MAX ) * 2.0 ); // Walking speed
    break;

  case TSLAPP6ASymbol::TypeSubSurface:
    m_speed = 11.8 + ( -7.0 + ( rand() / (double)RAND_MAX ) * 14.0 ); // Approximately 23 knots base
    m_altitude = -200.0 - ( rand() / (double)RAND_MAX ) * 500.0; // Variable depths from -200m to -800m
    break;

  case TSLAPP6ASymbol::TypeEquipment:
  case TSLAPP6ASymbol::TypeInstallation: // Installations are immobile, so leave them with speed of 0
  default:
    break;
  }

  m_type.height( 75.0 );
  m_type.heightType( TSLDimensionUnitsPixels );

  m_historyPoints.reserve( m_maxHistoryPoints );

  // Get TSLText objects for the annotations that we will dynamically update. These appear in a specific
  // position relative to the symbol, so using the TSLAPP6AHelper to create these ensures they are automatically
  // in the right place.
  TSLFeatureID speedLabelID = 1000;
  TSLFeatureID positionLabelID = 1001;

  m_type.speed( "speed", speedLabelID );
  m_type.latAndLong( "position", positionLabelID );

  TSLEntitySet *symbolSet = helper->getSymbolAsEntitySet( &m_type );
  if( symbolSet )
  {
    // Extract the TSLText objects that correspond to the fields we're interested in updating dynamically
    int numEntities = symbolSet->size();
    for( int i = numEntities - 1; i >= 0; --i )
    {
      TSLEntity *entity = ( *symbolSet )[i];
      if( entity->featureID() == speedLabelID )
      {
        // This entity is the annotation for the track's speed
        m_speedLabel = reinterpret_cast<TSLText*>( entity );
        symbolSet->removeEntity( m_speedLabel );

        // Apply halo rendering to make the label easier to read
        m_speedLabel->setRendering( TSLRenderingAttributeTextColour, TSLComposeRGB( 255, 255, 255 ) );
        m_speedLabel->setRendering( TSLRenderingAttributeTextBackgroundMode, TSLTextBackgroundModeHalo );
        m_speedLabel->setRendering( TSLRenderingAttributeTextBackgroundColour, TSLComposeRGB( 0, 0, 0 ) );
        m_speedLabel->setRendering( TSLRenderingAttributeTextRotatable, 0 );
      }
      else if( entity->featureID() == positionLabelID )
      {
        // This entity is the annotation for the track's speed
        m_positionLabel = reinterpret_cast<TSLText*>( entity );
        symbolSet->removeEntity( m_positionLabel );

        m_positionLabel->setRendering( TSLRenderingAttributeTextColour, TSLComposeRGB( 255, 255, 255 ) );
        m_positionLabel->setRendering( TSLRenderingAttributeTextBackgroundMode, TSLTextBackgroundModeHalo );
        m_positionLabel->setRendering( TSLRenderingAttributeTextBackgroundColour, TSLComposeRGB( 0, 0, 0 ) );
        m_positionLabel->setRendering( TSLRenderingAttributeTextRotatable, 0 );
      }
    }

    symbolSet->destroy();
  }
}

Track::~Track()
{
  if( m_speedLabel )
  {
    m_speedLabel->destroy();
  }
  if( m_positionLabel )
  {
    m_positionLabel->destroy();
  }
}

void Track::updatePosition( double elapsedSeconds, const TSLCoordinateSystem *coordSys, const TSLEnvelope &mapExtent, Track::DisplayInfo &displayInfo,
  TrackAnnotationLevel annotationLevel )
{
  double distanceToMove = m_speed * elapsedSeconds;

  if( m_targetDistance - distanceToMove <= 0.0 )
  {
    // The track has reached it's target point, alter it's heading and give it a new target point to
    // move to
    double headingDeltaDegrees = -m_maxHeadingDelta + ( rand() / (double)RAND_MAX ) * m_maxHeadingDelta * 2.0;
    m_heading += headingDeltaDegrees/* * (M_PI / 180.0)*/;

    m_targetDistance = m_minTargetDistance + ( rand() / (double)RAND_MAX ) * ( m_maxTargetDistance - m_minTargetDistance );
  }
  else
  {
    m_targetDistance -= distanceToMove;
  }

  if( distanceToMove > 0.0 )
  {
    double newLat, newLon;
    m_heading = TSLCoordinateConverter::vincentyDirect( m_lat, m_lon, m_heading, distanceToMove, newLat, newLon ) + 180.0;

    m_lat = newLat;
    m_lon = newLon;

    TSLTMC newMapPosX, newMapPosY;
    if( coordSys->latLongToTMC( newLat, newLon, &newMapPosX, &newMapPosY ) )
    {
      m_x = newMapPosX;
      m_y = newMapPosY;
    }

    // Calculate the angle of the heading indicator relative to the map (which may be different to the track's heading in lat/lon depending
    // on the map projection). We need to ensure we use a target position far enough away from our current position that it gives us 
    // a sufficiently different TMC position to use when calculating the angle.
    double futureLat, futureLon;
    TSLCoordinateConverter::vincentyDirect( m_lat, m_lon, m_heading, 1000.0, futureLat, futureLon );

    TSLTMC futureMapPosX, futureMapPosY;
    if( coordSys->latLongToTMC( futureLat, futureLon, &futureMapPosX, &futureMapPosY ) )
    {
      m_displayHeading = atan2( (double)( futureMapPosX - m_x ), (double)( futureMapPosY - m_y ) );
    }

    m_distanceFromLastHistoryPoint += distanceToMove;
    if( m_distanceFromLastHistoryPoint >= m_historyDropDistance )
    {
      // The track has moved far enough for us to record a historical location, add the current position to
      // the history list
      m_distanceFromLastHistoryPoint = 0.0;

      if( m_currentHistoryIndex >= m_historyPoints.size() )
      {
        m_historyPoints.push_back( TSLCoord( m_x, m_y ) );
      }
      else
      {
        m_historyPoints[m_currentHistoryIndex] = TSLCoord( m_x, m_y );
      }

      if( ++m_currentHistoryIndex >= m_maxHistoryPoints )
      {
        m_currentHistoryIndex = 0;
      }
    }

    // Ensure that the track doesn't move off the edges of the map by reflecting it off the map's extent
    if( m_x < mapExtent.bottomLeft().x() )
    {
      m_heading += 180.0;
      m_x = mapExtent.bottomLeft().x();
    }
    else if( m_x > mapExtent.topRight().x() )
    {
      m_heading -= 180.0;
      m_x = mapExtent.topRight().x();
    }
    if( m_y < mapExtent.bottomLeft().y() )
    {
      m_heading -= 180.0;
      m_y = mapExtent.bottomLeft().y();
    }
    else if( m_y > mapExtent.topRight().y() )
    {
      m_heading -= 180.0;
      m_y = mapExtent.topRight().y();
    }

    // Re-normalise the track's heading to be in the range -180 to 180 degrees
    while( m_heading < -180.0 )
    {
      m_heading += 360.0;
    }
    while( m_heading > 180.0 )
    {
      m_heading -= 360.0;
    }
  }

  displayInfo.m_x = m_x;
  displayInfo.m_y = m_y;
  displayInfo.m_lat = m_lat;
  displayInfo.m_lon = m_lon;
  displayInfo.m_size = m_type.height();
  displayInfo.m_symbolKey = m_type.key();
  displayInfo.m_hostility = m_type.hostility();
  displayInfo.m_heading = m_heading;
  displayInfo.m_displayHeading = m_displayHeading;
  displayInfo.m_sinDisplayHeading = sin( m_displayHeading );
  displayInfo.m_cosDisplayHeading = cos( m_displayHeading );
  displayInfo.m_historyPoints = m_historyPoints;
  displayInfo.m_speed = m_speed;
  displayInfo.m_altitude = m_altitude;

  if( annotationLevel >= AnnotationLow )
  {
    // At low and above we display the speed and position annotations for the APP6A symbol
    char speedString[64];
    snprintf( speedString, sizeof( speedString ), "%.1lf m/s", m_speed );

    if( displayInfo.m_speedLabel )
    {
      displayInfo.m_speedLabel->value( speedString );
    }
    else
    {
      displayInfo.m_speedLabel = reinterpret_cast<TSLText*>( m_speedLabel->clone() );
      displayInfo.m_speedLabel->value( speedString );
    }

    // Position the label in the correct place relative to where it should appear around the actual symbol
    displayInfo.m_speedLabel->position( m_speedLabel->position() );
    displayInfo.m_speedLabel->translate( m_x, m_y );

    // Display positions in lat/lon
    char positionString[64];
    //double lat, lon;
    //coordSys->TMCToLatLong( m_x, m_y, &lat, &lon );
    snprintf( positionString, sizeof( positionString ), "%.2lf %.2lf", m_lat, m_lon );

    if( displayInfo.m_positionLabel )
    {
      displayInfo.m_positionLabel->value( positionString );
    }
    else
    {
      displayInfo.m_positionLabel = reinterpret_cast<TSLText*>( m_positionLabel->clone() );
      displayInfo.m_positionLabel->value( positionString );
    }
    displayInfo.m_positionLabel->position( m_positionLabel->position() );
    displayInfo.m_positionLabel->translate( m_x, m_y );
  }
  else
  {
    if( displayInfo.m_speedLabel )
    {
      displayInfo.m_speedLabel->destroy();
    }
    if( displayInfo.m_positionLabel )
    {
      displayInfo.m_positionLabel->destroy();
    }

    displayInfo.m_speedLabel = NULL;
    displayInfo.m_positionLabel = NULL;
  }
}

bool Track::intersects( TSLTMC x, TSLTMC y, double tmcPerDU )
{
  // Calculate the display envelope of this track based on the TMC per pixel size given
  TSLEnvelope displayExtent( m_x, m_y, m_x, m_y );
  displayExtent.expand( m_type.height() / 2.0 * tmcPerDU );

  return displayExtent.contains( TSLCoord( x, y ) );
}

void Track::setPosition( double lat, double lon, const TSLCoordinateSystem *coordSys )
{
  m_lat = lat;
  m_lon = lon;

  // Ensure the TMC position is up to date
  coordSys->latLongToTMC( m_lat, m_lon, &m_x, &m_y );

  // Work out the initial display heading
  double futureLat, futureLon;
  TSLCoordinateConverter::vincentyDirect( m_lat, m_lon, m_heading, 100.0, futureLat, futureLon );

  TSLTMC futureMapPosX, futureMapPosY;
  if( coordSys->latLongToTMC( futureLat, futureLon, &futureMapPosX, &futureMapPosY ) )
  {
    m_displayHeading = atan2( (double)( futureMapPosX - m_x ), (double)( futureMapPosY - m_y ) );
  }
}
