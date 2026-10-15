#if !defined(AFX_PREFERENCES_H__6EFA6C59_1117_11D4_9287_00C04FA1187D__INCLUDED_)
#define AFX_PREFERENCES_H__6EFA6C59_1117_11D4_9287_00C04FA1187D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Preferences.h : header file
//

//Dialog box for user preferences.
//
//This is used for selection of display units.

class CPreferences : public CDialogBar
{
// Construction
public:
	//Dialog box constructor.
	//
	//Automatically generate by DevStudio Class Wizard.
	CPreferences();   // standard constructor
    bool initialiseAndCreate( CFrameWnd * frame ) ;


// Dialog Data
	//{{AFX_DATA(CPreferences)
	enum { IDD = IDD_PREFERENCES };
	CComboBox	m_cursorReadoutControl;
	double	m_angularValue;
	double	m_distanceValue;
	int		m_cursorReadoutType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPreferences)
	protected:
	//Dialog data exchange.
	//
	//Automatically generate by DevStudio Class Wizard.
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

    //Command Update handler.
    //
    //This is required to stop MFC from greying out all 
    //controls in the panel since there are no handlers in 
    //the parent class.
    virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler );
  
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPreferences)
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnChangeAngularValue();
	afx_msg void OnChangeDistanceValue();
	afx_msg void OnSelchangeCursorReadoutType();
	//}}AFX_MSG
	//Dialog initialisation for Attribute Panel.
	//
	//Reads configuration file and initialises controls.
	afx_msg LRESULT OnInitDialog( WPARAM wParam, LPARAM lParam ) ;
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFERENCES_H__6EFA6C59_1117_11D4_9287_00C04FA1187D__INCLUDED_)
