// NITFDirectImportSampleDoc.h : interface of the NITFDirectImportSampleDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_NITFDirectImportHarnessDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_NITFDirectImportHarnessDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CProgressDialog;
class CMetadataDlg;

class NITFDirectImportSampleDoc : public CDocument
{
  protected: // create from serialization only
    NITFDirectImportSampleDoc();
    DECLARE_DYNCREATE(NITFDirectImportSampleDoc)
    
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
  
    bool openMap(const char* mapPath);

    TSLNITFFilterDataLayer * getNITFDataLayer() {return m_filterDataLayer; }
    
    afx_msg void OnFileSaveAll();

    afx_msg void OnUpdateViewProgress(CCmdUI *pCmdUI);
    afx_msg void OnViewProgress();
    afx_msg void OnViewMetadatainfo();
    afx_msg void OnUpdateViewMetadatainfo(CCmdUI *pCmdUI);
    afx_msg void OnViewDisplayitemsettingsbeforeprocess();
    afx_msg void OnUpdateViewDisplayitemsettingsbeforeprocess(CCmdUI *pCmdUI);
    afx_msg void OnToolsDisplayitemsettings();
    afx_msg void OnUpdateToolsDisplayitemsettings(CCmdUI *pCmdUI);
    afx_msg void OnToolsDirectories();
    afx_msg void OnUpdateFileSave(CCmdUI *pCmdUI);

    CProgressDialog * m_progressDialog;
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(NITFDirectImportSampleDoc)
  public:
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
    virtual void DeleteContents();
    //}}AFX_VIRTUAL
  
    // Implementation
  public:
    virtual ~NITFDirectImportSampleDoc();
  #ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
  #endif
  
  protected:
  
    // Generated message map functions
  protected:
    //{{AFX_MSG(NITFDirectImportSampleDoc)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
    
  private:
  
    // The data layer containing the map
    TSLMapDataLayer * m_mapDataLayer ;
  
    // Name of my map layer
    static const char * m_mapLayerName ;

    //The Fitlers Data Layer
    TSLNITFFilterDataLayer * m_filterDataLayer;

    //Name of the filter layer
    static const char * m_filterLayerName;

    bool m_showProgressBar;

    bool m_showMetadataDlg;

    CMetadataDlg * m_metadatadlg;

    bool m_showItemSettingsBeforeDraw;

    CString m_attachmentsDir;
    CString m_outputDir;
    CString m_tempDirectory;

    bool m_pyramidRasters;

    static CString m_filterDLL;
    static CString m_unlockCode;
    static CString m_configFile;
    static bool m_alreadyUnlocked;

    //raster pyramid options
    TSLRasterInterpolation m_interpolation;
    bool m_multipage;
    TSLRasterType m_rtype;
    int m_compressionFactor;

public:
  afx_msg void OnFileSaveitem();
  afx_msg void OnToolsSettings();
  afx_msg void OnUpdateToolsSettings(CCmdUI *pCmdUI);
  afx_msg void OnSettingsPyramidrasters();
  afx_msg void OnUpdateSettingsPyramidrasters(CCmdUI *pCmdUI);
  afx_msg void OnSettingsPyramidingsettings();
  afx_msg void OnUpdateSettingsPyramidingsettings(CCmdUI *pCmdUI);
  afx_msg void OnClippingGeodeticclipping();
  afx_msg void OnClippingOutputclipping();
  afx_msg void OnUpdateClipping(CCmdUI *pCmdUI);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NITFDirectImportHarnessDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
