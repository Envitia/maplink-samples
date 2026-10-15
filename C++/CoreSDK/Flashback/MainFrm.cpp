// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Flashback.h"

#include "MainFrm.h"
#include "FlashbackDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
  //{{AFX_MSG_MAP(CMainFrame)
  // NOTE - the ClassWizard will add and remove mapping macros here.
  //    DO NOT EDIT what you see in these blocks of generated code !
  ON_WM_CREATE()
  ON_MESSAGE(UM_SELECT_VERSION, OnSelectVersion)
  ON_MESSAGE(UM_AVAILABLE_VERSIONS, OnAvailableVersions)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
  ID_SEPARATOR,           // status line indicator
    ID_INDICATOR_CAPS,
    ID_INDICATOR_NUM,
    ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
  // TODO: add member initialization code here

}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
    return -1;

  if (!m_wndToolBar.Create(this) ||
    !m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
  {
    TRACE0("Failed to create toolbar\n");
    return -1;      // fail to create
  }

  if (!m_wndStatusBar.Create(this) ||
    !m_wndStatusBar.SetIndicators(indicators,
    sizeof(indicators)/sizeof(UINT)))
  {
    TRACE0("Failed to create status bar\n");
    return -1;      // fail to create
  }

  // TODO: Remove this if you don't want tool tips or a resizeable toolbar
  m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
    CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

  // TODO: Delete these three lines if you don't want the toolbar to
  //  be dockable
  m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
  EnableDocking(CBRS_ALIGN_ANY);
  DockControlBar(&m_wndToolBar);
  
  if ( !m_versionPanel.initialiseAndCreate( this ) )
  {
    TRACE0("Failed to create version panel\n");
    return -1;
  }

  // Docking code.
  {
    RecalcLayout();
    
    CRect rect;
    
    m_wndToolBar.GetWindowRect( &rect );
    rect.OffsetRect( 1, 0 );
    
    DWORD dw = m_wndToolBar.GetBarStyle();
    
    UINT n = 0;
    
    n = ( dw & CBRS_ALIGN_TOP            ) ? AFX_IDW_DOCKBAR_TOP    : n;
    n = ( dw & CBRS_ALIGN_BOTTOM && n==0 ) ? AFX_IDW_DOCKBAR_BOTTOM : n;
    n = ( dw & CBRS_ALIGN_LEFT   && n==0 ) ? AFX_IDW_DOCKBAR_LEFT   : n;
    n = ( dw & CBRS_ALIGN_RIGHT  && n==0 ) ? AFX_IDW_DOCKBAR_RIGHT  : n;
    
    DockControlBar( &m_versionPanel, n, &rect );
  }

  return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
  // TODO: Modify the Window class or styles here by modifying
  //  the CREATESTRUCT cs

  return CFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
  CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
  CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
LRESULT CMainFrame::OnSelectVersion(WPARAM wparam, LPARAM lparam)
{
  CDocument* doc = GetActiveDocument();
  if ( doc )
  {
    FlashbackDoc* pDoc = (FlashbackDoc*)doc;
    TSLHistoryVersion version = (TSLHistoryVersion)lparam;
    pDoc->flashback( version );
  }

  return 0L;
}

LRESULT CMainFrame::OnAvailableVersions(WPARAM wParam, LPARAM lParam)
{
  TSLVersionHistorySet const* history = (TSLVersionHistorySet const*)lParam;
  m_versionPanel.updateVersions( history );

  return 0L;
}
