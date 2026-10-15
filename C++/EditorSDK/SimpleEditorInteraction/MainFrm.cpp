// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "SimpleEditorInteraction.h"
#include "SimpleEditorInteractionDoc.h"
#include "SimpleEditorInteractionView.h"
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
  //}}AFX_MSG_MAP
  ON_UPDATE_COMMAND_UI( ID_NUM_SELECTED, OnUpdateSelected )
END_MESSAGE_MAP()

static UINT indicators[] =
{
  ID_SEPARATOR,           // status line indicator
    ID_NUM_SELECTED
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
  delete normalImages;
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

  // Setup the status bars and toolbars
  m_wndStatusBar.SetPaneInfo( 1, ID_NUM_SELECTED, SBPS_NORMAL, 120 ) ;

  m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
    CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
  m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);

  attachToolbarImages( m_wndToolBar.GetToolBarCtrl(), TB_MAIN_NUM_IMAGES, IDB_TOOLBAR_ENABLED, IDB_TOOLBAR_DISABLED, 0 ) ;



  EnableDocking(CBRS_ALIGN_ANY);
  DockControlBar(&m_wndToolBar);

  return 0;
}

void CMainFrame::OnUpdateSelected( CCmdUI * pCmdUI )
{
  SimpleEditorInteractionView * view = (SimpleEditorInteractionView *)GetActiveView( ) ;

  TCHAR buffer[ 256 ] ;

  if ( !view->IsKindOf(RUNTIME_CLASS(SimpleEditorInteractionView)) )
  {
    // Print preview changes the active view class, so ignore this
    m_wndStatusBar.SetWindowText( _T("") ) ;
    buffer[0] = _T('\0');
  }
  else
  {
    int num, depth ;
    num = view->querySelectList( depth ) ;
    
    if ( depth )
      _sntprintf( buffer, 256, _T("Select (%d) : %d"), depth, num ) ;
    else
      _sntprintf( buffer, 256, _T("Select : %d"), num ) ;

    m_wndStatusBar.SetWindowText( (LPCTSTR)TSLUTF8Decoder(view->queryPrompt()) ) ;
  }

  pCmdUI->Enable( ) ;
  pCmdUI->SetText( buffer ) ;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
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
