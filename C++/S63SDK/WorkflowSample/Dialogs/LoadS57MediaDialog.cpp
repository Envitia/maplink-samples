// LoadS57MediaDialog.cpp : implementation file
//
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "../stdafx.h"
#include "../S63WorkFlowSample.h"
#include "LoadS57MediaDialog.h"


// LoadS57MediaDialog dialog

static int CALLBACK S57BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData)
{
  TCHAR szDir[MAX_PATH];

  LoadS57MediaDialog * dlg = (LoadS57MediaDialog*)pData;

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
string LoadS57MediaDialog::m_initialFolder;

IMPLEMENT_DYNAMIC(LoadS57MediaDialog, CDialog)

LoadS57MediaDialog::LoadS57MediaDialog(CWnd* pParent /*=NULL*/)
	: CDialog(LoadS57MediaDialog::IDD, pParent)
        , m_dataStore()
        , m_catalogFile()
        , m_catalogInput()
        , m_storeInput()
{
  m_dataStore = (LPCTSTR)TSLUTF8Decoder(m_initialFolder);
}

LoadS57MediaDialog::~LoadS57MediaDialog()
{
}

void LoadS57MediaDialog::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);

  DDX_Text (pDX, IDC_STORE_LOCATION, m_dataStore );
  DDX_Text (pDX, IDC_CATALOG_FILE, m_catalogFile );

  DDX_Control(pDX, IDC_STORE_LOCATION, m_storeInput);
  DDX_Control(pDX, IDC_CATALOG_FILE, m_catalogInput);

  m_initialFolder = TSLUTF8Encoder(m_dataStore);
}


BEGIN_MESSAGE_MAP(LoadS57MediaDialog, CDialog)

  ON_BN_CLICKED(IDC_CATALOG_BROWSE, &LoadS57MediaDialog::OnBnClickedCatalogBrowse)
  ON_BN_CLICKED(IDC_STORE_BROWSE, &LoadS57MediaDialog::OnBnClickedStoreBrowse)
END_MESSAGE_MAP()

void LoadS57MediaDialog::OnBnClickedCatalogBrowse()
{
  CFileDialog FileDlg(true, _T("CATALOG.*;*.000"), NULL, OFN_NOCHANGEDIR, 
    _T("S57 Files (CATALOG.*;*.000)|CATALOG.*;*.000|S57 CATALOG Files (CATALOG.*)|CATALOG.*|S57 Base Cells (*.000)|*.000|All Files (*.*)|*.*||"));

  FileDlg.m_pOFN->lpstrTitle = _T("Open S57 Data file");

  if( FileDlg.DoModal() != IDOK )
  {
    return;
  }
    
  m_catalogFile = FileDlg.GetPathName();

  //set the unlock key in it's current state.
  m_storeInput.GetWindowText(m_dataStore);
  if (!m_dataStore.IsEmpty())
  {
    string file = TSLUTF8Encoder(m_dataStore);
    file += "/logfile.txt";
    logFile(file);
  }
  UpdateData(false);
}

void LoadS57MediaDialog::OnBnClickedStoreBrowse()
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
  bi.lpfn           = S57BrowseCallbackProc;
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
      m_dataStore = szInitialDir;

      UpdateData( FALSE );
    }

    pMalloc->Free( pIIL );
    pMalloc->Release();
  }
  if (!m_dataStore.IsEmpty())
  {
    string file = TSLUTF8Encoder(m_dataStore);
    file += "/logfile.txt";
    logFile(file);
  }
}
        
CString& LoadS57MediaDialog::dataStore()
{
  return m_dataStore;
}