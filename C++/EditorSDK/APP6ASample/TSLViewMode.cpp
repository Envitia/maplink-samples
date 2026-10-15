
#include "stdafx.h"
#include "TSLViewMode.h"

TSLViewMode::TSLViewMode( TSLViewModeInterface * iViewMode, unsigned long modeID )
  : m_interface( iViewMode ), m_id( modeID )
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

bool TSLViewMode::OnMouseMove(TSLViewModeButton button, long x, long y, bool shift, bool control)
{
  return false ;    // No redraw required
}

const char * TSLViewMode::queryPrompt( )
{
  return "" ;
}

void TSLViewMode::OnSize( long cx, long cy )
{
}

void TSLViewMode::OnDraw( long x1, long y1, long x2, long y2 )
{
}

