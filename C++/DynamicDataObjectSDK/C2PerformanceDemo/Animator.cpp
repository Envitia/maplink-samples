// Animator.cpp: implementation of the CAnimator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Animator.h"
#include "C2PerformanceDemoDoc.h"
#include "Track.h"
#include "TrackManager.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAnimator::CAnimator( )
{
  m_timerID = 0 ;
  m_playbackRate = REAL_TIME ;
}

CAnimator::~CAnimator()
{
  if ( m_timerID )
  {
    KillTimer( 0, m_timerID ) ;
    m_timerID = 0 ;
  }
}

bool CAnimator::startAnimation( PLAYBACK_RATE playbackRate )
{
  if ( m_timerID )
  {
    if ( !stopAnimation() )
      return false ;
  }

  m_playbackRate = playbackRate ;

  // Set the timer interval depending on the 
  // playback speed requested by the user
  switch( m_playbackRate )
  {
  case REAL_TIME :
    m_timerID = SetTimer( 0, 0, 1000, OnTimer ) ;
    break ;
  case X2_SPEED :
    m_timerID = SetTimer( 0, 0, 500, OnTimer ) ;
    break ;
  case X5_SPEED :
    m_timerID = SetTimer( 0, 0, 200, OnTimer ) ;
    break ;
  case X10_SPEED :
    m_timerID = SetTimer( 0, 0, 100, OnTimer ) ;
    break ;
  case X50_SPEED :
    m_timerID = SetTimer( 0, 0, 20, OnTimer ) ;
  }

  return true;
}

bool CAnimator::stopAnimation()
{
  if ( m_timerID )
  {
    KillTimer( 0, m_timerID ) ;
    m_timerID = 0 ;
    return true ;
  }
  else
    return false ;
}

void CAnimator::OnTimer( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime )
{
  update();
}

void CAnimator::update ( bool moveTrack )
{
  // Get a pointer to the document
  CC2PerformanceDemoDoc * pDoc = (CC2PerformanceDemoDoc *)((CMainFrame * )AfxGetMainWnd())->GetActiveDocument() ;

  // Get a pointer to the object data layer
  TSLObjectDataLayer * pObjectLayer = pDoc->getObjectDataLayer() ;

  if ( moveTrack )
  {
    const TSLlist<TSLDynamicDataObject*>& ddoList = pObjectLayer->getDDOList() ;

    TSLMapDataLayer * pMapLayer = pDoc->getMapDataLayer() ;

    TSLlist<TSLDynamicDataObject*>::iterator scan  = ddoList.begin();
    TSLlist<TSLDynamicDataObject*>::iterator scanEnd = ddoList.end();
    for ( ; scan != scanEnd ; ++scan )
    { 
      ((CTrack *)(*scan))->translate( pMapLayer ) ;
    }

    // Now Update the views if a track has been selected
    if ( CTrackManager::selectedTrack() )
    {
      pDoc->UpdateAllViews( 0, HINT_UPDATE_SELECTED_TRACK, 0 ) ;
    }

  }

  pObjectLayer->notifyChanged( true ) ;

  pDoc->UpdateAllViews( 0, HINT_UPDATE_TOTE, 0 ) ;

}

PLAYBACK_RATE CAnimator::playbackRate() const
{
  return m_playbackRate ;
}