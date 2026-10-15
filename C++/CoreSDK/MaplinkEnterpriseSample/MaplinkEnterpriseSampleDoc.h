// MaplinkEnterpriseSampleDoc.h : interface of the CMaplinkEnterpriseSampleDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MFCDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_MFCDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CMaplinkEnterpriseSampleDoc : public CDocument
{
  protected: // create from serialization only
    CMaplinkEnterpriseSampleDoc();
    DECLARE_DYNCREATE(CMaplinkEnterpriseSampleDoc)
    
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

    void setSynchronousLoading(bool synchronous);
  
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMaplinkEnterpriseSampleDoc)
  public:
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void DeleteContents();
    //}}AFX_VIRTUAL
  
    // Implementation
  public:
    virtual ~CMaplinkEnterpriseSampleDoc();
  #ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
  #endif
  
  protected:
    void cleanup();

    // callback function - which via a bit of jiggerypokery causes  
    // SuccessfulCallbackAction to be called in the main thread if status = success and 100% done.
    static TSLLoaderCallbackReturn mapCallbackFunction( void* arg, const char* filename, TSLEnvelope extent, TSLLoaderStatus status, int percentDone );
    static void mapCallbackFunctionAllLoaded( void* arg );

    // check date.dat in map directory to see if persistent cache is out of date
    void checkForCacheClear( const char* cacheDir, TSLFileLoader* loader );
  
    // Generated message map functions
  protected:
    //{{AFX_MSG(CMaplinkEnterpriseSampleDoc)
      // NOTE - the ClassWizard will add and remove member functions here.
      //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
    
  private:

    // The data layer containing the map
    TSLMapDataLayer * m_mapDataLayer ;
public:
  virtual void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU = TRUE);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
