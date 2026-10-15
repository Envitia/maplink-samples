// DeclutterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "DeclutterDlg.h"


// DeclutterDlg dialog

IMPLEMENT_DYNAMIC(DeclutterDlg, CDialog)
DeclutterDlg::DeclutterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(DeclutterDlg::IDD, pParent)
        , m_hideText(FALSE)
        , m_hideGraphics(FALSE)
        , m_hideFrame(FALSE)
{
  //{{AFX_DATA_INIT(DeclutterDlg)
  //}}AFX_DATA_INIT
}

DeclutterDlg::~DeclutterDlg()
{
}

void DeclutterDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(DeclutterDlg)
  DDX_Check(pDX, IDC_HIDE_TEXT, m_hideText);
  DDX_Check(pDX, IDC_HIDE_GRAPHICS, m_hideGraphics);
  DDX_Check(pDX, IDC_HIDE_FRAME, m_hideFrame);
  //}}AFX_DATA_MAP
}

void DeclutterDlg::OnOK()
{
  UpdateData() ; 
  CDialog::OnOK() ;
}


BEGIN_MESSAGE_MAP(DeclutterDlg, CDialog)
//{{AFX_MSG_MAP(DeclutterDlg)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// DeclutterDlg message handlers
