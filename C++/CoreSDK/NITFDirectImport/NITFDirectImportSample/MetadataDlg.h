#pragma once


// CMetadataDlg dialog


class CMetadataDlg : public CDialog
{
  DECLARE_DYNAMIC(CMetadataDlg)

public:
  CMetadataDlg(bool & m_windowVisible, CWnd* pParent = NULL);   // standard constructor
  virtual ~CMetadataDlg();

  afx_msg void OnClose();
  void updateFromDataLayer(TSLNITFFilterDataLayer * dataLayer);
  // Dialog Data
  enum { IDD = IDD_METADATA_DLG };

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()

  bool & m_windowVisible;
};
