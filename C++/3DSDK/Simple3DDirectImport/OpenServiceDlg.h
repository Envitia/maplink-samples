#pragma once
////////////////////////////////////////////////////////
// Copyright (c) 1998 to 2017 by Envitia Group PLC
////////////////////////////////////////////////////////
#include "resource.h"

// COpenServiceDlg dialog
class CExtentedBitmapButton : public CBitmapButton
{
public:
  BOOL SetBitmaps(HBITMAP pBitmap,
                  HBITMAP pBitmapSel = 0,
		  HBITMAP pBitmapFocus = 0,
		  HBITMAP pBitmapDisabled = 0);

};

class COpenServiceDlg : public CDialog
{
  DECLARE_DYNAMIC(COpenServiceDlg)

public:
  COpenServiceDlg(CWnd* pParent = NULL);   // standard constructor
  virtual ~COpenServiceDlg();
  virtual BOOL OnInitDialog();
  virtual BOOL DestroyWindow();

  // Dialog Data
  enum { IDD = IDD_OPENSERVICEDLG };

  afx_msg void OnBnClickedGobutton();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnNMClickLayertree(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnLbnSelchangeLayerorder();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedTopbutton();
  afx_msg void OnBnClickedUpbutton();
  afx_msg void OnBnClickedDownbutton();
  afx_msg void OnBnClickedBottombutton();
  afx_msg void OnTvnSelchangedLayertree(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnTvnKeydownLayertree(NMHDR *pNMHDR, LRESULT *pResult);

  afx_msg LRESULT OnCapabilitiesLoaded(WPARAM wparam, LPARAM lparam);
  afx_msg LRESULT OnCapabilitiesLoadFail(WPARAM wparam, LPARAM lparam);
  afx_msg LRESULT OnCapabilitiesComplete(WPARAM wparam, LPARAM lparam);
  afx_msg LRESULT OnShowDSDialog(WPARAM wparam, LPARAM lparam);
  afx_msg LRESULT OnNoCommonCRS(WPARAM wparam, LPARAM lparam);
  afx_msg LRESULT OnNoSupportedCRS(WPARAM wparam, LPARAM lparam);
  
  void setDataLayer( TSLWMSDataLayer * layer )
  {
    m_layer = layer;
  }

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()
private:
  static HTREEITEM AddLayerToTree( CTreeCtrl * tree, HTREEITEM parent, HTREEITEM after, TSLWMSServiceLayer* layer);
  static void SetAllChildItemsToPartial(CListBox * layerList, CTreeCtrl * tree, HTREEITEM parent);
  static void SetAllChildItemsToOff(CListBox * layerList, CTreeCtrl * tree, HTREEITEM parent);

  void setAllChildLayersToBeNonVisible(TSLWMSServiceLayer * layer);

  TSLWMSDataLayer * m_layer;
  CEvent * m_event;
  TSLWMSServiceLayer** m_rootLayer;

  CImageList m_treeCheckboxes;
  CExtentedBitmapButton m_topButton;
  CExtentedBitmapButton m_upButton;
  CExtentedBitmapButton m_downButton;
  CExtentedBitmapButton m_bottomButton;
   
};
