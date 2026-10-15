// ViewAttsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "importexport.h"
#include "ViewAttsDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewAttsDialog dialog


CViewAttsDialog::CViewAttsDialog(CWnd* pParent /*=NULL*/)
: CDialog(CViewAttsDialog::IDD, pParent)
{
  //{{AFX_DATA_INIT(CViewAttsDialog)
		// NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
}


void CViewAttsDialog::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CViewAttsDialog)
  DDX_Control(pDX, IDC_ATT_LIST, m_listCtrl);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewAttsDialog, CDialog)
//{{AFX_MSG_MAP(CViewAttsDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewAttsDialog message handlers

BOOL CViewAttsDialog::OnInitDialog() 
{
  CDialog::OnInitDialog();
  
  RECT	rect;
  m_listCtrl.GetClientRect( &rect );
  int		totalWidth = rect.right - rect.left;
  m_listCtrl.InsertColumn( 0, _T("Attribute"), LVCFMT_LEFT, totalWidth / 2, 0 );
  m_listCtrl.InsertColumn( 1, _T("Value"), LVCFMT_LEFT, totalWidth / 2, 1 );

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void CViewAttsDialog::receiveEntityDataSet( TSLDataSet* dataSet )
{
  if ( !dataSet )
    return;

  m_listCtrl.DeleteAllItems();
#ifdef OLD
  for ( int i = 0; i < dataSet->numAvailableKeys(); i++ )
  {
    LV_ITEM			lvitem;
    lvitem.mask = LVIF_TEXT;
    lvitem.iItem = i;
    lvitem.iSubItem = 0;

    // First get the human readable version of the key
    const char* key = dataSet->getAvailableKey( i );
    const char* humanReadable = dataSet->findName( key );
    if ( !humanReadable )
    {
      // Use key instead. This means that the import has not been completed successfully.
      humanReadable = key;
    }
    // Take copy of humanReadable as we do not want to pass a const char* to MFC when MFC expects a non-const !
    char  copy[256];
    strcpy( copy, humanReadable );
    lvitem.pszText = copy;
    lvitem.iItem = m_listCtrl.InsertItem(&lvitem);

    const TSLVariant* v = dataSet->getData( lvitem.pszText );

    char  value[256];
    v->getValueAsString( value, sizeof(value) );

    lvitem.iSubItem = 1;
    lvitem.pszText = value;
    m_listCtrl.SetItem(&lvitem);

  }
#else
  for ( int i = 0; i < dataSet->sizeData(); i++ )
  {
    LV_ITEM			lvitem;
    lvitem.mask = LVIF_TEXT;
    lvitem.iItem = i;
    lvitem.iSubItem = 0;

    const TSLVariant* v = dataSet->getData( i );

    // First get the human readable version of the key
    char key[ 3 ] ;
    v->getID( key ) ;

    const char* humanReadable = dataSet->findName( key );
    if ( !humanReadable )
    {
      // Use key instead. This means that the import has not been completed successfully.
      humanReadable = key;
    }
    // Take copy of humanReadable as we do not want to pass a const char* to MFC when MFC expects a non-const !
    char  copy[256];
    strcpy( copy, humanReadable );
    lvitem.pszText = (LPTSTR)TSLUTF8Decoder(copy);
    lvitem.iItem = m_listCtrl.InsertItem(&lvitem);


    char  value[256];
    v->getValueAsString( value, sizeof(value) );

    lvitem.iSubItem = 1;
    lvitem.pszText = (LPTSTR)TSLUTF8Decoder(value);
    m_listCtrl.SetItem(&lvitem);

  }
#endif

  m_listCtrl.UpdateData(FALSE);
}

