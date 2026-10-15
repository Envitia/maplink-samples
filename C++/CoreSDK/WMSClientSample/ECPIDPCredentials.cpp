// IDPCredentials.cpp : implementation file
//

#include "stdafx.h"
#include "WMSClientSample.h"
#include "ECPIDPCredentials.h"


// CECPIDPCredentials dialog

IMPLEMENT_DYNAMIC(CECPIDPCredentials, CDialog)

CECPIDPCredentials::CECPIDPCredentials(CWnd* pParent, const TSLWMSECPServiceSettingsCallbacks::IDPOrganisation* organization)
: CDialog(CECPIDPCredentials::IDD, pParent)
{

}

CECPIDPCredentials::~CECPIDPCredentials()
{
}

void CECPIDPCredentials::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CECPIDPCredentials, CDialog)
END_MESSAGE_MAP()

// CECPIDPCredentials message handlers

void CECPIDPCredentials::OnOK()
{
  CEdit* user = (CEdit*)GetDlgItem(IDC_USERNAME);
  CEdit* pass = (CEdit*)GetDlgItem(IDC_PASSWORD);
  CString temp;
  user->GetWindowText(temp);
  m_username = TSLUTF8Encoder(temp);
  pass->GetWindowText(temp);
  m_password = TSLUTF8Encoder(temp);
  CDialog::OnOK();
}

BOOL CECPIDPCredentials::OnInitDialog()
{
  CDialog::OnInitDialog();

  CEdit* user = (CEdit*)GetDlgItem(IDC_USERNAME);
  user->SetFocus();

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}
