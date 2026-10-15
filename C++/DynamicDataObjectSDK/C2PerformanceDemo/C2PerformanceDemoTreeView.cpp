// C2PerformanceDemoTreeView.cpp : implementation file
//

#include "stdafx.h"
#include "C2PerformanceDemoTreeView.h"
#include "C2PerformanceDemoDoc.h"
#include "TrackManager.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Defines for setting bitmaps for the tree nodes
#define UNSELECTED_STATE 1
#define SELECTED_STATE 2
#define PARTIALLY_SELECTED_STATE 3

// The item data for the nodes are function pointers,

typedef void (*CLICK_FUNC)( bool ) ;		      // Normal track manager set functions

/////////////////////////////////////////////////////////////////////////////
// CC2PerformanceDemoTreeView

IMPLEMENT_DYNCREATE(CC2PerformanceDemoTreeView, CTreeView)

CC2PerformanceDemoTreeView::CC2PerformanceDemoTreeView()
: m_pImageList(0)
, m_TrackParentNode(0)
, m_lastNonApp6A(0)
, m_AssumedFriendly(0)
, m_Pending(0)
, m_Suspect(0)
, m_Joker(0)
, m_Faker(0)
{
}

CC2PerformanceDemoTreeView::~CC2PerformanceDemoTreeView()
{
  if ( m_pImageList )
  {
    delete m_pImageList;
  }
}


BEGIN_MESSAGE_MAP(CC2PerformanceDemoTreeView, CTreeView)
	//{{AFX_MSG_MAP(CC2PerformanceDemoTreeView)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CC2PerformanceDemoTreeView drawing

void CC2PerformanceDemoTreeView::OnDraw(CDC* pDC)
{
}

/////////////////////////////////////////////////////////////////////////////
// CC2PerformanceDemoTreeView diagnostics

#ifdef _DEBUG
void CC2PerformanceDemoTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CC2PerformanceDemoTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CC2PerformanceDemoTreeView message handlers

BOOL CC2PerformanceDemoTreeView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
  dwStyle |= TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_DISABLEDRAGDROP ;
  
  return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CC2PerformanceDemoTreeView::OnInitialUpdate() 
{
  CTreeView::OnInitialUpdate();
}


int CC2PerformanceDemoTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
  if (CTreeView::OnCreate(lpCreateStruct) == -1)
	  return -1;
	
  // Create the image list and add to the tree view
  
  CBitmap	bitmap ;

  m_pImageList = new CImageList() ;
  m_pImageList->Create( 12, 14, ILC_COLOR4 | ILC_MASK, 5,5 ) ;

  // Cannot use the first bitmap to display the tree node's selected
  // state
  bitmap.LoadBitmap( IDB_CHECK_EMPTY ) ;
  m_pImageList->Add( &bitmap, RGB( 255, 255, 255 ) ) ;
  bitmap.DeleteObject() ;

  // Add the three tree item state bitmaps to the list
  bitmap.LoadBitmap( IDB_CHECK_UNSELECTED ) ;
  m_pImageList->Add( &bitmap, RGB( 255, 255, 255 ) ) ;
  bitmap.DeleteObject() ;

  bitmap.LoadBitmap( IDB_CHECK_SELECTED ) ;
  m_pImageList->Add( &bitmap, RGB( 255, 255, 255 ) ) ;
  bitmap.DeleteObject() ;

  bitmap.LoadBitmap( IDB_CHECK_PART_SELECTED ) ;
  m_pImageList->Add( &bitmap, RGB( 255, 255, 255 ) ) ;
  bitmap.DeleteObject() ;

  // Add the image list to the tree control
  GetTreeCtrl().SetImageList( m_pImageList, TVSIL_STATE ) ;

  HTREEITEM newItem ;
  
  CLICK_FUNC  pFunction ;	// Set the item data to point to a function
  
  // Now set the main parent nodes in the tree
  m_TrackParentNode = GetTreeCtrl().InsertItem( _T("Tracks") ) ;

  // Toggle the display of track types
  newItem = GetTreeCtrl().InsertItem( _T("Hostile Tracks"), m_TrackParentNode ) ;
  pFunction = CTrackManager::displayHostileTracks ;
  GetTreeCtrl().SetItemData( newItem, (DWORD_PTR)pFunction ) ;
  GetTreeCtrl().SetItemState( newItem, INDEXTOSTATEIMAGEMASK( SELECTED_STATE ), TVIS_STATEIMAGEMASK ) ;
  
  newItem = GetTreeCtrl().InsertItem( _T("Friendly Tracks"), m_TrackParentNode ) ;
  pFunction = CTrackManager::displayFriendlyTracks ;
  GetTreeCtrl().SetItemData( newItem, (DWORD_PTR)pFunction ) ;
  GetTreeCtrl().SetItemState( newItem, INDEXTOSTATEIMAGEMASK( SELECTED_STATE ), TVIS_STATEIMAGEMASK ) ;
  
  newItem = GetTreeCtrl().InsertItem( _T("Neutral Tracks"), m_TrackParentNode ) ;
  pFunction = CTrackManager::displayNeutralTracks ;
  GetTreeCtrl().SetItemData( newItem, (DWORD_PTR)pFunction ) ;
  GetTreeCtrl().SetItemState( newItem, INDEXTOSTATEIMAGEMASK( SELECTED_STATE ), TVIS_STATEIMAGEMASK ) ;
  
  m_lastNonApp6A = GetTreeCtrl().InsertItem( _T("Unknown Tracks"), m_TrackParentNode ) ;
  pFunction = CTrackManager::displayUnknownTracks ;
  GetTreeCtrl().SetItemData( m_lastNonApp6A, (DWORD_PTR)pFunction ) ;
  GetTreeCtrl().SetItemState( m_lastNonApp6A, INDEXTOSTATEIMAGEMASK( SELECTED_STATE ), TVIS_STATEIMAGEMASK ) ;
  
  // Toggle the track numbers on and off....
  newItem = GetTreeCtrl().InsertItem( _T("Track Numbers"), m_TrackParentNode ) ;
  pFunction = CTrackManager::displayTrackNumbers ;
  GetTreeCtrl().SetItemData( newItem, (DWORD_PTR)pFunction ) ;
  GetTreeCtrl().SetItemState( newItem, INDEXTOSTATEIMAGEMASK( SELECTED_STATE ), TVIS_STATEIMAGEMASK ) ;

  // Toggle the platform codes on and off....
  newItem = GetTreeCtrl().InsertItem( _T("Platform Codes"), m_TrackParentNode ) ;
  pFunction = CTrackManager::displayPlatformCodes ;
  GetTreeCtrl().SetItemData( newItem, (DWORD_PTR)pFunction ) ;
  GetTreeCtrl().SetItemState( newItem, INDEXTOSTATEIMAGEMASK( SELECTED_STATE ), TVIS_STATEIMAGEMASK ) ;

  // Toggle the velocity vectors on and off....
  newItem = GetTreeCtrl().InsertItem( _T("Velocity Vectors"), m_TrackParentNode ) ;
  pFunction = CTrackManager::displayVelocityVectors ;
  GetTreeCtrl().SetItemData( newItem, (DWORD_PTR)pFunction ) ;
  GetTreeCtrl().SetItemState( newItem, INDEXTOSTATEIMAGEMASK( SELECTED_STATE ), TVIS_STATEIMAGEMASK ) ;	

  // Toggle the history tracks on and off....
  newItem = GetTreeCtrl().InsertItem( _T("History Points"), m_TrackParentNode ) ;
  pFunction = CTrackManager::displayHistoryPoints ;
  GetTreeCtrl().SetItemData( newItem, (DWORD_PTR)pFunction ) ;
  GetTreeCtrl().SetItemState( newItem, INDEXTOSTATEIMAGEMASK( SELECTED_STATE ), TVIS_STATEIMAGEMASK ) ;	

  return 0;
}

void CC2PerformanceDemoTreeView::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
  UINT	nodeState ;
  DWORD dwPos ;
  POINT pt;

  HTREEITEM	  nodeClicked ;
  TV_HITTESTINFO  nodeHitTest ;
  
  // Find out where the cursor was
  dwPos = GetMessagePos();
  pt.x = LOWORD(dwPos);
  pt.y = HIWORD(dwPos);
  
  ::MapWindowPoints( HWND_DESKTOP, pNMHDR->hwndFrom, &pt, 1);
  
  nodeHitTest.pt = pt ;
  nodeClicked = GetTreeCtrl().HitTest( &nodeHitTest );
  
  // If the state image was clicked, lets get the state from the item and toggle it.
  if ( nodeHitTest.flags & TVHT_ONITEMSTATEICON )
  {
    // Buffers to receive the node's label to use in processing the node click correctly
    TCHAR nodeText[32] ;	  nodeText[0]	= _T('\0') ;
    TCHAR parentText[32] ;        parentText[0] = _T('\0') ;

    TVITEM		tviNode ;
    tviNode.hItem	= nodeClicked;
    tviNode.mask	= TVIF_STATE | TVIF_TEXT ;
    tviNode.stateMask	= TVIS_STATEIMAGEMASK ;
    tviNode.pszText	= nodeText ;
    tviNode.cchTextMax	= 32 ;

    GetTreeCtrl().GetItem( &tviNode ) ;

    // Find out whether the node is selected or not
    nodeState = tviNode.state & TVIS_STATEIMAGEMASK ;
    bool nodeSelected = ( nodeState == INDEXTOSTATEIMAGEMASK( SELECTED_STATE ) ) || ( nodeState == INDEXTOSTATEIMAGEMASK( PARTIALLY_SELECTED_STATE ) ) ? true : false ;

    // Toggle the state
    tviNode.state = nodeSelected ? INDEXTOSTATEIMAGEMASK( UNSELECTED_STATE ) : INDEXTOSTATEIMAGEMASK( SELECTED_STATE ) ;
    tviNode.mask  = TVIF_STATE ;
    GetTreeCtrl().SetItem( &tviNode );

    nodeSelected = !nodeSelected ;

    // Find out who the parent is ( to determine how to process the node )
    HTREEITEM parent = GetTreeCtrl().GetParentItem( nodeClicked ) ;

    TVITEM		  tviParent ;
    tviParent.hItem	  = parent ;
    tviParent.mask	  = TVIF_TEXT ;
    tviParent.pszText	  = parentText ;
    tviParent.cchTextMax  = sizeof( parentText ) ;

    GetTreeCtrl().GetItem( &tviParent ) ;

    if ( _tcsicmp( tviParent.pszText, _T("Tracks") ) == 0 )
    {
      // Attempting to turn one of the track attributes on or off
      CLICK_FUNC pFunction = (CLICK_FUNC)GetTreeCtrl().GetItemData( nodeClicked ) ;
      pFunction( nodeSelected ) ;
    }
    else
    {
      // Must be attempting to turn one of the feature codes on or off
      // As we deselect each node in the function setChildNodes send a 
      // message to the view to declutter the relevant feature
      if ( nodeSelected )
      {
	// Set all the child nodes on
	setChildNodes( nodeClicked, true ) ;
	setParentNode( nodeClicked, true ) ;

	if ( !GetTreeCtrl().GetItemData( nodeClicked ) )
	{
	  // Virtual feature......
	  char buffer[1024] ; buffer[0] = '\0' ;
	  getNodePath( nodeClicked, buffer ) ;
	  // Turn the feature on
	  GetDocument()->UpdateAllViews( 0, HINT_UPDATE_VIRTUAL_DECLUTTER_STATUS_ON, (CObject *)buffer ) ;
	}
	else
	{
	  // Turn the feature on
	  GetDocument()->UpdateAllViews( 0, HINT_UPDATE_DECLUTTER_STATUS_ON, (CObject *)GetTreeCtrl().GetItemData( nodeClicked ) ) ;
	}
      }
      else
      {
	// Set all the child nodes off
	setChildNodes( nodeClicked, false ) ;
	setParentNode( nodeClicked, false ) ;

	if ( !GetTreeCtrl().GetItemData( nodeClicked ) )
	{
	  // Virtual feature......
	  char buffer[1024] ; buffer[0] = '\0' ;
	  getNodePath( nodeClicked, buffer ) ;
	  // Turn the feature on
	  GetDocument()->UpdateAllViews( 0, HINT_UPDATE_VIRTUAL_DECLUTTER_STATUS_OFF, (CObject *)buffer ) ;
	}
	else
	{
	  // Turn the feature off
	  GetDocument()->UpdateAllViews( 0, HINT_UPDATE_DECLUTTER_STATUS_OFF, (CObject *)GetTreeCtrl().GetItemData( nodeClicked ) ) ;
	}
      }
    }
  }
  
  *pResult = 0;
}

void CC2PerformanceDemoTreeView::setMapFeatureTree() 
{
  TSLMapDataLayer * pLayer = ((CC2PerformanceDemoDoc *)GetDocument())->getMapDataLayer() ;
  const TSLFeatureClassList * pList = pLayer->featureList() ;

  if ( pList ) 
  {
    string featureName ;
    char pFeature[256] ; pFeature[0] = '\0' ;
    char * token ;
    int i = 0 ;
    TSLFeatureID id = 0 ;

    CTreeCtrl& tree = GetTreeCtrl();

    HTREEITEM hRoot = tree.GetRootItem();
    hRoot = tree.GetNextSiblingItem( hRoot );
    if ( hRoot )
    {
      GetTreeCtrl().DeleteItem(hRoot);
    }

    
    // Create a new node in the tree for the map items
    hRoot = tree.InsertItem( _T("Map") ) ;
    // Navigational nodes
    HTREEITEM hParentItem, hChildItem, hNextItem ;

    // Create a structure to receive node information
    TVITEM  tviNode ;
    TCHAR buffer[32] ; buffer[0] = _T('\0') ;
    
    tviNode.mask	  = TVIF_TEXT ;
    tviNode.pszText	  = buffer ;
    tviNode.cchTextMax	  = 32 ;
    
    // As we cycle through the nodes from the FeatureClassList we want to check
    // whether a level exists already or whether it needs to be added
    bool nodeExists = false ;

    // Cycle through the list of features and add the features to the tree
    for ( ; i < pList->size() ; ++i )
    {
      featureName = pList->getDetails( i, &id ) ;

      hParentItem = hRoot ;

      strcpy( pFeature, featureName.c_str() ) ;

      token = strtok( pFeature, "." ) ;

      while ( token != NULL )
      {

	// Get the first child of the parent
	hChildItem = tree.GetChildItem( hParentItem ) ;

	nodeExists = false ;

	// Cycle through the siblings of this child
	while (hChildItem != NULL)
	{
	  // This node has children therefore its declutter setting is 
	  // a virtual one i.e. it will declutter all its children.  If we
	  // set the item data to 0, when the node is clicked the OnClick
	  // function will recognise this and pass the string representing the
	  // node's path to the setDeclutterStatus member function on the drawing
	  // surface rather than the feature id.  For more information see the MapLink
	  // API documentation

	  tree.SetItemData( hParentItem, 0 ) ;

	  tviNode.hItem = hChildItem ;
	  tree.GetItem( &tviNode ) ;
	  
	  // Check if the node already exists
	  if ( _tcsicmp( (LPCTSTR)TSLUTF8Decoder(token), tviNode.pszText ) == 0 ) 
	  {
	    // Found the node, set this node as the parent for the next iteration
	    nodeExists = true ;
	    hParentItem = hChildItem ;
	    break ;
	  }
	  
	  // If this node wasn't it, check the next sibling
	  hNextItem = tree.GetNextItem(hChildItem, TVGN_NEXT) ;
	  hChildItem = hNextItem ;

	}

	// If we didn't find the node create it now and set it as the parent for the next iteration
	if ( !nodeExists ) 
	{
	  hParentItem = tree.InsertItem( (LPCTSTR)TSLUTF8Decoder(token), hParentItem ) ;
	  // Set the item data to the feature ID, this is used 
	  // by the drawing surface to identify the feature
	  tree.SetItemData( hParentItem, id ) ;
	  tree.SetItemState( hParentItem, INDEXTOSTATEIMAGEMASK( SELECTED_STATE ), TVIS_STATEIMAGEMASK );
	}

	// Extract the next feature name
	token = strtok( NULL, "." ) ;

      }

    }
  }

}

void CC2PerformanceDemoTreeView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
  if ( lHint == HINT_NEW_MAP_LOADED ) 
  {
    setMapFeatureTree() ;
  }
}

// Either set or deselect the child nodes of a node
void CC2PerformanceDemoTreeView::setChildNodes( HTREEITEM node, bool select )
{
  if ( GetTreeCtrl().ItemHasChildren( node ) )
  {
    HTREEITEM hNextItem;
    HTREEITEM hChildItem = GetTreeCtrl().GetChildItem( node );

    while ( hChildItem != NULL )
    {
      setChildNodes( hChildItem, select ) ;
      hNextItem = GetTreeCtrl().GetNextItem( hChildItem, TVGN_NEXT );
      hChildItem = hNextItem;
    }
  }

  TVITEM	    tviNode ;
  tviNode.hItem	    = node ;
  tviNode.mask	    = TVIF_STATE ;
  tviNode.stateMask = TVIS_STATEIMAGEMASK ;

  // Toggle the state
  tviNode.state = select ? INDEXTOSTATEIMAGEMASK( SELECTED_STATE ) : INDEXTOSTATEIMAGEMASK( UNSELECTED_STATE ) ;
  tviNode.mask  = TVIF_STATE ;
  GetTreeCtrl().SetItem( &tviNode );
}

// Set the state of the parent node
void CC2PerformanceDemoTreeView::setParentNode( HTREEITEM node, bool nodeSelected )
{
  // Find out if the parent item is the "Map" root node on the 
  // tree, if so we won't need to set the check state
  HTREEITEM hParentItem = GetTreeCtrl().GetParentItem( node ) ;
  CString parentText = GetTreeCtrl().GetItemText( hParentItem ) ;

  if ( _tcsicmp( parentText, _T("Map") ) == 0 ) 
    return ;
  
  TVITEM	    tviNode ;
  tviNode.mask	    = TVIF_STATE ;
  tviNode.stateMask = TVIS_STATEIMAGEMASK ;

  UINT nodeState ;

  int numSiblings = 0 ;
  int numSelected = 0 ;
  int numPartiallySelected = 0 ;
  
  // Find out whether one or more of the siblings are selected
  HTREEITEM hNextItem ;
  HTREEITEM hSiblingItem = GetTreeCtrl().GetNextItem( node, TVGN_NEXT ) ;

  while ( hSiblingItem != NULL )
  {
    ++numSiblings ;

    tviNode.hItem = hSiblingItem ;
    GetTreeCtrl().GetItem( &tviNode ) ;

    // Find out whether the node is selected or not
    nodeState = tviNode.state & TVIS_STATEIMAGEMASK ;

    if ( nodeState == INDEXTOSTATEIMAGEMASK( SELECTED_STATE ) )
    {
      ++numSelected ;
    }
    else if ( nodeState == INDEXTOSTATEIMAGEMASK( PARTIALLY_SELECTED_STATE ) )
    {
      ++numPartiallySelected ;
    }

    hNextItem = GetTreeCtrl().GetNextItem( hSiblingItem, TVGN_NEXT );
    hSiblingItem = hNextItem;
  }

  hSiblingItem = GetTreeCtrl().GetNextItem( node, TVGN_PREVIOUS ) ;

  while ( hSiblingItem != NULL )
  {
    ++numSiblings ;

    tviNode.hItem = hSiblingItem ;
    GetTreeCtrl().GetItem( &tviNode ) ;

    // Find out whether the node is selected or not
    nodeState = tviNode.state & TVIS_STATEIMAGEMASK ;

    if ( nodeState == INDEXTOSTATEIMAGEMASK( SELECTED_STATE ) )
    {
      ++numSelected ;
    }
    else if ( nodeState == INDEXTOSTATEIMAGEMASK( PARTIALLY_SELECTED_STATE ) )
    {
      ++numPartiallySelected ;
    }

    hNextItem = GetTreeCtrl().GetNextItem( hSiblingItem, TVGN_PREVIOUS );
    hSiblingItem = hNextItem;
  }

  // If the node is unselected and no siblings are selected then set
  // the parent node to be unselected
  if ( nodeSelected )
  {
    if ( numSiblings == 0 )
    {
      GetTreeCtrl().SetItemState( hParentItem, INDEXTOSTATEIMAGEMASK( SELECTED_STATE ), TVIS_STATEIMAGEMASK ) ;
    }
    else if ( numSelected == numSiblings )
    {
      GetTreeCtrl().SetItemState( hParentItem, INDEXTOSTATEIMAGEMASK( SELECTED_STATE ), TVIS_STATEIMAGEMASK ) ;
    }
    else
    {
      GetTreeCtrl().SetItemState( hParentItem, INDEXTOSTATEIMAGEMASK( PARTIALLY_SELECTED_STATE ), TVIS_STATEIMAGEMASK ) ;
    }
  }
  else
  {
    if ( numSiblings == 0 )
    {
      GetTreeCtrl().SetItemState( hParentItem, INDEXTOSTATEIMAGEMASK( UNSELECTED_STATE ), TVIS_STATEIMAGEMASK ) ;
    }
    else if ( numSelected > 0 || numPartiallySelected > 0 )
    {
      GetTreeCtrl().SetItemState( hParentItem, INDEXTOSTATEIMAGEMASK( PARTIALLY_SELECTED_STATE ), TVIS_STATEIMAGEMASK ) ;
    }
    else if ( numSelected == 0 && numPartiallySelected == 0 )
    {
      GetTreeCtrl().SetItemState( hParentItem, INDEXTOSTATEIMAGEMASK( UNSELECTED_STATE ), TVIS_STATEIMAGEMASK ) ;
    }
  }

  // Set the parent of the parent.....
  setParentNode( hParentItem, nodeSelected ) ;

}

// Helper function to build up the node's path to pass for decluttering virtual features
void CC2PerformanceDemoTreeView::getNodePath( HTREEITEM node, char * buffer )
{
  char thisBuffer[1024] ; thisBuffer[0] = '\0' ;
  string temp ;
  
  temp = TSLUTF8Encoder(GetTreeCtrl().GetItemText( node )) ;

  while ( temp != "Map" )
  {
    strcat( thisBuffer, "." ) ;
    strcat( thisBuffer, temp.c_str() ) ;
    strcat( thisBuffer, buffer ) ;
    strcpy(buffer, thisBuffer);
    thisBuffer[0] = '\0' ;
    node = GetTreeCtrl().GetParentItem( node ) ;
    temp = TSLUTF8Encoder(GetTreeCtrl().GetItemText( node )) ;
  }

  // Only thing now is that the path name has a preceding '.' that needs to be removed
  int size = strlen( buffer ) ;

  for ( int i = 0 ; i < size ; ++i )
  {
    buffer[i] = buffer[i + 1] ;
  }
 
}

void CC2PerformanceDemoTreeView::setSymbolType ( bool isAPP6A )
{
  if ( isAPP6A )
  {
    //insert after m_lastNonApp6A
    CLICK_FUNC  pFunction ;	// Set the item data to point to a function
    m_AssumedFriendly = GetTreeCtrl().InsertItem( _T("Assumed Friendly Tracks"), m_TrackParentNode, m_lastNonApp6A ) ;
    pFunction = CTrackManager::displayAssumedFriendlyTracks ;
    GetTreeCtrl().SetItemData( m_AssumedFriendly, (unsigned long)pFunction ) ;
    GetTreeCtrl().SetItemState( m_AssumedFriendly, INDEXTOSTATEIMAGEMASK( SELECTED_STATE ), TVIS_STATEIMAGEMASK ) ;

    m_Pending = GetTreeCtrl().InsertItem( _T("Pending Tracks"), m_TrackParentNode, m_AssumedFriendly ) ;
    pFunction = CTrackManager::displayPendingTracks;
    GetTreeCtrl().SetItemData( m_Pending, (unsigned long)pFunction ) ;
    GetTreeCtrl().SetItemState( m_Pending, INDEXTOSTATEIMAGEMASK( SELECTED_STATE ), TVIS_STATEIMAGEMASK ) ;

    m_Suspect = GetTreeCtrl().InsertItem( _T("Suspect Tracks"), m_TrackParentNode, m_Pending ) ;
    pFunction = CTrackManager::displaySuspectTracks ;
    GetTreeCtrl().SetItemData( m_Suspect, (unsigned long)pFunction ) ;
    GetTreeCtrl().SetItemState( m_Suspect, INDEXTOSTATEIMAGEMASK( SELECTED_STATE ), TVIS_STATEIMAGEMASK ) ;

    m_Joker = GetTreeCtrl().InsertItem( _T("Joker Tracks"), m_TrackParentNode, m_Suspect ) ;
    pFunction = CTrackManager::displayJokerTracks ;
    GetTreeCtrl().SetItemData( m_Joker, (unsigned long)pFunction ) ;
    GetTreeCtrl().SetItemState( m_Joker, INDEXTOSTATEIMAGEMASK( SELECTED_STATE ), TVIS_STATEIMAGEMASK ) ;

    m_Faker = GetTreeCtrl().InsertItem( _T("Faker Tracks"), m_TrackParentNode, m_Joker ) ;
    pFunction = CTrackManager::displayFakerTracks ;
    GetTreeCtrl().SetItemData( m_Faker, (unsigned long)pFunction ) ;
    GetTreeCtrl().SetItemState( m_Faker, INDEXTOSTATEIMAGEMASK( SELECTED_STATE ), TVIS_STATEIMAGEMASK ) ;

  }
  else
  {
    GetTreeCtrl().DeleteItem(m_AssumedFriendly);
    GetTreeCtrl().DeleteItem(m_Pending);
    GetTreeCtrl().DeleteItem(m_Suspect);
    GetTreeCtrl().DeleteItem(m_Joker);
    GetTreeCtrl().DeleteItem(m_Faker);
    m_AssumedFriendly = 0;
    m_Pending = 0;
    m_Suspect = 0;
    m_Joker = 0;
    m_Faker = 0;
  }
}

