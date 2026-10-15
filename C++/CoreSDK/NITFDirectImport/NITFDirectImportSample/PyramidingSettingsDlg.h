#pragma once


// CPyramidingSettingsDlg dialog

class CPyramidingSettingsDlg : public CDialog
{
  DECLARE_DYNAMIC(CPyramidingSettingsDlg)

public:
  CPyramidingSettingsDlg(TSLRasterInterpolation & interpolation,
    bool & multipage,
    TSLRasterType & rtype,
    int & compressionFactor,
    CWnd* pParent = NULL);   // standard constructor
  virtual ~CPyramidingSettingsDlg();
  virtual BOOL OnInitDialog();
  // Dialog Data
  enum { IDD = IDD_PYRAMID_SETTINGS };

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual void OnOK();
  DECLARE_MESSAGE_MAP()

  TSLRasterInterpolation & m_interpolation;
  bool & m_multipage;
  TSLRasterType & m_rtype;
  int & m_compressionFactor;

  TSLNITFFilterDataLayer * m_dataLayer;
};
