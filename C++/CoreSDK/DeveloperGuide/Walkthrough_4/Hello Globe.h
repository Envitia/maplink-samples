// Hello Globe.h : main header file for the HELLO GLOBE application
//

#if !defined(AFX_HELLOGLOBE_H__CD941FB6_DC21_44F0_BAAA_87B59EF1B059__INCLUDED_)
#define AFX_HELLOGLOBE_H__CD941FB6_DC21_44F0_BAAA_87B59EF1B059__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CHelloGlobeApp:
// See Hello Globe.cpp for the implementation of this class
//

class CHelloGlobeApp : public CWinApp
{
public:
	CHelloGlobeApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHelloGlobeApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CHelloGlobeApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HELLOGLOBE_H__CD941FB6_DC21_44F0_BAAA_87B59EF1B059__INCLUDED_)
