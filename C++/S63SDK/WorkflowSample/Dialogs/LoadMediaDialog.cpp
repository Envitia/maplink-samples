// LoadMediaDialog.cpp : implementation file
//
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "../stdafx.h"
#include "../S63WorkFlowSample.h"
#include "LoadMediaDialog.h"


// LoadMediaDialog dialog

int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData)
{
  TCHAR szDir[MAX_PATH];

  LoadMediaDialog * dlg = (LoadMediaDialog*)pData;

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
string LoadMediaDialog::m_initialFolder;


IMPLEMENT_DYNAMIC(LoadMediaDialog, CDialog)

LoadMediaDialog::LoadMediaDialog(CWnd* pParent /*=NULL*/)
	: CDialog(LoadMediaDialog::IDD, pParent)
        , m_dataStore()
        , m_productFile()
        , m_productInput()
        , m_serialFile()
        , m_serialInput()
        , m_storeInput()
{
  m_dataStore = (LPCTSTR)TSLUTF8Decoder(m_initialFolder);
}

LoadMediaDialog::~LoadMediaDialog()
{
}

void LoadMediaDialog::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);

  DDX_Text (pDX, IDC_STORE_LOCATION, m_dataStore );
  DDX_Text (pDX, IDC_PRODUCTS_FILE, m_productFile );
  DDX_Text (pDX, IDC_SERIAL_FILE, m_serialFile );

  DDX_Control(pDX, IDC_STORE_LOCATION, m_storeInput);
  DDX_Control(pDX, IDC_PRODUCTS_FILE, m_productInput);
  DDX_Control(pDX, IDC_SERIAL_FILE, m_serialInput);

  m_initialFolder = TSLUTF8Encoder(m_dataStore);
}


BEGIN_MESSAGE_MAP(LoadMediaDialog, CDialog)

  ON_BN_CLICKED(IDC_PRODUCTS_BROWSE, &LoadMediaDialog::OnBnClickedProductsBrowse)
  ON_BN_CLICKED(IDC_STORE_BROWSE, &LoadMediaDialog::OnBnClickedStoreBrowse)
  ON_BN_CLICKED(IDC_SERIAL_BROWSE, &LoadMediaDialog::OnBnClickedSerialBrowse)
END_MESSAGE_MAP()

void LoadMediaDialog::OnBnClickedProductsBrowse()
{
  CFileDialog FileDlg(true, _T("PRODUCTS.txt"), NULL, OFN_NOCHANGEDIR, _T("Text Files (PRODUCTS.txt)|PRODUCTS.txt|All Files (*.*)|*.*||"));
  FileDlg.m_pOFN->lpstrTitle = _T("Open PRODUCTS.TXT file");

  if( FileDlg.DoModal() != IDOK )
  {
    return;
  }
    
  m_productFile = FileDlg.GetPathName();

  //set the unlock key in it's current state.
  m_storeInput.GetWindowText(m_dataStore);
  m_serialInput.GetWindowText(m_serialFile);

  UpdateData(false);
}

void LoadMediaDialog::OnBnClickedStoreBrowse()
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
  bi.lpfn           = BrowseCallbackProc;
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
}

void LoadMediaDialog::OnBnClickedSerialBrowse()
{
  CFileDialog FileDlg(true, _T("SERIAL.enc"), NULL,  OFN_NOCHANGEDIR, _T("ENC Files (SERIAL.enc)|SERIAL.enc|All Files (*.*)|*.*||"));
  FileDlg.m_pOFN->lpstrTitle = _T("Open SERIAL.ENC file");

  if( FileDlg.DoModal() != IDOK )
  {
    return;
  }
    
  m_serialFile = FileDlg.GetPathName();

  //set the unlock key in it's current state.
  m_storeInput.GetWindowText(m_dataStore);
  m_productInput.GetWindowText(m_productFile);
  
  UpdateData(false);
}
        
CString& LoadMediaDialog::dataStore()
{
  return m_dataStore;
}