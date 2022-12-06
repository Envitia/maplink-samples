/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef TRACK_H
#define TRACK_H

// This class represents a single track in the application. It holds information
// about the track, such as it's position, velocity and heading, as well as the
// type of the track.
// This class is not responsible for drawing the track.
#include <vector>

#define MAPLINK_NO_DRAWING_SURFACE
#include "MapLink.h"

#include "tslapp6asymbol.h"

#include "trackannotationenum.h"

using std::vector;
using std::pair;

class TSLText;
class TSLCoordinateSystem;
class TSLAPP6AHelper;

class Track
{
public:
  // This class contains all the information necessary to draw the track. Instances are populated as part
  // of the track update method in the track update thread and the results are passed to the drawing thread
  // for display.
  struct DisplayInfo
  {
  public:
    DisplayInfo();
    DisplayInfo( const DisplayInfo &rhs );
    ~DisplayInfo();

    TSLTMC m_x;
    TSLTMC m_y;
    double m_lat;
    double m_lon;
    int m_symbolKey; // The Id that can be used to look up the TSLAPP6ASymbol from the TSLAPP6AHelper if needed
    double m_size; // Size in pixels of the track
    double m_heading;
    double m_displayHeading;
    double m_speed;
    double m_altitude;
    double m_sinDisplayHeading;
    double m_cosDisplayHeading;
    TSLAPP6ASymbol::HostilityEnum m_hostility;

    TSLText *m_speedLabel;
    TSLText *m_positionLabel;

    vector< TSLCoord > m_historyPoints;
  };

  Track( const TSLAPP6ASymbol &type, TSLAPP6AHelper *helper );
  ~Track();

  void setPosition( double lat, double lon, const TSLCoordinateSystem *coordSys );
  void setHostility( TSLAPP6ASymbol::HostilityEnum newHostility );

  // Returns the current position of the track in TMC space
  TSLTMC x() const;
  TSLTMC y() const;

  const TSLAPP6ASymbol& type() const;
  double heading() const;
  static size_t maxHistoryPoints();

  void updatePosition( double elapsedSeconds, const TSLCoordinateSystem *coordSys, const TSLEnvelope &mapExtent, Track::DisplayInfo &displayInfo,
                       TrackAnnotationLevel annotationLevel );

  // Returns true if the displayed extent of this track intersects the given position. Used to pick tracks for use with the follow track
  // and track north operations.
  bool intersects( TSLTMC x, TSLTMC y, double tmcPerDU );

private:
  TSLAPP6ASymbol m_type;
  double m_speed; // In meters/second
  double m_heading; // True heading of the track in degrees
  double m_mapDisplayHeading; // Heading of the track relative to map projection in radians
  double m_displayHeading; // Angle of heading relative to the map
  double m_altitude; // In meters
  double m_targetDistance; // Distance in meters to the track's current destination
  double m_distanceFromLastHistoryPoint; // Distance in meters since the last history point was recorded
  
  // Current track position in the TMC coordinate system of the loaded map.
  TSLTMC m_x;
  TSLTMC m_y;

  // Current track position in lat/lon
  double m_lat;
  double m_lon;

  // Pre-positioned labels for dynamically updated annotations
  TSLText *m_speedLabel;
  TSLText *m_positionLabel;

  // Historical locations of the track
  vector< TSLCoord > m_historyPoints;
  size_t m_currentHistoryIndex;

  static double m_minTargetDistance; // The minimum distance a track can move along its heading before turning
  static double m_maxTargetDistance; // The maximum distance a track can move along its heading before turning
  static double m_maxHeadingDelta; // The maximum turn a track can make when choosing a new heading
  static double m_historyDropDistance; // Distance between historical points
  static size_t m_maxHistoryPoints; // The total number of history points to record for a track
};

inline void Track::setHostility( TSLAPP6ASymbol::HostilityEnum newHostility )
{
  m_type.hostility( newHostility );
}

inline TSLTMC Track::x() const
{
  return m_x;
}

inline TSLTMC Track::y() const
{
  return m_y;
}

inline const TSLAPP6ASymbol& Track::type() const
{
  return m_type;
}

inline double Track::heading() const
{
  return m_heading;
}

inline size_t Track::maxHistoryPoints()
{
  return m_maxHistoryPoints;
}

#endif // TRACK_H
