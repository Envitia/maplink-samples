// C2PerformanceDemoToteView.cpp : implementation file
//

#include "stdafx.h"
#include "C2PerformanceDemoToteView.h"
#include "Track.h"
#include "TrackManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CC2PerformanceDemoToteView

IMPLEMENT_DYNCREATE(CC2PerformanceDemoToteView, CListView)

  CC2PerformanceDemoToteView::CC2PerformanceDemoToteView()
{
}

CC2PerformanceDemoToteView::~CC2PerformanceDemoToteView()
{
  vector<CTrack*>::iterator scan    = m_tracks.begin() ;
  vector<CTrack*>::iterator scanEnd = m_tracks.end() ;
  for ( ; scan != scanEnd ; ++scan )
    delete *scan ;

}


BEGIN_MESSAGE_MAP(CC2PerformanceDemoToteView, CListView)
  //{{AFX_MSG_MAP(CC2PerformanceDemoToteView)
  ON_WM_CREATE()
  ON_WM_LBUTTONDOWN()
  ON_WM_KEYUP()
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CC2PerformanceDemoToteView drawing

void CC2PerformanceDemoToteView::OnDraw(CDC* pDC)
{
}

/////////////////////////////////////////////////////////////////////////////
// CC2PerformanceDemoToteView diagnostics

#ifdef _DEBUG
void CC2PerformanceDemoToteView::AssertValid() const
{
  CListView::AssertValid();
}

void CC2PerformanceDemoToteView::Dump(CDumpContext& dc) const
{
  CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CC2PerformanceDemoToteView message handlers

void CC2PerformanceDemoToteView::OnInitialUpdate() 
{
  CListView::OnInitialUpdate() ;

  CListCtrl& listCtrl = GetListCtrl() ;
  CRect rc ;

  listCtrl.GetClientRect( &rc ) ;

  listCtrl.SetColumnWidth( 0, rc.Width() / 5 ) ;
  listCtrl.SetColumnWidth( 1, rc.Width() / 5 ) ;
  listCtrl.SetColumnWidth( 2, rc.Width() / 5 ) ;
  listCtrl.SetColumnWidth( 3, rc.Width() / 5 ) ;
  listCtrl.SetColumnWidth( 4, rc.Width() / 5 ) ;

}

BOOL CC2PerformanceDemoToteView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
  dwStyle |= LVS_REPORT | LVS_NOSORTHEADER ;
  return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext) ;
}

int CC2PerformanceDemoToteView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
  if (CListView::OnCreate(lpCreateStruct) == -1)
    return -1;

  CListCtrl& listCtrl = GetListCtrl() ;

  listCtrl.InsertColumn( 0, _T("ID") ) ;
  listCtrl.InsertColumn( 1, _T("CODE") ) ;
  listCtrl.InsertColumn( 2, _T("HEAD") ) ;
  listCtrl.InsertColumn( 3, _T("VEL") ) ;
  listCtrl.InsertColumn( 4, _T("ALT") ) ;

  return 0;
}

void CC2PerformanceDemoToteView::addTrackToTote( CTrack * pTrack )
{
  // Cycle through the tracks already stored to avoid duplication
  vector<CTrack*>::iterator scan    = m_tracks.begin() ;
  vector<CTrack*>::iterator scanEnd = m_tracks.end() ;

  for ( ; scan != scanEnd ; ++scan ) 
  {
    if ( pTrack->trackNumber() == (*scan)->trackNumber() ) 
      return ;
  }

  m_tracks.push_back( pTrack ) ;

  GetDocument()->UpdateAllViews( 0 , HINT_UPDATE_TOTE, 0 ) ;
}

void CC2PerformanceDemoToteView::clearAllTracks ()
{
  m_tracks.clear();
  GetListCtrl().DeleteAllItems();
}

void CC2PerformanceDemoToteView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
  if ( lHint == HINT_UPDATE_TOTE && m_tracks.size() != 0 )
  {
    CListCtrl& listCtrl = GetListCtrl() ;

    LVITEM		  lvItem ;	// Item structure for ammending item data
    lvItem.mask		= LVIF_TEXT ;	// We're only adding text

    LVFINDINFO		  info ;	// Structure to search the list
    info.flags		= LVFI_PARAM ;	// Set the search structure to search for the pointer value
    info.vkDirection	= VK_NEXT ;	// Direction to search

    vector<CTrack*>::iterator scan    = m_tracks.begin() ;
    vector<CTrack*>::iterator scanEnd = m_tracks.end()  ;

    char buffer[256] ;			// Buffer to hold search text
    buffer[0] = '\0' ;

    int nItem = 0 ;			// Store for the index of the item if we find it

    // Scan through the list of tracks and compare them with what is
    // displayed in the list.  If they are present in the list of tracks
    // and the view then update, present in the list of tracks but not the 
    // view then add.

    for ( ; scan != scanEnd ; ++scan )
    {
      // Search the stored track pointer values for the item
      info.lParam = DWORD(*scan) ;
      nItem = listCtrl.FindItem( &info ) ;

      // If we have found the item then set the item to 
      // point to the correct index, otherwise create a 
      // new item to assign data to. Once this first item
      // has been set the other items can be set in the 
      // same way ( just to save code duplication )

      if ( nItem != -1 )    // Found the correct item
      {
        // Update the item with the new data
        lvItem.iItem = nItem ;

        // Track ID 
        lvItem.iSubItem = 0 ;
        itoa( (*scan)->trackNumber(), buffer, 10 ) ;
        TSLUTF8Decoder bufferDecoded( buffer );
        lvItem.pszText = (LPTSTR)bufferDecoded ;
        listCtrl.SetItem( &lvItem ) ;

      }
      else    // Not present in list, add it
      {	
        lvItem.iItem = listCtrl.GetItemCount() ;

        // Track ID 
        lvItem.iSubItem = 0 ;
        itoa( (*scan)->trackNumber(), buffer, 10 ) ;
        TSLUTF8Decoder bufferDecoded( buffer );
        lvItem.pszText = (LPTSTR)bufferDecoded ;
        listCtrl.InsertItem( &lvItem ) ;

        // Set the item data for the item to the value
        // of the pointer for this track, this can be used
        // as a key for further searches

        listCtrl.SetItemData( lvItem.iItem, (DWORD)*scan ) ;

      }

      // Platform description
      lvItem.iSubItem = 1 ;
      CTrackManager::platformDescription( *scan, buffer ) ;
      TSLUTF8Decoder bufferDecoded( buffer );
      lvItem.pszText = (LPTSTR)bufferDecoded ;
      listCtrl.SetItem( &lvItem ) ;

      // Heading
      lvItem.iSubItem = 2 ;
      sprintf( buffer, "%0.1f", (*scan)->heading() ) ;
      TSLUTF8Decoder bufferDecoded2( buffer );
      lvItem.pszText = (LPTSTR)bufferDecoded2 ;
      listCtrl.SetItem( &lvItem ) ;

      // Velocity
      lvItem.iSubItem = 3 ;
      sprintf( buffer, "%0.1f", (*scan)->velocity() ) ;
      TSLUTF8Decoder bufferDecoded3( buffer );
      lvItem.pszText = (LPTSTR)bufferDecoded3 ;
      listCtrl.SetItem( &lvItem ) ;

      // Altitude
      lvItem.iSubItem = 4 ;
      sprintf( buffer, "%0.1f", (*scan)->altitude() ) ;
      TSLUTF8Decoder bufferDecoded4( buffer );
      lvItem.pszText = (LPTSTR)bufferDecoded4 ;
      listCtrl.SetItem( &lvItem ) ;

    }
  }

  ///////////////////////////////////////
  /*  Sample code to retrive values from
  the list when the index has been
  been found.

  LVITEM lvItem ;
  char buffer[32] ;

  lvItem.mask		= LVIF_TEXT ;
  lvItem.iItem	= nItem ;
  lvItem.iSubItem	= 2 ;
  lvItem.pszText	= buffer ;
  lvItem.cchTextMax	= 32 ;

  listCtrl.GetItem( &lvItem ) ;

  */
  ///////////////////////////////////////

}

void CC2PerformanceDemoToteView::OnLButtonDown(UINT nFlags, CPoint point) 
{
  CListView::OnLButtonDown(nFlags, point);

  // See if the mouse was clicked on a track or not
  CListCtrl& listCtrl = GetListCtrl() ;
  int nItem = 0 ;

  nItem = listCtrl.HitTest( point ) ;

  if ( nItem != -1 ) 
  {
    // Got a track, find out the lParam ( pointer to a track )
    // and then store it in the TrackManager
    LVITEM    lvItem ;

    lvItem.mask	    = LVIF_PARAM ;
    lvItem.iItem    = nItem ;

    listCtrl.GetItem( &lvItem ) ;

    CTrackManager::selectedTrack( (CTrack *)lvItem.lParam ) ;

  }
}

void CC2PerformanceDemoToteView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
  CListView::OnKeyUp(nChar, nRepCnt, nFlags);

  // Check to see if the key pressed was the delete key,
  // if so remove the track details from the Tote display
  if ( nChar == VK_DELETE )
  {

    // Cycle through the list and find which one has been selected
    CListCtrl& listCtrl = GetListCtrl() ;
    int nItem = 0 ;

    for ( ; nItem < listCtrl.GetItemCount() ; ++nItem )
    {
      UINT itemState = listCtrl.GetItemState( nItem, LVIS_SELECTED ) ;

      if ( itemState == LVIS_SELECTED )	  // Found the selected item
      {
        // Get the item to compare the stored pointer value
        // against the track pointers stored in the Tote list
        LVITEM	lvItem ;

        lvItem.mask   = LVIF_PARAM ;
        lvItem.iItem  = nItem ;

        if ( !listCtrl.GetItem( &lvItem ) )
          return ;

        // Cycle through the tracks to find the one we're looking for
        vector<CTrack*>::iterator scan	   = m_tracks.begin() ;
        vector<CTrack*>::iterator scanEnd  = m_tracks.end() ;

        for ( ; scan != scanEnd ; ++scan )
        {
          if ( lvItem.lParam == (LPARAM)*scan )
          {
            // Found it, remove from the list of tracks...
            m_tracks.erase( scan ) ;

            // ...and from the tote
            listCtrl.DeleteItem( nItem ) ;

            return ;
          }
        } 
      }
    }
  }
}
