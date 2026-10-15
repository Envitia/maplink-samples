// RuntimeProjTestDoc.h : interface of the CRuntimeProjTestDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MFCDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_MFCDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CRuntimeProjTestDoc : public CDocument
{
  protected: // create from serialization only
    CRuntimeProjTestDoc();
    DECLARE_DYNCREATE(CRuntimeProjTestDoc)
    
      // Attributes
  public:
  
    // Operations
  public:
    //
    //This method adds the set of data layers contained 
    //within the document to the specified drawing surface.
    bool addToSurface( TSLDrawingSurface * surface );

    //This method removes the current set of data layers from 
    //the specified drawing surface.
    bool detachFromSurface( TSLDrawingSurface * surface );

    // called in main thread when successful tile load callback happened
    void SuccessfulCallbackAction( ); 

    // called in main thread when all tiles have loaded following a draw
    void AllLoadedCallbackAction( );

    // called in main thread as a result of message from projection changed callback
    void projectionChangedCallbackAction( ); 


    bool mapLoaded() const { return m_mapLoaded; }

    bool runtimeProjectionSet() const; 
    void initialProjection();
    void setProjection(int choice, double maxDistance, double lat, double lon);
  
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CRuntimeProjTestDoc)
  public:
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void DeleteContents();
    //}}AFX_VIRTUAL
  
    // Implementation
  public:
    virtual ~CRuntimeProjTestDoc();
  #ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
  #endif
  
  protected:
    void cleanup();

    // callback function - which via a bit of jiggerypokery causes  
    // SuccessfulCallbackAction to be called in the main thread if status = success and 100% done.
    static TSLLoaderCallbackReturn mapCallbackFunction( void* arg, const char* filename, TSLEnvelope extent, TSLLoaderStatus status, int percentDone );
  
    // allLoaded callback function - which by suitable means causes
    // AllLoadedCallbackAction to be called in the main thread
    static void allLoadedCallbackFunction( void* arg2 );

    // callback called when runtime projection has changed
    static void runtimeProjectionChangedCallback( void* arg, TSLRuntimeProjectionChangedEnum reason );

    // set the runtime projection on the map data layer
    bool setRuntimeProjection( TSLCoordinateSystem* runtimeCS );

    // work out the lat,long centre of the currently loaded map.
    // NOTE: this also clears any runtime projection already loaded.
    bool getMapCentre( double* latd, double* longtd );

    // Generated message map functions
  protected:
    //{{AFX_MSG(CRuntimeProjTestDoc)
	afx_msg void OnUpdateReproject(CCmdUI* pCmdUI);
	afx_msg void OnReproject();
	afx_msg void OnOptionsBeepwhenalltilesloaded();
	afx_msg void OnUpdateOptionsBeepwhenalltilesloaded(CCmdUI* pCmdUI);
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
    
  private:

    // The data layer containing the map
    TSLMapDataLayer * m_mapDataLayer ;
    bool m_mapLoaded;

    // Can only use a blocking loader in this context.
    TSLFileLoaderBlocking *m_mapFileLoader;

    double m_lastGreatCircleDistanceSpecified;
    bool m_beepOnAllLoaded;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
