#if !defined(AFX_LOSVIEW_H__47764611_5413_411E_BA9A_427C7432DD7F__INCLUDED_)
#define AFX_LOSVIEW_H__47764611_5413_411E_BA9A_427C7432DD7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LOSView.h : header file
//

//#include "tslLineofSightAreaMap.h"
/////////////////////////////////////////////////////////////////////////////
// CLOSView view

class CLOSView : public CScrollView
{
protected:
	CLOSView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CLOSView)

// Attributes
public:





// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLOSView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CLOSView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CLOSView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOSVIEW_H__47764611_5413_411E_BA9A_427C7432DD7F__INCLUDED_)
