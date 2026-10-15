//	********************************************************
//	*           Source Control Information
//	********************************************************
//	$Archive: /Development/Samples/NT/mfc/tsl3dviewmode.cpp $
//	$Author$
//	$Date$
//	$Revision$
//	********************************************************

#include "StdAfx.h"
#include "tsl3dviewmode.h"

TSL3DViewMode::TSL3DViewMode( TSL3DViewModeInterface * iViewMode, unsigned long modeID )
  : m_interface( iViewMode ), m_id( modeID )
{
  m_drawingSurface = m_interface->drawingSurface() ; 
}

TSL3DViewMode::~TSL3DViewMode()
{

}

bool TSL3DViewMode::OnLButtonDown(long x, long y, bool shift, bool control)
{
  return false ;    // No redraw required
}

bool TSL3DViewMode::OnMButtonDown(long x, long y, bool shift, bool control)
{
  return false ;    // No redraw required
}

bool TSL3DViewMode::OnRButtonDown(long x, long y, bool shift, bool control)
{
  return false ;    // No redraw required
}

bool TSL3DViewMode::OnLButtonUp(long x, long y, bool shift, bool control)
{
  return false ;    // No redraw required
}

bool TSL3DViewMode::OnMButtonUp(long x, long y, bool shift, bool control)
{
  return false ;    // No redraw required
}

bool TSL3DViewMode::OnRButtonUp(long x, long y, bool shift, bool control)
{
  return false ;    // No redraw required
}

bool TSL3DViewMode::OnMouseMove(long x, long y, bool shift, bool control)
{
  return false ;    // No redraw required
}
