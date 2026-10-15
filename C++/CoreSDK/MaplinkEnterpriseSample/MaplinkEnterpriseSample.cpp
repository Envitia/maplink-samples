// MaplinkEnterpriseSample.cpp : Defines the class behaviors for the application.
//

#include "stdafx_tlt.h"
#include "MaplinkEnterpriseSample.h"

#include "MainFrm.h"
#include "MaplinkEnterpriseSampleDoc.h"
#include "MaplinkEnterpriseSampleView.h"
#include "RemoteOpenDlg.h"
#include "MyCommandLine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// statics
TSLFileLoader* CMaplinkEnterpriseSampleApp::m_loaderRemote = 0;
TSLFileLoader* CMaplinkEnterpriseSampleApp::m_loaderLocal = 0;
bool CMaplinkEnterpriseSampleApp::m_remote = true;
string CMaplinkEnterpriseSampleApp::m_cacheConfigFile;
TSLPersistentCacheData* CMaplinkEnterpriseSampleApp::m_persistentCacheData = 0 ;

/////////////////////////////////////////////////////////////////////////////
// CMaplinkEnterpriseSampleApp

BEGIN_MESSAGE_MAP(CMaplinkEnterpriseSampleApp, CWinApp)
  //{{AFX_MSG_MAP(CMaplinkEnterpriseSampleApp)
    ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_OPENMAPREMOTE, OnFileOpenRemote)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	//}}AFX_MSG_MAP
  // Standard file based document commands
    ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
  // Standard print setup command
  ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMaplinkEnterpriseSampleApp construction

CMaplinkEnterpriseSampleApp::CMaplinkEnterpriseSampleApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMaplinkEnterpriseSampleApp object

CMaplinkEnterpriseSampleApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMaplinkEnterpriseSampleApp initialization

BOOL CMaplinkEnterpriseSampleApp::InitInstance()
{
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

  // Parse command line for standard shell commands, DDE, file open
  MyCommandLine cmdInfo;
  ParseCommandLine(cmdInfo);

  if( cmdInfo.failed() )
  {
    return false;
  }

  m_cacheConfigFile = cmdInfo.persistentCacheIniFile();

  // Clear the error stack so that we can get the errors that occurred here.
  TSLErrorStack::clear( ) ;

  // Create loaders for the application to use
  // Use 2 as the remote loader shouldn't be
  // used to access local files
  m_loaderRemote = new TSLFileLoaderRemote;
  m_loaderLocal = new TSLFileLoaderMT;
  
  // add the loader to the drawing surface
  TSLDrawingSurface::addLoader( m_loaderRemote ) ;

  // Initialise the drawing surface data files.
  // If you want to load the configuration files remotely as well as the map, then pass
  // the location of the config directory into this call, e.g. "http://localhost/config".
  TSLDrawingSurface::loadStandardConfig( );

  // Check for any errors that have occurred, and display them
  const char * msg = TSLErrorStack::errorString( "Error loading standard configuration files: " ) ;

  if ( msg )
  {
    // If we have any errors during initialisation, display the message
    // and exit the app
    AfxMessageBox( (LPCTSTR)TSLUTF8Decoder(msg), MB_ICONERROR ) ;
    exit(0);
  }

  // Register the application's document templates.  Document templates
  //  serve as the connection between documents, frame windows and views.
  
  CSingleDocTemplate* pDocTemplate;
  pDocTemplate = new CSingleDocTemplate(
    IDR_MAINFRAME,
    RUNTIME_CLASS(CMaplinkEnterpriseSampleDoc),
    RUNTIME_CLASS(CMainFrame),       // main SDI frame window
    RUNTIME_CLASS(CMaplinkEnterpriseSampleView));
  AddDocTemplate(pDocTemplate);
  
  if (!ProcessShellCommand(cmdInfo))
    return FALSE;

  // The one and only window has been initialized, so show and update it.
  m_pMainWnd->ShowWindow(SW_SHOW);
  m_pMainWnd->UpdateWindow();
  
  // Enable drag/drop open
  m_pMainWnd->DragAcceptFiles();

  // Add persistent cache if a .ini filename was passed into the program 
  // using the command line -pcache parameter
  if ( !m_cacheConfigFile.empty() )
  {
    m_persistentCacheData = new TSLPersistentCacheData;
    m_persistentCacheData->loadFromIniFile( m_cacheConfigFile.c_str(), "persistent cache" ) ;
  }
  
  // Check if map to open passed on command line
  if ( !cmdInfo.m_strFileName.IsEmpty() )
  {
    OpenDocumentFile( cmdInfo.m_strFileName );
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
void CMaplinkEnterpriseSampleApp::OnAppAbout()
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
// CMaplinkEnterpriseSampleApp commands


void CMaplinkEnterpriseSampleApp::OnFileOpen() 
{
  // Have to do this ourselves because SDI applications can't cope with multiple extensions (.map, .mpc)

  CString newName, filter;

  filter.LoadString( IDS_FILESFILTER );

  CFileDialog dlg( TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, NULL );

  if ( dlg.DoModal() == IDOK )
  {
    m_remote = false;
    OpenDocumentFile( dlg.GetPathName() );
  }
}


void CMaplinkEnterpriseSampleApp::OnFileOpenRemote() 
{
  // This stuff was copied from CWinApp::OnFileOpen in MFC and modified,
  // because MFC can't cope with remote files

  CRemoteOpenDlg openerDlg;

  if ( openerDlg.DoModal() == IDOK )
  {
    CString szPath = openerDlg.m_Location;

    // find the right sort of document template for this sort of file
    POSITION pos = GetFirstDocTemplatePosition();
    CDocTemplate::Confidence bestMatch = CDocTemplate::noAttempt;
    CDocTemplate* pBestTemplate = NULL;
    CDocument* pOpenDocument = NULL;
    
    
    while (pos != NULL)
    {
      CDocTemplate* pTemplate = GetNextDocTemplate(pos);
      ASSERT_KINDOF(CDocTemplate, pTemplate);
      
      CDocTemplate::Confidence match;
      ASSERT(pOpenDocument == NULL);
      match = pTemplate->MatchDocType(szPath, pOpenDocument);
      if (match > bestMatch)
      {
	bestMatch = match;
	pBestTemplate = pTemplate;
      }
      if (match == CDocTemplate::yesAlreadyOpen)
	break;      // stop here
    }
    
    if (pOpenDocument != NULL)
    {
      POSITION pos = pOpenDocument->GetFirstViewPosition();
      if (pos != NULL)
      {
	CView* pView = pOpenDocument->GetNextView(pos); // get first one
	ASSERT_VALID(pView);
	CFrameWnd* pFrame = pView->GetParentFrame();
	if (pFrame != NULL)
	  pFrame->ActivateFrame();
	else
	  TRACE0("Error: Can not find a frame for document to activate.\n");
	CFrameWnd* pAppFrame;
	if (pFrame != (pAppFrame = (CFrameWnd*)AfxGetApp()->m_pMainWnd))
	{
	  ASSERT_KINDOF(CFrameWnd, pAppFrame);
	  pAppFrame->ActivateFrame();
	}
      }
      else
      {
	TRACE0("Error: Can not find a view for document to activate.\n");
      }
      return;
    }
    
    if (pBestTemplate == NULL)
    {
      AfxMessageBox(AFX_IDP_FAILED_TO_OPEN_DOC);
      return;
    }

    m_remote = true;
    pBestTemplate->OpenDocumentFile( szPath );
  }
}

int CMaplinkEnterpriseSampleApp::ExitInstance() 
{
  if ( m_persistentCacheData )
  {
    m_persistentCacheData->destroy();
    m_persistentCacheData = 0;
  }

  if ( m_loaderRemote )
  {
    m_loaderRemote->destroy();
    m_loaderRemote = 0;
  }

  if ( m_loaderLocal )
  {
    m_loaderLocal->destroy();
    m_loaderLocal = 0;
  }

  return CWinApp::ExitInstance();
}

