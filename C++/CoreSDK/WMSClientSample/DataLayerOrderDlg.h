#pragma once
#include "afxwin.h"

class WMSClientSampleView;
struct NamedDataLayer;

// CDataLayerOrderDlg dialog

class CDataLayerOrderDlg : public CDialog
{
  DECLARE_DYNAMIC(CDataLayerOrderDlg)

public:
  CDataLayerOrderDlg(CWnd* pParent);
  virtual ~CDataLayerOrderDlg();

  virtual BOOL OnInitDialog();
  virtual BOOL DestroyWindow();

  void refresh();

// Dialog Data
  enum { IDD = IDD_LAYER_ORDER_DLG };

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual void OnOK();
  virtual void OnCancel();

  afx_msg void OnLbnSelchangeDatalayerorder();
  afx_msg void OnBnClickedDatalayerTopbutton();
  afx_msg void OnBnClickedDatalayerUpbutton();
  afx_msg void OnBnClickedDatalayerDownbutton();
  afx_msg void OnBnClickedDatalayerBottombutton();
  afx_msg void OnNMReleasedcaptureTransparencySlider(NMHDR *pNMHDR, LRESULT *pResult);

  DECLARE_MESSAGE_MAP()

private:
  CExtentedBitmapButton m_topButton;
  CExtentedBitmapButton m_upButton;
  CExtentedBitmapButton m_downButton;
  CExtentedBitmapButton m_bottomButton;

  CListBox m_dataLayerOrderList;
  CSliderCtrl m_transparencySlider;
};
