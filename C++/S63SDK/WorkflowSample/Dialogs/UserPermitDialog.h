#pragma once
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

/////////////////////////////////////
  // Test Plan Reference: 3.4.2.2
  //  - Provide a dialog for generating user permits.
  /////////////////////////////////////

class UserPermitDialog : public CDialog
{
  DECLARE_DYNAMIC(UserPermitDialog)

public:

  explicit UserPermitDialog(CWnd* pParent = NULL);   // standard constructor
  virtual ~UserPermitDialog();
  
  //Controls 
  CEdit m_hw_id_input;
  CEdit m_key_input;
  CEdit m_id_input;

  //Dialog variables
  CString m_hw_id;
  CString m_key;
  CString m_id;
  CString m_savePath;

  //ID
  enum { IDD = IDD_USER_PERMIT };

  //Events
  afx_msg void OnBnClickedBrowse();

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()
};
