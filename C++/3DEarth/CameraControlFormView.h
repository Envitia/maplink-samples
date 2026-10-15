#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include <iostream>
#include <memory>

#include "interactions/cameramanager.h"

// CCameraControlFormView form view

class CCameraControlFormView : public CFormView
{
	DECLARE_DYNCREATE(CCameraControlFormView)

protected:
	CCameraControlFormView();           // protected constructor used by dynamic creation
	virtual ~CCameraControlFormView();
	
	// Declaring to MFC that we want to map messages
	DECLARE_MESSAGE_MAP()

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAMERA_TOOLS_FORMVIEW };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

  // Sets up member variables using the defaults from the surface, and stores the surface pointer.
  void initFromSurface(earth::Surface3D* surface);

  // Updates the camera on the surface with the current values we have stored.
  void updateCamera();

  // update dislog with camera parameters
  void updateDialog(bool doingTrackUpdate = false);

  // Sets the track that the dialog text will update to follow
  // The camera will also follow this track if the "Follow with Camera" box is checked
  void selectedTrack(earth::Track* track, uint16_t id);

  // Called by MainFrm when it receives the EARTH_TRACKS_UPDATED message
  LRESULT OnSelectedTrackViewChanged(WPARAM wParam, LPARAM lParam);

  // handle view changes
  void handleViewChange();

  // reset camera manager dialog when resetview is called
  void resetCameraManager();

public:
  // Functions for moving the camera in Geocentric space
  afx_msg void camForward();
  afx_msg void camBack();
  afx_msg void camLeft();
  afx_msg void camRight();
  afx_msg void camUp();
  afx_msg void camDown();

  afx_msg void camRollUp();
  afx_msg void camRollDown();

  afx_msg void camFovUp();
  afx_msg void camFovDown();

  // Functions for moving the camera in Geodetic space
  afx_msg void camLatUp();
  afx_msg void camLatDown();
  afx_msg void camLonUp();
  afx_msg void camLonDown();
  afx_msg void camAltUp();
  afx_msg void camAltDown();
  afx_msg void targetLatUp();
  afx_msg void targetLatDown();
  afx_msg void targetLonUp();
  afx_msg void targetLonDown();
  afx_msg void targetAltUp();
  afx_msg void targetAltDown();

  // Functions for handling track dialog functions
  afx_msg void OnBnClickedButtonStarttracks();
  afx_msg void OnBnClickedButtonRemovetracks();
  // handling clicking on the follow tracks checkbox
  afx_msg void OnBnClickedCheckFollowtrack();
  // handling edit controls
  afx_msg void OnEnChangeNUMTRACKS();
  afx_msg void OnEnChange_CAMPOS_LAT();
  afx_msg void OnEnChange_CAMPOS_LON();
  afx_msg void OnEnChange_CAMPOS_ALT();
  afx_msg void OnEnChange_CAMTARGET_LAT();
  afx_msg void OnEnChange_CAMTARGET_LON();
  afx_msg void OnEnChange_CAMTARGET_ALT();
  afx_msg void OnEnChange_CAMFOV();
  afx_msg void OnEnChange_CAMROLL();
private:
	// camera manager
	std::unique_ptr<CameraManager> m_camManager;

	// Track data
	bool m_editingDialogParams = false;
	UINT m_numTracks;
	BOOL m_followTrack;
	CString m_trackID;
	CString m_trackPosition;
	CString m_trackSpeed;
	earth::Track* m_selectedTrack;
	uint16_t m_selectedTrackID;
};


