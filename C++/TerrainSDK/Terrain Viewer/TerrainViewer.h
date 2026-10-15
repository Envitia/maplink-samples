// TerrainViewer.h : main header file for the TerrainViewer application
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
// CTerrainViewerApp:
// See TerrainViewer.cpp for the implementation of this class
//

class CTerrainViewerApp : public CWinApp
{
public:
  
  CTerrainViewerApp();
  
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CTerrainViewerApp)
public:
  virtual BOOL InitInstance();
  //}}AFX_VIRTUAL
  
  // Implementation
  
  //{{AFX_MSG(CTerrainViewerApp)
  afx_msg void OnAppAbout();
  afx_msg void OnHelp();
  afx_msg void OnHelpContactEnvitia();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()

protected:

  std::string m_helpPath ;

};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFC_H__CF7BD0C6_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
