// TerrainContouringDoc.h : interface of the TerrainContouringDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TerrainContouringDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_TerrainContouringDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class TerrainContouringProgress;


class TerrainContouringDoc : public CDocument
{
protected: // create from serialization only
  TerrainContouringDoc();
  DECLARE_DYNCREATE(TerrainContouringDoc)

  // Attributes
public:
  inline TSLTerrainDatabase* getTerrainDatabase();
  inline TSLMapDataLayer* getMapLayer();
  inline double getMaximumTerrainHeight();
  inline TSLStandardDataLayer* getContourLayer();
  inline long getTextSizeFactor();

  // Operations
public:
  //This method adds the set of data layers contained 
  //within the document to the specified drawing surface.
  bool addToSurface( TSLDrawingSurface * surface );

  //This method removes the current set of data layers from 
  //the specified drawing surface.
  bool detachFromSurface( TSLDrawingSurface * surface );

  // This method loads data from a terrain database and converts it into 
  // a list of vertices with associated height data for the contouring.
  // process.
  bool loadTerrainData( const CString &terrainDBFile, bool useFullExtent, double x1 = 0.0, double y1 = 0.0, double x2 = 0.0, double y2 = 0.0 );

  // This method redraws the contours on the map.
  void drawContours();

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(TerrainContouringDoc)
public:
  virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
  virtual void DeleteContents();
  //}}AFX_VIRTUAL

  // Implementation
public:
  virtual ~TerrainContouringDoc();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

protected:

  // Generated message map functions
protected:
  //{{AFX_MSG(TerrainContouringDoc)
  // NOTE - the ClassWizard will add and remove member functions here.
  //    DO NOT EDIT what you see in these blocks of generated code !
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()

private:
  void updateMenuState();

  // The data layer containing the map
  TSLMapDataLayer * m_mapDataLayer ;

  // Handle to a standard data layer containing the contour lines
  TSLStandardDataLayer * m_contourLayer ;

  // The database containing the terrain data
  CString m_terrainDatabaseName ;
  TSLTerrainDatabase m_terrainDB;

  // The contouring interface object
  TSLTerrainContour * m_contour;

  // Dimensions to use for the terrain grid
  int m_terrainGridWidth;
  int m_terrainGridHeight;

  // Number of levels of contour lines to show
  int m_numContourLevels;

  // Lowest and highest height values for our terrain data
  double m_lowestHeight;
  double m_highestHeight;

  // Boundaries of the terrain data used when last loading the terrain data
  double m_terrainX1;
  double m_terrainY1;
  double m_terrainX2;
  double m_terrainY2;

  // Type of contour lines to draw when drawing lines
  TSLTerrainContourLineType m_contourLineType;

  // Whether we should draw polygons instead of lines
  bool m_drawContourPolygons;

  // Name of my map layer
  static string m_mapLayerName ;

  // Name of the contour layer
  static string m_contourLayerName;

  // Prefix to prepend to contour line text labels 
  static string m_contourLabelPrefix;

  // Height of the text labels in units of the terrain data
  long m_textSizeFactor;

  // Whether or not to draw text labels
  bool m_drawTextLabels;

  // The alignment of the text labels
  TSLVerticalAlignment m_textAlignment;

  TerrainContouringProgress *m_progressDialog;

public:
  afx_msg void OnTerrainDBOpen();
  afx_msg void OnRecontourVisibleArea();
  afx_msg void OnContourTypeSimple();
  afx_msg void OnContourTypeStandard();
  afx_msg void OnContourTypeConrec();
  afx_msg void OnContourTypePolygon();
  afx_msg void OnContouringToggleLabels();
  afx_msg void OnTextLabelAlignmentBelow();
  afx_msg void OnTextLabelAlignmentMiddle();
  afx_msg void OnTextLabelAlignmentAbove();
  afx_msg void OnChangeContouringOptions();
};

// Inline attribute retrieval functions
TSLStandardDataLayer* TerrainContouringDoc::getContourLayer()
{
  return m_contourLayer;
}

long TerrainContouringDoc::getTextSizeFactor()
{
  return m_textSizeFactor;
}

TSLTerrainDatabase* TerrainContouringDoc::getTerrainDatabase()
{
  return &m_terrainDB;
}

TSLMapDataLayer* TerrainContouringDoc::getMapLayer()
{
  return m_mapDataLayer;
}

double TerrainContouringDoc::getMaximumTerrainHeight()
{
  return m_highestHeight;
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TerrainContouringDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
