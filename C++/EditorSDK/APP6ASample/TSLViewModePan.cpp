#include "stdafx.h"
#include "TSLViewModepan.h"

TSLViewModePan::TSLViewModePan( TSLViewModeInterface * iViewMode, unsigned long modeID ) 
  : TSLViewMode( iViewMode, modeID )
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

bool TSLViewModePan::OnRButtonUp(long x, long y, bool shift, bool control)
{
  // Reset the current view mode to edit
  m_interface->resetMode() ;
  return false ;
}

const char * TSLViewModePan::queryPrompt( )
{
  return "Left button click to centre map on selected position, Right button click to finish" ;
}



