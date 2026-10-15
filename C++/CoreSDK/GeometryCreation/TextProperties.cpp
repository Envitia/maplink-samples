// TextProperties.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "geometrycreationdoc.h"
#include "TextProperties.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextProperties dialog


CTextProperties::CTextProperties( CGeometryCreationDoc * doc, TSLText * text, CWnd* pParent /*=NULL*/)
: CDialog(CTextProperties::IDD, pParent), m_doc( doc ), m_text( text )
{
  //{{AFX_DATA_INIT(CTextProperties)
  m_backgroundColour = 0;
  m_backgroundEdgeColour = 0;
  m_backgroundStyle = 0;
  m_colour = 0;
  m_entityID = 0;
  m_featureID = 0;
  m_fixedSize = FALSE;
  m_font = 0;
  m_height = 0;
  m_maxSize = 0;
  m_minSize = 0;
  m_position = _T("");
  m_renderLevel = 0;
  m_rotatable = FALSE;
  m_rotation = 0;
  m_string = _T("");
  m_offset = _T("");
  m_offsetUnits = -1;
  m_sizeFactor = 0.0;
  m_sizeFactorUnits = -1;
  //}}AFX_DATA_INIT
  
  m_text->getRendering( TSLRenderingAttributeFeatureID,   &m_featureID ) ;
  m_text->getRendering( TSLRenderingAttributeEntityID,    &m_entityID ) ;
  m_text->getRendering( TSLRenderingAttributeRenderLevel, &m_renderLevel ) ; 
  
  TSLCoord pos = m_text->position() ;
  
  TCHAR buf[ 256 ] ;
  
  _stprintf( buf, _T("%d %d"), pos.x(), pos.y() ) ;
  m_position = buf ;
  
  double x, y ;
  m_text->getRendering( TSLRenderingAttributeTextOffsetX, &x ) ; 
  m_text->getRendering( TSLRenderingAttributeTextOffsetY, &y ) ; 
  _stprintf( buf, _T("%g %g"), x, y ) ;
  m_offset = buf ;
  m_text->getRendering( TSLRenderingAttributeTextOffsetUnits, &m_offsetUnits ) ; 
  m_offsetUnits += 1 ; // Add one since the enum values start at -1
  
  m_text->getRendering( TSLRenderingAttributeTextFont,   &m_font ) ;
  m_text->getRendering( TSLRenderingAttributeTextColour, &m_colour ) ;
  m_text->getRendering( TSLRenderingAttributeTextSizeFactor,  &m_sizeFactor ) ;
  m_text->getRendering( TSLRenderingAttributeTextSizeFactorUnits,  &m_sizeFactorUnits ) ;
  m_sizeFactorUnits += 1 ; // Add one since the enum values start at -1
  
  m_rotation = (m_text->rotation() * 180.0) / M_PI ;
  m_height   = m_text->height() ;
  
  CString filteredValue = (LPCTSTR)TSLUTF8Decoder(m_text->value()) ;
  
  m_string = "" ;
  for ( int c = 0 ; c < filteredValue.GetLength() ; ++c )
  {
    TCHAR ch = filteredValue[c] ;
    
    if ( ch == _T('\n') )
      m_string += _T('\r') ;
    m_string += ch ;
  }
  
  bool flag(false) ;
  m_text->getRendering( TSLRenderingAttributeTextFixedHeight, &flag ) ;
  m_fixedSize = flag ;
  flag = false;
  m_text->getRendering( TSLRenderingAttributeTextRotatable, &flag ) ;
  m_rotatable = flag ;
  
  m_text->getRendering( TSLRenderingAttributeTextMinPixelHeight, &m_minSize ) ;
  m_text->getRendering( TSLRenderingAttributeTextMaxPixelHeight, &m_maxSize ) ;
  
  m_text->getRendering( TSLRenderingAttributeTextBackgroundStyle,  &m_backgroundStyle ) ;
  m_text->getRendering( TSLRenderingAttributeTextBackgroundColour, &m_backgroundColour ) ;
  m_text->getRendering( TSLRenderingAttributeTextBackgroundEdgeColour,  &m_backgroundEdgeColour ) ;
  
  Create( IDD_TEXT_DIALOG ) ;
  
  int val ;
  m_text->getRendering( TSLRenderingAttributeTextBackgroundMode,  &val ) ;
  m_backgroundMode.SetCurSel( val + 1 ) ;
  
  m_text->getRendering( TSLRenderingAttributeTextHorizontalAlignment,  &val ) ;
  m_horizontalAlignment.SetCurSel( val ) ;
  
  m_text->getRendering( TSLRenderingAttributeTextVerticalAlignment,  &val ) ;
  m_verticalAlignment.SetCurSel( val ) ;
}


void CTextProperties::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CTextProperties)
  DDX_Control(pDX, IDC_VERTICAL_ALIGNMENT, m_verticalAlignment);
  DDX_Control(pDX, IDC_HORIZONTAL_ALIGNMENT, m_horizontalAlignment);
  DDX_Control(pDX, IDC_BACKGROUND_MODE, m_backgroundMode);
  DDX_Text(pDX, IDC_BACKGROUND_COLOUR, m_backgroundColour);
  DDX_Text(pDX, IDC_BACKGROUND_EDGE_COLOUR, m_backgroundEdgeColour);
  DDX_Text(pDX, IDC_BACKGROUND_STYLE, m_backgroundStyle);
  DDX_Text(pDX, IDC_COLOUR, m_colour);
  DDX_Text(pDX, IDC_ENTITY_ID, m_entityID);
  DDX_Text(pDX, IDC_FEATURE_ID, m_featureID);
  DDX_Check(pDX, IDC_FIXED_SIZE, m_fixedSize);
  DDX_Text(pDX, IDC_FONT, m_font);
  DDX_Text(pDX, IDC_HEIGHT, m_height);
  DDX_Text(pDX, IDC_MAX_SIZE, m_maxSize);
  DDX_Text(pDX, IDC_MIN_SIZE, m_minSize);
  DDX_Text(pDX, IDC_POSITION, m_position);
  DDX_Text(pDX, IDC_RENDER_LEVEL, m_renderLevel);
  DDV_MinMaxInt(pDX, m_renderLevel, -5, 5);
  DDX_Check(pDX, IDC_ROTATABLE, m_rotatable);
  DDX_Text(pDX, IDC_ROTATION, m_rotation);
  DDV_MinMaxInt(pDX, m_rotation, 0, 360);
  DDX_Text(pDX, IDC_STRING, m_string);
  DDX_Text(pDX, IDC_OFFSET, m_offset);
  DDX_CBIndex(pDX, IDC_OFFSET_UNITS, m_offsetUnits);
  DDX_Text(pDX, IDC_SIZE_FACTOR, m_sizeFactor);
  DDX_CBIndex(pDX, IDC_SIZE_FACTOR_UNITS, m_sizeFactorUnits);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTextProperties, CDialog)
//{{AFX_MSG_MAP(CTextProperties)
ON_BN_CLICKED(ID_DELETE_PRIMITIVE, OnDeletePrimitive)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextProperties message handlers

void CTextProperties::OnOK() 
{
  UpdateData( TRUE ) ;
  
  m_text->setRendering( TSLRenderingAttributeFeatureID,   m_featureID ) ;
  m_text->setRendering( TSLRenderingAttributeEntityID,    m_entityID ) ;
  m_text->setRendering( TSLRenderingAttributeRenderLevel, m_renderLevel ) ; 
  
  int x, y ;
  _stscanf( m_position, _T("%d %d"), &x, &y ) ;
  m_text->position( TSLCoord( x, y ) ) ;
  
  double dx, dy ;
  _stscanf( m_offset, _T("%lg %lg"), &dx, &dy ) ;
  m_text->setRendering( TSLRenderingAttributeTextOffsetX, dx ) ;
  m_text->setRendering( TSLRenderingAttributeTextOffsetY, dy ) ;
  m_text->setRendering( TSLRenderingAttributeTextOffsetUnits, m_offsetUnits - 1 ) ;
  
  m_text->setRendering( TSLRenderingAttributeTextFont,   m_font ) ;
  m_text->setRendering( TSLRenderingAttributeTextColour, m_colour ) ;
  m_text->setRendering( TSLRenderingAttributeTextSizeFactor,  m_sizeFactor ) ;
  m_text->setRendering( TSLRenderingAttributeTextSizeFactorUnits, m_sizeFactorUnits - 1 ) ;  
  m_text->rotation( (m_rotation * M_PI) / 180.0 ) ;
  m_text->height(m_height) ;
  
  CString filteredValue = "" ;
  
  // Filter 
  for ( int c = 0 ; c < m_string.GetLength() ; ++c )
  {
    TCHAR ch = m_string[c] ;
    
    if ( ch != _T('\r') )
      filteredValue += ch ;
  }
  m_text->value( TSLUTF8Encoder(filteredValue) ) ;
  
  m_text->setRendering( TSLRenderingAttributeTextFixedHeight, (bool)m_fixedSize ) ;
  m_text->setRendering( TSLRenderingAttributeTextRotatable, (bool)m_rotatable ) ;
  
  m_text->setRendering( TSLRenderingAttributeTextMinPixelHeight, m_minSize ) ;
  m_text->setRendering( TSLRenderingAttributeTextMaxPixelHeight, m_maxSize ) ;
  
  m_text->setRendering( TSLRenderingAttributeTextBackgroundStyle,  m_backgroundStyle ) ;
  m_text->setRendering( TSLRenderingAttributeTextBackgroundColour, m_backgroundColour ) ;
  m_text->setRendering( TSLRenderingAttributeTextBackgroundEdgeColour,  m_backgroundEdgeColour ) ;
  
  int val = m_backgroundMode.GetCurSel() - 1 ;
  m_text->setRendering( TSLRenderingAttributeTextBackgroundMode, val ) ;
  val = m_horizontalAlignment.GetCurSel( ) ;
  m_text->setRendering( TSLRenderingAttributeTextHorizontalAlignment,  val ) ;
  val = m_verticalAlignment.GetCurSel( ) ;
  m_text->setRendering( TSLRenderingAttributeTextVerticalAlignment,  val ) ;
  
  m_doc->updatedPrimitive( m_text ) ;
  
  DestroyWindow() ;
}

void CTextProperties::OnDeletePrimitive() 
{
  m_text->destroy() ;
  
  m_doc->updatedPrimitive( 0 ) ;
  
  DestroyWindow() ;
}

void CTextProperties::PostNcDestroy() 
{  
  CDialog::PostNcDestroy();
  delete this ;
}

void CTextProperties::OnCancel() 
{ 
  DestroyWindow() ;
}
