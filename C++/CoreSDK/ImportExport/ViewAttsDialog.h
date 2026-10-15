#if !defined(AFX_VIEWATTSDIALOG_H__115AA080_B53D_4992_9781_6C0173DB383B__INCLUDED_)
#define AFX_VIEWATTSDIALOG_H__115AA080_B53D_4992_9781_6C0173DB383B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewAttsDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewAttsDialog dialog

class CViewAttsDialog : public CDialog
{
// Construction
public:
	CViewAttsDialog(CWnd* pParent = NULL);   // standard constructor
        void receiveEntityDataSet( TSLDataSet* dataSet );

// Dialog Data
	//{{AFX_DATA(CViewAttsDialog)
	enum { IDD = IDD_VIEW_ATTRIBUTES };
	CListCtrl	m_listCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewAttsDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CViewAttsDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWATTSDIALOG_H__115AA080_B53D_4992_9781_6C0173DB383B__INCLUDED_)
