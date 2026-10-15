// *********************************************************************************
// * Copyright (c) 1998 to 2021 by Envitia Group PLC.
// *********************************************************************************

#ifndef CREATESYMBOLINTERACTION_H
#define CREATESYMBOLINTERACTION_H

//! An interaction which creates an earth::geometry::Symbol when the left mouse is clicked
class CreateSymbolInteraction : public Interaction {
public:
  CreateSymbolInteraction() = delete;
  CreateSymbolInteraction(int modeID);
  virtual ~CreateSymbolInteraction();

  virtual void activate() override final;
  virtual void deactivate() override final;
  virtual bool onLButtonDown(TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control) override final;
};

#endif