// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#if !defined(AFX_MAINFRM_H__CF7BD0CA_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_MAINFRM_H__CF7BD0CA_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#include "customtoolbar.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include <string>

extern void addLine(std::string &line);

class CMainFrame : public CFrameWnd
{
protected: // create from serialization only
  CMainFrame();
  DECLARE_DYNCREATE(CMainFrame)

  void attachToolbarImages( CToolBarCtrl& toolbar, int numImages, UINT inNormalImageID, UINT inDisabledImageID, UINT inHotImageID ) ;


  // Attributes
public:

  // Operations
public:
  void resizeSplitter();

  CStatusBar* getStatusBar() { return &m_wndStatusBar; }

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CMainFrame)
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
  //}}AFX_VIRTUAL

  // Implementation
public:
  virtual ~CMainFrame();
  CWnd *getTextView()
  {
    return m_textView;
  }
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif
  CCustomToolbar    m_wndToolBar;

  void simpleInterface();
protected:  // control bar embedded members
  CStatusBar  m_wndStatusBar;

  // Generated message map functions
protected:
  //{{AFX_MSG(CMainFrame)
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  // NOTE - the ClassWizard will add and remove member functions here.
  //    DO NOT EDIT what you see in these blocks of generated code!
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
private:
  CImageList * normalImages;
  CImageList * disabledImages;
  CImageList * hotImages;
  CSplitterWnd m_wndSplitter;
  CWnd *m_textView;
  bool m_simpleInterface;
  CMenu m_wndMainMenu;
  bool m_created;
protected:
  virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
  afx_msg void OnSize(UINT nType, int cx, int cy);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__CF7BD0CA_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
