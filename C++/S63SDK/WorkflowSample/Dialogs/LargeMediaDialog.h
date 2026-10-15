#pragma once
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

// LargeMediaDialog dialog

  //////////////////////////////////////
  // Test Plan Reference: 3.4.2.3.2
  //  - Support opening of MEDIA.txt files
  /////////////////////////////////////

class LargeMediaDialog : public CDialog
{
  DECLARE_DYNAMIC(LargeMediaDialog)

public:

  explicit LargeMediaDialog(S63Workflow * workFlow, CWnd* pParent = NULL);   // standard constructor
  virtual ~LargeMediaDialog();

  //Events
  afx_msg void OnBnClickedMediaBrowse();
  afx_msg void OnBnClickedAdd();
  afx_msg void OnBnClickedRemove();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedStoreBrowse();

  //ID
  enum { IDD = IDD_LARGE_MEDIA };

  //Dialog variables
  CString m_largeMediaFile;
  CString m_dataStore;
  vector<string> m_mediaRecords;

  //Controls
  CEdit m_mediaInput;
  CEdit m_dataStoreInput;
  CListBox m_mediaRecordInput;
  CListBox m_selectedMediaInput;

  CButton m_addButton;
  CButton m_removeButton;

  //Methods
  CString& dataStore();

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()

private:

  LargeMediaDialog(); // not required

  void updateMediaList();
        
  S63Workflow * m_workFlow;
  static string m_initialFolder;
};
