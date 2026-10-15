// App6aViewerDoc.h : interface of the CApp6aViewerDoc class
//
/////////////////////////////////////////////////////////////////////////////

#define APP6A_CONFIG_FILE_NAME "app6a_symbol_config.txt"


#if !defined(AFX_MFCDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_MFCDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include "SymbolTreeView.h"
#include "TSLApp6aSymbolHelper.h"

class CApp6aViewerDoc : public CDocument
{
  protected: // create from serialization only
    CApp6aViewerDoc();
    DECLARE_DYNCREATE(CApp6aViewerDoc)
    
      // Attributes
  public:
    CSymbolTreeView * m_treeView ;
    TSLApp6aSymbolHelper * m_symbolHelper;
    // Operations
  public:
    //This method adds the set of data layers contained 
    //within the document to the specified drawing surface.
  
    bool addToSurface( TSLDrawingSurface * surface );
  
    //This method removes the current set of data layers from 
    //the specified drawing surface.
    bool detachFromSurface( TSLDrawingSurface * surface );
  
    void CApp6aViewerDoc::UpdateMainView() ;

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CApp6aViewerDoc)
  public:
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
    virtual void DeleteContents();
    //}}AFX_VIRTUAL
  
    // Implementation
  public:
    virtual ~CApp6aViewerDoc();
  #ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
  #endif

    
  
  protected:
  
    // Generated message map functions
  protected:
    //{{AFX_MSG(CApp6aViewerDoc)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
    
  private:
  
    // The data layer containing the map
    TSLStandardDataLayer * m_stdDataLayer ;
  
    // Name of my map layer
    static const char * m_stdLayerName ;
  
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
