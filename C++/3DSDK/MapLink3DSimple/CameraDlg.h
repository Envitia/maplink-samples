#pragma once


// CCameraDlg dialog

class CCameraDlg : public CDialog
{
  DECLARE_DYNAMIC(CCameraDlg)

public:
  CCameraDlg(CWnd* pParent = NULL);   // standard constructor
  virtual ~CCameraDlg();

  // Dialog Data
  enum { IDD = IDD_CAMERA };

protected:
  virtual void OnOK();
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()
public:
  double  m_xAxis;
  double  m_yAxis;
  double  m_zAxis;

  double  m_angle;
  double  m_fov;
  virtual BOOL OnInitDialog();
  
};
