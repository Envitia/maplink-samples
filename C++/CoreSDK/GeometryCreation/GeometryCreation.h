// GeometryCreation.h : main header file for the GEOMETRYCREATION application
//

#if !defined(AFX_GEOMETRYCREATION_H__09EF0C7D_3429_4343_A923_971885AC10DC__INCLUDED_)
#define AFX_GEOMETRYCREATION_H__09EF0C7D_3429_4343_A923_971885AC10DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CGeometryCreationApp:
// See GeometryCreation.cpp for the implementation of this class
//

class CGeometryCreationApp : public CWinApp
{
public:
	CGeometryCreationApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGeometryCreationApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CGeometryCreationApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GEOMETRYCREATION_H__09EF0C7D_3429_4343_A923_971885AC10DC__INCLUDED_)
