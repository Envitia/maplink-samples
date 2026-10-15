// C2DemoPanelView.cpp : implementation file
//

#include "stdafx.h"

#include "C2DemoPanelView.h"
#include "resource.h"
#include "TrackInfo.h"
#include "TrackManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CC2DemoPanelView

IMPLEMENT_DYNCREATE(CC2DemoPanelView, CView)

CC2DemoPanelView::CC2DemoPanelView()
: m_pTrackInfo( 0 )
{
}

CC2DemoPanelView::~CC2DemoPanelView()
{
  if ( m_pTrackInfo )
  {
    delete m_pTrackInfo ;
    m_pTrackInfo = 0 ;
  }
}


BEGIN_MESSAGE_MAP(CC2DemoPanelView, CView)
	//{{AFX_MSG_MAP(CC2DemoPanelView)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CC2DemoPanelView drawing

void CC2DemoPanelView::OnDraw(CDC* pDC)
{
	//CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CC2DemoPanelView diagnostics

#ifdef _DEBUG
void CC2DemoPanelView::AssertValid() const
{
	CView::AssertValid();
}

void CC2DemoPanelView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CC2DemoPanelView message handlers

void CC2DemoPanelView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{

  if ( lHint == HINT_UPDATE_SELECTED_TRACK && CTrackManager::selectedTrack() )
  { 
    if ( !m_pTrackInfo )
      createInfoPanel() ;

    m_pTrackInfo->refreshData() ;
  }

}

void CC2DemoPanelView::OnSize(UINT nType, int cx, int cy) 
{
  CView::OnSize(nType, cx, cy);

  if ( !m_pTrackInfo )
    return;

  CRect rc ;
  GetClientRect( &rc ) ;

  m_pTrackInfo->SetWindowPos( NULL, rc.left, rc.top, cx, cy, SWP_NOZORDER | SWP_NOACTIVATE ) ;
	
}

void CC2DemoPanelView::createInfoPanel()
{
  if ( !m_pTrackInfo )
  {
    m_pTrackInfo = new CTrackInfo( this ) ;
    m_pTrackInfo->Create( CTrackInfo::IDD, this ) ;
    m_pTrackInfo->ShowWindow( SW_SHOW ) ;
    // Set the size of the dialog to fit in the view
    CRect rc ;
    GetClientRect( &rc ) ;
    m_pTrackInfo->SetWindowPos( NULL, rc.left, rc.top, rc.right, rc.bottom, SWP_NOZORDER | SWP_NOACTIVATE ) ;
  }

}
