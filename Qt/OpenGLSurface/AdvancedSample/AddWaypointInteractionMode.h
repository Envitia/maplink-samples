/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef ADD_WAYPOINT_INTERACTION_MODE_H
#define ADD_WAYPOINT_INTERACTION_MODE_H

#include "MapLinkIMode.h"

class TrackCustomDataLayer;

// Simple class to handle user interactions with the track layer.
class AddWaypointInteractionMode : public TSLInteractionMode
{
public:
  AddWaypointInteractionMode( int modeID );
  virtual ~AddWaypointInteractionMode();

  virtual bool onLButtonDown( TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control );

  void setTargetLayer( TrackCustomDataLayer *trackLayer );

private:
  TrackCustomDataLayer *m_trackLayer;
};

#endif
