// C2PerformanceDemoPanelView.cpp : implementation file
//

#include "stdafx.h"
#include "C2PerformanceDemoPanelView.h"
#include "TrackInfo.h"
#include "TrackManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CC2PerformanceDemoPanelView

IMPLEMENT_DYNCREATE(CC2PerformanceDemoPanelView, CView)

CC2PerformanceDemoPanelView::CC2PerformanceDemoPanelView()
: m_pTrackInfo( 0 )
, m_isAPP6A(0)
{
}

CC2PerformanceDemoPanelView::~CC2PerformanceDemoPanelView()
{
  if ( m_pTrackInfo )
  {
    delete m_pTrackInfo ;
    m_pTrackInfo = 0 ;
  }
}


BEGIN_MESSAGE_MAP(CC2PerformanceDemoPanelView, CView)
	//{{AFX_MSG_MAP(CC2PerformanceDemoPanelView)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CC2PerformanceDemoPanelView drawing

void CC2PerformanceDemoPanelView::OnDraw(CDC* pDC)
{
}

/////////////////////////////////////////////////////////////////////////////
// CC2PerformanceDemoPanelView diagnostics

#ifdef _DEBUG
void CC2PerformanceDemoPanelView::AssertValid() const
{
	CView::AssertValid();
}

void CC2PerformanceDemoPanelView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CC2PerformanceDemoPanelView message handlers

void CC2PerformanceDemoPanelView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{

  if ( lHint == HINT_UPDATE_SELECTED_TRACK )
  {
    if ( CTrackManager::selectedTrack() )
    { 
      if ( !m_pTrackInfo )
        createInfoPanel() ;

      m_pTrackInfo->refreshData() ;
    }
    else if ( m_pTrackInfo )
    {
      delete m_pTrackInfo ;
      m_pTrackInfo = 0 ;
    }
  }

}

void CC2PerformanceDemoPanelView::OnSize(UINT nType, int cx, int cy) 
{
  CView::OnSize(nType, cx, cy);

  if ( !m_pTrackInfo )
    return;

  CRect rc ;
  GetClientRect( &rc ) ;

  m_pTrackInfo->SetWindowPos( NULL, rc.left, rc.top, cx, cy, SWP_NOZORDER | SWP_NOACTIVATE ) ;
	
}

void CC2PerformanceDemoPanelView::createInfoPanel()
{
  if ( !m_pTrackInfo )
  {
    m_pTrackInfo = new CTrackInfo( this ) ;
    m_pTrackInfo->Create( CTrackInfo::IDD, this ) ;
    m_pTrackInfo->ShowWindow( SW_SHOW ) ;
    if ( m_isAPP6A )
      m_pTrackInfo->setSymbolType(true);
    // Set the size of the dialog to fit in the view
    CRect rc ;
    GetClientRect( &rc ) ;
    m_pTrackInfo->SetWindowPos( NULL, rc.left, rc.top, rc.right, rc.bottom, SWP_NOZORDER | SWP_NOACTIVATE ) ;
  }

}

void CC2PerformanceDemoPanelView::setSymbolType(bool isAPP6A) 
{
  m_isAPP6A = isAPP6A;
}
