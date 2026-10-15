#pragma once

#include <string>
using namespace std;
// CSettingsDlg dialog

class CSettingsDlg : public CDialog
{
  DECLARE_DYNAMIC(CSettingsDlg)

public:
  CSettingsDlg(CString & dllName, 
    CString & unlockCode,
    CString & settingsFile,
    bool alreadyUnlocked,
    bool hideUnlockCode = false,
    CWnd* pParent = NULL);

  virtual ~CSettingsDlg();

  virtual BOOL OnInitDialog();
  // Dialog Data
  enum { IDD = IDD_SETTINGS_DLG };

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual void OnOK();
  DECLARE_MESSAGE_MAP()

  CString & m_dllName;
  CString & m_unlockCode;
  CString & m_settingsFile;
  bool m_alreadyUnlocked;
  bool m_hideUnlockCode;
};
