// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "NumberOfTracksDlg.h"
#include "mainfrm.h"
#include "C2PerformanceDemoView.h"
#include "C2PerformanceDemoToteView.h"
#include "C2PerformanceDemoTreeView.h"
#include "C2PerformanceDemoPanelView.h"
#include "C2PerformanceDemoDoc.h"
#include "TrackManager.h"
#include "Animator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// HTML help is not supported in Visual C++ 6
#if _MSC_VER > 1200
#pragma comment(lib, "htmlhelp.lib")
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
  //{{AFX_MSG_MAP(CMainFrame)
  ON_WM_CREATE()
  ON_COMMAND(ID_VIEW_RANGERINGS, OnViewRangerings)
  ON_COMMAND(ID_RANGE_100KM, OnRange100km)
  ON_COMMAND(ID_RANGE_10KM, OnRange10km)
  ON_COMMAND(ID_RANGE_200KM, OnRange200km)
  ON_COMMAND(ID_RANGE_500KM, OnRange500km)
  ON_COMMAND(ID_RANGE_50KM, OnRange50km)
  ON_COMMAND(ID_TOOLS_NORTHUP, OnToolsNorthup)
  ON_COMMAND(ID_TOOLS_TRACKHEADINGUP, OnToolsTrackheadingup)
  ON_COMMAND(ID_TOOLS_FOLLOWSELECTED, OnToolsFollowselected)
  ON_COMMAND(ID_HELP_CONTACT, OnHelpContact)
  ON_UPDATE_COMMAND_UI(ID_RANGE_100KM, OnUpdateRange100km)
  ON_UPDATE_COMMAND_UI(ID_RANGE_10KM, OnUpdateRange10km)
  ON_UPDATE_COMMAND_UI(ID_RANGE_200KM, OnUpdateRange200km)
  ON_UPDATE_COMMAND_UI(ID_RANGE_500KM, OnUpdateRange500km)
  ON_UPDATE_COMMAND_UI(ID_RANGE_50KM, OnUpdateRange50km)
  ON_COMMAND(ID_SIMULATION_10SPEED, OnSimulation10speed)
  ON_UPDATE_COMMAND_UI(ID_SIMULATION_10SPEED, OnUpdateSimulation10speed)
  ON_COMMAND(ID_SIMULATION_2SPEED, OnSimulation2speed)
  ON_UPDATE_COMMAND_UI(ID_SIMULATION_2SPEED, OnUpdateSimulation2speed)
  ON_COMMAND(ID_SIMULATION_50SPEED, OnSimulation50speed)
  ON_UPDATE_COMMAND_UI(ID_SIMULATION_50SPEED, OnUpdateSimulation50speed)
  ON_COMMAND(ID_SIMULATION_5SPEED, OnSimulation5speed)
  ON_UPDATE_COMMAND_UI(ID_SIMULATION_5SPEED, OnUpdateSimulation5speed)
  ON_COMMAND(ID_SIMULATION_REALTIME, OnSimulationRealtime)
  ON_UPDATE_COMMAND_UI(ID_SIMULATION_REALTIME, OnUpdateSimulationRealtime)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_FOLLOWSELECTED, OnUpdateToolsFollowselected)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_NORTHUP, OnUpdateToolsNorthup)
  ON_COMMAND(ID_TOOLS_PAN, OnToolsPan)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_PAN, OnUpdateToolsPan)
  ON_COMMAND(ID_TOOLS_RESET, OnToolsReset)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_RESET, OnUpdateToolsReset)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_TRACKHEADINGUP, OnUpdateToolsTrackheadingup)
  ON_COMMAND(ID_TOOLS_TRACKSELECT, OnToolsTrackselect)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_TRACKSELECT, OnUpdateToolsTrackselect)
  ON_COMMAND(ID_TOOLS_ZOOM, OnToolsZoom)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOM, OnUpdateToolsZoom)
  ON_COMMAND(ID_TOOLS_ZOOMIN, OnToolsZoomin)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOMIN, OnUpdateToolsZoomin)
  ON_COMMAND(ID_TOOLS_ZOOMOUT, OnToolsZoomout)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOMOUT, OnUpdateToolsZoomout)
  ON_UPDATE_COMMAND_UI(ID_VIEW_RANGERINGS, OnUpdateViewRangerings)
  ON_COMMAND(ID_VIEW_STATUS_BAR, OnViewStatusBar)
  ON_UPDATE_COMMAND_UI(ID_VIEW_STATUS_BAR, OnUpdateViewStatusBar)
  ON_COMMAND(ID_VIEW_TOOLBAR, OnViewToolbar)
  ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR, OnUpdateViewToolbar)
  ON_COMMAND(ID_APP_HELP, OnAppHelp)
  ON_UPDATE_COMMAND_UI(ID_APP_HELP, OnUpdateAppHelp)
  ON_COMMAND(ID_TOOLS_GRAB, OnToolsGrab)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_GRAB, OnUpdateToolsGrab)
  ON_UPDATE_COMMAND_UI(ID_SYMBOLS_STANDARD, OnUpdateSymbolType)
  ON_UPDATE_COMMAND_UI(ID_SYMBOLS_APP6A, OnUpdateSymbolType)
  ON_UPDATE_COMMAND_UI(ID_SYMBOLS_2525B, OnUpdateSymbolType)
  ON_COMMAND(ID_SYMBOLS_APP6A, OnSymbolsApp6a)
  ON_COMMAND(ID_SYMBOLS_2525B, OnSymbols2525B)
  ON_COMMAND(ID_SYMBOLS_STANDARD, OnSymbolsStandard)
  ON_COMMAND(ID_PERFORMANCE_START, OnPerformanceStart)
  ON_COMMAND(ID_PERFORMANCE_STOP, OnPerformanceStop)
  ON_UPDATE_COMMAND_UI(ID_PERFORMANCE_START, OnUpdatePerformance)
  ON_UPDATE_COMMAND_UI(ID_PERFORMANCE_STOP, OnUpdatePerformance)
  ON_COMMAND(ID_SYMBOLS_CHANGETHENUMBEROFTRACKS, OnSymbolsChangethenumberoftracks)
  ON_UPDATE_COMMAND_UI(ID_APP6AANNOTATIONLEVEL_NONE, OnUpdateApp6aAnnotationlevel)
  ON_UPDATE_COMMAND_UI(ID_APP6AANNOTATIONLEVEL_LOW, OnUpdateApp6aAnnotationlevel)
  ON_UPDATE_COMMAND_UI(ID_APP6AANNOTATIONLEVEL_MEDIUM, OnUpdateApp6aAnnotationlevel)
  ON_UPDATE_COMMAND_UI(ID_APP6AANNOTATIONLEVEL_HIGH, OnUpdateApp6aAnnotationlevel)
  ON_COMMAND(ID_APP6AANNOTATIONLEVEL_NONE, OnApp6aAnnotationlevelNone)
  ON_COMMAND(ID_APP6AANNOTATIONLEVEL_LOW, OnApp6aAnnotationlevelLow)
  ON_COMMAND(ID_APP6AANNOTATIONLEVEL_MEDIUM, OnApp6aAnnotationlevelMedium)
  ON_COMMAND(ID_APP6AANNOTATIONLEVEL_HIGH, OnApp6aAnnotationlevelHigh)  
  ON_UPDATE_COMMAND_UI(ID_APP6ARENDERING_BITMAP, OnUpdateApp6aRendering)
  ON_UPDATE_COMMAND_UI(ID_APP6ARENDERING_ENTITY, OnUpdateApp6aRendering)
  ON_COMMAND(ID_APP6ARENDERING_BITMAP, OnApp6aRenderingBitmap)
  ON_COMMAND(ID_APP6ARENDERING_ENTITY, OnApp6aRenderingEntity)
  ON_COMMAND(ID_CUR_FR, OnDoNothing)
  ON_COMMAND(ID_AVG_FR, OnDoNothing)
  //}}AFX_MSG_MAP

END_MESSAGE_MAP()

//non performance status bar indicators
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

  if (!m_wndPerformanceToolBar.Create(this) ||
      !m_wndPerformanceToolBar.LoadToolBar(IDR_PERFORMANCE_BAR) )
  {
    TRACE0("Failed to create toolbar\n");
    return -1;      // fail to create
  }

  CToolBarCtrl& tbctrl = m_wndPerformanceToolBar.GetToolBarCtrl();
  tbctrl.EnableButton(ID_BUTTON32822);


  if (!m_wndStatusBar.Create(this) ||
    !m_wndStatusBar.SetIndicators(indicators,
    sizeof(indicators)/sizeof(UINT)))
  {
    TRACE0("Failed to create status bar\n");
    return -1;      // fail to create
  }

  m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
    CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

  m_wndPerformanceToolBar.SetBarStyle(m_wndPerformanceToolBar.GetBarStyle() |
    CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

  // TODO: Delete these three lines if you don't want the toolbar to
  //  be dockable
  m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
  EnableDocking(CBRS_ALIGN_ANY);
  DockControlBar(&m_wndToolBar);

  m_wndPerformanceToolBar.EnableDocking(CBRS_ALIGN_ANY);
  DockControlBar(&m_wndPerformanceToolBar);

  return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
  //sets the size of the window
  cs.cx = GetSystemMetrics( SM_CXFULLSCREEN ) ;
  cs.cy = GetSystemMetrics( SM_CYFULLSCREEN ) + GetSystemMetrics( SM_CYCAPTION ) + GetSystemMetrics( SM_CYEDGE ) + GetSystemMetrics( SM_CYEDGE ) ;

  //positions the window on the screen
  cs.x = 0 ;
  cs.y = 0 ;

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

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
  if ( !m_windowSplitterMain.CreateStatic(this, 1, 2, WS_CHILD | WS_VISIBLE | WS_BORDER ) )
  {
    TRACE0("Failed to create top splitter\n");
    return FALSE;
  }

  if ( !m_windowSplitterDisplay.CreateStatic( &m_windowSplitterMain, 3, 1, 
    WS_CHILD | WS_VISIBLE | WS_BORDER, m_windowSplitterMain.IdFromRowCol( 0,1 ) ) )
  {
    TRACE0( "Failed to create display splitter\n" );
    return FALSE ;
  }

  // Get screen widths and heights for splittter
  int x = (int)((double)GetSystemMetrics( SM_CXFULLSCREEN )) ;
  int y = (int)((double)GetSystemMetrics( SM_CYFULLSCREEN )) ;

  // Now create the views in the splitters
  m_windowSplitterMain.CreateView( 0, 0, RUNTIME_CLASS( CC2PerformanceDemoView ), CSize( 100, 100 ), pContext ) ;

  // Create the nested splitter windows
  m_windowSplitterDisplay.CreateView( 0, 0, RUNTIME_CLASS( CC2PerformanceDemoTreeView ), CSize( 100, 50 ), pContext ) ;
  m_windowSplitterDisplay.CreateView( 1, 0, RUNTIME_CLASS( CC2PerformanceDemoToteView ), CSize( 100, 50 ), pContext ) ;
  m_windowSplitterDisplay.CreateView( 2, 0, RUNTIME_CLASS( CC2PerformanceDemoPanelView ), CSize( 100, 50 ), pContext ) ;

  CRect	rect;
  GetClientRect( &rect );

  // Set the ideal and minimum heights for the main map display
  m_windowSplitterMain.SetRowInfo( 0, y, 0 ) ;

  // Set the ideal and minimum heights for rows in the display area, leaving
  // enough space in the bottom to display the panel view nicely
  m_windowSplitterDisplay.SetRowInfo( 0, CTrackManager::round( ( y - 500 ) / 2 ), 0 ) ;
  m_windowSplitterDisplay.SetRowInfo( 1, CTrackManager::round( ( y - 500 ) / 2 ), 0 ) ;
  m_windowSplitterDisplay.SetRowInfo( 2, 500, 0 ) ;

  // Set the ideal and minimum widths for the splitter columns 
  // making sure there is enough to display the panel view nicely
  m_windowSplitterMain.SetColumnInfo( 0, CTrackManager::round( x * 0.75 ) > 250 ? x - 250 : CTrackManager::round( x * 0.75 ), 0 ) ;

  m_windowSplitterMain.RecalcLayout() ;
  m_windowSplitterDisplay.RecalcLayout() ;

  // Set the member variables for the views
  m_mainView  = (CC2PerformanceDemoView *)m_windowSplitterMain.GetPane( 0, 0 ) ;
  m_panelView = (CC2PerformanceDemoPanelView *)m_windowSplitterDisplay.GetPane( 2, 0 ) ;
  m_toteView  = (CC2PerformanceDemoToteView *)m_windowSplitterDisplay.GetPane( 1, 0 ) ;
  m_treeView  = (CC2PerformanceDemoTreeView *)m_windowSplitterDisplay.GetPane( 0, 0 ) ;

  return TRUE ;
}

/////////////////////////////////////////////////////////
// Return a view from one of the splitters

// Return a view to the main display area
CC2PerformanceDemoView * CMainFrame::getMainView()
{
  return m_mainView ;
}

// Return a pointer to the Track information panel
CC2PerformanceDemoPanelView * CMainFrame::getPanelView()
{
  return m_panelView ;
}

// Return a pointer to the Tote view
CC2PerformanceDemoToteView * CMainFrame::getToteView()
{
  return m_toteView ;
}

// Return a pointer to the Tree view
CC2PerformanceDemoTreeView * CMainFrame::getTreeView()
{
  return m_treeView ;
}

// Return a pointer to the Tree view
CStatusBar * CMainFrame::getStatusBar()
{
  return &m_wndStatusBar ;
}

PerformanceToolBar* CMainFrame::getPerformanceBar()
{
  return &m_wndPerformanceToolBar;
}

void CMainFrame::OnViewRangerings() 
{
  CTrackManager::displayRangeRings( !CTrackManager::displayRangeRings() ) ;
}

void CMainFrame::OnRange100km() 
{
  CTrackManager::ZoomTo( m_mainView->getDrawingSurface(), 100 ) ;
  GetActiveDocument()->UpdateAllViews( NULL, HINT_REDRAW_MAP_DISPLAY ) ;
}

void CMainFrame::OnRange10km() 
{
  CTrackManager::ZoomTo( m_mainView->getDrawingSurface(), 10 ) ;
  GetActiveDocument()->UpdateAllViews( NULL, HINT_REDRAW_MAP_DISPLAY ) ;
}

void CMainFrame::OnRange200km() 
{
  CTrackManager::ZoomTo( m_mainView->getDrawingSurface(), 200 ) ;
  GetActiveDocument()->UpdateAllViews( NULL, HINT_REDRAW_MAP_DISPLAY ) ;
}

void CMainFrame::OnRange500km() 
{
  CTrackManager::ZoomTo( m_mainView->getDrawingSurface(), 500 ) ;
  GetActiveDocument()->UpdateAllViews( NULL, HINT_REDRAW_MAP_DISPLAY ) ;
}

void CMainFrame::OnRange50km() 
{
  CTrackManager::ZoomTo( m_mainView->getDrawingSurface(), 50 ) ;
  GetActiveDocument()->UpdateAllViews( NULL, HINT_REDRAW_MAP_DISPLAY ) ;
}

void CMainFrame::OnToolsNorthup() 
{
  CTrackManager::northUp( !CTrackManager::northUp() ) ;
}

void CMainFrame::OnToolsTrackheadingup() 
{
  CTrackManager::trackHeadingUp( !CTrackManager::trackHeadingUp() ) ;
}

void CMainFrame::OnToolsFollowselected() 
{
  CTrackManager::followSelectedTrack( !CTrackManager::followSelectedTrack() ) ;
}

int CMainFrame::getGUIDisplacement()
{
  int depth = 0 ;
  CRect rc ;

  m_wndToolBar.IsWindowVisible() ? m_wndToolBar.GetClientRect( &rc ) : 0 ;
  depth = GetSystemMetrics( SM_CYMINIMIZED ) + GetSystemMetrics( SM_CYSIZEFRAME ) + GetSystemMetrics( SM_CYMENU ) + ( m_wndToolBar.IsWindowVisible() ? rc.Height() : 0 ) ;

  return depth ;
}

void CMainFrame::OnHelpContact() 
{
  ShellExecute( 
    NULL,
    _T("open"),
    _T("http://www.envitia.com/support"),
    NULL,
    NULL,
    0);	
}

void CMainFrame::OnUpdateRange100km(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( m_mainView->mapLoaded() && !CTrackManager::followSelectedTrack() ) ;	
}

void CMainFrame::OnUpdateRange10km(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( m_mainView->mapLoaded() && !CTrackManager::followSelectedTrack() ) ;	
}

void CMainFrame::OnUpdateRange200km(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( m_mainView->mapLoaded() && !CTrackManager::followSelectedTrack() ) ;	
}

void CMainFrame::OnUpdateRange500km(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( m_mainView->mapLoaded() && !CTrackManager::followSelectedTrack() ) ;	
}

void CMainFrame::OnUpdateRange50km(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( m_mainView->mapLoaded() && !CTrackManager::followSelectedTrack() ) ;	
}

void CMainFrame::OnSimulation10speed() 
{
  CAnimator * pAnimator = ((CC2PerformanceDemoDoc *)GetActiveDocument())->getAnimator() ;
  pAnimator->startAnimation( X10_SPEED ) ;	
}

void CMainFrame::OnUpdateSimulation10speed(CCmdUI* pCmdUI) 
{
  CAnimator * pAnimator = ((CC2PerformanceDemoDoc *)GetActiveDocument())->getAnimator() ;
  bool performanceTestRunning = m_mainView->isPerformanceTestRunning();
  pCmdUI->Enable( !performanceTestRunning && m_mainView->mapLoaded() && pAnimator ? true : false ) ;
  pCmdUI->SetCheck( pAnimator->playbackRate() == X10_SPEED ) ;
}

void CMainFrame::OnSimulation2speed() 
{
  CAnimator * pAnimator = ((CC2PerformanceDemoDoc *)GetActiveDocument())->getAnimator() ;
  pAnimator->startAnimation( X2_SPEED ) ;	
}

void CMainFrame::OnUpdateSimulation2speed(CCmdUI* pCmdUI) 
{
  CAnimator * pAnimator = ((CC2PerformanceDemoDoc *)GetActiveDocument())->getAnimator() ;
  bool performanceTestRunning = m_mainView->isPerformanceTestRunning();
  pCmdUI->Enable( !performanceTestRunning && m_mainView->mapLoaded() && pAnimator ? true : false ) ;
  pCmdUI->SetCheck( pAnimator->playbackRate() == X2_SPEED ) ;
}

void CMainFrame::OnSimulation50speed() 
{
  CAnimator * pAnimator = ((CC2PerformanceDemoDoc *)GetActiveDocument())->getAnimator() ;
  pAnimator->startAnimation( X50_SPEED ) ;	
}

void CMainFrame::OnUpdateSimulation50speed(CCmdUI* pCmdUI) 
{
  CAnimator * pAnimator = ((CC2PerformanceDemoDoc *)GetActiveDocument())->getAnimator() ;
  bool performanceTestRunning = m_mainView->isPerformanceTestRunning();
  pCmdUI->Enable( !performanceTestRunning && m_mainView->mapLoaded() && pAnimator ? true : false ) ;
  pCmdUI->SetCheck( pAnimator->playbackRate() == X50_SPEED ) ;
}

void CMainFrame::OnSimulation5speed() 
{
  CAnimator * pAnimator = ((CC2PerformanceDemoDoc *)GetActiveDocument())->getAnimator() ;
  pAnimator->startAnimation( X5_SPEED ) ;	
}

void CMainFrame::OnUpdateSimulation5speed(CCmdUI* pCmdUI) 
{
  CAnimator * pAnimator = ((CC2PerformanceDemoDoc *)GetActiveDocument())->getAnimator() ;
  bool performanceTestRunning = m_mainView->isPerformanceTestRunning();
  pCmdUI->Enable( !performanceTestRunning && m_mainView->mapLoaded() && pAnimator ? true : false ) ;
  pCmdUI->SetCheck( pAnimator->playbackRate() == X5_SPEED ) ;
}

void CMainFrame::OnSimulationRealtime() 
{
  CAnimator * pAnimator = ((CC2PerformanceDemoDoc *)GetActiveDocument())->getAnimator() ;
  pAnimator->startAnimation( REAL_TIME ) ;	
}

void CMainFrame::OnUpdateSimulationRealtime(CCmdUI* pCmdUI) 
{
  CAnimator * pAnimator = ((CC2PerformanceDemoDoc *)GetActiveDocument())->getAnimator() ;
  bool performanceTestRunning = m_mainView->isPerformanceTestRunning();
  pCmdUI->Enable( !performanceTestRunning && m_mainView->mapLoaded() && pAnimator ? true : false ) ;
  pCmdUI->SetCheck( pAnimator->playbackRate() == REAL_TIME ) ;
}

void CMainFrame::OnUpdateToolsFollowselected(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( m_mainView->mapLoaded() && CTrackManager::selectedTrack() ) ;
  pCmdUI->SetCheck( CTrackManager::followSelectedTrack() ) ;
}

void CMainFrame::OnUpdateToolsNorthup(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( m_mainView->mapLoaded() && CTrackManager::selectedTrack() ) ;
  pCmdUI->SetCheck( CTrackManager::northUp() ) ;
}

void CMainFrame::OnToolsPan() 
{
  m_mainView->setViewMode( ID_TOOLS_PAN ) ;
}

void CMainFrame::OnUpdateToolsPan(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( m_mainView->mapLoaded() && !CTrackManager::followSelectedTrack() ) ;
  pCmdUI->SetCheck( m_mainView->getViewMode() == ID_TOOLS_PAN ) ;
}

void CMainFrame::OnToolsReset() 
{
  if ( m_mainView->getDrawingSurface() )
  {
    // Reset the window to display the entire map, and request a window redraw.
    m_mainView->getDrawingSurface()->reset( false ) ;
    m_mainView->InvalidateRect( 0, false ) ;
  }
}

void CMainFrame::OnUpdateToolsReset(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( m_mainView->mapLoaded() && !CTrackManager::followSelectedTrack() ) ;
}

void CMainFrame::OnUpdateToolsTrackheadingup(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( m_mainView->mapLoaded() && CTrackManager::selectedTrack() ) ;
  pCmdUI->SetCheck( CTrackManager::trackHeadingUp() ) ;
}

void CMainFrame::OnToolsTrackselect() 
{
  m_mainView->setViewMode( ID_TOOLS_TRACKSELECT ) ;
}

void CMainFrame::OnUpdateToolsTrackselect(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( m_mainView->mapLoaded() ) ;
  pCmdUI->SetCheck( ( m_mainView->getViewMode() == ID_TOOLS_TRACKSELECT 
    || m_mainView->getViewMode() == ID_TOOLS_TRACKSELECT_DRAG ) ) ;
}

void CMainFrame::OnToolsZoom() 
{
  m_mainView->setViewMode( ID_TOOLS_ZOOM ) ;
}

void CMainFrame::OnUpdateToolsZoom(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( m_mainView->mapLoaded() && !CTrackManager::followSelectedTrack() ) ;
  pCmdUI->SetCheck( m_mainView->getViewMode() == ID_TOOLS_ZOOM ) ;
}

void CMainFrame::OnToolsZoomin() 
{
  if ( m_mainView->getDrawingSurface() )
  {
    // Zoom in by 30%, and request a window redraw
    m_mainView->getDrawingSurface()->zoom( 30, true, false ) ;
    m_mainView->InvalidateRect( 0, false ) ;
  }
}

void CMainFrame::OnUpdateToolsZoomin(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( m_mainView->mapLoaded() ) ;
}

void CMainFrame::OnToolsZoomout() 
{
  if ( m_mainView->getDrawingSurface() )
  {
    // Zoom in by 30%, and request a window redraw
    m_mainView->getDrawingSurface()->zoom( 30, false, false ) ;
    m_mainView->InvalidateRect( 0, false ) ;
  }
}

void CMainFrame::OnUpdateToolsZoomout(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( m_mainView->mapLoaded() ) ;
}

void CMainFrame::OnUpdateViewRangerings(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( m_mainView->mapLoaded() ) ;
  pCmdUI->SetCheck( CTrackManager::displayRangeRings() ) ;
}

void CMainFrame::OnViewStatusBar() 
{
  m_wndStatusBar.ShowWindow( m_wndStatusBar.IsWindowVisible() ? SW_HIDE : SW_SHOW ) ;
}

void CMainFrame::OnUpdateViewStatusBar(CCmdUI* pCmdUI) 
{
  pCmdUI->SetCheck( m_wndStatusBar.IsWindowEnabled() ) ;
}

void CMainFrame::OnViewToolbar() 
{
  m_wndToolBar.ShowWindow( m_wndToolBar.IsWindowVisible() ? SW_HIDE : SW_SHOW ) ;
}

void CMainFrame::OnUpdateViewToolbar(CCmdUI* pCmdUI) 
{
  pCmdUI->SetCheck( m_wndToolBar.IsWindowVisible() ) ;
}

void CMainFrame::OnAppHelp() 
{
  string maplHome = TSLUtilityFunctions::getMapLinkHome();
  if( !maplHome.empty() )
  {
    maplHome += "\\samples\\NT\\C2PerformanceDemo\\Help\\C2Help.chm";
    ::HtmlHelp( 0, (LPCTSTR)TSLUTF8Decoder(maplHome), HH_DISPLAY_TOC, 0 );		
  }
  else
  {
    AfxMessageBox( _T("MapLink home cannot be found to display help") ) ;
  }
}

void CMainFrame::OnUpdateAppHelp(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( TRUE ) ;	
}

void CMainFrame::OnToolsGrab() 
{
  m_mainView->setViewMode( ID_TOOLS_GRAB ) ;
}

void CMainFrame::OnUpdateToolsGrab(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( m_mainView->mapLoaded() && !CTrackManager::followSelectedTrack() ) ;
  pCmdUI->SetCheck( m_mainView->getViewMode() == ID_TOOLS_GRAB ) ;
}
void CMainFrame::OnUpdateSymbolType(CCmdUI *pCmdUI)
{
  bool isapp6a = ((CC2PerformanceDemoDoc *)GetActiveDocument())->isDisplayingAPP6A() ;
  bool is2525b = ((CC2PerformanceDemoDoc *)GetActiveDocument())->isDisplaying2525B() ;
  switch ( pCmdUI->m_nID )
  {
  case ID_SYMBOLS_STANDARD:
    pCmdUI->SetCheck( !isapp6a && !is2525b );
    break;
  case ID_SYMBOLS_APP6A:
    pCmdUI->SetCheck( isapp6a );
    break;
  case ID_SYMBOLS_2525B:
    pCmdUI->SetCheck( is2525b );
    break;
  }
  pCmdUI->Enable();
}

void CMainFrame::OnSymbolsApp6a()
{
  ((CC2PerformanceDemoDoc *)GetActiveDocument())->changeToAPP6aSymbols();
  m_treeView->setSymbolType(true);
  m_panelView->setSymbolType(true);
  m_toteView->clearAllTracks();
  m_mainView->Invalidate();
}

void CMainFrame::OnSymbols2525B()
{
  ((CC2PerformanceDemoDoc *)GetActiveDocument())->changeTo2525BSymbols();
  m_treeView->setSymbolType(true);
  m_panelView->setSymbolType(true);
  m_toteView->clearAllTracks();
  m_mainView->Invalidate();
}

void CMainFrame::OnSymbolsStandard()
{
  ((CC2PerformanceDemoDoc *)GetActiveDocument())->changeToStandardSymbols();
  m_treeView->setSymbolType(false);
  m_panelView->setSymbolType(false);
  m_toteView->clearAllTracks();
  m_mainView->Invalidate();
}

void CMainFrame::OnPerformanceStart()
{
  m_mainView->startPerformanceTest();
}

void CMainFrame::OnPerformanceStop()
{
  m_mainView->stopPerformanceTest();
  m_wndPerformanceToolBar.DrawFrameRate(0.0, 0.0, true);
}

void CMainFrame::OnUpdatePerformance(CCmdUI *pCmdUI)
{
  bool performanceTestRunning = m_mainView->isPerformanceTestRunning();
  if ( pCmdUI->m_nID == ID_PERFORMANCE_STOP )
  {
    pCmdUI->Enable(performanceTestRunning);
  }
  else
  {
    pCmdUI->Enable(!performanceTestRunning);
  }
}


void CMainFrame::OnSymbolsChangethenumberoftracks()
{
  CC2PerformanceDemoDoc * doc = (CC2PerformanceDemoDoc *)GetActiveDocument();

  int currentNoOfTracks = doc->getCurrentNoOfSymbolTracks();
  CNumberOfTracksDlg dlg( (CWnd*)m_mainView, currentNoOfTracks);
  if ( dlg.DoModal() != IDOK )
    return;

  int newNoOfTracks = dlg.getNoOfTracks();
  doc->setCurrentNoOfSymbolTracks(newNoOfTracks);
}

void CMainFrame::OnUpdateApp6aAnnotationlevel(CCmdUI *pCmdUI)
{
  bool isApp6a = ((CC2PerformanceDemoDoc *)GetActiveDocument())->isDisplayingAPP6A() ;
  bool is2525B = ((CC2PerformanceDemoDoc *)GetActiveDocument())->isDisplaying2525B() ;
  pCmdUI->Enable(isApp6a || is2525B);
  AnnotationLevelEnum level = CTrackManager::getAnnotationLevel();
  switch ( pCmdUI->m_nID )
  {
  case ID_APP6AANNOTATIONLEVEL_NONE:
    pCmdUI->SetCheck(level == AnnotationLevelNone);
    break;
  case ID_APP6AANNOTATIONLEVEL_LOW:
    pCmdUI->SetCheck(level == AnnotationLevelLow);
    break;
  case ID_APP6AANNOTATIONLEVEL_MEDIUM:
    pCmdUI->SetCheck(level == AnnotationLevelMedium);
    break;
  case ID_APP6AANNOTATIONLEVEL_HIGH:
    pCmdUI->SetCheck(level == AnnotationLevelHigh);
    break;
  }
}

void CMainFrame::OnApp6aAnnotationlevelNone()
{
  AnnotationLevelEnum currentLevel = CTrackManager::getAnnotationLevel();
  if ( currentLevel != AnnotationLevelNone )
  {
    CTrackManager::setAnnotationLevel(AnnotationLevelNone);
    ((CC2PerformanceDemoDoc *)GetActiveDocument())->refreshAPP6AAnnotationLevel();
  }
}

void CMainFrame::OnApp6aAnnotationlevelLow()
{
  AnnotationLevelEnum currentLevel = CTrackManager::getAnnotationLevel();
  if ( currentLevel != AnnotationLevelLow )
  {
    CTrackManager::setAnnotationLevel(AnnotationLevelLow);
    ((CC2PerformanceDemoDoc *)GetActiveDocument())->refreshAPP6AAnnotationLevel();
  }
}


void CMainFrame::OnApp6aAnnotationlevelMedium()
{
  AnnotationLevelEnum currentLevel = CTrackManager::getAnnotationLevel();
  if ( currentLevel != AnnotationLevelMedium )
  {
    CTrackManager::setAnnotationLevel(AnnotationLevelMedium);
    ((CC2PerformanceDemoDoc *)GetActiveDocument())->refreshAPP6AAnnotationLevel();
  }
}


void CMainFrame::OnApp6aAnnotationlevelHigh()
{
  AnnotationLevelEnum currentLevel = CTrackManager::getAnnotationLevel();
  if ( currentLevel != AnnotationLevelHigh )
  {
    CTrackManager::setAnnotationLevel(AnnotationLevelHigh);
    ((CC2PerformanceDemoDoc *)GetActiveDocument())->refreshAPP6AAnnotationLevel();
  }
}

void CMainFrame::OnUpdateApp6aRendering(CCmdUI *pCmdUI)
{
  bool isApp6a = ((CC2PerformanceDemoDoc *)GetActiveDocument())->isDisplayingAPP6A() ;
  bool is2525B = ((CC2PerformanceDemoDoc *)GetActiveDocument())->isDisplaying2525B() ;
  pCmdUI->Enable(isApp6a || is2525B);

  bool useBitmaps = CTrackManager::bitmapRendering();
  switch ( pCmdUI->m_nID )
  {
  case ID_APP6ARENDERING_BITMAP:
    pCmdUI->SetCheck( useBitmaps );
    break;

  case ID_APP6ARENDERING_ENTITY:
    pCmdUI->SetCheck( !useBitmaps );
    break;
  }
}

void CMainFrame::OnApp6aRenderingBitmap()
{
  bool useBitmaps = CTrackManager::bitmapRendering();
  if ( !useBitmaps )
  {
    CTrackManager::bitmapRendering( true );
  }
}

void CMainFrame::OnApp6aRenderingEntity()
{
  bool useBitmaps = CTrackManager::bitmapRendering();
  if ( useBitmaps )
  {
    CTrackManager::bitmapRendering( false );
  }
}
void CMainFrame::OnDoNothing(){}
