// app6addoView.h : interface of the CAPP6ADDOView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MFCVIEW_H__CF7BD0CE_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_MFCVIEW_H__CF7BD0CE_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CAPP6ADDOView : public CView
{
protected: // create from serialization only
	CAPP6ADDOView();
	DECLARE_DYNCREATE(CAPP6ADDOView)

// Attributes
public:
	CAPP6ADDODoc* GetDocument();
        bool isPrinting()
        {
          return m_printing ;
        }


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAPP6ADDOView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAPP6ADDOView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CAPP6ADDOView)
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
	//}}AFX_MSG
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

private:
	// Name of my map layer
	static const char * m_layerName ;

	// Name of my object layer.
	static const char* m_odlName ;

	// Handle to drawing surface
	TSLNTSurface * m_drawingSurface ;

	// Current interaction mode
	TSLViewMode * m_currentMode ;

	// Instance of OS specific interaction mode interface
	TSLViewModeInterfaceNT * m_modeInterface ;

	// Flag to say whether valid map is loaded.
	bool m_mapLoaded ;

	// Temporary storage when printing
	bool   m_printing ;
	double m_uux1 ;
	double m_uuy1 ;
	double m_uux2 ;
	double m_uuy2 ;
	TSLDeviceUnits   m_scrx1 ;
	TSLDeviceUnits   m_scry1 ;
	TSLDeviceUnits   m_scrx2 ;
	TSLDeviceUnits   m_scry2 ;

};

#ifndef _DEBUG  // debug version in ddoView.cpp
inline CAPP6ADDODoc* CAPP6ADDOView::GetDocument()
   { return (CAPP6ADDODoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCVIEW_H__CF7BD0CE_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
