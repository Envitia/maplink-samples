// TerrainViewerDoc.h : interface of the CTerrainViewerDoc class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _TERRAINVIEWERDOC_H_
#define _TERRAINVIEWERDOC_H_

#include <vector>

class CTerrainViewerDoc : public CDocument
{
public:
  static CTerrainViewerDoc* m_thisDocument;
  
  virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
  virtual void DeleteContents();
  
  virtual ~CTerrainViewerDoc();
  
  void openTerrain( LPCTSTR path ) ;
  TSLTerrainDatabase * getTerrainDB();
  const TSLCoordinateSystem* terrainCoordinateSystem();

  bool isConversionRequired() { return m_conversionRequired ; }

  bool calculateViewshed(double startX, double startY, const std::vector< TSLMUCoord > & fixedPositions, const TSLMUExtent& displayExtent, TSLDeviceUnits displayWidth, TSLDeviceUnits displayHeight);

  TSLStandardDataLayer* getVisDataLayer()
  {
    return m_visDataLayer;
  }
  
  TSLMapDataLayer *getMapDataLayer()
  { 
    return m_mapDataLayer ; 
  }
  
  TSLStandardDataLayer* getExtentLayer()
  {
    return m_extentLayer;
  }

  TSLViewShedDataLayer* getViewShedDataLayer() const
  {
    return m_viewShedDataLayer;
  }

#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif
  
  
protected: // create from serialization only
  CTerrainViewerDoc();
  DECLARE_DYNCREATE(CTerrainViewerDoc)
  afx_msg void OnImportTerrain();
  DECLARE_MESSAGE_MAP()
    
private:
  // This flag will be true, if the current map and terrain database 
  // are in different coordinate systems.
  bool m_conversionRequired;

  CString m_mapCoordinateSystem;
  CString m_terrainCoordinateSystem;

  // The data layer containing the map
  TSLMapDataLayer * m_mapDataLayer ;

  // The data layer for calculating and visualising viewsheds
  TSLViewShedDataLayer* m_viewShedDataLayer;

  // The data layer for visualising a single LOS
  TSLStandardDataLayer* m_visDataLayer;
  // The data layer for displaying the terrain database's extent
  TSLStandardDataLayer * m_extentLayer;
};

#endif
