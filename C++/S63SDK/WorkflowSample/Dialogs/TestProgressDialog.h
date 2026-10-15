#pragma once
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "resource.h"

// TestProgressDialog dialog

class TestProgressDialog : public CDialog
{
	DECLARE_DYNAMIC(TestProgressDialog)

public:
	explicit TestProgressDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~TestProgressDialog();

  void setNumberOfTests( const short number );
  void setProgress( const int progress );

  virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_TEST_PROGRESS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
  CProgressCtrl m_progress;
  short m_numberOfTests;
};
