// RemoteOpenDlg.cpp : implementation file
//

#include "stdafx_tlt.h"
#include "RemoteOpenDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRemoteOpenDlg dialog


CRemoteOpenDlg::CRemoteOpenDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRemoteOpenDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRemoteOpenDlg)
	m_Location = _T("http://");
	//}}AFX_DATA_INIT
}


void CRemoteOpenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRemoteOpenDlg)
	DDX_Text(pDX, IDC_LOCATION, m_Location);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRemoteOpenDlg, CDialog)
	//{{AFX_MSG_MAP(CRemoteOpenDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRemoteOpenDlg message handlers
