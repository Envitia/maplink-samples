//	********************************************************
//	*           Source Control Information
//	********************************************************
//	$Archive: /Samples/NT/DDO/tslviewmode.cpp $
//	$Author$
//	$Date$
//	$Revision$
//	********************************************************

#include "stdAfx.h"
#include "tslviewmode.h"

TSLViewMode::TSLViewMode( TSLViewModeInterface * iface, unsigned long modeID )
  : m_interface( iface ), m_id( modeID )
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
