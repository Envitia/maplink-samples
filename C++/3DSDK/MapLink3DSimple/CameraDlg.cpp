// CameraDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "CameraDlg.h"
// CCameraDlg dialog

IMPLEMENT_DYNAMIC(CCameraDlg, CDialog)
CCameraDlg::CCameraDlg(CWnd* pParent /*=NULL*/)
  : CDialog ( CCameraDlg::IDD, pParent ),
    m_xAxis ( 0.0 ),
    m_yAxis ( 0.0 ),
    m_zAxis ( 0.0 ),
    m_angle ( 0.0 ),
    m_fov   ( 45.0 )
{
}

CCameraDlg::~CCameraDlg()
{
}

void CCameraDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);

  DDX_Text(pDX, IDC_XAXIS, m_xAxis);
  DDX_Text(pDX, IDC_YAXIS, m_yAxis);
  DDX_Text(pDX, IDC_ZAXIS, m_zAxis);
  DDX_Text(pDX, IDC_ANGLE, m_angle);
  DDX_Text(pDX, IDC_FOV,   m_fov);
}


BEGIN_MESSAGE_MAP(CCameraDlg, CDialog)
END_MESSAGE_MAP()


// CCameraDlg message handlers


BOOL CCameraDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  UpdateData( FALSE );

  return TRUE;  // return TRUE unless you set the focus to a control
                // EXCEPTION: OCX Property Pages should return FALSE
}

void CCameraDlg::OnOK()
{
  UpdateData( TRUE );

  CDialog::OnOK();
}
