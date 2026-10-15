#pragma once
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "../S63/s63workflow.h"

/////////////////////////////////////
// Test Plan Reference: 3.4.2.4
//  - Provide a means of configuring the S63 Scale Bands.
/////////////////////////////////////

class ScaleBandDialog : public CDialog
{
  DECLARE_DYNAMIC(ScaleBandDialog)

public:

  explicit ScaleBandDialog(S63Workflow * workFlow, CWnd* pParent = NULL);   // standard constructor
  virtual ~ScaleBandDialog();

  // ID
  enum { IDD = IDD_SCALE_BANDS };

  // Overrides
  BOOL OnInitDialog();

  // Controls
  CEdit m_overviewLowerInput;
  CEdit m_overviewUpperInput;
  CEdit m_generalLowerInput;
  CEdit m_generalUpperInput;
  CEdit m_coastalLowerInput;
  CEdit m_coastalUpperInput;
  CEdit m_approachLowerInput;
  CEdit m_approachUpperInput;
  CEdit m_harbourLowerInput;
  CEdit m_harbourUpperInput;
  CEdit m_berthingLowerInput;
  CEdit m_berthingUpperInput;

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()

private:
 
  ScaleBandDialog(); // Not required.

  void resetScaleBandValues();
  void setScaleBandValues();

  S63Workflow * m_workFlow;

public:
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnResetScaleBandsClicked();
};
