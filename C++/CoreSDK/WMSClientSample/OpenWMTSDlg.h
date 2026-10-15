#pragma once


//Messages

#define WM_WMTS_CAPABILITIES_LOADED    (WM_WMTS_MESSAGE_BASE+1)
#define WM_WMTS_CAPABILITIES_LOAD_FAIL (WM_WMTS_MESSAGE_BASE+2)
#define WM_WMTS_CAPABILITIES_COMPLETE  (WM_WMTS_MESSAGE_BASE+3)
#define WM_WMTS_NO_COMMON_CRS          (WM_WMTS_MESSAGE_BASE+5)
#define WM_WMTS_NO_SUPPORTED_CRS       (WM_WMTS_MESSAGE_BASE+6)
#define WM_WMTS_CRS_CHOICE             (WM_WMTS_MESSAGE_BASE+7)

// COpenWMTSDlg dialog

class COpenWMTSDlg : public CDialog
{
  DECLARE_DYNAMIC(COpenWMTSDlg)

public:
  COpenWMTSDlg(CWnd* pParent = NULL);   // standard constructor
  virtual ~COpenWMTSDlg();
  virtual BOOL OnInitDialog();
  virtual BOOL DestroyWindow();

  // Dialog Data
  enum { IDD = IDD_OPENWMTSDLG };

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
  afx_msg void OnLvnItemchangedWmtsLayerDetails(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnBnClickedDsButton();

  afx_msg LRESULT OnCapabilitiesLoaded(WPARAM wparam, LPARAM lparam);
  afx_msg LRESULT OnCapabilitiesLoadFail(WPARAM wparam, LPARAM lparam);
  afx_msg LRESULT OnCapabilitiesComplete(WPARAM wparam, LPARAM lparam);
  afx_msg LRESULT OnShowDSDialog(WPARAM wparam, LPARAM lparam);
  afx_msg LRESULT OnNoCommonCRS(WPARAM wparam, LPARAM lparam);
  afx_msg LRESULT OnNoSupportedCRS(WPARAM wparam, LPARAM lparam);
  afx_msg LRESULT OnCrsChoice(WPARAM wParam, LPARAM lParam);

  void setDataLayer( TSLWMTSDataLayer * layer )
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
  static HTREEITEM AddLayersToTree( CTreeCtrl * tree, HTREEITEM parent, HTREEITEM after, TSLWMTSServiceInfo* serviceInfo);

  void resetDialogControls();

  void makeGoButtonDefault()
  {
    m_okButton.SetButtonStyle( ~BS_DEFPUSHBUTTON & m_okButton.GetButtonStyle() );
    m_goButton.SetButtonStyle( BS_DEFPUSHBUTTON | m_goButton.GetButtonStyle() );
    SetDefID( IDC_WMTS_GOBUTTON );
  }
  void makeOkButtonDefault()
  {
    m_goButton.SetButtonStyle( ~BS_DEFPUSHBUTTON & m_goButton.GetButtonStyle() );
    m_okButton.SetButtonStyle( BS_DEFPUSHBUTTON | m_okButton.GetButtonStyle() );
    SetDefID( IDOK );
  }

  TSLWMTSDataLayer * m_layer;
  string m_url;
  TSLWMTSServiceInfo** m_serviceInfo;
  CEvent * m_event;

  const TSLWMTSServiceLayer* m_layerDetailsSource;  // what m_layerDetails is describing
  map<const TSLWMTSServiceLayer*, int> m_tileMatrixChoices;  // user's choices
  bool m_stopDetectingWmtsLayerDetailsChanges;

  CImageList m_treeCheckboxes;
  CImageList m_layerDetailsCheckboxes;
  CExtentedBitmapButton m_topButton;
  CExtentedBitmapButton m_upButton;
  CExtentedBitmapButton m_downButton;
  CExtentedBitmapButton m_bottomButton;

  CComboBox m_addressCombo;
  CButton m_goButton;
  CListCtrl m_layerDetails;
  CListBox m_layerOrderList;
  CTreeCtrl m_layerTree;
  CButton m_okButton;
  CEdit m_layerAbstract;
  CEdit m_layerTitle;
  CButton m_layerDetailsGroup;
  CButton m_showDS;
  CButton m_extractPrefixParams;
};
