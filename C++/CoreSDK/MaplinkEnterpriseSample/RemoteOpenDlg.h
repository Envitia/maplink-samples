#if !defined(AFX_REMOTEOPENDLG_H__7B7D0B72_BB9D_11D4_A5BF_00104BB747B9__INCLUDED_)
#define AFX_REMOTEOPENDLG_H__7B7D0B72_BB9D_11D4_A5BF_00104BB747B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RemoteOpenDlg.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CRemoteOpenDlg dialog

class CRemoteOpenDlg : public CDialog
{
// Construction
public:
	CRemoteOpenDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRemoteOpenDlg)
	enum { IDD = IDD_REMOTEOPEN };
	CString	m_Location;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRemoteOpenDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRemoteOpenDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REMOTEOPENDLG_H__7B7D0B72_BB9D_11D4_A5BF_00104BB747B9__INCLUDED_)
