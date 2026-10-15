#pragma once

class CRSData
{
public:
  const char **m_crsChoices;
  int m_noOfCRSChoices;
  int m_chosenCRS;
  CRSData(const char **crsChoices, int noOfCRSChoices)
    : m_crsChoices(crsChoices)
    , m_noOfCRSChoices(noOfCRSChoices)
    , m_chosenCRS(0)
  {}
};

// CCRSDlg dialog

class CCRSDlg : public CDialog
{
  DECLARE_DYNAMIC(CCRSDlg)

public:
  CCRSDlg(CWnd* pParent, CRSData& crsData);   // standard constructor
  virtual ~CCRSDlg();

  // Dialog Data
  enum { IDD = IDD_CRS_DLG };

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual BOOL OnInitDialog();
  virtual void OnOK();

  DECLARE_MESSAGE_MAP()

private:
  afx_msg void OnNMDblclkCrsList(NMHDR *pNMHDR, LRESULT *pResult);

  CListCtrl m_list;
  CRSData& m_crsData;
};
