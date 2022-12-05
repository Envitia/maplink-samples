// *********************************************************************************
// * Copyright (c) 1998 to 2021 by Envitia Group PLC.
// *********************************************************************************

#ifndef CREATETEXTINTERACTION_H
#define CREATETEXTINTERACTION_H

//! An interaction which creates an earth::geometry::Text instance when the left mouse is clicked
class CreateTextInteraction : public Interaction {
public:
  CreateTextInteraction() = delete;
  CreateTextInteraction(int modeID);
  virtual ~CreateTextInteraction();

  virtual void activate() override final;
  virtual void deactivate() override final;

  virtual bool onLButtonDown(TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control) override final;
};

#endif