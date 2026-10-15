//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "../stdafx.h"
#include "../S63WorkFlowSample.h"
#include "../S63/s63workflow.h"
#include "LargeMediaDialog.h"

int CALLBACK LargeMediaBrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData)
{
  TCHAR szDir[MAX_PATH];

  LargeMediaDialog * dlg = (LargeMediaDialog*)pData;

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

string LargeMediaDialog::m_initialFolder;

IMPLEMENT_DYNAMIC(LargeMediaDialog, CDialog)

LargeMediaDialog::LargeMediaDialog(S63Workflow * workFlow, CWnd* pParent /*=NULL*/)
	: CDialog(LargeMediaDialog::IDD, pParent)
        , m_workFlow(workFlow)
        , m_largeMediaFile()
        , m_dataStore()
        , m_mediaRecords()
        , m_mediaInput()
        , m_dataStoreInput()
        , m_mediaRecordInput()
        , m_selectedMediaInput()
        , m_addButton()
        , m_removeButton()
{
  m_dataStore = (LPCTSTR)TSLUTF8Decoder(m_initialFolder); 
}

LargeMediaDialog::~LargeMediaDialog()
{
  m_workFlow = 0;
}

void LargeMediaDialog::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);

  DDX_Text (pDX, IDC_MEDIA_FILE, m_largeMediaFile );
  DDX_Text (pDX, IDC_DATA_STORE, m_dataStore );

  DDX_Control(pDX, IDC_MEDIA_FILE, m_mediaInput);
  DDX_Control(pDX, IDC_DATASET, m_mediaRecordInput);
  DDX_Control(pDX, IDC_SELECTED_DATASETS, m_selectedMediaInput);
  DDX_Control(pDX, IDC_ADD, m_addButton);
  DDX_Control(pDX, IDC_REMOVE, m_removeButton);
  DDX_Control(pDX, IDC_DATA_STORE, m_dataStoreInput);

  m_initialFolder = TSLUTF8Encoder(m_dataStore);
}


BEGIN_MESSAGE_MAP(LargeMediaDialog, CDialog)

  ON_BN_CLICKED(IDC_MEDIA_BROWSE, &LargeMediaDialog::OnBnClickedMediaBrowse)
  ON_BN_CLICKED(IDC_ADD, &LargeMediaDialog::OnBnClickedAdd)
  ON_BN_CLICKED(IDC_REMOVE, &LargeMediaDialog::OnBnClickedRemove)
  ON_BN_CLICKED(IDOK, &LargeMediaDialog::OnBnClickedOk)
  ON_BN_CLICKED(IDC_STORE_BROWSE, &LargeMediaDialog::OnBnClickedStoreBrowse)
END_MESSAGE_MAP()

void LargeMediaDialog::OnBnClickedMediaBrowse()
{
  CFileDialog FileDlg(true, _T(".txt"), NULL, OFN_NOCHANGEDIR, _T("MEDIA Files (*.txt)|*.txt|All Files (*.*)|*.*||"));
  FileDlg.m_pOFN->lpstrTitle = _T("Open MEDIA.TXT file");

  if( FileDlg.DoModal() != IDOK )
  {
    return;
  }
    
  UpdateData(TRUE);

  m_largeMediaFile = FileDlg.GetPathName();

  UpdateData(FALSE);

  //see if what they entered is a correct media file
  updateMediaList();
}

void LargeMediaDialog::updateMediaList()
{
  vector<string> mediaRecords;
  TSLUTF8Encoder largeMediaPathUTF8( m_largeMediaFile );
  string largeMediaPath( largeMediaPathUTF8 );

  m_mediaRecordInput.ResetContent();
  m_selectedMediaInput.ResetContent();  

  if ( m_workFlow->getMediaRecords(largeMediaPath, mediaRecords) )
  {
    vector<string>::const_iterator it;

    for ( it = mediaRecords.begin(); it != mediaRecords.end() ; ++it )
    {
      const CString mediaID = (LPCTSTR)TSLUTF8Decoder(it->c_str());
      m_mediaRecordInput.AddString(mediaID);
    }
  }
  else
  {
    AfxMessageBox(_T("The specified file is not a valid MEDIA.TXT file"), MB_ICONERROR ) ;
  }

  const bool hasRecords = (m_mediaRecordInput.GetCount() > 0);

  m_mediaRecordInput.EnableWindow(hasRecords);
  m_selectedMediaInput.EnableWindow(hasRecords);
  m_addButton.EnableWindow(hasRecords);
  m_removeButton.EnableWindow(hasRecords);
}
void LargeMediaDialog::OnBnClickedAdd()
{
  int currentSelection;
  if ( (currentSelection = m_mediaRecordInput.GetCurSel()) != LB_ERR )
  {
    CString selectedItem;
    m_mediaRecordInput.GetText( currentSelection, selectedItem );

    m_mediaRecordInput.DeleteString(currentSelection);   
    m_selectedMediaInput.AddString(selectedItem);
  } 
}

void LargeMediaDialog::OnBnClickedRemove()
{
  int currentSelection;
  if ( (currentSelection = m_selectedMediaInput.GetCurSel()) != LB_ERR )
  {
    CString selectedItem;
    m_selectedMediaInput.GetText( currentSelection, selectedItem );

    m_selectedMediaInput.DeleteString(currentSelection);   
    m_mediaRecordInput.AddString(selectedItem);
  } 
}

void LargeMediaDialog::OnBnClickedOk()
{
  const int mediaRecordCount = m_selectedMediaInput.GetCount();

  for ( int i = 0 ; i < mediaRecordCount ; ++i ) 
  {
    CString buffer;
    m_selectedMediaInput.GetText( i, buffer );

    std::string mediaRecord = TSLUTF8Encoder(buffer);
    m_mediaRecords.push_back(mediaRecord);
  }

  CDialog::OnOK();
}

void LargeMediaDialog::OnBnClickedStoreBrowse()
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
  bi.lpszTitle      = _T("Select the location of the data store.") ;
  bi.ulFlags        = BIF_RETURNONLYFSDIRS | BIF_USENEWUI | BIF_STATUSTEXT | BIF_EDITBOX;
  bi.lpfn           = LargeMediaBrowseCallbackProc;
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

CString& LargeMediaDialog::dataStore()
{
  return m_dataStore;
}