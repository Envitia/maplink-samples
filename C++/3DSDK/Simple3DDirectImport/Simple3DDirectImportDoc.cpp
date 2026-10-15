////////////////////////////////////////////////////////
// Copyright (c) 1998 to 2017 by Envitia Group PLC
////////////////////////////////////////////////////////
// Simple3DDirectImportDoc.cpp : implementation of the Simple3DDirectImportDoc class
//

#include "stdafx.h"
#include "Simple3DDirectImport.h"
#include <direct.h>

#include "Simple3DDirectImportDoc.h"
#include "Simple3DDirectImportView.h"
#include "OpenServiceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Simple3DDirectImportDoc

const char * Simple3DDirectImportDoc::m_mapLayerName = "map" ;
const char * Simple3DDirectImportDoc::m_terrainLayerName = "terrain" ;
const char * Simple3DDirectImportDoc::m_stdLayerName = "std" ;
const char * Simple3DDirectImportDoc::m_cadrgLayerName = "cadrg" ;
const char * Simple3DDirectImportDoc::m_wmsLayerName = "wms";

IMPLEMENT_DYNCREATE(Simple3DDirectImportDoc, CDocument)

BEGIN_MESSAGE_MAP(Simple3DDirectImportDoc, CDocument)
  //{{AFX_MSG_MAP(Simple3DDirectImportDoc)
  // NOTE - the ClassWizard will add and remove mapping macros here.
  //    DO NOT EDIT what you see in these blocks of generated code!
  //}}AFX_MSG_MAP
  ON_COMMAND(ID_FILE_OPENTERRAIN, OnFileOpenterrain)
  ON_COMMAND(ID_FILE_CADRG, OnFileLoadcadrgdata)
  ON_COMMAND(ID_CREATE_POLYGON, OnCreatePolygon)
  ON_COMMAND(ID_VIEW_CADRGONTOP, &Simple3DDirectImportDoc::OnViewCadrgontop)
  ON_UPDATE_COMMAND_UI(ID_VIEW_CADRGONTOP, &Simple3DDirectImportDoc::OnUpdateViewCadrgontop)
  ON_COMMAND(ID_VIEW_WMSONTOP, &Simple3DDirectImportDoc::OnViewWmsontop)
  ON_UPDATE_COMMAND_UI(ID_VIEW_WMSONTOP, &Simple3DDirectImportDoc::OnUpdateViewWmsontop)
  ON_COMMAND(ID_VIEW_CADRGTRANSPARENT, &Simple3DDirectImportDoc::OnViewCadrgtransparent)
  ON_UPDATE_COMMAND_UI(ID_VIEW_CADRGTRANSPARENT, &Simple3DDirectImportDoc::OnUpdateViewCadrgtransparent)
  ON_COMMAND(ID_VIEW_WMSTRANSPARENT, &Simple3DDirectImportDoc::OnViewWmstransparent)
  ON_UPDATE_COMMAND_UI(ID_VIEW_WMSTRANSPARENT, &Simple3DDirectImportDoc::OnUpdateViewWmstransparent)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Simple3DDirectImportDoc construction/destruction

Simple3DDirectImportDoc::Simple3DDirectImportDoc()
: m_mapDataLayer( NULL ), m_terrainDataLayer( NULL ), m_stdDataLayer( NULL ),
m_newMap( false ), m_mapName( "" ), m_callbacks( NULL ), m_wmsDataLayer( NULL ),
m_cadrgDataLayer( NULL ), m_surface( NULL ), m_cadrgTransparent( false ),
m_wmsTransparent( false ), m_onTop( "cadrg" )
{
}

Simple3DDirectImportDoc::~Simple3DDirectImportDoc()
{
  if ( m_mapDataLayer )
  {
    m_mapDataLayer->destroy() ;
    m_mapDataLayer = NULL ;
  }

  if ( m_terrainDataLayer )
  {
    m_terrainDataLayer->destroy() ;
    m_terrainDataLayer = NULL ;
  }

  if (m_stdDataLayer)
  {
    m_stdDataLayer->destroy();
    m_stdDataLayer = NULL ;
  }

  if (m_wmsDataLayer)
  {
    m_wmsDataLayer->destroy();
    m_wmsDataLayer = NULL ;
  }

  if (m_cadrgDataLayer)
  {
    m_cadrgDataLayer->destroy();
    m_cadrgDataLayer = NULL ;
  }

  if(m_callbacks)
  {
    delete m_callbacks ;
    m_callbacks = NULL ;
  }
}

/////////////////////////////////////////////////////////////////////////////
// Simple3DDirectImportDoc diagnostics

#ifdef _DEBUG
void Simple3DDirectImportDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void Simple3DDirectImportDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// Simple3DDirectImportDoc commands

BOOL Simple3DDirectImportDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
  if (!CDocument::OnOpenDocument(lpszPathName))
  {
    return FALSE;
  }
  else
  {
    m_newMap = true ;
    m_mapName = TSLUTF8Encoder(lpszPathName) ;
    return TRUE;
  }
}

void Simple3DDirectImportDoc::loadMap()
{
  if (!m_newMap) 
  {
    return ;
  }
  if ( !m_mapDataLayer )
  {
    m_mapDataLayer = new TSLMapDataLayer();
  }
  if ( !m_mapDataLayer )
  {
    return ;
  }

  // Clear the error stack, load the map then check for errors.
  TSLThreadedErrorStack::clear() ;

  // Load the map
  // This will automatically load all associated files such as 
  // the .pth and .pal files.  These needed to be loaded
  // directly by the application in earlier versions of MapLink
  m_mapDataLayer->loadData( m_mapName.c_str() ) ;

  // Display any errors that have occurred
  TSLSimpleString msg( "" );
  bool anyErrors = TSLThreadedErrorStack::errorString( msg, "Cannot load map\n" ) ;
  if ( anyErrors )
  {
    // The map load failed, so delete the data layer and path list
    AfxMessageBox( (LPCTSTR)TSLUTF8Decoder(msg), MB_ICONERROR ) ;
    m_mapDataLayer->removeData() ;
  }
  else
  {
    m_mapDataLayer->notifyChanged() ;
  }
}

BOOL Simple3DDirectImportDoc::OnOpenWMSDocument(LPCTSTR lpszPathName) 
{
  void* hWnd = (void*)AfxGetMainWnd()->m_hWnd;
  if ( !hWnd )
  {
    return false;
  }
  COpenServiceDlg openDlg(::AfxGetMainWnd());
  // Create a WMS layer setup if necessary.
  if ( !m_callbacks )
  {
    m_callbacks = new WMSSettingsCallbacks(&openDlg);
  }
  else
  {
    m_callbacks->setDlg(&openDlg);
  }
  if ( !m_callbacks )
  {
    return false;
  }

  if ( !m_wmsDataLayer )
  {
    m_wmsDataLayer = new TSLWMSDataLayer(m_callbacks);
    m_wmsDataLayer->extentExpansion(10.0) ;
  }
  if ( !m_wmsDataLayer )
  {
    return false;
  }
  //
  // Configure the following on the WMS data-layer before pointing it at a WMS Server
  //

  // Decent Cache size or the 3D will request too frequently
  m_wmsDataLayer->cacheSize(64*1024); 

  // Setup the Callbacks - these will not be called by the 3D or Accelerator SDKs as
  // we clone the layer into a background thread.
  m_wmsDataLayer->setDefaultLoaderCallbacks(TileLoaderCallback, hWnd, AllTilesLoadedCallback, hWnd);
  m_wmsDataLayer->synchronousLoadStrategy(false);
  //m_wmsDataLayer->setSynchronousLoading(true); // Not required for MapLink 6.0 and newer
  m_wmsDataLayer->tileLoadOrder(TSLWMSDataLayer::ClockwiseSpiral_CentreStart);

  // Query the map layer for the coordinate system
  const TSLCoordinateSystem *coordSystem = m_mapDataLayer ? m_mapDataLayer->queryCoordinateSystem() : NULL ;

  // The WMS Coordinate system must match the Map/3D settings.
  if (coordSystem)
  {
    double xoffset, yoffset;
    coordSystem->getMapOffsets(&xoffset, &yoffset);
    m_wmsDataLayer->setLinearTransformParameters(false, xoffset, yoffset, coordSystem->getTMCperMU());
  }
  else
  {
    // This is the settings for the 3D... The map for 3D should have these settings.
    m_wmsDataLayer->setLinearTransformParameters(false, 0, 0, 50);
  }

  // Query the user for the layers and WMS configuration.
  //
  // Example Public WMS's:
  //  http://wms1.ccgis.de/cgi-bin/mapserv?map=/data/umn/germany/germany.map&
  //  http://217.33.30.11/envitia.wms.ows7.met/wms.aspx?
  //  http://www2.demis.nl/wms/wms.ashx?

  openDlg.setDataLayer(m_wmsDataLayer);
  if ( openDlg.DoModal() != IDOK )
  {
    m_wmsDataLayer->destroy();
    m_wmsDataLayer = NULL ;

    delete m_callbacks;
    m_callbacks = NULL;
  }
  else
  {
    m_callbacks->setDlg(NULL);
  }

  return TRUE;
}

bool Simple3DDirectImportDoc::addToSurface( TSL3DWinGLSurface * surface )
{
  if(!surface)
  {
    return false ;
  }

  // Load the map specified by the user.
  Simple3DDirectImportDoc::loadMap() ;

  // Add the map to the specified drawing surface,   
  if (!m_mapDataLayer)
  {
    m_mapDataLayer = new TSLMapDataLayer();
  }
  surface->addDataLayer( m_mapDataLayer, m_mapLayerName ) ;

  // Helps text and symbols draw across texture tiles.
  surface->setDataLayerProps(m_mapLayerName, TSLPropertyLoadedSymbolsAndTextViewExpansion, 10);

  // Terrain
  if (!m_terrainDataLayer)
  {
    m_terrainDataLayer = new TSL3DTerrainDataLayer;
  }
  surface->addDataLayer( m_terrainDataLayer, m_terrainLayerName ) ;

  // 3D Standard Data-layer - polygons, polylines etc...
  if (!m_stdDataLayer)
  {
    m_stdDataLayer = new TSL3DStandardDataLayer ;
  }
  surface->addDataLayer( m_stdDataLayer, m_stdLayerName);

  // CADRG Data-layer
  if (!m_cadrgDataLayer)
  {
    m_cadrgDataLayer = new TSLCADRGDataLayer ;
  }
  surface->addDataLayer( m_cadrgDataLayer, m_cadrgLayerName);

  // WMS Data-layer - assumes it is created.
  if ( m_wmsDataLayer )
  {
    surface->addDataLayer( m_wmsDataLayer, m_wmsLayerName);
  }

  // Set layer properties.
  surface->setDataLayerProps( m_stdLayerName, TSLPropertyDetect,  true ) ;
  surface->setDataLayerProps( m_stdLayerName, TSLPropertyVisible, true ) ;
  surface->setDataLayerProps( m_mapLayerName, TSLPropertyVisible, true ) ;
  surface->setDataLayerProps( m_terrainLayerName, TSLPropertyVisible, true ) ;
  surface->setDataLayerProps( m_cadrgLayerName, TSLPropertyVisible, true ) ; 
  surface->setDataLayerProps( m_wmsLayerName, TSLPropertyVisible, true ) ; 

  SetTopLayerAndTransparencies() ;

  return true ;
}

bool Simple3DDirectImportDoc::detachFromSurface( TSLDrawingSurfaceBase * surface )
{
  if ( !m_mapDataLayer )
  {
    return false ;
  }

  // Remove the layers from the specified drawing surface,
  surface->removeDataLayer(m_mapLayerName) ;
  surface->removeDataLayer(m_terrainLayerName) ;
  surface->removeDataLayer(m_stdLayerName);
  surface->removeDataLayer(m_wmsLayerName);
  surface->removeDataLayer(m_cadrgLayerName);

  return true ;
}

void Simple3DDirectImportDoc::DeleteContents() 
{
  // Clean up by destroying the map and pathlist
  if ( m_mapDataLayer )
  {
    m_mapDataLayer->destroy() ;
    m_mapDataLayer = NULL ;
  }

  if ( m_terrainDataLayer )
  {
    m_terrainDataLayer->destroy() ;
    m_terrainDataLayer = NULL ;
  }

  if (m_stdDataLayer)
  {
    m_stdDataLayer->destroy();
    m_stdDataLayer = NULL;
  }

  if (m_cadrgDataLayer)
  {
    m_cadrgDataLayer->destroy();
    m_cadrgDataLayer = NULL;
  }

  if (m_wmsDataLayer)
  {
    m_cadrgDataLayer->destroy();
    m_cadrgDataLayer = NULL;
  }

  if (m_callbacks)
  {
    delete m_callbacks ;
    m_callbacks = NULL ;
  }

  CDocument::DeleteContents();
}

void Simple3DDirectImportDoc::AllTilesLoadedCallback (void* arg2)
{
  // WMS callback.
  PostMessage((HWND)arg2, WM_NEW_TILES_AVAILABLE, 0, 0);
}

TSLLoaderCallbackReturn Simple3DDirectImportDoc::TileLoaderCallback( void* arg, const char* filename, const TSLEnvelope extent, TSLLoaderStatus status, Int percentDone )
{
  // WMS callback.
  if ( percentDone == 100 && status == TSLLoadingOK )
  {
    PostMessage((HWND)arg, WM_NEW_TILES_AVAILABLE, 0, 0);
  }
  return TSLContinue;
}

void Simple3DDirectImportDoc::OnFileOpenterrain()
{
  CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR,
    _T("Terrain Database Files (*.tdf)|*.tdf|All Files (*.*)|*.*||") );

  if ( dlg.DoModal() == IDOK )
  {
    CString file_path = (LPCTSTR)dlg.GetPathName();

    if (!m_terrainDataLayer)
    {
      m_terrainDataLayer = new TSL3DTerrainDataLayer;
    }
    if (!m_terrainDataLayer)
    {
      return ;
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

void Simple3DDirectImportDoc::OnCreatePolygon()
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

  Simple3DDirectImportView* pView = (Simple3DDirectImportView*)GetNextView( pos );
  pView->InvalidateRect( 0, false );
}

void Simple3DDirectImportDoc::OnFileLoadcadrgdata()
{
  CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, _T("CADRG Data Files (*.toc)|*.toc|All Files (*.*)|*.*||") );

  if ( dlg.DoModal() == IDOK )
  {
    CString file_path = (LPCTSTR)dlg.GetPathName();

    // Create a cadrg data layer   
    if( !m_cadrgDataLayer )
    {
      m_cadrgDataLayer = new TSLCADRGDataLayer(); 
    }   
    if ( !m_cadrgDataLayer )
    {
      return ;
    }

    // Setup the default rendering for CADRG.
    setupCadrgRendering();

    // Clear the error stack, load the map then check for errors.
    TSLErrorStack::clear() ;

    // Load the CADRG Data.
    m_cadrgDataLayer->loadData( TSLUTF8Encoder(file_path) ) ;

    // We should query the scales contained in the CADRG and set the points that
    // the scales are displayed at.
    //
    // The following code provides the initial start of a framework for doing
    // this....

    /*
    int numScaleFactors = m_cadrgLayer->getNumScaleFactors() ;
    for ( int i( 0 ) ; i < numScaleFactors  ; ++i )
    {
      TSLCadrgScaleFactorEnum factor = m_cadrgLayer->getScaleFactor( i ) ;
      double upper, lower ;

      m_cadrgLayer->getScaleFactorThreshold( factor, &upper, &lower ) ;

      TBD... modify the threshold to control the point the scale will appear at.

      m_cadrgLayer->setScaleFactorThreshold( factor, upper, lower ) ;
    }
    */

    // Set the maximum display extent so we don't attempt to draw too much CADRG
    // This is less of a risk with 3D due to the tileing of the World. However you
    // should avoid drawing too detailed a scale at the top level as this will take 
    // a long time to complete.
    double maxExtent = 0;
    m_cadrgDataLayer->getMaxDisplayExtent(&maxExtent);
    m_cadrgDataLayer->setMaxDisplayExtent(maxExtent * 2);

    // The max' number of tiles to retain in the cache.
    m_cadrgDataLayer->cacheSize(32);

    // Display any errors that have occurred
    const char * msg = TSLErrorStack::errorString( "Cannot load Cadrg Data\n" ) ;
    if ( msg )
    {
      // The map load failed, so display an error
      AfxMessageBox( (LPCTSTR)TSLUTF8Decoder(msg), MB_ICONERROR ) ;
    }
    else
    {
      SetTopLayerAndTransparencies() ;
    }
  }
}

void Simple3DDirectImportDoc::SetTopLayerAndTransparencies()
{
  if ( !m_surface )
  {
    return ;
  }

  if( m_onTop == "cadrg" )
  {
    m_surface->bringToFront(m_cadrgLayerName) ;
    if( m_cadrgDataLayer )
    {
      m_cadrgDataLayer->notifyChanged() ;
    }
  }
  else
  {
    m_surface->bringToFront(m_wmsLayerName) ;
    if( m_wmsDataLayer )
    {
      m_wmsDataLayer->notifyChanged() ;
    }
  }

  m_surface->setDataLayerProps(m_cadrgLayerName, TSLPropertyTransparency, 
    m_cadrgTransparent ? 128 : 255) ;
  m_surface->setDataLayerProps(m_wmsLayerName, TSLPropertyTransparency, 
    m_wmsTransparent ? 128 : 255) ;
}

void Simple3DDirectImportDoc::setupCadrgRendering(void)
{
  double temp;
  char buffer[TSL_MAX_PATH] ;
  
  ////////////////////////////////////////////////////////////////////////////////////////
  // DATA EXTENT

  TSLProfileHelper::setDefaultSection( "DATA_EXTENT" ) ;

  int tempInt;
  TSLProfileHelper::lookupProfile ( "edge_style", &tempInt, 1 ) ;
  m_cadrgDataLayer->setFeatureRendering( "dataextent", 0, TSLRenderingAttributeEdgeStyle, tempInt ) ;

  TSLProfileHelper::lookupProfile ( "edge_colour", buffer, TSL_MAX_PATH ) ;
  m_cadrgDataLayer->setFeatureRendering( "dataextent", 0, TSLRenderingAttributeEdgeColour, getIDOfNearestColour( buffer ) ) ;

  TSLProfileHelper::lookupProfile ( "edge_thickness", &temp, 1.0 ) ;
  m_cadrgDataLayer->setFeatureRendering( "dataextent", 0, TSLRenderingAttributeEdgeThickness, temp ) ;

  m_cadrgDataLayer->setFeatureRendering( "dataextent", 0, TSLRenderingAttributeEdgeThicknessUnits, TSLDimensionUnitsPixels ) ;

  ////////////////////////////////////////////////////////////////////////////////////////
  // SCALE LABEL

  TSLProfileHelper::setDefaultSection( "SCALE_LABEL" ) ;

  TSLProfileHelper::lookupProfile ( "render_level", &tempInt, 1 ) ; 
  m_cadrgDataLayer->setFeatureRendering( "scalelabel", 0,  TSLRenderingAttributeRenderLevel, tempInt ) ;

  TSLProfileHelper::lookupProfile ( "font", &tempInt, 1 ) ; 
  m_cadrgDataLayer->setFeatureRendering( "scalelabel", 0, TSLRenderingAttributeTextFont, tempInt ) ;

  TSLProfileHelper::lookupProfile ( "colour", buffer, TSL_MAX_PATH ) ; 
  m_cadrgDataLayer->setFeatureRendering( "scalelabel", 0, TSLRenderingAttributeTextColour, getIDOfNearestColour( buffer ) ) ;

  TSLProfileHelper::lookupProfile ( "size_factor", &temp, 0.5 ) ; 
  m_cadrgDataLayer->setFeatureRendering( "scalelabel", 0, TSLRenderingAttributeTextSizeFactor, temp ) ;

  TSLProfileHelper::lookupProfile ( "size_factor_units", &tempInt, TSLDimensionUnitsUndefined ) ; 
  m_cadrgDataLayer->setFeatureRendering( "scalelabel", 0, TSLRenderingAttributeTextSizeFactorUnits, tempInt ) ; 

  TSLProfileHelper::lookupProfile ( "horizontal_alignment", &tempInt, TSLHorizontalAlignmentUndefined ) ; 
  m_cadrgDataLayer->setFeatureRendering( "scalelabel", 0, TSLRenderingAttributeTextHorizontalAlignment, tempInt ) ; 

  TSLProfileHelper::lookupProfile ( "vertical_alignment", &tempInt, TSLVerticalAlignmentUndefined ) ; 
  m_cadrgDataLayer->setFeatureRendering( "scalelabel", 0, TSLRenderingAttributeTextVerticalAlignment, tempInt ) ; 

  TSLProfileHelper::lookupProfile ( "background_mode", &tempInt, TSLTextBackgroundModeUndefined ) ; 
  m_cadrgDataLayer->setFeatureRendering( "scalelabel", 0, TSLRenderingAttributeTextBackgroundMode, tempInt ) ;

  TSLProfileHelper::lookupProfile ( "background_colour", buffer, TSL_MAX_PATH ) ; 
  m_cadrgDataLayer->setFeatureRendering( "scalelabel", 0, TSLRenderingAttributeTextBackgroundColour,
    getIDOfNearestColour( buffer ) ) ;

  TSLProfileHelper::lookupProfile ( "background_style", &tempInt, 1 ) ; 
  m_cadrgDataLayer->setFeatureRendering( "scalelabel", 0, TSLRenderingAttributeTextBackgroundStyle, tempInt); 

  TSLProfileHelper::lookupProfile ( "background_edge_colour", buffer, TSL_MAX_PATH ) ; 
  m_cadrgDataLayer->setFeatureRendering( "scalelabel", 0, TSLRenderingAttributeTextBackgroundEdgeColour,
    getIDOfNearestColour( buffer ) );

  TSLProfileHelper::lookupProfile ( "x_offset", &temp, 0.0 ) ; 
  m_cadrgDataLayer->setFeatureRendering( "scalelabel", 0, TSLRenderingAttributeTextOffsetX, temp );

  TSLProfileHelper::lookupProfile ( "y_offset", &temp, 0.0 ) ; 
  m_cadrgDataLayer->setFeatureRendering( "scalelabel", 0, TSLRenderingAttributeTextOffsetY, temp );

  TSLProfileHelper::lookupProfile ( "offset_units", &tempInt, TSLDimensionUnitsUndefined ) ; 
  m_cadrgDataLayer->setFeatureRendering( "scalelabel", 0, TSLRenderingAttributeTextOffsetUnits, tempInt );

  TSLProfileHelper::lookupProfile ( "rotatable", &tempInt, false ) ; 
  m_cadrgDataLayer->setFeatureRendering( "scalelabel", 0, TSLRenderingAttributeTextRotatable, tempInt);

  TSLProfileHelper::lookupProfile ( "min_pixel_height", &tempInt, 1 ) ; 
  m_cadrgDataLayer->setFeatureRendering( "scalelabel", 0, TSLRenderingAttributeTextMinPixelHeight, tempInt);

  TSLProfileHelper::lookupProfile ( "max_pixel_height", &tempInt, 2000 ) ; 
  m_cadrgDataLayer->setFeatureRendering( "scalelabel", 0, TSLRenderingAttributeTextMaxPixelHeight, tempInt);

  ////////////////////////////////////////////////////////////////////////////////////////
  // FRAME EXTENT

  TSLProfileHelper::setDefaultSection( "FRAME_EXTENT" ) ;

  TSLProfileHelper::lookupProfile ( "edge_style", &tempInt, 1 ) ;
  m_cadrgDataLayer->setFeatureRendering( "frameextent", 0, TSLRenderingAttributeEdgeStyle, tempInt ) ;

  TSLProfileHelper::lookupProfile ( "edge_colour", buffer, TSL_MAX_PATH ) ;
  m_cadrgDataLayer->setFeatureRendering( "frameextent", 0, TSLRenderingAttributeEdgeColour, getIDOfNearestColour( buffer ) ) ;

  TSLProfileHelper::lookupProfile ( "edge_thickness", &temp, 1.0 ) ;
  m_cadrgDataLayer->setFeatureRendering( "frameextent", 0, TSLRenderingAttributeEdgeThickness, temp ) ;

  m_cadrgDataLayer->setFeatureRendering( "frameextent", 0, TSLRenderingAttributeEdgeThicknessUnits, TSLDimensionUnitsPixels ) ;

  ////////////////////////////////////////////////////////////////////////////////////////
  // SCALE LABEL

  TSLProfileHelper::setDefaultSection( "FRAME_LABEL" ) ;

  TSLProfileHelper::lookupProfile ( "render_level", &tempInt, 1 ) ; 
  m_cadrgDataLayer->setFeatureRendering( "framelabel", 0,  TSLRenderingAttributeRenderLevel, tempInt ) ;
}

long Simple3DDirectImportDoc::getIDOfNearestColour( char * buffer )
{
  // This function takes a comma separated string and splits it
  // into its individual componenents. These are then converted to
  // long values and passed to the TSLDrawingSurface::getIDOfNearestColour
  // routine which returns a long value. This value is then
  // returned from the function.

  char seps[] = "," ;	// Setup the delimiter values.
  char * token ;		// Setup a pointer to the values.
  int rgb[3];			// Setup somewhere to store the values.
  int count = 0 ;	

  // Get the first value. Continue while there
  // is a string.
  token = strtok( buffer, seps ) ;
  while ( token != NULL )
  {
    rgb[count] = atoi( token ) ;
    count++;
    token = strtok ( NULL, seps ) ;
  }

  // Make sure three values have been returned.
  if ( count != 3 )
    return 0 ;

  // Return the colour.
  return TSLDrawingSurface::getIDOfNearestColour ( rgb[0], rgb[1], rgb[2] ) ;
}

void Simple3DDirectImportDoc::surface(TSL3DWinGLSurface * drawingSurface)
{
  m_surface = drawingSurface;
}

void Simple3DDirectImportDoc::OnViewCadrgontop()
{
  m_onTop = "cadrg" ;

  if ( m_surface )
  {
    m_surface->bringToFront(m_cadrgLayerName) ;
  }

  if ( m_cadrgDataLayer )
  {
    m_cadrgDataLayer->notifyChanged() ;
  }

  if ( m_wmsDataLayer )
  {
    m_wmsDataLayer->notifyChanged() ;
  }

  UpdateAllViews ( NULL );
}

void Simple3DDirectImportDoc::OnUpdateViewCadrgontop(CCmdUI *pCmdUI)
{
  if ( pCmdUI )
  {
    pCmdUI->SetCheck( m_onTop == "cadrg" ) ;
  }
}

void Simple3DDirectImportDoc::OnViewWmsontop()
{
  m_onTop = "wms" ;

  if ( m_surface )
  {
    m_surface->bringToFront(m_wmsLayerName) ;
  }

  if ( m_wmsDataLayer )
  {
    m_wmsDataLayer->notifyChanged() ;
  }

  if ( m_cadrgDataLayer )
  {
    m_cadrgDataLayer->notifyChanged() ;
  }

  UpdateAllViews( NULL ) ;
}

void Simple3DDirectImportDoc::OnUpdateViewWmsontop(CCmdUI *pCmdUI)
{
  if ( pCmdUI )
  {
    pCmdUI->SetCheck( m_onTop == "wms" ) ;
  }
}

void Simple3DDirectImportDoc::OnViewCadrgtransparent()
{
  m_cadrgTransparent = !m_cadrgTransparent ;
  int transparency = m_cadrgTransparent ? 128 : 255 ;

  if( m_surface )
  {
    m_surface->setDataLayerProps(m_cadrgLayerName, TSLPropertyTransparency, transparency);
  }

  UpdateAllViews ( NULL );
}

void Simple3DDirectImportDoc::OnUpdateViewCadrgtransparent(CCmdUI *pCmdUI)
{
  if( pCmdUI )
  {
    pCmdUI->SetCheck( m_cadrgTransparent ) ;
  }
}

void Simple3DDirectImportDoc::OnViewWmstransparent()
{
  m_wmsTransparent = !m_wmsTransparent ;
  int transparency = m_wmsTransparent ? 128 : 255 ;

  if( m_surface )
  {
    m_surface->setDataLayerProps(m_wmsLayerName, TSLPropertyTransparency, transparency);
  }

  UpdateAllViews ( NULL );
}

void Simple3DDirectImportDoc::OnUpdateViewWmstransparent(CCmdUI *pCmdUI)
{
  if( pCmdUI )
  {
    pCmdUI->SetCheck( m_wmsTransparent ) ;
  }
}

void Simple3DDirectImportDoc::newTileAvailable()
{
  UpdateAllViews( NULL );
}