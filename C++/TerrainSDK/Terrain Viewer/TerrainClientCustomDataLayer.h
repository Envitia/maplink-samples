// TerrainClientCustomDataLayer.h: interface for the CTerrainClientCustomDataLayer class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _TERRAINCLIENTCUSTOMDATALAYER_H_
#define _TERRAINCLIENTCUSTOMDATALAYER_H_

#include "MapLinkTerrainViewShed.h"
#include "TerrainViewerOptions.h"
#include <vector>

/*
 * This class acts as both the viewshed compositor object, and as a custom datalayer
 * which displays the calculated result.
 *
 * The viewshed results are stored in m_bitmap, which is created based on the extent of a particular viewshed operation, 
 * and the resolution of the input data.
 * This bitmap is scaled/located during drawLayer, in order to overlay the results on the current map.
 *
 * This layer has been superceeded by the TSLViewShedDataLayer
 * It has been left in the sample as an example of how to render viewsheds within a custom datalayer,
 * but is not the recommended approach unless applications require different functionality than the
 * TSLViewShedDataLayer can provide.
 */
class CTerrainClientCustomDataLayer : public TSLClientCustomDataLayer, public TSLTerrainVSCompositor
{
public:
  CTerrainClientCustomDataLayer();
  virtual ~CTerrainClientCustomDataLayer();

  bool drawLayer (TSLRenderingInterface* renderingInterface, const TSLEnvelope * extent, TSLCustomDataLayerHandler& layerHandler);

  void clearBitmap();  
  void resetBitmapToNoData();

  virtual void setData (unsigned int x, unsigned int y, TSLTerrainVSVisibility visibility, double terrainHeight, const TSLTerrainVSAlgorithm::Parameters& viewshedParameters);
  virtual bool valid (const TSLTerrainVSAlgorithm::Parameters& params);

  inline void setHDC( HDC hdc );
  inline void setOptions( TerrainViewerOptions& options );
  inline void clearOnNext( );
  void setOverallExtent( const TSLMUExtent & overallExtent, TSLTerrainVSInput * input, const std::vector< TSLMUCoord > & fixedPoints ) ; // Assume rounded to align with input grid

private:
  // Stored, so that the bitmap can be created during valid()
  HDC m_ldc;

  // Output bitmap
  HBITMAP m_bitmap;
  unsigned char* m_bits;
  unsigned int   m_rowLen;
  bool           m_cleared ;
  unsigned char* createDIB(HDC hdc, HBITMAP& hbmap, unsigned int width, unsigned int height, unsigned int* rowlength);

  // The output bitmap parameters
  unsigned int m_outputWidth;
  unsigned int m_outputHeight;
  TSLMUExtent m_overallQueryExtent ;
  TSLMUExtent m_overallDisplayExtent ;

  std::vector< TSLMUCoord > m_fixedPoints ;

  // For a single Viewshed operation
  int m_offsetX;
  int m_offsetY;
  TSLMUExtent m_outputExtent; 

  // The colours specified by the options dialog
  uint32_t m_visibleColour;
  uint32_t m_notVisibleColour;
  uint32_t m_noDataColour;
};


void CTerrainClientCustomDataLayer::setHDC( HDC hdc )
{
  m_ldc = CreateCompatibleDC(hdc);
}

void CTerrainClientCustomDataLayer::setOptions( TerrainViewerOptions& options )
{
  m_visibleColour = options.m_viewShedColourVisible;
  m_notVisibleColour = options.m_viewShedColourNotVisible;
  m_noDataColour = options.m_viewShedColourNoData;

  // Now merge in the alpha channel
  if ( !options.m_viewShedColourNoDataTransparent )
    m_noDataColour |= 0xFF000000 ;
  if ( !options.m_viewShedColourVisibleTransparent )
    m_visibleColour |= 0xFF000000 ;
  if ( !options.m_viewShedColourNotVisibleTransparent )
    m_notVisibleColour |= 0xFF000000 ;
}

void CTerrainClientCustomDataLayer::clearOnNext( )
{
  m_cleared = false ;
}

#endif
