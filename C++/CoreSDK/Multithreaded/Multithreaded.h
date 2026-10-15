// Multithreaded.h : main header file for the Multithreaded application
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
// CMultithreadedApp:
// See Multithreaded.cpp for the implementation of this class
//

class CMultithreadedApp : public CWinApp
{
public:
  CMultithreadedApp();
  
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CMultithreadedApp)
	public:
  virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL
  
  // Implementation
  
  //{{AFX_MSG(CMultithreadedApp)
  afx_msg void OnAppAbout();
	//}}AFX_MSG
  DECLARE_MESSAGE_MAP()
    

public:
  // loader for the system to use
  static TSLFileLoader*  m_loader;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFC_H__CF7BD0C6_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
