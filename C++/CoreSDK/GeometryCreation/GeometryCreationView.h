// GeometryCreationView.h : interface of the CGeometryCreationView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GEOMETRYCREATIONVIEW_H__30D7A2C3_5129_481B_9032_7F0B8E931D9F__INCLUDED_)
#define AFX_GEOMETRYCREATIONVIEW_H__30D7A2C3_5129_481B_9032_7F0B8E931D9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGeometryCreationView : public CView
{
  protected: // create from serialization only
    CGeometryCreationView();
    DECLARE_DYNCREATE(CGeometryCreationView)
    
      // Attributes
  public:
    CGeometryCreationDoc* GetDocument();
  
    // Operations
  public:
     void updateEntityExtent( TSLEntity * entity, const char *layerName ) ;
 
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CGeometryCreationView)
	public:
    virtual void OnDraw(CDC* pDC);  // overridden to draw this view
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL
  
    // Implementation
  public:
    virtual ~CGeometryCreationView();
  #ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
  #endif
  
  protected:
  
    // Generated message map functions
  protected:
    //{{AFX_MSG(CGeometryCreationView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnToolsGrab();
	afx_msg void OnToolsZoomin();
	afx_msg void OnToolsReset();
	afx_msg void OnToolsZoomout();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnToolsPan();
	afx_msg void OnToolsZoom();
	afx_msg void OnToolsZoompan();
	afx_msg void OnToolsEditproperties();
	afx_msg void OnTestsShowentityextents();
	afx_msg void OnUpdateTestsShowentityextents(CCmdUI* pCmdUI);
	afx_msg void OnTestsUniqueidforeachsurface();
	afx_msg void OnUpdateTestsUniqueidforeachsurface(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnUpdateToolsMode(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolsAction(CCmdUI* pCmdUI);
    DECLARE_MESSAGE_MAP()
    
  private:
    // Handle to drawing surface
    TSLNTSurface * m_drawingSurface ;
    static bool m_showExtents ;
    static bool m_uniqueID ;

    static TSLViewModeInterfaceNT m_modeInterface ;
    static TSLViewMode * m_currentMode ;

    void redraw( ) ;
};

#ifndef _DEBUG  // debug version in GeometryCreationView.cpp
inline CGeometryCreationDoc* CGeometryCreationView::GetDocument()
{ return (CGeometryCreationDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GEOMETRYCREATIONVIEW_H__30D7A2C3_5129_481B_9032_7F0B8E931D9F__INCLUDED_)
