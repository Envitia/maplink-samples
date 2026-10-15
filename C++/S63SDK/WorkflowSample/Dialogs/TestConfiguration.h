#pragma once
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "afxdtctl.h"
#include "afxwin.h"

/////////////////////////////////////
// Test Plan Reference: 3.4.1.1
//  - Dialog for configuring the Workflow Sample
/////////////////////////////////////

class TestConfiguration : public CDialog
{
  DECLARE_DYNAMIC(TestConfiguration)

public:

  explicit TestConfiguration(S63Workflow * workFlow, CWnd* pParent = NULL);
  virtual ~TestConfiguration();

  //Overrides
  BOOL OnInitDialog();

  //Dialog Variables 
  CString m_s57ConfigFile;
  CString m_logFile;
  CString m_unlockKey;
  CString m_hw_id;

  bool m_logPermanentWarnings;
  bool m_displayIngestDialog;
  long long m_configurationTime;
  long m_cacheSize;

  //Controls
  CEdit m_unlockInput;
  CEdit m_cacheInput;
  CEdit m_hardwareInput;
  CButton m_permanentWarnings;
  CButton m_ingestCellCheck;
  CDateTimeCtrl m_expiryDateInput;

  //ID
  enum { IDD = IDD_WORKFLOW_SETUP };

  //Events
  afx_msg void OnBnClickedBrowse();
  afx_msg void OnBnClickedS57Browse();
  afx_msg void OnBnClickedDisplayIngest();
  afx_msg void OnBnClickedDisableWarnings();
  afx_msg void OnBnClickedOk();

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()

private:

  S63Workflow * m_workFlow;

};
