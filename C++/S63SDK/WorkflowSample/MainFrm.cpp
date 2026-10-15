// MainFrm.cpp : implementation of the CMainFrame class
//
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "stdafx.h"
#include "S63WorkFlowSample.h"

#include "MainFrm.h"
#include "stdafx.h"
#include "S63WorkFlowDoc.h"
#include "S63WorkFlowView.h"
#include "textview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void addLogLine(const char* line)
{
  string logLine(line);
  addLogLine(logLine);
}

void addLogLine(const std::string &line)
{
  CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
  if (pWnd)
  {
    CTextView *ct = (CTextView*)pWnd->getTextView();
    if (ct)
    {
      SYSTEMTIME st;
      GetSystemTime(&st);

      char logMessage[1028];

      if ( line.length() > 1000 )
      {
        const string subString = line.substr(0, 1000);
        sprintf(logMessage, "[%02d:%02d:%02d] - %s%s", st.wHour, st.wMinute, st.wSecond, subString.c_str(), " ... ");
      }
      else
      {
        sprintf(logMessage, "[%02d:%02d:%02d] - %s", st.wHour, st.wMinute, st.wSecond, line.c_str());
      }

      string logLine(logMessage);
      ct->addLine(logLine);
    }
  }
}
bool logFile(const std::string &logFile)
{
  CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
  if (pWnd)
  {
    CTextView *ct = (CTextView*)pWnd->getTextView();
    if (ct)
    {
      if (ct->logFile(logFile))
      {
        return true;
      }
    }
  }
  AfxMessageBox(_T("Unable to create the log file"));
  return false;
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
  //{{AFX_MSG_MAP(CMainFrame)
  // NOTE - the ClassWizard will add and remove mapping macros here.
  //    DO NOT EDIT what you see in these blocks of generated code !
  ON_WM_CREATE()
  //}}AFX_MSG_MAP
  ON_WM_SIZE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
    ID_SEPARATOR,           // status line indicator
    ID_CURRENT_SCALE,           // status line indicator
    ID_SEPARATOR,
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
  : m_wndStatusBar()
  , m_wndToolBar()
  , normalImages()
  , disabledImages()
  , hotImages()
  , m_wndSplitter()
  , m_textView()
  , m_simpleInterface(false)
{
  normalImages = 0;
  disabledImages = 0;
  hotImages = 0;
  m_textView = NULL;
  m_created = false;
}

CMainFrame::~CMainFrame()
{
  delete normalImages ;
  delete disabledImages;
  delete hotImages;

  m_textView = NULL;

  // Cleaup the static data.
  TSLDrawingSurface::cleanup();
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

  // Zoom Scale
  m_wndToolBar.createComboBox( ID_BOX_BUTTON, ID_COMBO_BOX );
  m_wndToolBar.m_zoomCombo.AddScaleValues();

  // Scale Band
  //m_wndToolBar.createScaleBandComboBox( ID_BOX_BUTTON_SD, ID_BOX_BUTTON_SD );
  //m_wndToolBar.m_scaleBandCombo.AddScaleBandValues();

  //
  m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
    CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
  m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);

  attachToolbarImages( m_wndToolBar.GetToolBarCtrl(), TB_MAIN_NUM_IMAGES, IDB_TOOLBAR_ENABLED, IDB_TOOLBAR_DISABLED, 0 ) ;
  EnableDocking(CBRS_ALIGN_ANY);
  DockControlBar(&m_wndToolBar);
  
  //////////////////////////////////    
  if (!m_wndStatusBar.Create(this) ||
    !m_wndStatusBar.SetIndicators(indicators,
    sizeof(indicators)/sizeof(UINT)))
  {
    TRACE0("Failed to create status bar\n");
    return -1;      // fail to create
  }
  // Set up the Status Bar    
  UINT id, style;
  int width ;
  
  m_wndStatusBar.GetPaneInfo (0, id, style, width);
  m_wndStatusBar.SetPaneInfo (0, id, 0, 600);

  m_wndStatusBar.GetPaneInfo (1, id, style, width);
  m_wndStatusBar.SetPaneInfo (1, id, 0, 200);
  
  m_wndStatusBar.GetPaneInfo (2, id, style, width);
  m_wndStatusBar.SetPaneInfo (2, id, SBPS_STRETCH, 150);
  m_wndStatusBar.SetPaneText(2, _T("Lat : 0.0, Lon : 0.0"));

  CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

  return 0;
}

void CMainFrame::simpleInterface()
{
  m_simpleInterface = true;
  CMenu *menu = GetMenu();

 // menu->Detach();
  //menu->LoadMenu(IDR_MAINFRAME1);
  //http://www.functionx.com/visualc/menus/introduction.htm

  int index = 4;
  menu->DeleteMenu(index, MF_BYPOSITION);
  menu->DeleteMenu(index, MF_BYPOSITION);
  menu->DeleteMenu(index, MF_BYPOSITION);
  menu->DeleteMenu(index, MF_BYPOSITION);
  menu->DeleteMenu(index, MF_BYPOSITION);

  // force a redraw of the menu bar
  DrawMenuBar();
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
  m_wndSplitter.CreateStatic(this, 2, 1);

  m_created = true;
  CRect cr; 
  GetClientRect( &cr);
  if(!m_wndSplitter.CreateView(
    0, 0,
    RUNTIME_CLASS(S63WorkFlowView),
    CSize(cr.Width(), cr.Height()/10 * 9),
    pContext))
  {
    TRACE0("Failed to create view ");
    return FALSE;
  }
  if(!m_wndSplitter.CreateView(
    1, 0,
    RUNTIME_CLASS(CTextView),
    CSize(cr.Width(), cr.Height()/10),
    pContext))
  {
    TRACE0("Failed to create view ");
    return FALSE;
  }
  m_textView = m_wndSplitter.GetPane(1, 0);
  m_wndSplitter.RecalcLayout();
  return true;
}

void CMainFrame::resizeSplitter()
{
  CRect cr; 
  GetClientRect( &cr);

  int heightBottom = (cr.Height()/10) > 100 ? (cr.Height()/10) : 100;
  int heightTop = cr.Height() - heightBottom;

  m_wndSplitter.SetRowInfo(0, heightTop, 0);   // zero in this context for the 3rd param' is significant.
  m_wndSplitter.SetRowInfo(1, heightBottom, 0);
  m_wndSplitter.SetColumnInfo(0, cr.Width(), 0);

  m_wndSplitter.RecalcLayout();
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
  CFrameWnd::OnSize(nType, cx, cy);

  if (!m_created || nType == SIZE_MINIMIZED )
    return ;
  resizeSplitter();
}
