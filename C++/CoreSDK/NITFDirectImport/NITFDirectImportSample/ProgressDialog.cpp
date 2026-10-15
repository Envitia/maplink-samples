// ProgressDialog.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include ".\progressdialog.h"


// CProgressDialog dialog

IMPLEMENT_DYNAMIC(CProgressDialog, CDialog)
CProgressDialog::CProgressDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CProgressDialog::IDD, pParent)
{
}

CProgressDialog::~CProgressDialog()
{
}

void CProgressDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CProgressDialog, CDialog)
  ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


bool CProgressDialog::nonstaticUserCallback ( TSLFilterProgress callbackReason, double percentage)
{
  if ( !::IsWindow(this->m_hWnd) )
    return false;

  switch ( callbackReason )
  {
  case TSLFilterProgressLoadData:
    {
      CProgressCtrl* bar = (CProgressCtrl*) GetDlgItem(IDC_PROGRESS1);
      bar->SetPos((int)(percentage+0.5));
      break;
    }
  case TSLFilterProgressProcess:
    {
      CProgressCtrl* bar = (CProgressCtrl*) GetDlgItem(IDC_PROGRESS2);
      bar->SetPos((int)(percentage+0.5));
      break;
    }
  default:
    {
      break;
    }
  }
  SendMessage(WM_PAINT);
  return false;
}


bool CProgressDialog::staticUserCallback ( void * arg, TSLFilterProgress callbackType, double percentage)
{
  if ( arg )
  {
    return ((CProgressDialog*)arg)->nonstaticUserCallback(callbackType, percentage);
  }
  return true;
}
// CProgressDialog message handlers

void CProgressDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
  CDialog::OnShowWindow(bShow, nStatus);

  if ( bShow )
  {
    CProgressCtrl* bar1 = (CProgressCtrl*) GetDlgItem(IDC_PROGRESS1);
    CProgressCtrl* bar2 = (CProgressCtrl*) GetDlgItem(IDC_PROGRESS2);
    bar1->SetRange( 0, 100 );
    bar2->SetRange( 0, 100 );
  }
}
