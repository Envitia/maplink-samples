#pragma once


//Messages

#define WM_CAPABILITIES_LOADED    (WM_WMS_MESSAGE_BASE+1)
#define WM_CAPABILITIES_LOAD_FAIL (WM_WMS_MESSAGE_BASE+2)
#define WM_CAPABILITIES_COMPLETE  (WM_WMS_MESSAGE_BASE+3)
#define WM_SHOW_DS_DLG            (WM_WMS_MESSAGE_BASE+4)
#define WM_NO_COMMON_CRS          (WM_WMS_MESSAGE_BASE+5)
#define WM_NO_SUPPORTED_CRS       (WM_WMS_MESSAGE_BASE+6)
#define WM_IDP_CHOICE             (WM_WMS_MESSAGE_BASE+7)
#define WM_CREDENTIALS_REQUIRED   (WM_WMS_MESSAGE_BASE+8)
#define WM_CRS_CHOICE             (WM_WMS_MESSAGE_BASE+9)


// COpenServiceDlg dialog

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
  afx_msg void OnBnClickedEcpCheck();

  afx_msg LRESULT OnCapabilitiesLoaded(WPARAM wparam, LPARAM lparam);
  afx_msg LRESULT OnCapabilitiesLoadFail(WPARAM wparam, LPARAM lparam);
  afx_msg LRESULT OnCapabilitiesComplete(WPARAM wparam, LPARAM lparam);
  afx_msg LRESULT OnShowDSDialog(WPARAM wparam, LPARAM lparam);
  afx_msg LRESULT OnNoCommonCRS(WPARAM wparam, LPARAM lparam);
  afx_msg LRESULT OnNoSupportedCRS(WPARAM wparam, LPARAM lparam);
  afx_msg LRESULT OnCredentialsRequired(WPARAM wparam, LPARAM lparam);
  afx_msg LRESULT OnChoiceOfIDP(WPARAM wparam, LPARAM lparam);
  afx_msg LRESULT OnCrsChoice(WPARAM wParam, LPARAM lParam);
  
  void setDataLayer( TSLWMSDataLayer * layer )
  {
    m_layer = layer;
  }
  const string& chosenURL() const
  {
    return m_url;
  }

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()
private:
  static HTREEITEM AddLayerToTree( CTreeCtrl * tree, HTREEITEM parent, HTREEITEM after, TSLWMSServiceLayer* layer);
  static void SetAllChildItemsToPartial(CListBox * layerList, CTreeCtrl * tree, HTREEITEM parent);
  static void SetAllChildItemsToOff(CListBox * layerList, CTreeCtrl * tree, HTREEITEM parent);

  void setAllChildLayersToBeNonVisible(TSLWMSServiceLayer * layer);
  
  void resetDialogControls();

  void makeGoButtonDefault();
  void makeOkButtonDefault();

  TSLWMSDataLayer * m_layer;
  string m_url;
  CEvent * m_event;
  TSLWMSServiceLayer** m_rootLayer;
  TSLSimpleString m_username;
  TSLSimpleString m_password;

  CImageList m_treeCheckboxes;
  CExtentedBitmapButton m_topButton;
  CExtentedBitmapButton m_upButton;
  CExtentedBitmapButton m_downButton;
  CExtentedBitmapButton m_bottomButton;
};
