// SymbolTreeView.cpp : implementation file
//

#include "stdafx.h"
#include "App6aViewer.h"
#include "SymbolTreeView.h"
#include "Maplink.h"
#include "App6aViewerDoc.h"

#define UNSELECTED_STATE 1
#define SELECTED_STATE 2
#define PARTIALLY_SELECTED_STATE 3


//#ifdef _DEBUG
//#define new DEBUG_NEW
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
//#endif

char * display_str = NULL ;


/////////////////////////////////////////////////////////////////////////////
// CSymbolTreeView

IMPLEMENT_DYNCREATE(CSymbolTreeView, CTreeView)

CSymbolTreeView::CSymbolTreeView()
{
}

CSymbolTreeView::~CSymbolTreeView()
{
}


BEGIN_MESSAGE_MAP(CSymbolTreeView, CTreeView)
//{{AFX_MSG_MAP(CSymbolTreeView)
ON_WM_CREATE()
ON_WM_LBUTTONDOWN()
ON_WM_KEYDOWN()
ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
ON_COMMAND(ID_EXPAND_TREE, OnExpandTree)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSymbolTreeView drawing

void CSymbolTreeView::OnDraw(CDC* pDC)
{
  //CDocument* pDoc = GetDocument();
  // TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CSymbolTreeView diagnostics

#ifdef _DEBUG
void CSymbolTreeView::AssertValid() const
{
  CTreeView::AssertValid();
}

void CSymbolTreeView::Dump(CDumpContext& dc) const
{
  CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSymbolTreeView message handlers

BOOL CSymbolTreeView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
  dwStyle |= TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_DISABLEDRAGDROP | TVS_CHECKBOXES | TVS_TRACKSELECT;
  return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

int CSymbolTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
  
  if (CTreeView::OnCreate(lpCreateStruct) == -1)
    return -1;
  
  // Create the image list and add to the tree view
  
  CImageList *	pImageList ;
  CBitmap	bitmap ;
  
  pImageList = new CImageList() ;
  
  pImageList->Create( IDB_BITMAP_STATUS, 16, 2, RGB (255,255,255));
  
  GetTreeCtrl().SetImageList( pImageList, TVSIL_STATE);	

  return 0;
}

HTREEITEM item_stack [2000] ;
int item_stack_count = 0  ;


void CSymbolTreeView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
  char pFeature[256] ; pFeature[0] = '\0' ;
  
  // Create a new node in the tree for the map items
  HTREEITEM hRoot = GetTreeCtrl().InsertItem( _T("APP6 Symbols") ) ;
  
  // Navigational nodes
  HTREEITEM hParentItem ;//, hChildItem, hNextItem ;
  
 
  
  // Create a structure to receive node information
  TVITEM  tviNode ;
  TCHAR buffer[32] ;
  buffer[0] = _T('\0') ;
  
  tviNode.mask	  = TVIF_TEXT ;
  tviNode.pszText	  = buffer ;
  tviNode.cchTextMax	  = sizeof( buffer ) ;
  
  // As we cycle through the nodes from the FeatureClassList we want to check
  // whether a level exists already or whether it needs to be added
  CApp6aViewerDoc * doc = (CApp6aViewerDoc*) GetDocument() ;

  hParentItem = hRoot ;
  item_stack[item_stack_count] = hParentItem ; 
  
 
  for (int k=0; k< doc->m_symbolHelper->Size(); k++)
  {
    char *name = "";
    doc->m_symbolHelper->GetName(k,&name);
   

    int depth = doc->m_symbolHelper->GetDepth(k) ;
    if (depth > item_stack_count)
    {
      item_stack_count++;
    }
    else if (depth < item_stack_count)
    {
      while (depth < item_stack_count)
        item_stack_count--;
    }
    
    TV_INSERTSTRUCT Item;
    Item.hParent = item_stack[item_stack_count];
    Item.hInsertAfter = item_stack[item_stack_count];
    Item.item.mask = TVIF_STATE | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
    Item.item.state = INDEXTOSTATEIMAGEMASK( SELECTED_STATE );
    Item.item.stateMask = TVIS_STATEIMAGEMASK;
    Item.item.pszText = (LPTSTR)TSLUTF8Decoder(name);
    Item.item.iImage = IDB_CHECK_SELECTED;
    Item.item.iSelectedImage = IDB_CHECK_SELECTED;
    HTREEITEM hItem = GetTreeCtrl().InsertItem(&Item);
    item_stack [item_stack_count+1] = hItem ;
    hParentItem = hItem ;
  }  
}

void CSymbolTreeView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{  
  HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
  
  
  if (hItem)
	   display_str = (char *) GetTreeCtrl().GetItemData(hItem); 
  //	Invalidate(FALSE) ;
  *pResult = 0;

  CApp6aViewerDoc * doc = (CApp6aViewerDoc*) GetDocument() ;
  doc->UpdateMainView() ;
    
}

void CSymbolTreeView::OnExpandTree() 
{




}
  
bool CSymbolTreeView::IsItemChecked(HTREEITEM hItem)
{
  if (hItem)
  {
      return GetTreeCtrl().GetItemState( hItem, TVIS_STATEIMAGEMASK )>>12 == 2;
  }
  else 
      return false;
}
  
  HTREEITEM CSymbolTreeView::GetFirstCheckedItem()
  {
    for ( HTREEITEM hItem = GetTreeCtrl().GetRootItem(); hItem!=NULL;  )
	{
		int iImage = GetTreeCtrl().GetItemState( hItem, TVIS_STATEIMAGEMASK )>>12;
		if ( iImage == CHECKED || iImage == SELF_AND_CHILD_CHECKED )
			return hItem;
		
		if( iImage != CHILD_CHECKED )
		{
			HTREEITEM hti = GetTreeCtrl().GetNextItem( hItem, TVGN_NEXT );
			if( hti == NULL )
				hItem = GetNextItem( hItem );
			else 
				hItem = hti;
		}
		else
			hItem = GetNextItem( hItem );
	}

	return NULL;

  }
  
  HTREEITEM CSymbolTreeView::GetNextCheckedItem( HTREEITEM hItem )
  {
   
	hItem = GetNextItem( hItem );
	while( hItem!=NULL )
	{
		int iImage = GetTreeCtrl().GetItemState( hItem, TVIS_STATEIMAGEMASK )>>12;
		if ( iImage == CHECKED || iImage == SELF_AND_CHILD_CHECKED )
			return hItem;
		
		if( iImage != CHILD_CHECKED )
		{
			HTREEITEM hti = GetTreeCtrl().GetNextItem( hItem, TVGN_NEXT );
			if( hti == NULL )
				hItem = GetNextItem( hItem );
			else 
				hItem = hti;
		}
		else
			hItem = GetNextItem( hItem );
	}

	return NULL;

  }
  
  HTREEITEM CSymbolTreeView::GetPrevCheckedItem( HTREEITEM hItem )
  {
    for ( hItem = GetTreeCtrl().GetPrevSiblingItem( hItem ); hItem!=NULL; hItem = GetTreeCtrl().GetPrevSiblingItem( hItem ) )
      if ( IsItemChecked(hItem) )
        return hItem;
      
      return NULL;
  }

void CSymbolTreeView::OnLButtonDown(UINT nFlags, CPoint point) 
{
        
	UINT uFlags=0;
	HTREEITEM hItem = GetTreeCtrl().HitTest(point,&uFlags);

	if( uFlags & TVHT_ONITEMSTATEICON )
	{
		SetCheck(hItem, TOGGLE);
	}
        else if(uFlags)
        {
          GetTreeCtrl().Expand(hItem, TOGGLE);
        }

	CApp6aViewerDoc * doc = (CApp6aViewerDoc*) GetDocument() ;
	doc->UpdateMainView() ;
       
}


void CSymbolTreeView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
  if( nChar == VK_SPACE )
  {
    HTREEITEM hti = GetTreeCtrl().GetSelectedItem();
    int iImage = GetTreeCtrl().GetItemState( hti, TVIS_STATEIMAGEMASK )>>12;
    GetTreeCtrl().SetItemState( hti, INDEXTOSTATEIMAGEMASK(iImage == 1 ? 2 : 1),
      TVIS_STATEIMAGEMASK );
    return;
  }
	CApp6aViewerDoc * doc = (CApp6aViewerDoc*) GetDocument() ;
	doc->UpdateMainView() ;

}

bool CSymbolTreeView::GetItemData(HTREEITEM hItem, char *data)
{
  if (hItem)
  {
    data = (char *) GetTreeCtrl().GetItemData(hItem);
    return true;
  }
  return false;
}




HTREEITEM CSymbolTreeView::GetNextItem(HTREEITEM hItem)
{
  HTREEITEM       hti;
  
  if( GetTreeCtrl().ItemHasChildren( hItem ) )
    return GetTreeCtrl().GetChildItem( hItem );           // return first child
  else{
    // return next sibling item
    // Go up the tree to find a parent's sibling if needed.
    while( (hti = GetTreeCtrl().GetNextSiblingItem( hItem )) == NULL ){
      if( (hItem = GetTreeCtrl().GetParentItem( hItem ) ) == NULL )
        return NULL;
    }
  }
  return hti;

}

HTREEITEM CSymbolTreeView::GetFirstItem()
{
    return GetTreeCtrl().GetRootItem();
}

BOOL CSymbolTreeView::SetCheck(HTREEITEM hItem, CheckType nCheck)
{
if( hItem == NULL ) 
		return FALSE;

	UINT nState = GetTreeCtrl().GetItemState( hItem, TVIS_STATEIMAGEMASK ) >> 12;

        if( nCheck == TOGGLE )
        {
          switch( nState )
          {
          case UNCHECKED:		nState = CHECKED; break;
          case CHECKED:			nState = UNCHECKED; break;
          case CHILD_CHECKED:		nState = SELF_AND_CHILD_CHECKED; break;
          case SELF_AND_CHILD_CHECKED:	nState = CHILD_CHECKED; break;
          }
        }

        GetTreeCtrl().SetItemState( hItem, INDEXTOSTATEIMAGEMASK(nState), 
					TVIS_STATEIMAGEMASK );

        if (nState == UNCHECKED)
        {
          while( (hItem = GetTreeCtrl().GetChildItem( hItem )) != NULL )
          {
               GetTreeCtrl().SetItemState( hItem, INDEXTOSTATEIMAGEMASK(UNCHECKED), 
					TVIS_STATEIMAGEMASK );

               HTREEITEM htNextItem =  GetTreeCtrl().GetNextItem( hItem, TVGN_NEXT );

               while (htNextItem != NULL )
               {
                    GetTreeCtrl().SetItemState( htNextItem, INDEXTOSTATEIMAGEMASK(UNCHECKED), 
					TVIS_STATEIMAGEMASK );

                    if (GetTreeCtrl().GetChildItem( htNextItem) != NULL)
                    {
                        SetCheck(htNextItem, UNCHECK);
                    }

                    htNextItem =  GetTreeCtrl().GetNextItem( htNextItem, TVGN_NEXT );
               }
          }
        }

        else if (nState == CHECKED)
        {
          while( (hItem = GetTreeCtrl().GetChildItem( hItem )) != NULL )
          {
               GetTreeCtrl().SetItemState( hItem, INDEXTOSTATEIMAGEMASK(CHECKED), 
					TVIS_STATEIMAGEMASK );

               HTREEITEM htNextItem =  GetTreeCtrl().GetNextItem( hItem, TVGN_NEXT );

               while (htNextItem != NULL )
               {
                    GetTreeCtrl().SetItemState( htNextItem, INDEXTOSTATEIMAGEMASK(CHECKED), 
					TVIS_STATEIMAGEMASK );

                    if (GetTreeCtrl().GetChildItem( htNextItem) != NULL)
                    {
                        SetCheck(htNextItem, CHECK);
                    }

                    htNextItem =  GetTreeCtrl().GetNextItem( htNextItem, TVGN_NEXT );
               }
          }
        }
                                                                                             
	return TRUE;

}
