// ArchiveDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Flashback.h"
#include "ArchiveDialog.h"
#include ".\archivedialog.h"


int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData)
{
  TRACE( _T( "BrowseCallbackProc(0x%x,%d,%p,%p)\n" ), hwnd, uMsg, lp, pData );
  TCHAR szDir[MAX_PATH];

  CArchiveDialog* dlg = (CArchiveDialog*)pData;

  switch ( uMsg ) 
  {
  case BFFM_INITIALIZED: 
    // Initial directory is set here.
    _tcscpy( szDir, (LPCTSTR)dlg->archiveRoot() );
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

// CArchiveDialog dialog

IMPLEMENT_DYNAMIC(CArchiveDialog, CDialog)

CArchiveDialog::CArchiveDialog(char const* dir, CWnd* pParent /*=NULL*/)
  : CDialog           ( CArchiveDialog::IDD, pParent ),
    m_archiveRoot     ( (LPCTSTR)TSLUTF8Decoder(dir) ),
    m_archiveInSubdir ( TRUE )
{
}

CArchiveDialog::~CArchiveDialog()
{
}

void CArchiveDialog::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);

  DDX_Text(pDX, IDC_DIR, m_archiveRoot);
  DDX_Check( pDX, IDC_CHECK_SUBDIR, m_archiveInSubdir );
}


BEGIN_MESSAGE_MAP(CArchiveDialog, CDialog)
  ON_BN_CLICKED(IDC_FINDDIR, OnBnClickedFindDir)
END_MESSAGE_MAP()


// CArchiveDialog message handlers
BOOL CArchiveDialog::OnInitDialog()
{
  CDialog::OnInitDialog();

  // TODO:  Add extra initialization here
  UpdateData( FALSE );

  return TRUE;  // return TRUE unless you set the focus to a control
                // EXCEPTION: OCX Property Pages should return FALSE
}

void CArchiveDialog::OnBnClickedFindDir()
{
  UpdateData( TRUE );

  // Open a directory dialog.
  TCHAR szDisplayName[TSL_MAX_PATH];
  TCHAR szInitialDir[TSL_MAX_PATH];

  BROWSEINFO bi;
  memset( (LPVOID)&bi, 0, sizeof( bi ) );

  szDisplayName[0] = '\0';

  bi.hwndOwner      = AfxGetMainWnd()->m_hWnd;
  bi.pidlRoot       = 0;
  bi.pszDisplayName = szDisplayName;
  bi.lpszTitle      = _T( "Select the archive root directory." );
  bi.ulFlags        = BIF_RETURNONLYFSDIRS | BIF_USENEWUI | BIF_STATUSTEXT | BIF_EDITBOX;
  bi.lpfn           = BrowseCallbackProc;
  bi.lParam         = (LPARAM)this;

  LPITEMIDLIST pIIL = ::SHBrowseForFolder( &bi );

  if ( !pIIL )
  {
    // The user hit Cancel!
    return;
  }

  BOOL bRet = ::SHGetPathFromIDList( pIIL, (LPTSTR)&szInitialDir );
  if ( bRet )
  {
    LPMALLOC pMalloc;
    SHGetMalloc( &pMalloc );
    if ( _tcslen(szInitialDir) != 0 )
    {
      m_archiveRoot = szInitialDir;

      UpdateData( FALSE );
    }

    pMalloc->Free( pIIL );
    pMalloc->Release();
  }
}
