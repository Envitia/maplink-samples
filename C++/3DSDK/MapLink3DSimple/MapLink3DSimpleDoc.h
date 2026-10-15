// MapLink3DSimpleDoc.h : interface of the MapLink3DSimpleDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MapLink3DSimpleDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_MapLink3DSimpleDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class MapLink3DSimpleDoc : public CDocument
{
  protected: // create from serialization only
    MapLink3DSimpleDoc();
    DECLARE_DYNCREATE(MapLink3DSimpleDoc)
    
      // Attributes
  public:
  
    // Operations
  public:
    //This method adds the set of data layers contained 
    //within the document to the specified drawing surface.
    bool addToSurface( TSLDrawingSurfaceBase * surface );
  
    //This method removes the current set of data layers from 
    //the specified drawing surface.
    bool detachFromSurface( TSLDrawingSurfaceBase * surface );
  
    // This method creates a default white polygon.
    // Used for demonstrating lighting effects.
    void createDefaultPolygon();

    // used to demonstrate primitive creation
    void createPolygon();
    void createExtrudedPolygon();

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(MapLink3DSimpleDoc)
  public:
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
    virtual void DeleteContents();
    //}}AFX_VIRTUAL
  
    // Implementation
  public:
    virtual ~MapLink3DSimpleDoc();
  #ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
  #endif
  
  protected:
  
    // Generated message map functions
  protected:
    //{{AFX_MSG(MapLink3DSimpleDoc)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
    
  private:

    void loadMap();
  
    // The data layer containing the map
    TSLMapDataLayer       * m_mapDataLayer ;
    TSL3DTerrainDataLayer * m_terrainDataLayer ;
    TSL3DStandardDataLayer *m_stdDataLayer;
    TSLStandardDataLayer *m_dataLayer;
    TSLCustomDataLayer *m_customLayer;

    bool m_defaultCreated;
    bool m_newMap;
    std::string m_mapName;
  
    // Name of my map and terrain layers
    static const char * m_mapLayerName ;
    static const char * m_terrainLayerName ;
  
public:
  afx_msg void OnFileLoadterraindatabase();

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MapLink3DSimpleDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
