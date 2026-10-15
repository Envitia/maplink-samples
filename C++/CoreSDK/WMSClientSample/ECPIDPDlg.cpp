// ECPIDPDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WMSClientSample.h"
#include "ECPIDPDlg.h"


// CECPIDPDlg dialog

IMPLEMENT_DYNAMIC(CECPIDPDlg, CDialog)

CECPIDPDlg::CECPIDPDlg(CWnd* pParent, unsigned int numberOfIDPs, const TSLWMSECPServiceSettingsCallbacks::IDPOrganisation* organisations[])
: CDialog(CECPIDPDlg::IDD, pParent)
, m_numberOfIDPs(numberOfIDPs)
, m_organisations(organisations)
, m_index(0)
{

}

CECPIDPDlg::~CECPIDPDlg()
{
}

void CECPIDPDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_IDP_LIST, m_list);
}


BEGIN_MESSAGE_MAP(CECPIDPDlg, CDialog)
  ON_NOTIFY(LVN_ITEMCHANGED, IDC_IDP_LIST, &CECPIDPDlg::OnLvnItemchangedIdpList)
END_MESSAGE_MAP()


// CECPIDPDlg message handlers

BOOL CECPIDPDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_list.InsertColumn(0, _T("Organisation Name"));
  m_list.InsertColumn(1, _T("Organisation URL"));

  CRect	rect;
  m_list.GetClientRect( &rect );
  const int baseWidth( rect.Width() / 2 );
  m_list.SetColumnWidth( 0, baseWidth );
  m_list.SetColumnWidth( 1, baseWidth );


  for ( unsigned int i = 0; i < m_numberOfIDPs; ++i )
  {
    const TSLWMSECPServiceSettingsCallbacks::IDPOrganisation* organisation = m_organisations[i];
    if ( !organisation )
    {
      //it is possible for an IDP to not have an associated IDP
      m_list.InsertItem(i, _T("<Unknown>"));
      m_list.SetItemText(i, 1, _T("<Unknown>"));
      continue;
    }

    unsigned int noOfDisplayNames = organisation->noOfOrganisationDisplayNames();
    ASSERT(noOfDisplayNames); //schema insists that there should be 
    unsigned int noOfURLs = organisation->noOfOrganisationURLs();
    ASSERT(noOfURLs);

    unsigned int displayNameToUse = 0;
    if ( noOfDisplayNames > 1 )
    {
      for ( ; displayNameToUse < noOfDisplayNames; ++displayNameToUse )
      {
        const char* lang = organisation->organisationDisplayNameLanguageAt(displayNameToUse);
        if ( lang && !strncmp("en", lang, 2) )
          break;
      }

      if ( displayNameToUse == noOfDisplayNames )
        displayNameToUse = 0;
    }

    unsigned int urlToUse = 0;
    if ( noOfURLs > 1 )
    {
      for ( ; urlToUse < noOfURLs; ++urlToUse )
      {
        const char* url = organisation->organisationURLLanguageAt(urlToUse);
        if ( url && !strncmp("en", url, 2) )
          break;
      }

      if ( urlToUse == noOfURLs )
        urlToUse = 0;
    }

    const char* name = organisation->organisationDisplayNameAt(displayNameToUse);
    const char* url = organisation->organisationURLAt(urlToUse);
    m_list.InsertItem(i, name ? (LPCTSTR)TSLUTF8Decoder(name) : _T("<Unknown>"));
    m_list.SetItemText(i, 1, url ? (LPCTSTR)TSLUTF8Decoder(url) : _T("<Unknown>"));
  }

  m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void CECPIDPDlg::OnOK()
{
  POSITION pos = m_list.GetFirstSelectedItemPosition();

  if ( !pos )
    return ;

  m_index = m_list.GetNextSelectedItem(pos);

  CDialog::OnOK();
}

void CECPIDPDlg::OnLvnItemchangedIdpList(NMHDR *pNMHDR, LRESULT *pResult)
{
  UINT selectionCount = m_list.GetSelectedCount();
  CButton* okButton = (CButton*)GetDlgItem(IDOK);
  okButton->EnableWindow(selectionCount != 0);

  *pResult = 0;
}
