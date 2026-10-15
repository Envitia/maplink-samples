// TerrainViewerView.h : interface of the CTerrainViewerView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MFCVIEW_H__CF7BD0CE_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_MFCVIEW_H__CF7BD0CE_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "TerrainInfoDlg.h"
#include "tslviewmode.h"

class TSLViewModeInterfaceNT;
class CTerrainViewerDoc;

class CTerrainViewerView : public CView
{

protected: // create from serialization only

  CTerrainViewerView();

  DECLARE_DYNCREATE(CTerrainViewerView)

public:

  CTerrainViewerDoc* GetDocument();

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CTerrainViewerView)
public:
  virtual void OnDraw(CDC* pDC);  // overridden to draw this view
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
  virtual void OnInitialUpdate();

  void resetMode() ;

protected:
  virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
  virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
  virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
  virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
  virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
  //}}AFX_VIRTUAL

public:
  virtual ~CTerrainViewerView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  TSLNTSurface * GetDrawingSurface() { return  m_drawingSurface ;}

  bool isAreaLOSViewActive() { return (m_currentMode && (m_currentMode->id() == ID_TOOLS_LOSAREA));}
protected:

  // Generated message map functions
protected:
  //{{AFX_MSG(CTerrainViewerView)
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
  afx_msg void OnToolsTerrain();
  afx_msg void OnToolsAreagraph();
  afx_msg void OnViewTerrainExtent();
  afx_msg void OnUpdateViewTerrainExtent(CCmdUI* pCmdUI);
  afx_msg void OnToolsLos();
  afx_msg void OnViewTerraininfo();
  afx_msg void OnUpdateViewTerraininfo(CCmdUI* pCmdUI);
	afx_msg void OnToolsLosarea();
	afx_msg void OnUpdateToolsLosarea(CCmdUI* pCmdUI);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
  afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
  afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
  DECLARE_MESSAGE_MAP()

  void updateExtentRect();

private:
  // Name of my map layer
  static const char * m_layerName ;

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
  bool   m_viewTerrainExtent;

  CTerrainInfoDlg	  m_info;

};

#ifndef _DEBUG  // debug version in TerrainViewerView.cpp
inline CTerrainViewerDoc* CTerrainViewerView::GetDocument()
{ return (CTerrainViewerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCVIEW_H__CF7BD0CE_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
