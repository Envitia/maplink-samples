/////////////////////////////////////////////////////////////////////////////
// SplitterWndEx .h : header file
//
// The code contained in this file is based on the CSplitterWndEx
// class written by Oleg G. Galkin - OlegGa@vest.msk.ru
//		/splitter/show_hide_static_panes.shtml
//
// Class exended by Kirk Stowell - kstowell@codejockeys.com
//		http://www.codejockeys.com/kstowell/
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __SPLITTERWNDEX_H__
#define __SPLITTERWNDEX_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// CSplitterWndEx  is a CSplitterWnd derived class which adds the ability
// to hide and show splitter panes, based upon its index.
class CSplitterWndEx  : public CSplitterWnd
{
public:
  
  // Default constructor
  //
  CSplitterWndEx ();
  
  // Virtual destructor
  //
  virtual ~CSplitterWndEx ();
  
public:
  
  int m_nHiddenCol;	// Index of the hidden column.
  int m_nHiddenRow;	// Index of the hidden row.
  
public:
  
  // this member function is called to switch, or swap a splitter
  // view with another.
  //
  BOOL SwitchView(int nRow, int nCol, CView *pNewView);
  
  // this member function is called to replace an existing splitter
  // view with another.
  //
  BOOL ReplaceView(int nRow, int nCol, CRuntimeClass *pViewClass);
  
  // this member function is called to show the column that was
  // previously hidden.
  //
  void ShowColumn();
  
  // this member function will hide a column based upon its index.
  //
  void HideColumn(int nColHide);
  
  // this member function is called to show the row that was
  // previously hidden.
  //
  void ShowRow();
  
  // this member function will hide a row based upon its index.
  //
  void HideRow(int nRowHide);
  
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CSplitterWndEx )
  //}}AFX_VIRTUAL
  
  // Generated message map functions
protected:
  //{{AFX_MSG(CSplitterWndEx )
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __SPLITTERWNDEX_H__
