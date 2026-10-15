// ArcProperties.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "geometrycreationdoc.h"
#include "ArcProperties.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CArcProperties dialog


CArcProperties::CArcProperties( CGeometryCreationDoc * doc, TSLArc * arc, CWnd* pParent /*=NULL*/)
: CDialog(CArcProperties::IDD, pParent), m_doc( doc ), m_arc( arc )
{
  //{{AFX_DATA_INIT(CArcProperties)
  m_centre = _T("");
  m_edgeColour = 0;
  m_edgeStyle = 0;
  m_edgeThickness = 0;
  m_endAngle = 0.0;
  m_entityID = 0;
  m_featureID = 0;
  m_renderLevel = 0;
  m_rotation = 0.0;
  m_startAngle = 0.0;
  m_xRadius = 0;
  m_yRadius = 0;
  m_edgeThicknessUnits = -1;
  //}}AFX_DATA_INIT
  m_arc->getRendering( TSLRenderingAttributeFeatureID,   &m_featureID ) ;
  m_arc->getRendering( TSLRenderingAttributeEntityID,    &m_entityID ) ;
  m_arc->getRendering( TSLRenderingAttributeRenderLevel, &m_renderLevel ) ; 
  
  TCHAR buf[ 256 ] ;
  _stprintf( buf, _T("%d %d"), m_arc->centre().x(), m_arc->centre().y() ) ;
  m_centre = buf ;
  
  m_rotation   = (m_arc->rotation() * 180.0) / M_PI ;
  m_startAngle = (m_arc->startAngle() * 180.0) / M_PI ;
  m_endAngle   = (m_arc->endAngle() * 180.0) / M_PI ;
  
  m_xRadius = m_arc->xRadius() ;
  m_yRadius = m_arc->yRadius() ;
  
  m_arc->getRendering( TSLRenderingAttributeEdgeStyle,   &m_edgeStyle ) ;
  m_arc->getRendering( TSLRenderingAttributeEdgeColour,  &m_edgeColour ) ;
  m_arc->getRendering( TSLRenderingAttributeEdgeThickness, &m_edgeThickness ) ;
  m_arc->getRendering( TSLRenderingAttributeEdgeThicknessUnits, &m_edgeThicknessUnits ) ;
  m_edgeThicknessUnits += 1 ; // Add one since enum begins at -1
  
  Create( IDD_ARC_DIALOG ) ;
}


void CArcProperties::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CArcProperties)
  DDX_Text(pDX, IDC_CENTRE, m_centre);
  DDX_Text(pDX, IDC_EDGE_COLOUR, m_edgeColour);
  DDX_Text(pDX, IDC_EDGE_STYLE, m_edgeStyle);
  DDX_Text(pDX, IDC_EDGE_THICKNESS, m_edgeThickness);
  DDX_Text(pDX, IDC_END_ANGLE, m_endAngle);
  DDX_Text(pDX, IDC_ENTITY_ID, m_entityID);
  DDX_Text(pDX, IDC_FEATURE_ID, m_featureID);
  DDX_Text(pDX, IDC_RENDER_LEVEL, m_renderLevel);
  DDV_MinMaxInt(pDX, m_renderLevel, -5, 5);
  DDX_Text(pDX, IDC_ROTATION, m_rotation);
  DDX_Text(pDX, IDC_START_ANGLE, m_startAngle);
  DDX_Text(pDX, IDC_X_RADIUS, m_xRadius);
  DDX_Text(pDX, IDC_Y_RADIUS, m_yRadius);
  DDX_CBIndex(pDX, IDC_EDGE_THICKNESS_UNITS, m_edgeThicknessUnits);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CArcProperties, CDialog)
//{{AFX_MSG_MAP(CArcProperties)
ON_BN_CLICKED(ID_DELETE_PRIMITIVE, OnDeletePrimitive)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CArcProperties message handlers

void CArcProperties::OnDeletePrimitive() 
{
  m_arc->destroy() ;
  
  m_doc->updatedPrimitive( 0 ) ;
  
  DestroyWindow() ;
}

void CArcProperties::OnOK() 
{
  UpdateData( TRUE ) ;
  
  m_arc->setRendering( TSLRenderingAttributeFeatureID,   m_featureID ) ;
  m_arc->setRendering( TSLRenderingAttributeEntityID,    m_entityID ) ;
  m_arc->setRendering( TSLRenderingAttributeRenderLevel, m_renderLevel ) ; 
  
  int x, y ;
  _stscanf( m_centre, _T("%d %d"), &x, &y );
  TSLCoord centre( x, y ) ;
  m_arc->centre( centre ) ;
  
  m_arc->rotation( (m_rotation * M_PI) / 180.0) ;
  m_arc->startAngle( (m_startAngle * M_PI) / 180.0)  ;
  m_arc->endAngle( (m_endAngle * M_PI) / 180.0 ) ;
  
  m_arc->xRadius( m_xRadius ) ;
  m_arc->yRadius( m_yRadius ) ;
  
  m_arc->setRendering( TSLRenderingAttributeEdgeStyle,   m_edgeStyle ) ;
  m_arc->setRendering( TSLRenderingAttributeEdgeColour,  m_edgeColour ) ;
  m_arc->setRendering( TSLRenderingAttributeEdgeThickness, m_edgeThickness ) ;
  m_arc->setRendering( TSLRenderingAttributeEdgeThicknessUnits, m_edgeThicknessUnits - 1 ) ;
  
  m_doc->updatedPrimitive( m_arc ) ;
  
  DestroyWindow() ;
}

void CArcProperties::OnCancel() 
{
  DestroyWindow() ;
}

void CArcProperties::PostNcDestroy() 
{
  CDialog::PostNcDestroy();
  delete this ;
}
