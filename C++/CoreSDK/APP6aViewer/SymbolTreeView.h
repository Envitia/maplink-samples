#if !defined(AFX_SYMBOLTREEVIEW_H__8866D83E_5239_46C9_9843_27DB50C6A338__INCLUDED_)
#define AFX_SYMBOLTREEVIEW_H__8866D83E_5239_46C9_9843_27DB50C6A338__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSymbolTreeView view

class CSymbolTreeView : public CTreeView
{
protected:
	CSymbolTreeView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CSymbolTreeView)

// Attributes
public:

// Operations
public:
	HTREEITEM GetFirstItem();
	HTREEITEM GetNextItem( HTREEITEM hItem );
        enum CheckState{ NOSTATE = 0, UNCHECKED, CHECKED, CHILD_CHECKED, 
		SELF_AND_CHILD_CHECKED };
	enum CheckType{ CHECK, UNCHECK, TOGGLE, REFRESH };
	
        bool GetItemData(HTREEITEM hItem, char* data);

        HTREEITEM GetFirstCheckedItem();
        HTREEITEM GetNextCheckedItem( HTREEITEM hItem );
        HTREEITEM GetPrevCheckedItem( HTREEITEM hItem );
        bool IsItemChecked(HTREEITEM hItem);

  
  // Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSymbolTreeView)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
       
	//}}AFX_VIRTUAL
       
        

// Implementation
protected:
	virtual ~CSymbolTreeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CSymbolTreeView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnExpandTree();
        afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
        afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL SetCheck( HTREEITEM hItem, CheckType nCheck );
	
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYMBOLTREEVIEW_H__8866D83E_5239_46C9_9843_27DB50C6A338__INCLUDED_)
