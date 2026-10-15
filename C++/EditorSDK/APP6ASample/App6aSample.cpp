// APP6ASample.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "APP6ASample.h"
#include "APP6ASampleDoc.h"
#include "APP6ASampleView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//This class is a specialised command line parser.
//
//It is called by MFC for each argument on the command line.  
//This will trap the specification of the initialisation 
//file. using '-I<path>LandLinkSample.ini
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
  std::string const& maplHome() const
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
  std::string    m_maplHome;
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
    else if ( 0 == _tcsicmp( pszParam, _T("I") ) )
    {
      m_next = Next_IniFile;
    }
    else if ( 0 == _tcsicmp( pszParam, _T("home") ) ||
              pszParam[0] == _T('m') || pszParam[0] == _T('M'))
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
        // Specify initialisation file
        CString iniFile( pszParam ) ;

        // Obtain the full path to the ini file
        // or windows on writing updated entries will store it in
        // Windows or WINT directory depending on OS!
        
        LPTSTR path = _tfullpath(NULL, iniFile, 0);

        TSLProfileHelper::setDefaultFilename( TSLUTF8Encoder(path) ) ;

        free( path );
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
// CAPP6ASampleApp

BEGIN_MESSAGE_MAP(CAPP6ASampleApp, CWinApp)
  //{{AFX_MSG_MAP(CAPP6ASampleApp)
    ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
  // Standard file based document commands
    ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
    ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
  // Standard print setup command
  ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAPP6ASampleApp construction

CAPP6ASampleApp::CAPP6ASampleApp() : m_rootDirectory("")
{
  // Place all significant initialization in InitInstance
#if _MSC_VER > 1200
  EnableHtmlHelp();
#endif
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAPP6ASampleApp object

CAPP6ASampleApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CAPP6ASampleApp initialization

CAPP6ASampleApp * CAPP6ASampleApp::getAPP ()
{
  return &theApp ;
}

BOOL CAPP6ASampleApp::InitInstance()
{
#ifdef _DEBUG
  AfxEnableMemoryTracking( FALSE ) ;
#endif

  AfxEnableControlContainer();

  // Standard initialization
  // If you are not using these features and wish to reduce the size
  //  of your final executable, you should remove from the following
  //  the specific initialization routines you do not need.

  // InitCommonControls() is required on Windows XP if an application 
  // manifest specifies use of ComCtl32.dll version 6 or later to enable 
  // visual styles. Otherwise, any window creation will fail. 
  InitCommonControls() ;

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
  
  TSLProfileHelper::setDefaultFilename( "" ) ;

  // Parse command line for standard shell commands, DDE, file open
  // Also look for '-Iconfigfilename'
  CCommandParser cmdInfo;
  ParseCommandLine(cmdInfo);

  if( cmdInfo.failed() )
  {
    return false;
  }

  TSLSimpleString iniFile;
  TSLProfileHelper::getDefaultFilename( iniFile ) ;

  string symbolsFile ;

  if ( iniFile.empty() )
  {
    // No ini file, use the default MapLink one
    const char * maplinkHome = TSLUtilityFunctions::getMapLinkHome() ;
    if ( maplinkHome )
    {
      string filename = maplinkHome ;
      filename += "\\samples\\nt\\APP6ASample\\APP6ASample.ini" ;

      FILE * fp = fopen( filename.c_str(), "rb" ) ;

      if ( fp )
      {
        fclose( fp ) ;
        TSLProfileHelper::setDefaultFilename( filename.c_str() ) ;
      }

      // Get the specific symbols.dat file
      symbolsFile = maplinkHome ;
      symbolsFile += "\\config\\tslsymbolsAPP6A.dat" ;
    }

    TSLProfileHelper::getDefaultFilename( iniFile ) ;

    if ( iniFile.empty() )
    {
      AfxMessageBox( _T("No initialisation file specified on the command line eg. '-IAPP6ASample.ini'\n")
		     _T("Add this to the shortcut parameters or Settings->Debug->Program arguments"), MB_ICONERROR ) ;
      exit( 1 ) ;
    }
  }

  CFileStatus status;
  if ( !CFile::GetStatus( (LPCTSTR)TSLUTF8Decoder(iniFile), status ) )
  {
    AfxMessageBox( _T("Can't find the initialisation file specified on the command line"), MB_ICONERROR ) ;
    exit( 1 ) ;
  }

  // Now get the ini file directory and store it in 'm_rootDirectory'
  WCHAR drive[_MAX_DRIVE];
  WCHAR dir[TSL_MAX_WPATH];
  WCHAR fname[_MAX_FNAME];
  WCHAR ext[_MAX_EXT];
  WCHAR path[TSL_MAX_WPATH] ;

  _wsplitpath( TSLUTF8Decoder(iniFile), drive, dir, fname, ext );
  _wmakepath( path, drive, dir, 0, 0 ) ;
  
  m_rootDirectory = TSLUTF8Encoder(path) ;

  // Clear the error stack so that we can get the errors that occurred here.
  TSLErrorStack::clear( ) ;

  TSLDrawingSurface::loadStandardConfig() ;

  // Now load the APP6A symbols file - overriding the standard one
  if ( symbolsFile.size() )
    TSLDrawingSurface::setupSymbols( symbolsFile.c_str() ) ;

  // Check for any errors that have occurred, and display them
  const char * msg = TSLErrorStack::errorString() ;

  if ( msg )
  {
    // If we have any errors during initialisation, display the message
    // and exit.
    AfxMessageBox( (LPCTSTR)TSLUTF8Decoder(msg), MB_ICONERROR ) ;
    exit( 0 ) ;
  }

  // Register the application's document templates.  Document templates
  //  serve as the connection between documents, frame windows and views.
  
  CSingleDocTemplate* pDocTemplate;
  pDocTemplate = new CSingleDocTemplate(
    IDR_MAINFRAME,
    RUNTIME_CLASS(CAPP6ASampleDoc),
    RUNTIME_CLASS(CMainFrame),       // main SDI frame window
    RUNTIME_CLASS(CAPP6ASampleView));
  AddDocTemplate(pDocTemplate);

  // Enable DDE Execute open
  EnableShellOpen();
  
  // Dispatch commands specified on the command line
  if (!ProcessShellCommand(cmdInfo))
    return FALSE;
  
  // The one and only window has been initialized, so show and update it.
  m_pMainWnd->ShowWindow(SW_SHOW);
  m_pMainWnd->UpdateWindow();
  
  // Enable drag/drop open
  m_pMainWnd->DragAcceptFiles();
    
  return TRUE;
}

//CAboutDlg dialog used for LandLink Sample About.
//
//Automatically generate by DevStudio Class Wizard.

class CAboutDlg : public CDialog
{
public:
	//Dialog box constructor.
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
	//Dialog data exchange.
	//
	//Automatically generate by DevStudio Class Wizard.
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

// App command to run the dialog
void CAPP6ASampleApp::OnAppAbout()
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
// CAPP6ASampleApp commands

const char * CAPP6ASampleApp::rootDirectory()
{
  return m_rootDirectory.c_str() ;
}

// HTML help is not supported on Visual C++ 6
#if _MSC_VER > 1200
void CAPP6ASampleApp::HtmlHelp ( DWORD dwData, UINT nCmd  )
{
  //Disabled
}
#endif // _MSC_VER > 1200

const std::string CAPP6ASampleApp::resolveConfigPath(const char * path)
{
  wchar_t drive[_MAX_DRIVE];
  wchar_t dir[TSL_MAX_WPATH];
  wchar_t fname[_MAX_FNAME];
  wchar_t ext[_MAX_EXT];

  _wsplitpath( TSLUTF8Decoder(path), drive, dir, fname, ext );

  if ( drive[0] || dir[0] == L'\\' )
  {
    // This path is absolute, just return it
    return path ;
  }
  
  // Get the root directory
  wchar_t rootDrive[_MAX_DRIVE];
  wchar_t rootDir[TSL_MAX_WPATH];
  wchar_t rootFname[_MAX_FNAME];
  wchar_t rootExt[_MAX_EXT];

  _wsplitpath( TSLUTF8Decoder(rootDirectory()), rootDrive, rootDir, rootFname, rootExt );

 
  // Now combine the subdirectories

  std::wstring combinedDir;
  if ( rootDir[0] && dir[0] )
  {
    combinedDir = rootDir;
    combinedDir += dir;
  }
  else if ( rootDir[0] )
  {
    combinedDir = rootDir;
  }
  else
  {
    combinedDir = dir;
  }

  wchar_t fullpath[TSL_MAX_WPATH];
  // Make the full path and return it.
  _wmakepath( fullpath, rootDrive, combinedDir.c_str(), fname, ext ) ;
  std::string fullpathUtf8 = TSLUTF8Encoder( fullpath );
  return fullpathUtf8;
}

int CAPP6ASampleApp::ExitInstance() 
{
  TSLDrawingSurface::cleanup() ;
  return CWinApp::ExitInstance();
}
