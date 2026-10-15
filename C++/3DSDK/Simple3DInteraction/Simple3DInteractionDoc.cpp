// Simple3DInteractionDoc.cpp : implementation of the Simple3DInteractionDoc class
//

#include "stdafx.h"
#include "Simple3DInteraction.h"
#include <direct.h>

#include "Simple3DInteractionDoc.h"
#include "Simple3DInteractionView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Simple3DInteractionDoc

const char * Simple3DInteractionDoc::m_mapLayerName = "map" ;
const char * Simple3DInteractionDoc::m_terrainLayerName = "terrain" ;
const char * Simple3DInteractionDoc::m_stdLayerName = "std" ;


IMPLEMENT_DYNCREATE(Simple3DInteractionDoc, CDocument)

BEGIN_MESSAGE_MAP(Simple3DInteractionDoc, CDocument)
//{{AFX_MSG_MAP(Simple3DInteractionDoc)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG_MAP
ON_COMMAND(ID_FILE_OPENTERRAIN, OnFileOpenterrain)
ON_COMMAND(ID_CREATE_POLYGON, OnCreatePolygon)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Simple3DInteractionDoc construction/destruction

Simple3DInteractionDoc::Simple3DInteractionDoc()
{
  // Initialise my variables
  m_mapDataLayer = 0 ;
  m_terrainDataLayer = 0;
  m_stdDataLayer = 0;
}

Simple3DInteractionDoc::~Simple3DInteractionDoc()
{
  if ( m_mapDataLayer )
  {
    m_mapDataLayer->destroy() ;
    m_mapDataLayer = 0 ;
  }

  if ( m_terrainDataLayer )
  {
    m_terrainDataLayer->destroy() ;
    m_terrainDataLayer = 0 ;
  }

  if (m_stdDataLayer)
  {
    m_stdDataLayer->destroy();
    m_stdDataLayer = 0;
  }
}

/////////////////////////////////////////////////////////////////////////////
// Simple3DInteractionDoc diagnostics

#ifdef _DEBUG
void Simple3DInteractionDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void Simple3DInteractionDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// Simple3DInteractionDoc commands

BOOL Simple3DInteractionDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
  if (!CDocument::OnOpenDocument(lpszPathName))
    return FALSE;
  
  if (!m_mapDataLayer)
  {
    m_mapDataLayer = new TSLMapDataLayer();
  }
  // Clear the error stack, load the map then check for errors.
  TSLThreadedErrorStack::clear() ;

  // Load the map
  // This will automatically load all associated files such as 
  // the .pth and .pal files.  These needed to be loaded
  // directly by the application in earlier versions of MapLink
  m_mapDataLayer->loadData( TSLUTF8Encoder(lpszPathName) ) ;

  // Display any errors that have occurred
  TSLSimpleString msg( "" );
  bool anyErrors = TSLThreadedErrorStack::errorString( msg, "Cannot load map\n" ) ;
  if ( anyErrors )
  {
    // The map load failed, so delete the data layer and path list
    AfxMessageBox( (LPCTSTR)TSLUTF8Decoder(msg), MB_ICONERROR ) ;
    m_mapDataLayer->destroy() ;
    m_mapDataLayer = 0 ;
  }

  return TRUE;
}

bool Simple3DInteractionDoc::addToSurface( TSLDrawingSurfaceBase * surface )
{
  // Add the map to the specified drawing surface,
  if (!m_mapDataLayer)
  {
    m_mapDataLayer = new TSLMapDataLayer();
  }
  surface->addDataLayer( m_mapDataLayer, m_mapLayerName ) ;

  // Helps text and symbols draw across texture tiles.
  surface->setDataLayerProps(m_mapLayerName, TSLPropertyLoadedSymbolsAndTextViewExpansion, 10);

  if (!m_terrainDataLayer)
  {
    m_terrainDataLayer = new TSL3DTerrainDataLayer;
  }
  surface->addDataLayer( m_terrainDataLayer, m_terrainLayerName ) ;

  if (!m_stdDataLayer)
  {
    m_stdDataLayer = new TSL3DStandardDataLayer ;
  }
  surface->addDataLayer( m_stdDataLayer, m_stdLayerName);
  surface->setDataLayerProps( m_stdLayerName, TSLPropertyVisible, true ) ;
  surface->setDataLayerProps( m_stdLayerName, TSLPropertyDetect,  true ) ;

  return true ;
}

bool Simple3DInteractionDoc::detachFromSurface( TSLDrawingSurfaceBase * surface )
{
  if ( !m_mapDataLayer  )
    return false;

  // Remove the map to the specified drawing surface,
  surface->removeDataLayer( m_mapLayerName ) ;
  // Remove the terrain database from the specified drawing surface,
  surface->removeDataLayer( m_terrainLayerName ) ;

  surface->removeDataLayer(m_stdLayerName);

  return true ;
}

void Simple3DInteractionDoc::DeleteContents() 
{
  // Clean up by destroying the map and pathlist
  if ( m_mapDataLayer )
  {
    m_mapDataLayer->destroy() ;
    m_mapDataLayer = 0 ;
  }

  if ( m_terrainDataLayer )
  {
    m_terrainDataLayer->destroy() ;
    m_terrainDataLayer = 0 ;
  }

  if (m_stdDataLayer)
  {
    m_stdDataLayer->destroy();
    m_stdDataLayer = 0;
  }

  CDocument::DeleteContents();
}





void Simple3DInteractionDoc::OnFileOpenterrain()
{
  CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, _T("Terrain Database Files (*.tdf)|*.tdf|All Files (*.*)|*.*||") );
  
  if ( dlg.DoModal() == IDOK )
  {
    CString file_path = (LPCTSTR)dlg.GetPathName();
  
    if (!m_terrainDataLayer)
    {
      m_terrainDataLayer = new TSL3DTerrainDataLayer;
    }

    // Create a map data layer
    m_terrainDataLayer->removeData() ;
    
    // Clear the error stack, load the map then check for errors.
    TSLThreadedErrorStack::clear() ;

    // Load the map
    // This will automatically load all associated files such as 
    // the .pth and .pal files.  These needed to be loaded
    // directly by the application in earlier versions of MapLink
    m_terrainDataLayer->loadData( TSLUTF8Encoder(file_path) ) ;

    // Display any errors that have occurred
    TSLSimpleString msg( "" );
    bool anyErrors = TSLThreadedErrorStack::errorString( msg, "Cannot load terrain\n" ) ;
    if ( anyErrors )
    {
      // The map load failed, so display an error
      AfxMessageBox( (LPCTSTR)TSLUTF8Decoder(msg), MB_ICONERROR ) ;
    }
    else
    {
      m_terrainDataLayer->notifyChanged() ;
    }
  }
}

void Simple3DInteractionDoc::OnCreatePolygon()
{
  TSL3DEntitySet *es = m_stdDataLayer->entitySet();
  TSL3DCoordSet *cs = new TSL3DCoordSet;

  // Specify the polygon's position in latitude and longitude.
  // The altitude type defaults to TSL3DAltitudeTypeMeanSeaLevel.
  // Any other altitude type needs to be specified for every coordinate to which it should apply.
  cs->add(TSL3DCoord(50.0, 50.0, 100000.0));
  cs->add(TSL3DCoord(60.5, 50.0, 100000.0));
  cs->add(TSL3DCoord(60.5, 60.5, 100000.0));
  cs->add(TSL3DCoord(50.5, 60.5, 100000.0));

  TSL3DPolygon *polygon = es->create3DPolygon(0, cs);

  polygon->setRendering(TSLRenderingAttributeEdgeColour, 181);
  polygon->setRendering(TSLRenderingAttributeEdgeStyle, 1);
  polygon->setRendering(TSLRenderingAttributeEdgeThickness, 2.0);
  // Note that the edge thickness can be set in map units or in pixels.
  // Setting the edge thickness to be displayed in map units rather than pixels means that the
  // edge thickness changes as the user zooms in and out.  However, there is a performance cost associated
  // with this and where possible, to ensure best performance, TSLDimensionUnitsPixels (the default)
  // should be used.
  // polygon->setRendering(TSLRenderingAttributeEdgeThicknessUnits, TSLDimensionUnitsMapUnits);
  polygon->setRendering(TSLRenderingAttributeFillStyle, 1);
  polygon->setRendering(TSLRenderingAttributeFillColour, 31);

  // create an inner
  cs->clear();
  cs->add(TSL3DCoord(55.0, 55.0, 100000.0));
  cs->add(TSL3DCoord(57.0, 55.0, 100000.0));
  cs->add(TSL3DCoord(57.0, 57.0, 100000.0));
  cs->add(TSL3DCoord(55.0, 57.0, 100000.0));
  polygon->addInner(cs);

  /////////////////////////////////////////////
  // Create a polygon which parially lies below the
  // surface of the sea. Near the Falklands.
  cs->clear();

  // Specify the polygon's position in latitude and longitude.
  // The altitude type defaults to TSL3DAltitudeTypeMeanSeaLevel.
  // Any other altitude type needs to be specified for every coordinate to which it should apply.
  cs->add(TSL3DCoord(-50.0, -50.0, -10000.0, TSL3DAltitudeTypeHighestResolutionTerrain));
  cs->add(TSL3DCoord(-60.5, -50.0, -10000.0, TSL3DAltitudeTypeHighestResolutionTerrain));
  cs->add(TSL3DCoord(-60.5, -60.5, 100000.0, TSL3DAltitudeTypeHighestResolutionTerrain));
  cs->add(TSL3DCoord(-50.5, -60.5, 100000.0, TSL3DAltitudeTypeHighestResolutionTerrain));

  TSL3DPolygon *polygon1 = es->create3DPolygon(0, cs);

  polygon1->setRendering(TSLRenderingAttributeEdgeColour, 181);
  polygon1->setRendering(TSLRenderingAttributeEdgeStyle, 1);
  polygon1->setRendering(TSLRenderingAttributeEdgeThickness, 2.0);
  // Note that the edge thickness can be set in map units or in pixels.
  // Setting the edge thickness to be displayed in map units rather than pixels means that the
  // edge thickness changes as the user zooms in and out.  However, there is a performance cost associated
  // with this and where possible, to ensure best performance, TSLDimensionUnitsPixels (the default)
  // should be used.
  // polygon->setRendering(TSLRenderingAttributeEdgeThicknessUnits, TSLDimensionUnitsMapUnits);
  polygon1->setRendering(TSLRenderingAttributeFillStyle, 1);
  polygon1->setRendering(TSLRenderingAttributeFillColour, 31);

  /////////////////////////////////////////////
  // Create a polygon which parially lies below the
  // surface of the sea. 
  cs->clear();

  // Specify the polygon's position in latitude and longitude.
  // The altitude type defaults to TSL3DAltitudeTypeMeanSeaLevel.
  // Any other altitude type needs to be specified for every coordinate to which it should apply.
  cs->add(TSL3DCoord(-50.0, 50.0, -10000.0, TSL3DAltitudeTypeDynamicTerrain));
  cs->add(TSL3DCoord(-60.5, 50.0, -10000.0, TSL3DAltitudeTypeDynamicTerrain));
  cs->add(TSL3DCoord(-60.5, 60.5, 100000.0, TSL3DAltitudeTypeDynamicTerrain));
  cs->add(TSL3DCoord(-50.5, 60.5, 100000.0, TSL3DAltitudeTypeDynamicTerrain));

  TSL3DPolygon *polygon2 = es->create3DPolygon(0, cs);

  polygon2->setRendering(TSLRenderingAttributeEdgeColour, 182);
  polygon2->setRendering(TSLRenderingAttributeEdgeStyle, 1);
  polygon2->setRendering(TSLRenderingAttributeEdgeThickness, 2.0);
  // Note that the edge thickness can be set in map units or in pixels.
  // Setting the edge thickness to be displayed in map units rather than pixels means that the
  // edge thickness changes as the user zooms in and out.  However, there is a performance cost associated
  // with this and where possible, to ensure best performance, TSLDimensionUnitsPixels (the default)
  // should be used.
  // polygon->setRendering(TSLRenderingAttributeEdgeThicknessUnits, TSLDimensionUnitsMapUnits);
  polygon2->setRendering(TSLRenderingAttributeFillStyle, 1);
  polygon2->setRendering(TSLRenderingAttributeFillColour, 31);

    /////////////////////////////////////////////
  // Create a polygon which parially lies below the
  // surface of the sea. 
  cs->clear();

  // Specify the polygon's position in latitude and longitude.
  // The altitude type defaults to TSL3DAltitudeTypeMeanSeaLevel.
  // Any other altitude type needs to be specified for every coordinate to which it should apply.
  cs->add(TSL3DCoord(50.0, -50.0, -10000.0));
  cs->add(TSL3DCoord(60.5, -50.0, -10000.0));
  cs->add(TSL3DCoord(60.5, -60.5, 100000.0));
  cs->add(TSL3DCoord(50.5, -60.5, 100000.0));

  TSL3DPolygon *polygon3 = es->create3DPolygon(0, cs);

  polygon3->setRendering(TSLRenderingAttributeEdgeColour, 182);
  polygon3->setRendering(TSLRenderingAttributeEdgeStyle, 1);
  polygon3->setRendering(TSLRenderingAttributeEdgeThickness, 2.0);
  // Note that the edge thickness can be set in map units or in pixels.
  // Setting the edge thickness to be displayed in map units rather than pixels means that the
  // edge thickness changes as the user zooms in and out.  However, there is a performance cost associated
  // with this and where possible, to ensure best performance, TSLDimensionUnitsPixels (the default)
  // should be used.
  // polygon->setRendering(TSLRenderingAttributeEdgeThicknessUnits, TSLDimensionUnitsMapUnits);
  polygon3->setRendering(TSLRenderingAttributeFillStyle, 1);
  polygon3->setRendering(TSLRenderingAttributeFillColour, 31);

  m_stdDataLayer->notifyChanged();
  cs->destroy();

  //////////////////////////////////////////////////
  // Cause the view to be updated.
  POSITION pos = GetFirstViewPosition();
  if( !pos )
  {
    // No view exists, so we cannot proceed.
    return;
  }

  Simple3DInteractionView* pView = (Simple3DInteractionView*)GetNextView( pos );
  pView->InvalidateRect( 0, false );
}
