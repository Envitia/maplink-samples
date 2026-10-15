// TSLViewModeLOS.cpp: implementation of the TSLViewModeLOS class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TerrainViewer.h"
#include "TerrainViewerDoc.h"
#include "TerrainView.h"
#include "TSLViewModeLOS.h"
#include "Losview.h"
#include "MainFrm.h"
#include "TerrainViewerOptions.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CTerrainViewerApp theApp;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TSLViewModeLOS::TSLViewModeLOS( TSLViewModeInterface * iViewMode, unsigned long modeID, CTerrainView * pTerrainView, CTerrainViewerDoc * pDoc )
  : TSLViewMode( iViewMode, modeID ), 
    m_terrain_view( pTerrainView ), 
    m_los_view ( NULL ),
    m_start_mux( 0.0 ),
    m_start_muy( 0.0 ),
    m_end_mux( 0.0 ),
    m_end_muy( 0.0 ),
    m_central_meridian( 0.0 ),
    m_pressed( false ),
    m_isLatLon( false ),
    m_doc( pDoc )
{
  m_layer = pDoc->getVisDataLayer();
  m_convert = pDoc->isConversionRequired();
}

TSLViewModeLOS::TSLViewModeLOS( TSLViewModeInterface * iViewMode, unsigned long modeID, CLOSView * pTerrainLOSView, CTerrainViewerDoc * pDoc )
  : TSLViewMode( iViewMode, modeID ),
    m_terrain_view( NULL ), 
    m_los_view(pTerrainLOSView),
    m_startX( 0 ),
    m_startY( 0 ),
    m_start_mux( 0.0 ),
    m_start_muy( 0.0 ),
    m_end_mux( 0.0 ),
    m_end_muy( 0.0 ),
    m_pressed( false ),
    m_isLatLon( false ),
    m_doc( pDoc)
{
  m_layer = pDoc->getVisDataLayer();
  m_convert = pDoc->isConversionRequired();
}

TSLViewModeLOS::~TSLViewModeLOS()
{
  m_layer = m_doc->getVisDataLayer();
  m_layer->entitySet()->clear();
  m_layer->notifyChanged();
}

bool TSLViewModeLOS::OnLButtonDown(long x, long y, bool shift, bool control)
{
  m_startX = x ;
  m_startY = y ;
  m_pressed = true ;

  if ( m_convert )
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
      m_start_mux = lng;
      m_start_muy = lat;
    }

    if( lat == m_start_muy && lng == m_start_mux )
    {
      m_isLatLon = true;
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

bool TSLViewModeLOS::OnMouseMove(long x, long y, bool shift, bool control)
{
  TSLTerrainDatabase* terrainDB(m_doc->getTerrainDB());
  if (!terrainDB)
  {
    return false;
  }

  static double terr_data[NUMPOINTS] ;

  if ( !m_pressed ) 
    return true ;

  if ( m_convert )
  {
    double lat, lng ;
    m_drawingSurface->DUToLatLong( x, y, &lat, &lng ) ;
    const TSLCoordinateSystem *terrainCS = m_doc->terrainCoordinateSystem();
    if( terrainCS )
    {
      terrainCS->latLongToMU( lat, lng, &m_end_mux , &m_end_muy ) ;
    }
    else
    {
      m_end_mux = lng;
      m_end_muy = lat;
    }
  }
  else
  {   
    // Convert as normal.
    m_drawingSurface->DUToMU( x, y, &m_end_mux, &m_end_muy );
  }

  TSLTerrainDataItem di[NUMPOINTS];

  bool highestRes = shift;

  bool crossesDateLine = false;
  // Work out if we're crossing the dateline or not
  if( m_isLatLon && m_central_meridian != 0.0 && ( m_start_mux >=0 != m_end_mux >= 0 ) &&
    ( ( m_end_mux >= m_start_mux && x <= m_startX ) || ( m_end_mux <= m_start_mux && x >= m_startX ) ) )
  {
    crossesDateLine = true;
  }
  if ( terrainDB->queryLine ( m_start_mux, m_start_muy, m_end_mux, m_end_muy, NUMPOINTS, di, TSLTerrainData_Min, highestRes, TSLTerrainInterpolate_NONE, crossesDateLine ) == TSLTerrain_OK )
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
  double mux2 = m_end_mux;
  double muy2 = m_end_muy;
  m_drawingSurface->DUToMU( m_startX, m_startY, &mux1, &muy1 );
  m_drawingSurface->DUToMU( x, y, &mux2, &muy2 );

  double range = sqrt( ( mux2 - mux1 ) * ( mux2 - mux1 ) + ( muy2 - muy1 ) * ( muy2 - muy1 ) );

  if (m_terrain_view)
    m_terrain_view->SetData(NUMPOINTS, terr_data, range) ; 	

  m_interface->setEchoLine( m_startX, m_startY, x, y);

  return false ;    // No redraw required
}

bool TSLViewModeLOS::OnLButtonUp(long x, long y, bool shift, bool control)
{
  m_interface->releaseMouse( ) ;

  TSLTerrainDataItem di[NUMLOSPOINTS];

  // Query the terrain database passing the Map Unit values of the
  // start and end of the line.
  bool crossesDateLine = false;
  if( m_isLatLon && m_central_meridian != 0.0 && ( m_start_mux >=0 != m_end_mux >= 0 ) &&
    ( ( m_end_mux > m_start_mux && x <= m_startX ) || ( m_end_mux < m_start_mux && x >= m_startX ) ) )
  {
    crossesDateLine = true;
  }

  if (m_doc->getTerrainDB() && m_doc->getTerrainDB()->queryLine( m_start_mux, m_start_muy, m_end_mux, m_end_muy, NUMLOSPOINTS, di, TSLTerrainData_Min, shift, TSLTerrainInterpolate_NONE, crossesDateLine ) == TSLTerrain_OK)
  {
    m_layer->entitySet()->clear();

    Double startLon = di[0].m_x;
    Double startLat = di[0].m_y;
    Double startHeight;

    Double endLon = di[NUMLOSPOINTS - 1].m_x;
    Double endLat = di[NUMLOSPOINTS - 1].m_y;
    Double endHeight;

    Double currentLat;
    Double currentLon;
    Double currentHeight;

    TerrainViewerOptions& options = CMainFrame::instance()->options();

    // If the checkbox in the options dialog is checked, reference heights from sea level
    startHeight = options.m_losStartHeight;
    endHeight = options.m_losEndHeight;
    if( !options.m_losFromSeaLevel )
    {
      startHeight += di[0].m_z;
      endHeight += di[NUMLOSPOINTS-1].m_z;
    }
    
    // Flag to say whether the current point can be seen.
    bool bHidden = false ;

    // Style IDs for setting the colour of the line when it's drawn
    TSLStyleID green = TSLComposeRGB( 124, 252, 0 );
    TSLStyleID red   = TSLComposeRGB( 255,   0, 0 );

    Double heightDifference = endHeight - startHeight ;
    Double theoHeight = 0.0;

    // if we're applying the curvature of the earth the maths is a little complicated...
    if( options.m_losEarthCurvature )
    {
      Double earthRadius = 6378137.0;

      Double losDistance = TSLCoordinateConverter::greatCircleDistance(startLat, startLon, endLat, endLon);
      if( crossesDateLine && losDistance > 180 )
      {
        adjust_for_datelineGreatCircle(startLat, startLon, endLat, endLon, heightDifference, losDistance);
      }

      Double gradient = heightDifference/losDistance;

      Double theta = losDistance / earthRadius;
      
      // set constants here for performance
      Double hStart = earthRadius + startHeight;
      Double hEnd   = earthRadius + endHeight;
      Double sinTheta = sin( theta );

      // Loop through all the points on the line.
      for ( int i = 1; i < NUMLOSPOINTS - 1; ++i )
      {
        currentLat = di[i].m_y ;
        currentLon = di[i].m_x ;
        // The height of the terrain at the current point
        currentHeight = di[i].m_z ;

        Double currentDistance = TSLCoordinateConverter::greatCircleDistance(startLat, startLon, currentLat, currentLon);
        if( crossesDateLine && losDistance > 180 )
        {
          adjust_for_datelineGreatCircle(startLat, startLon, currentLat, currentLon, heightDifference, currentDistance);
        }

        // h(gamma) = (R + H1)(R + H2)sin(theta) - R((R + H2)sin(theta - gamma) + (R+H1)sin(gamma)) / (R + H2)sin(theta - gamma) + (R+H1)sin(gamma)
        // where:
        // h = height at current point
        // R = radius of earth
        // H1 = start height ( from ground/sea level )
        // H2 = end height ( from ground/sea level )
        // theta = D/R
        // D = Distance between start and end points
        // gamma = d/R
        // d = Distance between start and current points
        Double gamma = currentDistance / earthRadius;
        Double sinGamma = sin( gamma );
        Double sinDifference = sin( theta - gamma );

        theoHeight = ( ( hStart * hEnd * sinTheta ) - ( earthRadius * ( ( hEnd * sinDifference ) + ( hStart * sinGamma )))) / ( ( hEnd * sinDifference ) + ( hStart * sinGamma ) );

        // If the terrain height is above the theoretical LOS height then it is blocking
        // the line of sight.
        if ( currentHeight > theoHeight )
        {
          bHidden = true ;
          break;
        }

      }
    }
    else // otherwise it's very simple
    {
      Double losDistance = distance(startLon, startLat, endLon, endLat);
      if( crossesDateLine && losDistance > 180 )
      {
        adjust_for_dateline(startLat, startLon, endLat, endLon, heightDifference, losDistance);
      }

      Double gradient = heightDifference/losDistance;

      // Loop through all the points on the line.
      for ( int i = 1; i < NUMLOSPOINTS - 1; ++i )
      {
        currentLat = di[i].m_y ;
        currentLon = di[i].m_x ;
        // the height of the terrain at the current point
        currentHeight = di[i].m_z ;

        Double currentDistance = distance(startLon, startLat, currentLon, currentLat);
        if( crossesDateLine && losDistance > 180 )
        {
          adjust_for_dateline(startLat, startLon, currentLat, currentLon, heightDifference, currentDistance);
        }

        theoHeight = startHeight + ( gradient * currentDistance );

        // If the terrain height is above the theoretical LOS height then it is blocking
        // the line of sight.
        if ( currentHeight > theoHeight )
        {
          bHidden = true ;
          break;
        }
      }
    }

    // Draw the line of sight
    TSLTMC startTmcX, startTmcY ;
    TSLTMC endTmcX, endTmcY ;
    TSLTMC currentTmcX, currentTmcY ;

    if ( m_convert )
    {
      // Convert the points accounting for different coordinate systems
      double lat1, lat2, lat3, lng1, lng2,  lng3;
      const TSLCoordinateSystem *terrainCS = m_doc->terrainCoordinateSystem();
      if( terrainCS )
      {
        terrainCS->MUToLatLong( startLon, startLat, &lat1, &lng1 ) ;
        terrainCS->MUToLatLong( currentLon, currentLat, &lat2, &lng2 ) ;
        terrainCS->MUToLatLong( endLon, endLat, &lat3, &lng3 ) ;
      }
      else
      {
        lat1 = startLat;
        lng1 = startLon;
        lat2 = currentLat;
        lng2 = currentLon;
        lat3 = endLat;
        lng3 = endLon;
      }
      m_drawingSurface->latLongToTMC( lat1, lng1, &startTmcX, &startTmcY ) ;
      m_drawingSurface->latLongToTMC( lat2, lng2, &currentTmcX, &currentTmcY ) ;
      m_drawingSurface->latLongToTMC( lat3, lng3, &endTmcX, &endTmcY ) ;
    }
    else
    {   
      // Coordinate systems are the same, so convert as normal.
      m_drawingSurface->MUToTMC( startLon, startLat, &startTmcX, &startTmcY ) ;
      m_drawingSurface->MUToTMC( currentLon, currentLat, &currentTmcX, &currentTmcY ) ;
      m_drawingSurface->MUToTMC( endLon, endLat, &endTmcX, &endTmcY ) ;
    }

    // Create and draw the first line section (the part of the LOS that can be seen)
    // Create a new coordset and add points.
    TSLCoordSet *coords = new TSLCoordSet ;
    coords->add( startTmcX, startTmcY ) ;
    coords->add( currentTmcX, currentTmcY ) ;

    // Create a polyline.
    TSLPolyline* polyline = m_layer->entitySet()->createPolyline( 1, coords, true ) ;
    if (polyline)
    {
      // The first section of the LOS can be seen, so it should be green
      polyline->setRendering( TSLRenderingAttributeEdgeColour, green ) ;        
      polyline->setRendering( TSLRenderingAttributeEdgeThickness, 3.0 ) ;
      polyline->setRendering( TSLRenderingAttributeEdgeStyle, 1 ) ;        
    }

    // Create and draw the second line section, this is either the section that can't be seen, 
    // or the final point in the line if LOS is blocked
    TSLCoordSet *coords2 = new TSLCoordSet ;
    coords2->add( currentTmcX, currentTmcY ) ;
    coords2->add( endTmcX, endTmcY ) ;

    // Create a polyline.
    TSLPolyline* polyline2 = m_layer->entitySet()->createPolyline( 1, coords2, true ) ;
    if (polyline2)
    {
      // Colour the line appropriately, red if the LOS is blocked
      if(bHidden)
      {
        polyline2->setRendering( TSLRenderingAttributeEdgeColour, red ) ;
      }
      else
      {
        polyline->setRendering( TSLRenderingAttributeEdgeColour, green) ;        
      }

      polyline2->setRendering( TSLRenderingAttributeEdgeThickness, 3.0 ) ;
      polyline2->setRendering( TSLRenderingAttributeEdgeStyle, 1 ) ;        
    }
  }

  m_layer->notifyChanged();

  // clear the flags and any rectangle drawn
  m_interface->clearEchoLine( ) ;
  m_pressed = false ;

  return true ;
}

// Adjusts for line of sight calculations when the line crosses the dateline
void TSLViewModeLOS::adjust_for_dateline(const double start_x, const double start_y, const double end_x, const double end_y,
  double &length, double &dist) const
{
  const double dateline = 180.0;
  const double minusDateline = -180.0;
  length = dateline - start_x - ( -dateline - end_x );
  double height = end_y - start_y;
  double gradient = height / length;
  double datelineY = ( gradient * ( dateline - start_x ) ) + start_y;
  dist = distance( start_x, start_y, dateline, datelineY ) +
    distance( minusDateline, datelineY, end_x, end_y );
}

void TSLViewModeLOS::adjust_for_datelineGreatCircle(const double start_lat, const double start_lon, const double end_lat, const double end_lon,
    double &length, double &dist) const 
{
  const double dateline = 180.0;
  const double minusDateline = -180.0;
  length = dateline - start_lon - ( -dateline - end_lon );
  double height = end_lat - start_lat;
  double gradient = height / length;
  double datelineY = ( gradient * ( dateline - start_lon ) ) + start_lat;
  dist = TSLCoordinateConverter::greatCircleDistance(start_lat, start_lon, datelineY, dateline) + 
         TSLCoordinateConverter::greatCircleDistance(datelineY, dateline, end_lat, end_lon);
}

void TSLViewModeLOS::setCentralMeridian(double meridian)
{
  m_central_meridian = meridian;
}
