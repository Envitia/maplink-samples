// SatellitePropagator3D.h : main header file for the SatellitePropagator3D application
//

#if !defined(AFX_SatellitePropagator3D_H__CF7BD0C6_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_SatellitePropagator3D_H__CF7BD0C6_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// SatellitePropagator3DApp:
// See SatellitePropagator3D.cpp for the implementation of this class
//

class SatellitePropagator3DApp : public CWinApp
{
public:
	SatellitePropagator3DApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SatellitePropagator3DApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(SatellitePropagator3DApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SatellitePropagator3D_H__CF7BD0C6_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
