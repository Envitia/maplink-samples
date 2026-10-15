// Hello GlobeView.h : interface of the CHelloGlobeView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_HELLOGLOBEVIEW_H__2A6FF601_30D3_4550_BC7F_7BD356905B8D__INCLUDED_)
#define AFX_HELLOGLOBEVIEW_H__2A6FF601_30D3_4550_BC7F_7BD356905B8D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CHelloGlobeView : public CView
{
protected: // create from serialization only
	CHelloGlobeView();
	DECLARE_DYNCREATE(CHelloGlobeView)

// Attributes
public:
	CHelloGlobeDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHelloGlobeView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHelloGlobeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CHelloGlobeView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

  TSLNTSurface * m_drawingSurface ;
};

#ifndef _DEBUG  // debug version in Hello GlobeView.cpp
inline CHelloGlobeDoc* CHelloGlobeView::GetDocument()
   { return (CHelloGlobeDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HELLOGLOBEVIEW_H__2A6FF601_30D3_4550_BC7F_7BD356905B8D__INCLUDED_)
