// TestSetup.cpp : implementation file
//
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "../stdafx.h"
#include "../S63WorkFlowSample.h"
#include "TestSetup.h"


// TestSetup dialog

IMPLEMENT_DYNAMIC(TestSetup, CDialog)

int CALLBACK SetupBrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData)
{
  TCHAR szDir[MAX_PATH];

  TestSetup * dlg = (TestSetup*)pData;

  switch ( uMsg ) 
  {
  case BFFM_INITIALIZED: 
    // Initial directory is set here.
    _tcscpy( szDir,  dlg->dataStore()  );
    {
      // WParam is TRUE since you are passing a path.
      // It would be FALSE if you were passing a pidl.
      SendMessage( hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)szDir );
    }
    break;

  case BFFM_SELCHANGED: 
    // Set the status window to the currently selected path.
    if( SHGetPathFromIDList( (LPITEMIDLIST)lp, szDir ) ) 
    {
      SendMessage( hwnd, BFFM_SETSTATUSTEXT, 0, (LPARAM)szDir );
    }
    break;

  default:
    break;
  }

  return 0;
}
static int CALLBACK SetSelProc (HWND hWnd,
                         UINT uMsg,
                         LPARAM lParam,
                         LPARAM lpData)
{
   if (uMsg==BFFM_INITIALIZED)
   {
      ::SendMessage(hWnd, BFFM_SETSELECTION, TRUE, lpData );
   }
   return 0;
}

string TestSetup::m_initialFolder;

TestSetup::TestSetup(CWnd* pParent /*=NULL*/)
	: CDialog(TestSetup::IDD, pParent)
        , m_configurationTime(0)
        , m_logFile()
        , m_dataLocation()
        , m_configurationDir()
        , m_resultsDir()
        , m_testDataInput()
        , m_certificateInput()
        , m_configurationInput()
        , m_resultsInput()
        , m_expiryDateInput()
{

}

TestSetup::~TestSetup()
{
}


BOOL TestSetup::OnInitDialog()
{
  CDialog::OnInitDialog();

  if ( m_configurationTime )
  {   
    m_expiryDateInput.SetTime(m_configurationTime);
  }
  else
  {
    const CTime defaultTime = CTime(2004, 8, 24, 12, 0, 0);
    m_expiryDateInput.SetTime(&defaultTime);

    m_configurationTime = defaultTime.GetTime();
  }

  return true;
}

void TestSetup::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);

  DDX_Text (pDX, IDC_DATA_LOCATION, m_dataLocation );
  DDX_Text (pDX, IDC_CONFIGURATION_DIR, m_configurationDir );
  DDX_Text (pDX, IDC_RESULTS_DIR, m_resultsDir );

  DDX_Control(pDX, IDC_DATA_LOCATION, m_testDataInput);
  DDX_Control(pDX, IDC_EXPIRY_DATE, m_expiryDateInput);
  DDX_Control(pDX, IDC_CONFIGURATION_DIR, m_configurationInput);
  DDX_Control(pDX, IDC_RESULTS_DIR, m_resultsInput);
}


BEGIN_MESSAGE_MAP(TestSetup, CDialog)
  ON_BN_CLICKED(IDC_DATA_LOCATION_BROWSE, &TestSetup::OnBnClickedDataLocationBrowse)
  ON_BN_CLICKED(IDC_SA_CERTIFICATE_BROWSE, &TestSetup::OnBnClickedSaCertificateBrowse)
  ON_BN_CLICKED(IDC_CONFIGURATION_BROWSE, &TestSetup::OnBnClickedConfigurationBrowse)
  ON_BN_CLICKED(IDC_RESULTS_BROWSE, &TestSetup::OnBnClickedResultsBrowse)
  ON_NOTIFY(DTN_DATETIMECHANGE, IDC_EXPIRY_DATE, &TestSetup::OnDtnDatetimechangeExpiryDate)
END_MESSAGE_MAP()


// TestSetup message handlers

void TestSetup::OnBnClickedDataLocationBrowse()
{
  UpdateData( TRUE );

  // Open a directory dialog.
  TCHAR szDisplayName[TSL_MAX_PATH];
  TCHAR szInitialDir[TSL_MAX_PATH];
  szDisplayName[0] = szInitialDir[0] = '\0';

  BROWSEINFO bi;
  memset( (LPVOID)&bi, 0, sizeof( bi ) );

  szDisplayName[0] = '\0';

  bi.hwndOwner      = AfxGetMainWnd()->m_hWnd;
  bi.pidlRoot       = 0;
  bi.pszDisplayName = szDisplayName;
  bi.lpszTitle      =  _T("Select the location of the data store.") ;
  bi.ulFlags        = BIF_RETURNONLYFSDIRS | BIF_USENEWUI | BIF_STATUSTEXT | BIF_EDITBOX;
  bi.lpfn           = SetupBrowseCallbackProc;
  bi.lParam         = (LPARAM)this;
  if (!m_initialFolder.empty())
  {
    // Set the callback Fn pointer to my function:
    bi.lpfn   = SetSelProc;
    // Set the lParam to point at my buffer containing the folder:
    bi.lParam = (LPARAM)(m_initialFolder.c_str());
  }
  else
  {
    bi.lpfn   = NULL;
    bi.lParam = 0;
  }
  const LPITEMIDLIST pIIL = ::SHBrowseForFolder( &bi );

  if ( !pIIL )
  {
    // The user hit Cancel!
    return;
  }

  const BOOL bRet = ::SHGetPathFromIDList( pIIL, szInitialDir );
  if ( bRet )
  {
    LPMALLOC pMalloc;
    HRESULT hr = SHGetMalloc( &pMalloc );
    if ( szInitialDir[0] != '\0' )
    {
      m_dataLocation = szInitialDir;

      UpdateData( FALSE );
    }

    pMalloc->Free( pIIL );
    pMalloc->Release();
  }

  if ( szInitialDir[0] != '\0' )
  {
    m_initialFolder = TSLUTF8Encoder(szInitialDir);
  }
}

void TestSetup::OnBnClickedSaCertificateBrowse()
{
  CFileDialog FileDlg(true, _T(".crt"), NULL,  OFN_NOCHANGEDIR, _T("Certificate Files (*.crt)|*.crt|Certificate Files (*.cer)|*.cer|All Files (*.*)|*.*||"));
  FileDlg.m_pOFN->lpstrTitle = _T("Select Scheme Administrator certificate");

  if( FileDlg.DoModal() != IDOK )
  {
    return;
  }    

  //set the unlock key in it's current state.
  m_testDataInput.GetWindowText(m_dataLocation);

  UpdateData(false);
}
  
CString& TestSetup::dataStore()
{
  return m_dataLocation;
}

void TestSetup::OnDtnDatetimechangeExpiryDate(NMHDR *pNMHDR, LRESULT *pResult)
{
  CTime time;
  m_expiryDateInput.GetTime(time);
  m_configurationTime = time.GetTime();

  *pResult = 0;
}

void TestSetup::OnBnClickedConfigurationBrowse()
{
  UpdateData( TRUE );

  // Open a directory dialog.
  TCHAR szDisplayName[TSL_MAX_PATH];
  TCHAR szInitialDir[TSL_MAX_PATH];
  szDisplayName[0] = szInitialDir[0] = '\0';

  BROWSEINFO bi;
  memset( (LPVOID)&bi, 0, sizeof( bi ) );

  szDisplayName[0] = '\0';

  bi.hwndOwner      = AfxGetMainWnd()->m_hWnd;
  bi.pidlRoot       = 0;
  bi.pszDisplayName = szDisplayName;
  bi.lpszTitle      =  _T("Select the location of the configuration directory.") ;
  bi.ulFlags        = BIF_RETURNONLYFSDIRS | BIF_USENEWUI | BIF_STATUSTEXT | BIF_EDITBOX;
  bi.lpfn           = SetupBrowseCallbackProc;
  bi.lParam         = (LPARAM)this;
  if (!m_initialFolder.empty())
  {
    // Set the callback Fn pointer to my function:
    bi.lpfn   = SetSelProc;
    // Set the lParam to point at my buffer containing the folder:
    bi.lParam = (LPARAM)(m_initialFolder.c_str());
  }
  else
  {
    bi.lpfn   = NULL;
    bi.lParam = 0;
  }
  const LPITEMIDLIST pIIL = ::SHBrowseForFolder( &bi );

  if ( !pIIL )
  {
    // The user hit Cancel!
    return;
  }

  const BOOL bRet = ::SHGetPathFromIDList( pIIL, szInitialDir );
  if ( bRet )
  {
    LPMALLOC pMalloc;
    HRESULT hr = SHGetMalloc( &pMalloc );
    if ( szInitialDir[0] != '\0' )
    {
      m_configurationDir = szInitialDir;

      UpdateData( FALSE );
    }

    pMalloc->Free( pIIL );
    pMalloc->Release();
  }
  if ( szInitialDir[0] != '\0' )
  {
    m_initialFolder = TSLUTF8Encoder(szInitialDir);
  }
}
void TestSetup::OnBnClickedResultsBrowse()
{
  UpdateData( TRUE );

  // Open a directory dialog.
  TCHAR szDisplayName[TSL_MAX_PATH];
  TCHAR szInitialDir[TSL_MAX_PATH];
  szDisplayName[0] = szInitialDir[0] = '\0';

  BROWSEINFO bi;
  memset( (LPVOID)&bi, 0, sizeof( bi ) );

  szDisplayName[0] = '\0';

  bi.hwndOwner      = AfxGetMainWnd()->m_hWnd;
  bi.pidlRoot       = 0;
  bi.pszDisplayName = szDisplayName;
  bi.lpszTitle      =  _T("Select the location of the results directory.") ;
  bi.ulFlags        = BIF_RETURNONLYFSDIRS | BIF_USENEWUI | BIF_STATUSTEXT | BIF_EDITBOX;
  bi.lpfn           = SetupBrowseCallbackProc;
  bi.lParam         = (LPARAM)this;
  if (!m_initialFolder.empty())
  {
    // Set the callback Fn pointer to my function:
    bi.lpfn   = SetSelProc;
    // Set the lParam to point at my buffer containing the folder:
    bi.lParam = (LPARAM)(m_initialFolder.c_str());
  }
  else
  {
    bi.lpfn   = NULL;
    bi.lParam = 0;
  }
  const LPITEMIDLIST pIIL = ::SHBrowseForFolder( &bi );

  if ( !pIIL )
  {
    // The user hit Cancel!
    return;
  }

  const BOOL bRet = ::SHGetPathFromIDList( pIIL, szInitialDir );
  if ( bRet )
  {
    LPMALLOC pMalloc;
    HRESULT hr = SHGetMalloc( &pMalloc );
    if ( szInitialDir[0] != '\0' )
    {
      m_resultsDir = szInitialDir;

      UpdateData( FALSE );
    }

    pMalloc->Free( pIIL );
    pMalloc->Release();
  }
  if ( szInitialDir[0] != '\0' )
  {
    m_initialFolder = TSLUTF8Encoder(szInitialDir);
  }
}
