// CRSDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WMSClientSample.h"
#include "CRSDlg.h"
#include "OpenServiceDlg.h"


// CCRSDlg dialog

IMPLEMENT_DYNAMIC(CCRSDlg, CDialog)

CCRSDlg::CCRSDlg(CWnd* pParent, CRSData& crsData)
	: CDialog(CCRSDlg::IDD, pParent)
        , m_crsData(crsData)
{
}

CCRSDlg::~CCRSDlg()
{
}

void CCRSDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_CRS_LIST, m_list);
}


BEGIN_MESSAGE_MAP(CCRSDlg, CDialog)
  ON_NOTIFY(NM_DBLCLK, IDC_CRS_LIST, &CCRSDlg::OnNMDblclkCrsList)
END_MESSAGE_MAP()


// CCRSDlg message handlers


BOOL CCRSDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_list.InsertColumn(0, _T("CRS"));
  m_list.InsertColumn(1, _T("Info"));

  CRect	rect;
  m_list.GetClientRect( &rect );
  m_list.SetColumnWidth( 0, (int)(rect.Width() * 0.35) );
  m_list.SetColumnWidth( 1, (int)(rect.Width() * 0.6) );

  for ( int i = 0; i < m_crsData.m_noOfCRSChoices; ++i )
  {
    const char *crs = m_crsData.m_crsChoices[i];
    m_list.InsertItem(i, (LPCTSTR)TSLUTF8Decoder(crs));

    if (strnicmp(crs, "EPSG:", 5) == 0)
    {
      if (int epsgid = atoi(crs+5))
      {
        // Use the Transverse Mercator Snyder formula instead of JHS formula as the Snyder
        // formula is predominant.
        const TSLCoordinateSystem *coordSys = TSLCoordinateSystem::findByEPSG(epsgid);
        if (coordSys)
        {
          m_list.SetItemText(i, 1, (LPCTSTR)TSLUTF8Decoder(coordSys->name()));
        }
      }
    }
  }

  m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);
  m_list.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void CCRSDlg::OnOK()
{
  POSITION pos = m_list.GetFirstSelectedItemPosition();

  if ( !pos )
    return ;

  m_crsData.m_chosenCRS = m_list.GetNextSelectedItem(pos);

  CDialog::OnOK();
}

void CCRSDlg::OnNMDblclkCrsList(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

  int pos = m_list.HitTest(pNMItemActivate->ptAction);
  if (pos >= 0)
  {
    OnOK();
  }

  *pResult = 0;
}
