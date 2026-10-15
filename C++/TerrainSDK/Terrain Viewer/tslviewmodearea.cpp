//	********************************************************
//	*           Source Control Information
//	********************************************************
//	$Archive: /MapLinkWizard/Template/tslviewmodeArea.cpp $
//	$Author$
//	$Date$
//	$Revision$
//	********************************************************

#include "stdafx.h"
#include "resource.h"
#include "areadialog.h"
#include "TerrainViewerDoc.h"
#include "TerrainViewerView.h"
#include "tslviewmodeArea.h"
#include "MainFrm.h"
#include "TerrainViewerOptions.h"

TSLViewModeArea::TSLViewModeArea( TSLViewModeInterface * iViewMode, 
  unsigned long modeID, CTerrainViewerDoc * pDoc ) 
  : TSLViewMode( iViewMode, modeID )
{
  m_doc = pDoc ;
}

TSLViewModeArea::~TSLViewModeArea()
{ 

}

bool TSLViewModeArea::DUToMU( long x, long y, double & mux, double & muy )
{
  if ( m_doc->isConversionRequired() )
  {
    double lat, lon;
    m_drawingSurface->DUToLatLong( x, y, &lat, &lon ) ;
    const TSLCoordinateSystem *terrainCS = m_doc->terrainCoordinateSystem();
    if( !terrainCS )
      return false ;
    
    terrainCS->latLongToMU ( lat, lon, &mux, &muy );
  }
  else
  {
    m_drawingSurface->DUToMU( x, y, &mux, &muy ) ;
  }
  return true ;
}

void TSLViewModeArea::getExtent( double mux, double  muy, TSLMUExtent & extent ) 
{
}

bool TSLViewModeArea::performViewshed(long x, long y)
{
  double mux, muy;

  // Convert the center point to map units in the TSLViewShedDataLayer/terrain database's coordinate system
  if ( !DUToMU( x, y, mux, muy ) )
    return false ;

  // Query the current extent of the drawing surface in map Units
  // This may not be in the same coordinate system as the viewshed datalayer
  double x1, y1, x2, y2;

  m_drawingSurface->getMUExtent(&x1, &y1, &x2, &y2);
  if (m_doc->isConversionRequired())
  {
    double lat1, lon1, lat2, lon2;
    m_drawingSurface->MUToLatLong(x1, y1, &lat1, &lon1);
    m_drawingSurface->MUToLatLong(x2, y2, &lat2, &lon2);
    const TSLCoordinateSystem *terrainCS = m_doc->terrainCoordinateSystem();
    if (terrainCS)
    {
      terrainCS->latLongToMU(lat1, lon1, &x1, &y1);
      terrainCS->latLongToMU(lat2, lon2, &x2, &y2);
    }
    else
    {
      return false;
    }
  }
  
  // And the extent of the current view in display units
  TSLDeviceUnits duX1, duY1, duX2, duY2;
  m_drawingSurface->getDUExtent(&duX1, &duY1, &duX2, &duY2);

  // Pass the parameters to the doc, and perform the viewshed.
  if( m_doc->calculateViewshed(mux, muy, m_fixedPositions, TSLMUExtent(x1, y1, x2, y2), (duX2 - duX1), (duY2 - duY1)) )
  {
    // Force the surface to redraw. This is needed as mfc buffers up draw events and only sends the last one
    m_drawingSurface->redraw();
    return true;
  }
  return false;
}

bool TSLViewModeArea::OnMouseMove(long x, long y, bool shift, bool control)
{
  return performViewshed(x, y);
}

bool TSLViewModeArea::OnLButtonDown(long x, long y, bool shift, bool control)
{
  if ( shift )
  {
    double mux, muy ;
    if ( DUToMU( x, y, mux, muy ) )
    {
      m_fixedPositions.push_back( TSLMUCoord( mux, muy ) ) ;
    }
    return false ;
  }

  double uux = 0.0 ;
  double uuy = 0.0 ;
 
  // Get the position pressed in user units
  m_drawingSurface->DUToUU( x, y, &uux, &uuy ) ;

  // Attempt to pan, without auto_redraw enabled.
  // Request redraw only if pan is successful
  return m_drawingSurface->pan( uux, uuy, false ) ;
}

bool TSLViewModeArea::OnRButtonDown(long x, long y, bool shift, bool control)
{
  // Reset back to no mode - would be simpler using standard TSLInteractionModes
  ((CTerrainViewerView *)(  ((CMainFrame*)::AfxGetMainWnd())->GetActiveView() ))->resetMode() ;
  return true ; // and redraw the screen to wipe out the image
}

bool TSLViewModeArea::OnLButtonUp(long x, long y, bool shift, bool control)
{
  return false;

}

bool TSLViewModeArea::OnMButtonDown( long x, long y, bool shift, bool control )
{
  return false ;
}

bool TSLViewModeArea::OnMouseWheel(long x, long y, int delta, bool shift, bool control)
{
  if ( m_drawingSurface )
  {
    double uux, uuy ;
    m_drawingSurface->DUToUU( x, y, &uux, &uuy ) ;
    bool sts = m_drawingSurface->zoom( 30, delta > 0, false ) ;
    if ( sts )
      m_drawingSurface->pan( uux, uuy, x, y, false ) ;

    return sts ;
  }
  return false ;    // No redraw required
}


