#pragma once

// COptionsDlg dialog

class COptionsDlg : public CDialog
{
  DECLARE_DYNAMIC(COptionsDlg)

public:
  COptionsDlg(CWnd* pParent = NULL);   // standard constructor
  virtual ~COptionsDlg();

  // Dialog Data
  enum { IDD = IDD_OPTIONSDLG };

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()
public:
  CSliderCtrl m_cacheSlider;
  virtual BOOL OnInitDialog();
  CSliderCtrl m_sfSlider;
  CButton m_certCNInvalid;
  CButton m_unknownCA;
  CComboBox m_tileLoadOrder;
protected:
  virtual void OnOK();
public:
  CComboBox m_tileLevels;
  CComboBox m_initalWMSVersion;
  int m_numberOfThreads;
};
