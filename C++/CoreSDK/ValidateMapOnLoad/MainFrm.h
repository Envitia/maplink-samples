// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__CF7BD0CA_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_MAINFRM_H__CF7BD0CA_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CRuntimeProjTestDoc;
class CRuntimeProjTestView;


class CMainFrame : public CFrameWnd
{
protected: // create from serialization only
  CMainFrame();
  DECLARE_DYNCREATE(CMainFrame)
    
    
    CRuntimeProjTestDoc* GetMyActiveDocument();
  CRuntimeProjTestView* GetMyActiveView();
  
  // Attributes
public:

  // Operations
public:
  void SetStatus( const CString& sInfo );
  
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CMainFrame)
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
  //}}AFX_VIRTUAL
  
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
  afx_msg LRESULT OnPWMCallback(WPARAM wparam, LPARAM lparam);
  afx_msg LRESULT OnAllLoadedCallback(WPARAM wparam, LPARAM lparam);
  afx_msg LRESULT OnProjectionChangedCallback(WPARAM wparam, LPARAM lparam);
  afx_msg LRESULT OnProjectionOriginChangedCallback(WPARAM wparam, LPARAM lparam);
  afx_msg LRESULT OnDrawingSurfaceNoCRCCallback(WPARAM wparam, LPARAM lparam);
  afx_msg LRESULT OnDrawingSurfaceCRCFailureCallback(WPARAM wparam, LPARAM lparam);
  afx_msg LRESULT OnMapNoCRCCallback(WPARAM wparam, LPARAM lparam);
  afx_msg LRESULT OnMapCRCFailureCallback(WPARAM wparam, LPARAM lparam);
  afx_msg LRESULT OnMapCanNotFindFileCallback(WPARAM wparam, LPARAM lparam);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__CF7BD0CA_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
