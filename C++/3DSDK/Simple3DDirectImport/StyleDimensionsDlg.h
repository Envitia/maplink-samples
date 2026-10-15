////////////////////////////////////////////////////////
// Copyright (c) 1998 to 2017 by Envitia Group PLC
////////////////////////////////////////////////////////
#pragma once

// CStyleDimensionsDlg dialog
#include "resource.h"

class CStyleDimensionsDlg : public CDialog
{
  DECLARE_DYNAMIC(CStyleDimensionsDlg)

public:
  CStyleDimensionsDlg(CWnd* pParent, TSLWMSServiceLayer* rootLayer);   // standard constructor
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
  TSLWMSServiceLayer* m_rootLayer;
  static bool findAllVisibleLayers( TSLWMSServiceLayer * layer, map<int, TSLWMSServiceLayer *, less<int> > & visibleLayers );
  CComboBox m_listStyleCombo;
public:
  afx_msg void OnNMKillfocusStylelist(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnNMSetfocusStylelist(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnCbnSelchangeStylecombo();
  afx_msg void OnTvnSelchangedDimTree(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnBnClickedOk();
};
