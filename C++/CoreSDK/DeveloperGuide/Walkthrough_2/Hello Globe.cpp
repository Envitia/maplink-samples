// Hello Globe.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Hello Globe.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "Hello GlobeDoc.h"
#include "Hello GlobeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHelloGlobeApp

BEGIN_MESSAGE_MAP(CHelloGlobeApp, CWinApp)
	//{{AFX_MSG_MAP(CHelloGlobeApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHelloGlobeApp construction

CHelloGlobeApp::CHelloGlobeApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CHelloGlobeApp object

CHelloGlobeApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CHelloGlobeApp initialization

BOOL CHelloGlobeApp::InitInstance()
{
	// Standard initialization

  // InitCommonControls() is required on Windows XP if an application 
  // manifest specifies use of ComCtl32.dll version 6 or later to enable 
  // visual styles. Otherwise, any window creation will fail. 
  InitCommonControls() ;

  // Change the registry key under which our settings are stored.
  SetRegistryKey(_T("Local AppWizard-Generated Applications"));

  LoadStdProfileSettings();  // Load standard INI file options (including MRU)

  TSLErrorStack::clear() ;
  const char * configDirPath = 0 ;
  TSLDrawingSurface::loadStandardConfig( configDirPath ) ;

  const char * msg = TSLErrorStack::errorString( "Initialisation Errors : \n" ) ;
  if ( msg ) 
  {
    AfxMessageBox( (LPCTSTR)TSLUTF8Decoder(msg), MB_OK ) ;
    exit( 0 ) ;
  }

	// Register document templates

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_HELLOGTYPE,
		RUNTIME_CLASS(CHelloGlobeDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CHelloGlobeView));
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
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
void CHelloGlobeApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CHelloGlobeApp message handlers


int CHelloGlobeApp::ExitInstance() 
{
  TSLDrawingSurface::cleanup() ;
  
  return CWinApp::ExitInstance();
}
