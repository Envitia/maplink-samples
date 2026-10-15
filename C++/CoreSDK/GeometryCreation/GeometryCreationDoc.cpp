// GeometryCreationDoc.cpp : implementation of the CGeometryCreationDoc class
//

#include "stdafx.h"
#include "GeometryCreation.h"
#include "GeometryCreationView.h"

#include "GeometryCreationDoc.h"
#include "RectangleProperties.h"
#include "PolygonProperties.h"
#include "PolylineProperties.h"
#include "SymbolProperties.h"
#include "TextProperties.h"
#include "ArcProperties.h"
#include "EllipseProperties.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const char * CGeometryCreationDoc::m_layerName = "geometry" ;

/////////////////////////////////////////////////////////////////////////////
// CGeometryCreationDoc

IMPLEMENT_DYNCREATE(CGeometryCreationDoc, CDocument)

BEGIN_MESSAGE_MAP(CGeometryCreationDoc, CDocument)
//{{AFX_MSG_MAP(CGeometryCreationDoc)
	ON_COMMAND(ID_PRIMITIVES_RECTANGLE, OnPrimitivesRectangle)
	ON_COMMAND(ID_PRIMITIVES_POLYGON, OnPrimitivesPolygon)
	ON_COMMAND(ID_PRIMITIVES_POLYLINE, OnPrimitivesPolyline)
	ON_COMMAND(ID_PRIMITIVES_SYMBOL, OnPrimitivesSymbol)
	ON_COMMAND(ID_PRIMITIVES_TEXT, OnPrimitivesText)
	ON_COMMAND(ID_PRIMITIVES_ARC, OnPrimitivesArc)
	ON_COMMAND(ID_PRIMITIVES_ELLIPSE, OnPrimitivesEllipse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGeometryCreationDoc construction/destruction

CGeometryCreationDoc::CGeometryCreationDoc()
{
  // Initialise my variables
  m_dataLayer = 0 ;
}

CGeometryCreationDoc::~CGeometryCreationDoc()
{
  // Destroy any data layer and path list loaded
  if ( m_dataLayer )
  {
    m_dataLayer->destroy() ;
    m_dataLayer = 0 ;
  }
}

BOOL CGeometryCreationDoc::OnNewDocument()
{
  if (!CDocument::OnNewDocument())
    return FALSE;
  
  // TODO: add reinitialization code here

  if ( !m_dataLayer )
  {
    m_dataLayer = new TSLStandardDataLayer ;
    if ( !m_dataLayer )
      return false ;
  }
  else
    m_dataLayer->removeData( ) ;

  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CGeometryCreationDoc diagnostics

#ifdef _DEBUG
void CGeometryCreationDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void CGeometryCreationDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGeometryCreationDoc commands

bool CGeometryCreationDoc::addToSurface( TSLDrawingSurface * surface )
{
  if ( !m_dataLayer  )
    return false;

  // Add the geometry layer to the specified drawing surface,
  surface->addDataLayer( m_dataLayer, m_layerName ) ;
  surface->setDataLayerProps( m_layerName, TSLPropertyVisible, true ) ;
  surface->setDataLayerProps( m_layerName, TSLPropertyDetect,  true ) ;

  return true ;
}

bool CGeometryCreationDoc::detachFromSurface( TSLDrawingSurface * surface )
{
  if ( !m_dataLayer  )
    return false;

  // Remove the map to the specified drawing surface,
  surface->removeDataLayer( m_layerName ) ;

  return true ;
}

void CGeometryCreationDoc::updatedPrimitive( TSLEntity * entity )
{
  if ( m_dataLayer )
  {
    m_dataLayer->notifyChanged() ;

    POSITION pos = GetFirstViewPosition();
    while (pos != NULL)
    {
      CGeometryCreationView * view = static_cast< CGeometryCreationView * >( GetNextView(pos) ) ;
      if ( entity )
        view->updateEntityExtent( entity, m_layerName ) ;

    } 
    UpdateAllViews( 0 ) ;
  }
}


BOOL CGeometryCreationDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
  if (!CDocument::OnOpenDocument(lpszPathName))
    return FALSE;

  if ( !m_dataLayer )
    m_dataLayer = new TSLStandardDataLayer();
  else
    m_dataLayer->removeData() ;
  
  // Clear the error stack, load the map then check for errors.
  TSLErrorStack::clear() ;
 
  m_dataLayer->loadData( TSLUTF8Encoder(lpszPathName) ) ;

  // Display any errors that have occurred
  const char * msg = TSLErrorStack::errorString( "Cannot load tmf file\n" ) ;
  if ( msg )
  {
    // The map load failed, so delete the data layer and path list
    AfxMessageBox( (LPCTSTR)TSLUTF8Decoder(msg), MB_ICONERROR ) ;
    m_dataLayer->destroy() ;
    m_dataLayer = 0 ;
  }
  
  return TRUE;
}

void CGeometryCreationDoc::DeleteContents() 
{
  // Clean up by destroying the map and pathlist
  if ( m_dataLayer )
  {
    m_dataLayer->destroy() ;
    m_dataLayer = 0 ;
  }
	
  CDocument::DeleteContents();
}

BOOL CGeometryCreationDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
  return m_dataLayer ? m_dataLayer->saveData( TSLUTF8Encoder(lpszPathName) ) : FALSE ;
}

void CGeometryCreationDoc::OnPrimitivesRectangle() 
{
  // Create a rectangle with some default properties and invoke dialog
  TSLEntitySet * topLevelSet = m_dataLayer->entitySet() ;
  
  TSLCoord bottomLeft( -100000, -100000 ) ;
  TSLCoord topRight( 100000, 100000 ) ;

  TSLRectangle * rect = topLevelSet->createRectangle( 0, bottomLeft, topRight ) ;
  rect->setRendering( TSLRenderingAttributeFillColour, 181 ) ;
  rect->setRendering( TSLRenderingAttributeFillStyle,  1 ) ;
  rect->setRendering( TSLRenderingAttributeExteriorEdgeStyle, 1 ) ;
  rect->setRendering( TSLRenderingAttributeExteriorEdgeColour, 1 ) ;
  rect->setRendering( TSLRenderingAttributeExteriorEdgeThickness, 2 ) ;

  updatedPrimitive( rect ) ;

  new CRectangleProperties( this, rect ) ;
}

void CGeometryCreationDoc::OnPrimitivesPolygon() 
{
  TSLEntitySet * topLevelSet = m_dataLayer->entitySet() ;
  TSLCoordSet * cs = new TSLCoordSet ;

  cs->add( -100000, -100000 ) ;
  cs->add( 0, 100000 ) ;
  cs->add( 100000, 0 ) ;

  TSLPolygon * poly = topLevelSet->createPolygon( 0, cs, true ) ;

  poly->setRendering( TSLRenderingAttributeFillColour, 181 ) ;
  poly->setRendering( TSLRenderingAttributeFillStyle,  1 ) ;
  poly->setRendering( TSLRenderingAttributeExteriorEdgeStyle, 1 ) ;
  poly->setRendering( TSLRenderingAttributeExteriorEdgeColour, 1 ) ;
  poly->setRendering( TSLRenderingAttributeExteriorEdgeThickness, 2 ) ;

  updatedPrimitive( poly ) ;

  new CPolygonProperties( this, poly ) ;
}

void CGeometryCreationDoc::OnPrimitivesPolyline() 
{
  TSLEntitySet * topLevelSet = m_dataLayer->entitySet() ;  
  TSLCoordSet * cs = new TSLCoordSet ;

  cs->add( -100000, -100000 ) ;
  cs->add( 0, 100000 ) ;
  cs->add( 100000, 0 ) ;

  TSLPolyline * poly = topLevelSet->createPolyline( 0, cs, true ) ;

  poly->setRendering( TSLRenderingAttributeEdgeStyle, 1 ) ;
  poly->setRendering( TSLRenderingAttributeEdgeColour, 1 ) ;
  poly->setRendering( TSLRenderingAttributeEdgeThickness, 2 ) ;

  updatedPrimitive( poly ) ;

  new CPolylineProperties( this, poly ) ;
}

void CGeometryCreationDoc::OnPrimitivesSymbol() 
{
  // Create a rectangle with some default properties and invoke dialog
  TSLEntitySet * topLevelSet = m_dataLayer->entitySet() ;

  TSLSymbol * symbol = topLevelSet->createSymbol( 0, 0, 0, 10000 ) ;
  symbol->setRendering( TSLRenderingAttributeSymbolColour, 181 ) ;
  symbol->setRendering( TSLRenderingAttributeSymbolStyle,  1001 ) ;

  updatedPrimitive( symbol ) ;

  new CSymbolProperties( this, symbol ) ;
}

void CGeometryCreationDoc::OnPrimitivesText() 
{
  // Create a rectangle with some default properties and invoke dialog
  TSLEntitySet * topLevelSet = m_dataLayer->entitySet() ;

  TSLText * text = topLevelSet->createText( 0, 0, 0, "Hello World", 10000 ) ;
  text->setRendering( TSLRenderingAttributeTextColour, 1 ) ;
  text->setRendering( TSLRenderingAttributeTextFont, 1 ) ;

  updatedPrimitive( text ) ;

  new CTextProperties( this, text ) ;
}

void CGeometryCreationDoc::OnPrimitivesArc() 
{
  // Create a rectangle with some default properties and invoke dialog
  TSLEntitySet * topLevelSet = m_dataLayer->entitySet() ;

  int cx = 0, cy = 0 ;
  int radx = 10000, rady = 10000 ;
  double startAngle = 0 ;
  double endAngle = M_PI / 2 ;
  double rotation = 0 ;
  TSLArc * arc = topLevelSet->createArc( 0, startAngle, endAngle, cx, cy, radx, rady, rotation ) ;

  arc->setRendering( TSLRenderingAttributeEdgeColour, 181 ) ;
  arc->setRendering( TSLRenderingAttributeEdgeStyle,  1 ) ;
  arc->setRendering( TSLRenderingAttributeEdgeThickness, 2 ) ;

  updatedPrimitive( arc ) ;

  new CArcProperties( this, arc ) ;
}

void CGeometryCreationDoc::OnPrimitivesEllipse() 
{
  // Create a rectangle with some default properties and invoke dialog
  TSLEntitySet * topLevelSet = m_dataLayer->entitySet() ;

  int cx = 0, cy = 0 ;
  int radx = 10000, rady = 10000 ;
  double rotation = 0 ;
  TSLEllipse * ellipse = topLevelSet->createEllipse( 0, cx, cy, radx, rady, rotation ) ;

  ellipse->setRendering( TSLRenderingAttributeEdgeColour, 1 ) ;
  ellipse->setRendering( TSLRenderingAttributeEdgeStyle,  1 ) ;
  ellipse->setRendering( TSLRenderingAttributeEdgeThickness, 2 ) ;
  ellipse->setRendering( TSLRenderingAttributeFillColour, 106 ) ;
  ellipse->setRendering( TSLRenderingAttributeFillStyle,  1 ) ;

  updatedPrimitive( ellipse ) ;

  new CEllipseProperties( this, ellipse ) ;
}
