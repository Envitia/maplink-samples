// OverlayOpacityDoc.h : interface of the COverlayOpacityDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OverlayOpacityDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_OverlayOpacityDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class COverlayOpacityDoc : public CDocument
{
  protected: // create from serialization only
    COverlayOpacityDoc();
    DECLARE_DYNCREATE(COverlayOpacityDoc)
    
      // Attributes
  public:
  
    // Operations
  public:
    //This method adds the set of data layers contained 
    //within the document to the specified drawing surface.
      bool addToSurface(TSLDrawingSurface *surface, int index, bool layerBuffered = false);
  
    //This method removes the current set of data layers from 
    //the specified drawing surface.
      void detachFromSurface( TSLDrawingSurface * surface, int index);
  
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(COverlayOpacityDoc)
  public:
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
    virtual void DeleteContents();
    //}}AFX_VIRTUAL
  
    // Implementation
  public:
    virtual ~COverlayOpacityDoc();
  #ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
  #endif
  
    bool addMapDataLayer(LPCTSTR lpszPathName);

    void getMapName(std::string &, int);
    const int getNumberOfDataLayers() { return m_mapDataLayers.size() ; } 
    TSLMapDataLayer * getMapDataLayer(int index = 0);
    long cacheSize() { return m_cacheSize ; }
    void cacheSize(int cs);
    void getFilePathName(CString &path);

    // Opacity varies from:
    //   0 (not visible)
    //   255 (fully opaque)
    bool opacity( int opacity );
    const int opacity() { return m_opacity; }

    bool updateOverlayLayers( TSLDrawingSurface *surface );


  protected:
  
    // Generated message map functions
  protected:
    //{{AFX_MSG(COverlayOpacityDoc)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

  private:
  
    long m_cacheSize;

    int m_opacity;

    // Create the vector to hold the map data layers.
    std::vector <TSLMapDataLayer *> m_mapDataLayers ;
    
    // To hold the map names.
    std::vector <std::string> m_mapLayerNames ;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OverlayOpacityDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
