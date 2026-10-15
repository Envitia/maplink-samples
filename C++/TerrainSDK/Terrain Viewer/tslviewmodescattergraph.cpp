//	********************************************************
//	*           Source Control Information
//	********************************************************
//	$Archive: /MapLinkWizard/Template/TSLViewModeScatterGraph.cpp $
//	$Author$
//	$Date$
//	$Revision$
//	********************************************************

#include "stdafx.h"
#include "resource.h"
#include "areadialog.h"
#include "TerrainViewerDoc.h"
#include "TSLViewModeScatterGraph.h"
#include "MainFrm.h"

TSLViewModeScatterGraph::TSLViewModeScatterGraph( TSLViewModeInterface * iViewMode, 
  unsigned long modeID, CTerrainViewerDoc * pDoc ) 
  : TSLViewMode( iViewMode, modeID )
{
  // Clear down the starting flags.
  m_startX = 0 ;
  m_startY = 0 ;
  m_pressed = false ;
  m_doc = pDoc ;
}

TSLViewModeScatterGraph::~TSLViewModeScatterGraph()
{
  // If are in the middle of an operation, then remove any rubber rectangle,
  // and release the mouse capture.
  if ( m_pressed )
  {
    m_interface->clearEchoRectangle( ) ;
    m_interface->releaseMouse( ) ;
  }
}

bool TSLViewModeScatterGraph::OnMouseMove(long x, long y, bool shift, bool control)
{
  double mux, muy;
  
  // Only do anything if we have pressed
  if ( !m_pressed )
  {
    double lat, lon;

    m_drawingSurface->DUToLatLong( x, y, &lat, &lon ) ;

    if ( m_doc->isConversionRequired() )
    {
      const TSLCoordinateSystem *terrainCS = m_doc->terrainCoordinateSystem();
      if( terrainCS )
      {
        terrainCS->latLongToMU ( lat, lon, &mux, &muy );
      }
      else
      {
	// The terrain database is already in lat/lon
	mux = lon;
	muy = lat;
      }
    }
    else
    {
      m_drawingSurface->DUToMU( x, y, &mux, &muy ) ;
    }
    return false ;
  }

  // Calculate delta offsets from the press event location
  long dx = x - m_startX ;
  long dy = y - m_startY ;

  if ( abs( dx ) < 3 && abs( dy ) < 3 )
  {
    // If we haven't moved more than 3 pixels, then assume its just mouse wobble.
    m_interface->clearEchoRectangle( ) ;
  }
  else
  {
    // We have moved more than 3 pixels from the start position.
    // Set the position of the echo rectangle. Automatically causes it to be drawn
    m_interface->setEchoRectangle( m_startX, m_startY, x, y ) ;
  }

  // We never want a redraw here
  return false ;
}

bool TSLViewModeScatterGraph::OnLButtonDown(long x, long y, bool shift, bool control)
{
  // Remember where the event occurred but wait until the release
  // before doing anything.
  m_startX = x ;
  m_startY = y ;
  m_pressed = true ;

  // Capture the mouse so that we get the Button Up event even if the user leaves
  // the window.
  m_interface->captureMouse( ) ;
  return false ;
}

bool TSLViewModeScatterGraph::OnRButtonDown(long x, long y, bool shift, bool control)
{
  double mux, muy;
 
    double lat, lon;

    m_drawingSurface->DUToLatLong( x, y, &lat, &lon ) ;

    if ( m_doc->isConversionRequired() )
    {
      const TSLCoordinateSystem *terrainCS = m_doc->terrainCoordinateSystem();
      if( terrainCS )
      {
        terrainCS->latLongToMU ( lat, lon, &mux, &muy );
      }
      else
      {
        // The terrain database is already in lat/lon
        mux = lon;
        muy = lat;
      }
    }
    else
    {
      m_drawingSurface->DUToMU( x, y, &mux, &muy ) ;
    }

  //  m_pressed = true ;

  // Capture the mouse so that we get the Button Up event even if the user leaves
  // the window.
  //  m_interface->captureMouse( ) ;
  return false ;
}

bool TSLViewModeScatterGraph::OnLButtonUp(long x, long y, bool shift, bool control)
{
  TSLTerrainDatabase* terrainDB(m_doc->getTerrainDB());
  if (!terrainDB)
  {
    return false;
  }
  // Now the user has released the button, we can deduce what they want to do.

  // Release the mouse capture.
  m_interface->releaseMouse( ) ;

  // Firstly clear the flags and any rectangle drawn.
  m_interface->clearEchoRectangle( ) ;
  m_pressed = false ;

  // Calculate the size of any rectangle dragged
  long dx = x - m_startX ;
  long dy = y - m_startY ;

  if ( abs( dx ) < 3 && abs( dy ) < 3 )
  {
    // If we haven't moved more than 3 pixels, then assume its just mouse wobble.
    // Area in by the configured percentage and request redraw if successful.
    return false;
  }

  // Here we have a rectangle to Area to.
  double lat1, lon1, lat2, lon2, lat3, lon3;
  double mux1, muy1, mux2, muy2;

  // Calculate the corners in map units or lat/lon
  m_drawingSurface->DUToLatLong( m_startX, m_startY, &lat1, &lon1 ) ;
  m_drawingSurface->DUToLatLong( x, y, &lat2, &lon2 ) ;

  if ( m_doc->isConversionRequired() )
  {
    const TSLCoordinateSystem *terrainCS = m_doc->terrainCoordinateSystem();
    if( terrainCS )
    {
      terrainCS->latLongToMU ( lat1, lon1, &mux1, &muy1 );
      terrainCS->latLongToMU ( lat2, lon2, &mux2, &muy2 ) ;
    }
    else
    {
      // The database is already in lat/lon
      mux1 = lon1;
      muy1 = lat1;
      mux2 = lon2;
      muy2 = lat2;
    }
  }
  else
  {
    m_drawingSurface->DUToMU( m_startX, m_startY, &mux1, &muy1 ) ;
    m_drawingSurface->DUToMU( x, y, &mux2, &muy2 ) ;
  }

  if ( lat2 < lat1 )
  {
    lat3 = lat1;
    lat1 = lat2;
    lat2 = lat3;
  }
  if ( lon2 < lon1 )
  {
    lon3 = lon1;
    lon1 = lon2;
    lon2 = lon3;
  }
  if ( mux2 < mux1 )
  {
    double tmp = mux2;
    mux2 = mux1;
    mux1 = tmp;
  }
  if ( muy2 < muy1 )
  {
    double tmp = muy2;
    muy2 = muy1;
    muy1 = tmp;
  }

	// Setup the terrain database, so that an appropriate layer and resolution
	// will be used by the subsequent query.
	TSLDeviceUnits dux1, duy1, dux2, duy2;
	m_drawingSurface->getDUExtent(&dux1, &duy1, &dux2, &duy2);
    terrainDB->displayExtent(std::abs(dux2 - dux1), std::abs(duy2 - duy1), mux1, muy1, mux2, muy2);

	AreaDialog dlg;
	int size = 100;

	TSLTerrainDataItem* data = new TSLTerrainDataItem[ size * size ];
	double* dblData = new double[ size * size ];
	memset( data, 0, size * size * sizeof( TSLTerrainDataItem ) );
    terrainDB->queryArea( mux1, muy1, mux2, muy2, size, size, data );
	for ( int i = 0; i < ( size * size ); i++ )
	{
		dblData[i] = data[i].m_z;
		if ( dblData[i] > 5000.0 )
			dblData[i] = 0.0;
	}
	dlg.setData( lat1, lon1, lat2, lon2, size, size, dblData );

	dlg.DoModal();
	delete[] data;
  delete[] dblData;

  return false;

}

bool TSLViewModeScatterGraph::OnMButtonUp( long x, long y, bool shift, bool control )
{
  return false;
}
