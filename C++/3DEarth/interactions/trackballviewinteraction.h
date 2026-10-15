// *********************************************************************************
// * Copyright (c) 1998 to 2021 by Envitia Group PLC.
// *********************************************************************************

#ifndef TRACKBALLVIEWINTERACTION_H
#define TRACKBALLVIEWINTERACTION_H

#include <map>

//! TrackBallViewInteraction
//! This class provides an interaction mode that lets the user easily control the 3D camera using the mouse.
//! 
//! By default, the class is set up the following way:
//! - Click and drag the left mouse button:   Pan (moves the camera across the globe, maintaining its orientation)
//! - Click and drag the right mouse button:  Tilt (rotates the camera around the target point)
//! - Move the mouse wheel:                   Zoom (moves the camera closer to/further from the target)
//! 
//! The buttons for Pan and Tilt can be changed by passing a new map of bindings into the alternative constructor.
//! Tilt and Zoom sensitivity (the factor by which the camera moves relative to mouse movement) can also be set here,
//! or via the separate setter functions.
//!
//! This interaction stores information about the camera's angle and position relative to the target,
//! and this information must be updated each time the interaction mode is switched to. Use the 'activate'
//! function to update the camera state upon switching.

class TrackballViewInteraction : public Interaction {
public:
  enum ViewAction {
    Inactive,
    Pan,
    Tilt
  };

  TrackballViewInteraction() = delete;

  // Construct interaction with default bindings
  // Left Mouse - Pan
  // Right Mouse - Tilt
  TrackballViewInteraction(int modeID);

  // Construct interaction with customised bindings
  // the actionBindings map contains pairs of buttons and actions, where each button can be mapped to an action.
  // Custom values for tilt and zoom sensitivity can also be set here.
  TrackballViewInteraction(int modeID, double tiltSensitivity, double zoomSensitivity);

  virtual ~TrackballViewInteraction();

  // activate
  // Stores the current state of the camera and performs necessary setup.
  // To be called when switching to this interaction mode.
  virtual void activate() override final;
  virtual void deactivate() override final;

  // Various functions for handling mouse events
  virtual bool onLButtonDown(TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control) override final;
  virtual bool onRButtonDown(TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control) override final;

  virtual bool onLButtonUp(TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control) override final;
  virtual bool onRButtonUp(TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control) override final;
  virtual bool onMButtonUp(TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control) override final;
  
  virtual bool onMouseMove(TSLButtonType button, TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control) override final;
  virtual bool onMouseWheel(short zDelta, TSLDeviceUnits x, TSLDeviceUnits y) override final;

  // Setters for zoom and tilt sensitivity
  void zoomSensitivity(double sens) { m_zoomSensitivity = sens; }
  void tiltSensitivity(double sens) { m_tiltSensitivity = sens; }

private:

  // Used for getting the initial angles from the camera upon activation
  double calculateHeadingFromCamera();
  double calculateTiltFromCamera();

  // Rotates a GeocentricPoint around the origin of world space, by the values given for each axis.
  // Standard implementation of a 3D rotation method.
  earth::GeocentricPoint rotate3D( const earth::GeocentricPoint& point, double xRot, double yRot, double zRot) const;

  // Moves the given target point across the globe.
  // Calculates the distance moved by the mouse in latitude/longitude,
  // and returns a new target point with the movement added to it.
  earth::GeodeticPoint calculateTarget(const earth::GeodeticPoint& target, double mouseDistanceX, double mouseDistanceY);

  // Modifies the tilt and heading angles based on the mouse movement
  void calculateTiltHeading(double mouseDistanceX, double mouseDistanceY);

  // Calculates where the camera should be relative to the given target, based on the given angles and distance.
  earth::GeodeticPoint calculateCameraPosition(earth::GeodeticPoint target, double tilt, double heading, double distance) const;

  // Update the camera based on target point and member variables, 
  // and triggering any updates needed by the main window
  void updateCamera(earth::GeodeticPoint target);

  // Storage for the parameters we use to position the camera
  // m_tiltAngle is different from camera pitch: 0 degrees on this tilt angle is equivalent to looking straight down at the target, and
  // 90 degrees is tangent to the surface of the globe.
  double m_tiltAngle = 1.0;
  double m_headingAngle = 0.0;
  double m_cameraDistanceMetres = 0.0;
  double m_rollAngle = 0.0;

  // A map of which mouse buttons perform certain actions
  ViewAction m_activeAction = ViewAction::Inactive;

  // The last position of the mouse on the screen - used for calculating mouse movement
  TSLDeviceUnits m_lastMouseX = 0;
  TSLDeviceUnits m_lastMouseY = 0;

  // Limit the maximum pan speed (to maintain smooth movement when the mouse isn't over the globe)
  double m_maxPanDistance = 2.5;
  earth::GeodeticDirection m_lastPanDistance;

  // Sensitivity modifiers to control how much effect mouse movement has on the view
  double m_tiltSensitivity = 0.1;
  double m_zoomSensitivity = 0.001;

  // Used to detect whether the interaction has been used before being activated
  // False until the first activation is done
  bool m_firstActivated = false;
};

#endif