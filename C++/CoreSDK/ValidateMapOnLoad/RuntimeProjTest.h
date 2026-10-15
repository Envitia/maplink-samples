// RuntimeProjTest.h : main header file for the RuntimeProjTest application
//

#if !defined(AFX_MFC_H__CF7BD0C6_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_MFC_H__CF7BD0C6_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx_tlt.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

class TSLFileLoader;



/////////////////////////////////////////////////////////////////////////////
// CRuntimeProjTestApp:
// See RuntimeProjTest.cpp for the implementation of this class
//

class CRuntimeProjTestApp : public CWinApp
{
public:
  CRuntimeProjTestApp();
  
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CRuntimeProjTestApp)
	public:
  virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL
  
  // Implementation
  
  //{{AFX_MSG(CRuntimeProjTestApp)
  afx_msg void OnAppAbout();
	afx_msg void OnFileOpen();
	afx_msg void OnOptionsUsecallbacksforprojectionchanges();
	afx_msg void OnUpdateOptionsUsecallbacksforprojectionchanges(CCmdUI* pCmdUI);
	//}}AFX_MSG
  DECLARE_MESSAGE_MAP()
    

public:
  // loader for the system to use
  static TSLFileLoader*  m_loader;

  // option whether to use callbacks on projection changes
  static bool m_useCallbacksForProjectionChanges;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFC_H__CF7BD0C6_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
