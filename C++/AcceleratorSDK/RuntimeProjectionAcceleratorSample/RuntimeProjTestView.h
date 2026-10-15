// RuntimeProjTestView.h : interface of the CRuntimeProjTestView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MFCVIEW_H__CF7BD0CE_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_MFCVIEW_H__CF7BD0CE_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000



class TSLViewMode;
class TSLViewModeInterfaceNT;
struct OriginChangeInfo;


class CRuntimeProjTestView : public CView
{
protected: // create from serialization only
  CRuntimeProjTestView();
  DECLARE_DYNCREATE(CRuntimeProjTestView)
    
    // Attributes
public:
  
  // Operations
public:
  // return pointer to the active document
  CRuntimeProjTestDoc* GetDocument();
  
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CRuntimeProjTestView)
public:
  virtual void OnDraw(CDC* pDC);	// overridden to draw this view
  virtual void OnInitialUpdate();
protected:
  virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
  virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
  virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
  //}}AFX_VIRTUAL
  
public:
  bool getViewedLatLongRange( double* lattd, double* longtd, double* range ) const;
  bool setViewedLatLongRange( double lattd, double longtd, double range );
  
  
  // called in main thread as a result of message from projection origin changed callback
  void projectionOriginChangedCallbackAction( OriginChangeInfo* info  ); 
  
    void stopBackgroundThread();
  void resumeBackgroundThread();

  // Implementation
public:
  virtual ~CRuntimeProjTestView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif
  
protected:
  
private:
  
  
  // Generated message map functions
protected:
  //{{AFX_MSG(CRuntimeProjTestView)
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
  afx_msg void OnSetprojcentre();
  //}}AFX_MSG
  afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
  afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnTimer(UINT_PTR nIDEvent);
  DECLARE_MESSAGE_MAP()
    
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

  // Current interaction mode
  TSLViewMode * m_currentMode ;
  
  // Instance of OS specific interaction mode interface
  TSLViewModeInterfaceNT * m_modeInterface ;

  bool m_mapLoaded ;
};

#ifndef _DEBUG	// debug version in RuntimeProjTestView.cpp
inline CRuntimeProjTestDoc* CRuntimeProjTestView::GetDocument()
{ return (CRuntimeProjTestDoc*)m_pDocument; }
#endif



/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCVIEW_H__CF7BD0CE_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
