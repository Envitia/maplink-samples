// EllipseProperties.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "geometrycreationdoc.h"
#include "EllipseProperties.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEllipseProperties dialog


CEllipseProperties::CEllipseProperties( CGeometryCreationDoc * doc, TSLEllipse * ellipse, CWnd* pParent /*=NULL*/)
: CDialog(CEllipseProperties::IDD, pParent), m_ellipse( ellipse ), m_doc( doc )
{
  //{{AFX_DATA_INIT(CEllipseProperties)
  m_centre = _T("");
  m_edgeColour = 0;
  m_edgeStyle = 0;
  m_entityID = 0;
  m_featureID = 0;
  m_fillColour = 0;
  m_fillStyle = 0;
  m_renderLevel = 0;
  m_rotation = 0;
  m_xRadius = 0;
  m_yRadius = 0;
  m_thicknessUnits = -1;
  m_edgeThickness = 0.0;
  //}}AFX_DATA_INIT
  m_ellipse->getRendering( TSLRenderingAttributeFeatureID,   &m_featureID ) ;
  m_ellipse->getRendering( TSLRenderingAttributeEntityID,    &m_entityID ) ;
  m_ellipse->getRendering( TSLRenderingAttributeRenderLevel, &m_renderLevel ) ; 
  
  TCHAR buf[ 256 ] ;
  _stprintf( buf, _T("%d %d"), m_ellipse->centre().x(), m_ellipse->centre().y() ) ;
  m_centre = buf ;
  
  m_rotation   = (m_ellipse->rotation() * 180.0) / M_PI ;
  
  m_xRadius = m_ellipse->xRadius() ;
  m_yRadius = m_ellipse->yRadius() ;
  
  m_ellipse->getRendering( TSLRenderingAttributeEdgeStyle,     &m_edgeStyle ) ;
  m_ellipse->getRendering( TSLRenderingAttributeEdgeColour,    &m_edgeColour ) ;
  m_ellipse->getRendering( TSLRenderingAttributeEdgeThickness, &m_edgeThickness ) ;
  m_ellipse->getRendering( TSLRenderingAttributeEdgeThicknessUnits, &m_thicknessUnits ) ;
  m_thicknessUnits += 1 ;
  
  m_ellipse->getRendering( TSLRenderingAttributeFillStyle,   &m_fillStyle ) ;
  m_ellipse->getRendering( TSLRenderingAttributeFillColour,  &m_fillColour ) ;
  
  Create( IDD_ELLIPSE_DIALOG ) ;
}


void CEllipseProperties::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CEllipseProperties)
  DDX_Text(pDX, IDC_CENTRE, m_centre);
  DDX_Text(pDX, IDC_EDGE_COLOUR, m_edgeColour);
  DDX_Text(pDX, IDC_EDGE_STYLE, m_edgeStyle);
  DDX_Text(pDX, IDC_ENTITY_ID, m_entityID);
  DDX_Text(pDX, IDC_FEATURE_ID, m_featureID);
  DDX_Text(pDX, IDC_FILL_COLOUR, m_fillColour);
  DDX_Text(pDX, IDC_FILL_STYLE, m_fillStyle);
	DDV_MinMaxInt(pDX, m_fillStyle, 0, 50000);
  DDX_Text(pDX, IDC_RENDER_LEVEL, m_renderLevel);
  DDX_Text(pDX, IDC_ROTATION, m_rotation);
  DDX_Text(pDX, IDC_X_RADIUS, m_xRadius);
  DDX_Text(pDX, IDC_Y_RADIUS, m_yRadius);
  DDX_CBIndex(pDX, IDC_EDGE_THICKNESS_UNITS, m_thicknessUnits);
  DDX_Text(pDX, IDC_EDGE_THICKNESS, m_edgeThickness);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEllipseProperties, CDialog)
//{{AFX_MSG_MAP(CEllipseProperties)
ON_BN_CLICKED(ID_DELETE_PRIMITIVE, OnDeletePrimitive)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEllipseProperties message handlers

void CEllipseProperties::OnDeletePrimitive() 
{
  m_ellipse->destroy() ;
  
  m_doc->updatedPrimitive( 0 ) ;
  
  DestroyWindow() ;
}

void CEllipseProperties::OnOK() 
{
  UpdateData( TRUE ) ;
  
  m_ellipse->setRendering( TSLRenderingAttributeFeatureID,   m_featureID ) ;
  m_ellipse->setRendering( TSLRenderingAttributeEntityID,    m_entityID ) ;
  m_ellipse->setRendering( TSLRenderingAttributeRenderLevel, m_renderLevel ) ; 
  
  int x, y ;
  _stscanf( m_centre, _T("%d %d"), &x, &y ) ;
  TSLCoord centre( x, y ) ;
  m_ellipse->centre( centre ) ;
  
  m_ellipse->rotation( (m_rotation * M_PI) / 180.0) ;
  
  m_ellipse->xRadius(m_xRadius) ;
  m_ellipse->yRadius(m_yRadius) ;
  
  m_ellipse->setRendering( TSLRenderingAttributeEdgeStyle,     m_edgeStyle ) ;
  m_ellipse->setRendering( TSLRenderingAttributeEdgeColour,    m_edgeColour ) ;
  m_ellipse->setRendering( TSLRenderingAttributeEdgeThickness, m_edgeThickness ) ;
  m_ellipse->setRendering( TSLRenderingAttributeEdgeThicknessUnits, m_thicknessUnits - 1 ) ;
  m_ellipse->setRendering( TSLRenderingAttributeFillStyle,   m_fillStyle ) ;
  m_ellipse->setRendering( TSLRenderingAttributeFillColour,  m_fillColour ) ;
  
  m_doc->updatedPrimitive( m_ellipse ) ;
  
  DestroyWindow() ;
}

void CEllipseProperties::PostNcDestroy() 
{
  CDialog::PostNcDestroy();
  delete this ;
}

void CEllipseProperties::OnCancel() 
{
  DestroyWindow() ;
}
