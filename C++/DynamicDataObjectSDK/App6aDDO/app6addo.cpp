// ddo.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "app6addo.h"

#include "MainFrm.h"
#include "app6addodoc.h"
#include "app6addoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAPP6ADDOApp

BEGIN_MESSAGE_MAP(CAPP6ADDOApp, CWinApp)
  //{{AFX_MSG_MAP(CAPP6ADDOApp)
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
// CAPP6ADDOApp construction

CAPP6ADDOApp::CAPP6ADDOApp()
{
  // TODO: add construction code here,
  // Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAPP6ADDOApp object

CAPP6ADDOApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CAPP6ADDOApp initialization

BOOL CAPP6ADDOApp::InitInstance()
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
  SetRegistryKey(_T("Local AppWizard-Generated Applications"));
  
  LoadStdProfileSettings();  // Load standard INI file options (including MRU)
  
  // Clear the error stack so that we can get the errors that occurred here.
  TSLErrorStack::clear( ) ;

  // Initialise the drawing surface data files.
  TSLDrawingSurface::loadStandardConfig( );

  // Need to load the APP6a symbols file
  const char * maplinkHome = TSLUtilityFunctions::getMapLinkHome() ;
  if ( maplinkHome )
  {
    // Get the specific symbols.dat file
    std::string symbolsFile = maplinkHome ;
    symbolsFile += "\\config\\tslsymbolsAPP6A.dat" ;

    TSLDrawingSurface::setupSymbols( symbolsFile.c_str() ) ;
  }

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
    RUNTIME_CLASS(CAPP6ADDODoc),
    RUNTIME_CLASS(CMainFrame),       // main SDI frame window
    RUNTIME_CLASS(CAPP6ADDOView));
  AddDocTemplate(pDocTemplate);
  
  // Enable DDE Execute open
  //EnableShellOpen();
  //RegisterShellFileTypes(TRUE);
  
  //Above two generated lines were commented to stop this sample associating 
  //itself with Envitia maps everytime it is executed
  //CR5335 NT 09/01/2005

  // Parse command line for standard shell commands, DDE, file open
  CCommandLineInfo cmdInfo;
  ParseCommandLine(cmdInfo);
  
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

// App command to run the dialog
void CAPP6ADDOApp::OnAppAbout()
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
// CAPP6ADDOApp commands
