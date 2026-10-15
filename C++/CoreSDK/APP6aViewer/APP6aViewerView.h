// App6aViewerView.h : interface of the CApp6aViewerView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MFCVIEW_H__CF7BD0CE_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_MFCVIEW_H__CF7BD0CE_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CApp6aViewerView : public CView
{
protected: // create from serialization only
	CApp6aViewerView();
	DECLARE_DYNCREATE(CApp6aViewerView)

// Attributes
public:
        int checked_count;
	CApp6aViewerDoc* GetDocument();


// Operations
public:
    


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CApp6aViewerView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	void OnRefresh();
	void drawSymbols();
	void OnButtonDown();
	void OnButtonUp();

	virtual ~CApp6aViewerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CApp6aViewerView)
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
        TSLStandardDataLayer * m_stdDataLayer ;
	TSLNTSurface * m_drawingSurface ;

	// Current interaction mode
	TSLViewMode * m_currentMode ;

	// Instance of OS specific interaction mode interface
	TSLViewModeInterfaceNT * m_modeInterface ;

	// Flag to say whether valid map is loaded.
	bool m_mapLoaded ;

        // Stored extent to print, captured in OnFilePrint and OnFilePrintPreview
        TSLEnvelope  m_extentToPrint ;
};

#ifndef _DEBUG  // debug version in App6aViewerView.cpp
inline CApp6aViewerDoc* CApp6aViewerView::GetDocument()
   { return (CApp6aViewerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCVIEW_H__CF7BD0CE_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
