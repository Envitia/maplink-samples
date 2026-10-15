
// EarthSample.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "EarthSample.h"
#include "MainFrm.h"

#include "EarthSampleDoc.h"
#include "EarthSampleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEarthSampleApp

BEGIN_MESSAGE_MAP(CEarthSampleApp, CWinApp)
  ON_COMMAND(ID_APP_ABOUT, &CEarthSampleApp::OnAppAbout)
  // Standard file based document commands
  ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()

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
    m_isLocalMaplHome(false),
    m_next(Next_None),
    m_maplHome(""),
    m_failed(false)
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
  if (bFlag)
  {
    if (0 == _tcsicmp(pszParam, _T("help")))
    {
      m_next = Next_None;
      ::MessageBox(0, _T("Help:\n")
        _T("  app /home path_to_install\t(The directory containing the config directory)\n")
        , _T("Help"), MB_OK);
      m_failed = true;
    }
    else if (0 == _tcsicmp(pszParam, _T("home")))
    {
      m_isLocalMaplHome = true;
      m_next = Next_MaplHome;
    }
    else
    {
      CCommandLineInfo::ParseParam(pszParam, bFlag, bLast);
      m_next = Next_Default;
    }
  }
  else
  {
    switch (m_next)
    {
    case Next_MaplHome:
    {
      TSLUTF8Encoder encode(pszParam);
      m_maplHome = encode;
      if (m_maplHome == ".")
      {
        wchar_t* directory = _wgetcwd(NULL, 0);
        TSLUTF8Encoder dirEncode(directory);
        m_maplHome = dirEncode;
        m_maplHome += "/../";
        free(directory);
      }
      if (!m_maplHome.empty())
        TSLUtilityFunctions::setMapLinkHome(m_maplHome.c_str(), true);
    }
    m_next = Next_None;
    break;

    case Next_Default:
      CCommandLineInfo::ParseParam(pszParam, bFlag, bLast);
      m_next = Next_None;
      break;

    default:
      CCommandLineInfo::ParseParam(pszParam, bFlag, bLast);
      break;
    }
  }
}

// CEarthSampleApp construction

CEarthSampleApp::CEarthSampleApp()
{
  // TODO: add construction code here,
  // Place all significant initialization in InitInstance
}

// The one and only CEarthSampleApp object

CEarthSampleApp theApp;


// CEarthSampleApp initialization

BOOL CEarthSampleApp::InitInstance()
{
  // InitCommonControlsEx() is required on Windows XP if an application
  // manifest specifies use of ComCtl32.dll version 6 or later to enable
  // visual styles.  Otherwise, any window creation will fail.
  INITCOMMONCONTROLSEX InitCtrls;
  InitCtrls.dwSize = sizeof(InitCtrls);
  // Set this to include all the common control classes you want to use
  // in your application.
  InitCtrls.dwICC = ICC_WIN95_CLASSES;
  InitCommonControlsEx(&InitCtrls);

  CWinApp::InitInstance();

  EnableTaskbarInteraction(FALSE);

  // AfxInitRichEdit2() is required to use RichEdit control	
  // AfxInitRichEdit2();

  // Standard initialization
  // If you are not using these features and wish to reduce the size
  // of your final executable, you should remove from the following
  // the specific initialization routines you do not need

  // Change the registry key under which our settings are stored
  SetRegistryKey(TSL_MAPLINK_REGISTRY_STRING);
  LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)

  // Register the application's document templates.  Document templates
  //  serve as the connection between documents, frame windows and views
  CSingleDocTemplate* pDocTemplate;
  pDocTemplate = new CSingleDocTemplate(
    IDR_MAINFRAME,
    RUNTIME_CLASS(CEarthSampleDoc),
    RUNTIME_CLASS(CMainFrame),       // main SDI frame window
    RUNTIME_CLASS(CEarthSampleView));
  if (!pDocTemplate)
    return FALSE;
  m_pDocTemplate = pDocTemplate;
  AddDocTemplate(pDocTemplate);


  // Parse command line for standard shell commands, DDE, file open
  CCommandParser cmdInfo;
  ParseCommandLine(cmdInfo);

  // Initialise the MapLink runtime
  // Load the standard configuration
  bool initialisationSucceeded = false;
  TSLThreadedErrorStack::clear();
  // Initialise the drawing surface data files.
  // This call uses the TSLUtilityFunctions::getMapLinkHome method to determine
  // where MapLink is currently installed.  It then proceeds to load the
  // following files : tsllinestyles.dat, tslfillstyles.dat, tslfonts.dat, tslsymbols.dat, tslcolours.dat,
  // and tsltransforms.dat
  // When deploying your application, pass in a full path to the directory containing
  // these files.
  initialisationSucceeded = TSLDrawingSurface::loadStandardConfig() && TSLCoordinateSystem::loadCoordinateSystems();
  if (!initialisationSucceeded) {
    // Read more information on the errors from the stack
    TSLSimpleString msg("");
    TSLThreadedErrorStack::errorString(msg, "MapLink Initialisation Failed: \n");
    AfxMessageBox((LPCTSTR)TSLUTF8Decoder(msg), MB_OK);
    exit(1);
  }

  // Dispatch commands specified on the command line.  Will return FALSE if
  // app was launched with /RegServer, /Register, /Unregserver or /Unregister.
  if (!ProcessShellCommand(cmdInfo))
    return FALSE;

  // The one and only window has been initialized, so show and update it
  m_pMainWnd->ShowWindow(SW_SHOW);
  m_pMainWnd->UpdateWindow();

  return TRUE;
}

// CEarthSampleApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
  CAboutDlg();

  // Dialog Data
#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_ABOUTBOX };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
  DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CEarthSampleApp::OnAppAbout()
{
  CAboutDlg aboutDlg;
  aboutDlg.DoModal();
}
