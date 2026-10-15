// TSLViewModeTerrain.cpp: implementation of the TSLViewModeTerrain class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TerrainViewer.h"
#include "TerrainView.h"
#include "TerrainViewerDoc.h"
#include "TSLViewModeTerrain.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CTerrainViewerApp theApp;

bool mousedown = false ;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TSLViewModeTerrain::TSLViewModeTerrain( TSLViewModeInterface * iViewMode, unsigned long modeID, CTerrainView * pTerrainView, CTerrainViewerDoc * pDoc )
: TSLViewMode( iViewMode, modeID ), m_terrain_view( pTerrainView )
{
  m_startX = 0 ;
  m_startY = 0 ;
  m_start_mux = 0.0 ;
  m_start_muy = 0.0 ;
  m_pressed = false ;
  m_doc = pDoc ;
}

TSLViewModeTerrain::~TSLViewModeTerrain()
{
}

bool TSLViewModeTerrain::OnLButtonDown(long x, long y, bool shift, bool control)
{
  
  mousedown = true ;
  m_startX = x ;
  m_startY = y ;
  m_pressed = true ;
  
  if ( m_doc->isConversionRequired() )
  {
    double lat, lng ;
    m_drawingSurface->DUToLatLong( x, y, &lat, &lng ) ;
    const TSLCoordinateSystem *terrainCS = m_doc->terrainCoordinateSystem();
    if( terrainCS )
    {
      terrainCS->latLongToMU( lat, lng, &m_start_mux , &m_start_muy ) ;
    }
    else
    {
      // The terrain database is in lat/lon already
      m_start_mux = lng;
      m_start_muy = lat;
    }
  }
  else
  {   
    // Convert as normal.
    m_drawingSurface->DUToMU( x, y, &m_start_mux, &m_start_muy );
  }

  m_interface->captureMouse( ) ;
  
  // Attempt to pan, without auto_redraw enabled.
  // Request redraw only if pan is successful
  return true ;
}

bool TSLViewModeTerrain::OnMouseMove(long x, long y, bool shift, bool control)
{
  
#define NUMPOINTS 100
  
  static double terr_data[NUMPOINTS] ;
  
  double end_mux ;
  double end_muy ;
  
  if (!mousedown) 
    return true ;
 
  if ( m_doc->isConversionRequired() )
  {
    double lat, lng ;
    m_drawingSurface->DUToLatLong( x, y, &lat, &lng ) ;
    const TSLCoordinateSystem *terrainCS = m_doc->terrainCoordinateSystem();
    if( terrainCS )
    {
      terrainCS->latLongToMU( lat, lng, &end_mux, &end_muy );
    }
    else
    {
      // The terrain DB is in lat/lon already
      end_mux = lng;
      end_muy = lat;
    }    
  }
  else
  {   
    // Convert as normal.
    m_drawingSurface->DUToMU( x, y, &end_mux, &end_muy );
  }

  
  TSLTerrainDataItem di[NUMPOINTS];
  
  bool highestRes = shift;
  
  if ( m_doc->getTerrainDB() && m_doc->getTerrainDB()->queryLine( m_start_mux, m_start_muy, end_mux, end_muy, NUMPOINTS, di, TSLTerrainData_All, highestRes, TSLTerrainInterpolate_LINEAR ) == TSLTerrain_OK )
  {
    for ( int i = 0; i < NUMPOINTS; i++ )
    {
      if ( di[i].m_isNull )
      {
        terr_data[i] = 0.0;
      }
      else
      {
        terr_data[i] = di[i].m_z;
      }
    }
  }
  else
  {
    memset( terr_data, 0, sizeof( double ) * NUMPOINTS );
  }
  
  // Find the range
  double mux1 = m_start_mux;
  double muy1 = m_start_muy;
  double mux2 = end_mux;
  double muy2 = end_muy;
  m_drawingSurface->DUToMU( m_startX, m_startY, &mux1, &muy1 );
  m_drawingSurface->DUToMU( x, y, &mux2, &muy2 );
  
  double range = sqrt( ( mux2 - mux1 ) * ( mux2 - mux1 ) + ( muy2 - muy1 ) * ( muy2 - muy1 ) );
  
  m_terrain_view->SetData(NUMPOINTS,terr_data, range) ; 	
  
  m_interface->setEchoLine( m_startX, m_startY, x, y) ; 
  
  return false ;    // No redraw required
}

bool TSLViewModeTerrain::OnLButtonUp(long x, long y, bool shift, bool control)
{
  
  // Get the position pressed in user units
  
  m_interface->releaseMouse( ) ;  
  
  // Firstly clear the flags and any rectangle drawn.
  m_interface->clearEchoLine( ) ; 
  m_pressed = false ;	          
  
  mousedown = false ;
  
  // Attempt to pan, without auto_redraw enabled.
  // Request redraw only if pan is successful
  return true ;
}
