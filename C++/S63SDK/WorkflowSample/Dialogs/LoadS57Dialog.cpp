//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "../stdafx.h"
#include "../S63WorkFlowSample.h"
#include "../S63/s63workflow.h"
#include "LoadS57Dialog.h"

static int CALLBACK S57LoadCallbackProc(HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData)
{
  TCHAR szDir[MAX_PATH];

  LoadS57Dialog * dlg = (LoadS57Dialog*)pData;

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
string LoadS57Dialog::m_initialFolder;

IMPLEMENT_DYNAMIC(LoadS57Dialog, CDialog)

LoadS57Dialog::LoadS57Dialog(S63Workflow * workFlow, CWnd* pParent /*=NULL*/)
	: CDialog(LoadS57Dialog::IDD, pParent)
        , m_storeInput()
        , m_dataStore()
        , m_workFlow(workFlow)
{
  m_dataStore = (LPCTSTR)TSLUTF8Decoder(m_initialFolder);
}

LoadS57Dialog::~LoadS57Dialog()
{
  m_workFlow = 0;
}

void LoadS57Dialog::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);

  DDX_Text (pDX, IDC_STORE_LOCATION, m_dataStore );
  DDX_Control(pDX, IDC_STORE_LOCATION, m_storeInput);

  m_initialFolder = TSLUTF8Encoder(m_dataStore);
}


BEGIN_MESSAGE_MAP(LoadS57Dialog, CDialog)

  ON_BN_CLICKED(IDC_STORE_BROWSE, &LoadS57Dialog::OnBnClickedStoreBrowse)
END_MESSAGE_MAP()

  
BOOL LoadS57Dialog::OnInitDialog()
{
  CDialog::OnInitDialog();

  return TRUE;
}
        
CString& LoadS57Dialog::dataStore()
{
  return m_dataStore;
}

void LoadS57Dialog::OnBnClickedStoreBrowse()
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
  bi.lpfn           = S57LoadCallbackProc;
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
