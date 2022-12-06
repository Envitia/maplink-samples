/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#undef Bool
#undef Status
#include <QMessageBox>

#include "AddWaypointInteractionMode.h"
#include "layers/TrackCustomDataLayer.h"

AddWaypointInteractionMode::AddWaypointInteractionMode(int modeID)
  : TSLInteractionMode(modeID)
  , m_trackLayer(NULL)
{
}

AddWaypointInteractionMode::~AddWaypointInteractionMode()
{
}

bool AddWaypointInteractionMode::onLButtonDown(TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control)
{
  double cursorLat = 0.0, cursorLon = 0.0;
  if (m_trackLayer && m_drawingSurface)
  {
    if (m_drawingSurface->DUToLatLong(x, y, &cursorLat, &cursorLon, true))
    {
      m_trackLayer->addWaypoint(cursorLat, cursorLon);
    }
    else
    {
      QMessageBox::information(NULL, "Selected position outside valid projection range",
        "The selected position is too far from the projection centre to be used as a waypoint", QMessageBox::Ok);
    }
  }

  return false;
}

void AddWaypointInteractionMode::setTargetLayer(TrackCustomDataLayer *trackLayer)
{
  m_trackLayer = trackLayer;
}
