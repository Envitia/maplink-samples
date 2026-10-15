// DisplayItemSettings.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include ".\displayitemsettings.h"
#include "clippingdlg.h"


typedef map< int, int, less<int> > DisplayMap;
typedef map< int, HTREEITEM, less<int> > AddedItemsMap;

// CDisplayItemSettings dialog

IMPLEMENT_DYNAMIC(CDisplayItemSettings, CDialog)
CDisplayItemSettings::CDisplayItemSettings(TSLFilterDataLayer * dataLayer, 
                                           CWnd* pParent /*=NULL*/)
	: CDialog(CDisplayItemSettings::IDD, pParent)
        , m_dataLayer(dataLayer)
        , m_currentSelection(0)
        , m_currentSelectionIndex(-1)
{
  
}

CDisplayItemSettings::~CDisplayItemSettings()
{
}

void CDisplayItemSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDisplayItemSettings, CDialog)
  ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, OnTvnSelchangedTree1)
  ON_CBN_SELCHANGE(IDC_VISIBILITY, OnCbnSelchangeVisibility)
  ON_BN_CLICKED(ID_PROCESS_ITEM, OnBnClickedProcessItem)
  ON_BN_CLICKED(ID_PROCESS_ALL, OnBnClickedProcessAll)
  ON_BN_CLICKED(ID_ADV_CLEAR, OnBnClickedAdvClear)
  ON_BN_CLICKED(ID_ADV_APPLY, OnBnClickedAdvApply)
  ON_EN_KILLFOCUS(IDC_U_MIN_X, OnEnKillfocusAdvExtent)
  ON_EN_KILLFOCUS(IDC_U_MIN_Y, OnEnKillfocusAdvExtent)
  ON_EN_KILLFOCUS(IDC_U_MAX_X, OnEnKillfocusAdvExtent)
  ON_EN_KILLFOCUS(IDC_U_MAX_Y, OnEnKillfocusAdvExtent)
  ON_BN_CLICKED(ID_CLIPPING, OnBnClickedClipping)
END_MESSAGE_MAP()


// CDisplayItemSettings message handlers

BOOL CDisplayItemSettings::OnInitDialog()
{
  CDialog::OnInitDialog();

  if ( !m_dataLayer)
  {
    return false;
  }

  CTreeCtrl * tree = (CTreeCtrl*) GetDlgItem(IDC_TREE1);

  TVINSERTSTRUCT tvInsert;
  tvInsert.hParent = NULL;
  tvInsert.hInsertAfter = NULL;
  tvInsert.item.mask = TVIF_TEXT | TVIF_STATE | TVIF_PARAM;
  tvInsert.item.pszText = _T("Filter Data Layer");
  tvInsert.item.state = TVIS_EXPANDED;
  tvInsert.item.lParam = -1;

  HTREEITEM hRoot = tree->InsertItem(&tvInsert);

  //Where we populate all the details

  DisplayMap displayMap;
  AddedItemsMap addedItems;
  const int noOfDisplayItems = m_dataLayer->numDisplayItems();

  for ( int i = 0; i < noOfDisplayItems; ++i )
  {
    TSLFilterDataLayerDisplayItem * item = m_dataLayer->getDisplayItemAt(i);
    int displayLevel = item->displayLevel();
    int parentDisplayLevel = item->parentDisplayLevel();
    displayMap[displayLevel] = parentDisplayLevel;
  }

  TCHAR buffer[MAX_PATH];
  while ( addedItems.size() != displayMap.size() )
  {
    DisplayMap::iterator it ( displayMap.begin() );
    DisplayMap::iterator itEnd ( displayMap.end() );

    while ( it != itEnd )
    {
      AddedItemsMap::iterator aimEnd(addedItems.end());

      //has this iteration already been added
      AddedItemsMap::iterator aim(addedItems.find((*it).first));
      if ( aim != aimEnd )
      { //already added
        ++it;
        continue;
      }

      //is root item
      if ( (*it).second == 0 )
      {
        tvInsert.hParent = hRoot;
        tvInsert.hInsertAfter = NULL;
        if (tree->ItemHasChildren(tvInsert.hParent))
        {
          HTREEITEM hNextItem = tree->GetChildItem(tvInsert.hParent);

          while (hNextItem != NULL)
          {
            TVITEM item;
            item.hItem = hNextItem;
            item.mask = TVIF_PARAM;
            tree->GetItem(&item);
            if ( (*it).first < item.lParam )
            {
              break;
            }
            tvInsert.hInsertAfter = hNextItem;
            hNextItem = tree->GetNextItem(tvInsert.hInsertAfter, TVGN_NEXT);
          }
        }

        tvInsert.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_STATE;
        _sntprintf(buffer, MAX_PATH, _T("Display Level %d"), (*it).first);
        tvInsert.item.pszText = buffer; 
        tvInsert.item.lParam = (*it).first;

        HTREEITEM hNewItem = tree->InsertItem(&tvInsert);

        addedItems[(*it).first] = hNewItem;
        ++it;
        continue;
      }

      //it parent added
      aim = addedItems.find((*it).second);
      if ( aim == aimEnd )
      {
        //parent has not been added yet
        ++it;
        continue;
      }

      //add the item
      tvInsert.hParent = (*aim).second;
      tvInsert.hInsertAfter = NULL;
      if (tree->ItemHasChildren(tvInsert.hParent))
      {
        HTREEITEM hNextItem = tree->GetChildItem(tvInsert.hParent);

        while (hNextItem != NULL)
        {
          TVITEM item;
          item.hItem = hNextItem;
          item.mask = TVIF_PARAM;
          tree->GetItem(&item);
          if ( (*it).first < item.lParam )
          {
            break;
          }
          tvInsert.hInsertAfter = hNextItem;
          hNextItem = tree->GetNextItem(tvInsert.hInsertAfter, TVGN_NEXT);
        }
      }

      tvInsert.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_STATE;
      _sntprintf(buffer, MAX_PATH, _T("Display Level %d"), (*it).first);
      tvInsert.item.pszText = buffer; 

      tvInsert.item.lParam = (*it).first;
      HTREEITEM hNewItem = tree->InsertItem(&tvInsert);

      addedItems[(*it).first] = hNewItem;
      ++it;
    }
  } 

  return TRUE;  
}

void CDisplayItemSettings::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
  *pResult = 0;
  if ( !m_dataLayer )
  {
    return;
  }

  LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

  LPARAM selDisplayLevel = pNMTreeView->itemNew.lParam;
  m_currentSelection = 0;
  m_currentSelectionIndex = -1;

  if ( selDisplayLevel != -1 )
  { 
    const int noOfDisplayItems = m_dataLayer->numDisplayItems();
    for ( int i = 0; i < noOfDisplayItems; ++i )
    {
      TSLFilterDataLayerDisplayItem * item = m_dataLayer->getDisplayItemAt(i);
      int displayLevel = item->displayLevel();
      if ( displayLevel == selDisplayLevel )
      {
        m_currentSelection = item;
        m_currentSelectionIndex = i;
        break;
      }
    }
  }
  UpdateFromCurrent();
}

void CDisplayItemSettings::OnCbnSelchangeVisibility()
{
  if ( !m_currentSelection )
  {
    return;
  }
  CComboBox * visibilityCombo = (CComboBox*) GetDlgItem(IDC_VISIBILITY) ;
  if ( visibilityCombo->GetCurSel() != (int)m_currentSelection->visible() )
  {
    m_currentSelection->visible(visibilityCombo->GetCurSel());
  }
}

void CDisplayItemSettings::UpdateFromCurrent()
{
  CComboBox * visibilityCombo = (CComboBox*) GetDlgItem(IDC_VISIBILITY) ;
  CEdit* descriptionEdit = (CEdit*) GetDlgItem(IDC_DESCRIPTION);
  CEdit* geolocatedEdit = (CEdit*) GetDlgItem(IDC_GEOLOCATED);
  CEdit* userDefined = (CEdit*) GetDlgItem(IDC_USER_DEFINED);
  CEdit* processStatus = (CEdit*) GetDlgItem(IDC_PROCESS_STATUS);
  CButton* processItem = (CButton*) GetDlgItem(ID_PROCESS_ITEM);
  CButton* clipItem = (CButton*) GetDlgItem(ID_CLIPPING);
  clipItem->EnableWindow(FALSE);
  CButton* advClear = (CButton*) GetDlgItem(ID_ADV_CLEAR);
  advClear->EnableWindow(FALSE);
  CButton* advApply = (CButton*) GetDlgItem(ID_ADV_APPLY);
  advApply->EnableWindow(FALSE);
  CStatic* advGroup = (CStatic*) GetDlgItem(IDC_ADV_STATIC);
  advGroup->SetWindowText(_T("Advance Settings"));
  CEdit* uminX = (CEdit*) GetDlgItem(IDC_U_MIN_X);
  uminX->EnableWindow(FALSE);
  CEdit* uminY = (CEdit*) GetDlgItem(IDC_U_MIN_Y);
  uminY->EnableWindow(FALSE);
  CEdit* umaxX = (CEdit*) GetDlgItem(IDC_U_MAX_X);
  umaxX->EnableWindow(FALSE);
  CEdit* umaxY = (CEdit*) GetDlgItem(IDC_U_MAX_Y);
  umaxY->EnableWindow(FALSE);

  CEdit* minX = (CEdit*) GetDlgItem(IDC_MIN_X);
  CEdit* minY = (CEdit*) GetDlgItem(IDC_MIN_Y);
  CEdit* maxX = (CEdit*) GetDlgItem(IDC_MAX_X);
  CEdit* maxY = (CEdit*) GetDlgItem(IDC_MAX_Y);

  if ( m_currentSelection == 0 )
  { //Item is the root item, this doesn't refer to a display item
    visibilityCombo->EnableWindow(FALSE);
    visibilityCombo->SetCurSel(-1);
    descriptionEdit->EnableWindow(FALSE);
    descriptionEdit->SetWindowText(_T(""));
    geolocatedEdit->EnableWindow(FALSE);
    geolocatedEdit->SetWindowText(_T(""));
    userDefined->EnableWindow(FALSE);
    userDefined->SetWindowText(_T(""));
    processStatus->EnableWindow(FALSE);
    processStatus->SetWindowText(_T(""));

    minX->EnableWindow(FALSE);
    minX->SetWindowText(_T(""));
    minY->EnableWindow(FALSE);
    minY->SetWindowText(_T(""));
    maxX->EnableWindow(FALSE);
    maxX->SetWindowText(_T(""));
    maxY->EnableWindow(FALSE);
    maxY->SetWindowText(_T(""));
  }
  else
  {
    visibilityCombo->EnableWindow();
    visibilityCombo->SetCurSel(m_currentSelection->visible());
    char* description;
    m_currentSelection->description(&description);
    descriptionEdit->EnableWindow();
    descriptionEdit->SetWindowText((LPCTSTR)TSLUTF8Decoder(description));
    delete [] description;
    geolocatedEdit->EnableWindow();
    userDefined->EnableWindow();
    bool isValid, isUserDefined;
    double x1, y1, x2, y2;
    m_currentSelection->geolocation(isValid, isUserDefined, x1, y1, x2, y2);
    geolocatedEdit->SetWindowText(isValid ? _T("TRUE") : _T("FALSE") );
    userDefined->SetWindowText(isUserDefined ? _T("TRUE") : _T("FALSE") );

    TCHAR xString[256] ;
    _sntprintf( xString, 256, _T("%10.6lf"), x1 ) ;
    minX->SetWindowText( xString ) ;
    _sntprintf( xString, 256, _T("%10.6lf"), y1 ) ;
    minY->SetWindowText( xString ) ;
    _sntprintf( xString, 256, _T("%10.6lf"), x2 ) ;
    maxX->SetWindowText( xString ) ;
    _sntprintf( xString, 256, _T("%10.6lf"), y2 ) ;
    maxY->SetWindowText( xString ) ;

    processStatus->EnableWindow();
    TSLFilterProcessStatus status = m_currentSelection->processingStatus();
    processItem->EnableWindow();
    switch ( status )
    {
      case TSLFilterProcessStatusUnknown:
        {
          processStatus->SetWindowText(_T("Unprocessed"));
          
          break;
        }
      case TSLFilterProcessStatusFail:
        {
          processStatus->SetWindowText(_T("Processing Failed"));
          break;
        }
      case TSLFilterProcessStatusSuccess:
        {
          processStatus->SetWindowText(_T("Processing Successful"));
          break;
        }
      default:
         {
          processStatus->SetWindowText(_T("Unknown !?!?!"));
         }
    }

    int parentDisplayLevel = m_currentSelection->parentDisplayLevel();
    if ( !parentDisplayLevel )
    {
      if ( isUserDefined )
      {
        advClear->EnableWindow();
      }
      CString windowText;
      uminX->EnableWindow();
      minX->GetWindowText(windowText);
      m_advMinX = x1;
      uminX->SetWindowText(windowText);
      uminY->EnableWindow();
      minY->GetWindowText(windowText);
      m_advMinY = y1;
      uminY->SetWindowText(windowText);
      umaxX->EnableWindow();
      maxX->GetWindowText(windowText);
      m_advMaxX = x2;
      umaxX->SetWindowText(windowText);
      umaxY->EnableWindow();
      maxY->GetWindowText(windowText);
      m_advMaxY = y2;
      umaxY->SetWindowText(windowText);
    }

    clipItem->EnableWindow(TRUE);
    
  }
}

void CDisplayItemSettings::OnBnClickedProcessItem()
{
  if ( !m_dataLayer || !m_currentSelection )
  {
    return;
  }
  if ( !m_dataLayer->processDisplayItem(m_currentSelectionIndex) )
  {
    AfxMessageBox(_T("Processing Item Failed"), 0, 0);
  }
  UpdateFromCurrent();  
}

void CDisplayItemSettings::OnBnClickedProcessAll()
{
  if ( !m_dataLayer )
  {
    return;
  }
  if ( !m_dataLayer->process() )
  {
    AfxMessageBox(_T("Processing Failed"), 0, 0);
  }
  UpdateFromCurrent();
}

void CDisplayItemSettings::OnBnClickedAdvClear()
{
  if ( !m_dataLayer || !m_currentSelection )
  {
    return;
  }
  if ( !m_currentSelection->clearGeolocation() )
  {
    AfxMessageBox(_T("Failed to clear user geolocation"), 0, 0);
  }
  UpdateFromCurrent();
}

void CDisplayItemSettings::OnBnClickedAdvApply()
{
  if ( !m_dataLayer || !m_currentSelection )
  {
    return;
  }
  CEdit* uminX = (CEdit*) GetDlgItem(IDC_U_MIN_X);
  CEdit* uminY = (CEdit*) GetDlgItem(IDC_U_MIN_Y);
  CEdit* umaxX = (CEdit*) GetDlgItem(IDC_U_MAX_X);
  CEdit* umaxY = (CEdit*) GetDlgItem(IDC_U_MAX_Y);
  TCHAR buffer[MAX_PATH];
  uminX->GetWindowText(buffer, MAX_PATH);
  m_advMinX = _tstof(buffer);
  uminY->GetWindowText(buffer, MAX_PATH);
  m_advMinY = _tstof(buffer);
  umaxX->GetWindowText(buffer, MAX_PATH);
  m_advMaxX = _tstof(buffer);
  umaxY->GetWindowText(buffer, MAX_PATH);
  m_advMaxY = _tstof(buffer);

  if ( !m_currentSelection->setGeolocation(m_advMinX, m_advMinY, m_advMaxX, m_advMaxY) )
  {
    AfxMessageBox(_T("Failed to set user geolocation"), 0, 0);
  }
  UpdateFromCurrent();
}

void CDisplayItemSettings::OnEnKillfocusAdvExtent()
{
  CEdit* uminX = (CEdit*) GetDlgItem(IDC_U_MIN_X);
  CEdit* uminY = (CEdit*) GetDlgItem(IDC_U_MIN_Y);
  CEdit* umaxX = (CEdit*) GetDlgItem(IDC_U_MAX_X);
  CEdit* umaxY = (CEdit*) GetDlgItem(IDC_U_MAX_Y);
  bool changed = false;
  TCHAR buffer[MAX_PATH];

  uminX->GetWindowText(buffer, MAX_PATH);
  double current = _tstof(buffer);
  if ( m_advMinX != current )
  {
    changed = true;
  }
  if ( !changed )
  {
    uminY->GetWindowText(buffer, MAX_PATH);
    current = _tstof(buffer);
    if ( m_advMinY != current )
    {
      changed = true;
    }
  }
  if ( !changed )
  {
    umaxX->GetWindowText(buffer, MAX_PATH);
    current = _tstof(buffer);
    if ( m_advMaxX != current )
    {
      changed = true;
    }
  }
  if ( !changed )
  {
    umaxY->GetWindowText(buffer, MAX_PATH);
    current = _tstof(buffer);
    if ( m_advMaxY != current )
    {
      changed = true;
    }
  }

  CButton* advApply = (CButton*) GetDlgItem(ID_ADV_APPLY);
  CStatic* advGroup = (CStatic*) GetDlgItem(IDC_ADV_STATIC);
  if ( changed )
  {   
    advApply->EnableWindow();
    advGroup->SetWindowText(_T("Advance Settings*"));
  }
  else
  {
    advApply->EnableWindow(FALSE);
    advGroup->SetWindowText(_T("Advance Settings"));
  }

}

void CDisplayItemSettings::OnBnClickedClipping()
{
  if ( !m_currentSelection )
  {
    return;
  }
  bool clip;
  double minX, minY, maxX, maxY;
  m_currentSelection->getInputClipping(clip, &minX, &minY, &maxX, &maxY);
  if ( !clip )
  { //if clipping is not enabled, then set values to be extent
    bool dumb1, dumb2;
    m_currentSelection->geolocation(dumb1, dumb2, minX, minY, maxX, maxY);
  }

  CClippingDlg * dlg = new CClippingDlg(clip, minX, minY, maxX, maxY, ::AfxGetApp()->GetMainWnd());
  dlg->DoModal();
  delete dlg;

  m_currentSelection->setInputClipping(clip, minX, minY, maxX, maxY);
}
