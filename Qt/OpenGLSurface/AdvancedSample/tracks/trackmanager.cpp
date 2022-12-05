/************m****************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include <QTime>
#include "trackmanager.h"
#include "track.h"
#include "trackupdater.h"
#include "layermanager.h"
#include "MapLink.h"
#include "MapLinkDrawing.h"
#include "tslapp6asymbol.h"
#include "tslapp6ahelper.h"

#include <set>

using namespace std;


TrackManager::TrackManager()
  : m_trackUpdater( new TrackUpdater( this ) )
  , m_currentUpdateRate( 0.0 )
  , m_averageUpdateRate( 0.0 )
  , m_numTrackTypes( 50 )
  , m_numTracks( 100 )
  , m_drawThreadDisplayData( NULL )
  , m_previousDrawThreadDisplayData( NULL )
  , m_symbolHelper( new TSLAPP6AHelper() )
  , m_trackFollowEnabled( false )
  , m_displayTrackUp( false )
{
  m_trackUpdater->moveToThread( &m_updateThread );

  // Connect our signals that will be sent in the draw thread to the slots in the track update thread
  connect( m_trackUpdater, SIGNAL( setTrackUpdateRate( double, double ) ), this, SLOT( setTrackUpdateRate( double, double ) ) );
  connect( m_trackUpdater, SIGNAL( signalLoadSymbolConfig( const QString& ) ), m_trackUpdater, SLOT( loadSymbolConfig( const QString& ) ) );
  connect( this, SIGNAL( setSimulationTimeCompression( double ) ), m_trackUpdater, SLOT( setSimulationTimeCompression( double ) ) );
  connect( this, SIGNAL( selectTrack( double, double, double ) ), m_trackUpdater, SLOT( selectTrack( double, double, double ) ) );
  connect( this, SIGNAL( clearTrackSelection() ), m_trackUpdater, SLOT( clearTrackSelection() ) );
  connect( this, SIGNAL( changeTrackHostility( quint32, qint32 ) ), m_trackUpdater, SLOT( changeTrackHostility( quint32, qint32 ) ) );
  connect( this, SIGNAL( startTrackUpdates() ), m_trackUpdater, SLOT( startTrackUpdates() ) );
  connect( this, SIGNAL( stopTrackUpdates() ), m_trackUpdater, SLOT( stopTrackUpdates() ) );
  connect( this, SIGNAL( createTracks( quint32, quint32 ) ), m_trackUpdater, SLOT( createTracks( quint32, quint32 ) ) );
  connect( this, SIGNAL( setCoordinateAttributes( qint32, qint32, qint32, qint32, TSLCoordinateSystem* ) ), m_trackUpdater, SLOT( setCoordinateAttributes( qint32, qint32, qint32, qint32, TSLCoordinateSystem* ) ) );
  connect( this, SIGNAL( setTrackAnnotationLevel( qint32 ) ), m_trackUpdater, SLOT( setTrackAnnotationLevel( qint32 ) ) );

  m_updateThread.start();
}

TrackManager::~TrackManager()
{
  stopTrackUpdates();

  m_updateThread.quit();
  m_updateThread.wait();

  delete m_trackUpdater;

  if( m_drawThreadDisplayData != m_previousDrawThreadDisplayData )
  {
    delete m_previousDrawThreadDisplayData;
  }
  delete m_drawThreadDisplayData;

  m_symbolHelper->destroy();
}

TrackManager& TrackManager::instance()
{
  static TrackManager singleton;
  return singleton;
}

TrackManager::DisplayInfo* TrackManager::acquireTrackDisplayInfo()
{
  return m_currentDisplayInfo.fetchAndStoreOrdered( NULL );
}

TrackManager::DisplayInfo* TrackManager::returnTrackDisplayInfo( TrackManager::DisplayInfo *info )
{
  return m_previousDisplayInfo.fetchAndStoreOrdered( info );
}

void TrackManager::setTrackUpdateRate( double current, double average )
{
  m_currentUpdateRate = current;
  m_averageUpdateRate = average;
}

void TrackManager::enableTrackFollow( bool follow )
{
  m_trackFollowEnabled = follow;
}

void TrackManager::enableTrackUpOrientation( bool trackUp )
{
  m_displayTrackUp = trackUp;
}

void TrackManager::setTrackSelectionCallbacks( QObject *object )
{
  connect( m_trackUpdater, SIGNAL( trackSelectionStatusChanged( bool ) ), object, SLOT( trackSelectionStatusChanged( bool ) ) );
}

void TrackManager::preDraw( TSLDrawingSurface *drawingSurface )
{
  m_drawThreadDisplayData = acquireTrackDisplayInfo();
  if( !m_drawThreadDisplayData )
  {
    m_drawThreadDisplayData = m_previousDrawThreadDisplayData;
  }

  if( m_drawThreadDisplayData && m_drawThreadDisplayData->m_selectedTrack < m_drawThreadDisplayData->m_tracks.size() )
  {
    // Refresh the information view for the selected track
    m_infoModel.refreshTrackDisplay();

    if (drawingSurface && m_trackFollowEnabled)
    {
      // A track is currently selected and track following is enabled. Make the view always centre on the selected track.
      double uuX1, uuY1;
      double trackLat = m_drawThreadDisplayData->m_tracks[m_drawThreadDisplayData->m_selectedTrack].m_lat;
      double trackLon = m_drawThreadDisplayData->m_tracks[m_drawThreadDisplayData->m_selectedTrack].m_lon;
      drawingSurface->latLongToUU(trackLat, trackLon, &uuX1, &uuY1);
      drawingSurface->pan(uuX1, uuY1, false);
    }
  }

  if (drawingSurface)
  {
    if (m_displayTrackUp && m_drawThreadDisplayData && m_drawThreadDisplayData->m_selectedTrack < m_drawThreadDisplayData->m_tracks.size())
    {
      // A track is currently selected and the view orientation is set to be along the track heading. Rotate the drawing surface
      // to match the track's heading
      drawingSurface->rotate(m_drawThreadDisplayData->m_tracks[m_drawThreadDisplayData->m_selectedTrack].m_displayHeading);
    }
  }

  // Refresh the displayed information about pinned tracks to match the new display data we have
  m_pinnedModel.refreshTrackData();
}

void TrackManager::postDraw( TSLDrawingSurface* /*drawingSurface*/ )
{
  // Return any unneeded display data back to the track update thread for reuse
  if( m_previousDrawThreadDisplayData && m_drawThreadDisplayData != m_previousDrawThreadDisplayData )
  {
    TrackManager::DisplayInfo *defunctInfo = returnTrackDisplayInfo( m_previousDrawThreadDisplayData );
    delete defunctInfo;
  }

  m_previousDrawThreadDisplayData = m_drawThreadDisplayData;
  m_drawThreadDisplayData = NULL;
}

void TrackManager::loadSymbolConfig( const QString& configFile )
{
  // Changing the symbology type invalidates all existing tracks, so delete any that currently exist
  createTracks( 0, 0 );

  // Replace the symbology helper with a new one using the referenced configuration file
  m_trackUpdater->signalLoadSymbolConfig( configFile );
  if (m_symbolHelper)
  {
    m_symbolHelper->destroy();
  }
  m_symbolHelper = new TSLAPP6AHelper( configFile.toUtf8() );

  // Then recreate the same number of tracks as we had before
  createTracks( numRequestedTracks(), numRequestedTrackTypes() );
}
