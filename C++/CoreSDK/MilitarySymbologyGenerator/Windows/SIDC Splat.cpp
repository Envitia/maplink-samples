
// SIDC Splat.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "SIDC Splat.h"
#include "MainFrm.h"
#include <MapLink.h>

#include "ChildFrm.h"
#include "SIDC SplatDoc.h"
#include "SIDC SplatView.h"

#include <filesystem>

BEGIN_MESSAGE_MAP(CSidcSplatApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CSidcSplatApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
END_MESSAGE_MAP()


CSidcSplatApp::CSidcSplatApp() noexcept
{
	m_bHiColorIcons = TRUE;


	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("SidcSplat.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CSidcSplatApp object

CSidcSplatApp theApp;


// CSidcSplatApp initialization

BOOL CSidcSplatApp::InitInstance()
{
	CWinAppEx::InitInstance();


	EnableTaskbarInteraction();

	// AfxInitRichEdit2() is required to use RichEdit control
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	std::atexit([]()
		{
			TSLDrawingSurface::cleanup();
		});

	TSLThreadedErrorStack::clear();
	TSLDrawingSurface::loadStandardConfig();
	TSLSimpleString msg("");
	bool anyErrors = TSLThreadedErrorStack::errorString(msg, "Initialisation Errors : \n");
	if (anyErrors)
	{
		AfxMessageBox((LPCTSTR)TSLUTF8Decoder(msg), MB_OK);
		exit(0);
	}


	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	AddDocTemplate(new CMultiDocTemplate (IDR_SidcSplatTYPE, RUNTIME_CLASS(CSidcSplatDoc), RUNTIME_CLASS(CChildFrame), RUNTIME_CLASS(CSidcSplatView)));

	// create main MDI Frame window
	auto pMainFrame = std::make_unique<CMainFrame>();
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame.release();

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// The main window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(m_nCmdShow);
	m_pMainWnd->UpdateWindow();

	auto syms = std::filesystem::path{ __FILE__ }.parent_path().parent_path().parent_path().parent_path().parent_path().parent_path() / "config/symbols/tslsymbolsosm.dat";
	assert(TSLDrawingSurface::setupSymbols(syms.u8string().c_str()));

	return TRUE;
}

// CSidcSplatApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

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

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CSidcSplatApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CSidcSplatApp customization load/save methods

void CSidcSplatApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CSidcSplatApp::LoadCustomState()
{
}

void CSidcSplatApp::SaveCustomState()
{
}

// CSidcSplatApp message handlers



