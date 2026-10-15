// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "TerrainViewer.h"

#include "MainFrm.h"
#include "TerrainView.h"
#include "LOSView.h"
#include "TerrainViewerDoc.h"
#include "TerrainViewerView.h"
#include "OptionsDialog.h"

#include "areadialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CTerrainViewerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
//{{AFX_MSG_MAP(CMainFrame)
ON_WM_CREATE()
	ON_COMMAND(ID_TOOLS_OPTIONS, OnToolsOptions)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
  ID_SEPARATOR,           // status line indicator
    ID_SEPARATOR,           // status line indicator
    ID_SEPARATOR,
    ID_INDICATOR_CAPS,
    ID_INDICATOR_NUM,
    ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
  : m_currentMapView(NULL)
  , m_currentTerrainView(NULL)
{
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
  
  UINT id, style;
  int width ;
  
  m_wndStatusBar.GetPaneInfo (0, id, style, width);
  m_wndStatusBar.SetPaneInfo (0, id, 0, 100);

  m_wndStatusBar.GetPaneInfo (1, id, style, width);
  m_wndStatusBar.SetPaneInfo (1, id, 0, 350);
  
  m_wndStatusBar.GetPaneInfo (2, id, style, width);
  m_wndStatusBar.SetPaneInfo (2, id, SBPS_STRETCH, 540);
  m_wndStatusBar.SetPaneText(2, _T("Lat : 0.0, Lon : 0.0"));
  
  // TODO: Remove this if you don't want tool tips or a resizeable toolbar
  m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
    CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
  
  // TODO: Delete these three lines if you don't want the toolbar to
  //  be dockable
  m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
  EnableDocking(CBRS_ALIGN_ANY);
  DockControlBar(&m_wndToolBar);

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

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
  CRect rc;
  GetClientRect( rc );

  if ( !m_windowSplitter.CreateStatic(this, 2, 1 ) )
  {
    TRACE0("Failed to create top splitter\n");
    return FALSE;
  }
  
  // Create the map viewer in the second section of the split
  m_windowSplitter.CreateView( 0, 0, RUNTIME_CLASS( CTerrainViewerView ),
    CSize( 0, rc.Height() ), pContext ) ;
  
  // Create the tree in the first split
  m_windowSplitter.CreateView( 1, 0, RUNTIME_CLASS( CTerrainView ),
    CSize( 0, 0 ), pContext ) ;

  m_options.load();

  return CFrameWnd::OnCreateClient(lpcs, pContext);
}

void CMainFrame::ActivateFrame(int nCmdShow) 
{
  m_currentMapView = ( CTerrainViewerView * ) m_windowSplitter.GetPane ( 0, 0 ) ;
  m_currentTerrainView = ( CTerrainView * ) m_windowSplitter.GetPane ( 1, 0 ) ;

  CFrameWnd::ActivateFrame(nCmdShow);
}

void CMainFrame::OnToolsOptions() 
{
  COptionsDialog options(m_options, 0) ;
  options.DoModal() ;	
}

BOOL CMainFrame::DestroyWindow()
{
  m_wndToolBar.DestroyWindow();
  m_wndStatusBar.DestroyWindow();
  m_windowSplitter.DestroyWindow();
  return CFrameWnd::DestroyWindow();
}
