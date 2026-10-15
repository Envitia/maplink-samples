// TestSetup.cpp : implementation file
//
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "../stdafx.h"
#include "../S63WorkFlowSample.h"
#include "FileDialog.h"


// TestSetup dialog

IMPLEMENT_DYNAMIC(FileDialog, CDialog)

FileDialog::FileDialog(LPCTSTR title, LPCTSTR label, LPCTSTR filter, CWnd* pParent /*=NULL*/)
	: CDialog(FileDialog::IDD, pParent)
        , m_title(title)
        , m_labelText(label)
        , m_filter(filter)
        , m_input()
        , m_label()
{

}

FileDialog::~FileDialog()
{

}

void FileDialog::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);

  DDX_Text (pDX, IDC_INPUT, m_input );
  DDX_Control(pDX, IDC_LABEL, m_label);
}

BOOL FileDialog::OnInitDialog()
{
   CDialog::OnInitDialog();

   this->SetWindowText( m_title );
   m_label.SetWindowText( m_labelText );
   
   return TRUE;
}


BEGIN_MESSAGE_MAP(FileDialog, CDialog)
  ON_BN_CLICKED(ID_BROWSE, &FileDialog::OnBnClickedBrowse)
END_MESSAGE_MAP()

void FileDialog::OnBnClickedBrowse()
{
  CFileDialog FileDlg(true, &m_filter[1], NULL, OFN_NOCHANGEDIR, m_filter);
  if (m_labelText != NULL)
    FileDlg.m_pOFN->lpstrTitle  = m_labelText;

  if( FileDlg.DoModal() != IDOK )
  {
    return;
  }
  m_input.SetString(FileDlg.GetPathName());

  UpdateData(false);
}
