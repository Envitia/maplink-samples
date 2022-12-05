// *********************************************************************************
// * Copyright (c) 1998 to 2021 by Envitia Group PLC.
// *********************************************************************************
#ifdef WINNT
# define WIN32_LEAN_AND_MEAN
# define VC_EXTRALEAN
# include "windows.h"
#endif


#include "MapLink3DIMode.h"
#include "trackballviewinteraction.h"


#define _USE_MATH_DEFINES
#include <math.h>
#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

TrackballViewInteraction::TrackballViewInteraction(int modeID)
  : Interaction(modeID)
{}

TrackballViewInteraction::TrackballViewInteraction(int modeID, double tiltSensitivity, double zoomSensitivity)
  : Interaction(modeID)
  , m_tiltSensitivity(tiltSensitivity)
  , m_zoomSensitivity(zoomSensitivity)
{}

TrackballViewInteraction::~TrackballViewInteraction() {}

void TrackballViewInteraction::activate() {

  auto surface = m_drawingSurface;
  if (!surface) return;
  auto& cam = surface->camera();

  // Save the distance from camera to target
  auto posECEF = cam.positionGeocentric();
  auto targECEF = cam.lookAtGeocentric();
  earth::GeocentricDirection toTarget = targECEF - posECEF;
  // Distance from cam to target in metres
  m_cameraDistanceMetres = toTarget.length();

  m_headingAngle = calculateHeadingFromCamera();
  m_tiltAngle = calculateTiltFromCamera();

  if (std::isnan(m_headingAngle))
	  m_headingAngle = 0.0;
  if (std::isnan(m_tiltAngle))
	  m_tiltAngle = 0.0;

  m_rollAngle = cam.roll();
  m_firstActivated = true;
}

double TrackballViewInteraction::calculateHeadingFromCamera()
{
  auto surface = m_drawingSurface;
  if (!surface) return 0.0;
  auto& cam = surface->camera();

  // Calculate heading angle by comparing camera heading to north
  // Get the camera's position ignoring height
  auto posENU = cam.position();
  auto targENU = cam.lookAt();
  targENU.z(0.0);
  earth::GeodeticPoint groundPosENU(posENU.x(), posENU.y(), 0.0);

  // Make a position north of the target
  earth::GeodeticPoint northPointENU(targENU.x(), targENU.y() + 0.5, 0.0);

  // Convert all to ECEF
  auto targECEF = surface->geodeticToGeocentric(targENU);
  auto northPointECEF = surface->geodeticToGeocentric(northPointENU);
  auto groundPosECEF = surface->geodeticToGeocentric(groundPosENU);

  // Now we can calculate:
  // - the direction pointing north from the target in Geocentric space, flat to the ground,
  // - the direction from the camera to the target in Geocentric space, flat to the ground.
  // This lets us calculate the true heading in geocentric space and avoid any skew from lat/lon projection.
  earth::GeocentricDirection toNorthECEF = northPointECEF - targECEF;
  earth::GeocentricDirection headingECEF = targECEF - groundPosECEF;
  toNorthECEF.normalise();
  headingECEF.normalise();

  // Now calculate the angle between north and heading
  double cosHeadingAngle = toNorthECEF * headingECEF;
  // This value will always be positive, so we have to check the direction below and reverse it if necessary
  auto headingAngle = acos(min(1.0, max(-1.0, cosHeadingAngle)))*(180.0 / M_PI);

  // Deal with crossing the dateline to avoid issues checking the direction
  if (targENU.x() - posENU.x() > 270)
  {
    posENU.x(posENU.x() + 360);
  }
  if (targENU.x() - posENU.x() < -270)
  {
    targENU.x(targENU.x() + 360);
  }

  // Check if the sign should be flipped and do that if necessary
  if (targENU.x() - posENU.x() > 0) headingAngle *= -1.0;

  return headingAngle;
}

double TrackballViewInteraction::calculateTiltFromCamera()
{
  auto surface = m_drawingSurface;
  if (!surface) return 1;
  auto& cam = surface->camera();

  auto targENU = cam.lookAt();

  // Tilt is the angle between camera forward vector, and world up at the target point
  auto worldUpECEF = surface->geodeticToUpVector(targENU.y(), targENU.x());
  auto camForwardECEF = cam.forwardGeocentric();
  camForwardECEF.normalise();
  worldUpECEF.normalise();
  // now calculate the angle between up and forward
  double cosTiltAngle = worldUpECEF * camForwardECEF;
  return 180 - acos(cosTiltAngle)*(180.0 / M_PI);
}

void TrackballViewInteraction::deactivate()
{
  m_firstActivated = false;
}

bool TrackballViewInteraction::onLButtonDown(TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control) {

  if (!m_firstActivated)
  {
    // Do first-time setup if not done
    activate();
  }

  // Set m_activeAction based on m_actionBindings
  m_activeAction = ViewAction::Pan;
  m_lastMouseX = x;
  m_lastMouseY = y;
  return true;
}

bool TrackballViewInteraction::onRButtonDown(TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control) {

	if (!m_firstActivated)
	{
		// Do first-time setup if not done
		activate();
	}

	// Set m_activeAction based on m_actionBindings
	m_activeAction = ViewAction::Tilt;
	m_lastMouseX = x;
	m_lastMouseY = y;
	return true;
}

bool TrackballViewInteraction::onLButtonUp(TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control) {
  // Clear m_activeAction
  m_activeAction = ViewAction::Inactive;
  return true;
}

bool TrackballViewInteraction::onRButtonUp(TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control) {
	// Clear m_activeAction
	m_activeAction = ViewAction::Inactive;
	return true;
}

bool TrackballViewInteraction::onMButtonUp(TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control) {
	// Clear m_activeAction
	m_activeAction = ViewAction::Inactive;
	return true;
}

bool TrackballViewInteraction::onMouseMove(TSLButtonType button, TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control) {

  auto surface = m_drawingSurface;
  if (!surface) return false;
  auto& cam = surface->camera();

  // Get the Geodetic location of the target (the point the camera is focusing on)
  auto targENU = cam.lookAt();

  double mouseDistanceX = x - m_lastMouseX;
  double mouseDistanceY = y - m_lastMouseY;
  // Cap the mouse movement to keep it to a reasonable value
  double moveLimit = 20;
  mouseDistanceX = min(moveLimit, max(-moveLimit, mouseDistanceX));
  mouseDistanceY = min(moveLimit, max(-moveLimit, mouseDistanceY));

  switch (m_activeAction)
  {
  case TrackballViewInteraction::Pan:
  {
    targENU = calculateTarget(targENU, mouseDistanceX, mouseDistanceY);
    updateCamera(targENU);
    break;
  }
  case TrackballViewInteraction::Tilt:
  {
    calculateTiltHeading(mouseDistanceX, mouseDistanceY);
    updateCamera(targENU);
    break;
  }
  case TrackballViewInteraction::Inactive:
  default:
    break;
  }

  m_lastMouseX = x;
  m_lastMouseY = y;

  return true;
}

bool TrackballViewInteraction::onMouseWheel(short zDelta, TSLDeviceUnits x, TSLDeviceUnits y)
{
  // This function takes care of the zoom operation

  if (!m_firstActivated)
  {
    // Do first-time setup if not done
    activate();
  }
  
  auto surface = m_drawingSurface;
  if (!surface) return false;
  auto& cam = surface->camera();

  auto targENU = cam.lookAt();

  // Reduce the distance between the camera and target by a fraction of itself, multiplied by movement of the wheel
  m_cameraDistanceMetres -= (m_cameraDistanceMetres*m_zoomSensitivity)*zDelta;

  updateCamera(targENU);

  return true;
}

earth::GeocentricPoint TrackballViewInteraction::rotate3D(const earth::GeocentricPoint& point, double xRot, double yRot, double zRot) const
{
  // Standard implementation of 3D co-ordinate rotation

  double cosa = cos(yRot);
  double sina = sin(yRot);

  double cosb = cos(xRot);
  double sinb = sin(xRot);

  double cosc = cos(zRot);
  double sinc = sin(zRot);

  double Axx = cosa*cosb;
  double Axy = cosa*sinb*sinc - sina*cosc;
  double Axz = cosa*sinb*cosc + sina*sinc;

  double Ayx = sina*cosb;
  double Ayy = sina*sinb*sinc + cosa*cosc;
  double Ayz = sina*sinb*cosc - cosa*sinc;

  double Azx = -sinb;
  double Azy = cosb*sinc;
  double Azz = cosb*cosc;

  double px = point.x();
  double py = point.y();
  double pz = point.z();

  earth::GeocentricPoint ret;

  ret.x( Axx*px + Axy*py + Axz*pz );
  ret.y( Ayx*px + Ayy*py + Ayz*pz );
  ret.z( Azx*px + Azy*py + Azz*pz );

  return ret;
}

earth::GeodeticPoint TrackballViewInteraction::calculateTarget(const earth::GeodeticPoint& target, double mouseDistanceX, double mouseDistanceY)
{
  auto surface = m_drawingSurface;

  // This function calculates how the camera's target should move when we use the Pan operation.
  // The principle is to take the distance that the mouse has moved across the globe,
  // and apply that movement to the camera's target, so that the clicked point stays underneath the mouse (more or less).

  // Calculate the distance the mouse moved in lat/lon
  double lonDistance, latDistance;
  double lastLat, lastLon, newLat, newLon;
  if (!surface->DUToLatLong(m_lastMouseX, m_lastMouseY, &lastLat, &lastLon) ||
    !surface->DUToLatLong(m_lastMouseX + mouseDistanceX, m_lastMouseY + mouseDistanceY, &newLat, &newLon)) {

    // Can't calculate lat/lon position of mouse,
    // meaning the mouse probably isn't on the globe
    // Try it with the centre of the view instead
	
	//RECT wndRect;
	//m_view->GetWindowRect(&wndRect);
	//double centerX = double(wndRect.right - wndRect.left) / 2;
	//double centerY = double(wndRect.bottom - wndRect.top) / 2;
	TSLEnvelope wndRect = m_requestHandler ? m_requestHandler->GetWindowEnvelope() : TSLEnvelope{};
	double centerX = (double)wndRect.width()/2;
	double centerY = (double)wndRect.height()/2;

    if (!surface->DUToLatLong(centerX, centerY, &lastLat, &lastLon) ||
      !surface->DUToLatLong(centerX + mouseDistanceX, centerY + mouseDistanceY, &newLat, &newLon)) {
      // Still can't calculate lat/lon position (unlikely to happen)
      // Use the most recent value and hope for the best
      // Will not work so well if heading != 0

      // Normalize mouse distance
      double mouseMoveLength = sqrt((mouseDistanceX*mouseDistanceX) + (mouseDistanceY*mouseDistanceY));
      double normMouseX = mouseDistanceX / mouseMoveLength;
      double normMouseY = mouseDistanceY / mouseMoveLength;

      // Apply the most recent pan distance, but in relation to current mouse movement
      lonDistance = m_lastPanDistance.length() * normMouseX;
      latDistance = -m_lastPanDistance.length() * normMouseY;
    }
    else
    {
      // Success, we can use the calculated lat/lon positions to move the camera
      latDistance = max(-m_maxPanDistance, min(newLat - lastLat, m_maxPanDistance));
      lonDistance = max(-m_maxPanDistance, min(newLon - lastLon, m_maxPanDistance));
      m_lastPanDistance = { lonDistance, latDistance, 0.0 };
    }
  }
  else
  {
    // Success, we can use the mouse's movement across the globe to move the camera
    latDistance = max(-m_maxPanDistance, min(newLat - lastLat, m_maxPanDistance));
    lonDistance = max(-m_maxPanDistance, min(newLon - lastLon, m_maxPanDistance));
    m_lastPanDistance = { lonDistance, latDistance, 0.0 };
  }
  
  earth::GeodeticPoint newTarget(target);

  // move the target by that amount
  newTarget.x(target.x() - lonDistance);
  newTarget.y(target.y() - latDistance);

  return newTarget;
}

void TrackballViewInteraction::calculateTiltHeading(double mouseDistanceX, double mouseDistanceY)
{
  // This function is used to move the camera in a spherical path around the target point,
  // based on two rotation values.
  // Tilt is based on vertical mouse movement, and
  // Heading is based on horizontal mouse movement.


  m_tiltAngle += mouseDistanceY*m_tiltSensitivity;

  // Limit the tilt angle, such that the camera can't go downwards through the floor,
  // and prevent it becoming unreliable when directly above the target.
  if (m_tiltAngle >= 90) m_tiltAngle = 90;
  if (m_tiltAngle <= 1) m_tiltAngle = 1;

  m_headingAngle -= mouseDistanceX*m_tiltSensitivity;
}

earth::GeodeticPoint TrackballViewInteraction::calculateCameraPosition(earth::GeodeticPoint target, double tilt, double heading, double distance) const
{
  // This function takes into account the target position, and finds a position for the camera that
  // resides on an imaginary sphere of radius 'distance', where the tilt and heading define its location on that sphere.

  auto surface = m_drawingSurface;
  auto targECEF = surface->geodeticToGeocentric(target);

  // We start with the camera positioned on the target.
  // Calculating the camera's position from scratch each time avoids accumulating error.
  earth::GeocentricPoint camPosECEF(targECEF);

  // From here, we need to move the camera away from the target, to the correct point on the sphere.
  // We need to find the right direction in which to move it.

  // Start with "backward" vector
  // With no rotations, Positive X is pointing out of the earth at lat 0 lon 0
  // Therefore if we had no tilt or heading, and the target position was at 0,0, this would be the full direction we need.
  earth::GeocentricDirection backward(1.0, 0.0, 0.0);

  // Rotate the directional vector to match heading and tilt
  // We do the vertical rotation first
  backward = rotate3D(backward, tilt*(M_PI / 180.0), 0.0, 0.0);
  backward = rotate3D(backward, 0.0, 0.0, heading*(M_PI / 180.0));

  // Now we need to add rotation for the geodetic position,
  // So that the rotations we applied above will be relative to the right point on the Earth's surface.
  backward = rotate3D(backward, -target.y()*(M_PI / 180.0), target.x()*(M_PI / 180.0), 0.0);

  // Normalising the vector makes it 1 metre in length, so that in the next step the full movement is the correct distance
  backward.normalise();

  // Now we can move the camera backwards from the target point, by 'distance', and it will be in the right place.
  camPosECEF += backward*distance;

  // Convert this back to Geodetic coordinate space
  auto camPosENU = surface->geocentricToGeodetic(camPosECEF);

  return camPosENU;
}

void TrackballViewInteraction::updateCamera(earth::GeodeticPoint target)
{
  auto surface = m_drawingSurface;
  auto newCamPos = calculateCameraPosition(target, m_tiltAngle, m_headingAngle, m_cameraDistanceMetres);

  auto& cam = surface->camera();

  // Check the altitude of the terrain here and make sure the camera doesn't go below it
  cam.minAltitude(0.0);
  double altitude = 0.0;
  surface->getTerrainHeight(newCamPos.y(), newCamPos.x(), altitude);
  // Set the camera height to be at least 100m off the ground, to avoid clipping through terrain
  newCamPos.z(max(newCamPos.z(), altitude+100));

  // Keep the target on the ground
  target.z(0.0);

  // Move the camera
  cam.lookAt(newCamPos, target, m_rollAngle);

  // Tell the main window to update anything else relying on the camera's position
  if (m_requestHandler)
	  m_requestHandler->viewChanged(m_drawingSurface);
}

