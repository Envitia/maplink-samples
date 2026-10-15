// DirectImportSimpleViewerView.h : interface of the DirectImportSimpleViewerView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DirectImportSimpleViewerVIEW_H__CF7BD0CE_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_DirectImportSimpleViewerVIEW_H__CF7BD0CE_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class DirectImportSimpleViewerView : public CView, TSLInteractionModeRequest
{
protected: // create from serialization only
	DirectImportSimpleViewerView();
	DECLARE_DYNCREATE(DirectImportSimpleViewerView)

// Attributes
public:
	DirectImportSimpleViewerDoc* GetDocument();

  
      static DirectImportSimpleViewerView * GetView();
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DirectImportSimpleViewerView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~DirectImportSimpleViewerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

  TSLDrawingSurface* drawingSurface() const;

  std::string m_position;

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(DirectImportSimpleViewerView)
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
	afx_msg void OnEditCopy();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnToolsGrab();
	afx_msg void OnFilePrint();
	afx_msg void OnFilePrintPreview();
	//}}AFX_MSG
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

private:
	// Handle to drawing surface
	TSLNTSurface * m_drawingSurface ;

	// Interaction manager
	TSLInteractionModeManagerNT * m_modeManager ;

	// Flag to say whether valid map is loaded.
	bool m_mapLoaded ;

        // Stored extent to print, captured in OnFilePrint and OnFilePrintPreview
        TSLEnvelope  m_extentToPrint ;
public:
  afx_msg void OnToolsMagnify();
  void resetMode(TSLInteractionMode * mode, TSLButtonType button, long xDU, long yDU);
  void viewChanged(TSLDrawingSurface* drawingSurface);
  afx_msg void OnSavedviewsGoto1();
  afx_msg void OnSavedviewsGoto2();
  afx_msg void OnSavedviewsGoto3();
  afx_msg void OnSavedviewsReset();
  afx_msg void OnSavedviewsSet1();
  afx_msg void OnUpdateSavedviewsReset(CCmdUI *pCmdUI);
  afx_msg void OnUpdateSavedviewsGoto3(CCmdUI *pCmdUI);
  afx_msg void OnUpdateSavedviewsGoto2(CCmdUI *pCmdUI);
  afx_msg void OnUpdateSavedviewsGoto1(CCmdUI *pCmdUI);
  afx_msg void OnUpdateSavedviewsSet1(CCmdUI *pCmdUI);
  afx_msg void OnSavedviewsSet2();
  afx_msg void OnUpdateSavedviewsSet2(CCmdUI *pCmdUI);
  afx_msg void OnSavedviewsSet3();
  afx_msg void OnUpdateSavedviewsSet3(CCmdUI *pCmdUI);
  afx_msg void OnViewstackNextview();
  afx_msg void OnUpdateViewstackNextview(CCmdUI *pCmdUI);
  afx_msg void OnViewstackPreviousview();
  afx_msg void OnUpdateViewstackPreviousview(CCmdUI *pCmdUI);
  afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};

#ifndef _DEBUG  // debug version in DirectImportSimpleViewerView.cpp
inline DirectImportSimpleViewerDoc* DirectImportSimpleViewerView::GetDocument()
   { return (DirectImportSimpleViewerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DirectImportSimpleViewerVIEW_H__CF7BD0CE_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
