// *********************************************************************************
// * Copyright (c) 1998 to 2021 by Envitia Group PLC.
// *********************************************************************************
#ifdef WINNT
# define WIN32_LEAN_AND_MEAN
# define VC_EXTRALEAN
# include "windows.h"
#endif

#include "MapLink3DIMode.h"

#include "cameramanager.h"
#include <iostream>

#include <sstream>


CameraManager::CameraManager(envitia::maplink::earth::Surface3D * surface)
	: m_surface(surface)
	, m_metresIncrement(1000.0)
	, m_degreeIncrement(0.5)
	, m_camAltSpeed(0.2)
{
	resetCamera();
}

CameraManager::~CameraManager()
{
}

void CameraManager::resetCamera()
{
	if (!m_surface)
		return;

	m_camRollAngle = 0;
	m_camTargetLat = 1.0;
	m_camTargetLon = 0.0;
	m_camTargetAlt = 0.0;
	auto& cam = m_surface->camera();

	auto pos = cam.position();
	m_camPosLat = pos.y();
	m_camPosLon = pos.x();
	m_camPosAlt = pos.z();
	m_camFOV = cam.fov();

	// update the camera with initial settings
	updateCamera();
}
void CameraManager::updateCamera()
{
	if (!m_surface)
		return;

	auto camPos = m_surface->camera().position();
	camPos.y(m_camPosLat);
	camPos.x(m_camPosLon);
	camPos.z(m_camPosAlt);
	earth::GeodeticPoint geodeticTarget(m_camTargetLon, m_camTargetLat, m_camTargetAlt);

	m_surface->camera().lookAt(camPos, geodeticTarget, m_camRollAngle);
	m_surface->camera().fov(m_camFOV);
	m_surface->redraw();
}

void CameraManager::handleViewChange()
{
	if (!m_surface)
		return;
	auto& cam = m_surface->camera();

	m_camRollAngle = cam.roll();

	auto pos = cam.position();

	m_camPosLat = pos.y();
	m_camPosLon = pos.x();
	m_camPosAlt = pos.z();

	auto targ = cam.lookAt();
	m_camTargetLat = targ.y();
	m_camTargetLon = targ.x();
	m_camTargetAlt = targ.z();
}

bool CameraManager::followTrack(envitia::maplink::earth::Track* selectedTrack)
{
	if (!selectedTrack)
		return false;

	// Update the text in the dialog to match the track's properties
	auto pos = selectedTrack->position();

	// Move the camera target to follow the track
	m_camTargetLat = pos.y();
	m_camTargetLon = pos.x();
	m_camTargetAlt = pos.z();

	m_camPosLat = pos.y() - 1;
	m_camPosLon = pos.x();

	updateCamera();

	return true;
}

void CameraManager::moveCamByVector(envitia::maplink::earth::GeocentricDirection d)
{
	if (!m_surface)
		return;

	// Get the geocentric point and add the vector
	auto& cam = m_surface->camera();
	auto position = cam.positionGeocentric();
	position += d;
	cam.positionGeocentric(position);

	// Update the geodetic positions for the text boxes
	auto camPos = m_surface->camera().position();
	m_camPosLon = camPos.x();
	m_camPosLat = camPos.y();
	m_camPosAlt = camPos.z();

	updateCamera();
}

void CameraManager::camForward()
{
	if (!m_surface)
		return;

	earth::GeocentricDirection dir = m_surface->camera().forwardGeocentric();
	dir *= m_metresIncrement;
	moveCamByVector(dir);
}

void CameraManager::camBack()
{
	if (!m_surface)
		return;

	earth::GeocentricDirection dir = m_surface->camera().forwardGeocentric();
	dir *= (0.0 - m_metresIncrement);
	moveCamByVector(dir);
}

void CameraManager::camLeft()
{
	if (!m_surface)
		return;

	earth::GeocentricDirection dir = m_surface->camera().rightGeocentric();
	dir *= (0.0 - m_metresIncrement);
	moveCamByVector(dir);
}

void CameraManager::camRight()
{
	if (!m_surface)
		return;

	earth::GeocentricDirection dir = m_surface->camera().rightGeocentric();
	dir *= m_metresIncrement;
	moveCamByVector(dir);
}

void CameraManager::camUp()
{
	if (!m_surface)
		return;

	earth::GeocentricDirection dir = m_surface->camera().upGeocentric();
	dir *= m_metresIncrement;
	moveCamByVector(dir);
}

void CameraManager::camDown()
{
	if (!m_surface)
		return;

	earth::GeocentricDirection dir = m_surface->camera().upGeocentric();
	dir *= (0.0 - m_metresIncrement);
	moveCamByVector(dir);

}

void CameraManager::camRollUp() {
	if (!m_surface)
		return;
	m_camRollAngle += 1.0;
	updateCamera();
}
void CameraManager::camRollDown() {
	if (!m_surface)
		return;
	m_camRollAngle -= 1.0;
	updateCamera();
}

void CameraManager::camFovUp() {
	if (!m_surface)
		return;
	if ((m_camFOV + 10) <= m_maxFOV)
		m_camFOV += 10;
	updateCamera();
}
void CameraManager::camFovDown() {
	if (!m_surface)
		return;
	if ((m_camFOV - 10) >= m_minFOV)
		m_camFOV -= 10;
	updateCamera();
}

// Functions for moving the camera in Geodetic space
void CameraManager::camLatUp() {
	if (!m_surface)
		return;
	m_camPosLat += m_degreeIncrement;
	updateCamera();
}
void CameraManager::camLatDown() {
	if (!m_surface)
		return;
	m_camPosLat -= m_degreeIncrement;
	updateCamera();
}
void CameraManager::camLonUp() {
	if (!m_surface)
		return;
	m_camPosLon += m_degreeIncrement;
	updateCamera();
}
void CameraManager::camLonDown() {
	if (!m_surface)
		return;
	m_camPosLon -= m_degreeIncrement;
	updateCamera();
}
void CameraManager::camAltUp() {
	if (!m_surface)
		return;
	m_camPosAlt *= (1.0 + m_camAltSpeed);
	updateCamera();
}
void CameraManager::camAltDown() {
	if (!m_surface)
		return;
	m_camPosAlt *= (1.0 - m_camAltSpeed);
	updateCamera();
}
void CameraManager::targetLatUp() {
	if (!m_surface)
		return;
	m_camTargetLat += m_degreeIncrement;
	updateCamera();
}
void CameraManager::targetLatDown() {
	if (!m_surface)
		return;
	m_camTargetLat -= m_degreeIncrement;
	updateCamera();
}
void CameraManager::targetLonUp() {
	if (!m_surface)
		return;
	m_camTargetLon += m_degreeIncrement;
	updateCamera();
}
void CameraManager::targetLonDown() {
	if (!m_surface)
		return;
	m_camTargetLon -= m_degreeIncrement;
	updateCamera();
}
void CameraManager::targetAltUp() {
	if (!m_surface)
		return;
	m_camTargetAlt += m_metresIncrement;
	updateCamera();
}
void CameraManager::targetAltDown() {
	if (!m_surface)
		return;
	m_camTargetAlt -= m_metresIncrement;
	updateCamera();
}


double CameraManager::camPosLat()
{
	return m_camPosLat;
}
void CameraManager::camPosLat(double val)
{
	m_camPosLat = val;
}
double CameraManager::camPosLon()
{
	return m_camPosLon;
}
void CameraManager::camPosLon(double val)
{
	m_camPosLon = val;
}
double CameraManager::camPosAlt()
{
	return m_camPosAlt;
}
void CameraManager::camPosAlt(double val)
{
	m_camPosAlt = val;
}
double CameraManager::camTargetLat()
{
	return m_camTargetLat;
}
void CameraManager::camTargetLat(double val)
{
	m_camTargetLat = val;
}
double CameraManager::camTargetLon()
{
	return m_camTargetLon;
}
void CameraManager::camTargetLon(double val)
{
	m_camTargetLon = val;
}
double CameraManager::camTargetAlt()
{
	return m_camTargetAlt;
}
void CameraManager::camTargetAlt(double val)
{
	m_camTargetAlt = val;
}
double CameraManager::camRollAngle()
{
	return m_camRollAngle;
}
void CameraManager::camRollAngle(double val)
{
	m_camRollAngle = val;
}
int CameraManager::camFOV()
{
	return m_camFOV;
}
void CameraManager::camFOV(int val)
{
	m_camFOV = val;
}