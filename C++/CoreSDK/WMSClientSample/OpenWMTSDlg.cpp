// OpenServiceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WMSClientSample.h"
#include "WMTSSettingsCallbacks.h"
#include "OpenWMTSDlg.h"
#include "StyleDimensionsDlg.h"
#include "CRSDlg.h"
#include "mainfrm.h"
#include <string>
#include <sstream>

#define TREE_UNTICK_IMAGE_IDX   1
#define TREE_TICK_IMAGE_IDX     2
#define TREE_GREYTICK_IMAGE_IDX 3
#define TREE_UNNAMED_IMAGE_IDX  4

#define LIST_UNTICK_IMAGE_IDX   2
#define LIST_TICK_IMAGE_IDX     3
#define LIST_GREY_IMAGE_IDX     5

#define NUMBER_OF_MRU_ADDRESSES 16

// COpenWMTSDlg dialog

IMPLEMENT_DYNAMIC(COpenWMTSDlg, CDialog)

COpenWMTSDlg::COpenWMTSDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COpenWMTSDlg::IDD, pParent)
        , m_layer(0)
        , m_event(0)
        , m_serviceInfo(0)
        , m_layerDetailsSource(0)
        , m_stopDetectingWmtsLayerDetailsChanges(false)
{
}

COpenWMTSDlg::~COpenWMTSDlg()
{
}

void COpenWMTSDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_WMTS_ADDRESS_COMBO, m_addressCombo);
  DDX_Control(pDX, IDC_WMTS_GOBUTTON, m_goButton);
  DDX_Control(pDX, IDC_WMTS_LAYER_DETAILS, m_layerDetails);
  DDX_Control(pDX, IDC_WMTS_LAYERORDER, m_layerOrderList);
  DDX_Control(pDX, IDC_WMTS_LAYERTREE, m_layerTree);
  DDX_Control(pDX, IDOK, m_okButton);
  DDX_Control(pDX, IDC_WMTS_LAYER_ABSTRACT, m_layerAbstract);
  DDX_Control(pDX, IDC_WMTS_LAYER_TITLE, m_layerTitle);
  DDX_Control(pDX, IDC_WMTS_LAYERDETAILS_GROUP, m_layerDetailsGroup);
  DDX_Control(pDX, IDC_WMTS_SHOW_DS, m_showDS);
  DDX_Control(pDX, IDC_WMTS_EXTRACT_PREFIX_PARAMS, m_extractPrefixParams);
}

BEGIN_MESSAGE_MAP(COpenWMTSDlg, CDialog)
  ON_BN_CLICKED(IDC_WMTS_GOBUTTON, &COpenWMTSDlg::OnBnClickedGobutton)
  ON_MESSAGE(WM_WMTS_CAPABILITIES_LOADED, OnCapabilitiesLoaded)
  ON_MESSAGE(WM_WMTS_CAPABILITIES_LOAD_FAIL, OnCapabilitiesLoadFail)
  ON_MESSAGE(WM_WMTS_CAPABILITIES_COMPLETE, OnCapabilitiesComplete)
  ON_MESSAGE(WM_WMTS_NO_COMMON_CRS, OnNoCommonCRS)
  ON_MESSAGE(WM_WMTS_NO_SUPPORTED_CRS, OnNoSupportedCRS)
  ON_BN_CLICKED(IDCANCEL, &COpenWMTSDlg::OnBnClickedCancel)
  ON_NOTIFY(NM_CLICK, IDC_WMTS_LAYERTREE, &COpenWMTSDlg::OnNMClickLayertree)
  ON_LBN_SELCHANGE(IDC_WMTS_LAYERORDER, &COpenWMTSDlg::OnLbnSelchangeLayerorder)
  ON_BN_CLICKED(IDOK, &COpenWMTSDlg::OnBnClickedOk)
  ON_BN_CLICKED(IDC_WMTS_TOPBUTTON, &COpenWMTSDlg::OnBnClickedTopbutton)
  ON_BN_CLICKED(IDC_WMTS_UPBUTTON, &COpenWMTSDlg::OnBnClickedUpbutton)
  ON_BN_CLICKED(IDC_WMTS_DOWNBUTTON, &COpenWMTSDlg::OnBnClickedDownbutton)
  ON_BN_CLICKED(IDC_WMTS_BOTTOMBUTTON, &COpenWMTSDlg::OnBnClickedBottombutton)
  ON_NOTIFY(TVN_SELCHANGED, IDC_WMTS_LAYERTREE, &COpenWMTSDlg::OnTvnSelchangedLayertree)
  ON_NOTIFY(TVN_KEYDOWN, IDC_WMTS_LAYERTREE, &COpenWMTSDlg::OnTvnKeydownLayertree)
  ON_NOTIFY(LVN_ITEMCHANGED, IDC_WMTS_LAYER_DETAILS, &COpenWMTSDlg::OnLvnItemchangedWmtsLayerDetails)
  ON_MESSAGE(WM_WMTS_CRS_CHOICE, &COpenWMTSDlg::OnCrsChoice)
END_MESSAGE_MAP()

// COpenWMTSDlg message handlers

void COpenWMTSDlg::OnBnClickedGobutton()
{
  CString content;
  m_addressCombo.GetWindowText(content);

  if ( content.IsEmpty() )
  {
    AfxMessageBox(_T("Error: Please provide a valid address"));
    return;
  }

  //Add to the drop down list , but first check it doesn't match an existing entry
  int foundindex = m_addressCombo.FindStringExact(0, content);
  if ( foundindex != -1 )
  {
    m_addressCombo.DeleteString(foundindex);
  }
  m_addressCombo.InsertString(0, content);
  m_addressCombo.SelectString(-1, content);

  m_goButton.EnableWindow(false);
  makeOkButtonDefault();
  m_addressCombo.EnableWindow(false);

  TSLErrorStack::clear();
  TSLUTF8Encoder contentUTF8( content );
  if ( m_layer->loadData(contentUTF8) )
  {
    m_url = contentUTF8;
    return;
  }

  const char* err = TSLErrorStack::errorString();
  if ( err )
  {
    AfxMessageBox((LPCTSTR)TSLUTF8Decoder(err));
  }

  m_addressCombo.SetWindowText(_T(""));
  m_addressCombo.EnableWindow(true);
  m_goButton.EnableWindow(true);
  makeGoButtonDefault();
}

BOOL COpenWMTSDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  CWinApp * app = AfxGetApp();
  ASSERT(app);

  TCHAR buffer[1024];
  for ( unsigned int i = 0; i < NUMBER_OF_MRU_ADDRESSES; ++i )
  {
    _stprintf(buffer, _T("File%u"), i);
    CString mruf = app->GetProfileString(MRU_URL_APPSETTING, buffer);
    if ( !mruf.IsEmpty() ) m_addressCombo.AddString(mruf);
  }

  m_treeCheckboxes.Create(IDB_CHECKBOXES, 16, 4, RGB(255,0,255));

  m_layerTree.SetImageList(&m_treeCheckboxes, TVSIL_STATE);


  HMODULE thisMod = ::GetModuleHandle(0);
  m_upButton.SubclassDlgItem(IDC_WMTS_UPBUTTON, this);
  m_downButton.SubclassDlgItem(IDC_WMTS_DOWNBUTTON, this);
  m_topButton.SubclassDlgItem(IDC_WMTS_TOPBUTTON, this);
  m_bottomButton.SubclassDlgItem(IDC_WMTS_BOTTOMBUTTON, this);
  m_upButton.SetBitmaps( (HBITMAP)::LoadImage(thisMod, MAKEINTRESOURCE(IDB_UP), IMAGE_BITMAP, 16,16, LR_LOADTRANSPARENT|LR_LOADMAP3DCOLORS), 0, 0,
                         (HBITMAP)::LoadImage(thisMod, MAKEINTRESOURCE(IDB_UPD), IMAGE_BITMAP, 16,16, LR_LOADTRANSPARENT|LR_LOADMAP3DCOLORS));
  m_downButton.SetBitmaps( (HBITMAP)::LoadImage(thisMod, MAKEINTRESOURCE(IDB_DOWN), IMAGE_BITMAP, 16,16, LR_LOADTRANSPARENT|LR_LOADMAP3DCOLORS), 0, 0,
                            (HBITMAP)::LoadImage(thisMod, MAKEINTRESOURCE(IDB_DOWND), IMAGE_BITMAP, 16,16, LR_LOADTRANSPARENT|LR_LOADMAP3DCOLORS));
  m_topButton.SetBitmaps( (HBITMAP)::LoadImage(thisMod, MAKEINTRESOURCE(IDB_TOP), IMAGE_BITMAP, 16,16, LR_LOADTRANSPARENT|LR_LOADMAP3DCOLORS), 0, 0,
                          (HBITMAP)::LoadImage(thisMod, MAKEINTRESOURCE(IDB_TOPD), IMAGE_BITMAP, 16,16, LR_LOADTRANSPARENT|LR_LOADMAP3DCOLORS));
  m_bottomButton.SetBitmaps( (HBITMAP)::LoadImage(thisMod, MAKEINTRESOURCE(IDB_BOTTOM), IMAGE_BITMAP, 16,16, LR_LOADTRANSPARENT|LR_LOADMAP3DCOLORS), 0, 0,
                             (HBITMAP)::LoadImage(thisMod, MAKEINTRESOURCE(IDB_BOTTOMD), IMAGE_BITMAP, 16,16, LR_LOADTRANSPARENT|LR_LOADMAP3DCOLORS));


  m_showDS.SetCheck(BST_UNCHECKED);
  m_extractPrefixParams.SetCheck(BST_CHECKED);

  m_layerDetailsCheckboxes.Create(IDB_CHECKBOXES, 16, 4, RGB(255,0,255));

  m_layerDetails.SetExtendedStyle( LVS_EX_CHECKBOXES | m_layerDetails.GetExtendedStyle() );
  m_layerDetails.SetImageList(&m_layerDetailsCheckboxes, LVSIL_STATE);

  CRect layerDetailsHeaderRect; 
  m_layerDetails.GetHeaderCtrl()->GetWindowRect(&layerDetailsHeaderRect);
  int width = layerDetailsHeaderRect.Width();
  m_layerDetails.InsertColumn(0, _T("Tile Matrix Set"),  0, (int)floor(0.4 * width));
  m_layerDetails.InsertColumn(1, _T("CRS"), 0, (int)floor(0.6 * width));

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT COpenWMTSDlg::OnCapabilitiesLoaded(WPARAM wparam, LPARAM lparam)
{
  m_event = (CEvent*) wparam;
  //We can hold on to the layer through the callback loop as it
  //will be the same instance passed each time. We mustn't hold on
  //to it beyond the callback loop however
  m_serviceInfo = (TSLWMTSServiceInfo**)lparam;

  ASSERT(m_event && m_serviceInfo);

  HTREEITEM root = AddLayersToTree(&m_layerTree, 0, 0, *m_serviceInfo);
  if ( root )
  {
    m_layerTree.Expand(root, TVE_EXPAND);
  }

  m_layerTree.EnableWindow();

  m_layerOrderList.EnableWindow();
  return 0;
}

static void getErrorString(TSLSimpleString& err, const char *hdr)
{
  if ( !TSLThreadedErrorStack::errorString(err, hdr) )
  {
    err = hdr;
  }
}

LRESULT COpenWMTSDlg::OnCapabilitiesLoadFail(WPARAM wparam, LPARAM lparam)
{
  TSLWMTSServiceSettingsCallbacks::CapabilitiesLoadFailureReason reason = (TSLWMTSServiceSettingsCallbacks::CapabilitiesLoadFailureReason) wparam;

  TSLSimpleString err ;

  switch (reason)
  {
  case TSLWMTSServiceSettingsCallbacks::LoadingFailedInternetSettings:
    getErrorString( err, "Error: Internet settings invalid\n");
    break;
  case TSLWMTSServiceSettingsCallbacks::LoadingFailedLoadFailed:
    getErrorString( err, "Error: Failed to access service\n");
    break;
  case TSLWMTSServiceSettingsCallbacks::LoadingFailedParseFailure:
    getErrorString( err, "Error: Failed to parse service metadata\n");
    break;
  case TSLWMTSServiceSettingsCallbacks::LoadingFailedServiceUnsupported:
    getErrorString( err, "Error: Service not supported\n");
    break;
  case TSLWMTSServiceSettingsCallbacks::LoadingFailedUnableToDetermineExtent:
    getErrorString( err, "Error: Unable to determine layer extent. Service not supported\n");
    break;
  default:
    break;
  }

  if ( !err.empty() )
  {
    AfxMessageBox( (LPCTSTR)TSLUTF8Decoder(err)) ;
  }

  resetDialogControls();
  return 0;
}

void COpenWMTSDlg::resetDialogControls()
{
  m_layerTree.DeleteAllItems();
  m_upButton.EnableWindow(FALSE);
  m_downButton.EnableWindow(FALSE);
  m_topButton.EnableWindow(FALSE);
  m_bottomButton.EnableWindow(FALSE);

  m_layerOrderList.ResetContent();

  m_layerDetails.DeleteAllItems();
  m_layerDetailsSource = 0;
  m_layerDetailsGroup.SetWindowText(_T("Layer Details"));

  ASSERT( m_layer );

  m_addressCombo.SetWindowText(_T(""));
  m_addressCombo.EnableWindow();

  m_goButton.EnableWindow();
  m_okButton.EnableWindow(FALSE);
  makeGoButtonDefault();
}

LRESULT COpenWMTSDlg::OnCapabilitiesComplete(WPARAM wparam, LPARAM lparam)
{
  CMainFrame* mainWindow = (CMainFrame*)::AfxGetMainWnd();
  ASSERT(mainWindow);

  CString content;
  m_addressCombo.GetWindowText(content);
  const CString& title = mainWindow->GetActiveDocument()->GetTitle();
  if (title != "Untitled")
  {
    content = content + _T(", ") + title;
  }
  mainWindow->GetActiveDocument()->SetTitle(content);

  OnOK();
  return 0;
}

/*LRESULT COpenWMTSDlg::OnInvalidUserLinearTransform(WPARAM wparam, LPARAM lparam)
{
  AfxMessageBox( "Bad coordinate system.\n\n"
    "This is probably due to the background map or rendition maps using a "
    "larger TMC/MU value (i.e. covering a smaller area) than the server.", MB_OK );
  return 0;
}*/

LRESULT COpenWMTSDlg::OnNoCommonCRS(WPARAM wparam, LPARAM lparam)
{
  m_event = (CEvent*) wparam;
  m_serviceInfo = (TSLWMTSServiceInfo**)lparam;

  AfxMessageBox(_T("The visible layers do not share a commonly supported CRS/SRS.\nPlease modify the visible layers before continuing"), MB_OK);

  m_okButton.EnableWindow();

  return 0;
}

LRESULT COpenWMTSDlg::OnNoSupportedCRS(WPARAM wparam, LPARAM lparam)
{
  m_event = (CEvent*) wparam;
  m_serviceInfo = (TSLWMTSServiceInfo**)lparam;

  AfxMessageBox(_T("Of the visible layers, although there is a common CRS/SRS, MapLink does not support it.\nPlease modify the visible layers before continuing"), MB_OK);

  m_okButton.EnableWindow();

  return 0;
}

void COpenWMTSDlg::OnBnClickedCancel()
{
  if ( m_event )
  {
    if ( m_serviceInfo )
    {
      *m_serviceInfo = 0;
    }
    m_event->SetEvent();
    m_event = 0;
  }
  OnCancel();
}

HTREEITEM COpenWMTSDlg::AddLayersToTree( CTreeCtrl * tree, HTREEITEM parent, HTREEITEM after, TSLWMTSServiceInfo* serviceInfo)
{
  HTREEITEM rootItem = 0;

  int numLayers = serviceInfo->numLayers();
  for( int i = 0; i < numLayers; ++i )
  {
    TSLWMTSServiceLayer *layer = serviceInfo->getLayerAt(i);
    const char * layerName = layer->identifier();
    HTREEITEM newItem = 0;
    if ( layerName )
    {
      newItem = tree->InsertItem(TVIF_TEXT | TVIF_PARAM, (LPCTSTR)TSLUTF8Decoder(layerName), 0, 0, 0, 0, 
                                 (LPARAM)layer, rootItem, after);
    }
    else
    {
      newItem = tree->InsertItem(TVIF_STATE | TVIF_TEXT | TVIF_PARAM, 
                                  _T("[UNNAMED]"), 0, 0, 
                                  INDEXTOSTATEIMAGEMASK(TREE_UNNAMED_IMAGE_IDX), 
                                  TVIS_STATEIMAGEMASK, (LPARAM)layer, rootItem, after);
    }
  }

  return rootItem;
}

void COpenWMTSDlg::OnNMClickLayertree(NMHDR *pNMHDR, LRESULT *pResult)
{
  CPoint pt;
  GetCursorPos(&pt);
  m_layerTree.ScreenToClient(&pt);
  UINT flags = 0;
  HTREEITEM item = m_layerTree.HitTest(pt, &flags);
  if ( !item )
    return;

  *pResult = 0;

  if ( flags & TVHT_ONITEMSTATEICON )
  {
    TSLWMTSServiceLayer* associatedLayer = (TSLWMTSServiceLayer*)m_layerTree.GetItemData(item);
    ASSERT(associatedLayer);
    const char * layerName = associatedLayer->identifier();
    *pResult = 1;
    if ( layerName != 0 )
    {
      UINT image = m_layerTree.GetItemState(item, TVIS_STATEIMAGEMASK);
      image = image >> 12;
      if ( image != TREE_GREYTICK_IMAGE_IDX )
      {
        if ( ++image == TREE_GREYTICK_IMAGE_IDX ) image = TREE_UNTICK_IMAGE_IDX;
        m_layerTree.SetItemState(item, INDEXTOSTATEIMAGEMASK(image), TVIS_STATEIMAGEMASK);
        if ( image == TREE_UNTICK_IMAGE_IDX )
        {
          //unchecked
          int noOfVisibleLayers = m_layerOrderList.GetCount();
          for ( int i = 0; i < noOfVisibleLayers; ++i )
          {
            if ( m_layerOrderList.GetItemData(i) == (DWORD_PTR)associatedLayer )
            {
              m_layerOrderList.DeleteString(i);
              OnLbnSelchangeLayerorder();
              break;
            }
          }
          m_okButton.EnableWindow(m_layerOrderList.GetCount() > 0);
        }
        else
        {
          //checked
          int index = m_layerOrderList.InsertString(-1, (LPCTSTR)TSLUTF8Decoder(layerName));
          m_layerOrderList.SetItemData(index, (DWORD_PTR) associatedLayer);
          OnLbnSelchangeLayerorder();
          m_okButton.EnableWindow();
        }
        makeOkButtonDefault();
      }
    }
  }
}

void COpenWMTSDlg::OnLbnSelchangeLayerorder()
{
  m_upButton.EnableWindow(FALSE);
  m_downButton.EnableWindow(FALSE);
  m_topButton.EnableWindow(FALSE);
  m_bottomButton.EnableWindow(FALSE);

  int selectedIndex = m_layerOrderList.GetCurSel();
  if ( selectedIndex == -1 )
    return;

  int itemCount = m_layerOrderList.GetCount();
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

void COpenWMTSDlg::OnBnClickedOk()
{
  //We first need to apply all of the layer visibility settings
  //then release the event to allow the WMTS data to continue its
  //load
  m_okButton.EnableWindow( false );

  const char* additionalPrefixParameters = NULL;

  if ( m_extractPrefixParams.GetCheck() == BST_CHECKED )
  {
    size_t urlSize = m_url.size();
    size_t q = m_url.find_first_of('?');
    if ( urlSize > 1 && q != string::npos && q < urlSize-1 )
    {
      additionalPrefixParameters = &m_url.c_str()[q+1];
    }
  }

  if ( *m_serviceInfo )
  {
    int noOfVisibleLayers = m_layerOrderList.GetCount();
    for ( int i = 0; i < noOfVisibleLayers; ++i )
    {
      TSLWMTSServiceLayer* associatedLayer = (TSLWMTSServiceLayer*)m_layerOrderList.GetItemData(i);
      //leave the visibility order as the default -1, this will make it the next
      //most visible layer
      associatedLayer->setVisibility(true);

      map<const TSLWMTSServiceLayer*, int>::const_iterator it = m_tileMatrixChoices.find(associatedLayer);
      if (it != m_tileMatrixChoices.end())
      {
        associatedLayer->selectTileMatrixSet(it->second);
      }
      else
      {
        associatedLayer->selectTileMatrixSet(0);
      }

      if (additionalPrefixParameters)
        associatedLayer->setAdditionalPrefixParameters(additionalPrefixParameters);
    }
  }

  CWinApp * app = AfxGetApp();
  ASSERT( app );

  TCHAR buffer[256];
  CString rfl;
  int noOfComboBoxEntries = m_addressCombo.GetCount();
  for ( int i = 0; i < noOfComboBoxEntries && i < NUMBER_OF_MRU_ADDRESSES; ++i )
  {
    m_addressCombo.GetLBText(i, rfl);
    _stprintf(buffer, _T("File%d"), i);
    app->WriteProfileString(MRU_URL_APPSETTING, buffer, rfl);
  }

  if ( m_showDS.GetCheck() != BST_CHECKED )
  {
    if ( m_event )
    {
      m_event->SetEvent();
      m_event = 0;
    }
  }
  else
  {
    INT_PTR dlgResult = 0;
    if ( m_serviceInfo )
    {
      CStyleDimensionsDlg dlg(this, *m_serviceInfo);
      dlgResult = dlg.DoModal();
    }

    if ( m_event )
    {
      if ( m_serviceInfo && dlgResult != IDOK)
      {
        *m_serviceInfo = 0;
      }
      m_event->SetEvent();
      m_event = 0;
    }

    if ( dlgResult != IDOK )
    {
      OnCancel();
    }
  }
}

void COpenWMTSDlg::OnBnClickedTopbutton()
{
  int currentSel = m_layerOrderList.GetCurSel();
  if ( currentSel < 1 )
    return;

  TSLWMTSServiceLayer* associatedLayer = (TSLWMTSServiceLayer*)m_layerOrderList.GetItemData(currentSel);
  m_layerOrderList.DeleteString(currentSel);
  m_layerOrderList.InsertString(0, (LPCTSTR)TSLUTF8Decoder(associatedLayer->identifier()));
  m_layerOrderList.SetItemData(0, (DWORD_PTR)associatedLayer);
  m_layerOrderList.SetCurSel(0);
  OnLbnSelchangeLayerorder();
}

void COpenWMTSDlg::OnBnClickedUpbutton()
{
  int currentSel = m_layerOrderList.GetCurSel();
  if ( currentSel < 1 )
    return;

  TSLWMTSServiceLayer* associatedLayer = (TSLWMTSServiceLayer*)m_layerOrderList.GetItemData(currentSel);
  m_layerOrderList.DeleteString(currentSel);
  m_layerOrderList.InsertString(--currentSel, (LPCTSTR)TSLUTF8Decoder(associatedLayer->identifier()));
  m_layerOrderList.SetItemData(currentSel, (DWORD_PTR)associatedLayer);
  m_layerOrderList.SetCurSel(currentSel);
  OnLbnSelchangeLayerorder();
}

void COpenWMTSDlg::OnBnClickedDownbutton()
{
  int currentSel = m_layerOrderList.GetCurSel();
  int itemCount = m_layerOrderList.GetCount();
  if ( currentSel < 0 || currentSel == itemCount-1 )
    return;

  TSLWMTSServiceLayer* associatedLayer = (TSLWMTSServiceLayer*)m_layerOrderList.GetItemData(currentSel);
  m_layerOrderList.DeleteString(currentSel);
  m_layerOrderList.InsertString(++currentSel, (LPCTSTR)TSLUTF8Decoder(associatedLayer->identifier()));
  m_layerOrderList.SetItemData(currentSel, (DWORD_PTR)associatedLayer);
  m_layerOrderList.SetCurSel(currentSel);
  OnLbnSelchangeLayerorder();
}

void COpenWMTSDlg::OnBnClickedBottombutton()
{
  int currentSel = m_layerOrderList.GetCurSel();
  int itemCount = m_layerOrderList.GetCount();
  if ( currentSel < 0 || currentSel == itemCount-1 )
    return;

  TSLWMTSServiceLayer* associatedLayer = (TSLWMTSServiceLayer*)m_layerOrderList.GetItemData(currentSel);
  m_layerOrderList.DeleteString(currentSel);
  m_layerOrderList.InsertString(--itemCount, (LPCTSTR)TSLUTF8Decoder(associatedLayer->identifier()));
  m_layerOrderList.SetItemData(itemCount, (DWORD_PTR)associatedLayer);
  m_layerOrderList.SetCurSel(itemCount);
  OnLbnSelchangeLayerorder();
}

BOOL COpenWMTSDlg::DestroyWindow()
{
  m_upButton.UnsubclassWindow();
  m_downButton.UnsubclassWindow();
  m_topButton.UnsubclassWindow();
  m_bottomButton.UnsubclassWindow();

  return CDialog::DestroyWindow();
}

void COpenWMTSDlg::OnTvnSelchangedLayertree(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
  m_layerDetails.DeleteAllItems();
  m_layerDetailsSource = 0;
  m_layerDetailsGroup.SetWindowText(_T("Layer Details"));

  HTREEITEM newItem = pNMTreeView->itemNew.hItem;
  if ( newItem == 0 )
  {
    m_layerDetails.EnableWindow(FALSE);
    return;
  }

  m_layerDetails.EnableWindow();

  TSLWMTSServiceLayer* associatedLayer = (TSLWMTSServiceLayer*)m_layerTree.GetItemData(newItem);

  if( associatedLayer )
  {
    m_layerDetailsSource = associatedLayer;
    char layerDetailsGroupText[256];
    sprintf_s(layerDetailsGroupText, "Layer Details - %.200s", associatedLayer->identifier());
    m_layerDetailsGroup.SetWindowText((LPCTSTR)TSLUTF8Decoder(layerDetailsGroupText));

    m_stopDetectingWmtsLayerDetailsChanges = true;

    int noOfItems = associatedLayer->numTileMatrixSets();
    for ( int i = 0 ; i < noOfItems ; ++i )
    {
      const TSLWMTSServiceTileMatrixSet* mxset = associatedLayer->getTileMatrixSetAt(i);
      m_layerDetails.InsertItem(i, (LPCTSTR)TSLUTF8Decoder(mxset->identifier()));
      m_layerDetails.SetItemText(i, 1, (LPCTSTR)TSLUTF8Decoder(mxset->supportedCRS()));
      m_layerDetails.SetItemState(i, INDEXTOSTATEIMAGEMASK(LIST_UNTICK_IMAGE_IDX), LVIS_STATEIMAGEMASK);
    }

    map<const TSLWMTSServiceLayer*, int>::const_iterator it = m_tileMatrixChoices.find(m_layerDetailsSource);
    if (it != m_tileMatrixChoices.end())
    {
      m_layerDetails.SetItemState(it->second, INDEXTOSTATEIMAGEMASK(LIST_TICK_IMAGE_IDX), LVIS_STATEIMAGEMASK);
    }

    m_stopDetectingWmtsLayerDetailsChanges = false;

    if (associatedLayer->numAbstracts() > 0)
    {
      m_layerAbstract.SetWindowText((LPCTSTR)TSLUTF8Decoder(associatedLayer->getAbstractAt(0)));
    }
    else
    {
      m_layerAbstract.SetWindowText(_T("(none)"));
    }

    if (associatedLayer->numTitles() > 0)
    {
      m_layerTitle.SetWindowText((LPCTSTR)TSLUTF8Decoder(associatedLayer->getTitleAt(0)));
    }
    else
    {
      m_layerTitle.SetWindowText(_T("(none)"));
    }
  }

  *pResult = 0;
}

void COpenWMTSDlg::OnTvnKeydownLayertree(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMTVKEYDOWN pTVKeyDown = reinterpret_cast<LPNMTVKEYDOWN>(pNMHDR);
  
  //return value is ignored
  *pResult = 0;

  HTREEITEM item = m_layerTree.GetSelectedItem( );
  if ( pTVKeyDown->wVKey != ' ' || item == 0 )
  {
    return;
  }

  TSLWMTSServiceLayer* associatedLayer = (TSLWMTSServiceLayer*)m_layerTree.GetItemData(item);
  ASSERT(associatedLayer);
  const char * layerName = associatedLayer->identifier();
  *pResult = 1;
  if ( layerName != 0 )
  {
    UINT image = m_layerTree.GetItemState(item, TVIS_STATEIMAGEMASK);

    //NOTE: This image is one less than what will appear
    //as the space bar message will be caught later by the tree
    //control which will increment it
    image = image >> 12;

    if ( image != TREE_GREYTICK_IMAGE_IDX )
    {
      if ( image == TREE_TICK_IMAGE_IDX )
      {
        m_layerTree.SetItemState(item, INDEXTOSTATEIMAGEMASK(0), TVIS_STATEIMAGEMASK);
      }
      if ( image == TREE_TICK_IMAGE_IDX )
      {
        //unchecked
        int noOfVisibleLayers = m_layerOrderList.GetCount();
        for ( int i = 0; i < noOfVisibleLayers; ++i )
        {
          if ( m_layerOrderList.GetItemData(i) == (DWORD_PTR)associatedLayer )
          {
            m_layerOrderList.DeleteString(i);
            OnLbnSelchangeLayerorder();
            break;
          }
        }
        m_okButton.EnableWindow(m_layerOrderList.GetCount() > 0);
      }
      else
      {
        //checked
        int index = m_layerOrderList.InsertString(-1, (LPCTSTR)TSLUTF8Decoder(layerName));
        m_layerOrderList.SetItemData(index, (DWORD_PTR) associatedLayer);
        OnLbnSelchangeLayerorder();
        m_okButton.EnableWindow();
      }
      makeOkButtonDefault();
    }
    else
    {
      //The tree control will automatically add one to the state image, therefore
      //set the state image to one less than what we want it to be.
      m_layerTree.SetItemState(item, INDEXTOSTATEIMAGEMASK(TREE_TICK_IMAGE_IDX), TVIS_STATEIMAGEMASK);
    }
  }
  else
  {
    //The tree control will automatically add one to the state image, therefore
    //set the state image to one less than what we want it to be.
    m_layerTree.SetItemState(item, INDEXTOSTATEIMAGEMASK(TREE_GREYTICK_IMAGE_IDX), TVIS_STATEIMAGEMASK);
  }
  
}


void COpenWMTSDlg::OnLvnItemchangedWmtsLayerDetails(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

  if (m_stopDetectingWmtsLayerDetailsChanges)
  {
    *pResult = 0;
    return;
  }

  if (m_layerDetailsSource && ((pNMLV->uOldState & TVIS_STATEIMAGEMASK) != (pNMLV->uNewState & TVIS_STATEIMAGEMASK)))
  {
    UINT image = (pNMLV->uNewState & TVIS_STATEIMAGEMASK) >> 12;

    m_stopDetectingWmtsLayerDetailsChanges = true;  // get recursive calls to OnLvnItemchangedWmtsLayerDetails

    map<const TSLWMTSServiceLayer*, int>::iterator it = m_tileMatrixChoices.find(m_layerDetailsSource);
    if (it != m_tileMatrixChoices.end() && it->second != pNMLV->iItem)
    {
      m_layerDetails.SetItemState(it->second, INDEXTOSTATEIMAGEMASK(LIST_UNTICK_IMAGE_IDX), TVIS_STATEIMAGEMASK);
    }

    switch (image)
    {
    default:
      m_layerDetails.SetItemState(pNMLV->iItem, INDEXTOSTATEIMAGEMASK(LIST_UNTICK_IMAGE_IDX), TVIS_STATEIMAGEMASK);
      //fallthough

    case LIST_UNTICK_IMAGE_IDX:
      //became unchecked - remove tile matrix choice assignment
      m_tileMatrixChoices.erase(m_layerDetailsSource);
      break;

    case LIST_TICK_IMAGE_IDX:
      //became checked - add tile matrix choice assignment, removing the old one
      m_tileMatrixChoices[m_layerDetailsSource] = pNMLV->iItem;
      break;
    }

    m_stopDetectingWmtsLayerDetailsChanges = false;
  }

  *pResult = 0;
}



afx_msg LRESULT COpenWMTSDlg::OnCrsChoice(WPARAM wParam, LPARAM lParam)
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