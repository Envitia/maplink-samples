#pragma once

// CDirectoriesDlg dialog

class CDirectoriesDlg : public CDialog
{
  DECLARE_DYNAMIC(CDirectoriesDlg)

public:
  CDirectoriesDlg(CString & attachDir,
    CString & outputDir,
    CString & tempDir,
    CWnd* pParent = NULL);   // standard constructor
  virtual ~CDirectoriesDlg();

  // Dialog Data
  enum { IDD = IDD_DIR_DLG };
  virtual BOOL OnInitDialog();
protected:
  virtual void OnOK();
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()

  CString & m_attachDir;
  CString & m_outputDir;
  CString & m_tempDir;  
};
