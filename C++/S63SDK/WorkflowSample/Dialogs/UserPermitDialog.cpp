// TestSetup.cpp : implementation file
//
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "../stdafx.h"
#include "../S63WorkFlowSample.h"
#include "UserPermitDialog.h"


// TestConfiguration dialog

IMPLEMENT_DYNAMIC(UserPermitDialog, CDialog)

UserPermitDialog::UserPermitDialog(CWnd* pParent /*=NULL*/)
	: CDialog(UserPermitDialog::IDD, pParent)
        , m_hw_id()
        , m_hw_id_input()
        , m_id()
        , m_id_input()
        , m_key()
        , m_key_input()
        , m_savePath()
{

}

UserPermitDialog::~UserPermitDialog()
{

}

void UserPermitDialog::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);

  DDX_Text (pDX, IDC_HW_ID, m_hw_id );
  DDX_Text (pDX, IDC_M_KEY, m_key );
  DDX_Text (pDX, IDC_M_ID, m_id );
  DDX_Text (pDX, IDC_SAVE_PERMIT, m_savePath );

  DDX_Control(pDX, IDC_HW_ID, m_hw_id_input);
  DDX_Control(pDX, IDC_M_KEY, m_key_input);
  DDX_Control(pDX, IDC_M_ID, m_id_input);
}


BEGIN_MESSAGE_MAP(UserPermitDialog, CDialog)
  ON_BN_CLICKED(IDC_BROWSE, &UserPermitDialog::OnBnClickedBrowse)
END_MESSAGE_MAP()

void UserPermitDialog::OnBnClickedBrowse()
{CFileDialog FileDlg(false, _T(".txt"), NULL, OFN_NOCHANGEDIR, _T("User Permit Files (*.txt)|*.txt|All Files (*.*)|*.*||"));
  FileDlg.m_pOFN->lpstrTitle = _T("Select user permit file");

  if( FileDlg.DoModal() == IDOK )
  {
    m_savePath.SetString(FileDlg.GetPathName());
  }

  //set the current text values before we update
  m_hw_id_input.GetWindowText(m_hw_id);
  m_key_input.GetWindowText(m_key);
  m_id_input.GetWindowText(m_id);

  UpdateData(false);
}
