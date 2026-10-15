#pragma once

#include "resource.h"
// CNumberOfTracksDlg dialog

class CNumberOfTracksDlg : public CDialog
{
  DECLARE_DYNAMIC(CNumberOfTracksDlg)

public:
  CNumberOfTracksDlg(CWnd* pParent = NULL, int initialNoOfTracks = 30);   // standard constructor
  virtual ~CNumberOfTracksDlg();

  // Dialog Data
  enum { IDD = IDD_NO_OF_TRACKS };

  afx_msg void OnOkClicked();

  int getNoOfTracks ();

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()

private:
  int m_noOfTracks;

public:
  virtual BOOL OnInitDialog();
};
