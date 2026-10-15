// FlashbackDoc.h : interface of the FlashbackDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FlashbackDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_FlashbackDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class FlashbackDoc : public CDocument
{
  protected: // create from serialization only
    FlashbackDoc();
    DECLARE_DYNCREATE(FlashbackDoc)
    
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

    // This invokes the flashback.
    void flashback(TSLHistoryVersion version);

    void getArchiveRoot(const char* layerName, TSLHistoryVersion version, char* directory);

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(FlashbackDoc)
  public:
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
    virtual void DeleteContents();
    //}}AFX_VIRTUAL
  
    // Implementation
  public:
    virtual ~FlashbackDoc();
  #ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
  #endif
  
  protected:
  
    // Generated message map functions
  protected:
    //{{AFX_MSG(FlashbackDoc)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
    
  private:
  
    // The data layer containing the map
    TSLMapDataLayer * m_mapDataLayer ;
  
    // Name of my map layer
    static const char * m_mapLayerName ;

    std::string m_archiveRoot;      // The root directory for the map's archives.
    bool        m_archiveInSubdir;  // Indicates whether the archives are kept in subdirectories
                                    // wrt m_archiveRoot.
  
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FlashbackDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
