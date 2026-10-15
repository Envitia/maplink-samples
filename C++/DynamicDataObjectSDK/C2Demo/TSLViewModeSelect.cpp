// TSLViewModeSelect.cpp: implementation of the TSLViewModeSelect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TSLViewModeSelect.h"
#include "MainFrm.h"
#include "C2DemoDoc.h"
#include "TrackManager.h"
#include "C2DemoToteView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TSLViewModeSelect::TSLViewModeSelect( TSLViewModeInterface * iViewMode, unsigned long modeID, const char * objectLayerName )
: TSLViewMode( iViewMode, modeID )
{
  m_objectLayerName = strdup( objectLayerName ) ;
  m_ownShipTrack = ((CC2DemoDoc *)((CMainFrame *)AfxGetApp()->GetMainWnd())->GetActiveDocument())->getOwnShipTrack() ;
  m_pressed = false ;
  m_dragging = false ;
  m_startX = 0 ;
  m_startY = 0 ;
  m_originalID = m_id ;
}

TSLViewModeSelect::~TSLViewModeSelect()
{
  free( m_objectLayerName );
}

bool TSLViewModeSelect::OnLButtonDown(long x, long y, bool shift, bool control)
{
  m_pressed = true ;
  m_startX = x ;
  m_startY = y ;
  return true ; 
}

bool TSLViewModeSelect::OnLButtonUp( long x, long y, bool shift, bool control ) 
{
  // Check that the user hasn't tried to drag a track ( see logic in OnMouseMove )
  // and released accidentally over a different track.

  if ( !m_dragging )
  {
    // Select a track...
    TSLObjectDataLayer * pLayer = (TSLObjectDataLayer *)m_drawingSurface->getDataLayer( m_objectLayerName ) ;
    TSLTMC x1, y1, dx, dy ;

    m_drawingSurface->DUToTMC( x , y , &x1 , &y1 ) ;
    m_drawingSurface->DUToTMC( x + 10, y + 10, &dx , &dy ) ;
    dx -= x1 ;
    dy -= y1 ;
    long aperture = sqrt( (double)dx * (double)dx + (double)dy * (double)dy ) ;
    CTrack * pTrack = (CTrack *)pLayer->findDDO( x1, y1, aperture ) ;
    
    if ( pTrack )
    {
      CTrackManager::selectedTrack( pTrack ) ;
    }

    // Once we have selected a track the views are updated in the CAnimator::OnTimer()
    // function where a test for a selected track is made before calling the UpdateAllViews()
    // function.  The Track Info panel and other views test the CTrackManager for a 
    // selected track before attempting to update any data.
  }
  else
  {
    // Finshed dragging, we needed to find a Track before the drag operation 
    // could be started, so we can assume that the dragTrack in TrackManager
    // is the correct track to place in the tote, if the mouse was released in
    // the Tote view, of course

    // Get the tote view and find out its co-ordinate location
    CC2DemoToteView * pTote = ((CMainFrame *)AfxGetMainWnd())->getToteView() ;
    CRect rc ;
    pTote->GetWindowRect( &rc ) ;

    // Take into account the size of the toolbar, menu bar and caption bar
    int yDisplacement = ((CMainFrame *)AfxGetMainWnd())->getGUIDisplacement() ;
    rc.bottom = rc.bottom - yDisplacement ;
    rc.top    = rc.top - yDisplacement ;

    // Now find out if the current mouse position is in the 
    // Tote window space...
    CPoint pt( x, y ) ;

    // If the point is in the Totes window rectangle...
    if ( rc.PtInRect( pt ) )
      pTote->addTrackToTote( CTrackManager::dragTrack() ) ;
    
    // Clear the current drag track
    CTrackManager::dragTrack( NULL ) ;

    // Set the id and other variables 
    // back to the original value
    m_id = m_originalID ;
    m_dragging = false ;
    m_startX = 0 ;
    m_startY = 0 ;

  }

  m_pressed = false ;

  return true ;
}

bool TSLViewModeSelect::OnMouseMove(long x, long y, bool shift, bool control)
{
  // If the user has the mouse pressed and has moved > 10 pixels  
  // in either direction then assume this is a drag operation....
  if ( m_pressed )
  {
    if ( ( abs( x - m_startX ) > 5 || abs( y - m_startY ) > 5 ) )
    {
      // ...but first check whether there is actually 
      // a track under the mouse to be dragged to the Tote
      TSLObjectDataLayer * pLayer = (TSLObjectDataLayer *)m_drawingSurface->getDataLayer( m_objectLayerName ) ;
      TSLTMC x1, y1, dx, dy ;

      m_drawingSurface->DUToTMC( m_startX , m_startY , &x1 , &y1 ) ;
      m_drawingSurface->DUToTMC( m_startX + 10, m_startY + 10, &dx , &dy ) ;
      dx -= x1 ;
      dy -= y1 ;
      long aperture = sqrt( (double)dx * (double)dx + (double)dy * (double)dy ) ;
      CTrack * pTrack = (CTrack *)pLayer->findDDO( x1, y1, aperture ) ;
    
      if ( pTrack )
      {
        // Set the TrackManager dragTrack to the track we've found
        CTrackManager::dragTrack( pTrack ) ;

        // Temporarily set the id to a user defined value
        // so that the view will display the drag mouse icon.
        m_id = ID_TOOLS_TRACKSELECT_DRAG ;
        m_dragging = true ;
      }
    }
  }

  // We never want to redraw here
  return false ;
}

