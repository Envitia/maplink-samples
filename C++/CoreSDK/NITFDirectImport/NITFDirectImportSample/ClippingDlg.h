#pragma once


// CClippingDlg dialog

class CClippingDlg : public CDialog
{
  DECLARE_DYNAMIC(CClippingDlg)

public:
  CClippingDlg(bool & applyClipping,
               double & minX, 
               double & minY, 
               double & maxX, 
               double & maxY, 
               CWnd* pParent = NULL);   // standard constructor

  virtual ~CClippingDlg();
  virtual BOOL OnInitDialog();
  afx_msg void OnBnClickedClip();
  // Dialog Data
  enum { IDD = IDD_CLIP_DLG };

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual void OnOK();

  DECLARE_MESSAGE_MAP()

  bool & m_applyClipping;

  double * m_minX;
  double * m_minY; 
  double * m_maxX; 
  double * m_maxY;
  
};
