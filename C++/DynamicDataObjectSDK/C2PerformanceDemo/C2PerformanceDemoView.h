// C2PerformanceDemoView.h : interface of the CC2PerformanceDemoView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MFCVIEW_H__CF7BD0CE_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_MFCVIEW_H__CF7BD0CE_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class TSLNTSurface;
class TSLViewMode;
class TSLViewModeInterfaceNT;
class CC2PerformanceDemoDoc;

class CC2PerformanceDemoView : public CView
{
protected: // create from serialization only
	CC2PerformanceDemoView();
	DECLARE_DYNCREATE(CC2PerformanceDemoView)

// Attributes
public:
	CC2PerformanceDemoDoc * GetDocument();
	TSLNTSurface * getDrawingSurface() { return m_drawingSurface ; } ;
	TSLViewModeInterfaceNT * getViewModeInterface() { return m_modeInterface ; } ;

	// Set the current view mode
	void setViewMode( UINT viewMode ) ;
	
	// Get the current view mode
	long getViewMode() ;

	// Is the map loaded ?
	bool mapLoaded() ;

        bool startPerformanceTest ();
        bool stopPerformanceTest ();
        bool isPerformanceTestRunning ();
        double getCurrentFrameRate ();
        double getAverageFrameRate ();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CC2PerformanceDemoView)
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
	virtual ~CC2PerformanceDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CC2PerformanceDemoView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	
	DECLARE_MESSAGE_MAP()

private:
  void setLatLonLayerRendering( TSLDataLayer& dLayer, bool tenths );

	// Name of my map layer
	static const char * m_mapLayerName ;
	
	// Name of my standard data layer
	static const char * m_standardLayerName ;

	// Name of my object data layer
  static const char * m_objectLayerName ;

  // Name of my grid data layer
  static const char * m_gridLayerName;

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

	// Cursor types to show which interaction mode the app is running in
	HCURSOR m_zoomCursor ;
	HCURSOR m_panCursor ;
	HCURSOR m_selectCursor ;
	HCURSOR m_dragCursor ;
	HCURSOR m_grabCursor ;
	HCURSOR m_grabbingCursor ;

        bool m_timingStarted;
        long long m_timingStart;
        long long m_timingStop;
        long long m_timingLastUpdate;
        long m_timingLastCount;
        long m_timingCount;

public:
  afx_msg void OnViewLatLonGrid();
  afx_msg void OnUpdateViewLatLon(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // debug version in CC2PerformanceDemoView.cpp
inline CC2PerformanceDemoDoc* CC2PerformanceDemoView::GetDocument()
   { return (CC2PerformanceDemoDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCVIEW_H__CF7BD0CE_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
