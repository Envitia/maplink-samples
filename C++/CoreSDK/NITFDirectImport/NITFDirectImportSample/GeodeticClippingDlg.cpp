// ClippingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include ".\geodeticclippingdlg.h"


// CClippingDlg dialog

IMPLEMENT_DYNAMIC(CGeodeticClippingDlg, CDialog)
CGeodeticClippingDlg::CGeodeticClippingDlg(bool & applyClipping,
                                             double & minLat, 
                                             double & maxLat, 
                                             double & refMer, 
                                             double & degEast, 
                                             double & degWest,
                                             CWnd* pParent)
	: CDialog(CGeodeticClippingDlg::IDD, pParent)
        , m_applyClipping(applyClipping)
        , m_minLat(minLat)
        , m_maxLat(maxLat)
        , m_refMer(refMer)
        , m_degEast(degEast)
        , m_degWest(degWest)
{
}

CGeodeticClippingDlg::~CGeodeticClippingDlg()
{
}

void CGeodeticClippingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGeodeticClippingDlg, CDialog)
  ON_BN_CLICKED(IDC_CLIP, OnBnClickedClip)
END_MESSAGE_MAP()


// CClippingDlg message handlers

BOOL CGeodeticClippingDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  CButton* applyButton = (CButton*) GetDlgItem(IDC_CLIP);
  applyButton->SetCheck(m_applyClipping);

  CEdit* minLatEdit = (CEdit*)GetDlgItem(IDC_MIN_LAT);
  CEdit* maxLatEdit = (CEdit*)GetDlgItem(IDC_MAX_LAT);
  CEdit* refMerEdit = (CEdit*)GetDlgItem(IDC_REF_MER);
  CEdit* degEastEdit = (CEdit*)GetDlgItem(IDC_DEG_EAST);
  CEdit* degWestEdit = (CEdit*)GetDlgItem(IDC_DEG_WEST);

  
  minLatEdit->EnableWindow(m_applyClipping);
  maxLatEdit->EnableWindow(m_applyClipping);
  refMerEdit->EnableWindow(m_applyClipping);
  degEastEdit->EnableWindow(m_applyClipping);
  degWestEdit->EnableWindow(m_applyClipping);

  TCHAR xString[256] ;
  _stprintf( xString, _T("%10.6lf"), m_minLat ) ;
  minLatEdit->SetWindowText( xString ) ;
  _stprintf( xString, _T("%10.6lf"), m_maxLat ) ;
  maxLatEdit->SetWindowText( xString ) ;
  _stprintf( xString, _T("%10.6lf"), m_refMer ) ;
  refMerEdit->SetWindowText( xString ) ;
  _stprintf( xString, _T("%10.6lf"), m_degEast ) ;
  degEastEdit->SetWindowText( xString ) ;
  _stprintf( xString, _T("%10.6lf"), m_degWest ) ;
  degWestEdit->SetWindowText( xString ) ;

  return TRUE;  
}

void CGeodeticClippingDlg::OnOK()
{
  CButton* applyButton = (CButton*) GetDlgItem(IDC_CLIP);
  m_applyClipping = applyButton->GetCheck();

  CEdit* minLatEdit = (CEdit*)GetDlgItem(IDC_MIN_LAT);
  CEdit* maxLatEdit = (CEdit*)GetDlgItem(IDC_MAX_LAT);
  CEdit* refMerEdit = (CEdit*)GetDlgItem(IDC_REF_MER);
  CEdit* degEastEdit = (CEdit*)GetDlgItem(IDC_DEG_EAST);
  CEdit* degWestEdit = (CEdit*)GetDlgItem(IDC_DEG_WEST);

  TCHAR buffer[MAX_PATH];
  minLatEdit->GetWindowText(buffer, MAX_PATH);
  m_minLat = _tstof(buffer);
  maxLatEdit->GetWindowText(buffer, MAX_PATH);
  m_maxLat = _tstof(buffer);
  refMerEdit->GetWindowText(buffer, MAX_PATH);
  m_refMer = _tstof(buffer);
  degEastEdit->GetWindowText(buffer, MAX_PATH);
  m_degEast = _tstof(buffer);
  degWestEdit->GetWindowText(buffer, MAX_PATH);
  m_degWest = _tstof(buffer);

  CDialog::OnOK();
}

void CGeodeticClippingDlg::OnBnClickedClip()
{
  CButton* applyButton = (CButton*) GetDlgItem(IDC_CLIP);
  bool checked = applyButton->GetCheck();

  CEdit* minLatEdit = (CEdit*)GetDlgItem(IDC_MIN_LAT);
  CEdit* maxLatEdit = (CEdit*)GetDlgItem(IDC_MAX_LAT);
  CEdit* refMerEdit = (CEdit*)GetDlgItem(IDC_REF_MER);
  CEdit* degEastEdit = (CEdit*)GetDlgItem(IDC_DEG_EAST);
  CEdit* degWestEdit = (CEdit*)GetDlgItem(IDC_DEG_WEST);

  minLatEdit->EnableWindow(checked);
  maxLatEdit->EnableWindow(checked);
  refMerEdit->EnableWindow(checked);
  degEastEdit->EnableWindow(checked);
  degWestEdit->EnableWindow(checked);
}
