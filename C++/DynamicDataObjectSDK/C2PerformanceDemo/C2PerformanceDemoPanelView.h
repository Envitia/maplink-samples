#if !defined(AFX_C2PERFORMANCEDEMOPANELVIEW_H__DFA3E2C3_4B64_11D5_BD30_00104BB05CAE__INCLUDED_)
#define AFX_C2PERFORMANCEDEMOPANELVIEW_H__DFA3E2C3_4B64_11D5_BD30_00104BB05CAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// C2PerformanceDemoPanelView.h : header file
//

class CTrackInfo;

/////////////////////////////////////////////////////////////////////////////
// CC2PerformanceDemoPanelView view

class CC2PerformanceDemoPanelView : public CView
{
protected:
	CC2PerformanceDemoPanelView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CC2PerformanceDemoPanelView)

// Attributes
public:

// Operations
public:
        void setSymbolType(bool isAPP6A);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CC2PerformanceDemoPanelView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL
        

// Implementation
protected:
	virtual ~CC2PerformanceDemoPanelView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CC2PerformanceDemoPanelView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void createInfoPanel() ;

	CTrackInfo * m_pTrackInfo ;

        bool m_isAPP6A;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_C2PERFORMANCEDEMOPANELVIEW_H__DFA3E2C3_4B64_11D5_BD30_00104BB05CAE__INCLUDED_)
