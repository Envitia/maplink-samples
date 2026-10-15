// SymbolProperties.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "geometrycreationdoc.h"
#include "SymbolProperties.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSymbolProperties dialog


CSymbolProperties::CSymbolProperties( CGeometryCreationDoc * doc, TSLSymbol * sym, CWnd* pParent /*=NULL*/)
: CDialog(CSymbolProperties::IDD, pParent), m_doc( doc ), m_symbol( sym )
{
  //{{AFX_DATA_INIT(CSymbolProperties)
  m_colour = 0;
  m_entityID = 0;
  m_featureID = 0;
  m_fixedSize = FALSE;
  m_height = 0;
  m_maxSize = 0;
  m_minSize = 0;
  m_position = _T("");
  m_renderLevel = 0;
  m_rotation = 0;
  m_style = 0;
  m_sizeFactor = 0.0;
  m_sizeFactorUnits = -1;
  m_rotatable = -1;
  //}}AFX_DATA_INIT
  
  m_symbol->getRendering( TSLRenderingAttributeFeatureID,   &m_featureID ) ;
  m_symbol->getRendering( TSLRenderingAttributeEntityID,    &m_entityID ) ;
  m_symbol->getRendering( TSLRenderingAttributeRenderLevel, &m_renderLevel ) ; 
  
  TSLCoord pos = m_symbol->position() ;
  
  TCHAR buf[ 256 ] ;
  
  _stprintf( buf, _T("%d %d"), pos.x(), pos.y() ) ;
  m_position = buf ;
  
  m_symbol->getRendering( TSLRenderingAttributeSymbolStyle,   &m_style ) ;
  m_symbol->getRendering( TSLRenderingAttributeSymbolColour,  &m_colour ) ;
  m_symbol->getRendering( TSLRenderingAttributeSymbolSizeFactor, &m_sizeFactor ) ;
  m_symbol->getRendering( TSLRenderingAttributeSymbolSizeFactorUnits, &m_sizeFactorUnits ) ;
  m_sizeFactorUnits += 1 ; // Add one since enum begins at -1
  
  m_symbol->getRendering( TSLRenderingAttributeSymbolRotatable, &m_rotatable ) ;
  m_rotatable += 1  ; // Add one since enum begins at -1
  
  m_rotation = (m_symbol->rotation() * 180.0) / M_PI ;
  m_height = m_symbol->height() ;
  
  bool val ;
  m_symbol->getRendering( TSLRenderingAttributeSymbolFixedSize, &val ) ;
  m_fixedSize = val ;
  
  
  m_symbol->getRendering( TSLRenderingAttributeSymbolMinPixelSize, &m_minSize ) ;
  m_symbol->getRendering( TSLRenderingAttributeSymbolMaxPixelSize, &m_maxSize ) ;
  
  Create( IDD_SYMBOL_DIALOG ) ;
  
}


void CSymbolProperties::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CSymbolProperties)
  DDX_Text(pDX, IDC_COLOUR, m_colour);
  DDV_MinMaxInt(pDX, m_colour, 0, 200000);
  DDX_Text(pDX, IDC_ENTITY_ID, m_entityID);
  DDX_Text(pDX, IDC_FEATURE_ID, m_featureID);
  DDX_Check(pDX, IDC_FIXED_SIZE, m_fixedSize);
  DDX_Text(pDX, IDC_HEIGHT, m_height);
  DDX_Text(pDX, IDC_MAX_SIZE, m_maxSize);
  DDX_Text(pDX, IDC_MIN_SIZE, m_minSize);
  DDV_MinMaxInt(pDX, m_minSize, 0, 200000);
  DDX_Text(pDX, IDC_POSITION, m_position);
  DDX_Text(pDX, IDC_RENDER_LEVEL, m_renderLevel);
  DDV_MinMaxInt(pDX, m_renderLevel, -5, 5);
  DDX_Text(pDX, IDC_ROTATION, m_rotation);
  DDV_MinMaxInt(pDX, m_rotation, 0, 360);
  DDX_Text(pDX, IDC_STYLE, m_style);
  DDX_Text(pDX, IDC_SIZE_FACTOR, m_sizeFactor);
  DDX_CBIndex(pDX, IDC_SIZE_FACTOR_UNITS, m_sizeFactorUnits);
  DDX_CBIndex(pDX, IDC_ROTATABLE, m_rotatable);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSymbolProperties, CDialog)
//{{AFX_MSG_MAP(CSymbolProperties)
ON_BN_CLICKED(ID_DELETE_PRIMITIVE, OnDeletePrimitive)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSymbolProperties message handlers

void CSymbolProperties::OnOK() 
{
  UpdateData( TRUE ) ;
  
  m_symbol->setRendering( TSLRenderingAttributeFeatureID,   m_featureID ) ;
  m_symbol->setRendering( TSLRenderingAttributeEntityID,    m_entityID ) ;
  m_symbol->setRendering( TSLRenderingAttributeRenderLevel, m_renderLevel ) ; 
  
  int x = 0, y = 0 ;
  _stscanf( m_position, _T("%d %d"), &x, &y ) ;
  
  m_symbol->position( TSLCoord( x, y ) ) ;
  
  m_symbol->setRendering( TSLRenderingAttributeSymbolStyle,   m_style ) ;
  m_symbol->setRendering( TSLRenderingAttributeSymbolColour,  m_colour ) ;
  m_symbol->setRendering( TSLRenderingAttributeSymbolSizeFactor, m_sizeFactor ) ;
  m_symbol->setRendering( TSLRenderingAttributeSymbolSizeFactorUnits, m_sizeFactorUnits - 1 ) ;
  
  m_symbol->rotation( (m_rotation * M_PI) / 180.0 ) ;
  m_symbol->height( m_height ) ;
  
  m_symbol->setRendering( TSLRenderingAttributeSymbolFixedSize, (bool)m_fixedSize ) ;
  
  m_symbol->setRendering( TSLRenderingAttributeSymbolMinPixelSize, m_minSize ) ;
  m_symbol->setRendering( TSLRenderingAttributeSymbolMaxPixelSize, m_maxSize ) ;
  
  m_symbol->setRendering( TSLRenderingAttributeSymbolRotatable, m_rotatable - 1 ) ;
  
  m_doc->updatedPrimitive( m_symbol ) ;
  
  DestroyWindow() ;
}

void CSymbolProperties::OnDeletePrimitive() 
{
  m_symbol->destroy() ;
  
  m_doc->updatedPrimitive( 0 ) ;
  
  DestroyWindow() ;
}

void CSymbolProperties::PostNcDestroy() 
{
  CDialog::PostNcDestroy();
  delete this ;
}

void CSymbolProperties::OnCancel() 
{
  DestroyWindow() ;
}
