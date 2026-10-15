#if !defined(AFX_TRACKINFO_H__3D08F993_501A_11D5_BD35_00104BB05CAE__INCLUDED_)
#define AFX_TRACKINFO_H__3D08F993_501A_11D5_BD35_00104BB05CAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TrackInfo.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CTrackInfo dialog

class CTrackInfo : public CDialog
{
// Construction
public:
	CTrackInfo(CWnd* pParent = NULL);   // standard constructor

	virtual ~CTrackInfo() ;
	
	bool refreshData() ;	    // Refresh the data for the selected track

        void setSymbolType(bool isAPP6A);
// Dialog Data
	//{{AFX_DATA(CTrackInfo)
	enum { IDD = IDD_TRACK_INFO };
	CComboBox	m_trackTypeCombo;
	CButton	m_reset;
	CButton	m_apply;
	CComboBox	m_hostilityCombo;
	CString	m_altitude;
	CString	m_heading;
	CString	m_trackNumber;
	CString	m_velocity;
	CString	m_latitude;
	CString	m_longitude;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrackInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation


protected:

	// Generated message map functions
	//{{AFX_MSG(CTrackInfo)
	afx_msg void OnReset();
	afx_msg void OnApply();
	virtual BOOL OnInitDialog();
	afx_msg void OnDropdownHostilityCombo();
	afx_msg void OnDropdownTracktypeCombo();
	afx_msg void OnSetfocusHeading();
	afx_msg void OnSetfocusVelocity();
	afx_msg void OnSetfocusAltitude();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void saveData() ;

	// Set when the user is ammending the track data
	// so no updates are processed until they have finished 
	// entering the new data
	bool m_updatingData ;

	// Storage variables in case the user want's to reset 
	// the initial values rather than update the track with
	// new values
	double	      m_oldVelocity ;
	double	      m_oldAltitude ;
	double	      m_oldHeading ;
	HOSTILE_TYPE  m_oldHostility ;
	TRACK_TYPE    m_oldTrackType ;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRACKINFO_H__3D08F993_501A_11D5_BD35_00104BB05CAE__INCLUDED_)
