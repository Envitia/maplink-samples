// S63WorkFlow.cpp : Defines the class behaviors for the application.
//
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "stdafx.h"
#include "S63WorkFlowSample.h"

#include "MainFrm.h"
#include "S63WorkFlowDoc.h"
#include "S63WorkFlowView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static const char THIS_FILE[] = __FILE__;
#endif

// HTML help is not supported in Visual C++ 6
#if _MSC_VER > 1200
# pragma comment(lib, "htmlhelp.lib")
#endif

/////////////////////////////////////////////////////////////////////////////
// S63WorkFlowApp

//This class is a specialised command line parser.
//
//It is called by MFC for each argument on the command line.  
//This will trap the specification of the initialisation 
//file. using '-I<path>LandLinkSample.ini
class CCommandParser : public CCommandLineInfo
{
private:
  enum NextValue { Next_None, Next_Default, Next_MaplHome, Next_MapFile, Next_S57Store, Next_SimpleViewer, Next_pos1, Next_pos2, Next_pos3 };
public:

  CCommandParser()
    : 
      m_isLocalMaplHome ( false ),
      m_next            ( Next_None ),
      m_maplHome        ( "" ),
      m_simpleInterface (false),
      m_latitude        (0.0),
      m_longitude       (0.0),
      m_scale           (-1),
      m_failed          (false)
  {
  }
  bool isLocalMaplHome() const
  {
    return m_isLocalMaplHome;
  }
  string const& maplHome() const
  {
    return m_maplHome;
  }
  string const& mapFile() const
  {
    return m_mapFile;
  }
  string const& s57Store() const
  {
    return m_s57Store;
  }

  bool simpleInterface() const
  {
    return m_simpleInterface;
  }
  void position(double &latitude, double &longitude, int &scale) const
  {
    latitude = m_latitude;
    longitude = m_longitude;
    scale = m_scale;
  }
  bool failed() const
  {
    return m_failed;
  }

  public:
    //This method parses each command line argument.  
    virtual void ParseParam(const TCHAR* pszParam, BOOL bFlag, BOOL bLast);

private:
  bool      m_isLocalMaplHome;
  bool      m_simpleInterface;
  NextValue m_next;
  string    m_maplHome;
  string    m_mapFile;
  string    m_s57Store;
  double    m_longitude;
  double    m_latitude;
  int       m_scale;
  bool      m_failed;
};


void CCommandParser::ParseParam(const TCHAR* pszParam, BOOL bFlag, BOOL bLast)
{
  if ( bFlag && m_next == Next_None || m_next == Next_Default )
  {
    if ( 0 == _tcsicmp(pszParam, _T("help")))
    {
      m_next = Next_None;
      ::MessageBox(0, _T("Help:\n")
                      _T("-home|-M :Path to installation (The directory containing the config directory)\n-s|-S :Path to Store\n-w|-W :Path & filename of Base Map to load\n-p|-P :Initial Position & Scale, Latitude Longitude Scale\n-q|-Q :Simplified Interface\n")
                      , _T("Help"), MB_OK);
      m_failed = true;
    }
    else if ( pszParam[0] == 'w' || pszParam[0] == 'W' ) // Base map
    {
      m_next = Next_MapFile;
    }
    else if ( pszParam[0] == 's' || pszParam[0] == 'S' ) // S57 Store
    {
      m_next = Next_S57Store;
    }
    else if ( (0 == _tcsicmp( pszParam, _T("home") ) ) || (0 == _tcsicmp( pszParam, _T("M") )) ) // set the maplink home directory
    {
      m_isLocalMaplHome = true;
      m_next = Next_MaplHome;
    }
    else if (pszParam[0] == 'q' || pszParam[0] == 'Q')
    {
      m_simpleInterface = true;
      m_next = Next_None;
    }
    else if (pszParam[0] == 'p' || pszParam[0] == 'P')
    {
      m_next = Next_pos1;
    }
    else if( 0 == _tcsicmp( pszParam, _T("opengl") ) )
    {
      // Tell the view (which creates the drawing surfaces) that it should use the OpenGL 2D surface instead of the default
      S63WorkFlowView::useOpenGLSurface( true );
    }
    else
    {
      CCommandLineInfo::ParseParam( pszParam, bFlag, bLast );
      m_next = Next_Default;
    }
  }
  else
  {
    switch ( m_next )
    {
    case Next_MaplHome:
      {
        TSLUTF8Encoder encode(pszParam);
        m_maplHome = encode;
        if ( m_maplHome == "." )
        {
          wchar_t* directory = _wgetcwd( NULL, 0 );
          TSLUTF8Encoder dirEncode( directory );
          m_maplHome = dirEncode;
          m_maplHome += "/../";
          free(directory);
        }
        if (!m_maplHome.empty())
          TSLUtilityFunctions::setMapLinkHome( m_maplHome.c_str(), true );
      }
      m_next = Next_None;
      break;

    case Next_Default:
      CCommandLineInfo::ParseParam( pszParam, bFlag, bLast );
      m_next = Next_None;
      break;

    case Next_MapFile:
      {
        LPTSTR path = _tfullpath(NULL, pszParam, 0);

        m_mapFile = TSLUTF8Encoder(path) ;
        free( path );
        bFlag = true;
        m_next = Next_None;
      }
      break;

    case Next_S57Store:
      {
        LPTSTR path = _tfullpath(NULL, pszParam, 0);

        m_s57Store = TSLUTF8Encoder(path) ;
        free( path );
        bFlag = true;
        m_next = Next_None;
      }
      break;
    case Next_pos1:
      {
        m_next = Next_pos2;
        bFlag = false;
        TCHAR ns;
        _stscanf(pszParam, _T("%c%lf"), &ns, &m_latitude);
        if (ns == 's' || ns == 'S')
          m_latitude *= -1;
      }
      break;

    case Next_pos2:
      {
        m_next = Next_pos3;
        bFlag = false;
        TCHAR ew;
        _stscanf(pszParam, _T("%c%lf"), &ew, &m_longitude);
        if (ew == 'w' || ew == 'W')
          m_longitude *= -1;
      }
      break;

    case Next_pos3:
      {
        m_next = Next_None;
        bFlag = false;
        _stscanf(pszParam, _T("%d"), &m_scale);
      }
      break;

    default:
      CCommandLineInfo::ParseParam( pszParam, bFlag, bLast );
      break;
    }
  }
}


BEGIN_MESSAGE_MAP(S63WorkFlowApp, CWinApp)
  //{{AFX_MSG_MAP(S63WorkFlowApp)
    ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
  // NOTE - the ClassWizard will add and remove mapping macros here.
  //    DO NOT EDIT what you see in these blocks of generated code!
  //}}AFX_MSG_MAP
  // Standard file based document commands
    ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
    ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
    ON_COMMAND(ID_LOAD_HELP, OnHelp)
  // Standard print setup command
  ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// S63WorkFlowApp construction

S63WorkFlowApp::S63WorkFlowApp()
{

}

/////////////////////////////////////////////////////////////////////////////
// The one and only S63WorkFlowApp object

S63WorkFlowApp theApp;

/////////////////////////////////////////////////////////////////////////////
// S63WorkFlowApp initialization

BOOL S63WorkFlowApp::InitInstance()
{
  // Standard initialization
  // If you are not using these features and wish to reduce the size
  //  of your final executable, you should remove from the following
  //  the specific initialization routines you do not need.
  
  // InitCommonControls() is required on Windows XP if an application 
  // manifest specifies use of ComCtl32.dll version 6 or later to enable 
  // visual styles. Otherwise, any window creation will fail. 
  //InitCommonControls() ;

  INITCOMMONCONTROLSEX icex;

  // Ensure that the common control DLL is loaded. 
  icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
  icex.dwICC  = ICC_LISTVIEW_CLASSES | ICC_TREEVIEW_CLASSES | ICC_BAR_CLASSES | ICC_STANDARD_CLASSES ;
  InitCommonControlsEx(&icex);

  // Initialize OLE libraries
  if ( !AfxOleInit() )
  {
    return FALSE;
  }

  // Change the registry key under which our settings are stored.
  // You should modify this string to be something appropriate
  // such as the name of your company or organization.
  SetRegistryKey(TSL_MAPLINK_REGISTRY_STRING);
  
  LoadStdProfileSettings();  // Load standard INI file options (including MRU)

  
  // Clear the error stack so that we can get the errors that occurred here.
  TSLThreadedErrorStack::clear( ) ;

  // Register the application's document templates.  Document templates
  //  serve as the connection between documents, frame windows and views.
  
  CSingleDocTemplate* pDocTemplate;
  pDocTemplate = new CSingleDocTemplate(
    IDR_MAINFRAME,
    RUNTIME_CLASS(S63WorkFlowDoc),
    RUNTIME_CLASS(CMainFrame),       // main SDI frame window
    RUNTIME_CLASS(S63WorkFlowView));
  AddDocTemplate(pDocTemplate);
  
  // Parse command line for standard shell commands, DDE, file open
  CCommandParser cmdInfo ;
  ParseCommandLine( cmdInfo );
    
  if( cmdInfo.failed() )
  {
    return false;
  }

  // Dispatch commands specified on the command line
  if (!ProcessShellCommand(cmdInfo))
    return FALSE;

  if (cmdInfo.isLocalMaplHome())
  {
    TSLUtilityFunctions::setMapLinkHome(cmdInfo.maplHome().c_str());
  }

  // Initialise the drawing surface data files.
  // This call uses the TSLUtilityFunctions::getMapLinkHome method to determine
  // where MapLink is currently installed.  It then proceeds to load the
  // following files : tsllinestyles.dat, tslfillstyles.dat, tslfonts.dat, tslsymbols.dat
  // and tslcolours.dat
  // When deploying your application, pass in a full path to the directory containing
  // these files.

  string mapLinkConfigDir;
  if (!getMapLinkConfigDirectory(mapLinkConfigDir))
  {
    AfxMessageBox(_T("Unable to find the MapLink configuration directory."), MB_ICONERROR ) ;
  }

  TSLDrawingSurface::loadStandardConfig(mapLinkConfigDir.c_str());

  // Check for any errors that have occurred, and display them
  TSLSimpleString msg;
  TSLThreadedErrorStack::errorString(msg) ;

  if ( !msg.empty() )
  {
    // If we have any errors during initialisation, display the message
    // and exit.
    AfxMessageBox( (LPCTSTR)TSLUTF8Decoder(msg), MB_ICONERROR ) ;
    exit( 0 ) ;
  }

  S63WorkFlowView *view = S63WorkFlowView::GetView();

  // re-configure the toolbar
  if (cmdInfo.simpleInterface())
  {
    ((CMainFrame*)m_pMainWnd)->simpleInterface();
    if (view)
      view->simpleInterface();
  }

  // The one and only window has been initialized, so show and update it.
  m_pMainWnd->ShowWindow(SW_SHOW);
  m_pMainWnd->UpdateWindow();
    
  // Enable drag/drop open
  m_pMainWnd->DragAcceptFiles();

  // load the map and store passed on the command line. 
  const string &mapFile = cmdInfo.mapFile();
  const string &s57Store = cmdInfo.s57Store();

  S63WorkFlowDoc * doc = ( S63WorkFlowDoc * )( ((CMainFrame *)AfxGetMainWnd())->GetActiveDocument() ) ;

  if ( !mapFile.empty() )
  {
    OpenDocumentFile( (LPCTSTR)TSLUTF8Decoder(mapFile) ) ;
  }


  if (view)
  {
    if ( !s57Store.empty() )
    {
      double latitude, longitude;
      int scale;
      cmdInfo.position(latitude, longitude, scale);
      view->loadS57DataStore( s57Store, latitude, longitude, scale ) ;
    }
  }

  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// S63WorkFlowApp commands

int S63WorkFlowApp::ExitInstance() 
{
  TSLDrawingSurface::cleanup();

  return CWinApp::ExitInstance();
}

void S63WorkFlowApp::OnHelp() 
{
// HTML help is not supported in Visual C++ 6
#if _MSC_VER > 1200

  // Build up a path to the HTML help file.
  string mapLinkConfigDir;

  if (!getMapLinkConfigDirectory(mapLinkConfigDir))
  {
    AfxMessageBox(_T("Unable to find the S52 configuration directory."), MB_ICONERROR ) ;
    return ;
  }

  string helpPath;
  helpPath = mapLinkConfigDir + "/../help/S63WorkflowSample.chm";

  if ( !::HtmlHelp(m_pMainWnd->m_hWnd, (LPCTSTR)TSLUTF8Decoder(helpPath), HH_DISPLAY_TOC, 0 ) )
  {
    // If it returns false, return that the file could not be located.
    AfxMessageBox(_T("Could not locate the HTML help file."), MB_ICONERROR);
  }
#endif
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
  CAboutDlg();
  ~CAboutDlg(){::DeleteObject(m_logo);}
  
  // Dialog Data
  //{{AFX_DATA(CAboutDlg)
  enum { IDD = IDD_ABOUTBOX };
  //}}AFX_DATA

  afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
  virtual BOOL OnInitDialog();
  afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
  
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CAboutDlg)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL
  
  // Implementation
protected:
  //{{AFX_MSG(CAboutDlg)
		// No message handlers
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
private:
  CStatic m_thirdParty;
  HBITMAP m_logo; 
  RGBQUAD m_ct;
};

CAboutDlg::CAboutDlg() 
: CDialog(CAboutDlg::IDD)
, m_thirdParty()
, m_logo(0)
{
  //{{AFX_DATA_INIT(CAboutDlg)
  //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CAboutDlg)
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_THIRDPARTY, m_thirdParty);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
//{{AFX_MSG_MAP(CAboutDlg)
// No message handlers
//}}AFX_MSG_MAP
ON_WM_CTLCOLOR()
ON_WM_DRAWITEM()
END_MESSAGE_MAP()

// App command to run the dialog
void S63WorkFlowApp::OnAppAbout()
{
  CAboutDlg aboutDlg;
  aboutDlg.DoModal();
}

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

  m_logo = (HBITMAP)LoadImage( GetModuleHandle(0), MAKEINTRESOURCE(IDB_ABOUT_BOX_IMG), IMAGE_BITMAP, 
                              0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE );
  memset(&m_ct, 0, sizeof(RGBQUAD));
  const COLORREF bg = GetSysColor(COLOR_BTNFACE);
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