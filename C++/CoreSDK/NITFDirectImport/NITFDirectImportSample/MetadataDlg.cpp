// MetadataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NITFDirectImportSample.h"
#include ".\metadatadlg.h"


// CMetadataDlg dialog

IMPLEMENT_DYNAMIC(CMetadataDlg, CDialog)
CMetadataDlg::CMetadataDlg(bool & windowVisible, CWnd* pParent /*=NULL*/)
	: CDialog(CMetadataDlg::IDD, pParent)
        , m_windowVisible(windowVisible)
{
}

CMetadataDlg::~CMetadataDlg()
{
}

void CMetadataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


void CMetadataDlg::updateFromDataLayer(TSLNITFFilterDataLayer * dataLayer)
{
  CListBox * list = (CListBox*) GetDlgItem( IDC_LIST1 );
  if ( !list )
  {
    return;
  }
  list->ResetContent();
  if ( dataLayer )
  {
    int metadataCount = dataLayer->metadataCount();
    for ( int i = 0; i < metadataCount; ++i )
    {
      char * string;
      dataLayer->metadata(i, &string);
      list->AddString((LPCTSTR)TSLUTF8Decoder(string));
      delete [] string;
    }
  }
}

BEGIN_MESSAGE_MAP(CMetadataDlg, CDialog)
  ON_WM_CLOSE()
END_MESSAGE_MAP()


// CMetadataDlg message handlers

void CMetadataDlg::OnClose()
{
  m_windowVisible = false;
  CDialog::OnClose();
}
