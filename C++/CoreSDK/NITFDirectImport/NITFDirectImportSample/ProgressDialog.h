#pragma once


// CProgressDialog dialog

class CProgressDialog : public CDialog
{
  DECLARE_DYNAMIC(CProgressDialog)

public:
  CProgressDialog(CWnd* pParent = NULL);   // standard constructor
  virtual ~CProgressDialog();

  // Dialog Data
  enum { IDD = IDD_PROGRESS_DIALOG };

  afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
  static bool staticUserCallback ( void * arg, TSLFilterProgress callbackType, double percentage);

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()

  bool nonstaticUserCallback ( TSLFilterProgress callbackReason, double percentage);
  
};
