// MaplinkEnterpriseSample.h : main header file for the MaplinkEnterpriseSample application
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
// CMaplinkEnterpriseSampleApp:
// See MaplinkEnterpriseSample.cpp for the implementation of this class
//

class CMaplinkEnterpriseSampleApp : public CWinApp
{
public:
  CMaplinkEnterpriseSampleApp();
  
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CMaplinkEnterpriseSampleApp)
	public:
  virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL
  
  // Implementation
  
  //{{AFX_MSG(CMaplinkEnterpriseSampleApp)
  afx_msg void OnAppAbout();
  afx_msg void OnFileOpenRemote();
	afx_msg void OnFileOpen();
	//}}AFX_MSG
  DECLARE_MESSAGE_MAP()
    

public:
  // loader for the system to use
  static TSLFileLoader*  m_loaderLocal;
  static TSLFileLoader* m_loaderRemote;

  // Flag to indicate which loader should be used
  static bool m_remote;

  // location of persistent cache file if this is given to the program
  // as a command line parameter
  static string m_cacheConfigFile;

  // Persistent cache data initialised from the ini file passed on the command line
  static TSLPersistentCacheData* m_persistentCacheData ;

};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFC_H__CF7BD0C6_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
