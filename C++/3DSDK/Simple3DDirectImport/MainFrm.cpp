////////////////////////////////////////////////////////
// Copyright (c) 1998 to 2017 by Envitia Group PLC
////////////////////////////////////////////////////////
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Simple3DDirectImport.h"
#include "Simple3DDirectImportView.h"
#include "Simple3DDirectImportDoc.h"

#include "MainFrm.h"

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
    ON_MESSAGE(WM_NEW_TILES_AVAILABLE, OnNewTileAvailableCallback)
	//}}AFX_MSG_MAP
        ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, OnUpdateFileOpen)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
        ID_INDICATOR_LAT,       // camera lat
        ID_INDICATOR_LON,       // camera lon
        ID_INDICATOR_ALT,       // camera alt
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};
/////////////////////////////////////////////////////////////////////////////
// True colour toolbar defintions

// these constants represent the dimensions and number of buttons in
// the default MFC-generated toolbar. If you need something different,
// feel free to change them. 

#define TB_IMAGE_WIDTH 24
#define TB_IMAGE_HEIGHT 24
#define TB_BIT_DEPTH ILC_COLOR24

#define TB_MAIN_NUM_IMAGES 15

// create an image list for the specified BMP resource
static void MakeToolbarImageList ( UINT inBitmapID, CImageList * outImageList, int nButtonCount )
{
  CBitmap bm;

  // if we use CBitmap::LoadBitmap() to load the bitmap, the colors
  // will be reduced to the bit depth of the main screen and we won't
  // be able to access the pixels directly. To avoid those problems,
  // we'll load the bitmap as a DIBSection instead and attach the
  // DIBSection to the CBitmap.
  VERIFY (bm.Attach (::LoadImage (::AfxFindResourceHandle(
    MAKEINTRESOURCE (inBitmapID), RT_BITMAP),
    MAKEINTRESOURCE (inBitmapID), IMAGE_BITMAP, 0, 0,
    (LR_DEFAULTSIZE | LR_CREATEDIBSECTION))));

  // create a 24 bit image list with the same dimensions and number
  // of buttons as the toolbar
  VERIFY( outImageList->Create( TB_IMAGE_WIDTH, TB_IMAGE_HEIGHT, TB_BIT_DEPTH | ILC_MASK, nButtonCount, 0 ) ) ;

  // attach the bitmap to the image list
  VERIFY( outImageList->Add( &bm, RGB ( 192, 192, 192 ) ) != -1) ;
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
  // TODO: add member initialization code here
  normalImages = 0;
  disabledImages = 0;
  hotImages = 0;
	
}

CMainFrame::~CMainFrame()
{
  delete normalImages ;
  delete disabledImages;
  delete hotImages;
}

void CMainFrame::attachToolbarImages( CToolBarCtrl& toolbar, int numImages, UINT inNormalImageID, UINT inDisabledImageID, UINT inHotImageID )
{
  // make high-color image lists for each of the bitmaps and attach to the toolbar

  if ( inNormalImageID != 0 )
  {
    normalImages = new CImageList ;
    MakeToolbarImageList( inNormalImageID, normalImages, numImages ) ;
    toolbar.SetImageList( normalImages ) ;
  }

  if ( inDisabledImageID != 0 )
  {
    disabledImages = new CImageList ;
    MakeToolbarImageList( inDisabledImageID, disabledImages, numImages ) ;
    toolbar.SetDisabledImageList( disabledImages ) ;
  }

  if ( inHotImageID != 0 )
  {
    hotImages = new CImageList ;
    MakeToolbarImageList( inHotImageID, hotImages, numImages ) ;
    toolbar.SetHotImageList( hotImages ) ;
  }
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

	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);

	attachToolbarImages( m_wndToolBar.GetToolBarCtrl(), TB_MAIN_NUM_IMAGES, IDB_TOOLBAR_ENABLED, IDB_TOOLBAR_DISABLED, 0 ) ;

        m_wndStatusBar.SetPaneInfo( 1, ID_INDICATOR_LAT, SBPS_NORMAL, 90 );
        m_wndStatusBar.SetPaneInfo( 2, ID_INDICATOR_LON, SBPS_NORMAL, 90 );
        m_wndStatusBar.SetPaneInfo( 3, ID_INDICATOR_ALT, SBPS_NORMAL, 90 );
        m_wndStatusBar.SetPaneInfo( 4, ID_SEPARATOR, SBPS_NOBORDERS, 128 );


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

void CMainFrame::OnUpdateFileOpen(CCmdUI *pCmdUI)
{
  Simple3DDirectImportView * view = (Simple3DDirectImportView *)GetActiveView( ) ;

  if ( !view->IsKindOf(RUNTIME_CLASS(Simple3DDirectImportView)) )
  {
    // Print preview changes the active view class, so ignore this
    m_wndStatusBar.SetWindowText( _T("") ) ;
  }
  else
  {
    m_wndStatusBar.SetWindowText( (LPCTSTR)TSLUTF8Decoder(view->queryPrompt()) ) ;
  }
}

afx_msg LRESULT CMainFrame::OnNewTileAvailableCallback( WPARAM wparam, LPARAM lparam)
{
  CDocument* pDoc = CFrameWnd::GetActiveDocument();
  ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(Simple3DDirectImportDoc)));
  Simple3DDirectImportDoc* wmsDoc = (Simple3DDirectImportDoc*)pDoc;
  wmsDoc->newTileAvailable( );
  return 0L;
}
