// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__CF7BD0CA_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_MAINFRM_H__CF7BD0CA_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CC2DemoView;
class CC2DemoPanelView;
class CC2DemoToteView;
class CC2DemoTreeView;

#include "SplitterWndEx.h"

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
	CC2DemoView	 * getMainView() ;
	CC2DemoPanelView * getPanelView() ;
	CC2DemoToteView	 * getToteView() ;
	CC2DemoTreeView	 * getTreeView() ;

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
	CSplitterWndEx	    m_windowSplitterMain ;    // The main window splitter
	CSplitterWndEx	    m_windowSplitterDisplay ; // Nested inside the main window splitter

	// Pointers to the views
	CC2DemoView	  * m_mainView ;
	CC2DemoPanelView  * m_panelView ;
	CC2DemoToteView	  * m_toteView ;
	CC2DemoTreeView	  * m_treeView ;

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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__CF7BD0CA_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
