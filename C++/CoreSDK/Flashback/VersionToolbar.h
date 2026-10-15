#if !defined(AFX_VERSIONTOOLBAR_H__86622B53_1A8B_400E_B658_7A1464EB1253__INCLUDED_)
#define AFX_VERSIONTOOLBAR_H__86622B53_1A8B_400E_B658_7A1464EB1253__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VersionToolbar.h : header file

#include "resource.h"
//

/////////////////////////////////////////////////////////////////////////////
// CVersionToolbar dialog

class CVersionCombo : public CComboBox
{
public:
  void RecalcDropWidth();
};


class CVersionToolbar : public CDialogBar
{
  // Construction
public:
  // constructor
  CVersionToolbar(CWnd* pParent = 0);

  // creation
  bool initialiseAndCreate( CFrameWnd * frame ) ;

  //This is required to stop MFC from greying out all controls 
  //in the panel since there are no handlers in the parent class.
  virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler );

  void updateVersions(TSLVersionHistorySet const* history);

private:
  // Dialog Data
  //{{AFX_DATA(CFindToolbar)
  enum { IDD = IDD_VERSION };
  CVersionCombo  m_versions;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CFindToolbar)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL


  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CVersionToolbar)
  afx_msg void OnSelChangeVersion();
  //}}AFX_MSG
  afx_msg LRESULT OnInitDialog( WPARAM wParam, LPARAM lParam ) ;
  afx_msg void OnDropDownVersions();
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VERSIONTOOLBAR_H__86622B53_1A8B_400E_B658_7A1464EB1253__INCLUDED_)
