/****************************************************************************
                   Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef TRACK_CUSTOM_DATA_LAYER_H
#define TRACK_CUSTOM_DATA_LAYER_H

#include "tgmapidll.h"
#include "tsltmsapi.h"
#include "tslsimplestring.h"
#include "MapLink_defines.h"
#include "tslatomic.h"
#include "tslclientcustomdatalayer.h"

#include <list>
#include <string>


// A custom data layer that represents a single track. It is responsible for updating the track
// and drawing it with its annotations.

class TrackCustomDataLayer : public TSLClientCustomDataLayer
{
public:
  TrackCustomDataLayer();
  virtual ~TrackCustomDataLayer();

  virtual bool drawLayer( TSLRenderingInterface *renderingInterface, const TSLEnvelope* extent, TSLCustomDataLayerHandler& layerHandler );

  // Updates the position of the track based on its speed and the time since the last update
  void updatePositions( double secsSinceLastFrame, double timeMultiplier );

  // Returns where the track currently is
  void getTrackPosition( double &lat, double &lon );

  // Adds the given position as a user-defined new waypoint and makes it the current destination of the track
  void addWaypoint( double lat, double lon );

  bool displayTrackInfo() const;
  void displayTrackInfo( bool show );

  bool loadWaypointList( const std::string &filename );

private:
  // Current position of the track
  double m_trackLatPos;
  double m_trackLonPos;

  double m_trackSpeed;
  double m_trackTurnRate;

  struct TrackWaypoint
  {
    TrackWaypoint();
    TrackWaypoint( double lat, double lon, bool addedByUser = false );

    double m_lat;
    double m_lon;
    bool m_addedByUser;
  };

  double m_distanceToNextWaypoint;
  double m_waypointAzimuth;

  std::list< TrackWaypoint > m_waypoints;
  std::list< TrackWaypoint >::iterator m_currentWaypoint;

  // Rendering attributes
  TSLStyleID m_trackInfoTextColour;
  TSLStyleID m_trackInfoTextStyle;
  double m_trackInfoTextSize;

  TSLStyleID m_trackColour;
  TSLStyleID m_trackStyle;
  double m_trackSize;

  TSLStyleID m_destinationMarkerColour;
  TSLStyleID m_destinationMarkerStyle;
  double m_destinationMarkerSize;

  bool m_showTrackInfo;
};

inline void TrackCustomDataLayer::getTrackPosition( double &lat, double &lon )
{
  lat = m_trackLatPos;
  lon = m_trackLonPos;
}

inline bool TrackCustomDataLayer::displayTrackInfo() const
{
  return m_showTrackInfo;
}

inline void TrackCustomDataLayer::displayTrackInfo( bool show )
{
  m_showTrackInfo = show;
}

#endif
