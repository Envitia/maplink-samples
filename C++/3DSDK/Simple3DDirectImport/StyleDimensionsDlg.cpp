////////////////////////////////////////////////////////
// Copyright (c) 1998 to 2017 by Envitia Group PLC
////////////////////////////////////////////////////////
// StyleDimensionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "StyleDimensionsDlg.h"

void SplitStringByChar( const string & str, char splitChar, vector<string>& results )
{
  const char* index,* index2 = str.c_str();
  while( (index = strchr(index2, splitChar) ) != 0 )
  {
    results.push_back( str.substr(index2 - str.c_str(), index - index2) );
    index2 = index+1 ;
  }
  results.push_back( index2 );
}

// CStyleDimensionsDlg dialog

IMPLEMENT_DYNAMIC(CStyleDimensionsDlg, CDialog)

CStyleDimensionsDlg::CStyleDimensionsDlg(CWnd* pParent, TSLWMSServiceLayer* rootLayer)
  : CDialog(CStyleDimensionsDlg::IDD, pParent)
  , m_rootLayer(rootLayer)
{

}

CStyleDimensionsDlg::~CStyleDimensionsDlg()
{
}

void CStyleDimensionsDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_STYLECOMBO, m_listStyleCombo);
}


BEGIN_MESSAGE_MAP(CStyleDimensionsDlg, CDialog)
  ON_NOTIFY(LVN_ITEMCHANGED, IDC_STYLELIST, &CStyleDimensionsDlg::OnLvnItemchangedStylelist)
  ON_NOTIFY(NM_CLICK, IDC_STYLELIST, &CStyleDimensionsDlg::OnNMClickStylelist)
  ON_NOTIFY(NM_KILLFOCUS, IDC_STYLELIST, &CStyleDimensionsDlg::OnNMKillfocusStylelist)
  ON_NOTIFY(NM_SETFOCUS, IDC_STYLELIST, &CStyleDimensionsDlg::OnNMSetfocusStylelist)
  ON_CBN_SELCHANGE(IDC_STYLECOMBO, &CStyleDimensionsDlg::OnCbnSelchangeStylecombo)
  ON_NOTIFY(TVN_SELCHANGED, IDC_DIM_TREE, &CStyleDimensionsDlg::OnTvnSelchangedDimTree)
  ON_BN_CLICKED(IDOK, &CStyleDimensionsDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CStyleDimensionsDlg message handlers

BOOL CStyleDimensionsDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  if ( !m_rootLayer )
    return FALSE;

  map<int, TSLWMSServiceLayer *, less<int> > visibleLayers;
  if ( !findAllVisibleLayers(m_rootLayer, visibleLayers ) )
  {
    return FALSE;
  }

  CTreeCtrl* treeCtrl = (CTreeCtrl*) GetDlgItem(IDC_DIM_TREE);
  CListCtrl* listCtrl = (CListCtrl*) GetDlgItem(IDC_STYLELIST);
  listCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT );
  
  CRect clientRect;
  listCtrl->GetClientRect(&clientRect);
  listCtrl->InsertColumn(0, _T("Layer Name"), 0, (int)floor(((double)clientRect.Width())/2));
  listCtrl->InsertColumn(1, _T("Layer Style"), 0, (int)floor(((double)clientRect.Width())/2));

  map<int, TSLWMSServiceLayer *, less<int> >::iterator it(visibleLayers.begin());
  map<int, TSLWMSServiceLayer *, less<int> >::iterator itEnd(visibleLayers.end());

  int index = 0;
  while ( it != itEnd )
  {
    TSLWMSServiceLayer * layer = it->second;
    listCtrl->InsertItem(index, (LPCTSTR)TSLUTF8Decoder(layer->name()));
    listCtrl->SetItemText(index, 1, _T("[Default]"));
    listCtrl->SetItemData(index, (DWORD_PTR)layer);

    int noOfDimensions = layer->noOfDimensions();
    if ( noOfDimensions )
    {
      HTREEITEM layerRoot = treeCtrl->InsertItem((LPCTSTR)TSLUTF8Decoder(layer->name()));
      treeCtrl->SetItemData(layerRoot, (DWORD_PTR)layer);
      for ( int i = 0; i < noOfDimensions; ++i )
      {
        const TSLWMSServiceLayerDimension *dim = layer->getDimensionAt(i);
        const char * dimName = dim->name();
        HTREEITEM dimItem = treeCtrl->InsertItem((LPCTSTR)TSLUTF8Decoder(dimName), layerRoot);
        treeCtrl->SetItemData(dimItem, (DWORD_PTR)i);
      }
      treeCtrl->Expand(layerRoot, TVE_EXPAND);
    }
  
    ++it;
    ++index;
  }
  return TRUE;
}

bool CStyleDimensionsDlg::findAllVisibleLayers( TSLWMSServiceLayer * layer, map<int, TSLWMSServiceLayer *, less<int> > & visibleLayers )
{
  bool isDerived = false;
  if ( layer->getVisibility(&isDerived) && !isDerived )
  {
    int visibilityOrderIndex = layer->getVisibilityOrderIndex();
    ASSERT(visibilityOrderIndex != -1);
    visibleLayers[visibilityOrderIndex] = layer;
  }

  int noOfSubLayers = layer->noOfSubLayers();
  for ( int i = 0; i < noOfSubLayers; ++i )
  {
    TSLWMSServiceLayer * childLayer = layer->getSubLayerAt(i);
    if ( !findAllVisibleLayers( childLayer, visibleLayers) )
    {
      return false;
    }
  }

  return true;
}

void CStyleDimensionsDlg::OnLvnItemchangedStylelist(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
  *pResult = 0;

  if ( (pNMLV->uChanged & LVIF_STATE) == 0 )
    return ;

  if ( pNMLV->uNewState & LVIS_SELECTED )
  {
    CListCtrl* listCtrl = (CListCtrl*) GetDlgItem(IDC_STYLELIST);
    CRect layerStyleRect;
    listCtrl->GetSubItemRect(pNMLV->iItem, 1, LVIR_BOUNDS, layerStyleRect);
    listCtrl->ClientToScreen(layerStyleRect);
    ScreenToClient(layerStyleRect);
    layerStyleRect.OffsetRect(0, -2);
    m_listStyleCombo.MoveWindow( &layerStyleRect, 0);
    m_listStyleCombo.ResetContent();

    m_listStyleCombo.AddString(_T("[Default]"));
    TSLWMSServiceLayer * layer = (TSLWMSServiceLayer *)listCtrl->GetItemData(pNMLV->iItem);
    int noOfStyles = layer->noOfStyles();

    const char * currentVal = layer->getStyleValue();
    int currentIndex = 0;
    for ( int i = 0; i < noOfStyles; ++i )
    {
      const char * styleName = layer->getStyleAt(i);
      if ( !styleName )
      {
        //This is the default
        continue;
      }
      if ( currentVal && strcmp(currentVal, styleName) == 0 )
      {
        currentIndex = i+1;
      }
      m_listStyleCombo.AddString((LPCTSTR)TSLUTF8Decoder(styleName));
    }
    m_listStyleCombo.SetCurSel(currentIndex);
  }
  if ( !(pNMLV->uNewState & LVIS_SELECTED) )
  {
    m_listStyleCombo.ShowWindow(SW_HIDE );
  }
}

void CStyleDimensionsDlg::OnNMClickStylelist(NMHDR *pNMHDR, LRESULT *pResult)
{
  DWORD pos = ::GetMessagePos();
  CPoint pt(LOWORD(pos), HIWORD(pos));
  CRect comboRect;
  m_listStyleCombo.GetWindowRect(&comboRect);
  if ( comboRect.PtInRect(pt) )
  {
    if ( m_listStyleCombo.IsWindowVisible() )
    {
      m_listStyleCombo.SetActiveWindow();
      m_listStyleCombo.ShowDropDown();
    }
    else
    {
      m_listStyleCombo.ShowWindow(SW_SHOW);
      m_listStyleCombo.SetActiveWindow();
    }
  }
  *pResult = 0;
}

void CStyleDimensionsDlg::OnNMKillfocusStylelist(NMHDR *pNMHDR, LRESULT *pResult)
{
  m_listStyleCombo.ShowWindow(SW_HIDE);
  CListCtrl* listCtrl = (CListCtrl*) GetDlgItem(IDC_STYLELIST);
  listCtrl->RedrawWindow();
  *pResult = 0;
}

void CStyleDimensionsDlg::OnNMSetfocusStylelist(NMHDR *pNMHDR, LRESULT *pResult)
{
  CListCtrl* listCtrl = (CListCtrl*) GetDlgItem(IDC_STYLELIST);
  POSITION pos = listCtrl->GetFirstSelectedItemPosition();
  
  if ( !pos )
  {
    m_listStyleCombo.ShowWindow(SW_HIDE );
    *pResult = 0;
  }
  
}

void CStyleDimensionsDlg::OnCbnSelchangeStylecombo()
{
  CListCtrl* listCtrl = (CListCtrl*) GetDlgItem(IDC_STYLELIST);
  POSITION pos = listCtrl->GetFirstSelectedItemPosition();
  ASSERT(pos);

  int index = listCtrl->GetNextSelectedItem(pos);
  int cursel = m_listStyleCombo.GetCurSel();
  TSLWMSServiceLayer * layer = (TSLWMSServiceLayer *)listCtrl->GetItemData(index);
  if ( cursel == 0 )
  {
    listCtrl->SetItemText(index, 1, _T("[Default]"));
    layer->setStyleValue(0);
  }
  else
  {
    const char * styleName = layer->getStyleAt(cursel - 1);
    layer->setStyleValue(styleName);
    listCtrl->SetItemText(index, 1, (LPCTSTR)TSLUTF8Decoder(styleName));
  }
}

void CStyleDimensionsDlg::OnTvnSelchangedDimTree(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
  CTreeCtrl* treeCtrl = (CTreeCtrl*) GetDlgItem(IDC_DIM_TREE);
  CComboBox * dimValue = (CComboBox*) GetDlgItem(IDC_VALUE_COMBO);

  HTREEITEM oldItem = pNMTreeView->itemOld.hItem;
  if ( oldItem && treeCtrl->GetParentItem(oldItem) )
  {
    CString val;
    dimValue->GetWindowText(val);
    if ( val.GetLength() )
    {
      HTREEITEM parentNode = treeCtrl->GetParentItem(oldItem);
      TSLWMSServiceLayer* visibleLayer = (TSLWMSServiceLayer*)treeCtrl->GetItemData(parentNode);
      HTREEITEM sibling = treeCtrl->GetPrevSiblingItem(oldItem);
      int dimNumber = 0;
      while ( sibling )
      {
        sibling = treeCtrl->GetPrevSiblingItem(sibling);
        ++dimNumber;
      }
      const TSLWMSServiceLayerDimension *dim = visibleLayer->getDimensionAt(dimNumber);
      const char * dimName = dim->name();
      visibleLayer->setDimensionValue(dimName, TSLUTF8Encoder(val));
    }
  }
  HTREEITEM selectedItem = pNMTreeView->itemNew.hItem;

  HTREEITEM parentNode = treeCtrl->GetParentItem(selectedItem);
  bool isDimensionNode = parentNode != 0;

  CStatic * dimName = (CStatic*) GetDlgItem(IDC_DIM_NAME);
  CStatic * dimUnit = (CStatic*) GetDlgItem(IDC_DIM_UNIT);
  CStatic * dimUnitSymbol = (CStatic*) GetDlgItem(IDC_DIM_UNITS);
  CStatic * dimCurrent = (CStatic*) GetDlgItem(IDC_DIM_C);
  CStatic * dimMV = (CStatic*) GetDlgItem(IDC_DIM_MV);
  CStatic * dimNV = (CStatic*) GetDlgItem(IDC_DIM_NV);
  CStatic * dimDV = (CStatic*) GetDlgItem(IDC_DIM_DV);
  

  if ( isDimensionNode )
  {
    TSLWMSServiceLayer* visibleLayer = (TSLWMSServiceLayer*)treeCtrl->GetItemData(parentNode);
    int dimNumber = 0;

    HTREEITEM sibling = treeCtrl->GetPrevSiblingItem(selectedItem);
    while ( sibling )
    {
      sibling = treeCtrl->GetPrevSiblingItem(sibling);
      ++dimNumber;
    }

    const char * unit = 0, * unitSymbol = 0;
    const char * name = visibleLayer->getDimensionAt(dimNumber, &unit, &unitSymbol);
    dimName->SetWindowText((LPCTSTR)TSLUTF8Decoder(name));
    dimUnit->SetWindowText((LPCTSTR)TSLUTF8Decoder(unit));
    dimUnitSymbol->SetWindowText(unitSymbol ? (LPCTSTR)TSLUTF8Decoder(unitSymbol) : _T("N/A"));

    int noOfExtents = visibleLayer->noOfExtents();
    int index;
    for ( index = 0; index < noOfExtents; ++index )
    {
      if ( strcmp(visibleLayer->getExtentAt(index), name) == 0 )
      {
        break;
      }
    }

    const char * value = 0, *defaultValue = 0;
    bool multipleValues = false, nearestValue = false, current = false;

    if ( index != noOfExtents )
    {
      visibleLayer->getExtentAt(index, &value, &defaultValue, &nearestValue, &multipleValues, &current );
    }
     
    dimCurrent->SetWindowText(current ? _T("True") : _T("False"));
    dimMV->SetWindowText(multipleValues ? _T("True") : _T("False"));
    dimNV->SetWindowText(nearestValue ? _T("True") : _T("False"));
    dimDV->SetWindowText(defaultValue ? (LPCTSTR)TSLUTF8Decoder(defaultValue) : _T("N/A"));

    dimValue->EnableWindow();

    const char * currentValue = visibleLayer->getDimensionValue(name);

    if ( value )
    {
      vector<string> commaSeparatedValues;
      SplitStringByChar(value, ',', commaSeparatedValues);
      int noOfCommaSeparatedValues = (int)commaSeparatedValues.size();
      for ( int i = 0; i < noOfCommaSeparatedValues; ++i )
      {
        vector<string> forwardSlashSeparatedValues;
        string & entry = commaSeparatedValues[i];
        dimValue->AddString((LPCTSTR)TSLUTF8Decoder(entry));
      }
    }

    if ( currentValue )
    {
      dimValue->SetWindowText((LPCTSTR)TSLUTF8Decoder(currentValue));
    }
  }
  else
  {
    dimName->SetWindowText(_T("N/A"));
    dimUnit->SetWindowText(_T("N/A"));
    dimUnitSymbol->SetWindowText(_T("N/A"));
    dimCurrent->SetWindowText(_T("N/A"));
    dimMV->SetWindowText(_T("N/A"));
    dimNV->SetWindowText(_T("N/A"));
    dimDV->SetWindowText(_T("N/A"));
    dimValue->SetWindowText(_T(""));
    dimValue->EnableWindow(0);
  }

  *pResult = 0;
}

void CStyleDimensionsDlg::OnBnClickedOk()
{
  CTreeCtrl* treeCtrl = (CTreeCtrl*) GetDlgItem(IDC_DIM_TREE);
  CComboBox * dimValue = (CComboBox*) GetDlgItem(IDC_VALUE_COMBO);
  HTREEITEM selectedItem = treeCtrl->GetSelectedItem();

  if ( selectedItem && treeCtrl->GetParentItem(selectedItem) )
  {
    CString val;
    dimValue->GetWindowText(val);
    if ( val.GetLength() )
    {
      HTREEITEM parentNode = treeCtrl->GetParentItem(selectedItem);
      TSLWMSServiceLayer* visibleLayer = (TSLWMSServiceLayer*)treeCtrl->GetItemData(parentNode);
      HTREEITEM sibling = treeCtrl->GetPrevSiblingItem(selectedItem);
      int dimNumber = 0;
      while ( sibling )
      {
        sibling = treeCtrl->GetPrevSiblingItem(sibling);
        ++dimNumber;
      }
      const TSLWMSServiceLayerDimension *dim = visibleLayer->getDimensionAt(dimNumber);
      const char * dimName = dim->name();
      visibleLayer->setDimensionValue(dimName, TSLUTF8Encoder(val));
    }
  }
  OnOK();
}
