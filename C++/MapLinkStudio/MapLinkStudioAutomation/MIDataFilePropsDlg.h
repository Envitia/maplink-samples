#if !defined(AFX_MIDATAFILEPROPSDLG_H__B46CCEFE_FA25_48EB_A47E_0FDA793F3628__INCLUDED_)
#define AFX_MIDATAFILEPROPSDLG_H__B46CCEFE_FA25_48EB_A47E_0FDA793F3628__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MIDataFilePropsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMIDataFilePropsDlg dialog

class CMIDataFilePropsDlg : public CDialog
{
  // Construction
public:
  CMIDataFilePropsDlg(CWnd* pParent, datafileInfo& fileInfo, vector<string>& libraryNames, vector<string>& resolutionNames);

  // Dialog Data
  //{{AFX_DATA(CMIDataFilePropsDlg)
  enum { IDD = IDD_DATAFILE_PROPS };
  CEdit	m_datafile_tr_lon;
  CEdit	m_datafile_tr_lat;
  CEdit	m_datafile_bl_lon;
  CEdit	m_datafile_bl_lat;
  CComboBox	m_resolution;
  CComboBox	m_library;
  //}}AFX_DATA

  datafileInfo m_datafileInfo;

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CMIDataFilePropsDlg)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CMIDataFilePropsDlg)
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()

  vector<string> m_libraryNames;
  vector<string> m_resolutionNames;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIDATAFILEPROPSDLG_H__B46CCEFE_FA25_48EB_A47E_0FDA793F3628__INCLUDED_)
