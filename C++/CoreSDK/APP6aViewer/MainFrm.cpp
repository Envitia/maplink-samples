// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "App6aViewer.h"
#include "SymbolTreeView.h"

#include "MainFrm.h"
#include "App6aViewerDoc.h"
#include "App6aViewerView.h"

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
       // m_viewBar.SetBarStyle(m_viewBar.GetBarStyle() |
//		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
       
        

        CRect rect;

        rect.top = 0;
        rect.left = 600;
        rect.bottom = 200;
        rect.right = 0;

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
        //m_viewBar.EnableDocking(CBRS_ALIGN_TOP);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar, AFX_IDW_DOCKBAR_TOP);
        //DockControlBar(&m_viewBar,  AFX_IDW_DOCKBAR_TOP,rect);

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
    if ( !m_windowSplitter.CreateStatic(this, 1, 2 ) )
    {
      TRACE0("Failed to create top splitter\n");
      return FALSE;
    }

    m_windowSplitter.CreateView( 0, 0, RUNTIME_CLASS( CSymbolTreeView ),
    CSize( 100, 100 ), pContext ) ;

    m_windowSplitter.CreateView( 0, 1, RUNTIME_CLASS( CApp6aViewerView ),
    CSize( 100, 100 ), pContext ) ;

    m_windowSplitter.SetColumnInfo( 0, 200 , 100 ) ;
    m_windowSplitter.SetColumnInfo( 1, 100, 100 ) ;
    m_windowSplitter.SetRowInfo( 0, 100, 10 ) ;
    m_windowSplitter.RecalcLayout();

    return CFrameWnd::OnCreateClient(lpcs, pContext);
}


void CMainFrame::ActivateFrame(int nCmdShow) 
{

    //m_treeView     = ( CSymbolTreeView* )m_windowSplitter.GetPane( 0, 0 );
    
    m_graphicsView = ( CApp6aViewerView* )m_windowSplitter.GetPane( 0, 1 );
    m_graphicsView->GetDocument()->m_treeView = ( CSymbolTreeView* )m_windowSplitter.GetPane( 0, 0 );
    // Now the tree view is initialised, can draw the symbols
    m_graphicsView->drawSymbols();
    CFrameWnd::ActivateFrame(nCmdShow);

}
