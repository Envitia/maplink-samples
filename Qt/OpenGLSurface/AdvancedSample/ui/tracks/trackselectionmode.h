/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

// A custom interaction mode used to select a specific track. The selection is used to
// enable the follow track and track north display settings.

#include "MapLinkIMode.h"

class TrackSelectionMode : public TSLInteractionMode
{
public:
  TrackSelectionMode (int modeID, bool middleButtonPansToPoint = true);
  virtual ~TrackSelectionMode();

  virtual bool onLButtonDown (TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control);
  virtual void deactivate ();
};
