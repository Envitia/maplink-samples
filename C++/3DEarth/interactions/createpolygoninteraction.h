// *********************************************************************************
// * Copyright (c) 1998 to 2021 by Envitia Group PLC.
// *********************************************************************************

#ifndef CREATEPOLYGONINTERACTION_H
#define CREATEPOLYGONINTERACTION_H

//! An interaction which creates an earth::geometry::Polygon instance
//! as the user clicks
//! - Left click to start the primitive/add points
//! - Right click to finish the primitive
class CreatePolygonInteraction : public Interaction {
public:
  CreatePolygonInteraction() = delete;
  CreatePolygonInteraction(int modeID, std::string styleName);
  virtual ~CreatePolygonInteraction();

  virtual void activate() override final;
  virtual void deactivate() override final;

  virtual bool onLButtonDown(TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control) override final;
  virtual bool onRButtonDown(TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control) override final;
  virtual bool onMouseMove(TSLButtonType button, TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control) override final;
private:
  // Create the geometry just above the surface - To reduce clipping with the terrain
  const double m_height = 1.0;
  envitia::maplink::earth::geometry::Polygon* m_polygon = nullptr;
  // The style to use when rendering the line
  std::string m_styleName;
};

#endif