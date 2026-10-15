// S63WorkFlowDoc.h : interface of the S63WorkFlowDoc class
//
/////////////////////////////////////////////////////////////////////////////
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#if !defined(AFX_S63WorkFlowDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_S63WorkFlowDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// Custom datalayer for drawing 'Not For Navigation'
class NotForNavigationLayer : public TSLClientCustomDataLayer
{
public:
  NotForNavigationLayer() {}
  virtual ~NotForNavigationLayer() {}

  virtual bool drawLayer (TSLRenderingInterface *renderingInterface, const TSLEnvelope* extent, TSLCustomDataLayerHandler& layerHandler);
  virtual bool hasCoordinateSystem () const;
  virtual void releaseResources (int surfaceID);
};

class S63WorkFlowDoc : public CDocument
{
  protected: // create from serialization only
    S63WorkFlowDoc();
    DECLARE_DYNCREATE(S63WorkFlowDoc)
    
      // Attributes
  public:
  
    // Operations
  public:
    //This method adds the set of data layers contained 
    //within the document to the specified drawing surface.
    bool addMapToSurface( TSLDrawingSurface * surface );
  
    //This method removes the current set of data layers from 
    //the specified drawing surface.
    bool detachFromSurface( TSLDrawingSurface * surface ) const;
      
    bool addLayerToSurface( const char * layerName, TSLDataLayer * customLayer, TSLDrawingSurface * surface );

    bool addCustomLayerToSurface( const char * layerName, TSLCustomDataLayer * customLayer, TSLDrawingSurface * surface );

    bool addS63LayerToSurface( TSLS63DataLayer * s63DataLayer, TSLDrawingSurface * surface );
  
    bool addMGRSGridLayerToSurface( TSLDrawingSurface * surface );
    bool addGridLayerToSurface( TSLDrawingSurface * surface );
    bool removeGridLayerFromSurface( TSLDrawingSurface * surface );
    bool removeMGRSGridLayerFromSurface( TSLDrawingSurface * surface );
    bool showingGridLayer();
    bool showingMGRSLayer();

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(S63WorkFlowDoc)
  public:
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
    virtual void DeleteContents();
    //}}AFX_VIRTUAL
  
    // Implementation
  public:
    virtual ~S63WorkFlowDoc();
  #ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
  #endif
  
  protected:
  
    // Generated message map functions
  protected:
    //{{AFX_MSG(S63WorkFlowDoc)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
    
  private:

    // Create any custom layers and attach to the surface.
    void createAndAddCustomLayers(TSLDrawingSurface * surface);

    // The data layer containing the map
    TSLMapDataLayer * m_mapDataLayer ;

    // The S63 data layer
    TSLS63DataLayer * m_s63DataLayer ;

    // Layer to draw the 'Not For Navigation' warning.
    NotForNavigationLayer *m_notForNavigationClientLayer;
    TSLCustomDataLayer *m_notForNavigationCustomLayer;

    TSLLatLongGridDataLayer   *m_pGridLayer;
    TSLMGRSGridDataLayer   *m_pMGRSGridLayer;

  public:
    // Name of my map layer
    static const char * m_mapLayerName ;  
    // Name of the s63 layers 
    static const char * m_s63LayerName ;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_S63WorkFlowDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
