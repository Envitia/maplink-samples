// TerrainViewer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TerrainViewer.h"

#include "MainFrm.h"
#include "TerrainViewerDoc.h"
#include "TerrainViewerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// HTML help is not supported in Visual C++ 6
#if _MSC_VER > 1200
# pragma comment(lib, "htmlhelp.lib")
#endif

#include <direct.h>

//This class is a specialised command line parser.
//
//It is called by MFC for each argument on the command line.  
//This will trap the specification of the initialisation 
//file. using '-I<path>TerrainViewer.ini
class CCommandParser : public CCommandLineInfo
{
private:
  enum NextValue { Next_None, Next_Default, Next_MaplHome, Next_IniFile };
public:

  CCommandParser()
    : 
      m_isLocalMaplHome ( false ),
      m_next            ( Next_None ),
      m_maplHome        ( "" ),
      m_failed          ( false )
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
  bool failed() const
  {
    return m_failed;
  }
  public:
    //This method parses each command line argument.  When it 
    //is triggered with a -I, is sets up the configuration 
    //filename in the TSLProfileHelper class.
    virtual void ParseParam(const TCHAR* pszParam, BOOL bFlag, BOOL bLast);

private:
  bool      m_isLocalMaplHome;
  NextValue m_next;
  string    m_maplHome;
  bool      m_failed;
};


void CCommandParser::ParseParam(const TCHAR* pszParam, BOOL bFlag, BOOL bLast)
{
  if ( bFlag )
  {
    if ( 0 == _tcsicmp(pszParam, _T("help")))
    {
      m_next = Next_None;
      ::MessageBox(0, _T("Help:\n")
                      _T("  app /home path_to_install\t(The directory containing the config directory)\n")
                      _T("  app /I path_to_iniFile\t(The configuration file to use)")
                      , _T("Help"), MB_OK);
      m_failed = true;
    }
    else if ( pszParam[0] == 'i' || pszParam[0] == 'I' ) 
    {
      m_next = Next_IniFile;
    }
    else if ( 0 == _tcsicmp( pszParam, _T("home")) || pszParam[0] == _T('m') || pszParam[0] == _T('M'))
    {
      m_isLocalMaplHome = true;
      m_next = Next_MaplHome;
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

    case Next_IniFile:
      {
        string iniFile = TSLUTF8Encoder(pszParam) ;
        size_t pos = iniFile.find_first_not_of(" \n\v\f\r\t");
        if( pos != string::npos )
        {
          iniFile = iniFile.substr( pos, iniFile.length() );
        }

        TSLProfileHelper::setDefaultFilename( iniFile.c_str() ) ;

        TSLProfileHelper::setDefaultSection ( "options" ) ;

        m_next = Next_None;
      }
      break;

    default:
      CCommandLineInfo::ParseParam( pszParam, bFlag, bLast );
      break;
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
// CTerrainViewerApp

BEGIN_MESSAGE_MAP(CTerrainViewerApp, CWinApp)
//{{AFX_MSG_MAP(CTerrainViewerApp)
ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
#if _MSC_VER > 1200
ON_COMMAND(ID_HELP, OnHelp)
#endif
ON_COMMAND(ID_HELP_CONTACT_ENVITIA, OnHelpContactEnvitia)
//}}AFX_MSG_MAP
// Standard file based document commands
ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
// Standard print setup command
ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTerrainViewerApp construction

CTerrainViewerApp::CTerrainViewerApp()
:m_helpPath( "" )
{
  // TODO: add construction code here,
  // Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTerrainViewerApp object

CTerrainViewerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTerrainViewerApp initialization

BOOL CTerrainViewerApp::InitInstance()
{
  // Standard initialization
  // If you are not using these features and wish to reduce the size
  //  of your final executable, you should remove from the following
  //  the specific initialization routines you do not need.
  
  // InitCommonControls() is required on Windows XP if an application 
  // manifest specifies use of ComCtl32.dll version 6 or later to enable 
  // visual styles. Otherwise, any window creation will fail. 
  InitCommonControls() ;
  
  // Change the registry key under which our settings are stored.
  // You should modify this string to be something appropriate
  // such as the name of your company or organization.
  SetRegistryKey(TSL_MAPLINK_REGISTRY_STRING);
  
  LoadStdProfileSettings();  // Load standard INI file options (including MRU)
  
  CoInitialize(NULL);

  // Register the application's document templates.  Document templates
  //  serve as the connection between documents, frame windows and views.
  
  CSingleDocTemplate* pDocTemplate;
  pDocTemplate = new CSingleDocTemplate(
    IDR_MAINFRAME,
    RUNTIME_CLASS(CTerrainViewerDoc),
    RUNTIME_CLASS(CMainFrame),       // main SDI frame window
    NULL) ; //RUNTIME_CLASS(CTerrainViewerView));
  AddDocTemplate(pDocTemplate);
  
  TSLProfileHelper::setDefaultFilename( "" ) ;

  // Parse command line for the ini file
  CCommandParser cmdInfo ;
  ParseCommandLine( cmdInfo );

  if( cmdInfo.failed() )
  {
    return false;
  }

  TSLDrawingSurface::loadStandardConfig() ;
  TSLCoordinateSystem::loadCoordinateSystems();
  
  // Check for any errors that have occurred, and display them
  const char * msg = TSLErrorStack::errorString() ;
  
  if ( msg )
  {
    // If we have any errors during initialisation, display the message
    // and exit.
    CString error;
    error.Format( _T("Error reading configuration files: %s"), (LPCTSTR)TSLUTF8Decoder(msg) );
    AfxMessageBox( error, MB_ICONERROR ) ;
    TSLDrawingSurface::cleanup();
    exit( 0 ) ;
  }

  TSLSimpleString ini;
  TSLProfileHelper::getDefaultFilename (ini);
  const char * iniPath = ini.c_str() ; 

  CFileStatus status ;

  if ( !iniPath || !CFile::GetStatus( (LPCTSTR)TSLUTF8Decoder(iniPath), status ) )
  {
    const char * maplinkUserHome = TSLUtilityFunctions::getMapLinkUserHome() ;
    string userIniFilename = maplinkUserHome ;
    userIniFilename += "\\config\\" ;
    userIniFilename += ( iniPath && iniPath[0] != '\0' ) ? iniPath : "TerrainViewer.ini" ;
    
    if( !TSLFileHelper::fileExists( userIniFilename.c_str()))
    {
      const char * maplinkHome = TSLUtilityFunctions::getMapLinkHome() ;
      string defaultIniFilename;
      if (NULL == maplinkHome)
      {
        // assume the config directory is one up from where we were run from.
        wchar_t* cwd = _wgetcwd(NULL, 0);
        TSLUTF8Encoder currentWorkingDirectory( cwd );
        defaultIniFilename = currentWorkingDirectory;
        defaultIniFilename += "/../";
        free( cwd );
      }
      else
      {
        defaultIniFilename = maplinkHome ;
      }
      defaultIniFilename += "\\config\\" ;
      defaultIniFilename += ( iniPath && iniPath[0] != '\0' ) ? iniPath : "TerrainViewer.ini" ;
      string maplinkUserConfigDir = maplinkUserHome ;
      maplinkUserConfigDir += "\\config\\" ;

      //Create user home directory if needed
      if( !TSLFileHelper::directoryExists( maplinkUserHome ))
      {    
        TSLFileHelper::createDirectory( maplinkUserHome ) ;
      }
      if( !TSLFileHelper::directoryExists( maplinkUserConfigDir.c_str() ))
      {
        TSLFileHelper::createDirectory( maplinkUserConfigDir.c_str() ) ;
      }
      //copy the default ini file to the user home
      TSLFileHelper::copyFile( defaultIniFilename.c_str(), userIniFilename.c_str() ) ;
    }

    TSLProfileHelper::setDefaultFilename( userIniFilename.c_str() ) ;
    TSLProfileHelper::setDefaultSection ( "options" ) ;

    TSLSimpleString ini;
    TSLProfileHelper::getDefaultFilename (ini);
    const char * iniFile = ini.c_str() ;

    if ( !CFile::GetStatus( (LPCTSTR)TSLUTF8Decoder(iniFile), status ) )
    {
      AfxMessageBox( _T("No initialisation file specified on the command line eg. '-I TerrainViewer.ini'\n")
		     _T("and a default one could not be added.\n\nAdd this to the shortcut parameters or Settings->Debug->Program arguments"), MB_ICONERROR ) ;
      TSLDrawingSurface::cleanup();
      exit( 1 ) ;
    }
  }
  
  string map = "", terrain = "" ;
  char path[ TSL_MAX_WPATH ] = "" ;

  // look in the ini file to find out whether any map or terrain data has been specified
  TSLProfileHelper::setDefaultSection ( "paths" ) ;
  
  TSLProfileHelper::lookupProfile( "map", path, TSL_MAX_WPATH, "" ) ;
  map = path ;

  TSLProfileHelper::lookupProfile( "terrain", path, TSL_MAX_WPATH, "" ) ;
  terrain = path ;

  TSLProfileHelper::setDefaultSection ( "options" ) ;

  // Dispatch commands specified on the command line
  if ( !ProcessShellCommand( cmdInfo ) )
    return FALSE;
  
  // The one and only window has been initialized, so show and update it.
  m_pMainWnd->ShowWindow(SW_MAXIMIZE);
  m_pMainWnd->UpdateWindow();
  
  // Enable drag/drop open
  m_pMainWnd->DragAcceptFiles();

  CTerrainViewerDoc * doc = ( CTerrainViewerDoc * )( ((CMainFrame *)AfxGetMainWnd())->GetActiveDocument() ) ;

  // Check to see whether we have found a map
  // in the config file
  if ( map.compare( "" ) != 0 )
  {
    wchar_t fullPath[ TSL_MAX_WPATH ] ;

    _wfullpath( fullPath, (LPCWSTR)TSLUTF8Decoder(map), TSL_MAX_WPATH ) ;
  
    OpenDocumentFile( fullPath ) ;
  }

  // Check to see whether we have found a terrain database
  // in the config file
  if ( terrain.compare( "" ) != 0 )
  {
    wchar_t fullPath[ TSL_MAX_WPATH ] ;

    _wfullpath( fullPath, (LPCWSTR)TSLUTF8Decoder(terrain), TSL_MAX_WPATH ) ;
    
    doc->openTerrain( fullPath ) ;
  }
  
  // Build the path to the help file
  if (TSLUtilityFunctions::getMapLinkHome())
  {
    m_helpPath = TSLUtilityFunctions::getMapLinkHome();
    m_helpPath += "/help/TerrainViewer.chm";
  }
  return TRUE;
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
  CStatic	m_thirdParty;
  CString	m_version;
  HBITMAP m_logo; 
  RGBQUAD m_ct;
  //}}AFX_DATA
  
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CAboutDlg)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL
  
  // Implementation
protected:
  //{{AFX_MSG(CAboutDlg)
  virtual BOOL OnInitDialog();
  afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
  afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);

  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
  m_logo = 0;
  //{{AFX_DATA_INIT(CAboutDlg)
  m_version = _T("");
  //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CAboutDlg)
  DDX_Control(pDX, IDC_THIRDPARTY, m_thirdParty);
  DDX_Text(pDX, IDC_ABOUT_STRING_BOX, m_version);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
//{{AFX_MSG_MAP(CAboutDlg)
ON_WM_CTLCOLOR()
ON_WM_DRAWITEM()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTerrainViewerApp commands

void CTerrainViewerApp::OnAppAbout()
{
  CAboutDlg aboutDlg;
  aboutDlg.DoModal();
}

void CTerrainViewerApp::OnHelp() 
{
// HTML help is not supported in Visual C++ 6
#if _MSC_VER > 1200
  ::HtmlHelp(m_pMainWnd->m_hWnd, (LPCTSTR)TSLUTF8Decoder(m_helpPath), HH_DISPLAY_TOC, 0 );		
#endif
}

void CTerrainViewerApp::OnHelpContactEnvitia() 
{
  ShellExecute( 
    NULL,
    _T("open"),
    _T("http://www.envitia.com/support"),
    NULL,
    NULL,
    0);	
}

BOOL CAboutDlg::OnInitDialog() 
{
  CDialog::OnInitDialog();

  m_logo = (HBITMAP)LoadImage( GetModuleHandle(0), MAKEINTRESOURCE(IDB_ABOUT_BOX_IMG), IMAGE_BITMAP, 
                              0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE );
  memset(&m_ct, 0, sizeof(RGBQUAD));
  COLORREF bg = GetSysColor(COLOR_BTNFACE);
  m_ct.rgbBlue = GetBValue(bg); m_ct.rgbGreen = GetGValue(bg); m_ct.rgbRed = GetRValue(bg);
  
  TCHAR version[256];
  time_t ltime ;
  struct tm * today ;
  
  time( &ltime ) ;
  today = localtime( &ltime ) ;
  
  const char* vinfo = TSLUtilityFunctions::getMapLinkVersion( 0, 0 );
  
  if ( vinfo && vinfo[0] )
  {
    CString localString;
    localString.LoadString( IDS_ABOUT_STRING_BOX );
    _sntprintf( version, sizeof( version ), (LPCTSTR)localString, (LPCTSTR)TSLUTF8Decoder(vinfo), today->tm_year + 1900 );
  }
  else
  {
    _sntprintf( version, sizeof( version ), _T("MapLink 8.0 Terrain Viewer\nCopyright © 1991-%d Envitia Group PLC."), today->tm_year + 1900 );
  }
  
  m_version = version;
  UpdateData( FALSE );
  
  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
  
  HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
  
  // Grey out the LeadTools text
  if ( nCtlColor == CTLCOLOR_STATIC && m_thirdParty.m_hWnd == pWnd->m_hWnd )
  {
    pDC->SetTextColor( GetSysColor(COLOR_BTNSHADOW) ) ;
  }
  
  return hbr;
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


