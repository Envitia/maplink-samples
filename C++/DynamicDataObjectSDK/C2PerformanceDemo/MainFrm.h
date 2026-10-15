// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__CF7BD0CA_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_MAINFRM_H__CF7BD0CA_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "performancetoolbar.h"
#include "SplitterWndEx.h"

class CC2PerformanceDemoView;
class CC2PerformanceDemoPanelView;
class CC2PerformanceDemoToteView;
class CC2PerformanceDemoTreeView;
        
class CMainFrame : public CFrameWnd
{
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

	// Return the size taken up by menu, toolbar and caption bar
	// ( Used when dragging tracks onto the Tote. See TSLViewModeSelect::OnLButtonUp() )
	int getGUIDisplacement() ;

	// Return pointers to the views
	CC2PerformanceDemoView	 * getMainView() ;
	CC2PerformanceDemoPanelView * getPanelView() ;
	CC2PerformanceDemoToteView	 * getToteView() ;
	CC2PerformanceDemoTreeView	 * getTreeView() ;
        CStatusBar*                         getStatusBar();
        PerformanceToolBar* getPerformanceBar();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar	    m_wndStatusBar ;
	CToolBar	    m_wndToolBar ;
        PerformanceToolBar  m_wndPerformanceToolBar ;
	CSplitterWndEx	    m_windowSplitterMain ;    // The main window splitter
	CSplitterWndEx	    m_windowSplitterDisplay ; // Nested inside the main window splitter

	// Pointers to the views
	CC2PerformanceDemoView	  * m_mainView ;
	CC2PerformanceDemoPanelView  * m_panelView ;
	CC2PerformanceDemoToteView	  * m_toteView ;
	CC2PerformanceDemoTreeView	  * m_treeView ;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewRangerings();
	afx_msg void OnRange100km();
	afx_msg void OnRange10km();
	afx_msg void OnRange200km();
	afx_msg void OnRange500km();
	afx_msg void OnRange50km();
	afx_msg void OnToolsNorthup();
	afx_msg void OnToolsTrackheadingup();
	afx_msg void OnToolsFollowselected();
	afx_msg void OnHelpContact();
	afx_msg void OnUpdateRange100km(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRange10km(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRange200km(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRange500km(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRange50km(CCmdUI* pCmdUI);
	afx_msg void OnSimulation10speed();
	afx_msg void OnUpdateSimulation10speed(CCmdUI* pCmdUI);
	afx_msg void OnSimulation2speed();
	afx_msg void OnUpdateSimulation2speed(CCmdUI* pCmdUI);
	afx_msg void OnSimulation50speed();
	afx_msg void OnUpdateSimulation50speed(CCmdUI* pCmdUI);
	afx_msg void OnSimulation5speed();
	afx_msg void OnUpdateSimulation5speed(CCmdUI* pCmdUI);
	afx_msg void OnSimulationRealtime();
	afx_msg void OnUpdateSimulationRealtime(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolsFollowselected(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolsNorthup(CCmdUI* pCmdUI);
	afx_msg void OnToolsPan();
	afx_msg void OnUpdateToolsPan(CCmdUI* pCmdUI);
	afx_msg void OnToolsReset();
	afx_msg void OnUpdateToolsReset(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolsTrackheadingup(CCmdUI* pCmdUI);
	afx_msg void OnToolsTrackselect();
	afx_msg void OnUpdateToolsTrackselect(CCmdUI* pCmdUI);
	afx_msg void OnToolsZoom();
	afx_msg void OnUpdateToolsZoom(CCmdUI* pCmdUI);
	afx_msg void OnToolsZoomin();
	afx_msg void OnUpdateToolsZoomin(CCmdUI* pCmdUI);
	afx_msg void OnToolsZoomout();
	afx_msg void OnUpdateToolsZoomout(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewRangerings(CCmdUI* pCmdUI);
	afx_msg void OnViewStatusBar();
	afx_msg void OnUpdateViewStatusBar(CCmdUI* pCmdUI);
	afx_msg void OnViewToolbar();
	afx_msg void OnUpdateViewToolbar(CCmdUI* pCmdUI);
	afx_msg void OnAppHelp();
	afx_msg void OnUpdateAppHelp(CCmdUI* pCmdUI);
	afx_msg void OnToolsGrab();
  afx_msg void OnUpdateToolsGrab(CCmdUI* pCmdUI);
  afx_msg void OnUpdateSymbolType(CCmdUI *pCmdUI);
  afx_msg void OnSymbolsApp6a();
  afx_msg void OnSymbols2525B();
  afx_msg void OnSymbolsStandard();
  afx_msg void OnPerformanceStart();
  afx_msg void OnPerformanceStop();
  afx_msg void OnUpdatePerformance(CCmdUI *pCmdUI);
  afx_msg void OnSymbolsChangethenumberoftracks();
  afx_msg void OnUpdateApp6aAnnotationlevel(CCmdUI *pCmdUI);
  afx_msg void OnApp6aAnnotationlevelNone();
  afx_msg void OnApp6aAnnotationlevelLow();
  afx_msg void OnApp6aAnnotationlevelMedium();
  afx_msg void OnApp6aAnnotationlevelHigh();
  afx_msg void OnUpdateApp6aRendering(CCmdUI *pCmdUI);
  afx_msg void OnApp6aRenderingBitmap();
  afx_msg void OnApp6aRenderingEntity();
  afx_msg void OnDoNothing();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__CF7BD0CA_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
