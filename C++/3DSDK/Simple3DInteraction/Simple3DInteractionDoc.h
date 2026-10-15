// Simple3DInteractionDoc.h : interface of the Simple3DInteractionDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_Simple3DInteractionDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_Simple3DInteractionDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class Simple3DInteractionDoc : public CDocument
{
  protected: // create from serialization only
    Simple3DInteractionDoc();
    DECLARE_DYNCREATE(Simple3DInteractionDoc)
    
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
  
  
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(Simple3DInteractionDoc)
  public:
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
    virtual void DeleteContents();
    //}}AFX_VIRTUAL
  
    // Implementation
  public:
    virtual ~Simple3DInteractionDoc();
  #ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
  #endif
  
  protected:
  
    // Generated message map functions
  protected:
    //{{AFX_MSG(Simple3DInteractionDoc)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
    
  private:
  
    // The data layer containing the map
    TSLMapDataLayer * m_mapDataLayer ;
    TSL3DTerrainDataLayer * m_terrainDataLayer ;
    TSL3DStandardDataLayer *m_stdDataLayer;

    // Name of my map layer
    static const char * m_mapLayerName ;
    static const char * m_terrainLayerName ;
    static const char * m_stdLayerName ;
  
public:
  afx_msg void OnFileOpenterrain();
  afx_msg void OnCreatePolygon();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_Simple3DInteractionDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
