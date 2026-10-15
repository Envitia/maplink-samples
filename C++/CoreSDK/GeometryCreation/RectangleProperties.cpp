// RectangleProperties.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "geometrycreationdoc.h"
#include "RectangleProperties.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRectangleProperties dialog


CRectangleProperties::CRectangleProperties( CGeometryCreationDoc * doc, TSLRectangle * rect, CWnd* pParent /*=NULL*/)
: CDialog(CRectangleProperties::IDD, pParent), m_doc( doc ), m_rect( rect ) 
{
  //{{AFX_DATA_INIT(CRectangleProperties)
  m_edgeColour = 0;
  m_edgeStyle = 0;
  m_entityID = 0;
  m_featureID = 0;
  m_fillColour = 0;
  m_fillStyle = 0;
  m_lowerLeft = _T("");
  m_renderLevel = 0;
  m_rotation = 0.0;
  m_topRight = _T("");
  m_thicknessUnits = -1;
  m_edgeThickness = 0.0;
  //}}AFX_DATA_INIT
  rect->getRendering( TSLRenderingAttributeFeatureID,  &m_featureID ) ;
  rect->getRendering( TSLRenderingAttributeEntityID,    &m_entityID ) ;
  rect->getRendering( TSLRenderingAttributeRenderLevel, &m_renderLevel ) ; 
  
  TSLCoord ll, tr ;
  rect->unrotatedCorners( ll, tr ) ;
  TCHAR buf[ 256 ] ;
  _stprintf( buf, _T("%d %d"), ll.x(), ll.y() ) ;
  m_lowerLeft = buf ;
  
  _stprintf( buf, _T("%d %d"), tr.x(), tr.y() ) ;
  m_topRight = buf ;
  
  m_rotation = (rect->rotation() * 180.0) / M_PI ;
  
  rect->getRendering( TSLRenderingAttributeEdgeStyle,   &m_edgeStyle ) ;
  rect->getRendering( TSLRenderingAttributeEdgeColour,  &m_edgeColour ) ;
  rect->getRendering( TSLRenderingAttributeEdgeThickness, &m_edgeThickness ) ;
  rect->getRendering( TSLRenderingAttributeEdgeThicknessUnits, &m_thicknessUnits ) ;
  m_thicknessUnits += 1 ; // Add one since enum starts at -1
  
  rect->getRendering( TSLRenderingAttributeFillStyle,   &m_fillStyle ) ;
  rect->getRendering( TSLRenderingAttributeFillColour,  &m_fillColour ) ;
  
  Create( IDD_RECTANGLE_DIALOG ) ;
}


void CRectangleProperties::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CRectangleProperties)
  DDX_Text(pDX, IDC_EDGE_COLOUR, m_edgeColour);
  DDV_MinMaxInt(pDX, m_edgeColour, 0, 200000);
  DDX_Text(pDX, IDC_EDGE_STYLE, m_edgeStyle);
  DDV_MinMaxInt(pDX, m_edgeStyle, 0, 100);
  DDX_Text(pDX, IDC_ENTITY_ID, m_entityID);
  DDX_Text(pDX, IDC_FEATURE_ID, m_featureID);
  DDX_Text(pDX, IDC_FILL_COLOUR, m_fillColour);
  DDV_MinMaxInt(pDX, m_fillColour, 0, 200000);
  DDX_Text(pDX, IDC_FILL_STYLE, m_fillStyle);
	DDV_MinMaxInt(pDX, m_fillStyle, 0, 50000);
  DDX_Text(pDX, IDC_LOWER_LEFT, m_lowerLeft);
  DDV_MaxChars(pDX, m_lowerLeft, 20);
  DDX_Text(pDX, IDC_RENDER_LEVEL, m_renderLevel);
  DDV_MinMaxInt(pDX, m_renderLevel, -5, 5);
  DDX_Text(pDX, IDC_ROTATION, m_rotation);
  DDV_MinMaxDouble(pDX, m_rotation, 0., 360.);
  DDX_Text(pDX, IDC_TOP_RIGHT, m_topRight);
  DDV_MaxChars(pDX, m_topRight, 20);
  DDX_CBIndex(pDX, IDC_EDGE_THICKNESS_UNITS, m_thicknessUnits);
  DDX_Text(pDX, IDC_EDGE_THICKNESS, m_edgeThickness);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRectangleProperties, CDialog)
//{{AFX_MSG_MAP(CRectangleProperties)
ON_BN_CLICKED(ID_DELETE_PRIMITIVE, OnDeletePrimitive)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRectangleProperties message handlers

void CRectangleProperties::OnOK() 
{
  UpdateData( TRUE ) ;
  
  m_rect->setRendering( TSLRenderingAttributeFeatureID,   m_featureID ) ;
  m_rect->setRendering( TSLRenderingAttributeEntityID,    m_entityID ) ;
  m_rect->setRendering( TSLRenderingAttributeRenderLevel, m_renderLevel ) ; 
  
  int x, y ;
  _stscanf( m_lowerLeft, _T("%d %d"), &x, &y ) ;
  TSLCoord ll( x, y ) ;
  
  _stscanf( m_topRight, _T("%d %d"), &x, &y ) ;
  TSLCoord tr( x, y ) ;
  
  m_rect->corners( ll, tr ) ;
  m_rect->rotation( (m_rotation * M_PI) / 180.0 ) ;
  
  m_rect->setRendering( TSLRenderingAttributeEdgeStyle,   m_edgeStyle ) ;
  m_rect->setRendering( TSLRenderingAttributeEdgeColour,  m_edgeColour ) ;
  m_rect->setRendering( TSLRenderingAttributeEdgeThickness, m_edgeThickness ) ;
  m_rect->setRendering( TSLRenderingAttributeEdgeThicknessUnits, m_thicknessUnits - 1) ;
  
  m_rect->setRendering( TSLRenderingAttributeFillStyle,   m_fillStyle ) ;
  m_rect->setRendering( TSLRenderingAttributeFillColour,  m_fillColour ) ;
  
  m_doc->updatedPrimitive( m_rect ) ;
  
  DestroyWindow() ;
}

void CRectangleProperties::OnDeletePrimitive() 
{
  m_rect->destroy() ;
  
  m_doc->updatedPrimitive( 0 ) ;
  
  DestroyWindow() ;
}

void CRectangleProperties::OnCancel() 
{
  DestroyWindow() ;
}

void CRectangleProperties::PostNcDestroy() 
{
  CDialog::PostNcDestroy();
  delete this ;
}
