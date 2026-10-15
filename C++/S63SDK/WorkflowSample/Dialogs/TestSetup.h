#pragma once
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

// TestSetup dialog

/////////////////////////////////////
// Test Plan Reference: 3.4.1.2
//  - Dialog for configuring the IHO Mandated Tests
/////////////////////////////////////

class TestSetup : public CDialog
{
  DECLARE_DYNAMIC(TestSetup)

public:

  explicit TestSetup(CWnd* pParent = NULL);   // standard constructor
  virtual ~TestSetup();

  //Overrides
  BOOL OnInitDialog();

  //Dialog variables
  CString m_logFile;

  CString m_dataLocation;
  CString m_configurationDir;
  CString m_resultsDir;

  long long m_configurationTime;

  //Controls
  CEdit m_testDataInput;
  CEdit m_certificateInput;
  CEdit m_configurationInput;
  CEdit m_resultsInput;

  CDateTimeCtrl m_expiryDateInput;

  //ID
  enum { IDD = IDD_TEST_SETUP };

  //Events
  afx_msg void OnBnClickedDataLocationBrowse();
  afx_msg void OnBnClickedSaCertificateBrowse();
  afx_msg void OnDtnDatetimechangeExpiryDate(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnBnClickedConfigurationBrowse();
  afx_msg void OnBnClickedResultsBrowse();

  //Methods
  CString& dataStore();

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()

  static string m_initialFolder;
public:

};
