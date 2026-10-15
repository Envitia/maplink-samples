// NumberOfTracksDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NumberOfTracksDlg.h"


// CNumberOfTracksDlg dialog

IMPLEMENT_DYNAMIC(CNumberOfTracksDlg, CDialog)
CNumberOfTracksDlg::CNumberOfTracksDlg(CWnd* pParent /*=NULL*/, int initialNoOfTracks)
: CDialog(CNumberOfTracksDlg::IDD, pParent)
, m_noOfTracks ( initialNoOfTracks )
{
}

CNumberOfTracksDlg::~CNumberOfTracksDlg()
{
}

void CNumberOfTracksDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNumberOfTracksDlg, CDialog)
  ON_BN_CLICKED(IDOK, OnOkClicked)
END_MESSAGE_MAP()


// CNumberOfTracksDlg message handlers

void CNumberOfTracksDlg::OnOkClicked()
{
  CEdit* editBox = (CEdit*) GetDlgItem(IDC_NO_TRACKS);
  TCHAR buffer[20];
  editBox->GetWindowText(&buffer[0], 20);
  m_noOfTracks = _ttoi(&buffer[0]);
  OnOK();
}

int CNumberOfTracksDlg::getNoOfTracks ()
{
  return m_noOfTracks;
}

BOOL CNumberOfTracksDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  CString formatter;
  formatter.Format(_T("%d"), m_noOfTracks);
  CEdit* editBox = (CEdit*) GetDlgItem(IDC_NO_TRACKS);
  editBox->SetWindowText(formatter);
  editBox->SetActiveWindow();

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}
