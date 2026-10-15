#pragma once
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "resource.h"

  //////////////////////////////////////
  // Test Plan Reference: 3.4.2.3
  //  - Provide feedback of Media Ingest progress.
  /////////////////////////////////////

class ProgressDialog : public CDialog
{
  DECLARE_DYNAMIC(ProgressDialog)

public:

  explicit ProgressDialog(CWnd* pParent = NULL);   // standard constructor
  virtual ~ProgressDialog();

  // Updates the two progress bars on the dialog to the specified values.
  void setProgress( double percentOfExchangeSetDone, double percentOfCurrentCellDone );

  // If the cancel button on the dialog has been pressed, this will return true. This is
  // used to determine if the user has requested the operation be aborted.
  bool hasBeenCancelled() const;

  virtual BOOL OnInitDialog();

  //ID
  enum { IDD = IDD_PROGRESS_DIALOG };

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()

private:

  bool m_cancelled;

  // MFC controls for the two progress bars
  CProgressCtrl m_ingestProgress;
  CProgressCtrl m_cellProgress;

public:

  afx_msg void OnBnClickedCancel();
};
