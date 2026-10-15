// MaplinkEnterpriseSampleView.h : interface of the CMaplinkEnterpriseSampleView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MFCVIEW_H__CF7BD0CE_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_MFCVIEW_H__CF7BD0CE_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000



class TSLViewMode;
class TSLViewModeInterfaceNT;



class CMaplinkEnterpriseSampleView : public CView
{
protected: // create from serialization only
  CMaplinkEnterpriseSampleView();
  DECLARE_DYNCREATE(CMaplinkEnterpriseSampleView)
    
    // Attributes
public:
  
  // Operations
public:
  // return pointer to the active document
  CMaplinkEnterpriseSampleDoc* GetDocument();
  
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CMaplinkEnterpriseSampleView)
	public:
  virtual void OnDraw(CDC* pDC);	// overridden to draw this view
  virtual void OnInitialUpdate();
	protected:
  virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
  virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
  virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL
  
  // Implementation
public:
  virtual ~CMaplinkEnterpriseSampleView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif
  
protected:
  
private:
  
  
  // Generated message map functions
protected:
  //{{AFX_MSG(CMaplinkEnterpriseSampleView)
  afx_msg void OnToolsZoomin();
  afx_msg void OnToolsZoomout();
  afx_msg void OnToolsReset();
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnUpdateToolsMode(CCmdUI* pCmdUI);
  afx_msg void OnUpdateToolsAction(CCmdUI* pCmdUI);
  afx_msg void OnToolsPan();
  afx_msg void OnToolsZoom();
  afx_msg void OnToolsZoompan();
  afx_msg void OnEditCopy();
  afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
  afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
  afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  //}}AFX_MSG
  afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
  afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
  afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
  DECLARE_MESSAGE_MAP()
    
private:
 
  // Handle to drawing surface
  TSLNTSurface * m_drawingSurface ;
  
  // Current interaction mode
  TSLViewMode * m_currentMode ;
  
  // Instance of OS specific interaction mode interface
  TSLViewModeInterfaceNT * m_modeInterface ;
  
  // Flag to say whether valid map is loaded.
  bool m_mapLoaded ;
};

#ifndef _DEBUG	// debug version in MaplinkEnterpriseSampleView.cpp
inline CMaplinkEnterpriseSampleDoc* CMaplinkEnterpriseSampleView::GetDocument()
  { return (CMaplinkEnterpriseSampleDoc*)m_pDocument; }
#endif



/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCVIEW_H__CF7BD0CE_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
