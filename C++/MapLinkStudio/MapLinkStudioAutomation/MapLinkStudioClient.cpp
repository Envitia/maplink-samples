// MapLinkStudioClient.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MapLinkStudioClient.h"

#include "MainFrm.h"
#include "MapLinkStudioClientDoc.h"
#include "MapLinkStudioClientView.h"
#include <initguid.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
  string const& maplHome() const
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

/////////////////////////////////////////////////////////////////////////////
// CMapLinkStudioClientApp



BEGIN_MESSAGE_MAP(CMapLinkStudioClientApp, CWinApp)
  //{{AFX_MSG_MAP(CMapLinkStudioClientApp)
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
// CMapLinkStudioClientApp construction

CMapLinkStudioClientApp::CMapLinkStudioClientApp()
{
  m_maplinkHome = "";
  m_maplinkIniFile = "";
  m_projectFile = "";
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMapLinkStudioClientApp object

CMapLinkStudioClientApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMapLinkStudioClientApp initialization

BOOL CMapLinkStudioClientApp::InitInstance()
{
  // Standard initialization
  // If you are not using these features and wish to reduce the size
  //  of your final executable, you should remove from the following
  //  the specific initialization routines you do not need.

  // InitCommonControls() is required on Windows XP if an application 
  // manifest specifies use of ComCtl32.dll version 6 or later to enable 
  // visual styles. Otherwise, any window creation will fail. 
  InitCommonControls() ;
  
  if (!AfxOleInit())
  {
	AfxMessageBox(_T("Can't initialise OLE"));
	return FALSE;
  }

  if (!InitATL())
    return FALSE;

  AfxEnableControlContainer();
	_Module.RegisterClassObjects(CLSCTX_LOCAL_SERVER, 
		REGCLS_MULTIPLEUSE);

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

  // set maplink home directory and ini file
  m_maplinkHome = TSLUtilityFunctions::getMapLinkHome();
  m_maplinkIniFile = m_maplinkHome + "\\config\\mapl.ini";

  // Clear the error stack so that we can get the errors that occurred here.
  TSLErrorStack::clear( ) ;

  // Initialise the drawing surface data files.
  TSLDrawingSurface::loadStandardConfig( );

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
    RUNTIME_CLASS(CMapLinkStudioClientDoc),
    RUNTIME_CLASS(CMainFrame),       // main SDI frame window
    RUNTIME_CLASS(CMapLinkStudioClientView));
  AddDocTemplate(pDocTemplate);
  

  if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
    return TRUE;

  // Dispatch commands specified on the command line
  if (!ProcessShellCommand(cmdInfo))
    return FALSE;

  if (m_maplinkHome.size())
  {  
    m_pMainWnd->UpdateWindow();  
    // Enable drag/drop open
    m_pMainWnd->DragAcceptFiles();
  }
  else
  { 
    AfxMessageBox(_T("Failed to find MapLink .ini file"), MB_ICONERROR);
    return FALSE;
  }

  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMapLinkStudioClientApp commands

int CMapLinkStudioClientApp::ExitInstance() 
{
	if (m_bATLInited)
	{
		_Module.RevokeClassObjects();
		_Module.Term();
	
	}


  return CWinApp::ExitInstance();
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

// App command to run the dialog
void CMapLinkStudioClientApp::OnAppAbout()
{
  CAboutDlg aboutDlg;
  aboutDlg.DoModal();
}


	
CMapLinkStudioClientModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()

LONG CMapLinkStudioClientModule::Unlock()
{
	AfxOleUnlockApp();
	return 0;
}

LONG CMapLinkStudioClientModule::Lock()
{
	AfxOleLockApp();
	return 1;
}


BOOL CMapLinkStudioClientApp::InitATL()
{
	m_bATLInited = TRUE;

	_Module.Init(ObjectMap, AfxGetInstanceHandle());
	_Module.dwThreadID = GetCurrentThreadId();

	return TRUE;

}

void CMapLinkStudioClientApp::close()
{
  m_pMainWnd->PostMessage(WM_CLOSE);
}
