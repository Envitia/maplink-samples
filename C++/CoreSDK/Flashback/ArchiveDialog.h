#pragma once

#include "resource.h"


// CArchiveDialog dialog

class CArchiveDialog : public CDialog
{
  DECLARE_DYNAMIC(CArchiveDialog)

public:
  CArchiveDialog(char const* dir, CWnd* pParent = NULL);   // standard constructor
  virtual ~CArchiveDialog();

  // Dialog Data
  enum { IDD = IDD_ARCHIVEDIALOG };

  CString const& archiveRoot() const
  {
    return m_archiveRoot;
  }

  BOOL archiveInSubdir() const
  {
    return m_archiveInSubdir;
  }

  virtual BOOL OnInitDialog();
  afx_msg void OnBnClickedFindDir();

private:
  CString m_archiveRoot;
  BOOL    m_archiveInSubdir;

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()
  
};
