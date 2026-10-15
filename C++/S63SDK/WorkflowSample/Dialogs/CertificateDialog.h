#pragma once
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************
#include "../S63WorkFlowSample.h"

  /////////////////////////////////////
  // Test Plan Reference: 3.4.2.1
  //  - Provide a means of installing certificates 
  /////////////////////////////////////

class CertificateDialog : public CDialog
{
  DECLARE_DYNAMIC(CertificateDialog)

public:

  CertificateDialog(S63Workflow * workFlow, const bool dataServerCertificate, CWnd* pParent = NULL);   // standard constructor
  virtual ~CertificateDialog();

  //ID
  enum { IDD = IDD_CERTIFICATE_DIALOG };

  //Variables
  CString m_certificatePath;
  CString m_expiryDate;

  //Events
  afx_msg void OnBnClickedCertificateBrowse();
  afx_msg void OnBnClickedOk();

  //Controls
  CEdit m_certificateInput;
  CStatic m_certificateInformation;
  CStatic m_validFrom;
  CStatic m_validTo;
  CButton m_okButton;

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()

private:

  CertificateDialog(); // not required

  void updateCertificateInformation();

  S63Workflow * m_workFlow;

  bool m_dataServerCertificate;

};
