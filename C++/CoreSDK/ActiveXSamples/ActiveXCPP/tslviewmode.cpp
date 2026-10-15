//	********************************************************
//	*           Source Control Information
//	********************************************************
//	$Archive: /Development/Samples/NT/ActiveXCPP/tslviewmode.cpp $
//	$Author$
//	$Date$
//	$Revision$
//	********************************************************

#include "stdafx.h"
#include "tslviewmode.h"

TSLViewMode::TSLViewMode( TSLViewModeInterface * iViewMode, long modeID, TSLViewModeCursor upCursor, TSLViewModeCursor downCursor /*= 0*/ )
  : m_interface( iViewMode ), m_id( modeID ), m_upCursor( upCursor ), m_downCursor( downCursor )
{
  m_drawingSurface = m_interface->drawingSurface() ; 
}

TSLViewMode::~TSLViewMode()
{

}

bool TSLViewMode::OnLButtonDown(long x, long y, bool shift, bool control)
{
  return false ;    // No redraw required
}

bool TSLViewMode::OnMButtonDown(long x, long y, bool shift, bool control)
{
  return false ;    // No redraw required
}

bool TSLViewMode::OnRButtonDown(long x, long y, bool shift, bool control)
{
  return false ;    // No redraw required
}

bool TSLViewMode::OnLButtonUp(long x, long y, bool shift, bool control)
{
  return false ;    // No redraw required
}

bool TSLViewMode::OnMButtonUp(long x, long y, bool shift, bool control)
{
  return false ;    // No redraw required
}

bool TSLViewMode::OnRButtonUp(long x, long y, bool shift, bool control)
{
  return false ;    // No redraw required
}

bool TSLViewMode::OnMouseMove(long x, long y, bool shift, bool control)
{
  return false ;    // No redraw required
}

void TSLViewMode::OnSetCursor( bool down )
{
  // Set the down cursor if we can & it's a mouse button down message
  if ( m_downCursor && down ) 
    m_interface->setCursor( m_downCursor ) ;
  else if ( m_upCursor && !down )
    m_interface->setCursor( m_upCursor ) ;
}