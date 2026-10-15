#pragma once
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

// ManageCellPermitsDialog dialog

class ManageCellPermitsDialog : public CDialog
{
  DECLARE_DYNAMIC(ManageCellPermitsDialog)

public:

  explicit ManageCellPermitsDialog(S63Workflow * workFlow, CWnd* pParent = NULL);   // standard constructor
  virtual ~ManageCellPermitsDialog();

  //Overrides
  BOOL OnInitDialog();

  //ID
  enum { IDD = IDD_CELLPERMIT_DIALOG };

  //Variables
  CString m_cellPermitPath;

  //Events
  afx_msg void OnBnClickedAddPermit();
  afx_msg void OnBnClickedPermitBrowse();
  afx_msg void OnLbnSelchangePermitList();
  afx_msg void OnBnClickedRemovePermit();

  //Controls
  CStatic m_expiryLabel;
  CStatic m_dataServerLabel;
  CStatic m_cellNameLabel;

  CEdit m_cellPermitInput;
  CEdit m_dataServerInput;

  CListBox m_cellPermitList;

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()

private:

  ManageCellPermitsDialog(); // not required

  S63Workflow * m_workFlow ;

  void updateCellPermits() ;

  void listItemToString(const string &listItem, string &cellName, string &dataServerID) const;

};
