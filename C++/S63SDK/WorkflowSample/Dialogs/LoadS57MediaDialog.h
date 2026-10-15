#pragma once
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

////////////////////////////////////////////////
////      TEST PLAN REFERENCE: 3.4.2.3      ////
////////////////////////////////////////////////

class LoadS57MediaDialog : public CDialog
{
  DECLARE_DYNAMIC(LoadS57MediaDialog)

public:

  explicit LoadS57MediaDialog(CWnd* pParent = NULL);   // standard constructor
  virtual ~LoadS57MediaDialog();

  //Events
  afx_msg void OnBnClickedCatalogBrowse();
  afx_msg void OnBnClickedStoreBrowse();

  //Variables
  CString m_dataStore;
  CString m_catalogFile;

  //Controls
  CEdit m_storeInput;
  CEdit m_catalogInput;

  //Methods
  CString & dataStore();

  //ID
  enum { IDD = IDD_LOAD_S57_MEDIA };

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()

  static string m_initialFolder;
};
