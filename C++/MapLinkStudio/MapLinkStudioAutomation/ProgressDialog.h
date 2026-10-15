#if !defined(AFX_PROGRESSDIALOG_H__466F9754_2D2F_4965_8BFA_A7084EB1489A__INCLUDED_)
#define AFX_PROGRESSDIALOG_H__466F9754_2D2F_4965_8BFA_A7084EB1489A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProgressDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProgressDialog dialog

class CProgressDialog : public CDialog
{
  // Construction
public:
  CProgressDialog(CWnd* pParent = NULL);   // standard constructor

  bool cancelled()
  {
    return m_cancelled;
  }

  // Dialog Data
  //{{AFX_DATA(CProgressDialog)
  enum { IDD = IDD_PROGRESS_DIALOG };
  CProgressCtrl	m_progressBar3;
  CProgressCtrl	m_progressBar2;
  CProgressCtrl	m_progressBar1;
  CString	m_text1;
  CString	m_text2;
  CString	m_text3;
  //}}AFX_DATA


  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CProgressDialog)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CProgressDialog)
  virtual void OnCancel();
  virtual BOOL OnInitDialog();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()

  bool m_cancelled;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGRESSDIALOG_H__466F9754_2D2F_4965_8BFA_A7084EB1489A__INCLUDED_)
