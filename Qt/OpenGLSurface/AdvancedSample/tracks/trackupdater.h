/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef TRACKUPDATER_H
#define TRACKUPDATER_H


#include <QObject>
#include <QTimer>
#include "tslatomic.h"
#include <vector>
#include "trackmanager.h"

#ifdef WIN32
# include <Windows.h>
#endif

class TrackManager;
class TSLCoordinateSystem;


// This class updates the positions of tracks over time. It runs in its own thread,
// and is therefore independent of the drawing performed in the main application thread.
// In a real application track information and positions would be read from an external data source,
// with either defined update rate or sporadic update notifications. As this sample is standalone
// tracks are created, positioned and move randomly to simulate an external data source.
//
// The update thread runs as fast as possible for performance measurements, this can be changed
// by modifying the timeout on m_updateTrigger.

class TrackUpdater : public QObject
{
  Q_OBJECT
public:
  TrackUpdater( TrackManager *manager );
  virtual ~TrackUpdater();

public slots:
  // Populates the manager with the number of tracks requested. These
  // tracks will consist of a random selection of types based on the
  // number of types specified.
  void createTracks( quint32 numTracks, quint32 numTrackTypes );

  // Updates the positions of tracks based on how much time has elapsed since the last update
  void updateTracks();

  // Changes the current time compression which makes tracks appear to move faster or slower
  // than normal.
  void setSimulationTimeCompression( double compression );

  // Marks the track at the given position (if any) as selected by the user
  void selectTrack(double lat, double lon, double tmcPerDU);

  // Clears any currently selected track
  void clearTrackSelection();

  // Changes the hostility of the given track to the specified hostility
  void changeTrackHostility( quint32 trackID, qint32 newHostility );

  // Changes the amount of annotation to display on tracks
  void setTrackAnnotationLevel( qint32 level );

  // Sets values that will be used to position tracks within the extent of the map
  void setCoordinateAttributes( qint32 x1, qint32 y1, qint32 x2, qint32 y2, TSLCoordinateSystem *cs );

  // Starts and stops updates to track positions
  void startTrackUpdates();
  void stopTrackUpdates();

  // Changes the symbology type in use to the one referenced by the named config file. The two
  // config files available define APP6A or 2525B symbology types.
  void loadSymbolConfig( const QString& configFile );

signals:
  void setTrackUpdateRate( double current, double average );
  void trackSelectionStatusChanged( bool trackSelected );
  void signalLoadSymbolConfig( const QString& configFile );

private:
  TrackManager *m_manager;

  // Timing counters for determining how much time has elapsed since the previous update.
#ifdef WIN32
  LARGE_INTEGER m_counterFrequency;
  LARGE_INTEGER m_lastUpdateTime;
  LARGE_INTEGER m_startTime;
#else
  timespec m_lastUpdateTime;
  timespec m_startTime;
  clockid_t m_clockType;
#endif

  // Used to trigger updates to the track positions when this thread's event queue is empty.
  QTimer *m_updateTrigger;

  // Used to measure track update rate
  uint32_t m_numUpdates;
  uint32_t m_totalNumUpdates;
  double m_cumulativeTime;

  // Current time compression, values < 1.0 make time slower, > 1.0 make time faster.
  double m_timeCompressionFactor;

  // Stops tracks from being moved when position updates are disabled
  bool m_inhibitUpdates;

  // The actual tracks themselves
  std::vector< Track* > m_tracks;

  size_t m_currentTrackSelection; // Index into m_tracks of the currently selected track

  // The amount of annotation to put on symbols
  TrackAnnotationLevel m_annotationLevel;

  // The TMC extent of the currently loaded map. Used to prevent tracks from moving
  // off the edges of the map
  TSLEnvelope m_mapExtent;

  // Previously created display data that is available for reuse
  TrackManager::DisplayInfo *m_unusedDisplayData;

  TSLAPP6AHelper *m_helper;
  TSLCoordinateSystem *m_coordSys; // Coordinate system for the currently loaded map
};

#endif
