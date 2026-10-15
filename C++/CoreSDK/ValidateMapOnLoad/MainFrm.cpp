// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx_tlt.h"
#include "RuntimeProjTest.h"

#include "MainFrm.h"
#include "RuntimeProjTestDoc.h"
#include "RuntimeProjTestView.h"

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
  ON_WM_CREATE()
  ON_MESSAGE(PWM_CALLBACK, OnPWMCallback)
  ON_MESSAGE(PWM_ALLLOADED, OnAllLoadedCallback)
  ON_MESSAGE(PWM_PROJCHANGED, OnProjectionChangedCallback)
  ON_MESSAGE(PWM_PROJORIGINCHANGED, OnProjectionOriginChangedCallback)
  ON_MESSAGE(PWM_DRAWING_SURFACE_NO_CRC, OnDrawingSurfaceNoCRCCallback)
  ON_MESSAGE(PWM_DRAWING_SURFACE_CRC_FAILURE, OnDrawingSurfaceCRCFailureCallback)
  ON_MESSAGE(PWM_MAP_NO_CRC, OnMapNoCRCCallback)
  ON_MESSAGE(PWM_MAP_CRC_FAILURE, OnMapCRCFailureCallback)
  ON_MESSAGE(PWM_MAP_CANN0T_FIND_FILE, OnMapCanNotFindFileCallback)
  
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


CRuntimeProjTestDoc* CMainFrame::GetMyActiveDocument()
{
  CDocument* pDoc = CFrameWnd::GetActiveDocument();
  ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CRuntimeProjTestDoc)));
  return (CRuntimeProjTestDoc*)pDoc;
}


CRuntimeProjTestView* CMainFrame::GetMyActiveView()
{
  CView* pVw = CFrameWnd::GetActiveView();
  ASSERT(pVw->IsKindOf(RUNTIME_CLASS(CRuntimeProjTestView)));
  return (CRuntimeProjTestView*)pVw;
}


afx_msg LRESULT CMainFrame::OnPWMCallback( WPARAM wparam, LPARAM lparam)
{
  // route main tile loaded callback to document
  GetMyActiveDocument()->SuccessfulCallbackAction( );

  return 0L;
}

afx_msg LRESULT CMainFrame::OnAllLoadedCallback( WPARAM wparam, LPARAM lparam)
{
  // route all tiles loaded callback to document
  GetMyActiveDocument()->AllLoadedCallbackAction( );

  return 0L;
}

afx_msg LRESULT CMainFrame::OnProjectionChangedCallback( WPARAM wparam, LPARAM lparam)
{
  // route projection changed callback to document
  GetMyActiveDocument()->projectionChangedCallbackAction();

  return 0L;
}

afx_msg LRESULT CMainFrame::OnProjectionOriginChangedCallback( WPARAM wparam, LPARAM lparam)
{
  // route projection origin changed callback to view
  GetMyActiveView()->projectionOriginChangedCallbackAction( (OriginChangeInfo*)wparam );

  return 0L;
}

afx_msg LRESULT CMainFrame::OnDrawingSurfaceNoCRCCallback( WPARAM wparam, LPARAM lparam)
{
  // We only report the first one we load that has no CRC data.
  AfxMessageBox(_T("The Drawing Surface configuration files do not appear to have CRC values"));
  return 0L;
}

afx_msg LRESULT CMainFrame::OnDrawingSurfaceCRCFailureCallback( WPARAM wparam, LPARAM lparam)
{
  TSLSimpleString *filename = (TSLSimpleString*)lparam;
  // We report all the CRC failures (we found a CRC marker but the CRC calculated did not match the one stored).
  string message("The Drawing Surface configuration files CRC check failed for file: ");
  message += filename->c_str();
  AfxMessageBox((LPCTSTR)TSLUTF8Decoder(message));
  delete filename;
  return 0L;
}

afx_msg LRESULT CMainFrame::OnMapNoCRCCallback( WPARAM wparam, LPARAM lparam)
{
  // We only report the first one we load that has no CRC data.
  AfxMessageBox(_T("The Map does not appear to have CRC markers."));
  return 0L;
}

afx_msg LRESULT CMainFrame::OnMapCRCFailureCallback( WPARAM wparam, LPARAM lparam)
{
  // We report all the CRC failures (we found a CRC marker but the CRC calculated did not match the one stored).
  TSLSimpleString *filename = (TSLSimpleString*)lparam;
  string message("The Map CRC check failed for file: ");
  message += filename->c_str();
  AfxMessageBox((LPCTSTR)TSLUTF8Decoder(message));
  delete filename;
  return 0L;
}

afx_msg LRESULT CMainFrame::OnMapCanNotFindFileCallback( WPARAM wparam, LPARAM lparam)
{
  // We are attempting to check a complex file and are unable to find the file.
  TSLSimpleString *filename = (TSLSimpleString*)lparam;
  string message("Could not find the file for checking : ");
  message += filename->c_str();
  AfxMessageBox((LPCTSTR)TSLUTF8Decoder(message));
  delete filename;
  return 0L;
}


void CMainFrame::SetStatus( const CString& sInfo )
{
  m_wndStatusBar.SetWindowText( sInfo );
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

