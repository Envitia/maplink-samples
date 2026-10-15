// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "resource.h"
#include "LayerTreeDlg.h"
#include "LayerProperties.h"
#include "MainFrm.h"
#include "OverlayOpacity.h"

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

  ON_MESSAGE(UPDATE_LOADED_LAYERS, UpdateLoadedLayers)
  ON_MESSAGE(UPDATE_SELECTED_LAYER, UpdateSelectedLayer)

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

        if (!m_layerBar.Create(this, IDD_LAYER_LIST, WS_CHILD |CBRS_LEFT, IDD_LAYER_LIST))
        {
          TRACE0("Failed to create layers window");
        }
        m_layerBar.SetBarStyle(m_wndToolBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
        m_layerBar.EnableDocking(CBRS_ALIGN_LEFT);

        RECT rect;
        rect.top = 0;
        rect.bottom = 231;
        rect.left = 0;
        rect.right = 211;
        DockControlBar(&m_layerBar, AFX_IDW_DOCKBAR_LEFT, &rect);

        if (!m_layerProps.Create(this, IDD_LAYER_PROPERTIES, WS_CHILD |CBRS_LEFT, IDD_LAYER_PROPERTIES))
        {
          TRACE0("Failed to create properties window");
        }
        m_layerProps.SetBarStyle(m_wndToolBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
        m_layerProps.EnableDocking(CBRS_ALIGN_LEFT);
        rect.top = 231;
        rect.bottom = 411;
        rect.left = 0;
        rect.right = 211;
        DockControlBar(&m_layerProps, AFX_IDW_DOCKBAR_LEFT, &rect);

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


LRESULT CMainFrame::UpdateLoadedLayers(WPARAM wparam, LPARAM lparam)
{
  m_layerBar.UpdateTree((COverlayOpacityDoc*)wparam, (TSLDrawingSurface*)lparam);
  return 0;
}

LRESULT CMainFrame::UpdateSelectedLayer(WPARAM wparam, LPARAM lparam)
{
  m_layerProps.Update((string *) wparam, (TSLDrawingSurface*)lparam);
  return 0;
}
/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
