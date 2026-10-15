//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "../stdafx.h"
#include "../S63WorkFlowSample.h"
#include "../S63/s63workflow.h"
#include "LoadS63Dialog.h"

static int CALLBACK S63BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData)
{
  TCHAR szDir[MAX_PATH];

  LoadS63Dialog * dlg = (LoadS63Dialog*)pData;

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
string LoadS63Dialog::m_initialFolder;

IMPLEMENT_DYNAMIC(LoadS63Dialog, CDialog)

LoadS63Dialog::LoadS63Dialog(S63Workflow * workFlow, CWnd* pParent /*=NULL*/)
	: CDialog(LoadS63Dialog::IDD, pParent)
        , m_storeInput()
        , m_hw_id_input()
        , m_dataServerInput()
        , m_permitInput() 
        , m_permitList()
        , m_expiryLabel()
        , m_dataServerLabel()
        , m_dataStore()
        , m_cellPermits()
        , m_workFlow(workFlow)
        , m_cellNameLabel()
{
  m_dataStore = (LPCTSTR)TSLUTF8Decoder(m_initialFolder);
}

LoadS63Dialog::~LoadS63Dialog()
{
  m_workFlow = 0;
}

void LoadS63Dialog::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);

  DDX_Text (pDX, IDC_STORE_LOCATION, m_dataStore );

  DDX_Control(pDX, IDC_STORE_LOCATION, m_storeInput);
  DDX_Control(pDX, IDC_DATA_SERVER_ID, m_dataServerInput);
  DDX_Control(pDX, IDC_PERMIT_LIST, m_permitList);
  DDX_Control(pDX, IDC_EXPIRY, m_expiryLabel);
  DDX_Control(pDX, IDC_DATA_SERVER, m_dataServerLabel);
  DDX_Control(pDX, IDC_CELL_PERMIT, m_permitInput);
  DDX_Control(pDX, IDC_CELL_NAME, m_cellNameLabel);

  m_initialFolder = TSLUTF8Encoder(m_dataStore);
}


BEGIN_MESSAGE_MAP(LoadS63Dialog, CDialog)

  ON_BN_CLICKED(IDC_STORE_BROWSE, &LoadS63Dialog::OnBnClickedStoreBrowse)
  ON_BN_CLICKED(IDC_ADD_PERMIT, &LoadS63Dialog::OnBnClickedAddPermit)
  ON_BN_CLICKED(IDC_REMOVE_PERMIT, &LoadS63Dialog::OnBnClickedRemovePermit)

  ON_BN_CLICKED(IDC_PERMIT_BROWSE, &LoadS63Dialog::OnBnClickedPermitBrowse)
  ON_LBN_SELCHANGE(IDC_PERMIT_LIST, &LoadS63Dialog::OnLbnSelchangePermitList)
END_MESSAGE_MAP()

  
BOOL LoadS63Dialog::OnInitDialog()
{
  CDialog::OnInitDialog();

  return TRUE;
}
        
CString& LoadS63Dialog::dataStore()
{
  return m_dataStore;
}



void LoadS63Dialog::OnBnClickedStoreBrowse()
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
  bi.lpszTitle      =  _T("Select the location of the S63 data store.") ;
  bi.ulFlags        = BIF_RETURNONLYFSDIRS | BIF_USENEWUI | BIF_STATUSTEXT | BIF_EDITBOX;
  bi.lpfn           = S63BrowseCallbackProc;
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

void LoadS63Dialog::OnBnClickedAddPermit()
{
  m_storeInput.GetWindowText(m_dataStore);

  CString dataServer;
  CString permitPath;

  m_permitInput.GetWindowText(permitPath);
  m_dataServerInput.GetWindowText(dataServer);

  string dataServerID = TSLUTF8Encoder(dataServer);
  string permitFile = TSLUTF8Encoder(permitPath);

  vector<pair<const TSLS63CellPermit*, string> > cellPermits;
  m_workFlow->retrieveCellPermits(permitFile, dataServerID, cellPermits);

  vector<pair<const TSLS63CellPermit*, string> >::const_iterator it;
  for ( it = cellPermits.begin(); it != cellPermits.end() ; ++it ) 
  {
    m_cellPermits.push_back((*it));
  }

  updateCellPermits();

  UpdateData(false);
}

void LoadS63Dialog::OnBnClickedRemovePermit()
{
  int currentSelection;
  if ( (currentSelection = m_permitList.GetCurSel()) != LB_ERR )
  {
    string cellName;
    string dataServerID;

    CString selectedItem;
    m_permitList.GetText( currentSelection, selectedItem );
    string selectedItemUTF8 = TSLUTF8Encoder( selectedItem );
    listItemToString(selectedItemUTF8, cellName, dataServerID);

    vector<pair<const TSLS63CellPermit*, string> >::iterator it;
   
    // Iterate through all the cell permits in the store
    for ( it = m_cellPermits.begin() ; it != m_cellPermits.end() ; ++it ) 
    {
      if (!it->first)
      {
        continue;
      }

      // The cell name and dataServerID should uniquely identify a permit,
      // so loop through until we find ther permit which matches.
      if ( cellName == it->first->cellName() && it->second == dataServerID ) 
      {
        m_cellPermits.erase(it);
        break;
      }
    }

    updateCellPermits();

    // Clear the information regarding the currently selected item.
    m_cellNameLabel.SetWindowText(_T("-"));
    m_dataServerLabel.SetWindowText(_T("-"));
    m_expiryLabel.SetWindowText(_T("-"));

  }
}

void LoadS63Dialog::OnBnClickedPermitBrowse()
{
  CFileDialog FileDlg(true, _T(".txt"), NULL, OFN_NOCHANGEDIR, _T("Permit Files (*.txt)|*.txt|All Files (*.*)|*.*||"));
  FileDlg.m_pOFN->lpstrTitle = _T("Open PERMIT.TXT file");

  CString permitLocation;

  if( FileDlg.DoModal() != IDOK )
  {
    return;
  }
  
  m_permitInput.SetWindowText(FileDlg.GetPathName());
}

void LoadS63Dialog::updateCellPermits()
{
  vector< pair< const TSLS63CellPermit*, string> >::const_iterator it;

  //clear the list
  m_permitList.ResetContent();

  //for each cell permit
  for ( it = m_cellPermits.begin(); it != m_cellPermits.end() ; ++it )
  {
    string listItem = it->second;
    listItem.append(" - ");
    listItem.append(it->first->cellName());

    m_permitList.AddString((LPCTSTR)TSLUTF8Decoder( listItem ));
  }

}

void LoadS63Dialog::listItemToString(const string &listItem, string &cellName, string &dataServerID) const
{
  const int delimiterLocation = listItem.find(" - ");
  cellName = listItem.substr(delimiterLocation + 3);
  dataServerID = listItem.substr(0, delimiterLocation);
}

void LoadS63Dialog::OnLbnSelchangePermitList()
{
  string cellName("-");
  string dataServerID("-");
  long long expiryDate(0);

  int currentSelection;
  if ( (currentSelection = m_permitList.GetCurSel()) != LB_ERR )
  {
    CString selectedItem;
    m_permitList.GetText( currentSelection, selectedItem );
    string selectedItemUTF8 = TSLUTF8Encoder( selectedItem );
    listItemToString(selectedItemUTF8, cellName, dataServerID);

    vector< pair< const TSLS63CellPermit*, string > >::const_iterator it;
    
    // To get the cell expiry date, simply iterate over the cell permit store
    // and search for the appropriate cell permit.

    for ( it = m_cellPermits.begin() ; it != m_cellPermits.end() ; ++it ) 
    {
      // As a cell is uniquely identified by it's name and dataserverID, simply
      // compare all keys in the store with the provided name and id.
      if ( cellName == it->first->cellName() && it->second == dataServerID ) 
      {
        expiryDate = it->first->expiryDate();
        break;
      }
    }
  }

  if ( expiryDate )
  {
    CTime time(expiryDate) ;
    m_expiryLabel.SetWindowText(time.Format(_T("%A, %B %d, %Y")));
  }
  else
  {
    m_expiryLabel.SetWindowText(_T("-"));
  }

  m_cellNameLabel.SetWindowText( (LPCTSTR)TSLUTF8Decoder(cellName) );
  m_dataServerLabel.SetWindowText( (LPCTSTR)TSLUTF8Decoder(dataServerID) );
}
