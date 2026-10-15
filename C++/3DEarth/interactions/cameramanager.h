// *********************************************************************************
// * Copyright (c) 1998 to 2021 by Envitia Group PLC.
// *********************************************************************************

#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

namespace envitia {
	namespace maplink {
		namespace earth
		{
			class Surface3D;
			class Track;
			class GeocentricDirection;
		}
	}
}
class CameraManager
{
public:
	CameraManager(envitia::maplink::earth::Surface3D* surface);           // protected constructor used by dynamic creation
	virtual ~CameraManager();

	void resetCamera();

	// Updates the camera on the surface with the current values we have stored.
	void updateCamera();

	// update the settings with the current camera position
	void handleViewChange();

	// Move the camera to follow track
	bool followTrack(envitia::maplink::earth::Track* selectedTrack);
	
	// Moves the camera by the given direction vector in Geocentric space. Used for the top set of "move" buttons.
	void moveCamByVector(envitia::maplink::earth::GeocentricDirection d);

public:
	// Functions for moving the camera in Geocentric space
	void camForward();
	void camBack();
	void camLeft();
	void camRight();
	void camUp();
	void camDown();

	void camRollUp();
	void camRollDown();

	void camFovUp();
	void camFovDown();

	// Functions for moving the camera in Geodetic space
	void camLatUp();
	void camLatDown();
	void camLonUp();
	void camLonDown();
	void camAltUp();
	void camAltDown();
	void targetLatUp();
	void targetLatDown();
	void targetLonUp();
	void targetLonDown();
	void targetAltUp();
	void targetAltDown();

public:
	double camPosLat();
	void camPosLat(double val);
	double camPosLon();
	void camPosLon(double val);
	double camPosAlt();
	void camPosAlt(double val);
	double camTargetLat();
	void camTargetLat(double val);
	double camTargetLon();
	void camTargetLon(double val);
	double camTargetAlt();
	void camTargetAlt(double val);
	double camRollAngle();
	void camRollAngle(double val);
	int camFOV();
	void camFOV(int val);

private:
	// Camera variables
	// These are all set by the dialog
	double m_camPosLat;
	double m_camPosLon;
	double m_camPosAlt;
	double m_camTargetLat;
	double m_camTargetLon;
	double m_camTargetAlt;

	double m_camRollAngle;
	int m_camFOV;

	// Value increments for the buttons
	double m_degreeIncrement;
	double m_metresIncrement;
	double m_camAltSpeed;

	// Fov limits
	int m_minFOV = 15;
	int m_maxFOV = 120;

	// The surface containing the camera
	envitia::maplink::earth::Surface3D* m_surface;
};


#endif