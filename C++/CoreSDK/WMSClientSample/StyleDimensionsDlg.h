#pragma once

// CStyleDimensionsDlg dialog

class CStyleDimensionsDlg : public CDialog
{
  DECLARE_DYNAMIC(CStyleDimensionsDlg)

public:
  CStyleDimensionsDlg(CWnd* pParent, TSLWMSServiceLayer* rootLayer);   // constructor - WMS
  CStyleDimensionsDlg(CWnd* pParent, TSLWMTSServiceInfo* serviceInfo);   // constructor - WMTS
  virtual ~CStyleDimensionsDlg();
  virtual BOOL OnInitDialog();
  // Dialog Data
  enum { IDD = IDD_STYLESDIMENSIONS };

  afx_msg void OnLvnItemchangedStylelist(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnNMClickStylelist(NMHDR *pNMHDR, LRESULT *pResult);
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()
private:
  BOOL OnInitDialog_WMS();
  BOOL OnInitDialog_WMTS();

  void OnTvnSelchangedDimTree_WMS(LPNMTREEVIEW pNMTreeView, LRESULT *pResult);
  void OnTvnSelchangedDimTree_WMTS(LPNMTREEVIEW pNMTreeView, LRESULT *pResult);

  TSLWMSServiceLayer* m_rootWMSServiceLayer;
  TSLWMTSServiceInfo* m_WMTSServiceInfo;
  static bool findAllVisibleLayers( TSLWMSServiceLayer * layer, map<int, TSLWMSServiceLayer *, less<int> > & visibleLayers );
  CComboBox m_listStyleCombo;
public:
  afx_msg void OnNMKillfocusStylelist(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnNMSetfocusStylelist(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnCbnSelchangeStylecombo();
  afx_msg void OnTvnSelchangedDimTree(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnBnClickedOk();
};
