//	********************************************************
//	*           Source Control Information
//	********************************************************
//	$Archive: /Development/Samples/NT/RuntimeProjTest/tslviewmode.cpp $
//	$Author$
//	$Date$
//	$Revision$
//	********************************************************

#include "stdafx_tlt.h"
#include "tslviewmode.h"

TSLViewMode::TSLViewMode( TSLViewModeInterface * iViewMode, unsigned long modeID )
  : m_interface( iViewMode ), m_id( modeID )
{
  m_drawingSurface = m_interface->drawingSurface() ; 
  m_mapDataLayer = m_interface->mapDataLayer();
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
