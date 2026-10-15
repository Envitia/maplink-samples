#if !defined(AFX_C2DEMOTREEVIEW_H__DFA3E2C4_4B64_11D5_BD30_00104BB05CAE__INCLUDED_)
#define AFX_C2DEMOTREEVIEW_H__DFA3E2C4_4B64_11D5_BD30_00104BB05CAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// C2DemoTreeView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CC2DemoTreeView view

class CC2DemoTreeView : public CTreeView
{
protected:
	CC2DemoTreeView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CC2DemoTreeView)

// Attributes
public:

// Operations
public:

	// Once the document has loaded a map successfully it calls UpdateAllViews
	// which in turn calls this function to add the map features to the tree
	void setMapFeatureTree() ; 
							      

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CC2DemoTreeView)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CC2DemoTreeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CC2DemoTreeView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void setChildNodes( HTREEITEM node, bool select ) ;
	void setParentNode( HTREEITEM node, bool nodeSelected ) ;

	void getNodePath( HTREEITEM node, char * buffer ) ;

        CImageList * m_pImageList ;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_C2DEMOTREEVIEW_H__DFA3E2C4_4B64_11D5_BD30_00104BB05CAE__INCLUDED_)
