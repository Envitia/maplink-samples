// ClippingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include ".\clippingdlg.h"


// CClippingDlg dialog

IMPLEMENT_DYNAMIC(CClippingDlg, CDialog)
CClippingDlg::CClippingDlg(bool & applyClipping,
                           double & minX, 
                           double & minY, 
                           double & maxX, 
                           double & maxY, 
                           CWnd* pParent /*=NULL*/)
	: CDialog(CClippingDlg::IDD, pParent)
        , m_applyClipping(applyClipping)
        , m_minX(&minX)
        , m_minY(&minY)
        , m_maxX(&maxX)
        , m_maxY(&maxY)
{
}


CClippingDlg::~CClippingDlg()
{
}

void CClippingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CClippingDlg, CDialog)
  ON_BN_CLICKED(IDC_CLIP, OnBnClickedClip)
END_MESSAGE_MAP()


// CClippingDlg message handlers

BOOL CClippingDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  CButton* applyButton = (CButton*) GetDlgItem(IDC_CLIP);
  applyButton->SetCheck(m_applyClipping);

  CEdit* minXEdit = (CEdit*)GetDlgItem(IDC_C_MIN_X);
  CEdit* minYEdit = (CEdit*)GetDlgItem(IDC_C_MIN_Y);
  CEdit* maxXEdit = (CEdit*)GetDlgItem(IDC_C_MAX_X);
  CEdit* maxYEdit = (CEdit*)GetDlgItem(IDC_C_MAX_Y);

  minXEdit->EnableWindow(m_applyClipping);
  minYEdit->EnableWindow(m_applyClipping);
  maxXEdit->EnableWindow(m_applyClipping);
  maxYEdit->EnableWindow(m_applyClipping);

  TCHAR xString[256] ;

  _stprintf( xString, _T("%10.6lf"), *m_minX ) ;
  minXEdit->SetWindowText( xString ) ;
  _stprintf( xString, _T("%10.6lf"), *m_minY ) ;
  minYEdit->SetWindowText( xString ) ;
  _stprintf( xString, _T("%10.6lf"), *m_maxX ) ;
  maxXEdit->SetWindowText( xString ) ;
  _stprintf( xString, _T("%10.6lf"), *m_maxY ) ;
  maxYEdit->SetWindowText( xString ) ;

  return TRUE;  
}

void CClippingDlg::OnOK()
{
  CButton* applyButton = (CButton*) GetDlgItem(IDC_CLIP);
  m_applyClipping = applyButton->GetCheck();

  CEdit* minXEdit = (CEdit*)GetDlgItem(IDC_C_MIN_X);
  CEdit* minYEdit = (CEdit*)GetDlgItem(IDC_C_MIN_Y);
  CEdit* maxXEdit = (CEdit*)GetDlgItem(IDC_C_MAX_X);
  CEdit* maxYEdit = (CEdit*)GetDlgItem(IDC_C_MAX_Y);

  TCHAR buffer[MAX_PATH];
  minXEdit->GetWindowText(buffer, MAX_PATH);
  *m_minX = _tstof(buffer);
  minYEdit->GetWindowText(buffer, MAX_PATH);
  *m_minY = _tstof(buffer);
  maxXEdit->GetWindowText(buffer, MAX_PATH);
  *m_maxX = _tstof(buffer);
  maxYEdit->GetWindowText(buffer, MAX_PATH);
  *m_maxY = _tstof(buffer);

  CDialog::OnOK();
}

void CClippingDlg::OnBnClickedClip()
{
  CButton* applyButton = (CButton*) GetDlgItem(IDC_CLIP);
  bool checked = applyButton->GetCheck();

  CEdit* minXEdit = (CEdit*)GetDlgItem(IDC_C_MIN_X);
  CEdit* minYEdit = (CEdit*)GetDlgItem(IDC_C_MIN_Y);
  CEdit* maxXEdit = (CEdit*)GetDlgItem(IDC_C_MAX_X);
  CEdit* maxYEdit = (CEdit*)GetDlgItem(IDC_C_MAX_Y);

  minXEdit->EnableWindow(checked);
  minYEdit->EnableWindow(checked);
  maxXEdit->EnableWindow(checked);
  maxYEdit->EnableWindow(checked);
}
