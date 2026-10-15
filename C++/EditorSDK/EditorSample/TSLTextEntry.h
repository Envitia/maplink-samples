#if !defined(AFX_TEXTENTRY_H__59E32AD1_FF71_11D1_B2BB_0060087B0780__INCLUDED_)
#define AFX_TEXTENTRY_H__59E32AD1_FF71_11D1_B2BB_0060087B0780__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TextEntry.h : header file
//

//Standard LandLink Dialog to ask for some Text from the 
//user. 
//
//This may be replaced or enhanced by the client application.

class TSLTextEntry : public CDialog
{
// Construction
public:
	//Method to initialise the dialog members and ask for 
	//some text.
	//
	//This method invokes DoModal internally and returns the 
	//string entered by the user.
  int askForData( const std::string & prompt, std::string & value );
	//Constructor for Text Entry Dialog.
	TSLTextEntry(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(TSLTextEntry)
	enum { IDD = IDD_TEXT_ENTRY };
	//Attribute containing text string to be displayed as a 
	//prompt to the user.
	CString	m_prompt;
	//Attribute containing text string entered by the user.
	CString	m_value;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TSLTextEntry)
	protected:
	//Dialog data exchange.
	//
	//Automatically generate by DevStudio Class Wizard.
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(TSLTextEntry)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTENTRY_H__59E32AD1_FF71_11D1_B2BB_0060087B0780__INCLUDED_)
