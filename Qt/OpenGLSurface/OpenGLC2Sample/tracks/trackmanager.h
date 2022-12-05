/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef TRACKMANAGER_H
#define TRACKMANAGER_H

// This class is the container for all tracks in existance in the application.
// It handles creating and destroying tracks, as well as forwarding events to the
// individual track objects in order to update their poisitions.
// Updates to the tracks themselves are handled by the track updater, which runs
// in it's own thread. Other parts of the application do not access the track 
// updater directly, and instead use this manager to ensure thread safety.

#undef Bool
#undef Status

#include <QWidget>
#include <QThread>
#include <QAtomicPointer>
#include <vector>

#include "track.h"
#include "trackinfomodel.h"
#include "pinnedtrackmodel.h"
#include "trackannotationenum.h"

class TrackUpdater;
class TSLDrawingSurface;
class TSLCoordinateSystem;
class TSLAPP6AHelper;


using std::vector;

class TrackManager : public QThread
{
  Q_OBJECT
public:
  class DisplayInfo
  {
  public:
    vector< Track::DisplayInfo > m_tracks;
    size_t m_selectedTrack;
    TrackAnnotationLevel m_annotationLevel;
  };

  TrackManager();
  ~TrackManager();

  // Returns the current track display information for use by the drawing thread
  const DisplayInfo* displayInformation();

  // Returns how many and how many types types of tracks were requested in the last call to createTracks()
  size_t numRequestedTrackTypes() const;
  void numRequestedTrackTypes( size_t numTypes );
  size_t numRequestedTracks() const;
  void numRequestedTracks( size_t numTracks );

  double currentUpdateRate() const;
  double averageUpdateRate() const;

  void enableTrackFollow( bool follow );
  void enableTrackUpOrientation( bool trackUp );

  static TrackManager& instance();

  // Sets up signal/slot connections for the track update thread to call when the selection status for tracks changes
  void setTrackSelectionCallbacks( QObject *object );

  // Called before drawing begins. This is used to get the most recent track display information from the
  // track update update thread and to implement track following.
  void preDraw( TSLDrawingSurface *drawingSurface );

  // Called after drawing has finished. Used to transfer old display information back to the track update thread
  // for reuse.
  void postDraw( TSLDrawingSurface *drawingSurface );

  // Returns the model implementation that can be used to update UI controls with information about the selected track
  TrackInfoModel& trackInfoModel();

  // Returns the model implementation that can be used to update UI controls with information about a list of tracks to follow
  PinnedTrackModel& pinnedTrackModel();

  // Switches the symbology helpers used between the named configs, i.e. between APP6A and 2525B symbols
  void loadSymbolConfig( const QString& configFile );

  // Returns the symbol helper for the drawing thread.
  TSLAPP6AHelper* symbolHelper();

signals:
  // Populates the manager with the number of tracks requested. These
  // tracks will consist of a random selection of types based on the
  // number of types specified.
  void createTracks( quint32 numTracks, quint32 numTrackTypes );

  // Sets values that will be used to position tracks within the extent of the map
  void setCoordinateAttributes( qint32 x1, qint32 y1, qint32 x2, qint32 y2, TSLCoordinateSystem *cs );

  // Updates the time compression factor for simulation playback. This signal is posted to the thread containing
  // the track updater.
  void setSimulationTimeCompression( double compression );

  // Selects the track at the given position. This signal is posted to the thread containing
  // the track updater.
  void selectTrack( qint32 x, qint32 y, double tmcPerDU );

  // Clears any currently selected track
  void clearTrackSelection();

  // Changes the hostility of the given track to the specified hostility
  void changeTrackHostility( quint32 trackID, qint32 newHostility );

  // Changes the amount of annotation to display on tracks
  void setTrackAnnotationLevel( qint32 level );

  // Starts and stops updates in the track update thread
  void startTrackUpdates();
  void stopTrackUpdates();

  private slots:
  // Called by the track update thread to report how often the track positions are being updated. Used by the
  // framerate data layer to display the track update rate.
  void setTrackUpdateRate( double current, double average );

private:
  // Shared data for migration of drawing information to and from the draw and track update threads
  QAtomicPointer< DisplayInfo > m_currentDisplayInfo;
  QAtomicPointer< DisplayInfo > m_previousDisplayInfo;

  DisplayInfo* acquireTrackDisplayInfo();
  DisplayInfo* returnTrackDisplayInfo( DisplayInfo *info );

  size_t m_numTracks; // The last number of tracks requested
  size_t m_numTrackTypes; // The number of types of tracks that could potentially exist

  TrackUpdater *m_trackUpdater;
  QThread m_updateThread;
  double m_currentUpdateRate;
  double m_averageUpdateRate;

  TSLAPP6AHelper *m_symbolHelper;

  // The track display information that the draw thread should use
  TrackManager::DisplayInfo *m_drawThreadDisplayData;
  TrackManager::DisplayInfo *m_previousDrawThreadDisplayData;

  bool m_trackFollowEnabled;
  bool m_displayTrackUp;

  // Model class that maps information about the selected track to a UI control
  TrackInfoModel m_infoModel;

  // Model class that maps information about a set of tracks to a UI control
  PinnedTrackModel m_pinnedModel;

  friend class TrackUpdater;
};

inline double TrackManager::currentUpdateRate() const
{
  return m_currentUpdateRate;
}

inline double TrackManager::averageUpdateRate() const
{
  return m_averageUpdateRate;
}

inline const TrackManager::DisplayInfo* TrackManager::displayInformation()
{
  if( m_drawThreadDisplayData )
  {
    return m_drawThreadDisplayData;
  }
  if( m_previousDrawThreadDisplayData )
  {
    return m_previousDrawThreadDisplayData;
  }

  return NULL;
}

inline TrackInfoModel& TrackManager::trackInfoModel()
{
  return m_infoModel;
}

inline PinnedTrackModel& TrackManager::pinnedTrackModel()
{
  return m_pinnedModel;
}

inline TSLAPP6AHelper* TrackManager::symbolHelper()
{
  return m_symbolHelper;
}

inline size_t TrackManager::numRequestedTrackTypes() const
{
  return m_numTrackTypes;
}

inline void TrackManager::numRequestedTrackTypes( size_t numTypes )
{
  m_numTrackTypes = numTypes;
}

inline size_t TrackManager::numRequestedTracks() const
{
  return m_numTracks;
}

inline void TrackManager::numRequestedTracks( size_t numTracks )
{
  m_numTracks = numTracks;
}

#endif //TRACKMANAGER_H
