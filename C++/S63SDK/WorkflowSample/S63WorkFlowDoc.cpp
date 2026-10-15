// S63WorkFlowDoc.cpp : implementation of the S63WorkFlowDoc class
//
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "stdafx.h"
#include "s63\S63WorkFlow.h"
#include "S63WorkFlowDoc.h"
#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



bool NotForNavigationLayer::drawLayer (TSLRenderingInterface *renderingInterface, const TSLEnvelope* extent, TSLCustomDataLayerHandler& layerHandler)
{
  // we want to draw along the bottom the string 'Not for Navigation'

  // Setup the text drawing so that we draw the text on a grey background.
  // The magenta colour for the text was recommended by UKHO.
  renderingInterface->setupTextAttributes(3, TSLComposeRGB(255, 0, 255), 20.0, TSLDimensionUnitsPixels, 20, 20, 0.0, 0.0, TSLDimensionUnitsPixels,
   0.0, TSLHorizontalAlignmentRight, TSLVerticalAlignmentBaseline, TSLTextBackgroundModeRectangle, 1, TSLComposeRGB(200, 200, 200));

  // workout where to draw the text.
  TSLEnvelope envelope(renderingInterface->extent());
  TSLCoord point(envelope.bottomLeft());
  point.m_x = envelope.topRight().m_x;

  TSLTMC xr, yr;
  renderingInterface->TMCPerDU(xr, yr);
  point.m_y +=  (yr * 5);

  // draw the text
  renderingInterface->drawText(point, "Not for Navigation.");

  return true;
}

bool NotForNavigationLayer::hasCoordinateSystem () const
{
  return false;
}

void NotForNavigationLayer::releaseResources (int surfaceID)
{
  // do nothing for now.
}

/////////////////////////////////////////////////////////////////////////////
// S63WorkFlowDoc

const char * S63WorkFlowDoc::m_mapLayerName = MAP_LAYER_NAME;
const char * S63WorkFlowDoc::m_s63LayerName = S63_LAYER_NAME ;

IMPLEMENT_DYNCREATE(S63WorkFlowDoc, CDocument)

BEGIN_MESSAGE_MAP(S63WorkFlowDoc, CDocument)
//{{AFX_MSG_MAP(S63WorkFlowDoc)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// S63WorkFlowDoc construction/destruction



S63WorkFlowDoc::S63WorkFlowDoc()
{
  // Initialise my variables
  m_mapDataLayer = NULL ;
  m_s63DataLayer = NULL ;
  m_pGridLayer = NULL;
  m_pMGRSGridLayer = NULL;
  m_notForNavigationClientLayer = NULL;
  m_notForNavigationCustomLayer = NULL;

  TSLCoordinateSystem::loadCoordinateSystems();
}

S63WorkFlowDoc::~S63WorkFlowDoc()
{  
  if ( m_mapDataLayer )
  {
    m_mapDataLayer->destroy();
    m_mapDataLayer = NULL;
  }

  if (m_s63DataLayer)
  {
    m_s63DataLayer = NULL;
  }
  
  if (m_notForNavigationClientLayer)
  {
    delete m_notForNavigationClientLayer;
    m_notForNavigationClientLayer= NULL;
  }

  if (m_notForNavigationCustomLayer)
  {
    m_notForNavigationCustomLayer->destroy();
    m_notForNavigationCustomLayer = NULL;
  }

  if (m_pGridLayer)
  {
    m_pGridLayer->destroy();
    m_pGridLayer = NULL;
  }
}

/////////////////////////////////////////////////////////////////////////////
// S63WorkFlowDoc diagnostics

#ifdef _DEBUG
void S63WorkFlowDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void S63WorkFlowDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// S63WorkFlowDoc commands

BOOL S63WorkFlowDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
  if (!CDocument::OnOpenDocument(lpszPathName))
    return FALSE;
  
  // Create a map data layer
  m_mapDataLayer = new TSLMapDataLayer();
  m_mapDataLayer->cacheSize( 512 * 1024 );
  
  // Clear the error stack, load the map then check for errors.
  TSLThreadedErrorStack::clear() ;

  // Load the map
  // This will automatically load all associated files such as 
  // the .pth and .pal files.  These needed to be loaded
  // directly by the application in earlier versions of MapLink
  m_mapDataLayer->loadData( TSLUTF8Encoder(lpszPathName) ) ;

  // Display any errors that have occurred
  TSLSimpleString msg;
  TSLThreadedErrorStack::errorString( msg, "Cannot load map\n" ) ;
  if ( !msg.empty() )
  {
    // The map load failed, so delete the data layer and path list
    AfxMessageBox( (LPCTSTR)TSLUTF8Decoder(msg), MB_ICONERROR ) ;
    m_mapDataLayer->destroy() ;
    m_mapDataLayer = NULL ;
  }

  string logMessage("Loaded Map: ");
  logMessage += TSLUTF8Encoder(lpszPathName);

  addLogLine(logMessage);

  CMainFrame *mainWnd = (CMainFrame*)AfxGetMainWnd();
  return TRUE;
}


void S63WorkFlowDoc::createAndAddCustomLayers(TSLDrawingSurface * surface)
{
  // Create and add the Custom Layer for drawing the string 'Not For Navigation'.
  if (!m_notForNavigationCustomLayer)
  {
    m_notForNavigationClientLayer = new NotForNavigationLayer;
    m_notForNavigationCustomLayer = new TSLCustomDataLayer;
    m_notForNavigationCustomLayer->setClientCustomDataLayer(m_notForNavigationClientLayer, false);
    surface->removeDataLayer("NotForNavigation");
    surface->addDataLayer( m_notForNavigationCustomLayer, "NotForNavigation");
    surface->setDataLayerProps( "NotForNavigation", TSLPropertyVisible, true ) ;
    surface->setDataLayerProps( "NotForNavigation", TSLPropertyDetect, false ) ;
    surface->setDataLayerProps( "NotForNavigation", TSLPropertySelect, false ) ;
    surface->setDataLayerProps( "NotForNavigation", TSLPropertyBuffered, false ) ;  
  }
  // Has to be drawn on top of everything.
  surface->bringToFront("NotForNavigation");
  
  // Note: we could add things like a Compass, Scale Bar, and Mariner's Objects using custom data-layers.
}


bool S63WorkFlowDoc::addMapToSurface( TSLDrawingSurface * surface )
{
  if ( !m_mapDataLayer )
    return false;

  // Add the map to the specified drawing surface,
  surface->removeDataLayer(m_mapLayerName);
  surface->addDataLayer( m_mapDataLayer, m_mapLayerName ) ;
  surface->setDataLayerProps( m_mapLayerName, TSLPropertyVisible, true ) ;
  surface->setDataLayerProps( m_mapLayerName, TSLPropertyBuffered,true ) ;
  surface->sendToBack( m_mapLayerName ) ;

  if ( m_s63DataLayer )
  {
    surface->bringInFrontof( m_s63LayerName, m_mapLayerName ) ;
  }

  createAndAddCustomLayers(surface);

  return true ;
}

bool S63WorkFlowDoc::addLayerToSurface( const char * layerName, TSLDataLayer * dataLayer, TSLDrawingSurface * surface )
{
  if ( !layerName || !dataLayer )
    return false;

  surface->removeDataLayer( layerName ) ;
  surface->addDataLayer( dataLayer, layerName ) ;
  surface->setDataLayerProps( layerName, TSLPropertyVisible, true ) ;
  surface->setDataLayerProps( layerName, TSLPropertyBuffered,true ) ;

  createAndAddCustomLayers(surface);

  dataLayer->notifyChanged(true);

  return true ;
}

bool S63WorkFlowDoc::addCustomLayerToSurface( const char * layerName, TSLCustomDataLayer * customLayer, TSLDrawingSurface * surface )
{
  if ( !layerName || !customLayer )
    return false;

  surface->removeDataLayer( layerName ) ;
  surface->addDataLayer( customLayer, layerName ) ;
  surface->setDataLayerProps( layerName, TSLPropertyVisible, true ) ;
  surface->bringInFrontof( layerName, m_s63LayerName ) ;

  createAndAddCustomLayers(surface);

  customLayer->notifyChanged(true);

  return true ;
}

bool  S63WorkFlowDoc::showingGridLayer()
{
  return (m_pGridLayer!=NULL);
}

bool S63WorkFlowDoc::showingMGRSLayer()
{
  return (m_pMGRSGridLayer!=NULL);
}

bool S63WorkFlowDoc::removeGridLayerFromSurface( TSLDrawingSurface * surface )
{
  if (m_pGridLayer)
  {
    surface->removeDataLayer("gridlayer");
    m_pGridLayer->destroy();
    m_pGridLayer = NULL;
  }

  return true;
}

bool S63WorkFlowDoc::removeMGRSGridLayerFromSurface( TSLDrawingSurface * surface )
{
  if (m_pMGRSGridLayer)
  {
    surface->removeDataLayer("MGRSgridlayer");
    m_pMGRSGridLayer->destroy();
    m_pMGRSGridLayer = NULL;
  }

  return true;
}

bool S63WorkFlowDoc::addMGRSGridLayerToSurface( TSLDrawingSurface * surface )
{
  if ( !m_pMGRSGridLayer )
  {
    // Create the new data layer if not already created
    m_pMGRSGridLayer = new TSLMGRSGridDataLayer();
  }
  
  // Set the map layer that the grid is to overlay
  ((TSLMGRSGridDataLayer*)m_pMGRSGridLayer)->setDataLayer( m_mapDataLayer );
  
  // Ensure the grid layer is visible
  surface->setDataLayerProps( "MGRSgridlayer", TSLPropertyVisible, true ) ;
  surface->setDataLayerProps( "MGRSgridlayer", TSLPropertyDetect, false ) ;
  surface->setDataLayerProps( "MGRSgridlayer", TSLPropertySelect, false ) ;
  surface->setDataLayerProps( "MGRSgridlayer", TSLPropertyBuffered, false ) ;  

  // Setup the rendering of the Grid
  // If required, other attributes can be set, for example "latMajorDegree", "lonMinute", "lonTenths"

  m_pMGRSGridLayer->setFeatureRendering("utm100km", 0, TSLRenderingAttributeVisible, true);
  m_pMGRSGridLayer->setFeatureRendering("utm100km", 0, TSLRenderingAttributeEdgeColour, TSLDrawingSurface::getIDOfNearestColour(255, 255, 0));
  m_pMGRSGridLayer->setFeatureRendering("utm100km", 0, TSLRenderingAttributeEdgeStyle, 1);
  m_pMGRSGridLayer->setFeatureRendering("utm100km", 0, TSLRenderingAttributeEdgeThickness, 4);

  m_pMGRSGridLayer->setFeatureRendering("utm10km", 0, TSLRenderingAttributeVisible, true);
  m_pMGRSGridLayer->setFeatureRendering("utm10km", 0, TSLRenderingAttributeEdgeColour, TSLDrawingSurface::getIDOfNearestColour(0, 0, 255));
  m_pMGRSGridLayer->setFeatureRendering("utm10km", 0, TSLRenderingAttributeEdgeStyle, 6);
  m_pMGRSGridLayer->setFeatureRendering("utm10km", 0, TSLRenderingAttributeEdgeThickness, 3);
  
  m_pMGRSGridLayer->setFeatureRendering("utm1km", 0, TSLRenderingAttributeVisible, true);
  m_pMGRSGridLayer->setFeatureRendering("utm1km", 0, TSLRenderingAttributeEdgeColour, TSLDrawingSurface::getIDOfNearestColour(255, 0, 0));
  m_pMGRSGridLayer->setFeatureRendering("utm1km", 0, TSLRenderingAttributeEdgeStyle, 6);
  m_pMGRSGridLayer->setFeatureRendering("utm1km", 0, TSLRenderingAttributeEdgeThickness, 1);
  
  m_pMGRSGridLayer->setFeatureRendering("utmLabel", 0, TSLRenderingAttributeVisible, true);
  m_pMGRSGridLayer->setFeatureRendering("utmLabel", 0, TSLRenderingAttributeTextColour, TSLDrawingSurface::getIDOfNearestColour(255, 0, 0));
  m_pMGRSGridLayer->setFeatureRendering("utmLabel", 0, TSLRenderingAttributeTextFont, 3);
  m_pMGRSGridLayer->setFeatureRendering("utmLabel", 0, TSLRenderingAttributeTextMinPixelHeight, 12);
  m_pMGRSGridLayer->setFeatureRendering("utmLabel", 0, TSLRenderingAttributeTextMaxPixelHeight, 12);
  m_pMGRSGridLayer->setFeatureRendering("utmLabel", 0, TSLRenderingAttributeTextOffsetUnits, TSLDimensionUnitsPixels);

  m_pMGRSGridLayer->setFeatureRendering("mgrsLabel", 0, TSLRenderingAttributeVisible, true);
  m_pMGRSGridLayer->setFeatureRendering("mgrsLabel", 0, TSLRenderingAttributeTextColour, 1);
  m_pMGRSGridLayer->setFeatureRendering("mgrsLabel", 0, TSLRenderingAttributeTextFont, 1);
  m_pMGRSGridLayer->setFeatureRendering("mgrsLabel", 0, TSLRenderingAttributeTextMinPixelHeight, 8);
  m_pMGRSGridLayer->setFeatureRendering("mgrsLabel", 0, TSLRenderingAttributeTextMaxPixelHeight, 8);
  m_pMGRSGridLayer->setFeatureRendering("mgrsLabel", 0, TSLRenderingAttributeTextOffsetUnits, TSLDimensionUnitsPixels);

  // Add the data layer to the drawing surface
  surface->addDataLayer( m_pMGRSGridLayer, "MGRSgridlayer");

  // Make sure the MGRSgridlayer is at the top
  surface->bringToFront("MGRSgridlayer");  
  
  return true;
}


bool S63WorkFlowDoc::addGridLayerToSurface( TSLDrawingSurface * surface )
{
  if ( !m_pGridLayer )
  {
    // Create the new data layer if not already created
    m_pGridLayer = new TSLLatLongGridDataLayer();
  }

  // Set the map layer that the grid is to overlay
  ((TSLLatLongGridDataLayer*)m_pGridLayer)->setDataLayer( m_mapDataLayer );

  // Ensure the grid layer is visible
  surface->setDataLayerProps( "gridlayer", TSLPropertyVisible, true ) ;
  surface->setDataLayerProps( "gridlayer", TSLPropertyDetect, false ) ;
  surface->setDataLayerProps( "gridlayer", TSLPropertySelect, false ) ;
  surface->setDataLayerProps( "gridlayer", TSLPropertyBuffered, false ) ;  

  // Setup the rendering of the Grid
  // If required, other attributes can be set, for example "latMajorDegree", "lonMinute", "lonTenths"

  // Set latitude rendering.
  m_pGridLayer->setFeatureRendering("lonMajorDegree", 0, TSLRenderingAttributeVisible, true);
  m_pGridLayer->setFeatureRendering("lonMajorDegree", 0, TSLRenderingAttributeEdgeStyle, 1);
  m_pGridLayer->setFeatureRendering("lonMajorDegree", 0, TSLRenderingAttributeEdgeColour, TSLComposeRGB(0, 0, 0)); // Black
  m_pGridLayer->setFeatureRendering("lonMajorDegree", 0, TSLRenderingAttributeEdgeThickness, 1);

  m_pGridLayer->setFeatureRendering("latMajorDegree", 0, TSLRenderingAttributeVisible, true);
  m_pGridLayer->setFeatureRendering("latMajorDegree", 0, TSLRenderingAttributeEdgeStyle, 1);
  m_pGridLayer->setFeatureRendering("latMajorDegree", 0, TSLRenderingAttributeEdgeColour, TSLComposeRGB(0, 0, 0));
  m_pGridLayer->setFeatureRendering("latMajorDegree", 0, TSLRenderingAttributeEdgeThickness, 1);

  m_pGridLayer->setFeatureRendering("lonDegree", 0, TSLRenderingAttributeVisible, true);
  m_pGridLayer->setFeatureRendering("lonDegree", 0, TSLRenderingAttributeEdgeStyle, 1);
  m_pGridLayer->setFeatureRendering("lonDegree", 0, TSLRenderingAttributeEdgeColour, TSLComposeRGB(255, 0, 0)); // Red
  m_pGridLayer->setFeatureRendering("lonDegree", 0, TSLRenderingAttributeEdgeThickness, 1);

/*
  m_pGridLayer->setFeatureRendering("lonMinute", 0, TSLRenderingAttributeVisible, true);
  m_pGridLayer->setFeatureRendering("lonMinute", 0, TSLRenderingAttributeEdgeStyle, 1);
  m_pGridLayer->setFeatureRendering("lonMinute", 0, TSLRenderingAttributeEdgeColour, TSLComposeRGB(0, 255, 0)); // Green
  m_pGridLayer->setFeatureRendering("lonMinute", 0, TSLRenderingAttributeEdgeThickness, 1);
*/

  m_pGridLayer->setFeatureRendering("lonTenths", 0, TSLRenderingAttributeVisible, true);
  m_pGridLayer->setFeatureRendering("lonTenths", 0, TSLRenderingAttributeEdgeStyle, 1);
  m_pGridLayer->setFeatureRendering("lonTenths", 0, TSLRenderingAttributeEdgeColour, TSLComposeRGB(0, 0, 255)); // Blue
  m_pGridLayer->setFeatureRendering("lonTenths", 0, TSLRenderingAttributeEdgeThickness, 1);

  m_pGridLayer->setFeatureRendering("latDegree", 0, TSLRenderingAttributeVisible, true);
  m_pGridLayer->setFeatureRendering("latDegree", 0, TSLRenderingAttributeEdgeStyle, 1);
  m_pGridLayer->setFeatureRendering("latDegree", 0, TSLRenderingAttributeEdgeColour, TSLComposeRGB(255, 0, 0));
  m_pGridLayer->setFeatureRendering("latDegree", 0, TSLRenderingAttributeEdgeThickness, 1);

/*
  m_pGridLayer->setFeatureRendering("latMinute", 0, TSLRenderingAttributeVisible, true);
  m_pGridLayer->setFeatureRendering("latMinute", 0, TSLRenderingAttributeEdgeStyle, 1);
  m_pGridLayer->setFeatureRendering("latMinute", 0, TSLRenderingAttributeEdgeColour, TSLComposeRGB(0, 255, 0));
  m_pGridLayer->setFeatureRendering("latMinute", 0, TSLRenderingAttributeEdgeThickness, 1);
*/
  m_pGridLayer->setFeatureRendering("latTenths", 0, TSLRenderingAttributeVisible, true);
  m_pGridLayer->setFeatureRendering("latTenths", 0, TSLRenderingAttributeEdgeStyle, 1);
  m_pGridLayer->setFeatureRendering("latTenths", 0, TSLRenderingAttributeEdgeColour, TSLComposeRGB(0, 0, 255));
  m_pGridLayer->setFeatureRendering("latTenths", 0, TSLRenderingAttributeEdgeThickness, 1);

  m_pGridLayer->setFeatureRendering("degreeLineLabel", 0, TSLRenderingAttributeVisible, true);
  m_pGridLayer->setFeatureRendering("degreeLineLabel", 0, TSLRenderingAttributeTextFont, 3);
  m_pGridLayer->setFeatureRendering("degreeLineLabel", 0, TSLRenderingAttributeTextColour, TSLComposeRGB(0, 0, 0));
  m_pGridLayer->setFeatureRendering("degreeLineLabel", 0, TSLRenderingAttributeTextSizeFactor, 15);
  m_pGridLayer->setFeatureRendering("degreeLineLabel", 0, TSLRenderingAttributeTextSizeFactorUnits, TSLDimensionUnitsPoints);

  m_pGridLayer->setFeatureRendering("subgridLineLabel", 0, TSLRenderingAttributeVisible, true);
  m_pGridLayer->setFeatureRendering("subgridLineLabel", 0, TSLRenderingAttributeTextFont, 3);
  m_pGridLayer->setFeatureRendering("subgridLineLabel", 0, TSLRenderingAttributeTextColour, TSLComposeRGB(0, 0, 0));
  m_pGridLayer->setFeatureRendering("subgridLineLabel", 0, TSLRenderingAttributeTextSizeFactor, 15);
  m_pGridLayer->setFeatureRendering("subgridLineLabel", 0, TSLRenderingAttributeTextSizeFactorUnits, TSLDimensionUnitsPoints);
  m_pGridLayer->setFeatureRendering("subgridLineLabel", 0, TSLRenderingAttributeTextHorizontalAlignment, TSLHorizontalAlignmentLeft);
  m_pGridLayer->setFeatureRendering("subgridLineLabel", 0, TSLRenderingAttributeTextVerticalAlignment, TSLVerticalAlignmentFullBottom);

  // Setup grid label positions
  m_pGridLayer->setLatitudeLabelPosition(TSLLatLongGridDataLayer::TSLGridLabelLHS);
  m_pGridLayer->setLongitudeLabelPosition(TSLLatLongGridDataLayer::TSLGridLabelBottom);
  m_pGridLayer->setSubGridLabelPosition(TSLLatLongGridDataLayer::TSLSubGridLabelLHSBottom);

  // change the default tick lengths
  //m_pGridLayer->tickLength (15, 10);

  // Add the data layer to the drawing surface
  surface->addDataLayer( m_pGridLayer, "gridlayer");

  // Make sure the gridlayer is at the top
  surface->bringToFront("gridlayer");  

  return true ;
}

bool S63WorkFlowDoc::addS63LayerToSurface( TSLS63DataLayer * s63DataLayer, TSLDrawingSurface * surface )
{
  if ( !s63DataLayer )
    return false;

  if ( m_s63DataLayer )
  {
    m_s63DataLayer->destroy();
  }

  m_s63DataLayer = s63DataLayer;

  // Add the map to the specified drawing surface,
  surface->removeDataLayer( m_s63LayerName ) ;
  surface->addDataLayer( m_s63DataLayer, m_s63LayerName ) ;
  surface->setDataLayerProps( m_s63LayerName, TSLPropertyVisible, true ) ;
  surface->setDataLayerProps( m_s63LayerName, TSLPropertyBuffered,true ) ;
  
  surface->bringInFrontof( m_s63LayerName, m_mapLayerName ) ;
  surface->bringInFrontof( m_s63LayerName, NO_DATA_LAYER_NAME ) ;

  createAndAddCustomLayers(surface);

  m_s63DataLayer->notifyChanged(true);

  return true ;
}

bool S63WorkFlowDoc::detachFromSurface( TSLDrawingSurface * surface ) const
{
  if ( !m_mapDataLayer || !m_s63DataLayer )
    return false;

  // Remove the map to the specified drawing surface,
  surface->removeDataLayer( m_mapLayerName ) ;
  surface->removeDataLayer( m_s63LayerName ) ;

  return true ; 
}

void S63WorkFlowDoc::DeleteContents() 
{
  // Clean up by destroying the map
  if ( m_mapDataLayer )
  {
    m_mapDataLayer->destroy() ;
    m_mapDataLayer = NULL ;
  }
  if (m_s63DataLayer)
  {
    m_s63DataLayer = NULL;
  }
  
  if (m_notForNavigationClientLayer)
  {
    delete m_notForNavigationClientLayer;
    m_notForNavigationClientLayer= NULL;
  }

  if (m_notForNavigationCustomLayer)
  {
    m_notForNavigationCustomLayer->destroy();
    m_notForNavigationCustomLayer = NULL;
  }

  if (m_pGridLayer)
  {
    m_pGridLayer->destroy();
    m_pGridLayer = NULL;
  }
  CDocument::DeleteContents();
}
