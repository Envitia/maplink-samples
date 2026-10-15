// AcceleratedSampleDoc.h : interface of the AcceleratedSampleDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_AcceleratedSampleDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_AcceleratedSampleDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class AcceleratedSampleCustomDataLayer;


class AcceleratedSampleDoc : public CDocument
{
  protected: // create from serialization only
    AcceleratedSampleDoc();
    DECLARE_DYNCREATE(AcceleratedSampleDoc)
    
      // Attributes
  public:
  
    // Operations
  public:
    //This method adds the set of data layers contained 
    //within the document to the specified drawing surface.
    bool addToSurface( TSLDrawingSurface * surface );
  
    //This method removes the current set of data layers from 
    //the specified drawing surface.
    bool detachFromSurface( TSLDrawingSurface * surface );
  
    //This method returns the TMC extent of the map data layer.
    void getMapExtent(TSLEnvelope &envelope);
  
    bool mapLoaded()
    {
      return m_mapDataLayer ? true : false;
    }
  
    void rasterBuffering(TSLDrawingSurface * surface);
    void changeFrontLayerVisibility(TSLDrawingSurface * surface);
    void changeBackLayerVisibility(TSLDrawingSurface * surface);

    bool getFrontLayerVisibility() const
    {
      return m_frontLayerVisibility;
    }

    bool getBackLayerVisibility() const
    {
      return m_backLayerVisibility;
    }

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(AcceleratedSampleDoc)
  public:
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
    virtual void DeleteContents();
    afx_msg void OnToolsUnrotateRectangle();
    afx_msg void OnUpdateToolsAction(CCmdUI *pCmdUI);
    //}}AFX_VIRTUAL
  
    // Implementation
  public:
    virtual ~AcceleratedSampleDoc();
  #ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
  #endif
  
  protected:
  
    // Generated message map functions
  protected:
    //{{AFX_MSG(AcceleratedSampleDoc)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
    
  private:
    void createGeometry(TSLStandardDataLayer *layer);

    // The data layer containing the map
    TSLMapDataLayer * m_mapDataLayer ;

    // Accelerator SDK Custom data layer.
    TSLAcceleratedCustomDataLayer *m_customDataLayer;

    // Client to add to the custom Layer : does the user drawing.
    AcceleratedSampleCustomDataLayer *m_customClient;

    // standard datalayer
    bool m_backLayerVisibility;
    bool m_frontLayerVisibility;
    TSLStandardDataLayer * m_stdDataLayer1 ; // back
    TSLStandardDataLayer * m_stdDataLayer2 ; // front

    // raster data layer
    bool m_rasterBuffering;
    TSLRasterDataLayer * m_rasterDataLayer ;

    // Name of my map layer
    static const char * m_mapLayerName ;

    // Name of my custom layer.
    static const char * m_customLayerName ;

    // Name of standard data layer.
    static const char * m_stdLayerName1 ;
    static const char * m_stdLayerName2 ;

    // Name of raster datalayer
    static const char * m_rasterLayerName ;

public:
  afx_msg void OnFileLoadraster();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AcceleratedSampleDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
