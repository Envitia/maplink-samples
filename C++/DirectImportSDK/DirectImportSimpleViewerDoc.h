// DirectImportSimpleViewerDoc.h : interface of the DirectImportSimpleViewerDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DirectImportSimpleViewerDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_DirectImportSimpleViewerDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define DISPLAY_AS_DATA_CRS -1

class MyCustomClassifier;
class TSLDeviceCapabilities;

class DirectImportSimpleViewerDoc : public CDocument, public TSLDirectImportDataLayerCallbacks
{
  protected: // create from serialization only
    DirectImportSimpleViewerDoc();
    DECLARE_DYNCREATE(DirectImportSimpleViewerDoc)
    
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
  
    void setDefaultInputCRS( long epsgID ) ;
    void setDisplayCRS( long epsgID ) ;    
    
      // Direct Import callback handling
  virtual void requestRedraw();
  virtual void onDeviceCapabilitiesRequired( TSLDeviceCapabilities& capabilities );
  virtual const TSLCoordinateSystem* onNoCoordinateSystem( const TSLDirectImportDataSet* dataSet, TSLDirectImportDriver* driver );
  virtual void onTileLoadComplete( const TSLDirectImportDataSet* dataSet, unsigned int numProcessing, unsigned int numProcessingTotal );

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(DirectImportSimpleViewerDoc)
  public:
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
    virtual void DeleteContents();
    //}}AFX_VIRTUAL
  
    // Implementation
  public:
    virtual ~DirectImportSimpleViewerDoc();
  #ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
  #endif
  
  protected:
  
    // Generated message map functions
  protected:
    //{{AFX_MSG(DirectImportSimpleViewerDoc)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
  afx_msg void OnCrsDefaultTo27700();
  afx_msg void OnUpdateCrsDefaultTo4326(CCmdUI *pCmdUI);
  afx_msg void OnUpdateCrsDefaultTo27700(CCmdUI *pCmdUI);
  afx_msg void OnCrsDefaultTo4326();
  afx_msg void OnCrsDisplayUsingDataCrs();
  afx_msg void OnUpdateCrsDisplayUsingDataCrs(CCmdUI *pCmdUI);
  afx_msg void OnCrsDisplayUsing4326();
  afx_msg void OnUpdateCrsDisplayUsing4326(CCmdUI *pCmdUI);
  afx_msg void OnCrsDisplayUsing27700();
  afx_msg void OnUpdateCrsDisplayUsing27700(CCmdUI *pCmdUI);
  afx_msg void OnCrsDisplayUsing3395();
  afx_msg void OnUpdateCrsDisplayUsing3395(CCmdUI *pCmdUI);    
  private:

    TSLMapDataLayer          * m_mapDataLayer;
    TSLDirectImportDataLayer * m_directImportDataLayer ;
    MyCustomClassifier       * m_myClassifier;
  
    // Name of my direct import layer
    static const char * m_layerName ;
    static const char * m_mapLayerName;
  
    long m_defaultInputCRS ;
    TSLCoordinateSystem* m_defaultInputCS;
    long m_displayCRS ;

    const TSLCoordinateSystem*  determineOutputCoordinateSystem( bool requiresMapUnits, const TSLDirectImportDataSet * dataSet, int selectedCRS);
    TSLFeatureClassConfig *  getFeatureClassConfig(TSLDirectImportDataSet * dataSet);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DirectImportSimpleViewerDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
