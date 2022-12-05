/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "trackmanager.h"
#include "MapLink.h"
#include "MapLinkDrawing.h"
#include "tslapp6ahelper.h"

#include "trackupdater.h"

#include <set>

#ifndef SIZE_MAX
# define SIZE_MAX  (-1)
#endif

using std::set;

TrackUpdater::TrackUpdater( TrackManager *manager )
  : m_manager( manager )
  , m_numUpdates( 0 )
  , m_totalNumUpdates( 0 )
  , m_cumulativeTime( 0 )
  , m_timeCompressionFactor( 1.0 )
  , m_unusedDisplayData( NULL )
  , m_updateTrigger( NULL )
  , m_currentTrackSelection( SIZE_MAX ) // An Invalid index mean no selection
  , m_annotationLevel( AnnotationNone )
  , m_inhibitUpdates( true )
  , m_helper( new TSLAPP6AHelper() )
  , m_coordSys( NULL )
{
#ifndef WIN32
# if _POSIX_TIMERS > 0
  m_clockType = sysconf( _POSIX_MONOTONIC_CLOCK ) >= 0 ? CLOCK_MONOTONIC : CLOCK_REALTIME;
# else
  m_clockType = CLOCK_REALTIME;
# endif
#else
  QueryPerformanceFrequency( &m_counterFrequency );
#endif
}

TrackUpdater::~TrackUpdater()
{
  // Clean up
  delete m_unusedDisplayData;
  delete m_updateTrigger;

  size_t numTracks = m_tracks.size();
  for( size_t i = 0; i < numTracks; ++i )
  {
    delete m_tracks[i];
  }

  if( m_coordSys )
  {
    m_coordSys->destroy();
  }
}

void TrackUpdater::updateTracks()
{
  // Identify how long it has been since the last time the track positions were updated
  double secsSinceLastUpdate = 0.0, secsSinceStart = 0.0;
  if( !m_inhibitUpdates )
  {
#ifdef _MSC_VER
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter( &currentTime );

    secsSinceLastUpdate = ( currentTime.QuadPart - m_lastUpdateTime.QuadPart ) / (double)m_counterFrequency.QuadPart;
    secsSinceStart = ( currentTime.QuadPart - m_startTime.QuadPart ) / (double)m_counterFrequency.QuadPart;

    m_lastUpdateTime = currentTime;
#else
    timespec currentTime;
    clock_gettime( m_clockType, &currentTime );

    // Work out the time difference
    timespec updateTime;
    updateTime.tv_sec = currentTime.tv_sec - m_lastUpdateTime.tv_sec;
    updateTime.tv_nsec = currentTime.tv_nsec - m_lastUpdateTime.tv_nsec;
    if( updateTime.tv_nsec < 0 )
    {
      --updateTime.tv_sec;
      updateTime.tv_nsec += 1000000000;
    }

    timespec totalTime;
    totalTime.tv_sec = currentTime.tv_sec - m_startTime.tv_sec;
    totalTime.tv_nsec = currentTime.tv_nsec - m_startTime.tv_nsec;
    if( totalTime.tv_nsec < 0 )
    {
      --totalTime.tv_sec;
      totalTime.tv_nsec += 1000000000;
    }

    secsSinceLastUpdate = updateTime.tv_sec + ( updateTime.tv_nsec / 1000000000.0 );
    secsSinceStart = totalTime.tv_sec + ( totalTime.tv_nsec / 1000000000.0 );

    m_lastUpdateTime = currentTime;
#endif
  }

  double elapsedSeconds = secsSinceLastUpdate * m_timeCompressionFactor;

  // Get the display data structure to populate. If there is an existing one that is no longer used by the
  // drawing thread, reuse it. Otherwise create a new one.
  TrackManager::DisplayInfo *oldDisplayData = m_manager->m_previousDisplayInfo.fetchAndStoreOrdered( NULL );
  if( !oldDisplayData )
  {
    oldDisplayData = m_unusedDisplayData;
    m_unusedDisplayData = NULL;
  }
  if( !oldDisplayData )
  {
    oldDisplayData = new TrackManager::DisplayInfo();
  }

  oldDisplayData->m_selectedTrack = m_currentTrackSelection;
  oldDisplayData->m_annotationLevel = m_annotationLevel;

  // Update the positions of all the tracks
  size_t numTracks = m_tracks.size();
  oldDisplayData->m_tracks.resize( numTracks );
  for( size_t i = 0; i < numTracks; ++i )
  {
    Track *currentTrack = m_tracks[i];
    currentTrack->updatePosition( elapsedSeconds, m_coordSys, m_mapExtent, oldDisplayData->m_tracks[i], m_annotationLevel );
  }

  // Update the current/average performance counter that records how often we are updating track positions
  ++m_numUpdates;
  ++m_totalNumUpdates;
  m_cumulativeTime += secsSinceLastUpdate;
  if( m_cumulativeTime >= 1.0 )
  {
    setTrackUpdateRate( m_numUpdates / m_cumulativeTime, m_totalNumUpdates / secsSinceStart );
    m_numUpdates = 0;
    m_cumulativeTime = 0;
  }

  // Send the completed display information to the draw thread to be used when it next updates.
  TrackManager::DisplayInfo *defunctInfo = m_manager->m_currentDisplayInfo.fetchAndStoreOrdered( oldDisplayData );
  if( m_unusedDisplayData )
  {
    // We already have some old display data to recycle, clean up the one we just got as it's not required.
    delete defunctInfo;
  }
  else
  {
    // Store the old display data for reuse in future in order to avoid always reallocating the display data
    // structures.
    m_unusedDisplayData = defunctInfo;
  }
}

void TrackUpdater::createTracks( quint32 numTracks, quint32 numTrackTypes )
{
  if( numTracks < m_tracks.size() )
  {
    // Remove tracks until we are down to the requested number
    for( size_t i = m_tracks.size() - 1; i > numTracks; --i )
    {
      delete m_tracks[i];
    }
    m_tracks.resize( numTracks );
  }
  else
  {
    // We need to create additional tracks up to the requested number. Before we can do that,
    // we want to define what each of the possible track types are. Each individual track
    // can then be chosen from these types;
    vector< TSLAPP6ASymbol > trackTypes;
    trackTypes.resize( numTrackTypes );
    int numAvailableTypes = m_helper->numOfSymbols();
    for( size_t i = 0; i < numTrackTypes; ++i )
    {
      int typeIndex = 0;
      do
      {
        typeIndex = ( ( rand() / (double)RAND_MAX ) * ( numAvailableTypes - 1 ) ) + 0.5;
        m_helper->getSymbol( typeIndex, trackTypes[i] );
      } while( trackTypes[i].type() == TSLAPP6ASymbol::TypeNone ||
        trackTypes[i].type() == TSLAPP6ASymbol::TypeHeader || // Don't include headers as valid selections as they don't have a visualisation
        trackTypes[i].type() == TSLAPP6ASymbol::TypeEquipment );
    }

    // Now we have a set of track types available, generate the requested number of tracks. Each track will
    // be randomly selected to be one of the types above.
    for( size_t i = m_tracks.size(); i < numTracks; ++i )
    {
      int trackType = ( ( rand() / (double)RAND_MAX ) * ( trackTypes.size() - 1 ) ) + 0.5;

      TSLAPP6ASymbol trackSymbol( trackTypes[trackType] );

      // Choose a hostility for the track
      TSLAPP6ASymbol::HostilityEnum hostilityTypes[] = { TSLAPP6ASymbol::HostilityFriend, TSLAPP6ASymbol::HostilityHostile, TSLAPP6ASymbol::HostilityNeutral,
                                                         TSLAPP6ASymbol::HostilityUnknown, TSLAPP6ASymbol::HostilitySuspect,
                                                         TSLAPP6ASymbol::HostilityAssumedFriend };
      int hostilityType = ( rand() / (double)RAND_MAX ) * ( ( sizeof( hostilityTypes ) / sizeof( TSLAPP6ASymbol::HostilityEnum ) ) - 1 );
      trackSymbol.hostility( hostilityTypes[hostilityType] );

      m_tracks.push_back( new Track( trackSymbol, m_helper ) );

      // Position the new track at a random latitude/longitude point that is valid for the currently loaded map
      bool validPosition = false;
      double lat = 0.0, lon = 0.0;
      do
      {
        lat = -90.0 + ( rand() / (double)RAND_MAX ) * 180.0;
        lon = -180.0 + ( rand() / (double)RAND_MAX ) * 360.0;

        // Validate that this position is legitimate for the map
        TSLTMC x, y;
        validPosition = m_coordSys->latLongToTMC( lat, lon, &x, &y );

        if( !m_mapExtent.contains( TSLCoord( x, y ) ) )
        {
          // The point is valid for the projection, but is not within the extent of the map
          validPosition = false;
        }
      } while( !validPosition );

      m_tracks.back()->setPosition( lat, lon, m_coordSys );
    }
  }

  // Clear the current track selection, if any
  m_currentTrackSelection = SIZE_MAX;

  updateTracks(); // Update the display data to include the new tracks
  trackSelectionStatusChanged( false );
}

void TrackUpdater::setSimulationTimeCompression( double factor )
{
  m_timeCompressionFactor = factor;
}

void TrackUpdater::selectTrack( qint32 x, qint32 y, double tmcPerDU )
{
  size_t numTracks = m_tracks.size();
  for( size_t i = 0; i < numTracks; ++i )
  {
    // Run backwards through the tracks as tracks are displayed in the order they are present in the vector.
    // Therefore by working backwards we ensure that the track that appears on top is the one selected.
    Track *currentTrack = m_tracks[numTracks - i - 1];

    if( currentTrack->intersects( x, y, tmcPerDU ) )
    {
      m_currentTrackSelection = numTracks - i - 1;
      updateTracks(); // Update the display data to include the new track selection
      trackSelectionStatusChanged( true );
      return;
    }
  }

  // No track at this position, set an invalid index to indicate no selection
  m_currentTrackSelection = SIZE_MAX;
  updateTracks(); // Update the display data to clear the track selection
  trackSelectionStatusChanged( false );
}

void TrackUpdater::clearTrackSelection()
{
  // No track at this position, set an invalid index to indicate no selection
  m_currentTrackSelection = SIZE_MAX;
  updateTracks(); // Update the display data to clear the track selection
  trackSelectionStatusChanged( m_currentTrackSelection < m_tracks.size() );
}

void TrackUpdater::changeTrackHostility( quint32 trackID, qint32 newHostility )
{
  TSLAPP6ASymbol::HostilityEnum hostility = static_cast<TSLAPP6ASymbol::HostilityEnum>( newHostility );

  if( trackID < m_tracks.size() )
  {
    m_tracks[trackID]->setHostility( hostility );
    updateTracks(); // Update the display data to show the new hostility
    trackSelectionStatusChanged( m_currentTrackSelection < m_tracks.size() );
  }
}

void TrackUpdater::setTrackAnnotationLevel( qint32 level )
{
  m_annotationLevel = static_cast<TrackAnnotationLevel>( level );
  updateTracks(); // Update the display data to show the new annotations
  trackSelectionStatusChanged( m_currentTrackSelection < m_tracks.size() );
}

void TrackUpdater::startTrackUpdates()
{
  if( !m_updateTrigger )
  {
    // To avoid using queued connections we need to create the timer used to trigger updates when this thread's event queue
    // is empty in the same thread that it will be using. Otherwise we can build up unprocessed events in the queue
    // that cause the tracks to continue to update after they should stop.
    m_updateTrigger = new QTimer( this );
    connect( m_updateTrigger, SIGNAL( timeout() ), this, SLOT( updateTracks() ) );
  }

  m_inhibitUpdates = false;

  // Reset performance measurement counters
  m_numUpdates = 0;
  m_totalNumUpdates = 0;
  m_cumulativeTime = 0.0;

#ifdef _MSC_VER
  QueryPerformanceCounter( &m_lastUpdateTime );
  QueryPerformanceCounter( &m_startTime );
#else
  clock_gettime( m_clockType, &m_lastUpdateTime );
  clock_gettime( m_clockType, &m_startTime );
#endif

  m_updateTrigger->start( 0 );
}

void TrackUpdater::stopTrackUpdates()
{
  if( m_updateTrigger )
  {
    m_updateTrigger->stop();
  }
  m_inhibitUpdates = true;
}

void TrackUpdater::setCoordinateAttributes( qint32 x1, qint32 y1, qint32 x2, qint32 y2, TSLCoordinateSystem *cs )
{
  m_mapExtent.corners( x1, y1, x2, y2 );

  // Replace any previous coordinate system - this happens when a new map is loaded.
  if( m_coordSys )
  {
    m_coordSys->destroy();
  }
  m_coordSys = cs;
}

void TrackUpdater::loadSymbolConfig( const QString& configFile )
{
  m_helper->destroy();
  m_helper = new TSLAPP6AHelper( configFile.toUtf8() );
}
