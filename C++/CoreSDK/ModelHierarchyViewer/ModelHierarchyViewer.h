// ModelHierarchyViewer.h : main header file for the MODELHIERARCHYVIEWER application
//

#if !defined(AFX_MODELHIERARCHYVIEWER_H__C04F8A36_6B40_11D3_B383_0060087B197C__INCLUDED_)
#define AFX_MODELHIERARCHYVIEWER_H__C04F8A36_6B40_11D3_B383_0060087B197C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CModelHierarchyViewerApp:
// See ModelHierarchyViewer.cpp for the implementation of this class
//

class CModelHierarchyViewerApp : public CWinApp
{
public:
	CModelHierarchyViewerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModelHierarchyViewerApp)
	public:
	virtual BOOL InitInstance();
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CModelHierarchyViewerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	afx_msg void OnFileNew( );


	DECLARE_MESSAGE_MAP()

public:
	static CString m_filename;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODELHIERARCHYVIEWER_H__C04F8A36_6B40_11D3_B383_0060087B197C__INCLUDED_)
