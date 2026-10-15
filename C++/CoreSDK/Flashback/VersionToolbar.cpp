// VersionToolbar.cpp : implementation file
//

#include "stdafx.h"
#include "VersionToolbar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void CVersionCombo::RecalcDropWidth()
{
  // Reset the dropped width
  int nNumEntries = GetCount();
  int nWidth = 0;
  CString str;

  CClientDC dc(this);
  int nSave = dc.SaveDC();
  dc.SelectObject( GetFont() );

  int nScrollWidth = ::GetSystemMetrics( SM_CXVSCROLL );
  for ( int i = 0; i < nNumEntries; ++i )
  {
    GetLBText( i, str );
    int nLength = dc.GetTextExtent( str ).cx + nScrollWidth;
    nWidth = max( nWidth, nLength );
  }
  
  // Add margin space to the calculations
  nWidth += dc.GetTextExtent( "0" ).cx;

  dc.RestoreDC( nSave );
  SetDroppedWidth( nWidth );
}


/////////////////////////////////////////////////////////////////////////////
// CVersionToolbar dialog


CVersionToolbar::CVersionToolbar(CWnd* pParent /*=NULL*/)
  : CDialogBar	()
{
  //{{AFX_DATA_INIT(CVersionToolbar)
  //}}AFX_DATA_INIT
}


void CVersionToolbar::DoDataExchange(CDataExchange* pDX)
{
  CDialogBar::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CVersionToolbar)
  DDX_Control(pDX, IDC_VERSION_COMBO, m_versions);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVersionToolbar, CDialogBar)
  //{{AFX_MSG_MAP(CVersionToolbar)
  ON_CBN_SELCHANGE(IDC_VERSION_COMBO, OnSelChangeVersion)
  //}}AFX_MSG_MAP
  ON_MESSAGE( WM_INITDIALOG, OnInitDialog )
  ON_CBN_DROPDOWN(IDC_VERSION_COMBO, OnDropDownVersions)
END_MESSAGE_MAP()


bool CVersionToolbar::initialiseAndCreate( CFrameWnd * frame )
{
  if ( !Create( frame, IDD_VERSION, WS_CHILD | CBRS_ALIGN_TOP, IDD_VERSION ) )
  {
    return false;
  }

  SetWindowText( _T("Version Selector") );
  EnableDocking( CBRS_ALIGN_ANY );

  CRect rect;

  frame->GetClientRect( &rect );
  frame->ClientToScreen( &rect );

  rect.left   = rect.right - 20;
  rect.bottom = rect.top - 1;

  frame->DockControlBar( this, CBRS_ALIGN_TOP, &rect );

  return true;
}

/////////////////////////////////////////////////////////////////////////////
// CVersionToolbar message handlers

LRESULT CVersionToolbar::OnInitDialog(WPARAM wParam, LPARAM lParam)
{
  HandleInitDialog( wParam, lParam );

  if ( !UpdateData( FALSE ) )
  {
    TRACE0( "Warning: UpdateData failed during dialog init.\n" );
    return FALSE;
  }

  m_versions.AddString( _T("Current") );
  m_versions.SetCurSel( 0 );

  return TRUE;  // return TRUE unless you set the focus to a control
                // EXCEPTION: OCX Property Pages should return FALSE
}

void CVersionToolbar::OnSelChangeVersion() 
{
  UpdateData( TRUE );

  int version = m_versions.GetCurSel();

  CWnd* pMainWnd = AfxGetMainWnd();
  if ( pMainWnd->IsWindowVisible() )
  {
    pMainWnd->SendMessage( UM_SELECT_VERSION, 0, (LPARAM)version );
  }
}

void CVersionToolbar::OnDropDownVersions()
{
  m_versions.RecalcDropWidth();
}

void CVersionToolbar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
  UpdateDialogControls( pTarget, FALSE );
} 

void CVersionToolbar::updateVersions(TSLVersionHistorySet const* history)
{
  m_versions.ResetContent();
  m_versions.AddString( _T("Current") );
  m_versions.SetCurSel( 0 );

  if ( history )
  {
    bool isEmpty = history->empty();
    int N = history->size();

    if ( !isEmpty )
    {
      for ( int i( 0 ); i < N; ++i )
      {
        TSLHistoryVersion version;
        TSLHistoryTimestamp timestamp;
		const TSLVersionUserInfo * userInfo = 0;
        history->getHistory( i, version, timestamp, userInfo );

        CString txt;
        txt.Format( _T("%d"), version );
        m_versions.AddString( (LPCTSTR)txt );
      }
    }
  }
}
