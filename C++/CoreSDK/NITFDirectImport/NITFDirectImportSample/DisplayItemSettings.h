#pragma once

// CDisplayItemSettings dialog

class CDisplayItemSettings : public CDialog
{
  DECLARE_DYNAMIC(CDisplayItemSettings)

public:
  CDisplayItemSettings(TSLFilterDataLayer * dataLayer, CWnd* pParent = NULL);   // standard constructor
  virtual ~CDisplayItemSettings();
  virtual BOOL OnInitDialog();
  afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnCbnSelchangeVisibility();
  afx_msg void OnBnClickedProcessItem();
  afx_msg void OnBnClickedProcessAll();
  afx_msg void OnBnClickedAdvClear();
  afx_msg void OnBnClickedAdvApply();
  afx_msg void OnEnKillfocusAdvExtent();
  afx_msg void OnBnClickedClipping();

  // Dialog Data
  enum { IDD = IDD_DIS_ITEM_SETTINGS };

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()

private:
  TSLFilterDataLayer * m_dataLayer;
  TSLFilterDataLayerDisplayItem * m_currentSelection;
  int m_currentSelectionIndex;

  void UpdateFromCurrent();

  double m_advMinX, m_advMinY, m_advMaxX, m_advMaxY;
};
