// AcceleratedSampleView.h : interface of the AcceleratedSampleView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_AcceleratedSampleVIEW_H__CF7BD0CE_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_AcceleratedSampleVIEW_H__CF7BD0CE_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class AcceleratedSampleView : public CView, TSLInteractionModeRequest
{
protected: // create from serialization only
  AcceleratedSampleView();
  DECLARE_DYNCREATE(AcceleratedSampleView)

  // Attributes
public:
  AcceleratedSampleDoc* GetDocument();


  //http://support.microsoft.com/kb/108587
  static AcceleratedSampleView * GetView();

  void stopBackgroundThread();
  void resumeBackgroundThread();
  void detachLayers();
  void redraw();

  // Operations
public:

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(AcceleratedSampleView)
public:
  virtual void OnDraw(CDC* pDC);  // overridden to draw this view
  virtual void OnInitialUpdate();
protected:
  virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
  virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
  //}}AFX_VIRTUAL

  // Implementation
public:
  virtual ~AcceleratedSampleView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

protected:

  // Generated message map functions
protected:
  //{{AFX_MSG(AcceleratedSampleView)
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
  afx_msg void OnToolsRendererUpdate(CCmdUI* pCmdUI);
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
  afx_msg void OnTimer(UINT_PTR nIDEvent);
  afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnToolsStopthread();
  afx_msg void OnToolsResumethread();
  afx_msg void OnToolsSwaprenderers();
  afx_msg void OnToolsThreadedrenderer();
  afx_msg void OnToolsBlockingrenderer();
  afx_msg void OnToolsTogglenearest();
  afx_msg void OnToolsToggleborder();
  afx_msg void OnLayerRasterbuffering();
  afx_msg void OnLayerBackgroundtilesopaque();
  afx_msg void OnLayerBacklayervisability();
  afx_msg void OnLayerFrontlayervisability();
  DECLARE_MESSAGE_MAP()

  void resetMode(TSLInteractionMode * mode, TSLButtonType button, TSLDeviceUnits xDU, TSLDeviceUnits yDU);
  void viewChanged(TSLDrawingSurface* drawingSurface);

private:
  // Handle to drawing surface
  TSLWGLAcceleratedSurface * m_drawingSurface ;

  // Surface Configuraton
  TSLAcceleratorConfiguration m_configuration;

  // Rendering control to use.
  bool m_useThreadedRenderer;

  TSLAcceleratedMTRenderControl *m_mtRenderControl;

  bool m_threadStopped;

  TSLAcceleratedBlockingRenderControl *m_blockingRenderControl;

  // Interaction manager
  TSLInteractionModeManagerNT * m_modeManager ;

  // Flag to say whether valid map is loaded.
  bool m_mapLoaded ;

  TSLEnvelope m_tmcExtent;

  // keypress information
  bool	  m_upPressed;
  bool	  m_downPressed;
  bool	  m_leftPressed;
  bool	  m_rightPressed;
  bool	  m_zPressed;
  bool	  m_xPressed;

  // current surface rotation.
  double	  m_rotate;

  bool m_toggleNearest;
  bool m_toggleBorder;
  bool m_rasterBuffering;
  bool m_backgroundOpaque;

};

#ifndef _DEBUG  // debug version in AcceleratedSampleView.cpp
inline AcceleratedSampleDoc* AcceleratedSampleView::GetDocument()
{ return (AcceleratedSampleDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AcceleratedSampleVIEW_H__CF7BD0CE_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
