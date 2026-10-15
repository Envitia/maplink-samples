#if !defined(AFX_C2PERFORMANCEDEMOTOTEVIEW_H__DFA3E2C5_4B64_11D5_BD30_00104BB05CAE__INCLUDED_)
#define AFX_C2PERFORMANCEDEMOTOTEVIEW_H__DFA3E2C5_4B64_11D5_BD30_00104BB05CAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// C2PerformanceDemoToteView.h : header file
//

class CTrack;

/////////////////////////////////////////////////////////////////////////////
// CC2PerformanceDemoToteView view

class CC2PerformanceDemoToteView : public CListView
{
protected:
	CC2PerformanceDemoToteView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CC2PerformanceDemoToteView)

// Attributes
public:

// Operations
public:

  void addTrackToTote( CTrack * pTrack ) ;
  void clearAllTracks ( );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CC2PerformanceDemoToteView)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CC2PerformanceDemoToteView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CC2PerformanceDemoToteView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	vector<CTrack*> m_tracks ;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_C2PERFORMANCEDEMOTOTEVIEW_H__DFA3E2C5_4B64_11D5_BD30_00104BB05CAE__INCLUDED_)
