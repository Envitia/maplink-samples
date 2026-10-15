// ModelHierarchyViewerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ModelHierarchyViewer.h"
#include "ModelHierarchyViewerDlg.h"
#include "ModelHierarchy.h"
#include "EditData.h"

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
  CAboutDlg();
  ~CAboutDlg(){::DeleteObject(m_logo);}

  afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
  virtual BOOL OnInitDialog();
  afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
  
  // Dialog Data
  //{{AFX_DATA(CAboutDlg)
  enum { IDD = IDD_ABOUTBOX };
  //}}AFX_DATA
  
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CAboutDlg)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL
  
  // Implementation
protected:
  //{{AFX_MSG(CAboutDlg)
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
private:
  CStatic m_thirdParty;
  HBITMAP m_logo; 
  RGBQUAD m_ct;
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
  m_logo = 0;
  //{{AFX_DATA_INIT(CAboutDlg)
  //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CAboutDlg)
  DDX_Control(pDX, IDC_THIRDPARTY, m_thirdParty);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
//{{AFX_MSG_MAP(CAboutDlg)
ON_WM_CTLCOLOR()
ON_WM_DRAWITEM()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
  HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
  if ( pWnd == &m_thirdParty )
  {
    pDC->SetTextColor(GetSysColor(COLOR_BTNSHADOW));
  }
  return hbr;
}

BOOL CAboutDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_logo = (HBITMAP)LoadImage( GetModuleHandle(0), MAKEINTRESOURCE(IDB_ABOUT_BOX_IMG), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE );
  memset(&m_ct, 0, sizeof(RGBQUAD));
  COLORREF bg = GetSysColor(COLOR_BTNFACE);
  m_ct.rgbBlue = GetBValue(bg); m_ct.rgbGreen = GetGValue(bg); m_ct.rgbRed = GetRValue(bg);

  return TRUE; 
}

void CAboutDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
  CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
  if ( nIDCtl == IDC_LOGO && lpDrawItemStruct)
  {
    HDC hMemDC = CreateCompatibleDC(lpDrawItemStruct->hDC);
    HGDIOBJ old = ::SelectObject(hMemDC, m_logo);    
    
    
    SetDIBColorTable( hMemDC, 110, 1, &m_ct );    
    ::BitBlt(lpDrawItemStruct->hDC, 
             lpDrawItemStruct->rcItem.left,
             lpDrawItemStruct->rcItem.top,
             lpDrawItemStruct->rcItem.right,
             lpDrawItemStruct->rcItem.bottom,
             hMemDC,
             0, 0,
             SRCCOPY);
    ::SelectObject(hMemDC, old);
    ::DeleteDC(hMemDC);
  }
}

/////////////////////////////////////////////////////////////////////////////
// CModelHierarchyViewerDlg dialog

CModelHierarchyViewerDlg::CModelHierarchyViewerDlg(CWnd* pParent /*=NULL*/)
  : CDialog(CModelHierarchyViewerDlg::IDD, pParent)
{
  // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CModelHierarchyViewerDlg::~CModelHierarchyViewerDlg()
{
  clearLoadedModels();

  if ( m_pimagelist )
  {
    m_pimagelist->DeleteImageList();
    delete m_pimagelist;
    m_pimagelist = 0;
  }

  TSLDrawingSurface::cleanup();
}

void CModelHierarchyViewerDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control( pDX, IDC_TREE, m_tree );
  DDX_Control( pDX, IDC_COLLAPSE_TREE, m_collapseButton );
  DDX_Control( pDX, IDC_OPEN_MODEL, m_openButton );
  DDX_Control( pDX, IDCANCEL, m_closeButton );
}

BEGIN_MESSAGE_MAP(CModelHierarchyViewerDlg, CDialog)
  ON_WM_SYSCOMMAND()
  ON_WM_PAINT()
  ON_WM_QUERYDRAGICON()
  ON_BN_CLICKED(IDC_OPEN_MODEL, &CModelHierarchyViewerDlg::OnOpenModel)
  ON_NOTIFY(NM_DBLCLK, IDC_TREE, &CModelHierarchyViewerDlg::OnDblclkCSysTree)
  ON_BN_CLICKED(IDC_COLLAPSE_TREE, &CModelHierarchyViewerDlg::OnCollapseTree)
  ON_WM_SIZE()
  ON_WM_GETMINMAXINFO()
  ON_WM_DROPFILES()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModelHierarchyViewerDlg message handlers

BOOL CModelHierarchyViewerDlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  
  int				i = 0 ;
  CBitmap		bitmap ;
  static const UINT nID[ ] = { IDB_BLANK, IDB_POLYLINE, IDB_POLYGON, IDB_POLYGON_OUTER, 
                               IDB_POLYGON_INNER, IDB_TEXT, IDB_SYMBOL, IDB_GROUP, 
                               IDB_ARC, IDB_ELLIPSE, IDB_RECTANGLE, IDB_BOTTOM_LEFT, 
                               IDB_TOP_RIGHT, IDB_HEIGHT, IDB_ROTATION, IDB_TEXT_STRING,
                               IDB_POINTS, IDB_UNKNOWN };



  m_pimagelist = new CImageList() ;
  m_pimagelist->Create( 20, 20, ILC_COLOR4 | ILC_MASK, 9,9 ) ;
  for ( i = 0; i < _countof(nID) ; ++i )  // load bitmaps
  {
    bitmap.LoadBitmap( nID[ i ] ) ;
    m_pimagelist->Add( &bitmap, RGB(255,255,255) ) ;
    bitmap.DeleteObject() ;
  }
  m_tree.SetImageList( m_pimagelist, TVSIL_NORMAL ) ;
  
  // IDM_ABOUTBOX must be in the system command range.
  ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
  ASSERT(IDM_ABOUTBOX < 0xF000);
  
  CMenu* pSysMenu = GetSystemMenu(FALSE);
  if (pSysMenu != NULL)
  {
    CString strAboutMenu;
    strAboutMenu.LoadString(IDS_ABOUTBOX);
    if (!strAboutMenu.IsEmpty())
    {
      pSysMenu->AppendMenu(MF_SEPARATOR);
      pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
    }
  }
  
  // Set the icon for this dialog.  The framework does this automatically
  //  when the application's main window is not a dialog
  SetIcon(m_hIcon, TRUE);			// Set big icon
  SetIcon(m_hIcon, FALSE);		// Set small icon
  
  // TODO: Add extra initialization here

  if ( CModelHierarchyViewerApp::m_filename != "" )
  {
    m_tree.DeleteAllItems();

    const CString& fileName = CModelHierarchyViewerApp::m_filename;
    loadFile( fileName );
  }
  
  return TRUE;  // return TRUE  unless you set the focus to a control
}

void CModelHierarchyViewerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
  if ((nID & 0xFFF0) == IDM_ABOUTBOX)
  {
    CAboutDlg dlgAbout;
    dlgAbout.DoModal();
  }
  else
  {
    CDialog::OnSysCommand(nID, lParam);
  }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CModelHierarchyViewerDlg::OnQueryDragIcon()
{
  return (HCURSOR) m_hIcon;
}




void CModelHierarchyViewerDlg::OnOpenModel() 
{
  static  CString  lastDir;

  CFileDialog dlg( TRUE, NULL, 0, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT | OFN_NOCHANGEDIR,
                   _T("TMF Files (*.tmf)|*.tmf||") );
  TCHAR	files[8192];
  dlg.m_ofn.lpstrFile = files;
  dlg.m_ofn.nMaxFile = 8192;
  dlg.m_ofn.lpstrInitialDir = lastDir.IsEmpty() ? NULL : (LPCTSTR)lastDir;
  files[0] = '\0';
  if ( dlg.DoModal() != IDOK )
    return;

  m_tree.DeleteAllItems();

  clearLoadedModels();

  POSITION pos;
  pos = dlg.GetStartPosition();
  while ( pos )
  {
    const CString& fileName = dlg.GetNextPathName( pos );
    loadFile( fileName );
  }
}

ModelHierarchy* CModelHierarchyViewerDlg::findEntity(const HTREEITEM& hItem)
{
   //! find entity
   for( vector<ModelHierarchy*>::iterator modelIt = m_models.begin(); modelIt != m_models.end() ; ++modelIt )
   {
     ModelHierarchy* model = *modelIt;
     for( vector< pair< TSLEntity*, HTREEITEM> >::iterator it = model->m_entityItemPairs.begin(); it != model->m_entityItemPairs.end(); ++it )
     {
       if(it->second == hItem)
       {
         model->m_foundEntity = it->first;
         return model;
       }
     }
   }

   return NULL;
}

void CModelHierarchyViewerDlg::clearLoadedModels()
{
  for ( vector<ModelHierarchy*>::iterator modelIt = m_models.begin(); modelIt != m_models.end(); ++modelIt )
  {
    ModelHierarchy* model = *modelIt;
    delete model;
  }
  m_models.clear();
}

void CModelHierarchyViewerDlg::loadFile( const CString& filename )
{
  ModelHierarchy* model = new ModelHierarchy( filename, m_tree );
  model->ViewHierarchyFile();
  m_models.push_back( model );
}

void CModelHierarchyViewerDlg::OnDblclkCSysTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
  *pResult = 0;

  HTREEITEM hItem = m_tree.GetSelectedItem();
  if(hItem == NULL)
    return;

  //! check if it is in the text tree
  CString strItemText = m_tree.GetItemText(hItem);
  int find = strItemText.Find(_T("Text"));
  if( find == -1)
  {
    hItem = m_tree.GetParentItem(hItem);
    strItemText = m_tree.GetItemText(hItem);
    find = strItemText.Find(_T("Text"));
    if( find == -1)
    {
      return;
    }
  }

  //! find entity
  ModelHierarchy* foundModel = findEntity(hItem);
  if (!foundModel)
  {
    return;    // this is a file node
  }

  //! edit entity 
  TSLEntity* foundEntity = foundModel->m_foundEntity;
  if ( TSLText* textEntity = TSLText::isText( foundEntity ) )
  {
    EditData sys;
    sys.setEntity( textEntity );
    if ( sys.DoModal() == IDOK )
    {
      foundModel->m_dataLayer->saveData( TSLUTF8Encoder( foundModel->m_fileName ) );

      //! Redraw the tree
      m_tree.DeleteAllItems();
      foundModel->ViewHierarchyFile();
    }
  }
}

void ExpandTreeItem(CTreeCtrl &tree, HTREEITEM &hItem, UINT nCode)
{
  HTREEITEM hChild;
  
  if(tree.ItemHasChildren(hItem))
  {
    tree.Expand(hItem, nCode);
    hChild = tree.GetChildItem(hItem);
    
    while(hChild)
    {
      ExpandTreeItem(tree, hChild, nCode);
      hChild = tree.GetNextItem(hChild, TVGN_NEXT);
    }
  }
}

void CModelHierarchyViewerDlg::OnCollapseTree()
{
  CString task;
  m_collapseButton.GetWindowText(task);
  UINT nCode = (task == "Expand Tree") ? TVE_EXPAND : TVE_COLLAPSE;
  

  HTREEITEM hItem = m_tree.GetRootItem();
  if(hItem == NULL)//< tree is empty
    return;
  while(hItem != NULL)
  {
    ExpandTreeItem(m_tree, hItem, nCode);
    hItem = m_tree.GetNextVisibleItem(hItem);
  }

  CString nexttask = (task == "Expand Tree") ? "Collapse Tree" : "Expand Tree";
  m_collapseButton.SetWindowText(nexttask);
}

static int buttonHeight( CButton& b)
{
  RECT rc;
  b.GetClientRect( &rc );
  return rc.bottom;
}
static int buttonWidth( CButton& b )
{
  RECT rc;
  b.GetClientRect( &rc );
  return rc.right;
}

void CModelHierarchyViewerDlg::OnSize( UINT nType, int cx, int cy )
{
  CDialog::OnSize( nType, cx, cy );

  if ( !m_collapseButton.GetSafeHwnd() )
    return;    // window not initialised yet

  const int M = 10;     // margin
  const int Mx2 = M + M;
  const int Mx3 = Mx2 + M;
  int h = buttonHeight( m_collapseButton );

  m_tree.MoveWindow( M, M, cx - Mx2, cy - Mx3 - h );

  m_collapseButton.MoveWindow( M, cy - M - h, buttonWidth( m_collapseButton ), h );
  m_closeButton.MoveWindow( cx - M - buttonWidth( m_closeButton ), cy - M - h, buttonWidth( m_closeButton ), h );
  m_openButton.MoveWindow( cx - Mx2 - buttonWidth( m_closeButton ) - buttonWidth( m_openButton ), cy - M - h, buttonWidth( m_openButton ), h );
}


void CModelHierarchyViewerDlg::OnGetMinMaxInfo( MINMAXINFO* lpMMI )
{
  lpMMI->ptMinTrackSize.x = 300;    // min wnd width
  lpMMI->ptMinTrackSize.y = 100;    // min wnd height
}


void CModelHierarchyViewerDlg::OnDropFiles( HDROP hDropInfo )
{
  UINT n = ::DragQueryFile( hDropInfo, (UINT)-1, NULL, 0 );
  for ( UINT i = 0; i < n; i++ )
  {
    TCHAR buf[MAX_PATH] = _T("");
    ::DragQueryFile( hDropInfo, i, buf, _countof( buf ) );
    loadFile( buf );
  }

  CDialog::OnDropFiles( hDropInfo );
}
