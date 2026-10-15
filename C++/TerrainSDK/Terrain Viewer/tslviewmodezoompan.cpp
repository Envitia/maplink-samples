//	********************************************************
//	*           Source Control Information
//	********************************************************
//	$Archive: /MapLinkWizard/Template/tslviewmodezoompan.cpp $
//	$Author$
//	$Date$
//	$Revision$
//	********************************************************

#include "stdafx.h"
#include "tslviewmodezoompan.h"

TSLViewModeZoomPan::TSLViewModeZoomPan( TSLViewModeInterface * iViewMode, 
				       unsigned long modeID, double zoomPercentage ) 
  : TSLViewMode( iViewMode, modeID )
{
  // Remember how much we should zoom by.
  m_percentage = zoomPercentage ;
}

TSLViewModeZoomPan::~TSLViewModeZoomPan()
{

}

bool TSLViewModeZoomPan::OnLButtonDown(long x, long y, bool shift, bool control)
{
  double uux = 0.0 ;
  double uuy = 0.0 ;
 
  // Get the position pressed in user units.
  m_drawingSurface->DUToUU( x, y, &uux, &uuy ) ;

  // Zoom in by required percentage. If successful, pan.
  // Request redraw if both operations are successful
  if ( m_drawingSurface->zoom( m_percentage, true, false ) )
    return m_drawingSurface->pan( uux, uuy, false ) ;

  return false ;
}

bool TSLViewModeZoomPan::OnRButtonDown(long x, long y, bool shift, bool control)
{
  double uux = 0.0 ;
  double uuy = 0.0 ;
 
  // Get the position pressed in user units
  m_drawingSurface->DUToUU( x, y, &uux, &uuy ) ;

  // Zoom out by required percentage. If successful, pan.
  // Request redraw if both operations are successful
  if ( m_drawingSurface->zoom( m_percentage, false, false ) )
    return m_drawingSurface->pan( uux, uuy, false ) ;

  return false ;
}

