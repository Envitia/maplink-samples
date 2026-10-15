// PolygonProperties.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "geometrycreationdoc.h"
#include "PolygonProperties.h"
#include <sstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPolygonProperties dialog


CPolygonProperties::CPolygonProperties( CGeometryCreationDoc * doc, TSLPolygon * poly, CWnd* pParent /*=NULL*/)
: CDialog(CPolygonProperties::IDD, pParent), m_doc( doc ), m_poly( poly )
{
  //{{AFX_DATA_INIT(CPolygonProperties)
  m_coordinates = _T("");
  m_edgeColour = 0;
  m_edgeStyle = 0;
  m_entityID = 0;
  m_featureID = 0;
  m_fillColour = 0;
  m_fillStyle = 0;
  m_renderLevel = 0;
  m_thicknessUnits = -1;
  m_edgeThickness = 0.0;
  //}}AFX_DATA_INIT
  
  poly->getRendering( TSLRenderingAttributeFeatureID,   &m_featureID ) ;
  poly->getRendering( TSLRenderingAttributeEntityID,    &m_entityID ) ;
  poly->getRendering( TSLRenderingAttributeRenderLevel, &m_renderLevel ) ; 
  
  const TSLCoordSet & cs = poly->outer() ;
  
  m_coordinates = "" ;
  
  char buf[ 256 ] ;
  
  for ( int i = 0 ; i < cs.size() ; ++i )
  {
    sprintf( buf, "%d %d\r\n", cs[i].x(), cs[i].y() ) ;
    m_coordinates += buf ;
  }
  
  poly->getRendering( TSLRenderingAttributeEdgeStyle,   &m_edgeStyle ) ;
  poly->getRendering( TSLRenderingAttributeEdgeColour,  &m_edgeColour ) ;
  poly->getRendering( TSLRenderingAttributeEdgeThickness, &m_edgeThickness ) ;
  poly->getRendering( TSLRenderingAttributeEdgeThicknessUnits, &m_thicknessUnits ) ;
  m_thicknessUnits += 1 ; // Add one since enum starts at -1
  
  poly->getRendering( TSLRenderingAttributeFillStyle,   &m_fillStyle ) ;
  poly->getRendering( TSLRenderingAttributeFillColour,  &m_fillColour ) ;
  
  Create( IDD_POLYGON_DIALOG ) ;
}


void CPolygonProperties::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CPolygonProperties)
  DDX_Text(pDX, IDC_COORDINATES, m_coordinates);
  DDX_Text(pDX, IDC_EDGE_COLOUR, m_edgeColour);
  DDV_MinMaxInt(pDX, m_edgeColour, 0, 200000);
  DDX_Text(pDX, IDC_EDGE_STYLE, m_edgeStyle);
  DDV_MinMaxInt(pDX, m_edgeStyle, 0, 200);
  DDX_Text(pDX, IDC_ENTITY_ID, m_entityID);
  DDX_Text(pDX, IDC_FEATURE_ID, m_featureID);
  DDX_Text(pDX, IDC_FILL_COLOUR, m_fillColour);
  DDV_MinMaxInt(pDX, m_fillColour, 0, 200000);
  DDX_Text(pDX, IDC_FILL_STYLE, m_fillStyle);
	DDV_MinMaxInt(pDX, m_fillStyle, 0, 50000);
  DDX_Text(pDX, IDC_RENDER_LEVEL, m_renderLevel);
  DDV_MinMaxInt(pDX, m_renderLevel, -5, 5);
  DDX_CBIndex(pDX, IDC_EDGE_THICKNESS_UNITS, m_thicknessUnits);
  DDX_Text(pDX, IDC_EDGE_THICKNESS, m_edgeThickness);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPolygonProperties, CDialog)
//{{AFX_MSG_MAP(CPolygonProperties)
ON_BN_CLICKED(IDOK, OnOk)
ON_BN_CLICKED(ID_DELETE_PRIMITIVE, OnDeletePrimitive)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPolygonProperties message handlers

void CPolygonProperties::OnOk() 
{
  UpdateData( TRUE ) ;
  
  m_poly->setRendering( TSLRenderingAttributeFeatureID,   m_featureID ) ;
  m_poly->setRendering( TSLRenderingAttributeEntityID,    m_entityID ) ;
  m_poly->setRendering( TSLRenderingAttributeRenderLevel, m_renderLevel ) ; 
  
  TSLCoordSet * cs = new TSLCoordSet ;
  
  TSLUTF8Encoder coordsUTF8( m_coordinates );
  std::string coordsUTF8Str = coordsUTF8;
  std::istringstream is( coordsUTF8Str ) ;
  
  int x, y ;
  
  for ( ; ; )
  {
    is >> x >> y ;
    
    if ( is.eof() )
      break ;
    
    cs->add( x, y ) ;
  }
  
  m_poly->outer( cs, true ) ;
  
  m_poly->setRendering( TSLRenderingAttributeEdgeStyle,   m_edgeStyle ) ;
  m_poly->setRendering( TSLRenderingAttributeEdgeColour,  m_edgeColour ) ;
  m_poly->setRendering( TSLRenderingAttributeEdgeThickness, m_edgeThickness ) ;
  m_poly->setRendering( TSLRenderingAttributeEdgeThicknessUnits, m_thicknessUnits - 1 ) ;
  
  m_poly->setRendering( TSLRenderingAttributeFillStyle,   m_fillStyle ) ;
  m_poly->setRendering( TSLRenderingAttributeFillColour,  m_fillColour ) ;
  
  m_doc->updatedPrimitive( m_poly ) ;
  
  DestroyWindow() ;
}

void CPolygonProperties::OnDeletePrimitive() 
{
  m_poly->destroy() ;
  
  m_doc->updatedPrimitive( 0 ) ;
  
  DestroyWindow() ;
}

void CPolygonProperties::PostNcDestroy() 
{
  CDialog::PostNcDestroy();
  delete this ;
}

void CPolygonProperties::OnCancel() 
{
  DestroyWindow() ;
}
