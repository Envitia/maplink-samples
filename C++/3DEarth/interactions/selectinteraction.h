// *********************************************************************************
// * Copyright (c) 1998 to 2021 by Envitia Group PLC.
// *********************************************************************************

#ifndef SELECTINTERACTION_H
#define SELECTINTERACTION_H

//! An interaction which picks, and deltes geometry when left mouse is clicked
class SelectInteraction : public Interaction {
public:
  SelectInteraction() = delete;
  SelectInteraction(int modeID);
  virtual ~SelectInteraction();

  virtual void activate() override final;
  virtual void deactivate() override final;

  virtual bool onLButtonDown(TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control) override final;

private:
  // Selection action for tracks - When selected the track will be updated to show a selection
  // border, and the view may be set to follow the track
  bool selectTrack(TSLDeviceUnits x, TSLDeviceUnits y);

  // Selection action for geometry - When selected the geometry style will toggle
  // between selected and unselected
  void selectGeometry(TSLDeviceUnits x, TSLDeviceUnits y);

  
};

#endif