#pragma once
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

// LoadMedia dialog

class LoadS57Dialog : public CDialog
{
  DECLARE_DYNAMIC(LoadS57Dialog)

public:

  explicit LoadS57Dialog(S63Workflow * workFlow, CWnd* pParent = NULL);   // standard constructor
  virtual ~LoadS57Dialog();

  //Overrides
  BOOL OnInitDialog();

  //ID
  enum { IDD = IDD_LOAD_S57 };

  //Events
  afx_msg void OnBnClickedStoreBrowse();

  //Methods
  CString & dataStore();

  //Controls
  CEdit m_storeInput;
  
  //Dialog Variables
  CString m_dataStore;

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()

private:

  LoadS57Dialog(); // not required
  
  S63Workflow * m_workFlow;
  static string m_initialFolder;
public:

};
