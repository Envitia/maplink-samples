#pragma once


// CECPIDPDlg dialog

class CECPIDPDlg : public CDialog
{
  DECLARE_DYNAMIC(CECPIDPDlg)

public:
  CECPIDPDlg(CWnd* pParent, unsigned int numberOfIDPs, const TSLWMSECPServiceSettingsCallbacks::IDPOrganisation* organisations[]);
  virtual ~CECPIDPDlg();

  // Dialog Data
  enum { IDD = IDD_ECP_IDP_DLG };

  virtual BOOL OnInitDialog();
  virtual void OnOK();

  unsigned int choosenIndex() const
  {
    return m_index;
  }

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()
private:
  CListCtrl m_list;
  unsigned int m_index;
  unsigned int m_numberOfIDPs;
  const TSLWMSECPServiceSettingsCallbacks::IDPOrganisation** m_organisations;
public:
  afx_msg void OnLvnItemchangedIdpList(NMHDR *pNMHDR, LRESULT *pResult);
};
