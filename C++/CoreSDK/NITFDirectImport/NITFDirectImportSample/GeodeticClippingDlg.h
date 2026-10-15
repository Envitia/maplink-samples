#pragma once


// CGeodeticClippingDlg dialog

class CGeodeticClippingDlg : public CDialog
{
  DECLARE_DYNAMIC(CGeodeticClippingDlg)

public:
  CGeodeticClippingDlg(bool & applyClipping,
                        double & minLat, 
                        double & maxLat, 
                        double & refMer, 
                        double & degEast, 
                        double & degWest,
                        CWnd* pParent = NULL);   // standard constructor

  virtual ~CGeodeticClippingDlg();
  virtual BOOL OnInitDialog();
  afx_msg void OnBnClickedClip();

  // Dialog Data
  enum { IDD = IDD_GEO_CLIP_DLG };

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual void OnOK();

  DECLARE_MESSAGE_MAP()

  bool & m_applyClipping;

  double & m_minLat; 
  double & m_maxLat; 
  double & m_refMer;
  double & m_degEast; 
  double & m_degWest;

};
