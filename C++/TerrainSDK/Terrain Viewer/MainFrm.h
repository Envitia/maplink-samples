// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__CF7BD0CA_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_MAINFRM_H__CF7BD0CA_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "TerrainViewerOptions.h"

class CTerrainView;
class CTerrainViewerView;

class CMainFrame : public CFrameWnd
{
protected: // create from serialization only
  CMainFrame();
  DECLARE_DYNCREATE(CMainFrame)
    
    // Attributes
public:
  
  // Operations
public:
  CStatusBar* getStatusBar() { return &m_wndStatusBar; }
  
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CMainFrame)
public:
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
  virtual void ActivateFrame(int nCmdShow = -1);
protected:
  virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
  //}}AFX_VIRTUAL
  
  CSplitterWnd m_windowSplitter;
  
  
  // Implementation
public:
  virtual ~CMainFrame();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif
  
protected:  // control bar embedded members
  CStatusBar  m_wndStatusBar;
  CToolBar    m_wndToolBar;
  
  // Generated message map functions
protected:
  //{{AFX_MSG(CMainFrame)
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnToolsOptions();
	//}}AFX_MSG
  DECLARE_MESSAGE_MAP()
private:
  CTerrainView * m_currentTerrainView;
  CTerrainViewerView * m_currentMapView;
  TerrainViewerOptions m_options;

public:
  CTerrainView * getTerrainView() { return m_currentTerrainView ; }
  CTerrainViewerView * getCurrentMapView() { return m_currentMapView ; }
  TerrainViewerOptions& options()
  {
    return m_options;
  }
  static CMainFrame* instance()
  {
    return (CMainFrame*)AfxGetMainWnd();
  }

  virtual BOOL DestroyWindow();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__CF7BD0CA_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
