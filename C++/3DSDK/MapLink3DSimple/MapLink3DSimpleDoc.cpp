// MapLink3DSimpleDoc.cpp : implementation of the MapLink3DSimpleDoc class
//

#include "stdafx.h"
#include "MapLink3DSimple.h"
#include <direct.h>

#include "MapLink3DSimpleDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif 

extern __declspec(dllimport) bool m_loadInMainThread ;

/////////////////////////////////////////////////////////////////////////////
// MapLink3DSimpleDoc

const char * MapLink3DSimpleDoc::m_mapLayerName = "map" ;
const char * MapLink3DSimpleDoc::m_terrainLayerName = "terrain" ;

IMPLEMENT_DYNCREATE(MapLink3DSimpleDoc, CDocument)

BEGIN_MESSAGE_MAP(MapLink3DSimpleDoc, CDocument)
//{{AFX_MSG_MAP(MapLink3DSimpleDoc)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG_MAP
ON_COMMAND(ID_FILE_LOADTERRAINDATABASE, OnFileLoadterraindatabase)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MapLink3DSimpleDoc construction/destruction

MapLink3DSimpleDoc::MapLink3DSimpleDoc() 
 : m_defaultCreated(false)
  , m_newMap(false)
{
  // Initialise my variables
  m_mapDataLayer = new TSLMapDataLayer ;
  m_terrainDataLayer = new TSL3DTerrainDataLayer ;
  m_stdDataLayer = new TSL3DStandardDataLayer ;
  m_dataLayer = new TSLStandardDataLayer;
}

MapLink3DSimpleDoc::~MapLink3DSimpleDoc()
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

  if (m_dataLayer)
  {
    m_dataLayer->destroy();
    m_dataLayer = 0;
  }
}

/////////////////////////////////////////////////////////////////////////////
// MapLink3DSimpleDoc diagnostics

#ifdef _DEBUG
void MapLink3DSimpleDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void MapLink3DSimpleDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// MapLink3DSimpleDoc commands

BOOL MapLink3DSimpleDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
  if (!CDocument::OnOpenDocument(lpszPathName))
    return FALSE;

  // We delay loading the map until addToSurface is called.
  m_newMap = true;
  m_mapName = TSLUTF8Encoder(lpszPathName);

  return TRUE;
}

void MapLink3DSimpleDoc::loadMap()
{
  if (!m_newMap)
    return ;

  // Create a map data layer
  m_mapDataLayer->removeData() ;
  
  // Clear the error stack, load the map then check for errors.
  TSLThreadedErrorStack::clear() ;

  // Load the map
  // This will automatically load all associated files such as 
  // the .pth and .pal files.  These needed to be loaded
  // directly by the application in earlier versions of MapLink
  m_mapDataLayer->loadData( m_mapName.c_str() ) ;

  // Display any errors that have occurred
  TSLSimpleString msg;
  bool anyErrors = TSLThreadedErrorStack::errorString( msg, "Cannot load map\n" ) ;
  if ( anyErrors )
  {
    // The map load failed, so set an error
    AfxMessageBox( (LPCTSTR)TSLUTF8Decoder(msg), MB_ICONERROR ) ;
  }
  else
  {
    m_mapDataLayer->notifyChanged() ;
  }
}

bool MapLink3DSimpleDoc::addToSurface( TSLDrawingSurfaceBase * surface )
{
  if ( !m_mapDataLayer && !m_terrainDataLayer && !m_stdDataLayer && !m_dataLayer)
    return false;

  loadMap(); // load the map.

  // Add the map to the specified drawing surface,
  surface->addDataLayer( m_mapDataLayer, m_mapLayerName ) ;
  surface->addDataLayer( m_terrainDataLayer, m_terrainLayerName ) ;
  surface->addDataLayer( m_stdDataLayer, "std");
  surface->setDataLayerProps( "std", TSLPropertyVisible, true ) ;
  surface->setDataLayerProps( "std", TSLPropertyDetect,  true ) ;

  surface->addDataLayer(m_dataLayer, "std2d");

  return true ;
}

bool MapLink3DSimpleDoc::detachFromSurface( TSLDrawingSurfaceBase * surface )
{
  // Remove the map from the specified drawing surface,
  surface->removeDataLayer( m_mapLayerName ) ;

  // Remove the terrain database from the specified drawing surface,
  surface->removeDataLayer( m_terrainLayerName ) ;

  surface->removeDataLayer("std");
  surface->removeDataLayer("std2d");

  return true ;
}

void MapLink3DSimpleDoc::DeleteContents() 
{
  CDocument::DeleteContents();
}

void MapLink3DSimpleDoc::OnFileLoadterraindatabase()
{
  CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, _T("Terrain Database Files (*.tdf)|*.tdf|All Files (*.*)|*.*||") );
  
  if ( dlg.DoModal() == IDOK )
  {
    CString file_path = (LPCTSTR)dlg.GetPathName();
  
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
    TSLSimpleString msg;
    bool anyErrors  = TSLThreadedErrorStack::errorString( msg, "Cannot load terrain\n" ) ;
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

void MapLink3DSimpleDoc::createPolygon()
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

  m_stdDataLayer->notifyChanged();
  cs->destroy();
}

void MapLink3DSimpleDoc::createExtrudedPolygon()
{
  // Need both 2D and 3D entity sets.
  TSLEntitySet *et = m_dataLayer->entitySet();
  TSL3DEntitySet *es = m_stdDataLayer->entitySet();

  // When creating an extruded primitive specify the vertices in latitude and longitude
  // but use the scaled values to create the 2D coordinate set.
  TSL3DCoord p1(12.0, 12.0, 100000.0);
  TSL3DCoord p2(12.0, 20.0, 100000.0);
  TSL3DCoord p3(20.0, 20.0, 100000.0);
  TSL3DCoord p4(20.0, 12.0, 100000.0);

  TSLCoordSet *cs2d = new TSLCoordSet;
  cs2d->add(TSLCoord(p1.scaledLon(), p1.scaledLat()));
  cs2d->add(TSLCoord(p2.scaledLon(), p2.scaledLat()));
  cs2d->add(TSLCoord(p3.scaledLon(), p3.scaledLat()));
  cs2d->add(TSLCoord(p4.scaledLon(), p4.scaledLat()));
  TSLPolygon *polygon = et->createPolygon(0, cs2d);

  cs2d->clear();

  TSL3DCoord p5(15.0, 15.0, 100000.0);
  TSL3DCoord p6(15.0, 17.0, 100000.0);
  TSL3DCoord p7(17.0, 17.0, 100000.0);
  TSL3DCoord p8(17.0, 15.0, 100000.0);

  cs2d->add(TSLCoord(p5.scaledLon(), p5.scaledLat()));
  cs2d->add(TSLCoord(p6.scaledLon(), p6.scaledLat()));
  cs2d->add(TSLCoord(p7.scaledLon(), p7.scaledLat()));
  cs2d->add(TSLCoord(p8.scaledLon(), p8.scaledLat()));
  polygon->addInner(cs2d);

  TSLExtruded2DPolygon *extPolygon = es->createExtruded2DPolygon(0, polygon);
  extPolygon->altitude(30000.0);
  extPolygon->extrudedHeight(10000.0);

  extPolygon->setRendering(TSLRenderingAttributeEdgeColour, 181);
  extPolygon->setRendering(TSLRenderingAttributeEdgeStyle, 1);
  extPolygon->setRendering(TSLRenderingAttributeEdgeThicknessUnits, TSLDimensionUnitsPixels);
  extPolygon->setRendering(TSLRenderingAttributeEdgeThickness, 2.0);
  extPolygon->setRendering(TSLRenderingAttributeFillStyle, 1);
  extPolygon->setRendering(TSLRenderingAttributeFillColour, 21);

  m_stdDataLayer->notifyChanged();
  cs2d->destroy();
}

void MapLink3DSimpleDoc::createDefaultPolygon()
{  
  if (!m_defaultCreated)
  {
    // Render a white polygon to demonstrate the lighting effect.
    TSL3DEntitySet *es = m_stdDataLayer->entitySet();
    TSL3DCoordSet *cs = new TSL3DCoordSet;

    cs->add(TSL3DCoord(70.0, 65.0, 100000.0));
    cs->add(TSL3DCoord(80.5, 65.0, 100000.0));
    cs->add(TSL3DCoord(80.5, 70.5, 100000.0));
    cs->add(TSL3DCoord(70.0, 70.5, 100000.0));

    TSL3DPolygon *polygon = es->create3DPolygon(0, cs);

    polygon->setRendering(TSLRenderingAttributeEdgeColour, 181);
    polygon->setRendering(TSLRenderingAttributeEdgeStyle, 1);
    polygon->setRendering(TSLRenderingAttributeEdgeThickness, 2.0);
    polygon->setRendering(TSLRenderingAttributeFillStyle, 1);
    polygon->setRendering(TSLRenderingAttributeFillColour, 216);

    cs->destroy();
    m_defaultCreated = true;
  }
}
