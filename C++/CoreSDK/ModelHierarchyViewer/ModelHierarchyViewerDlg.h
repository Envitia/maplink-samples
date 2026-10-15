// ModelHierarchyViewerDlg.h : header file
//

#if !defined(AFX_MODELHIERARCHYVIEWERDLG_H__C04F8A38_6B40_11D3_B383_0060087B197C__INCLUDED_)
#define AFX_MODELHIERARCHYVIEWERDLG_H__C04F8A38_6B40_11D3_B383_0060087B197C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <vector>

class ModelHierarchy;

/////////////////////////////////////////////////////////////////////////////
// CModelHierarchyViewerDlg dialog

class CModelHierarchyViewerDlg : public CDialog
{
public:
  CModelHierarchyViewerDlg(CWnd* pParent = NULL);	// standard constructor
  ~CModelHierarchyViewerDlg();

  // Dialog Data
  enum { IDD = IDD_MODELHIERARCHYVIEWER_DIALOG };
  
protected:
  virtual BOOL OnInitDialog();
  virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

  ModelHierarchy* findEntity( const HTREEITEM& hItem );
  void clearLoadedModels();
  void loadFile( const CString& filename );
  
  afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
  afx_msg HCURSOR OnQueryDragIcon();
  afx_msg void OnOpenModel();
  afx_msg void OnDblclkCSysTree(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnCollapseTree();
  afx_msg void OnSize( UINT nType, int cx, int cy );
  afx_msg void OnGetMinMaxInfo( MINMAXINFO* lpMMI );
  afx_msg void OnDropFiles( HDROP hDropInfo );

  DECLARE_MESSAGE_MAP()

private:
  CTreeCtrl m_tree;
  CButton m_collapseButton;
  CButton m_openButton;
  CButton m_closeButton;
  HICON m_hIcon;
  CImageList* m_pimagelist;
  vector<ModelHierarchy*> m_models;   //< it is a vector to allow viewing multiple files
};

#endif // !defined(AFX_MODELHIERARCHYVIEWERDLG_H__C04F8A38_6B40_11D3_B383_0060087B197C__INCLUDED_)
