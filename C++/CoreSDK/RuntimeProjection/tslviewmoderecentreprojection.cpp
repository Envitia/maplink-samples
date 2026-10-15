//	********************************************************
//	*           Source Control Information
//	********************************************************
//	$Archive: /Development/Samples/NT/RuntimeProjTest/tslviewmoderecentreprojection.cpp $
//	$Author$
//	$Date$
//	$Revision$
//	********************************************************

#include "stdafx_tlt.h"
#include "tslviewmoderecentreprojection.h"
#include "RuntimeProjTest.h"
#include "originchangeinfo.h"




TSLViewModeRecentreProjection::TSLViewModeRecentreProjection( TSLViewModeInterface * iViewMode, unsigned long modeID ) 
  : TSLViewMode( iViewMode, modeID )
{
}


TSLViewModeRecentreProjection::~TSLViewModeRecentreProjection()
{
}


bool TSLViewModeRecentreProjection::OnLButtonDown(long x, long y, bool shift, bool control)
{
  Double lattdCentre, longtdCentre, range;
  Double lattdPressed, longtdPressed;

  // get the current view centre and range
  if ( !m_drawingSurface->getViewedLatLongRange( &lattdCentre, &longtdCentre, &range, true, false ) )
    return false;

  // get the location clicked
  if ( !m_drawingSurface->DUToLatLong( x, y, &lattdPressed, &longtdPressed, true, false ) )
    return false;

  // change projection centre to where clicked
  if ( CRuntimeProjTestApp::m_useCallbacksForProjectionChanges )
  {
    // in background thread, callback given when done
    OriginChangeInfo* info = new OriginChangeInfo;
    info->m_mainWindow = AfxGetMainWnd()->m_hWnd;
    info->m_latitude = lattdPressed;
    info->m_longitude = longtdPressed;
    info->m_range = range;
    info->m_mapLayer = m_mapDataLayer;
    info->m_llDataLayer = m_llDataLayer;

    m_mapDataLayer->setRuntimeProjectionOrigin( lattdPressed, longtdPressed, runtimeProjectionChangedCallback, info );

    // don't redraw, even if successful - wait for callback
    return false; 
  }
  else
  {
    // synchronously
    if (!m_mapDataLayer->setRuntimeProjectionOrigin( lattdPressed, longtdPressed, 0, 0 ) )
      return false;

    // set view to new centre with range that it had before
    if ( !m_drawingSurface->setViewedLatLongRange( lattdPressed, longtdPressed, range, true, true, true ) )
      return false;

    // invalidate the map data layer
    m_mapDataLayer->notifyChanged( true );

    // request redraw
    return true;
  }
}


void TSLViewModeRecentreProjection::runtimeProjectionChangedCallback( void* arg, TSLRuntimeProjectionChangedEnum reason )
{
  // callback from a worker thread - send a message to the main frame, which calls back into the view

  // only expected for origin change; projection change has a different callback, in CRuntimeProjTestDoc
  ASSERT ( reason == TSLRuntimeProjectionChangeOriginChangedOnly );

  OriginChangeInfo* info = (OriginChangeInfo*)arg;

  // send message to main window, which calls back into the View
  ::PostMessage( info->m_mainWindow, PWM_PROJORIGINCHANGED, (WPARAM)info, NULL );
}



