// app6addodoc.h : interface of the CAPP6ADDODoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MFCDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_MFCDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CAPP6ADDODoc : public CDocument
{
  protected: // create from serialization only
    CAPP6ADDODoc();
    DECLARE_DYNCREATE(CAPP6ADDODoc)
    
      // Attributes
  public:
  
    // Operations
  public:
    //
    // Query the data layer that has been loaded into the document.
    TSLMapDataLayer *getDataLayer()
    { 
      return m_dataLayer ; 
    }
  
    // Query the object layer.
    TSLObjectDataLayer *getObjectLayer() const
    { 
      return m_objectLayer;
    }
  
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAPP6ADDODoc)
	public:
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void DeleteContents();
	//}}AFX_VIRTUAL
  
    // Implementation
  public:
    virtual ~CAPP6ADDODoc();
  #ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
  #endif
  
    static long m_reset_timer;	  // true when m_ddo_timer_dt needs to be reset
    static long m_ddo_timer_dt;   // time between timer calls

  protected:
  
    // Generated message map functions
  protected:
    //{{AFX_MSG(CAPP6ADDODoc)
      // NOTE - the ClassWizard will add and remove member functions here.
      //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
    
    // This drives the simulation.
    static VOID CALLBACK OnUpdate( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime );

  private:

    // The data layer containing the 
    TSLMapDataLayer * m_dataLayer ;

    // The object layer.
    TSLObjectDataLayer* m_objectLayer;

    UINT_PTR		m_ddoTimerID;	// A timer for animation.
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
