
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "EarthSample.h"

#include "MainFrm.h"

#include "EarthSampleView.h"
#include "EarthSampleDoc.h"

#include "CameraControlFormView.h"
#include "ScreenshotDialog.h"

#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
  ON_WM_CREATE()
  ON_COMMAND(ID_FILE_CLOSE, &CMainFrame::OnFileClose)
  ON_MESSAGE(WM_SIZING, &CMainFrame::OnSizing)
  ON_COMMAND(ID_SCREENSHOT, &CMainFrame::OnScreenshot)
  ON_COMMAND(ID_GEOMETRY_POLYGON, &CMainFrame::OnGeometryPolygon)
  ON_COMMAND(ID_GEOMETRY_POLYLINE, &CMainFrame::OnGeometryPolyline)
  ON_COMMAND(ID_GEOMETRY_TEXT, &CMainFrame::OnGeometryText)
  ON_COMMAND(ID_GEOMETRY_SYMBOL, &CMainFrame::OnGeometrySymbol)
  ON_COMMAND(ID_INTERACTIONMODE_EXTRUDEDPOLYGON, &CMainFrame::OnInteractionmodeExtrudedpolygon)
  ON_COMMAND(ID_INTERACTIONMODE_EXTRUDEDPOLYLINE, &CMainFrame::OnInteractionmodeExtrudedpolyline)
  ON_COMMAND(ID_INTERACTIONMODE_DELETEGEOMETRY, &CMainFrame::OnInteractionmodeDeletegeometry)
  ON_COMMAND(ID_INTERACTIONMODE_SELECTGEOMETRY, &CMainFrame::OnInteractionmodeSelectgeometry)
  ON_COMMAND(ID_INTERACTIONMODE_TRACKBALLVIEWINTERACTION, &CMainFrame::OnInteractionmodeTrackball)
  ON_COMMAND(ID_RESETVIEW, &CMainFrame::OnResetView)
END_MESSAGE_MAP()

static UINT indicators[] =
{
  ID_SEPARATOR,           // status line indicator
  ID_INDICATOR_CAPS,
  ID_INDICATOR_NUM,
  ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{

}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
    return -1;

  if (!m_wndStatusBar.Create(this))
  {
    TRACE0("Failed to create status bar\n");
    return -1;      // fail to create
  }
  m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));

  return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
  if (!CFrameWnd::PreCreateWindow(cs))
    return FALSE;

  return TRUE;
}

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


// CMainFrame message handlers


BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext)
{
  // base class does the real work

  if (!CFrameWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
  {
    return FALSE;
  }

  CWinApp* pApp = AfxGetApp();
  if (pApp->m_pMainWnd == NULL)
    pApp->m_pMainWnd = this;

  return TRUE;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) {
  CRect cr;
  GetClientRect(&cr);

  // Setup the window split
  // Parent window, rows, columns
  if (!m_splitterWnd.CreateStatic(this, 1, 2)) {
    MessageBox(_T("Error creating splitter"),
      _T("Initialisation Error"), MB_OK | MB_ICONERROR);
    return FALSE;
  }

  // Create views within the splitter
  if (!m_splitterWnd.CreateView(0, 0,
    RUNTIME_CLASS(CEarthSampleView),
    CSize(cr.Width() - 300, cr.Height()), pContext)) {
    MessageBox(_T("Error creating view in splitter"),
      _T("Initialisation Error"), MB_OK | MB_ICONERROR);
    return FALSE;
  }

  if (!m_splitterWnd.CreateView(0, 1,
    RUNTIME_CLASS(CCameraControlFormView),
    CSize(300, cr.Height()), pContext)) {
    MessageBox(_T("Error creating view in splitter"),
      _T("Initialisation Error"), MB_OK | MB_ICONERROR);
    return FALSE;
  }
  m_splitterWndInitialised = true;

  m_screenshotDialog = std::unique_ptr<ScreenshotDialog>(new ScreenshotDialog());
  m_screenshotDialog->Create(IDD_SCREENSHOT, this);

  return TRUE;
}

LRESULT CMainFrame::OnSizing(/*UINT*/ WPARAM fwSide, /*LPRECT*/ LPARAM pRect)
{
  auto lpRect = (LPRECT)pRect;
  CFrameWnd::OnSizing(fwSide, lpRect);
  auto width = lpRect->right - lpRect->left;
  auto height = lpRect->bottom - lpRect->top;

  auto earthWidth = (std::max)(width - 322, 0l);

  if (m_splitterWndInitialised) {
    m_splitterWnd.SetRowInfo(0, height, 0);
    m_splitterWnd.SetColumnInfo(0, earthWidth, 50);
    m_splitterWnd.SetColumnInfo(1, 322, 50);
    m_splitterWnd.RecalcLayout();
  }
  return TRUE;
}

void CMainFrame::OnFileClose()
{
  DestroyWindow();
}

// CEarthSampleApp message handlers
void CMainFrame::OnTrackballMove()
{
  // Update the camera control form with any moved values
  ((CCameraControlFormView*)getCameraControlFormView())->handleViewChange();
}


void CMainFrame::setStatusText(const std::string& sInfo)
{
  m_wndStatusBar.SetWindowText(TSLUTF8Decoder(sInfo));
}

void CMainFrame::attachCameraUIToSurface(earth::Surface3D * surface)
{
  auto camFormView = (CCameraControlFormView *)m_splitterWnd.GetPane(0, 1);
  camFormView->initFromSurface(surface);
}

CView * CMainFrame::getEarthView()
{
  auto earthView = (CEarthSampleView *)m_splitterWnd.GetPane(0, 0);
  return earthView;
}

CView * CMainFrame::getCameraControlFormView()
{
  auto camControlsView = (CCameraControlFormView *)m_splitterWnd.GetPane(0, 1);
  return camControlsView;
}


void CMainFrame::OnScreenshot()
{
  // Alter the screenshot window to half the size of the main view
  auto earthView = ((CEarthSampleView *)getEarthView());
  RECT viewBounds;

  earthView->GetWindowRect(&viewBounds);
  auto viewWidth = viewBounds.right - viewBounds.left;
  auto viewHeight = viewBounds.bottom - viewBounds.top;

  auto width = viewWidth / 2;
  auto height = viewHeight / 2;

  // Setup the bounds we want for the client rect on the screenshot dialog
  RECT dialogBounds = viewBounds;
  dialogBounds.right = viewBounds.left + width;
  dialogBounds.bottom = viewBounds.top + height;

  // Adjust the rect to account for the window header bar
  AdjustWindowRect(&dialogBounds, m_screenshotDialog->GetStyle(), false);

  //Set the dialog size
  m_screenshotDialog->MoveWindow(&dialogBounds);

  // Show the dialog
  m_screenshotDialog->ShowWindow(SW_SHOWNORMAL);

  // Grab the dialog context
  auto cdc = m_screenshotDialog->GetDC();
  HDC hdc = cdc->GetSafeHdc();

  if (hdc)
  {
    // Draw to the HDC
    TSLDeviceContext tslDC = (TSLDeviceContext)hdc;
    earthView->drawToHDC(tslDC, width, height);
  }

  // We have to release the DC
  ReleaseDC(cdc);
}

void CMainFrame::OnGeometryPolygon()
{
  auto earthView = ((CEarthSampleView *)getEarthView());
  if (!earthView) return;
  earthView->interaction(ID_TOOLS_CreatePolygon);
}

void CMainFrame::OnGeometryPolyline()
{
  auto earthView = ((CEarthSampleView *)getEarthView());
  if (!earthView) return;
  earthView->interaction(ID_TOOLS_CreatePolyline);
}

void CMainFrame::OnGeometryText()
{
  auto earthView = ((CEarthSampleView *)getEarthView());
  if (!earthView) return;
  earthView->interaction(ID_TOOLS_CreateText);
}

void CMainFrame::OnGeometrySymbol()
{
  auto earthView = ((CEarthSampleView *)getEarthView());
  if (!earthView) return;
  earthView->interaction(ID_TOOLS_CreateSymbol);
}

void CMainFrame::OnInteractionmodeExtrudedpolygon()
{
  auto earthView = ((CEarthSampleView *)getEarthView());
  if (!earthView) return;
  earthView->interaction(ID_TOOLS_CreateExtrudedPolygon);
}

void CMainFrame::OnInteractionmodeExtrudedpolyline()
{
  auto earthView = ((CEarthSampleView *)getEarthView());
  if (!earthView) return;
  earthView->interaction(ID_TOOLS_CreateExtrudedPolyline);
}

void CMainFrame::OnInteractionmodeDeletegeometry()
{
  auto earthView = ((CEarthSampleView *)getEarthView());
  if (!earthView) return;
  earthView->interaction(ID_TOOLS_DeleteGeometry);
}

void CMainFrame::OnInteractionmodeSelectgeometry()
{
  auto earthView = ((CEarthSampleView *)getEarthView());
  if (!earthView) return;
  earthView->interaction(ID_TOOLS_Select);
}


void CMainFrame::OnInteractionmodeTrackball()
{
  auto earthView = ((CEarthSampleView *)getEarthView());
  if (!earthView) return;
  earthView->interaction(ID_TOOLS_Trackball);
}

void CMainFrame::OnResetView()
{
	auto earthView = ((CEarthSampleView *)getEarthView());
	if (!earthView) return;
	earthView->resetView();
}