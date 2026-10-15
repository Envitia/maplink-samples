// DirectoriesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include ".\directoriesdlg.h"


// CDirectoriesDlg dialog

IMPLEMENT_DYNAMIC(CDirectoriesDlg, CDialog)
CDirectoriesDlg::CDirectoriesDlg(CString & attachDir,
                                 CString & outputDir,
                                 CString & tempDir,
                                 CWnd* pParent /*=NULL*/)
	: CDialog(CDirectoriesDlg::IDD, pParent)
        , m_attachDir(attachDir)
        , m_outputDir(outputDir)
        , m_tempDir(tempDir)
{
}

CDirectoriesDlg::~CDirectoriesDlg()
{
}

void CDirectoriesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDirectoriesDlg, CDialog)
END_MESSAGE_MAP()


// CDirectoriesDlg message handlers

BOOL CDirectoriesDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  CEdit* attachEdit = (CEdit*) GetDlgItem(IDC_ATTACHMENTS_DIR);
  CEdit* outputEdit = (CEdit*) GetDlgItem(IDC_OUTPUT_DIR);
  CEdit* tempEdit = (CEdit*) GetDlgItem(IDC_TEMP_DIR);
  attachEdit->SetWindowText(m_attachDir);
  outputEdit->SetWindowText(m_outputDir);
  tempEdit->SetWindowText(m_tempDir);

  return TRUE;  
}

void CDirectoriesDlg::OnOK()
{
  CEdit* attachEdit = (CEdit*) GetDlgItem(IDC_ATTACHMENTS_DIR);
  CEdit* outputEdit = (CEdit*) GetDlgItem(IDC_OUTPUT_DIR);
  CEdit* tempEdit = (CEdit*) GetDlgItem(IDC_TEMP_DIR);

  attachEdit->GetWindowText( m_attachDir );
  outputEdit->GetWindowText( m_outputDir );
  tempEdit->GetWindowText( m_tempDir );
  
  CDialog::OnOK();
}
