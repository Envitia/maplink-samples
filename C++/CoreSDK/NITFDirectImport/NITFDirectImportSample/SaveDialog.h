#pragma once


// CSaveDialog dialog

class CSaveDialog : public CDialog
{
  DECLARE_DYNAMIC(CSaveDialog)

public:
  CSaveDialog(TSLNITFFilterDataLayer * dataLayer, bool selectDisplayItem, CWnd* pParent = NULL);   // standard constructor
  virtual ~CSaveDialog();
  virtual BOOL OnInitDialog();

  // Dialog Data
  enum { IDD = IDD_SAVE_DLG };

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  TSLNITFFilterDataLayer* m_dataLayer;
  bool m_selectDisplayItem;
  DECLARE_MESSAGE_MAP()
  virtual void OnOK();
  bool saveDisplayItem( int index );

};
