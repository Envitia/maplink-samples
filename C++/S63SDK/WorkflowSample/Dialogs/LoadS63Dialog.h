#pragma once
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

// LoadMedia dialog

class LoadS63Dialog : public CDialog
{
  DECLARE_DYNAMIC(LoadS63Dialog)

public:

  explicit LoadS63Dialog(S63Workflow * workFlow, CWnd* pParent = NULL);   // standard constructor
  virtual ~LoadS63Dialog();

  //Overrides
  BOOL OnInitDialog();

  //ID
  enum { IDD = IDD_LOAD_S63 };

  //Events
  afx_msg void OnBnClickedStoreBrowse();
  afx_msg void OnBnClickedAddPermit();
  afx_msg void OnBnClickedRemovePermit();
  afx_msg void OnBnClickedPermitBrowse();
  afx_msg void OnLbnSelchangePermitList();

  //Methods
  CString & dataStore();

  //Controls
  CEdit m_storeInput;
  CEdit m_hw_id_input;
  CEdit m_dataServerInput;
  CEdit m_permitInput;
  CListBox m_permitList;

  CStatic m_expiryLabel;
  CStatic m_dataServerLabel;
  CStatic m_cellNameLabel;

  //Dialog Variables
  CString m_dataStore;
  vector<pair<const TSLS63CellPermit*, string> > m_cellPermits;

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()

private:

  LoadS63Dialog(); // not required

  void updateCellPermits();

  void listItemToString(const string &listItem, string &cellName, string &dataServerID) const;

  S63Workflow * m_workFlow;

  static string m_initialFolder;
public:

};
