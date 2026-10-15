// importexportView.h : interface of the CImportExportView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MFCVIEW_H__CF7BD0CE_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_MFCVIEW_H__CF7BD0CE_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "viewattsdialog.h"

class CImportExportView : public CView
{
protected: // create from serialization only
	CImportExportView();
	DECLARE_DYNCREATE(CImportExportView)

// Attributes
public:
	CImportExportDoc* GetDocument();


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImportExportView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CImportExportView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CImportExportView)
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
	afx_msg void OnToolsViewAtts();
	afx_msg void OnUpdateToolsViewAtts(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
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

        // The view attribute dialog
        CViewAttsDialog*  m_viewAtts;
};

#ifndef _DEBUG  // debug version in importexportView.cpp
inline CImportExportDoc* CImportExportView::GetDocument()
   { return (CImportExportDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCVIEW_H__CF7BD0CE_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
