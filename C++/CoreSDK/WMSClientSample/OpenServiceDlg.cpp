// OpenServiceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WMSClientSample.h"
#include "WMSSettingsCallbacks.h"
#include "OpenServiceDlg.h"
#include "StyleDimensionsDlg.h"
#include "ECPIDPCredentials.h"
#include "ECPIDPDlg.h"
#include "CRSDlg.h"
#include "mainfrm.h"
#include <string>
#include <sstream>

#define TREE_UNTICK_IMAGE_IDX   1
#define TREE_TICK_IMAGE_IDX     2
#define TREE_GREYTICK_IMAGE_IDX 3
#define TREE_UNNAMED_IMAGE_IDX  4

#define NUMBER_OF_MRU_ADDRESSES 16


// COpenServiceDlg dialog

IMPLEMENT_DYNAMIC(COpenServiceDlg, CDialog)

COpenServiceDlg::COpenServiceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COpenServiceDlg::IDD, pParent)
        , m_layer(0)
        , m_event(0)
        , m_rootLayer(0)
{
}

COpenServiceDlg::~COpenServiceDlg()
{
}

void COpenServiceDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(COpenServiceDlg, CDialog)
  ON_BN_CLICKED(IDC_GOBUTTON, &COpenServiceDlg::OnBnClickedGobutton)
  ON_MESSAGE(WM_CAPABILITIES_LOADED, OnCapabilitiesLoaded)
  ON_MESSAGE(WM_CAPABILITIES_LOAD_FAIL, OnCapabilitiesLoadFail)
  ON_MESSAGE(WM_CAPABILITIES_COMPLETE, OnCapabilitiesComplete)
  ON_MESSAGE(WM_SHOW_DS_DLG, OnShowDSDialog)
  ON_MESSAGE(WM_NO_COMMON_CRS, OnNoCommonCRS)
  ON_MESSAGE(WM_NO_SUPPORTED_CRS, OnNoSupportedCRS)
  ON_MESSAGE(WM_CREDENTIALS_REQUIRED, OnCredentialsRequired)
  ON_MESSAGE(WM_IDP_CHOICE, OnChoiceOfIDP)
  ON_BN_CLICKED(IDCANCEL, &COpenServiceDlg::OnBnClickedCancel)
  ON_NOTIFY(NM_CLICK, IDC_LAYERTREE, &COpenServiceDlg::OnNMClickLayertree)
  ON_LBN_SELCHANGE(IDC_LAYERORDER, &COpenServiceDlg::OnLbnSelchangeLayerorder)
  ON_BN_CLICKED(IDOK, &COpenServiceDlg::OnBnClickedOk)
  ON_BN_CLICKED(IDC_TOPBUTTON, &COpenServiceDlg::OnBnClickedTopbutton)
  ON_BN_CLICKED(IDC_UPBUTTON, &COpenServiceDlg::OnBnClickedUpbutton)
  ON_BN_CLICKED(IDC_DOWNBUTTON, &COpenServiceDlg::OnBnClickedDownbutton)
  ON_BN_CLICKED(IDC_BOTTOMBUTTON, &COpenServiceDlg::OnBnClickedBottombutton)
  ON_NOTIFY(TVN_SELCHANGED, IDC_LAYERTREE, &COpenServiceDlg::OnTvnSelchangedLayertree)
  ON_NOTIFY(TVN_KEYDOWN, IDC_LAYERTREE, &COpenServiceDlg::OnTvnKeydownLayertree)
  ON_BN_CLICKED(IDC_ECP_CHECK, &COpenServiceDlg::OnBnClickedEcpCheck)
  ON_MESSAGE(WM_CRS_CHOICE, &COpenServiceDlg::OnCrsChoice)
END_MESSAGE_MAP()

// COpenServiceDlg message handlers

void COpenServiceDlg::OnBnClickedGobutton()
{
  CComboBox* addressCombo = (CComboBox*) GetDlgItem(IDC_ADDRESS_COMBO);
  CComboBox* fmCombo = (CComboBox*) GetDlgItem(IDC_FM_ADDRESS_COMBO);
  CButton* ecpCheckbox = (CButton*)GetDlgItem(IDC_ECP_CHECK);
  ASSERT( addressCombo && m_layer && fmCombo && ecpCheckbox );

  CString content;
  addressCombo->GetWindowText(content);

  if ( content.IsEmpty() )
  {
    AfxMessageBox(_T("Error: Please provide a valid address"));
    return;
  }
  
  bool useECP = ecpCheckbox->GetCheck() == BST_CHECKED;

  CString fmContent;
  if ( useECP )
  {
    fmCombo->GetWindowText(fmContent);
    if ( fmContent.IsEmpty() )
    {
      AfxMessageBox(_T("Error: Please provide a valid Federation Metadata address"));
      return;
    }
  }

  //Add to the drop down list , but first check it doesn't match an existing entry
  int foundindex = addressCombo->FindStringExact(0, content);
  if ( foundindex != -1 )
  {
    addressCombo->DeleteString(foundindex);
  }
  addressCombo->InsertString(0, content);
  addressCombo->SelectString(-1, content);

  CButton * goButton = (CButton*) GetDlgItem(IDC_GOBUTTON);
  goButton->EnableWindow(false);
  makeOkButtonDefault();
  addressCombo->EnableWindow(false);
  ecpCheckbox->EnableWindow(false);
  fmCombo->EnableWindow(false);

  TSLErrorStack::clear();

  TSLUTF8Encoder contentUTF8( content );
  if ( useECP )
  {
    foundindex = fmCombo->FindStringExact(0, fmContent.GetString());
    if ( foundindex != -1 )
    {
      fmCombo->DeleteString(foundindex);
    }
    fmCombo->InsertString(0, fmContent.GetString());
    fmCombo->SelectString(-1, fmContent);
    if  ( m_layer->loadDataWithECP(contentUTF8, TSLUTF8Encoder(fmContent)) )
    {
      m_url = contentUTF8;
      return;
    }
  }
  else if ( m_layer->loadData(contentUTF8) )
  {
    m_url = contentUTF8;
    return;
  }

  const char* err = TSLErrorStack::errorString();
  if ( err )
  {
    AfxMessageBox((LPCTSTR)TSLUTF8Decoder(err));
  }

  resetDialogControls();
}

BOOL COpenServiceDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  CWinApp * app = AfxGetApp();
  ASSERT(app);
  CComboBox* addressCombo = (CComboBox*) GetDlgItem(IDC_ADDRESS_COMBO);
  ASSERT( addressCombo );
  CComboBox* fmCombo = (CComboBox*) GetDlgItem(IDC_FM_ADDRESS_COMBO);
  ASSERT( fmCombo );

  TCHAR buffer[1024];
  for ( unsigned int i = 0; i < NUMBER_OF_MRU_ADDRESSES; ++i )
  {
    _stprintf(buffer, _T("File%u"), i);
    CString mruf = app->GetProfileString(MRU_URL_APPSETTING, buffer);
    if ( !mruf.IsEmpty() ) addressCombo->AddString(mruf);
    mruf = app->GetProfileString(MRU_FMD_APPSETTING, buffer);
    if ( !mruf.IsEmpty() ) fmCombo->AddString(mruf);
  }

  m_treeCheckboxes.Create(IDB_CHECKBOXES, 16, 4, RGB(255,0,255));
  CTreeCtrl * layerTree = (CTreeCtrl *) GetDlgItem(IDC_LAYERTREE);
  ASSERT(layerTree);

  layerTree->SetImageList(&m_treeCheckboxes, TVSIL_STATE);


  HMODULE thisMod = ::GetModuleHandle(0);
  m_upButton.SubclassDlgItem(IDC_UPBUTTON, this);
  m_downButton.SubclassDlgItem(IDC_DOWNBUTTON, this);
  m_topButton.SubclassDlgItem(IDC_TOPBUTTON, this);
  m_bottomButton.SubclassDlgItem(IDC_BOTTOMBUTTON, this);
  m_upButton.SetBitmaps( (HBITMAP)::LoadImage(thisMod, MAKEINTRESOURCE(IDB_UP), IMAGE_BITMAP, 16,16, LR_LOADTRANSPARENT|LR_LOADMAP3DCOLORS), 0, 0,
                         (HBITMAP)::LoadImage(thisMod, MAKEINTRESOURCE(IDB_UPD), IMAGE_BITMAP, 16,16, LR_LOADTRANSPARENT|LR_LOADMAP3DCOLORS));
  m_downButton.SetBitmaps( (HBITMAP)::LoadImage(thisMod, MAKEINTRESOURCE(IDB_DOWN), IMAGE_BITMAP, 16,16, LR_LOADTRANSPARENT|LR_LOADMAP3DCOLORS), 0, 0,
                            (HBITMAP)::LoadImage(thisMod, MAKEINTRESOURCE(IDB_DOWND), IMAGE_BITMAP, 16,16, LR_LOADTRANSPARENT|LR_LOADMAP3DCOLORS));
  m_topButton.SetBitmaps( (HBITMAP)::LoadImage(thisMod, MAKEINTRESOURCE(IDB_TOP), IMAGE_BITMAP, 16,16, LR_LOADTRANSPARENT|LR_LOADMAP3DCOLORS), 0, 0,
                          (HBITMAP)::LoadImage(thisMod, MAKEINTRESOURCE(IDB_TOPD), IMAGE_BITMAP, 16,16, LR_LOADTRANSPARENT|LR_LOADMAP3DCOLORS));
  m_bottomButton.SetBitmaps( (HBITMAP)::LoadImage(thisMod, MAKEINTRESOURCE(IDB_BOTTOM), IMAGE_BITMAP, 16,16, LR_LOADTRANSPARENT|LR_LOADMAP3DCOLORS), 0, 0,
                             (HBITMAP)::LoadImage(thisMod, MAKEINTRESOURCE(IDB_BOTTOMD), IMAGE_BITMAP, 16,16, LR_LOADTRANSPARENT|LR_LOADMAP3DCOLORS));

  CButton * dsDefault = (CButton*) GetDlgItem(IDC_DS_DEFAULT);
  dsDefault->SetCheck(BST_CHECKED);

  CListCtrl * layerDetails = (CListCtrl*) GetDlgItem(IDC_LAYER_DETAILS);
  CRect layerDetailsHeaderRect; 
  layerDetails->GetHeaderCtrl()->GetWindowRect(&layerDetailsHeaderRect);
  int width = layerDetailsHeaderRect.Width();
  layerDetails->InsertColumn(0, _T("Name"),  0, (int)floor(0.4 * width));
  layerDetails->InsertColumn(1, _T("Value"), 0, (int)floor(0.55 * width));

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT COpenServiceDlg::OnCapabilitiesLoaded(WPARAM wparam, LPARAM lparam)
{
  m_event = (CEvent*) wparam;
  //We can hold on to the layer through the callback loop as it
  //will be the same instance passed each time. We mustn't hold on
  //to it beyond the callback loop however
  m_rootLayer = (TSLWMSServiceLayer**)lparam;

  ASSERT(m_event && m_rootLayer);

  CTreeCtrl * layerTree = (CTreeCtrl *) GetDlgItem(IDC_LAYERTREE);
  HTREEITEM root = AddLayerToTree(layerTree, 0, 0, *m_rootLayer);
  if ( root )
  {
    layerTree->Expand(root, TVE_EXPAND);
  }

  layerTree->EnableWindow();

  CListBox * layerOrderList = (CListBox *) GetDlgItem(IDC_LAYERORDER);
  layerOrderList->EnableWindow();
  return 0;
}

static void getErrorString(TSLSimpleString& err, const char *hdr)
{
  if ( !TSLThreadedErrorStack::errorString(err, hdr) )
  {
    err = hdr;
  }
}

LRESULT COpenServiceDlg::OnCapabilitiesLoadFail(WPARAM wparam, LPARAM lparam)
{
  TSLWMSServiceSettingsCallbacks::CapabilitiesLoadFailureReason reason = (TSLWMSServiceSettingsCallbacks::CapabilitiesLoadFailureReason) wparam;

  TSLSimpleString err ;

  switch (reason)
  {
  case TSLWMSServiceSettingsCallbacks::LoadingFailedInternetSettings:
    getErrorString( err, "Error: Internet settings invalid\n");
    break;
  case TSLWMSServiceSettingsCallbacks::LoadingFailedLoadFailed:
    getErrorString( err, "Error: Failed to access service\n");
    break;
  case TSLWMSServiceSettingsCallbacks::LoadingFailedParseFailure:
    getErrorString( err, "Error: Failed to parse service metadata\n");
    break;
  case TSLWMSServiceSettingsCallbacks::LoadingFailedServiceUnsupported:
    getErrorString( err, "Error: Service not supported\n");
    break;
  case TSLWMSServiceSettingsCallbacks::LoadingFailedUnableToDetermineExtent:
    getErrorString( err, "Error: Unable to determine layer extent. Service not supported\n");
    break;
  case TSLWMSServiceSettingsCallbacks::LoadingFailedToFitInTMCSpace:
    getErrorString( err, "Error: Unable to fit layer(s) extent into the drawing area. Service not supported\n");
    break;
  case TSLWMSServiceSettingsCallbacks::LoadingECPFailedBadAuthnRequest:
    getErrorString( err, "Error: The service provider returned an invalid AuthnRequest\n");
    break;
  case TSLWMSServiceSettingsCallbacks::LoadingECPFailedFederationMetadataNotAccessible:
    getErrorString( err, "Error: Failed to access the federation metadata\n");
    break;
  case TSLWMSServiceSettingsCallbacks::LoadingECPFailedBadFederationMetadata:
    getErrorString( err, "Error: Failed to load the federation metadata\n");
    break;
  case TSLWMSServiceSettingsCallbacks::LoadingECPFailedNoECPSupportingIDPs:
    getErrorString( err, "Error: None of the IDPs in the federation support the ECP profile\n");
    break;
  case TSLWMSServiceSettingsCallbacks::LoadingECPFailedIDPNotAccessible:
    getErrorString( err, "Error: Failed to access the IDP\n");
    break;
  case TSLWMSServiceSettingsCallbacks::LoadingECPFailedBadAuthnResponse:
    getErrorString( err, "Error: Failed to load the IDP's AuthnResponse\n");
    break;
  case TSLWMSServiceSettingsCallbacks::LoadingECPFailedSPECP:
    getErrorString( err, "Error: Failed to forward the IDP's AuthnResponse to the Service Provider for authentication\n");
    break;
  case TSLWMSServiceSettingsCallbacks::LoadingECPFailedAccessDenied:
    getErrorString( err, "Error: Access was denied. Perhaps the credentials provided were incorrect or insufficient\n");
    break;
  }


  // reduce the version requested...
  TSLWMSVersionEnum ver = m_layer->getInitialRequestWMSVersion ();
  if (ver > TSLWMSVersion_1_1_0)
  {
    m_layer->setInitialRequestWMSVersion((TSLWMSVersionEnum)(ver - 1));
    OnBnClickedGobutton();
  }
  else
  {
    if ( !err.empty() )
    {
      AfxMessageBox( (LPCTSTR)TSLUTF8Decoder(err) ) ;
    }
  }

  resetDialogControls();

  return 0;
}

void COpenServiceDlg::resetDialogControls()
{
  CTreeCtrl* layerTree = (CTreeCtrl*) GetDlgItem(IDC_LAYERTREE);
  CListBox* layerList = (CListBox*) GetDlgItem(IDC_LAYERORDER);
  CListCtrl* layerDetails = (CListCtrl*) GetDlgItem(IDC_LAYER_DETAILS);
  CComboBox* addressCombo = (CComboBox*) GetDlgItem(IDC_ADDRESS_COMBO);
  CComboBox* fmCombo = (CComboBox*) GetDlgItem(IDC_FM_ADDRESS_COMBO);
  CButton* ecpCheckbox = (CButton*) GetDlgItem(IDC_ECP_CHECK);
  CButton* goButton = (CButton*) GetDlgItem(IDC_GOBUTTON);
  CButton* okButton = (CButton*) GetDlgItem(IDOK);

  ASSERT( layerTree && layerList && layerDetails && addressCombo &&
          fmCombo && ecpCheckbox && goButton && okButton );

  layerTree->DeleteAllItems();
  layerDetails->DeleteAllItems();
  layerList->ResetContent();

  m_upButton.EnableWindow(FALSE);
  m_downButton.EnableWindow(FALSE);
  m_topButton.EnableWindow(FALSE);
  m_bottomButton.EnableWindow(FALSE);

  ASSERT( m_layer );

  addressCombo->SetWindowText(_T(""));
  addressCombo->EnableWindow();
  ecpCheckbox->EnableWindow();
  fmCombo->SetWindowText(_T(""));
  fmCombo->EnableWindow(ecpCheckbox->GetCheck() == BST_CHECKED);

  goButton->EnableWindow();
  okButton->EnableWindow(FALSE);
  makeGoButtonDefault();
}

LRESULT COpenServiceDlg::OnCapabilitiesComplete(WPARAM wparam, LPARAM lparam)
{
  CComboBox* addressCombo = (CComboBox*) GetDlgItem(IDC_ADDRESS_COMBO);
  CMainFrame* mainWindow = (CMainFrame*)::AfxGetMainWnd();
  ASSERT(addressCombo && mainWindow);

  CString content;
  addressCombo->GetWindowText(content);
  const CString& title = mainWindow->GetActiveDocument()->GetTitle();
  if (title != "Untitled")
  {
    content = content + _T(", ") + title;
  }
  mainWindow->GetActiveDocument()->SetTitle(content);

  OnOK();
  return 0;
}

LRESULT COpenServiceDlg::OnNoCommonCRS(WPARAM wparam, LPARAM lparam)
{
  m_event = (CEvent*) wparam;
  m_rootLayer = (TSLWMSServiceLayer**)lparam;

  AfxMessageBox(_T("The visible layers do not share a commonly supported CRS/SRS.\nPlease modify the visible layers before continuing"), MB_OK);

  CButton * okButton = (CButton*)GetDlgItem(IDOK);
  ASSERT( okButton );
  okButton->EnableWindow();

  return 0;
}

LRESULT COpenServiceDlg::OnNoSupportedCRS(WPARAM wparam, LPARAM lparam)
{
  m_event = (CEvent*) wparam;
  m_rootLayer = (TSLWMSServiceLayer**)lparam;

  AfxMessageBox(_T("Of the visible layers, although there is a common CRS/SRS, MapLink does not support it.\nPlease modify the visible layers before continuing"), MB_OK);

  CButton * okButton = (CButton*)GetDlgItem(IDOK);
  ASSERT( okButton );
  okButton->EnableWindow();

  return 0;
}

LRESULT COpenServiceDlg::OnCredentialsRequired(WPARAM wparam, LPARAM lparam)
{
  CEvent* _event = (CEvent*) wparam;
  void** vs = (void**)lparam;

  CECPIDPCredentials dlg(this, (const TSLWMSECPServiceSettingsCallbacks::IDPOrganisation*)vs[2]);
  if ( dlg.DoModal() == IDOK )
  {
    *(TSLSimpleString*)vs[0] = dlg.username();
    *(TSLSimpleString*)vs[1] = dlg.password();
  }
  else
  {
    vs[0] = 0;
    resetDialogControls();
  }

  if ( _event )
  {
    _event->SetEvent();
  }

  return 0;
}

LRESULT COpenServiceDlg::OnChoiceOfIDP(WPARAM wparam, LPARAM lparam)
{
  CEvent* _event = (CEvent*) wparam;
  void** params = (void**)lparam;

  unsigned int numberOfIDPChoices = *(unsigned int*)params[0];
  const TSLWMSECPServiceSettingsCallbacks::IDPOrganisation** organisations = (const TSLWMSECPServiceSettingsCallbacks::IDPOrganisation**)params[1];
  int& returnValue = *(int*)params[2];

  CECPIDPDlg dlg(this, numberOfIDPChoices, organisations);
  if ( dlg.DoModal() == IDOK )
  {
    returnValue = dlg.choosenIndex();
  }
  else
  {
    returnValue = -1;
    resetDialogControls();
  }

  if ( _event )
  {
    _event->SetEvent();
  }

  return 0;
}



afx_msg LRESULT COpenServiceDlg::OnCrsChoice(WPARAM wParam, LPARAM lParam)
{
  m_event = (CEvent*) wParam;
  CRSData* params = (CRSData*)lParam;


  CCRSDlg dlg(this, *params);
  if ( dlg.DoModal() == IDOK )
  {
    // CRSData::m_chosenCRS is set
  }
  else
  {
    params->m_chosenCRS = -1;

    resetDialogControls();
  }

  if ( m_event )
  {
    m_event->SetEvent();
    m_event = 0;
  }

  return 0;
}


LRESULT COpenServiceDlg::OnShowDSDialog(WPARAM wparam, LPARAM lparam)
{
  m_event = (CEvent*) wparam;
  m_rootLayer = (TSLWMSServiceLayer**)lparam;

  INT_PTR dlgResult = 0;
  if ( m_rootLayer )
  {
    CStyleDimensionsDlg dlg(this, *m_rootLayer);
    dlgResult = dlg.DoModal();
  }

  if ( m_event )
  {
    if ( m_rootLayer && dlgResult != IDOK)
    {
      *m_rootLayer = 0;
    }
    m_event->SetEvent();
    m_event = 0;
  }

  if ( dlgResult != IDOK )
  {
    OnCancel();
  }

  return 0;
}

void COpenServiceDlg::OnBnClickedCancel()
{
  if ( m_event )
  {
    if ( m_rootLayer )
    {
      *m_rootLayer = 0;
    }
    m_event->SetEvent();
    m_event = 0;
  }
  OnCancel();
}

HTREEITEM COpenServiceDlg::AddLayerToTree( CTreeCtrl * tree, HTREEITEM parent, HTREEITEM after, TSLWMSServiceLayer* layer)
{
  const char * layerName = layer->name();
  HTREEITEM newItem = 0;
  if ( layerName )
  {
    newItem = tree->InsertItem(TVIF_TEXT | TVIF_PARAM, (LPCTSTR)TSLUTF8Decoder(layerName), 0, 0, 0, 0, 
                               (LPARAM)layer, parent, after);
  }
  else
  {
    newItem = tree->InsertItem(TVIF_STATE | TVIF_TEXT | TVIF_PARAM, 
                                _T("[UNNAMED]"), 0, 0, 
                                INDEXTOSTATEIMAGEMASK(TREE_UNNAMED_IMAGE_IDX), 
                                TVIS_STATEIMAGEMASK, (LPARAM)layer, parent, after);
  }

  int noOfSubLayers = layer->noOfSubLayers();
  HTREEITEM last = 0;
  for ( int i  = 0; i < noOfSubLayers; ++i )
  {
    TSLWMSServiceLayer* subLayer = layer->getSubLayerAt(i);
    last = AddLayerToTree( tree, newItem, last, subLayer);
  }
  return newItem; 
}
void COpenServiceDlg::OnNMClickLayertree(NMHDR *pNMHDR, LRESULT *pResult)
{
  CTreeCtrl * layerTree = (CTreeCtrl *) GetDlgItem(IDC_LAYERTREE);
  CPoint pt;
  GetCursorPos(&pt);
  layerTree->ScreenToClient(&pt);
  UINT flags = 0;
  HTREEITEM item = layerTree->HitTest(pt, &flags);
  if ( !item )
    return;

  *pResult = 0;

  if ( flags & TVHT_ONITEMSTATEICON )
  {
    TSLWMSServiceLayer* associatedLayer = (TSLWMSServiceLayer*)layerTree->GetItemData(item);
    ASSERT(associatedLayer);
    const char * layerName = associatedLayer->name();
    *pResult = 1;
    if ( layerName != 0 )
    {
      UINT image = layerTree->GetItemState(item, TVIS_STATEIMAGEMASK);
      image = image >> 12;
      if ( image != TREE_GREYTICK_IMAGE_IDX )
      {
        if ( ++image == TREE_GREYTICK_IMAGE_IDX ) image = TREE_UNTICK_IMAGE_IDX;
        layerTree->SetItemState(item, INDEXTOSTATEIMAGEMASK(image), TVIS_STATEIMAGEMASK);
        CListBox * layerList = (CListBox *) GetDlgItem(IDC_LAYERORDER);
        CButton * okButton = (CButton*)GetDlgItem(IDOK);
        if ( image == TREE_UNTICK_IMAGE_IDX )
        {
          //unchecked
          int noOfVisibleLayers = layerList->GetCount();
          for ( int i = 0; i < noOfVisibleLayers; ++i )
          {
            if ( layerList->GetItemData(i) == (DWORD_PTR)associatedLayer )
            {
              layerList->DeleteString(i);
              OnLbnSelchangeLayerorder();
              break;
            }
          }
          SetAllChildItemsToOff(layerList, layerTree, item);
          okButton->EnableWindow(layerList->GetCount() > 0);
        }
        else
        {
          //checked
          int index = layerList->InsertString(-1, (LPCTSTR)TSLUTF8Decoder(layerName));
          layerList->SetItemData(index, (DWORD_PTR) associatedLayer);
          SetAllChildItemsToPartial(layerList, layerTree, item);
          OnLbnSelchangeLayerorder();
          okButton->EnableWindow();
        }
        makeOkButtonDefault();
      }
    }
  }
}

void COpenServiceDlg::SetAllChildItemsToPartial(CListBox * layerList, CTreeCtrl * tree, HTREEITEM parent)
{
  HTREEITEM child = tree->GetChildItem(parent);
  while ( child )
  {
    int currentState = tree->GetItemState(child, TVIS_STATEIMAGEMASK);
    if ( currentState >> 12 == TREE_TICK_IMAGE_IDX )
    {
      DWORD_PTR data = tree->GetItemData(child);
      int noOfVisibleLayers = layerList->GetCount();
      for ( int i = 0; i < noOfVisibleLayers; ++i )
      {
        if ( layerList->GetItemData(i) == data )
        {
          layerList->DeleteString(i);
          break;
        }
      }
    }
    tree->SetItemState(child, 
                        INDEXTOSTATEIMAGEMASK(TREE_GREYTICK_IMAGE_IDX),
                        TVIS_STATEIMAGEMASK);
    
    SetAllChildItemsToPartial(layerList, tree, child);
    child = tree->GetNextSiblingItem(child);
  }
}

void COpenServiceDlg::SetAllChildItemsToOff(CListBox * layerList, CTreeCtrl * tree, HTREEITEM parent)
{
  HTREEITEM child = tree->GetChildItem(parent);
  while ( child )
  {
    tree->SetItemState(child, 
                        INDEXTOSTATEIMAGEMASK(TREE_UNTICK_IMAGE_IDX),
                        TVIS_STATEIMAGEMASK);
    
    SetAllChildItemsToOff(layerList, tree, child);
    child = tree->GetNextSiblingItem(child);
  }
}

void COpenServiceDlg::OnLbnSelchangeLayerorder()
{
  m_upButton.EnableWindow(FALSE);
  m_downButton.EnableWindow(FALSE);
  m_topButton.EnableWindow(FALSE);
  m_bottomButton.EnableWindow(FALSE);

  CListBox * layerList = (CListBox *) GetDlgItem(IDC_LAYERORDER);
  int selectedIndex = layerList->GetCurSel();
  if ( selectedIndex == -1 )
    return;

  int itemCount = layerList->GetCount();
  if ( selectedIndex != 0 && itemCount > 1 )
  {
    m_upButton.EnableWindow(TRUE);
    m_topButton.EnableWindow(TRUE);
  }
  if ( selectedIndex != itemCount-1 )
  {
    m_downButton.EnableWindow(TRUE);
    m_bottomButton.EnableWindow(TRUE);
  }
}

void COpenServiceDlg::setAllChildLayersToBeNonVisible(TSLWMSServiceLayer * layer)
{
  ASSERT(layer);

  int noOfChildLayers = layer->noOfSubLayers();
  for ( int i = 0; i < noOfChildLayers; ++i )
  {
    TSLWMSServiceLayer * childLayer = layer->getSubLayerAt(i);
    childLayer->setVisibility(false);
    setAllChildLayersToBeNonVisible(childLayer);
  }
}

void COpenServiceDlg::makeGoButtonDefault()
{
  CButton * okButton = (CButton*) GetDlgItem(IDOK);
  CButton * goButton = (CButton*) GetDlgItem(IDC_GOBUTTON);
  okButton->SetButtonStyle( ~BS_DEFPUSHBUTTON & okButton->GetButtonStyle() );
  goButton->SetButtonStyle( BS_DEFPUSHBUTTON | goButton->GetButtonStyle() );
  SetDefID( IDC_GOBUTTON );
}
void COpenServiceDlg::makeOkButtonDefault()
{
  CButton * okButton = (CButton*) GetDlgItem(IDOK);
  CButton * goButton = (CButton*) GetDlgItem(IDC_GOBUTTON);
  goButton->SetButtonStyle( ~BS_DEFPUSHBUTTON & goButton->GetButtonStyle() );
  okButton->SetButtonStyle( BS_DEFPUSHBUTTON | okButton->GetButtonStyle() );
  SetDefID( IDOK );
}

void COpenServiceDlg::OnBnClickedOk()
{
  //We first need to apply all of the layer visibility settings
  //then release the event to allow the wms data to continue its
  //load
  CWnd* okButton = GetDlgItem(IDOK);
  okButton->EnableWindow( false );

  (*m_rootLayer)->setVisibility(false);
  setAllChildLayersToBeNonVisible(*m_rootLayer);

  CListBox * layerList = (CListBox *) GetDlgItem(IDC_LAYERORDER);
  if ( *m_rootLayer )
  {
    int noOfVisibleLayers = layerList->GetCount();
    for ( int i = 0; i < noOfVisibleLayers; ++i )
    {
      TSLWMSServiceLayer* associatedLayer = (TSLWMSServiceLayer*)layerList->GetItemData(i);
      //leave the visibility order as the default -1, this will make it the next
      //most visible layer
      associatedLayer->setVisibility(true); 
    }
  }

  CComboBox* addressCombo = (CComboBox*) GetDlgItem(IDC_ADDRESS_COMBO);
  CWinApp * app = AfxGetApp();
  CComboBox* fmCombo = (CComboBox*) GetDlgItem(IDC_FM_ADDRESS_COMBO);
  ASSERT( addressCombo && fmCombo && app );

  TCHAR buffer[256];
  CString rfl;
  int noOfComboBoxEntries = addressCombo->GetCount();
  for ( int i = 0; i < noOfComboBoxEntries && i < NUMBER_OF_MRU_ADDRESSES; ++i )
  {
    addressCombo->GetLBText(i, rfl);
    _stprintf(buffer, _T("File%d"), i);
    app->WriteProfileString(MRU_URL_APPSETTING, buffer, rfl);
  }

  noOfComboBoxEntries = fmCombo->GetCount();
  for ( int i = 0; i < noOfComboBoxEntries && i < NUMBER_OF_MRU_ADDRESSES; ++i )
  {
    fmCombo->GetLBText(i, rfl);
    _stprintf(buffer, _T("File%d"), i);
    app->WriteProfileString(MRU_FMD_APPSETTING, buffer, rfl);
  }

  // Check whether or not validation is turned on.
  CButton * validateCheck = (CButton *) GetDlgItem(IDC_VALIDATE);
  m_layer->validateGetMapRequest( validateCheck->GetCheck() );

  CButton * dsDefault = (CButton*) GetDlgItem(IDC_DS_DEFAULT);
  if ( dsDefault->GetCheck() == BST_CHECKED )
  {
    if ( m_event )
    {
      m_event->SetEvent();
      m_event = 0;
    }
  }
  else
  {
    OnShowDSDialog((WPARAM)m_event, (LPARAM)m_rootLayer);
  }
}

void COpenServiceDlg::OnBnClickedTopbutton()
{
  CListBox * layerList = (CListBox *) GetDlgItem(IDC_LAYERORDER);
  int currentSel = layerList->GetCurSel();
  if ( currentSel < 1 )
    return;

  TSLWMSServiceLayer* associatedLayer = (TSLWMSServiceLayer*)layerList->GetItemData(currentSel);
  layerList->DeleteString(currentSel);
  layerList->InsertString(0, (LPCTSTR)TSLUTF8Decoder(associatedLayer->name()));
  layerList->SetItemData(0, (DWORD_PTR)associatedLayer);
  layerList->SetCurSel(0);
  OnLbnSelchangeLayerorder();
}

void COpenServiceDlg::OnBnClickedUpbutton()
{
  CListBox * layerList = (CListBox *) GetDlgItem(IDC_LAYERORDER);
  int currentSel = layerList->GetCurSel();
  if ( currentSel < 1 )
    return;

  TSLWMSServiceLayer* associatedLayer = (TSLWMSServiceLayer*)layerList->GetItemData(currentSel);
  layerList->DeleteString(currentSel);
  layerList->InsertString(--currentSel, (LPCTSTR)TSLUTF8Decoder(associatedLayer->name()));
  layerList->SetItemData(currentSel, (DWORD_PTR)associatedLayer);
  layerList->SetCurSel(currentSel);
  OnLbnSelchangeLayerorder();
}

void COpenServiceDlg::OnBnClickedDownbutton()
{
  CListBox * layerList = (CListBox *) GetDlgItem(IDC_LAYERORDER);
  int currentSel = layerList->GetCurSel();
  int itemCount = layerList->GetCount();
  if ( currentSel < 0 || currentSel == itemCount-1 )
    return;

  TSLWMSServiceLayer* associatedLayer = (TSLWMSServiceLayer*)layerList->GetItemData(currentSel);
  layerList->DeleteString(currentSel);
  layerList->InsertString(++currentSel, (LPCTSTR)TSLUTF8Decoder(associatedLayer->name()));
  layerList->SetItemData(currentSel, (DWORD_PTR)associatedLayer);
  layerList->SetCurSel(currentSel);
  OnLbnSelchangeLayerorder();
}

void COpenServiceDlg::OnBnClickedBottombutton()
{
  CListBox * layerList = (CListBox *) GetDlgItem(IDC_LAYERORDER);
  int currentSel = layerList->GetCurSel();
  int itemCount = layerList->GetCount();
  if ( currentSel < 0 || currentSel == itemCount-1 )
    return;

  TSLWMSServiceLayer* associatedLayer = (TSLWMSServiceLayer*)layerList->GetItemData(currentSel);
  layerList->DeleteString(currentSel);
  layerList->InsertString(--itemCount, (LPCTSTR)TSLUTF8Decoder(associatedLayer->name()));
  layerList->SetItemData(itemCount, (DWORD_PTR)associatedLayer);
  layerList->SetCurSel(itemCount);
  OnLbnSelchangeLayerorder();
}

BOOL COpenServiceDlg::DestroyWindow()
{
  m_upButton.UnsubclassWindow();
  m_downButton.UnsubclassWindow();
  m_topButton.UnsubclassWindow();
  m_bottomButton.UnsubclassWindow();

  return CDialog::DestroyWindow();
}

void COpenServiceDlg::OnTvnSelchangedLayertree(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
  CListCtrl * layerDetails = (CListCtrl*) GetDlgItem(IDC_LAYER_DETAILS);
  CTreeCtrl * layerTree = (CTreeCtrl *) GetDlgItem(IDC_LAYERTREE);
  layerDetails->DeleteAllItems();

  HTREEITEM newItem = pNMTreeView->itemNew.hItem;
  if ( newItem == 0 )
  {
    layerDetails->EnableWindow(FALSE);
    return;
  }

  layerDetails->EnableWindow();

  TSLWMSServiceLayer* associatedLayer = (TSLWMSServiceLayer*)layerTree->GetItemData(newItem);

  int index = layerDetails->InsertItem(0, _T("Title"));
  layerDetails->SetItemText(index, 1, (LPCTSTR)TSLUTF8Decoder(associatedLayer->title()));

  if ( associatedLayer->abstract() )
  {
    index = layerDetails->InsertItem(index+1, _T("Abstract"));
    layerDetails->SetItemText(index, 1, (LPCTSTR)TSLUTF8Decoder(associatedLayer->abstract()));
  }

  TCHAR buffer[256];
  int noOfItems = associatedLayer->noOfKeywords();
  for ( int i = 0 ; i < noOfItems ; ++i )
  {
    _stprintf(&buffer[0], _T("Keyword %d"), i+1);
    index = layerDetails->InsertItem(index+1, &buffer[0]);
    layerDetails->SetItemText(index, 1, (LPCTSTR)TSLUTF8Decoder(associatedLayer->getKeywordAt(i)));
  }

  noOfItems = associatedLayer->noOfCRSs();
  for ( int i = 0 ; i < noOfItems ; ++i )
  {
    // Attempt to filter out the unsupported CRSs.
    bool addCRS = false;
    const char *ptr = associatedLayer->getCRSAt(i);
    const char *colon = strstr(ptr, ":");
    ostringstream name;
    if (NULL != colon)
    {
      string prefix(ptr, colon);
      if (stricmp(prefix.c_str(), "EPSG") == 0)
      {
        int epsgid;
        if ( (strlen(colon) > 1) && sscanf(colon+1, "%d", &epsgid) == 1)
        {
          // Use the Transverse Mercator Snyder formula instead of JHS formula as the Snyder
          // formula is predominant.
          const TSLCoordinateSystem *coordSys = TSLCoordinateSystem::findByEPSG(epsgid);
          if (coordSys)
          {
            name << " " << coordSys->name();
            addCRS = true;
          }
        }
      }
      else if (stricmp(prefix.c_str(), "CRS") == 0)
      {
        // We do not currently have a simple way to identify which ones
        // we support.
        addCRS = true;
      }
      else
      {
        // Currently we ignore everything else. We could attempt to map other prefixes to EPSG
        // however this is not something that is simple to do.
        addCRS = false;
      }
    }
    if (addCRS)
    {
      _stprintf(&buffer[0], _T("SRS %d"), i+1);
      index = layerDetails->InsertItem(index+1, &buffer[0]);
      string info(associatedLayer->getCRSAt(i));
      info += name.str();
      layerDetails->SetItemText(index, 1, (LPCTSTR)TSLUTF8Decoder(info));
    }
  }

  *pResult = 0;
}

void COpenServiceDlg::OnTvnKeydownLayertree(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMTVKEYDOWN pTVKeyDown = reinterpret_cast<LPNMTVKEYDOWN>(pNMHDR);
  
  //return value is ignored
  *pResult = 0;

  CTreeCtrl * layerTree = (CTreeCtrl *) GetDlgItem(IDC_LAYERTREE);

  HTREEITEM item = layerTree->GetSelectedItem( );
  if ( pTVKeyDown->wVKey != 32 || item == 0 )
  {
    return;
  }

  TSLWMSServiceLayer* associatedLayer = (TSLWMSServiceLayer*)layerTree->GetItemData(item);
  ASSERT(associatedLayer);
  const char * layerName = associatedLayer->name();
  *pResult = 1;
  if ( layerName != 0 )
  {
    UINT image = layerTree->GetItemState(item, TVIS_STATEIMAGEMASK);

    //NOTE: This image is one less than what will appear
    //as the space bar message will be caught later by the tree
    //control which will increment it
    image = image >> 12;

    if ( image != TREE_GREYTICK_IMAGE_IDX )
    {
      if ( image == TREE_TICK_IMAGE_IDX )
      {
        layerTree->SetItemState(item, INDEXTOSTATEIMAGEMASK(0), TVIS_STATEIMAGEMASK);
      }
      CListBox * layerList = (CListBox *) GetDlgItem(IDC_LAYERORDER);
      CButton * okButton = (CButton*)GetDlgItem(IDOK);
      if ( image == TREE_TICK_IMAGE_IDX )
      {
        //unchecked
        int noOfVisibleLayers = layerList->GetCount();
        for ( int i = 0; i < noOfVisibleLayers; ++i )
        {
          if ( layerList->GetItemData(i) == (DWORD_PTR)associatedLayer )
          {
            layerList->DeleteString(i);
            OnLbnSelchangeLayerorder();
            break;
          }
        }
        SetAllChildItemsToOff(layerList, layerTree, item);
        okButton->EnableWindow(layerList->GetCount() > 0);
      }
      else
      {
        //checked
        int index = layerList->InsertString(-1, (LPCTSTR)TSLUTF8Decoder(layerName));
        layerList->SetItemData(index, (DWORD_PTR) associatedLayer);
        SetAllChildItemsToPartial(layerList, layerTree, item);
        OnLbnSelchangeLayerorder();
        okButton->EnableWindow();
      }
      makeOkButtonDefault();
    }
    else
    {
      //The tree control will automatically add one to the state image, therefore
      //set the state image to one less than what we want it to be.
      layerTree->SetItemState(item, INDEXTOSTATEIMAGEMASK(TREE_TICK_IMAGE_IDX), TVIS_STATEIMAGEMASK);
    }
  }
  else
  {
    //The tree control will automatically add one to the state image, therefore
    //set the state image to one less than what we want it to be.
    layerTree->SetItemState(item, INDEXTOSTATEIMAGEMASK(TREE_GREYTICK_IMAGE_IDX), TVIS_STATEIMAGEMASK);
  }
  
}

void COpenServiceDlg::OnBnClickedEcpCheck()
{
  CComboBox* fmCombo = (CComboBox*) GetDlgItem(IDC_FM_ADDRESS_COMBO);
  ASSERT( fmCombo );

  CButton* ecpCheckbox = (CButton*)GetDlgItem(IDC_ECP_CHECK);
  ASSERT(ecpCheckbox);

  bool checked = ecpCheckbox->GetCheck() == BST_CHECKED;
  fmCombo->EnableWindow(checked);
}
