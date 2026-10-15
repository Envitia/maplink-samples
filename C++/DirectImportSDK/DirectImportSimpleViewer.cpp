// DirectImportSimpleViewer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "DirectImportSimpleViewer.h"

#include "MainFrm.h"
#include "DirectImportSimpleViewerDoc.h"
#include "DirectImportSimpleViewerView.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DirectImportSimpleViewerApp

//This class is a specialised command line parser.
//
//It is called by MFC for each argument on the command line.  
class CCommandParser : public CCommandLineInfo
{
private:
  enum NextValue { Next_None, Next_Default, Next_MaplHome };
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
    //This method parses each command line argument
    virtual void ParseParam(const TCHAR* pszParam, BOOL bFlag, BOOL bLast);

private:
  bool           m_isLocalMaplHome;
  NextValue      m_next;
  std::string    m_maplHome;
  bool           m_failed;
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
                      , _T("Help"), MB_OK);
      m_failed = true;
    }
    else if ( 0 == _tcsicmp( pszParam, _T("home") ) )
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

    default:
      CCommandLineInfo::ParseParam( pszParam, bFlag, bLast );
      break;
    }
  }
}

BEGIN_MESSAGE_MAP(DirectImportSimpleViewerApp, CWinApp)
  //{{AFX_MSG_MAP(DirectImportSimpleViewerApp)
    ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
  // NOTE - the ClassWizard will add and remove mapping macros here.
  //    DO NOT EDIT what you see in these blocks of generated code!
  //}}AFX_MSG_MAP
  // Standard file based document commands
    ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
    ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
  // Standard print setup command
  ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DirectImportSimpleViewerApp construction

DirectImportSimpleViewerApp::DirectImportSimpleViewerApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only DirectImportSimpleViewerApp object

DirectImportSimpleViewerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// DirectImportSimpleViewerApp initialization

BOOL DirectImportSimpleViewerApp::InitInstance()
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
  SetRegistryKey(_T("DirectImportSimpleViewer"));
  
  LoadStdProfileSettings();  // Load standard INI file options (including MRU)
  
  // Parse command line for standard shell commands, DDE, file open
  // Also look for '-Iconfigfilename'
  CCommandParser cmdInfo;
  ParseCommandLine(cmdInfo);

  if( cmdInfo.failed() )
  {
    return false;
  }

  // Clear the error stack so that we can get the errors that occurred here.
  TSLErrorStack::clear( ) ;

  // Initialise the drawing surface data files.
  // This call uses the TSLUtilityFunctions::getMapLinkHome method to determine
  // where MapLink is currently installed.  It then proceeds to load the
  // following files : tsllinestyles.dat, tslfillstyles.dat, tslfonts.dat, tslsymbols.dat
  // and tslcolours.dat
  // When deploying your application, pass in a full path to the directory containing
  // these files.
  TSLDrawingSurface::loadStandardConfig( );
  TSLCoordinateSystem::loadCoordinateSystems();

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
    RUNTIME_CLASS(DirectImportSimpleViewerDoc),
    RUNTIME_CLASS(CMainFrame),       // main SDI frame window
    RUNTIME_CLASS(DirectImportSimpleViewerView));
  AddDocTemplate(pDocTemplate);

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

/////////////////////////////////////////////////////////////////////////////
// DirectImportSimpleViewerApp commands

int DirectImportSimpleViewerApp::ExitInstance() 
{
  return CWinApp::ExitInstance();
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
  CAboutDlg();
  
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
		// No message handlers
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:

};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
  //{{AFX_DATA_INIT(CAboutDlg)
  //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CAboutDlg)
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
//{{AFX_MSG_MAP(CAboutDlg)
// No message handlers
//}}AFX_MSG_MAP

END_MESSAGE_MAP()

// App command to run the dialog
void DirectImportSimpleViewerApp::OnAppAbout()
{
  CAboutDlg aboutDlg;
  aboutDlg.DoModal();
}


