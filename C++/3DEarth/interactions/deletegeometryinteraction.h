// *********************************************************************************
// * Copyright (c) 1998 to 2021 by Envitia Group PLC.
// *********************************************************************************

#ifndef DELETEGEOMETRYINTERACTION_H
#define DELETEGEOMETRYINTERACTION_H

//! An interaction which picks, and deltes geometry when left mouse is clicked
class DeleteGeometryInteraction : public Interaction {
public:
  DeleteGeometryInteraction() = delete;
  DeleteGeometryInteraction(int modeID);
  virtual ~DeleteGeometryInteraction();

  virtual void activate() override final;
  virtual void deactivate() override final;

  virtual bool onLButtonDown(TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control) override final;
};

#endif