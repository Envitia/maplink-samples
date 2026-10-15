// SaveDialog.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include ".\savedialog.h"

// CSaveDialog dialog

IMPLEMENT_DYNAMIC(CSaveDialog, CDialog)
CSaveDialog::CSaveDialog(TSLNITFFilterDataLayer * dataLayer, 
                         bool selectDisplayItem, 
                         CWnd* pParent /*=NULL*/)
	: CDialog(CSaveDialog::IDD, pParent)
        , m_dataLayer(dataLayer)
        , m_selectDisplayItem(selectDisplayItem)
{
}

CSaveDialog::~CSaveDialog()
{
}

void CSaveDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSaveDialog, CDialog)
END_MESSAGE_MAP()


// CSaveDialog message handlers

bool CSaveDialog::saveDisplayItem( int index )
{
  TSLFilterDataLayerDisplayItem *item = m_dataLayer->getDisplayItemAt(index);
  if ( !item )
  {
    return false;
  }

  CEdit * input = (CEdit*)GetDlgItem(IDC_SAVE);
  CString buffer;
  input->GetWindowText(buffer);
  CString displayItemLocation;
  displayItemLocation.Format( _T("%s/displayItem%d%s"), buffer, index, item->isRasterItem() ? _T(".rdl") : _T(".tmf") );
  CComboBox * rasterTypeCombo = (CComboBox*)GetDlgItem(IDC_RASTER_FORMAT);
  int sel = rasterTypeCombo->GetCurSel();
  TSLRasterType rasterType;
  switch (sel)
  {
  case 1:
    {
      rasterType = TSLRasterTypeBmp;
      break;
    }
  case 2:
    {
      rasterType = TSLRasterTypeJpeg;
      break;
    }
  case 3:
    {
      rasterType = TSLRasterTypePCX;
      break;
    }
  case 4:
    {
      rasterType = TSLRasterTypeJpeg2K;
      break;
    }
  case 0:
  default:
    {
      rasterType = TSLRasterTypeTiff;
      break;
    }
  }

  return item->saveDataLayer(TSLUTF8Encoder(displayItemLocation), TSL_MAPLINK_DEFAULT_VERSION, rasterType);
}
void CSaveDialog::OnOK()
{
  if ( m_selectDisplayItem ) 
  {
    CComboBox * box = (CComboBox*)GetDlgItem(IDC_SEL_DIS_ITEM);
    saveDisplayItem(box->GetCurSel());
  }
  else
  {
     const int numDisplayItems = m_dataLayer->numDisplayItems();
     for ( int i = 0; i < numDisplayItems; ++i )
     {
        if ( !saveDisplayItem(i) )
        {
          break;
        }
     }
  }

  CDialog::OnOK();
}

BOOL CSaveDialog::OnInitDialog()
{
  CDialog::OnInitDialog();
  if ( !m_dataLayer )
  {
    return false;
  }

  int numDisplayItems = m_dataLayer->numDisplayItems();
  if ( !numDisplayItems && m_selectDisplayItem )
  {
    return false;
  }

  CComboBox * box = (CComboBox*)GetDlgItem(IDC_SEL_DIS_ITEM);
  box->EnableWindow(m_selectDisplayItem);
  box->ResetContent();

  if ( m_selectDisplayItem )
  {
    TCHAR buffer[100];
    for ( int i = 0; i < numDisplayItems; ++i )
    {
      _stprintf(buffer, _T("%d"), i);
      box->AddString(buffer);
    }
  }

  return TRUE;  
}
