// LayerTreeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OverlayOpacity.h"
#include "LayerTreeDlg.h"
#include "OverlayOpacityDoc.h"

// CLayerTreeDlg dialog

IMPLEMENT_DYNAMIC(CLayerTreeDlg, CDialogBar)

CLayerTreeDlg::CLayerTreeDlg()
: m_doc(0)
, m_mapName("")
{

}

CLayerTreeDlg::~CLayerTreeDlg()
{
}

BOOL CLayerTreeDlg::Create( CWnd* pParentWnd, UINT nIDTemplate,
                           UINT nStyle, UINT nID, BOOL bChange)
{
  if(!CDialogBar::Create(pParentWnd,nIDTemplate,nStyle,nID))
    return FALSE;
  
  SetWindowText( _T("Layers") );
  return TRUE;
}

BOOL CLayerTreeDlg::Create( CWnd* pParentWnd,
                           LPCTSTR lpszTemplateName, UINT nStyle,
                           UINT nID, BOOL bChange)
{
  if (!CDialogBar::Create( pParentWnd, lpszTemplateName,
    nStyle, nID))
    return FALSE;
  
  SetWindowText( _T("Layers") );
  return TRUE;
}

void CLayerTreeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLayerTreeDlg, CDialogBar)
  ON_NOTIFY(TVN_SELCHANGED, IDC_MAP_TREE, &CLayerTreeDlg::OnTvnSelchangedMapTree)
END_MESSAGE_MAP()


// CLayerTreeDlg message handlers

void CLayerTreeDlg::UpdateTree ( COverlayOpacityDoc * doc, TSLDrawingSurface * surface )
{
  CTreeCtrl * tree = (CTreeCtrl *) GetDlgItem(IDC_MAP_TREE);
  tree->DeleteAllItems();
  m_doc = doc;

  int noOfMapsLoaded = (int)doc->getNumberOfDataLayers();
  if ( !noOfMapsLoaded )
  {
    GetParentFrame()->PostMessage(UPDATE_SELECTED_LAYER, 0, (LPARAM)surface);
    return;
  }

  HTREEITEM root = tree->InsertItem(_T("Maps"));

  for ( int i = 0; i < noOfMapsLoaded; ++i )
  {
    doc->getMapName( m_mapName, i );
    tree->InsertItem((LPCTSTR)TSLUTF8Decoder(m_mapName.c_str()), root);
  }
  tree->Expand(root, TVE_EXPAND);

}
void CLayerTreeDlg::OnTvnSelchangedMapTree(NMHDR *pNMHDR, LRESULT *pResult)
{
  CTreeCtrl * tree = (CTreeCtrl *) GetDlgItem(IDC_MAP_TREE);

  HTREEITEM selectedItem = tree->GetSelectedItem();

  if ( !selectedItem || !tree->GetParentItem(selectedItem) )
  {
    GetParentFrame()->PostMessage(UPDATE_SELECTED_LAYER, 0, 0);
    return;
  }

  HTREEITEM current = tree->GetPrevSiblingItem(selectedItem);
  int index = 0;
  while (current != 0)
  {
    ++index;
    current =  tree->GetPrevSiblingItem(current);
  }

  m_doc->getMapName( m_mapName, index );

  // Update Layer properties dialog 
  GetParentFrame()->PostMessage(UPDATE_SELECTED_LAYER,(WPARAM)&m_mapName, 0);
  
  *pResult = 0;
}
