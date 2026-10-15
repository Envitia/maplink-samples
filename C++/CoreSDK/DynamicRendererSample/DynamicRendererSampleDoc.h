// DynamicRendererSampleDoc.h : interface of the DynamicRendererSampleDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DynamicRendererSampleDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_DynamicRendererSampleDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class DynamicRendererSampleDoc : public CDocument
{
  protected: // create from serialization only
    DynamicRendererSampleDoc();
    DECLARE_DYNCREATE(DynamicRendererSampleDoc)
    
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
  
  
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(DynamicRendererSampleDoc)
  public:
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
    virtual void DeleteContents();
    //}}AFX_VIRTUAL
  
    // Implementation
  public:
    virtual ~DynamicRendererSampleDoc();
  #ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
  #endif

    // Name of my map layer
    static const char * m_mapLayerName ;
  
  protected:
  
    // Generated message map functions
  protected:
    //{{AFX_MSG(DynamicRendererSampleDoc)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
    
  private:
  
    // The data layer containing the map
    TSLMapDataLayer * m_mapDataLayer ;
  
    virtual BOOL OnNewDocument();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DynamicRendererSampleDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
