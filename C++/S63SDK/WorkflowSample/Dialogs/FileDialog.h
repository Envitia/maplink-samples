#pragma once
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

// FileDialog dialog

class FileDialog : public CDialog
{
  DECLARE_DYNAMIC(FileDialog)

public:

  FileDialog(LPCTSTR title, LPCTSTR label, LPCTSTR filter, CWnd* pParent = NULL);   // standard constructor
  virtual ~FileDialog();

  //Overrides
  BOOL OnInitDialog();

  //Events
  afx_msg void OnBnClickedBrowse();

  //Dialog Variables
  CStatic m_label;
  CString m_input;

  //ID
  enum { IDD = IDD_FILE_DIALOG };

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()

private:

  LPCTSTR m_title;
  LPCTSTR m_labelText;
  LPCTSTR m_filter;

  FileDialog(); // not required
};
