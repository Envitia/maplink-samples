#if !defined(AFX_NEWPROJECTIONPROPERTIESDIALOG_H__CB1B2878_F1AF_48E5_9000_752D1FD2A8B2__INCLUDED_)
#define AFX_NEWPROJECTIONPROPERTIESDIALOG_H__CB1B2878_F1AF_48E5_9000_752D1FD2A8B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewProjectionPropertiesDialog.h : header file
//


#include "resource.h"


/////////////////////////////////////////////////////////////////////////////
// NewProjectionPropertiesDialog dialog

class NewProjectionPropertiesDialog : public CDialog
{
// Construction
public:
	NewProjectionPropertiesDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(NewProjectionPropertiesDialog)
	enum { IDD = IDD_NEWPROJECTIONPROPERTIESDIALOG };
	double	m_maxDist;
	double	m_initialLat;
	double	m_initialLong;
	int		m_projectionChoice;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NewProjectionPropertiesDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(NewProjectionPropertiesDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWPROJECTIONPROPERTIESDIALOG_H__CB1B2878_F1AF_48E5_9000_752D1FD2A8B2__INCLUDED_)
