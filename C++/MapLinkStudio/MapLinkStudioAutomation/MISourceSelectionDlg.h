#if !defined(AFX_MISOURCESELECTIONDLG_H__1284F1DB_C6ED_4340_B478_BAB96578471B__INCLUDED_)
#define AFX_MISOURCESELECTIONDLG_H__1284F1DB_C6ED_4340_B478_BAB96578471B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MISourceSelectionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMISourceSelectionDlg dialog

class CMISourceSelectionDlg : public CDialog
{
  // Construction
public:
  CMISourceSelectionDlg(CWnd* pParent = NULL);   // standard constructor

  // Dialog Data
  //{{AFX_DATA(CMISourceSelectionDlg)
  enum { IDD = IDD_SOURCE_TYPE_SEL };
  CButton	m_OK;
  CComboBox	m_source_type;
  CListCtrl	m_project_list;
  //}}AFX_DATA


  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CMISourceSelectionDlg)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  CImageList* m_itemImageList;

  // sources (datasets)
  vector<string> m_sources;

  BOOL getDataset(string& filename, string& dataset);

  // Generated message map functions
  //{{AFX_MSG(CMISourceSelectionDlg)
  virtual void OnOK();
  afx_msg void OnSelchangeMiSourceType();
  virtual BOOL OnInitDialog();
  afx_msg void OnClickMiProjectList(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnDblclkMiProjectList(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnEditchangeMiSourceType();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MISOURCESELECTIONDLG_H__1284F1DB_C6ED_4340_B478_BAB96578471B__INCLUDED_)
