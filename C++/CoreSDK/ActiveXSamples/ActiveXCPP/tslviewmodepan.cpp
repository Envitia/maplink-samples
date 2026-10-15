//	********************************************************
//	*           Source Control Information
//	********************************************************
//	$Archive: /Development/Samples/NT/ActiveXCPP/tslviewmodepan.cpp $
//	$Author$
//	$Date$
//	$Revision$
//	********************************************************

#include "stdafx.h"
#include "tslviewmodepan.h"

TSLViewModePan::TSLViewModePan( TSLViewModeInterface * iViewMode, long modeID, TSLViewModeCursor upCursor, TSLViewModeCursor downCursor ) 
  : TSLViewMode( iViewMode, modeID, upCursor, downCursor  )
{

}

TSLViewModePan::~TSLViewModePan()
{

}

bool TSLViewModePan::OnLButtonDown(long x, long y, bool shift, bool control)
{
  double uux = 0.0 ;
  double uuy = 0.0 ;
 
  // Get the position pressed in user units
  m_drawingSurface->DUToUU( x, y, &uux, &uuy ) ;

  // Attempt to pan, without auto_redraw enabled.
  // Request redraw only if pan is successful
  return m_drawingSurface->pan( uux, uuy, false ) ;
}

