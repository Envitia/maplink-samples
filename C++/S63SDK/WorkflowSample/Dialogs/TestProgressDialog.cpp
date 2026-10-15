// TestProgressDialog.cpp : implementation file
//
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "../stdafx.h"
#include "../S63WorkFlowSample.h"
#include "TestProgressDialog.h"


// TestProgressDialog dialog

IMPLEMENT_DYNAMIC(TestProgressDialog, CDialog)

TestProgressDialog::TestProgressDialog(CWnd* pParent /*=NULL*/)
	: CDialog(TestProgressDialog::IDD, pParent)
        , m_numberOfTests(0)
        , m_progress()
{

}

TestProgressDialog::~TestProgressDialog()
{
}

void TestProgressDialog::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_TEST_PROGRESS, m_progress);
}

void TestProgressDialog::setNumberOfTests( const short number )
{
  m_numberOfTests = number;
}

void TestProgressDialog::setProgress( const int progress )
{
  m_progress.SetPos( progress );
}

BOOL TestProgressDialog::OnInitDialog()
{
  CDialog::OnInitDialog(); 

  m_progress.SetRange( 0, m_numberOfTests );
  m_progress.SetPos( 0 );

  return TRUE;
}

BEGIN_MESSAGE_MAP(TestProgressDialog, CDialog)
END_MESSAGE_MAP()



// TestProgressDialog message handlers
