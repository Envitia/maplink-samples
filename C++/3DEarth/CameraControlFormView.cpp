// CameraControlFormView.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "CameraControlFormView.h"
#include <iostream>
#include "MainFrm.h"
#include "EarthSampleDoc.h"
#include "EarthSampleView.h"

#include <sstream>

// CCameraControlFormView

IMPLEMENT_DYNCREATE(CCameraControlFormView, CFormView)

CCameraControlFormView::CCameraControlFormView()
  : CFormView(IDD_CAMERA_TOOLS_FORMVIEW)
  , m_camManager(nullptr)
  , m_numTracks(250)
  , m_followTrack(FALSE)
  , m_trackID(_T(""))
  , m_trackPosition(_T(""))
  , m_trackSpeed(_T(""))
  , m_selectedTrack(nullptr)
{

}

CCameraControlFormView::~CCameraControlFormView()
{
}

BEGIN_MESSAGE_MAP(CCameraControlFormView, CFormView)
	ON_WM_KEYDOWN()
  ON_BN_CLICKED(IDC_BUTTON_CAMPOS_LAT_UP, &CCameraControlFormView::camLatUp)
  ON_BN_CLICKED(IDC_BUTTON_CAMPOS_LAT_DOWN, &CCameraControlFormView::camLatDown)
  ON_BN_CLICKED(IDC_BUTTON_CAMPOS_LON_UP, &CCameraControlFormView::camLonUp)
  ON_BN_CLICKED(IDC_BUTTON_CAMPOS_LON_DOWN, &CCameraControlFormView::camLonDown)
  ON_BN_CLICKED(IDC_BUTTON_CAMPOS_ALT_UP, &CCameraControlFormView::camAltUp)
  ON_BN_CLICKED(IDC_BUTTON_CAMPOS_ALT_DOWN, &CCameraControlFormView::camAltDown)
  ON_BN_CLICKED(IDC_BUTTON_CAMTARGET_LAT_UP, &CCameraControlFormView::targetLatUp)
  ON_BN_CLICKED(IDC_BUTTON_CAMTARGET_LAT_DOWN, &CCameraControlFormView::targetLatDown)
  ON_BN_CLICKED(IDC_BUTTON_CAMTARGET_LON_UP, &CCameraControlFormView::targetLonUp)
  ON_BN_CLICKED(IDC_BUTTON_CAMTARGET_LON_DOWN, &CCameraControlFormView::targetLonDown)
  ON_BN_CLICKED(IDC_BUTTON_CAMTARGET_ALT_UP, &CCameraControlFormView::targetAltUp)
  ON_BN_CLICKED(IDC_BUTTON_CAMTARGET_ALT_DOWN, &CCameraControlFormView::targetAltDown)

  ON_BN_CLICKED(IDC_BUTTON_CAMMOVE_FORWARD, &CCameraControlFormView::camForward)
  ON_BN_CLICKED(IDC_BUTTON_CAMMOVE_BACK, &CCameraControlFormView::camBack)
  ON_BN_CLICKED(IDC_BUTTON_CAMMOVE_LEFT, &CCameraControlFormView::camLeft)
  ON_BN_CLICKED(IDC_BUTTON_CAMMOVE_RIGHT, &CCameraControlFormView::camRight)
  ON_BN_CLICKED(IDC_BUTTON_CAMMOVE_UP, &CCameraControlFormView::camUp)
  ON_BN_CLICKED(IDC_BUTTON_CAMMOVE_DOWN, &CCameraControlFormView::camDown)

  ON_BN_CLICKED(IDC_BUTTON_CAMROLL_UP, &CCameraControlFormView::camRollUp)
  ON_BN_CLICKED(IDC_BUTTON_CAMROLL_DOWN, &CCameraControlFormView::camRollDown)

  ON_BN_CLICKED(IDC_BUTTON_CAMFOV_UP, &CCameraControlFormView::camFovUp)
  ON_BN_CLICKED(IDC_BUTTON_CAMFOV_DOWN, &CCameraControlFormView::camFovDown)

  ON_BN_CLICKED(IDC_BUTTON_STARTTRACKS, &CCameraControlFormView::OnBnClickedButtonStarttracks)
  ON_BN_CLICKED(IDC_BUTTON_REMOVETRACKS, &CCameraControlFormView::OnBnClickedButtonRemovetracks)
  ON_BN_CLICKED(IDC_CHECK_FOLLOWTRACK, &CCameraControlFormView::OnBnClickedCheckFollowtrack)

    ON_EN_CHANGE(IDC_EDIT_NUMTRACKS, &CCameraControlFormView::OnEnChangeNUMTRACKS)
	ON_EN_CHANGE(IDC_EDIT_CAMPOS_LAT, &CCameraControlFormView::OnEnChange_CAMPOS_LAT)
	ON_EN_CHANGE(IDC_EDIT_CAMPOS_LON, &CCameraControlFormView::OnEnChange_CAMPOS_LON)
	ON_EN_CHANGE(IDC_EDIT_CAMPOS_ALT, &CCameraControlFormView::OnEnChange_CAMPOS_ALT)
	ON_EN_CHANGE(IDC_EDIT_CAMTARGET_LAT, &CCameraControlFormView::OnEnChange_CAMTARGET_LAT)
	ON_EN_CHANGE(IDC_EDIT_CAMTARGET_LON, &CCameraControlFormView::OnEnChange_CAMTARGET_LON)
	ON_EN_CHANGE(IDC_EDIT_CAMTARGET_ALT, &CCameraControlFormView::OnEnChange_CAMTARGET_ALT)
	ON_EN_CHANGE(IDC_EDIT_CAMFOV, &CCameraControlFormView::OnEnChange_CAMFOV)
	ON_EN_CHANGE(IDC_EDIT_CAMROLL, &CCameraControlFormView::OnEnChange_CAMROLL)
END_MESSAGE_MAP()


// CCameraControlFormView diagnostics

#ifdef _DEBUG
void CCameraControlFormView::AssertValid() const
{
  CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCameraControlFormView::Dump(CDumpContext& dc) const
{
  CFormView::Dump(dc);
}


#endif
#endif //_DEBUG


void CCameraControlFormView::initFromSurface(earth::Surface3D * surface)
{
	m_camManager.reset(new CameraManager(surface));
	updateDialog();
}

void CCameraControlFormView::updateCamera()
{
	if (m_camManager)
	{
		m_camManager->updateCamera();
	}
}

void CCameraControlFormView::updateDialog(bool doingTrackUpdate)
{
	if (!m_camManager)
		return;

	m_editingDialogParams = true;
	if (doingTrackUpdate)
	{
		SetDlgItemText(IDC_TRACK_ID, m_trackID);
		SetDlgItemText(IDC_TRACK_POS, m_trackPosition);
		SetDlgItemText(IDC_TRACK_SPEED, m_trackSpeed);

		if (m_followTrack)
		{
			CString str;
			str.Format(L"%f", m_camManager->camPosLat());
			SetDlgItemText(IDC_EDIT_CAMPOS_LAT, str);
			str.Format(L"%f", m_camManager->camPosLon());
			SetDlgItemText(IDC_EDIT_CAMPOS_LON, str);
			str.Format(L"%f", m_camManager->camTargetLat());
			SetDlgItemText(IDC_EDIT_CAMTARGET_LAT, str);
			str.Format(L"%f", m_camManager->camTargetLon());
			SetDlgItemText(IDC_EDIT_CAMTARGET_LON, str);
			str.Format(L"%f", m_camManager->camTargetAlt());
			SetDlgItemText(IDC_EDIT_CAMTARGET_ALT, str);
		}
	}
	else
	{
		CString str;
		str.Format(L"%f", m_camManager->camPosLat());
		SetDlgItemText(IDC_EDIT_CAMPOS_LAT, str);
		str.Format(L"%f", m_camManager->camPosLon());
		SetDlgItemText(IDC_EDIT_CAMPOS_LON, str);
		str.Format(L"%f", m_camManager->camPosAlt());
		SetDlgItemText(IDC_EDIT_CAMPOS_ALT, str);
		str.Format(L"%f", m_camManager->camTargetLat());
		SetDlgItemText(IDC_EDIT_CAMTARGET_LAT, str);
		str.Format(L"%f", m_camManager->camTargetLon());
		SetDlgItemText(IDC_EDIT_CAMTARGET_LON, str);
		str.Format(L"%f", m_camManager->camTargetAlt());
		SetDlgItemText(IDC_EDIT_CAMTARGET_ALT, str);

		str.Format(L"%f", m_camManager->camRollAngle());
		SetDlgItemText(IDC_EDIT_CAMROLL, str);
		str.Format(L"%d", m_camManager->camFOV());
		SetDlgItemText(IDC_EDIT_CAMFOV, str);

		str.Format(L"%d", m_numTracks);
		SetDlgItemText(IDC_EDIT_NUMTRACKS, str);
		CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_FOLLOWTRACK);
		pBtn->SetCheck(m_followTrack);
	}
	m_editingDialogParams = false;
}

void CCameraControlFormView::selectedTrack(earth::Track * track, uint16_t id)
{
	m_selectedTrack = track;
	m_selectedTrackID = id;

	if (m_selectedTrack == nullptr)
	{
		m_trackPosition = _T("");
		m_trackSpeed = _T("");
		m_trackID = _T("");

		updateDialog(true);
	}
}

LRESULT CCameraControlFormView::OnSelectedTrackViewChanged(WPARAM wParam, LPARAM lParam)
{
	if (m_selectedTrack)
	{
		if (m_followTrack)
		{
			// Move the camera target to follow the track
			if (m_camManager)
			{
				m_camManager->followTrack(m_selectedTrack);
			}
		}

		// Update the text in the dialog to match the track's properties
		auto pos = m_selectedTrack->position();
		std::wstringstream ss(L"");
		ss << pos.y() << ", " << pos.x();
		m_trackPosition = ss.str().c_str();
		m_trackSpeed = m_selectedTrack->attributeValue(1);
		m_trackID = std::to_wstring(m_selectedTrackID).c_str();
		updateDialog(true);

		return TRUE;
	}
	return FALSE;
}

void CCameraControlFormView::handleViewChange()
{
	if (m_camManager)
	{
		m_camManager->handleViewChange();
	}

	// Update the dialog
	updateDialog();
}

void CCameraControlFormView::resetCameraManager()
{
	if (m_camManager)
	{
		m_camManager->resetCamera();
	}
	updateDialog();
}
// CCameraControlFormView message handlers

void CCameraControlFormView::camForward()
{
	if (m_camManager)
	{
		m_camManager->camForward();
	}
	// Update the dialog
	updateDialog();
}

void CCameraControlFormView::camBack()
{
	if (m_camManager)
	{
		m_camManager->camBack();
	}
	// Update the dialog
	updateDialog();
}

void CCameraControlFormView::camLeft()
{
	if (m_camManager)
	{
		m_camManager->camLeft();
	}
	// Update the dialog
	updateDialog();
}

void CCameraControlFormView::camRight()
{
	if (m_camManager)
	{
		m_camManager->camRight();
	}
	// Update the dialog
	updateDialog();
}

void CCameraControlFormView::camUp()
{
	if (m_camManager)
	{
		m_camManager->camUp();
	}
	// Update the dialog
	updateDialog();
}

void CCameraControlFormView::camDown()
{
	if (m_camManager)
	{
		m_camManager->camDown();
	}
	// Update the dialog
	updateDialog();
}

void CCameraControlFormView::camRollUp() {
	if (m_camManager)
	{
		m_camManager->camRollUp();
	}
	// Update the dialog
	updateDialog();
}
void CCameraControlFormView::camRollDown() {
	if (m_camManager)
	{
		m_camManager->camRollDown();
	}
	// Update the dialog
	updateDialog();
}

void CCameraControlFormView::camFovUp() {
	if (m_camManager)
	{
		m_camManager->camFovUp();
	}
	// Update the dialog
	updateDialog();
}
void CCameraControlFormView::camFovDown() {
	if (m_camManager)
	{
		m_camManager->camFovDown();
	}
	// Update the dialog
	updateDialog();
}

// Functions for moving the camera in Geodetic space
void CCameraControlFormView::camLatUp() {
	if (m_camManager)
	{
		m_camManager->camLatUp();
	}
	// Update the dialog
	updateDialog();
}
void CCameraControlFormView::camLatDown() {
	if (m_camManager)
	{
		m_camManager->camLatDown();
	}
	// Update the dialog
	updateDialog();
}
void CCameraControlFormView::camLonUp() {
	if (m_camManager)
	{
		m_camManager->camLonUp();
	}
	// Update the dialog
	updateDialog();
}
void CCameraControlFormView::camLonDown() {
	if (m_camManager)
	{
		m_camManager->camLonDown();
	}
	// Update the dialog
	updateDialog();
}
void CCameraControlFormView::camAltUp() {
	if (m_camManager)
	{
		m_camManager->camAltUp();
	}
	// Update the dialog
	updateDialog();
}
void CCameraControlFormView::camAltDown() {
	if (m_camManager)
	{
		m_camManager->camAltDown();
	}
	// Update the dialog
	updateDialog();
}
void CCameraControlFormView::targetLatUp() {
	if (m_camManager)
	{
		m_camManager->targetLatUp();
	}
	// Update the dialog
	updateDialog();
}
void CCameraControlFormView::targetLatDown() {
	if (m_camManager)
	{
		m_camManager->targetLatDown();
	}
	// Update the dialog
	updateDialog();
}
void CCameraControlFormView::targetLonUp() {
	if (m_camManager)
	{
		m_camManager->targetLonUp();
	}
	// Update the dialog
	updateDialog();
}
void CCameraControlFormView::targetLonDown() {
	if (m_camManager)
	{
		m_camManager->targetLonDown();
	}
	// Update the dialog
	updateDialog();
}
void CCameraControlFormView::targetAltUp() {
	if (m_camManager)
	{
		m_camManager->targetAltUp();
	}
	// Update the dialog
	updateDialog();
}
void CCameraControlFormView::targetAltDown() {
	if (m_camManager)
	{
		m_camManager->targetAltDown();
	}
	// Update the dialog
	updateDialog();
}

void CCameraControlFormView::OnBnClickedButtonStarttracks()
{
  auto mainFrm = static_cast<CMainFrame*>(AfxGetMainWnd());
  auto earthView = static_cast<CEarthSampleView*>(mainFrm->getEarthView());
  CEarthSampleDoc* doc = (CEarthSampleDoc*)earthView->GetDocument();

  // remove previous tracks
  OnBnClickedButtonRemovetracks();
  
  // Initialise the track simulation
  earthView->tracksManager().initialiseTracks(m_numTracks);

  // Add the tracks to the view
  auto& tracks = earthView->tracksManager().getTracks();
  earthView->addTracks(tracks);
}

void CCameraControlFormView::OnBnClickedButtonRemovetracks()
{
  m_selectedTrack = nullptr;
  auto mainFrm = static_cast<CMainFrame*>(AfxGetMainWnd());
  auto earthView = static_cast<CEarthSampleView*>(mainFrm->getEarthView());
  earthView->removeTracks();

  m_trackPosition = _T("");
  m_trackSpeed = _T("");
  m_trackID = _T("");

  updateDialog(true);
}


void CCameraControlFormView::OnBnClickedCheckFollowtrack()
{
	CButton *btnCheck = (CButton*)GetDlgItem(IDC_CHECK_FOLLOWTRACK);
	m_followTrack = btnCheck->GetCheck();
}

void CCameraControlFormView::OnEnChangeNUMTRACKS()
{
	if (m_editingDialogParams)
		return;
	CEdit  *btnEdit = (CEdit *)GetDlgItem(IDC_EDIT_NUMTRACKS);
	CString str;
	btnEdit->GetWindowText(str);
	m_numTracks = _ttoi((LPCTSTR)str);
}

void CCameraControlFormView::OnEnChange_CAMPOS_LAT()
{
	if (m_editingDialogParams || !m_camManager)
		return;
	CEdit  *btnEdit = (CEdit *)GetDlgItem(IDC_EDIT_CAMPOS_LAT);
	CString str;
	btnEdit->GetWindowText(str);
	double val = _ttof((LPCTSTR)str); 
	if (val < -90)
	{
		btnEdit->SetWindowText(L"-90");
		return;
	}
	else if (val > 90)
	{
		btnEdit->SetWindowText(L"90");
		return;
	}
	m_camManager->camPosLat(val);
	m_camManager->updateCamera();
}

void CCameraControlFormView::OnEnChange_CAMPOS_LON()
{
	if (m_editingDialogParams || !m_camManager)
		return;
	CEdit  *btnEdit = (CEdit *)GetDlgItem(IDC_EDIT_CAMPOS_LON);
	CString str;
	btnEdit->GetWindowText(str);
	double val = _ttof((LPCTSTR)str);
	if (val < -180)
	{
		btnEdit->SetWindowText(L"-180");
		return;
	}
	else if (val > 180)
	{
		btnEdit->SetWindowText(L"180");
		return;
	}
	m_camManager->camPosLon(val);
	m_camManager->updateCamera();
}

void CCameraControlFormView::OnEnChange_CAMPOS_ALT()
{
	if (m_editingDialogParams || !m_camManager)
		return;
	CEdit  *btnEdit = (CEdit *)GetDlgItem(IDC_EDIT_CAMPOS_ALT);
	CString str;
	btnEdit->GetWindowText(str);
	double val = _ttof((LPCTSTR)str);
	m_camManager->camPosAlt(val);
	m_camManager->updateCamera();
}

void CCameraControlFormView::OnEnChange_CAMTARGET_LAT()
{
	if (m_editingDialogParams || !m_camManager)
		return;
	CEdit  *btnEdit = (CEdit *)GetDlgItem(IDC_EDIT_CAMTARGET_LAT);
	CString str;
	btnEdit->GetWindowText(str);
	double val = _ttof((LPCTSTR)str);
	if (val < -90)
	{
		btnEdit->SetWindowText(L"-90");
		return;
	}
	else if (val > 90)
	{
		btnEdit->SetWindowText(L"90");
		return;
	}
	m_camManager->camTargetLat(val);
	m_camManager->updateCamera();
}
void CCameraControlFormView::OnEnChange_CAMTARGET_LON()
{
	if (m_editingDialogParams || !m_camManager)
		return;
	CEdit  *btnEdit = (CEdit *)GetDlgItem(IDC_EDIT_CAMTARGET_LON);
	CString str;
	btnEdit->GetWindowText(str);
	double val = _ttof((LPCTSTR)str);
	if (val < -180)
	{
		btnEdit->SetWindowText(L"-180");
		return;
	}
	else if (val > 180)
	{
		btnEdit->SetWindowText(L"180");
		return;
	}
	m_camManager->camTargetLon(val);
	m_camManager->updateCamera();
}

void CCameraControlFormView::OnEnChange_CAMTARGET_ALT()
{
	if (m_editingDialogParams || !m_camManager)
		return;
	CEdit  *btnEdit = (CEdit *)GetDlgItem(IDC_EDIT_CAMTARGET_ALT);
	CString str;
	btnEdit->GetWindowText(str);
	double val = _ttof((LPCTSTR)str);
	m_camManager->camTargetAlt(val);
	m_camManager->updateCamera();
}
void CCameraControlFormView::OnEnChange_CAMFOV()
{
	if (m_editingDialogParams || !m_camManager)
		return;
	CEdit  *btnEdit = (CEdit *)GetDlgItem(IDC_EDIT_CAMFOV);
	CString str;
	btnEdit->GetWindowText(str);
	int val = _ttoi((LPCTSTR)str);
	if (val < 15)
	{
		btnEdit->SetWindowText(L"15");
		return;
	}
	else if (val > 120)
	{
		btnEdit->SetWindowText(L"120");
		return;
	}
	m_camManager->camFOV(val);
	m_camManager->updateCamera();
}
void CCameraControlFormView::OnEnChange_CAMROLL()
{
	if (m_editingDialogParams || !m_camManager)
		return;
	CEdit  *btnEdit = (CEdit *)GetDlgItem(IDC_EDIT_CAMROLL);
	CString str;
	btnEdit->GetWindowText(str);
	double val = _ttof((LPCTSTR)str);
	if (val < -360)
	{
		btnEdit->SetWindowText(L"-360");
		return;
	}
	else if (val > 360)
	{
		btnEdit->SetWindowText(L"360");
		return;
	}
	m_camManager->camRollAngle(val);
	m_camManager->updateCamera();
}