// ProgressDialog.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "ProgressDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgressDialog dialog


CProgressDialog::CProgressDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CProgressDialog::IDD, pParent), m_cancelled(false)
{
	//{{AFX_DATA_INIT(CProgressDialog)
	m_text1 = _T("");
	m_text2 = _T("");
	m_text3 = _T("");
	//}}AFX_DATA_INIT
}


void CProgressDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProgressDialog)
	DDX_Control(pDX, IDC_PROGRESS3, m_progressBar3);
	DDX_Control(pDX, IDC_PROGRESS2, m_progressBar2);
	DDX_Control(pDX, IDC_PROGRESS1, m_progressBar1);
	DDX_Text(pDX, IDC_TEXT1, m_text1);
	DDX_Text(pDX, IDC_TEXT2, m_text2);
	DDX_Text(pDX, IDC_TEXT3, m_text3);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProgressDialog, CDialog)
	//{{AFX_MSG_MAP(CProgressDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgressDialog message handlers

void CProgressDialog::OnCancel() 
{
	// TODO: Add extra cleanup here
        m_cancelled = true;
	
	CDialog::OnCancel();
}

BOOL CProgressDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
        m_cancelled = false;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
