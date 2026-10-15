#pragma once


// CECPIDPCredentials dialog

class CECPIDPCredentials : public CDialog
{
  DECLARE_DYNAMIC(CECPIDPCredentials)

public:
  CECPIDPCredentials(CWnd* pParent, const TSLWMSECPServiceSettingsCallbacks::IDPOrganisation* organization);   // standard constructor
  virtual ~CECPIDPCredentials();

  // Dialog Data
  enum { IDD = IDD_ECP_CRED_DLG };

  const TSLSimpleString& username() const
  {
    return m_username;
  }

  const TSLSimpleString& password() const
  {
    return m_password;
  }

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  TSLSimpleString m_username;
  TSLSimpleString m_password;

  DECLARE_MESSAGE_MAP()
  virtual void OnOK();
public:
  virtual BOOL OnInitDialog();
};
