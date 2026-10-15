#pragma once

#include "AnalysisCallbacks.h"
#include "afxwin.h"

// VectorFeatureConfiguration dialog

class VectorFeatureConfiguration : public CDialogEx
{
  DECLARE_DYNAMIC(VectorFeatureConfiguration)

public:
  VectorFeatureConfiguration(TSLDirectImportDataLayer * dataLayer, TSLDirectImportDataSet * dataSet, TSLFeatureClassConfig * fcc, TSLClientFeatureClassifierCustom * myClassifier, CWnd* pParent = NULL);
  virtual ~VectorFeatureConfiguration();

  // Dialog Data
#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_FEATURE_CONFIG };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  TSLDirectImportDataLayer * m_dataLayer;
  TSLDirectImportDataSet *   m_dataSet;
  AnalysisCallbacks          m_analysisCallbacks;
  TSLFeatureClassConfig *    m_fcc;
  TSLClientFeatureClassifierCustom * m_myClassifier;

  DECLARE_MESSAGE_MAP()
public:

  afx_msg void OnBnClickedGenerateFcc();
  afx_msg void OnBnClickedCustomFcc();
  afx_msg void OnBnClickedLoadFcc();
  CButton m_customButton;
};
