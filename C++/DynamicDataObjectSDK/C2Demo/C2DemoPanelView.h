#if !defined(AFX_C2DEMOPANELVIEW_H__DFA3E2C3_4B64_11D5_BD30_00104BB05CAE__INCLUDED_)
#define AFX_C2DEMOPANELVIEW_H__DFA3E2C3_4B64_11D5_BD30_00104BB05CAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// C2DemoPanelView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CC2DemoPanelView view
class CTrackInfo;

class CC2DemoPanelView : public CView
{
protected:
  CC2DemoPanelView();           // protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CC2DemoPanelView)

  // Attributes
public:

  // Operations
public:

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CC2DemoPanelView)
protected:
  virtual void OnDraw(CDC* pDC);      // overridden to draw this view
  virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
  //}}AFX_VIRTUAL

  // Implementation
protected:
  virtual ~CC2DemoPanelView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
protected:
  //{{AFX_MSG(CC2DemoPanelView)
  afx_msg void OnSize(UINT nType, int cx, int cy);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()

  void createInfoPanel() ;

  CTrackInfo * m_pTrackInfo ;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_C2DEMOPANELVIEW_H__DFA3E2C3_4B64_11D5_BD30_00104BB05CAE__INCLUDED_)
