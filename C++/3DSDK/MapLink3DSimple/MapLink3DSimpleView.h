// MapLink3DSimpleView.h : interface of the MapLink3DSimpleView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MapLink3DSimpleVIEW_H__CF7BD0CE_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_MapLink3DSimpleVIEW_H__CF7BD0CE_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class TSL3DViewMode;
class TSL3DViewModeInterfaceNT;

class MapLink3DSimpleView : public CView
{
protected: // create from serialization only
  MapLink3DSimpleView();
  DECLARE_DYNCREATE(MapLink3DSimpleView)

  // Attributes
public:
  MapLink3DSimpleDoc* GetDocument();


  // Operations
public:

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(MapLink3DSimpleView)
public:
  virtual void OnDraw(CDC* pDC);  // overridden to draw this view
  virtual void OnInitialUpdate();
protected:
  //}}AFX_VIRTUAL

  // Implementation
public:
  virtual ~MapLink3DSimpleView();

#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

protected:

  // Generated message map functions
protected:
  //{{AFX_MSG(MapLink3DSimpleView)
  afx_msg void OnToolsReset();
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnUpdateToolsMode(CCmdUI* pCmdUI);
  afx_msg void OnUpdateToolsAction(CCmdUI* pCmdUI);
  afx_msg void OnToolsZoom();
  afx_msg void OnEditCopy();
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

  // Current interaction mode
  TSL3DViewMode * m_currentMode ;

  // Instance of OS specific interaction mode interface
  TSL3DViewModeInterfaceNT * m_modeInterface ;

  // data layer
  TSL3DStandardDataLayer *m_stdLayer;

  CString m_backdrop;

  BOOL m_lightingEnabled;

private:
  void setCameraOrientation();

public:
  static void renderingCallback(void * arg, int pendingTextures);

  afx_msg void OnToolsRotate();
  afx_msg void OnToolsFly();
  afx_msg void OnToolsCamera();

  afx_msg void OnUpdateLat(CCmdUI* pCmdUI);
  afx_msg void OnUpdateLon(CCmdUI* pCmdUI);
  afx_msg void OnUpdateAlt(CCmdUI* pCmdUI);

  afx_msg void OnEnableLighting();
  afx_msg void OnUpdateEnableLighting(CCmdUI *pCmdUI);

  afx_msg void OnCreatePolygon();
  afx_msg void OnCreateExtrudedPolygon();

  afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

};

#ifndef _DEBUG  // debug version in MapLink3DSimpleView.cpp
inline MapLink3DSimpleDoc* MapLink3DSimpleView::GetDocument()
{ return (MapLink3DSimpleDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MapLink3DSimpleVIEW_H__CF7BD0CE_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
