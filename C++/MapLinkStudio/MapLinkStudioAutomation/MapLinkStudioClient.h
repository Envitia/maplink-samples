// MapLinkStudioClient.h : main header file for the MAPLINKSTUDIOCLIENT application
//

#if !defined(AFX_MFC_H__CF7BD0C6_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_MFC_H__CF7BD0C6_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMapLinkStudioClientApp:
// See MapLinkStudioClient.cpp for the implementation of this class
//

class CMapLinkStudioClientApp : public CWinApp
{
public:
  CMapLinkStudioClientApp();

  string getMaplinkHome() { return m_maplinkHome; };

  string getCoordinateSystem() { return m_coordSystem; };

  string getMaplinkIniFile() { return m_maplinkIniFile; };

  string getProjectFile() { return m_projectFile; };

  void setProjectFile(string projectFile) { m_projectFile = projectFile; };

  string getSourceName() { return m_sourceName; };

  void setSourceName(string sourceName) { m_sourceName = sourceName; };

  string getTemplateName() { return m_templateName; };

  void setTemplateName(string templateName) { m_templateName = templateName; };

  void close();

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CMapLinkStudioClientApp)
public:
  virtual BOOL InitInstance();
  virtual int ExitInstance();
  //}}AFX_VIRTUAL

  // Implementation

  //{{AFX_MSG(CMapLinkStudioClientApp)
  afx_msg void OnAppAbout();
  // NOTE - the ClassWizard will add and remove member functions here.
  //    DO NOT EDIT what you see in these blocks of generated code !
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
private:

  BOOL InitATL();

  BOOL m_bATLInited;

protected:

  string m_maplinkHome;

  string m_maplinkIniFile;

  string m_projectFile;

  string m_sourceName;

  string m_templateName;

  string m_coordSystem;

};

extern CMapLinkStudioClientApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFC_H__CF7BD0C6_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
