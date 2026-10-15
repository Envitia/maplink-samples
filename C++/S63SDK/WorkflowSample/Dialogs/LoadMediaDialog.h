#pragma once
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

////////////////////////////////////////////////
////      TEST PLAN REFERENCE: 3.4.2.3      ////
////////////////////////////////////////////////

class LoadMediaDialog : public CDialog
{
  DECLARE_DYNAMIC(LoadMediaDialog)

public:

  explicit LoadMediaDialog(CWnd* pParent = NULL);   // standard constructor
  virtual ~LoadMediaDialog();

  //Events
  afx_msg void OnBnClickedProductsBrowse();
  afx_msg void OnBnClickedStoreBrowse();
  afx_msg void OnBnClickedSerialBrowse();

  //Variables
  CString m_dataStore;
  CString m_productFile;
  CString m_serialFile;

  //Controls
  CEdit m_storeInput;
  CEdit m_productInput;
  CEdit m_serialInput;

  //Methods
  CString & dataStore();

  //ID
  enum { IDD = IDD_LOAD_MEDIA };

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()

  static string m_initialFolder;
};
