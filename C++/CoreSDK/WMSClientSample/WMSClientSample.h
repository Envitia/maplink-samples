// WMSClientSample.h : main header file for the WMSClientSample application
//

#if !defined(AFX_WMSClientSample_H__CF7BD0C6_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_WMSClientSample_H__CF7BD0C6_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

class CDataLayerOrderDlg;

/////////////////////////////////////////////////////////////////////////////
// WMSClientSampleApp:
// See WMSClientSample.cpp for the implementation of this class
//

class WMSClientSampleApp : public CWinApp
{
public:
	WMSClientSampleApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(WMSClientSampleApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(WMSClientSampleApp)
	afx_msg void OnAppAbout();
        afx_msg void OnFileOpen();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
        afx_msg void OnFileOpenwmts();
        afx_msg void OnFileAddwms();
        afx_msg void OnFileAddwmts();
        afx_msg void OnFileAddbackgroundmap();
        afx_msg void OnEditLayerorder();
	DECLARE_MESSAGE_MAP()
private:
        CDataLayerOrderDlg* m_dataLayerOrderDlg;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WMSClientSample_H__CF7BD0C6_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
