// SettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include ".\settingsdlg.h"


// CSettingsDlg dialog

IMPLEMENT_DYNAMIC(CSettingsDlg, CDialog)
CSettingsDlg::CSettingsDlg(CString & dllName, 
               CString & unlockCode,
               CString & settingsFile,
               bool alreadyUnlocked,
               bool hideUnlockCode,
               CWnd* pParent /*=NULL*/)
	: CDialog(CSettingsDlg::IDD, pParent)
        , m_unlockCode(unlockCode)
        , m_dllName(dllName)
        , m_settingsFile(settingsFile)
        , m_hideUnlockCode(hideUnlockCode)
        , m_alreadyUnlocked(alreadyUnlocked)
{
}

CSettingsDlg::~CSettingsDlg()
{
}

void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSettingsDlg, CDialog)
END_MESSAGE_MAP()


// CSettingsDlg message handlers

BOOL CSettingsDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  CEdit* dllEdit = (CEdit*) GetDlgItem(IDC_DLL_NAME);
  CEdit* unlockEdit = (CEdit*) GetDlgItem(IDC_UNLOCK_CODE);
  CEdit* settingEdit = (CEdit*) GetDlgItem(IDC_SETTINGS_FILE);
  if ( m_hideUnlockCode )
  {
    unlockEdit->SetWindowText(_T("???"));
  }
  else
  {
    unlockEdit->SetWindowText(m_unlockCode);
  }
  dllEdit->SetWindowText(m_dllName);
  settingEdit->SetWindowText(m_settingsFile);
  if ( m_alreadyUnlocked || m_hideUnlockCode )
  {
    unlockEdit->EnableWindow(FALSE);
  }


  return TRUE; 
}

void CSettingsDlg::OnOK()
{
  CEdit* dllEdit = (CEdit*) GetDlgItem(IDC_DLL_NAME);
  CEdit* unlockEdit = (CEdit*) GetDlgItem(IDC_UNLOCK_CODE);
  CEdit* settingEdit = (CEdit*) GetDlgItem(IDC_SETTINGS_FILE);

  dllEdit->GetWindowText(m_dllName);
  if ( !m_hideUnlockCode )
  {
    unlockEdit->GetWindowText(m_unlockCode);
  }
  settingEdit->GetWindowText(m_settingsFile);

  CDialog::OnOK();
}
