// Hello GlobeDoc.cpp : implementation of the CHelloGlobeDoc class
//

#include "stdafx.h"
#include "Hello Globe.h"

#include "Hello GlobeDoc.h"
#include ".\hello globedoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHelloGlobeDoc

IMPLEMENT_DYNCREATE(CHelloGlobeDoc, CDocument)

BEGIN_MESSAGE_MAP(CHelloGlobeDoc, CDocument)
  //{{AFX_MSG_MAP(CHelloGlobeDoc)
  //}}AFX_MSG_MAP
  ON_COMMAND(ID_OVERLAYS_POLYGON, OnOverlaysPolygon)
  ON_COMMAND(ID_OVERLAYS_LINE, OnOverlaysPolyline)
  ON_COMMAND(ID_OVERLAYS_SYMBOL, OnOverlaysSymbol)
  ON_COMMAND(ID_OVERLAYS_TEXT, OnOverlaysText)
  ON_COMMAND(ID_OVERLAYS_FEATURE, OnOverlaysFeature)
  ON_UPDATE_COMMAND_UI(ID_OVERLAYS_POLYGON, OnUpdateOverlaysPolygon)
  ON_UPDATE_COMMAND_UI(ID_OVERLAYS_SYMBOL, OnUpdateOverlaysSymbol)
  ON_UPDATE_COMMAND_UI(ID_OVERLAYS_TEXT, OnUpdateOverlaysText)
  ON_UPDATE_COMMAND_UI(ID_OVERLAYS_LINE, OnUpdateOverlaysLine)
  ON_UPDATE_COMMAND_UI(ID_OVERLAYS_FEATURE, OnUpdateOverlaysFeature)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHelloGlobeDoc static members

int CHelloGlobeDoc::m_overlayType = ID_OVERLAYS_TEXT ;

/////////////////////////////////////////////////////////////////////////////
// CHelloGlobeDoc construction/destruction

CHelloGlobeDoc::CHelloGlobeDoc()
{
  m_mapDataLayer = 0 ;
  m_stdDataLayer = 0 ;
}

CHelloGlobeDoc::~CHelloGlobeDoc()
{
}

BOOL CHelloGlobeDoc::OnNewDocument()
{
  if (!CDocument::OnNewDocument())
    return FALSE;

  return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CHelloGlobeDoc serialization

void CHelloGlobeDoc::Serialize(CArchive& ar)
{
  if (ar.IsStoring())
  {
  }
  else
  {
  }
}

/////////////////////////////////////////////////////////////////////////////
// CHelloGlobeDoc diagnostics

#ifdef _DEBUG
void CHelloGlobeDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void CHelloGlobeDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHelloGlobeDoc commands

BOOL CHelloGlobeDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
  if (!CDocument::OnOpenDocument(lpszPathName))
    return FALSE;

  m_mapDataLayer = new TSLMapDataLayer() ;

  if ( !m_mapDataLayer->loadData( TSLUTF8Encoder( lpszPathName ) ) )
  {
    const char * msg = TSLErrorStack::errorString( "Cannot load map : \n" ) ;
    if ( msg ) 
      AfxMessageBox( (LPCTSTR)TSLUTF8Decoder(msg), MB_OK ) ;

    m_mapDataLayer->destroy() ;
    m_mapDataLayer = 0 ;

    return FALSE ;
  }

  m_stdDataLayer = new TSLStandardDataLayer() ;

  long black = TSLDrawingSurface::getIDOfNearestColour( 0, 0, 0 ) ;

  // Make up a feature name and numeric ID
  m_stdDataLayer->addFeatureRendering( "Airport", 123 ) ;

  // Associate some rendering with the new feature, use ID for efficiency
  m_stdDataLayer->setFeatureRendering( 0, 123, TSLRenderingAttributeSymbolStyle, 6003 ) ;
  m_stdDataLayer->setFeatureRendering( 0, 123, TSLRenderingAttributeSymbolColour, black ) ;
  m_stdDataLayer->setFeatureRendering( 0, 123, TSLRenderingAttributeSymbolSizeFactor, 40.0 ) ;
  m_stdDataLayer->setFeatureRendering( 0, 123, TSLRenderingAttributeSymbolSizeFactorUnits, TSLDimensionUnitsPixels ) ;
  
  return TRUE;
}

void CHelloGlobeDoc::DeleteContents() 
{
  if ( m_mapDataLayer )
  {
    m_mapDataLayer->destroy() ;
    m_mapDataLayer = 0 ;
  }

  if ( m_stdDataLayer )
  {
    m_stdDataLayer->destroy() ;
    m_stdDataLayer = 0 ;
  }

  CDocument::DeleteContents();
}

bool CHelloGlobeDoc::addToSurface( TSLDrawingSurface * drawingSurface )
{
  if ( !m_mapDataLayer || !m_stdDataLayer || !drawingSurface )
    return false ;

  bool sts = drawingSurface->addDataLayer( m_mapDataLayer, "map" ) ;
  
  if ( sts )
    sts = drawingSurface->addDataLayer( m_stdDataLayer, "overlay" ) ;

  return sts ;
}

void CHelloGlobeDoc::OnOverlaysPolygon()
{
  m_overlayType = ID_OVERLAYS_POLYGON ;
}

void CHelloGlobeDoc::OnOverlaysPolyline()
{
  m_overlayType = ID_OVERLAYS_LINE ;
}

void CHelloGlobeDoc::OnOverlaysSymbol()
{
  m_overlayType = ID_OVERLAYS_SYMBOL ;
}

void CHelloGlobeDoc::OnOverlaysText()
{
  m_overlayType = ID_OVERLAYS_TEXT ;
}

void CHelloGlobeDoc::OnOverlaysFeature()
{
  m_overlayType = ID_OVERLAYS_FEATURE ;
}

void CHelloGlobeDoc::OnUpdateOverlaysPolygon(CCmdUI *pCmdUI)
{
  pCmdUI->SetCheck( pCmdUI->m_nID == m_overlayType ) ;
}

void CHelloGlobeDoc::OnUpdateOverlaysSymbol(CCmdUI *pCmdUI)
{
  pCmdUI->SetCheck( pCmdUI->m_nID == m_overlayType ) ;
}

void CHelloGlobeDoc::OnUpdateOverlaysText(CCmdUI *pCmdUI)
{
  pCmdUI->SetCheck( pCmdUI->m_nID == m_overlayType ) ;
}

void CHelloGlobeDoc::OnUpdateOverlaysLine(CCmdUI *pCmdUI)
{
  pCmdUI->SetCheck( pCmdUI->m_nID == m_overlayType ) ;
}

void CHelloGlobeDoc::OnUpdateOverlaysFeature(CCmdUI *pCmdUI)
{
  pCmdUI->SetCheck( pCmdUI->m_nID == m_overlayType ) ;
}

bool CHelloGlobeDoc::createOverlay( long x, long y, TSLDrawingSurface * ds )
{
  switch ( m_overlayType )
  {
    case ID_OVERLAYS_LINE :    return createPolyline( x, y, ds ) ;
    case ID_OVERLAYS_POLYGON : return createPolygon( x, y, ds ) ;
    case ID_OVERLAYS_TEXT :    return createText( x, y, ds ) ;
    case ID_OVERLAYS_SYMBOL :  return createSymbol( x, y, ds ) ;
    case ID_OVERLAYS_FEATURE : return createFeature( x, y, ds ) ;
  }
  return false ;
}


bool CHelloGlobeDoc::createText( long x, long y, TSLDrawingSurface * ds )
{
  TSLEntitySet * es = m_stdDataLayer->entitySet() ;
  TSLText * txt = es->createText( 0, x, y, "Hello World" ) ;
  if ( !txt )
    return false ;

  long black = TSLDrawingSurface::getIDOfNearestColour( 0, 0, 0 ) ;

  // Set the rendering of the text to be black, Arial, 25 pixels high
  txt->setRendering( TSLRenderingAttributeTextFont, 1 ) ;
  txt->setRendering( TSLRenderingAttributeTextColour, black ) ;
  txt->setRendering( TSLRenderingAttributeTextSizeFactor, 25.0 ) ;
  txt->setRendering( TSLRenderingAttributeTextSizeFactorUnits, 
		     TSLDimensionUnitsPixels ) ;

  // Tell the layer that its contents have changed
  m_stdDataLayer->notifyChanged( true ) ;
  
  return true ;
}

bool CHelloGlobeDoc::createPolygon( long x, long y, TSLDrawingSurface * ds )
{
  TSLEntitySet * es = m_stdDataLayer->entitySet() ;

  // Create a coordinate list forming a triangle around the position
  // Use the Drawing Surafce to calculate the coordinates
  // We will make our triangle 25 pixels either side of the position
  // Note that the pixels are at the current zoom factor - the polygon
  // is always completely scalable
  TSLCoordSet * coords = new TSLCoordSet() ;
  if ( !coords )
    return false ;

  double tmcPerDUx, tmcPerDUy;
  ds->TMCperDU(tmcPerDUx, tmcPerDUy) ;
  coords->add( x - 25 * tmcPerDUx, y - 25 * tmcPerDUx ) ;
  coords->add( x + 25 * tmcPerDUx, y - 25 * tmcPerDUx ) ;
  coords->add( x,                  y + 25 * tmcPerDUx ) ;

  // Hand ownership of the coordset to the polygon
  TSLPolygon * poly = es->createPolygon( 0, coords, true ) ;
  if ( !poly )
    return false ;

  long yellow = TSLDrawingSurface::getIDOfNearestColour( 255, 255, 0 ) ;
  long black  = TSLDrawingSurface::getIDOfNearestColour( 0, 0, 0 ) ;
  poly->setRendering( TSLRenderingAttributeFillStyle, 1 ) ;
  poly->setRendering( TSLRenderingAttributeFillColour, yellow ) ;
  poly->setRendering( TSLRenderingAttributeEdgeStyle, 1 ) ;
  poly->setRendering( TSLRenderingAttributeEdgeColour, black ) ;
  poly->setRendering( TSLRenderingAttributeEdgeThickness, 1.0 ) ;

  // Tell the layer that its contents have changed
  m_stdDataLayer->notifyChanged( true ) ;
  
  return true ; 
}

bool CHelloGlobeDoc::createPolyline( long x, long y, TSLDrawingSurface * ds )
{
  TSLEntitySet * es = m_stdDataLayer->entitySet() ;

  // Create a coordinate list forming a triangle around the position
  // Use the Drawing Surafce to calculate the coordinates
  TSLCoordSet * coords = new TSLCoordSet() ;
  if ( !coords )
    return false ;

  // Make a triangle, 1km either side of the specified position
  double tmcPerMU = ds->TMCperMU() ;
  coords->add( x - 1000 * tmcPerMU, y + 1000 * tmcPerMU ) ;
  coords->add( x,                   y - 1000 * tmcPerMU ) ;
  coords->add( x + 1000 * tmcPerMU, y + 1000 * tmcPerMU ) ;

  // Hand ownership of the coordset to the polygon
  TSLPolyline * poly = es->createPolyline( 0, coords, true ) ;
  if ( !poly )
    return false ;

  // Use MapUnit thickness so the line thickness scales as we zoom in/out
  // Set it to 20m. Complex line styles - like style 48 have thickness
  // clamping applied automatically to avoid performance
  // or aesthetic problems
  long yellow = TSLDrawingSurface::getIDOfNearestColour( 255, 255, 0 ) ;
  poly->setRendering( TSLRenderingAttributeEdgeStyle, 48 ) ;
  poly->setRendering( TSLRenderingAttributeEdgeColour, yellow ) ;
  poly->setRendering( TSLRenderingAttributeEdgeThickness, 20.0 ) ;
  poly->setRendering( TSLRenderingAttributeEdgeThicknessUnits,
		      TSLDimensionUnitsMapUnits) ;

  // Tell the layer that its contents have changed
  m_stdDataLayer->notifyChanged( true ) ;
  
  return true ; 
}

bool CHelloGlobeDoc::createSymbol( long x, long y, TSLDrawingSurface * ds )
{
  TSLEntitySet * es = m_stdDataLayer->entitySet() ;

  TSLSymbol * symbol = es->createSymbol( 0, x, y ) ;
  if ( !symbol )
    return false ;

  // Create a green star, 1000m high. This looks sensible on the Dorset map!
  long green = TSLDrawingSurface::getIDOfNearestColour( 0, 255, 0 ) ;
  symbol->setRendering( TSLRenderingAttributeSymbolStyle, 14 ) ;
  symbol->setRendering( TSLRenderingAttributeSymbolColour, green ) ;
  symbol->setRendering( TSLRenderingAttributeSymbolSizeFactor, 1000.0 ) ;
  symbol->setRendering( TSLRenderingAttributeSymbolSizeFactorUnits, 
		        TSLDimensionUnitsMapUnits ) ;

  // Tell the layer that its contents have changed
  m_stdDataLayer->notifyChanged( true ) ;
  
  return true ;
}

bool CHelloGlobeDoc::createFeature( long x, long y, TSLDrawingSurface * ds )
{
  TSLEntitySet * es = m_stdDataLayer->entitySet() ;

  // 123 is the numeric feature code we assigned on the Data Layer
  TSLSymbol * symbol = es->createSymbol( 123, x, y ) ;

  if ( !symbol )
    return false ;

  // No need to configure any rendering, MapLink will look it up
  // from the Data Layer at display time.

  // Tell the layer that its contents have changed
  m_stdDataLayer->notifyChanged( true ) ;
  
  return true ;
}


