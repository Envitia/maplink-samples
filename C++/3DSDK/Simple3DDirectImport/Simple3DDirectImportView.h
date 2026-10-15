////////////////////////////////////////////////////////
// Copyright (c) 1998 to 2017 by Envitia Group PLC
////////////////////////////////////////////////////////
// Simple3DDirectImportView.h : interface of the Simple3DDirectImportView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_Simple3DDirectImportVIEW_H__CF7BD0CE_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_Simple3DDirectImportVIEW_H__CF7BD0CE_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class Simple3DDirectImportDoc ;

class Simple3DDirectImportView : public CView, TSL3DInteractionModeRequest
{
protected: // create from serialization only
  Simple3DDirectImportView();
  DECLARE_DYNCREATE(Simple3DDirectImportView)

  // Attributes
public:
  Simple3DDirectImportDoc* GetDocument();
  const char * Simple3DDirectImportView::queryPrompt( ) ;

  static void renderingCallback(void * arg, int pendingTextures);


  // Operations
public:

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(Simple3DDirectImportView)
public:
  virtual void OnDraw(CDC* pDC);  // overridden to draw this view
  virtual void OnInitialUpdate();
protected:
  //}}AFX_VIRTUAL

  // Implementation
public:
  virtual ~Simple3DDirectImportView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

protected:

  // Generated message map functions
protected:
  //{{AFX_MSG(Simple3DDirectImportView)
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
  afx_msg void OnToolsTrackballEyepoint();
  afx_msg void OnToolsTrackballWorld();
  afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
  afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
  afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  //}}AFX_MSG
  afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
  afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
  DECLARE_MESSAGE_MAP()

private:
  // Handle to drawing surface
  TSL3DWinGLSurface * m_drawingSurface ;

  // Interaction manager
  TSL3DInteractionModeManagerNT * m_modeManager ;

  CString m_backdrop;

  bool m_exaggerateTerrain;
  bool m_limitCameraHeight;


public:
  void resetMode(TSL3DInteractionMode * mode, TSLButtonType button, TSLDeviceUnits xDU, TSLDeviceUnits yDU);
  void viewChanged(TSL3DDrawingSurface* drawingSurface);
  afx_msg void OnUpdateLat(CCmdUI* pCmdUI);
  afx_msg void OnUpdateLon(CCmdUI* pCmdUI);
  afx_msg void OnUpdateAlt(CCmdUI* pCmdUI);
  afx_msg void OnSavedviewsGoto1();
  afx_msg void OnSavedviewsGoto2();
  afx_msg void OnSavedviewsGoto3();
  afx_msg void OnSavedviewsReset();
  afx_msg void OnSavedviewsSet1();
  afx_msg void OnSavedviewsSet2();
  afx_msg void OnSavedviewsSet3();
  afx_msg void OnUpdateSavedviewsReset(CCmdUI *pCmdUI);
  afx_msg void OnUpdateSavedviewsGoto3(CCmdUI *pCmdUI);
  afx_msg void OnUpdateSavedviewsGoto2(CCmdUI *pCmdUI);
  afx_msg void OnUpdateSavedviewsGoto1(CCmdUI *pCmdUI);
  afx_msg void OnViewstackNextview();
  afx_msg void OnUpdateViewstackNextview(CCmdUI *pCmdUI);
  afx_msg void OnViewstackPreviousview();
  afx_msg void OnUpdateViewstackPreviousview(CCmdUI *pCmdUI);
  afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
  afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnToolsExaggerateterrain();
  afx_msg void OnToolsWireframe();
  afx_msg void OnToolsLimitcamera();
};

#ifndef _DEBUG  // debug version in Simple3DDirectImportView.cpp
inline Simple3DDirectImportDoc* Simple3DDirectImportView::GetDocument()
{ return (Simple3DDirectImportDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_Simple3DDirectImportVIEW_H__CF7BD0CE_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
