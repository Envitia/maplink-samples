// SatellitePropagator3DDoc.h : interface of the SatellitePropagator3DDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SatellitePropagator3DDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_SatellitePropagator3DDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class SatellitePropagator3DDoc : public CDocument
{
  protected: // create from serialization only
    SatellitePropagator3DDoc();
    DECLARE_DYNCREATE(SatellitePropagator3DDoc)
    
      // Attributes
  public:
  
    // Operations
  public:
    // This method adds the set of data layers contained 
    // within the document to the specified drawing surface.
    bool addToSurface( TSLDrawingSurfaceBase * surface, TSLTimeServer * timeServer );
  
    // This method removes the current set of data layers from 
    // the specified drawing surface.
    bool detachFromSurface( TSLDrawingSurfaceBase * surface );

  
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(SatellitePropagator3DDoc)
  public:
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
    virtual void DeleteContents();
    //}}AFX_VIRTUAL
  
    // Implementation
  public:
    virtual ~SatellitePropagator3DDoc();
  #ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
  #endif
  
  protected:
  
    // Generated message map functions
  protected:
    //{{AFX_MSG(SatellitePropagator3DDoc)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
    
  private:

    // This method loads the satellites and adds them to the data layer
    void loadSatellites( ) ;

    // The data layer containing the map
    TSLMapDataLayer * m_mapDataLayer ;
    static const char * m_mapLayerName ;
  
    // The data layer containing the satellites
    TSL3DSatelliteDataLayer * m_satDataLayer ;
    static const char * m_satLayerName ;

  
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SatellitePropagator3DDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
