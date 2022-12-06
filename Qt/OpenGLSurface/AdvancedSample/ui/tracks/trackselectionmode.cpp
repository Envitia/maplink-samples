/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "tracks/trackmanager.h"

#include "MapLinkDrawing.h"
#include "trackselectionmode.h"

TrackSelectionMode::TrackSelectionMode( int modeID, bool middleButtonPansToPoint )
  : TSLInteractionMode( modeID, middleButtonPansToPoint )
{
}

TrackSelectionMode::~TrackSelectionMode()
{
}

bool TrackSelectionMode::onLButtonDown( TSLDeviceUnits x, TSLDeviceUnits y, bool /*shift*/, bool /*control*/ )
{
  if (!m_drawingSurface)
  {
    return false;
  }

  // Convert the point clicked from screen pixels to Lat/Lon
  double lat, lon;
  m_drawingSurface->DUToLatLong(x, y, &lat, &lon);

  double tmcPerDUX, tmcPerDUY;
  m_drawingSurface->TMCperDU( tmcPerDUX, tmcPerDUY );

  // Tell the track manager to select the track at this position (if any). This will happen in the track
  // update thread rather than the application thread that this callback was triggered from.
  TrackManager::instance().selectTrack( lat, lon, tmcPerDUY );

  return false;
}

void TrackSelectionMode::deactivate()
{
  // Tell the track manager to clear any currently selected track
  TrackManager::instance().clearTrackSelection();
}
