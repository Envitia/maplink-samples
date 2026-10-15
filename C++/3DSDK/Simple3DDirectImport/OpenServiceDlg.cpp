////////////////////////////////////////////////////////
// Copyright (c) 1998 to 2017 by Envitia Group PLC
////////////////////////////////////////////////////////
// OpenServiceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WMSSettingsCallbacks.h"
#include "OpenServiceDlg.h"
#include "StyleDimensionsDlg.h"
#include <string>
#include <sstream>

#define TREE_UNTICK_IMAGE_IDX 1
#define TREE_TICK_IMAGE_IDX 2
#define TREE_GREYTICK_IMAGE_IDX 3
#define TREE_UNNAMED_IMAGE_IDX 4

BOOL CExtentedBitmapButton::SetBitmaps(HBITMAP pBitmap,
                                        HBITMAP pBitmapSel,
		                        HBITMAP pBitmapFocus,
		                        HBITMAP pBitmapDisabled)
{
  // delete old bitmaps (if present)
  m_bitmap.DeleteObject();
  m_bitmapSel.DeleteObject();
  m_bitmapFocus.DeleteObject();
  m_bitmapDisabled.DeleteObject();
  m_bitmap.m_hObject = pBitmap;
  m_bitmapSel.m_hObject = pBitmapSel;
  m_bitmapFocus.m_hObject = pBitmapFocus;
  m_bitmapDisabled.m_hObject = pBitmapDisabled;

  return TRUE;
}

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
END_MESSAGE_MAP()

// COpenServiceDlg message handlers

void COpenServiceDlg::OnBnClickedGobutton()
{
  CComboBox* addressCombo = (CComboBox*) GetDlgItem(IDC_ADDRESS_COMBO);
  ASSERT( addressCombo && m_layer );

  CString content;
  addressCombo->GetWindowText(content);

  if ( content.IsEmpty() )
  {
    AfxMessageBox(_T("Error: Please provide a valid address"));
    return;
  }

  //Add to the drop down list , but first check it doesn't match an existing entry
  int foundindex = addressCombo->FindStringExact(0, content);
  if ( foundindex != -1 )
  {
    addressCombo->DeleteString(foundindex);
  }
  addressCombo->InsertString(0, content);

  CButton * goButton = (CButton*) GetDlgItem(IDC_GOBUTTON);
  goButton->EnableWindow(false);
  addressCombo->EnableWindow(false);
  if ( !m_layer->loadData(TSLUTF8Encoder(content)) )
  {
    addressCombo->DeleteString(0);
    addressCombo->SetWindowText(_T(""));
    addressCombo->EnableWindow(true);
    goButton->EnableWindow(true);
  }
}

BOOL COpenServiceDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  CWinApp * app = AfxGetApp();
  ASSERT(app);

  /*
  IDR_MAINFRAME MENU 

        MENUITEM "Recent File",                 ID_FILE_MRU_FILE1, GRAYED
        MENUITEM SEPARATOR

  */
  CString mruf1 = app->GetProfileString(_T("Recent File List"), _T("File1"));
  CString mruf2 = app->GetProfileString(_T("Recent File List"), _T("File2"));
  CString mruf3 = app->GetProfileString(_T("Recent File List"), _T("File3"));
  CString mruf4 = app->GetProfileString(_T("Recent File List"), _T("File4"));

  CComboBox* addressCombo = (CComboBox*) GetDlgItem(IDC_ADDRESS_COMBO);
  ASSERT( addressCombo );

  if ( !mruf1.IsEmpty() ) addressCombo->AddString(mruf1);
  if ( !mruf2.IsEmpty() ) addressCombo->AddString(mruf2);
  if ( !mruf3.IsEmpty() ) addressCombo->AddString(mruf3);
  if ( !mruf4.IsEmpty() ) addressCombo->AddString(mruf4);

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
  layerDetails->InsertColumn(0, _T("Name"),  0, (int)floor(0.5 * width));
  layerDetails->InsertColumn(1, _T("Value"), 0, (int)floor(0.5 * width));

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

LRESULT COpenServiceDlg::OnCapabilitiesLoadFail(WPARAM wparam, LPARAM lparam)
{
  TSLWMSServiceSettingsCallbacks::CapabilitiesLoadFailureReason reason = (TSLWMSServiceSettingsCallbacks::CapabilitiesLoadFailureReason) wparam;

  switch (reason)
  {
  case TSLWMSServiceSettingsCallbacks::LoadingFailedInternetSettings:
    AfxMessageBox(_T("Error: Internet settings invalid"));
    break;
  case TSLWMSServiceSettingsCallbacks::LoadingFailedLoadFailed:
    AfxMessageBox(_T("Error: Failed to access service"));
    break;
  case TSLWMSServiceSettingsCallbacks::LoadingFailedParseFailure:
    AfxMessageBox(_T("Error: Failed to parser service metadata"));
    break;
  case TSLWMSServiceSettingsCallbacks::LoadingFailedServiceUnsupported:
    AfxMessageBox(_T("Error: Service not supported"));
    break;
  case TSLWMSServiceSettingsCallbacks::LoadingFailedUnableToDetermineExtent:
    AfxMessageBox(_T("Error: Unable to determine layer extent. Service not supported"));
  }

  CTreeCtrl * layerTree = (CTreeCtrl *) GetDlgItem(IDC_LAYERTREE);
  ASSERT( layerTree );
  layerTree->DeleteAllItems();
  m_upButton.EnableWindow(FALSE);
  m_downButton.EnableWindow(FALSE);
  m_topButton.EnableWindow(FALSE);
  m_bottomButton.EnableWindow(FALSE);

  CListBox * layerList = (CListBox *) GetDlgItem(IDC_LAYERORDER);
  layerList->ResetContent();

  CListCtrl * layerDetails = (CListCtrl*) GetDlgItem(IDC_LAYER_DETAILS);
  layerDetails->DeleteAllItems();

  CComboBox* addressCombo = (CComboBox*) GetDlgItem(IDC_ADDRESS_COMBO);
  ASSERT( addressCombo );
  TCHAR buffer[1024];
  addressCombo->GetWindowText(buffer, 1024);
  int foundindex = addressCombo->FindStringExact(0, buffer);
  if ( foundindex != -1 )
  {
    addressCombo->DeleteString(foundindex);
  }

  addressCombo->SetWindowText(_T(""));
  addressCombo->EnableWindow();
  CButton * goButton = (CButton*) GetDlgItem(IDC_GOBUTTON);
  ASSERT( goButton );
  goButton->EnableWindow();

  CButton * okButton = (CButton*)GetDlgItem(IDOK);
  ASSERT( okButton );
  okButton->EnableWindow(FALSE);

  return 0;
}

LRESULT COpenServiceDlg::OnCapabilitiesComplete(WPARAM wparam, LPARAM lparam)
{
  OnOK();
  return 0;
}

LRESULT COpenServiceDlg::OnNoCommonCRS(WPARAM wparam, LPARAM lparam)
{
  m_event = (CEvent*) wparam;
  m_rootLayer = (TSLWMSServiceLayer**)lparam;

  AfxMessageBox(_T("The visible layers do not share a commonly supported CRS/SRS.\nPlease modify the visible layers before continuing"), MB_OK);

  return 0;
}

LRESULT COpenServiceDlg::OnNoSupportedCRS(WPARAM wparam, LPARAM lparam)
{
  m_event = (CEvent*) wparam;
  m_rootLayer = (TSLWMSServiceLayer**)lparam;

  AfxMessageBox(_T("Of the visible layers, although there is a common CRS/SRS, MapLink does not support it.\nPlease modify the visible layers before continuing"), MB_OK);

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
  TSLUTF8Decoder layerNameUTF8( layer->name() );
  CString layerName = (LPCTSTR)layerNameUTF8;
  HTREEITEM newItem = 0;
  if ( !layerName.IsEmpty() )
  {
    newItem = tree->InsertItem(TVIF_TEXT | TVIF_PARAM, layerName, 0, 0, 0, 0, 
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

void COpenServiceDlg::OnBnClickedOk()
{
  CWnd* okButton = GetDlgItem(IDOK);
  okButton->EnableWindow( false );

  //We first need to apply all of the layer visibility settings
  //then release the event to allow the wms data to continue its
  //load
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
  ASSERT( addressCombo );

  CString rfl[4];
  int noOfComboBoxEntries = addressCombo->GetCount();
  for ( int i = 0; i < noOfComboBoxEntries && i < 4; ++i )
  {
    addressCombo->GetLBText(i, rfl[i]);
  }

  // Check whether or not validation is turned on.
  CButton * validateCheck = (CButton *) GetDlgItem(IDC_VALIDATE);
  m_layer->validateGetMapRequest( validateCheck->GetCheck() );

  CWinApp * app = AfxGetApp();
  ASSERT(app);

  app->WriteProfileString(_T("Recent File List"), _T("File1"), rfl[0]);
  app->WriteProfileString(_T("Recent File List"), _T("File2"), rfl[1]);
  app->WriteProfileString(_T("Recent File List"), _T("File3"), rfl[2]);
  app->WriteProfileString(_T("Recent File List"), _T("File4"), rfl[3]);

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
          const TSLCoordinateSystem *coordSys = TSLCoordinateSystem::findByEPSG(epsgid);
          if (coordSys != NULL)
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
