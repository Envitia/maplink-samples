// *********************************************************************************
// * Copyright (c) 1998 to 2021 by Envitia Group PLC.
// *********************************************************************************

#ifndef CREATEPOLYLINEINTERACTION_H
#define CREATEPOLYLINEINTERACTION_H

//! An interaction which creates an earth::geometry::Polyline instance
//! as the user clicks
//! - Left click to start the line/add points
//! - Right click to finish the line
class CreatePolylineInteraction : public Interaction {
public:
  CreatePolylineInteraction() = delete;
  CreatePolylineInteraction(int modeID, std::string styleName);
  virtual ~CreatePolylineInteraction();

  virtual void activate() override final;
  virtual void deactivate() override final;

  virtual bool onLButtonDown(TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control) override final;
  virtual bool onRButtonDown(TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control) override final;
  virtual bool onMouseMove(TSLButtonType button, TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control) override final;
private:
  // Create the line just above the surface - To reduce clipping with the terrain
  const double m_lineHeight = 1.0;
  envitia::maplink::earth::geometry::Polyline* m_polyline = nullptr;
  // The style to use when rendering the line
  std::string m_styleName;
};

#endif