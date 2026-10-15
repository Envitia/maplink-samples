// TerrainClientCustomDataLayer.cpp: implementation of the CTerrainClientCustomDataLayer class.
//
//////////////////////////////////////////////////////////////////////

/*
 * This layer has been superceeded by the TSLViewShedDataLayer
 * It has been left in the sample as an example of how to render viewsheds within a custom datalayer,
 * but is not the recommended approach unless applications require different functionality than the
 * TSLViewShedDataLayer can provide.
 */

#include "stdafx.h"
#include "resource.h"
#include "TerrainViewerView.h"
#include "TerrainViewerDoc.h"
#include "MainFrm.h"
#include "TerrainClientCustomDataLayer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

inline int _round(double val)
{
  return (val < 0.0) ? (int)(val - 0.5) : (int)(val + 0.5) ;
}

CTerrainClientCustomDataLayer::CTerrainClientCustomDataLayer()
  : m_ldc(NULL)
  , m_bitmap(NULL)
  , m_bits(NULL)
  , m_rowLen(0)
  , m_outputWidth(0)
  , m_outputHeight(0)
  , m_offsetX(0)
  , m_offsetY(0)
  , m_outputExtent()
  , m_visibleColour(0x00000000)
  , m_notVisibleColour(0x00000000)
  , m_noDataColour(0x00000000)
  , m_cleared( false )
{
}

CTerrainClientCustomDataLayer::~CTerrainClientCustomDataLayer()
{
  if( m_bitmap )
  {
    DeleteObject(m_bitmap);
  }
  if( m_ldc )
  {
    DeleteDC(m_ldc);
  }
}

void CTerrainClientCustomDataLayer::clearBitmap()
{
  if( m_bitmap )
  {
    DeleteObject(m_bitmap);
	m_outputWidth = 0;
	m_outputHeight = 0;
    m_bitmap = NULL;
    m_bits = NULL;
  }
}

void CTerrainClientCustomDataLayer::resetBitmapToNoData()
{
  if ( m_bits )
  {
    // Reset the output to the 'no data' colour.
    for( unsigned int y(0); y < m_outputHeight; ++y )
    {
      uint32_t* pixel = (uint32_t*)(m_bits + (y * m_rowLen));
      for( unsigned int x(0); x < m_outputWidth; ++x )
      {
         *pixel++ = m_noDataColour ;
      }
    }
    m_cleared = true ;
  }
}

bool CTerrainClientCustomDataLayer::drawLayer (TSLRenderingInterface* renderingInterface, const TSLEnvelope * extent, TSLCustomDataLayerHandler& layerHandler)
{
  if( !CMainFrame::instance()->getCurrentMapView()->isAreaLOSViewActive() )
  {
    return false;
  }

  if( !m_bitmap )
  {
    return false;
  }

  // Convert the output extent to display units.
  const TSLCoordinateSystem* cs = renderingInterface->queryCoordinateSystem();
  TSLTMC viewshedX1, viewshedY1, viewshedX2, viewshedY2;
  TSLDeviceUnits dux1, duy1, dux2, duy2;
  if( CTerrainViewerDoc::m_thisDocument->isConversionRequired() )
  {
    const TSLCoordinateSystem* terrainCS = CTerrainViewerDoc::m_thisDocument->terrainCoordinateSystem();
    double lat1, lon1, lat2, lon2;
    terrainCS->MUToLatLong(m_overallDisplayExtent.minX(), m_overallDisplayExtent.minY(), &lat1, &lon1);
    terrainCS->MUToLatLong(m_overallDisplayExtent.maxX(), m_overallDisplayExtent.maxY(), &lat2, &lon2);

    cs->latLongToTMC(lat1, lon1, &viewshedX1, &viewshedY1);
    cs->latLongToTMC(lat2, lon2, &viewshedX2, &viewshedY2);
  }
  else
  {
    cs->MUToTMC(m_overallDisplayExtent.minX(), m_overallDisplayExtent.minY(), &viewshedX1, &viewshedY1);
    cs->MUToTMC(m_overallDisplayExtent.maxX(), m_overallDisplayExtent.maxY(), &viewshedX2, &viewshedY2);
  }

  renderingInterface->TMCToDU(viewshedX1, viewshedY1, &dux1, &duy1);
  renderingInterface->TMCToDU(viewshedX2, viewshedY2, &dux2, &duy2);

  // The output bitmap was created with 0,0 in the bottom-left.
  // Ensure the corners are the correct way round.
  if( duy2 < duy1 )
  {
    std::swap(duy1, duy2);
  }
  if( dux2 < dux1 )
  {
    std::swap(dux1, dux2);
  }

  unsigned int displayWidth = dux2 - dux1;
  unsigned int displayHeight = duy2 - duy1;

  // Draw the viewshed result.
  // The GDI bitmap functions will upscale the bitmap, but cannot perform interpolation in the process.
  HBITMAP old = (HBITMAP)SelectObject(m_ldc, m_bitmap);
  HDC hdc = (HDC)renderingInterface->handleToDrawable();

  SetStretchBltMode(hdc, HALFTONE);

  BLENDFUNCTION bf;
  bf.BlendOp             = AC_SRC_OVER;
  bf.BlendFlags          = 0;
  bf.SourceConstantAlpha = (BYTE)0x7F ;
  bf.AlphaFormat         = AC_SRC_ALPHA ;
  AlphaBlend(hdc, dux1, duy1, displayWidth, displayHeight, m_ldc, 0, 0, m_outputWidth, m_outputHeight, bf);

  SelectObject(m_ldc, old);

  // Now draw symbols at each of the fixed points
  if ( m_fixedPoints.size() ) 
  {
    ::SelectObject( hdc, ::GetStockObject( WHITE_BRUSH ) ) ;
    ::SelectObject( hdc, ::GetStockObject( BLACK_PEN ) ) ;
    for ( size_t i = 0 ; i < m_fixedPoints.size() ; ++i )
    {
      TSLDeviceUnits dux, duy ;
      layerHandler.drawingSurface()->MUToDU( m_fixedPoints[i].x(), m_fixedPoints[i].y(), &dux, &duy ) ;
      ::Ellipse( hdc, dux - 4, duy - 4, dux + 4, duy + 4) ;
    }
  }
  return true;
}

void CTerrainClientCustomDataLayer::setData (unsigned int x, unsigned int y, TSLTerrainVSVisibility visibility, double terrainHeight, const TSLTerrainVSAlgorithm::Parameters& viewshedParameters)
{
  // The current point is specified relative to the bottom-left of the input data object.
  // Adjust this to be relative to the bottom-left of the output bitmap.
  x -= m_offsetX;
  y -= m_offsetY;

  if( x >= m_outputWidth || y >= m_outputHeight )
  {
    return;
  }
  if( !m_bits )
  {
    return;
  }

  uint32_t colour = 0x00000000;

  switch( visibility )
  {
    case TSLTerrainVSVisible:
      colour = m_visibleColour;
      break;
    case TSLTerrainVSNotVisible:
      colour = m_notVisibleColour;
      break;
    case TSLTerrainVSNoData:
      colour = m_noDataColour;
      break;
  }

  // Colour the output pixel based on the visibility of this point
  // m_outputData is a 32-bit image.
  uint32_t* pixel = (uint32_t*)(m_bits + (y * m_rowLen) + (x * 4)); 

  if( *pixel == m_visibleColour )
  {
    /* The RFVS viewshed algorithm can pass over
     * some points more than once.
     * In this case, the point is considered visible if
     * any of the calculations say it is.
     * This is especially important when combining the RFVS viewshed
     * with earth curvature corrections.
     */
    return;
  }
  *pixel = colour;
}

bool CTerrainClientCustomDataLayer::valid (const TSLTerrainVSAlgorithm::Parameters& params)
{
  // This is called for each viewshed operation

  // Allocate the results array
  // Size is determined based on the MU extent of the current viewshed operation,
  // and the post distance of the input data.
  // This will not match the current display resolution.

  m_outputExtent = params.m_viewShedMUExtent;

 TSLTerrainVSInput* input = params.inputData();

  // Calculate the offset between the bottom-left of the input data,
  // and the bottom-left of the viewshed operation's extent.
  // This is needed as the viewshed operation is performed based on integer coordinates, 
  // relative to the bottom-left of the input data extent.
  m_offsetX = _round((m_overallQueryExtent.minX() - input->queryExtent().minX()) / input->postDistanceX());
  m_offsetY = _round((m_overallQueryExtent.minY() - input->queryExtent().minY()) / input->postDistanceY());
  
  return true;
}

void CTerrainClientCustomDataLayer::setOverallExtent( const TSLMUExtent & overallExtent, TSLTerrainVSInput * input, const vector< TSLMUCoord > & fixedPoints )
{
  // Allocate the results array
  // Size is determined based on the MU extent of the overall extent operation,
  // and the post distance of the input data.
  // This will not match the current display resolution.
  m_overallQueryExtent = overallExtent ;

 double widthMU  = m_overallQueryExtent.width() ;
 double heightMU = m_overallQueryExtent.height() ;
 double pdx = input->postDistanceX() ;
 double pdy = input->postDistanceY() ;
 double numPostIntervalsX = widthMU / pdx ;
 double numPostIntervalsY = heightMU / pdy ;

 unsigned int outputWidthDU =  _round( numPostIntervalsX ) + 1 ;
 unsigned int outputHeightDU = _round( numPostIntervalsY ) + 1 ;

  // Assume the query extent has been aligned to the grid at input - expand it by 1/2 pixel either side to get the display extent
  m_overallDisplayExtent.setExtent( overallExtent.minX() - pdx / 2.0, 
                                    overallExtent.minY() - pdy / 2.0, 
                                    overallExtent.maxX() + pdx / 2.0, 
                                    overallExtent.maxY() + pdy / 2.0 ) ; 

  if( outputWidthDU  != m_outputWidth ||
      outputHeightDU != m_outputHeight )
  {
    // Delete the old result, and recreate if needed.
    OutputDebugString( _T("Recreating bitmap since size changed\n" ) ) ;
    clearBitmap();
    m_outputWidth  = outputWidthDU;
    m_outputHeight = outputHeightDU;
    m_bits = createDIB(m_ldc, m_bitmap, m_outputWidth, m_outputHeight, &m_rowLen);
    m_cleared = false ;
  }

  if ( !m_cleared )
  {
    OutputDebugString( _T("Clearing bitmap to no data\n") ) ;
    resetBitmapToNoData() ;
  }

  m_fixedPoints = fixedPoints ;
}

unsigned char* CTerrainClientCustomDataLayer::createDIB(HDC hdc, HBITMAP& hbmap, unsigned int width, unsigned int height, unsigned int* rowlength)
{
  // Row length in bytes.
  unsigned int rowlen = width * 4;
  void* bits = 0;
  
  BITMAPINFO bmi;
  bmi.bmiHeader.biSize = sizeof (BITMAPINFOHEADER);
  bmi.bmiHeader.biWidth = width;
  bmi.bmiHeader.biHeight = height;
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 32;
  bmi.bmiHeader.biCompression = BI_RGB;
  bmi.bmiHeader.biSizeImage = rowlen * height;
  bmi.bmiHeader.biXPelsPerMeter = 0;
  bmi.bmiHeader.biYPelsPerMeter = 0;
  bmi.bmiHeader.biClrUsed = 0;
  bmi.bmiHeader.biClrImportant = 0;
  bmi.bmiHeader.biCompression = 0;

  hbmap = CreateDIBSection( hdc, &bmi, DIB_RGB_COLORS, &bits, 0, 0);

  *rowlength = rowlen;
  return (unsigned char*)bits;
}
