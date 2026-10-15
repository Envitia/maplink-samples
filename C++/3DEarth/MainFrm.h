
// MainFrm.h : interface of the CMainFrame class
//

#pragma once

class ScreenshotDialog;
#include <memory>

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
  virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
  
  // Handler methods for application messages
  afx_msg LRESULT OnSizing(WPARAM fwSide, LPARAM pRect);

  void OnTrackballMove();

  void CMainFrame::setStatusText(const std::string& sInfo);

  // Sends the given surface pointer to the Camera form, so that it can modify the surface's camera.
  void attachCameraUIToSurface(earth::Surface3D* surface);

  CView * getEarthView();
  CView * getCameraControlFormView();

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CStatusBar        m_wndStatusBar;
  CSplitterWnd      m_splitterWnd;
  bool              m_splitterWndInitialised = false;

  std::unique_ptr<ScreenshotDialog> m_screenshotDialog;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFileClose();
	afx_msg void OnScreenshot();

	DECLARE_MESSAGE_MAP()

public:
  afx_msg void OnGeometryPolygon();
  afx_msg void OnGeometryPolyline();
  afx_msg void OnGeometryText();
  afx_msg void OnGeometrySymbol();
  afx_msg void OnInteractionmodeExtrudedpolygon();
  afx_msg void OnInteractionmodeExtrudedpolyline();
  afx_msg void OnInteractionmodeDeletegeometry();
  afx_msg void OnInteractionmodeSelectgeometry();
  afx_msg void OnInteractionmodeTrackball();
  afx_msg void OnResetView();
};


