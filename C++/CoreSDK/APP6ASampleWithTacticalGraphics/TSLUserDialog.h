#if !defined(AFX_USERDIALOG_H__781EAB61_22CD_11D2_8996_00104B556E11__INCLUDED_)
#define AFX_USERDIALOG_H__781EAB61_22CD_11D2_8996_00104B556E11__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TSLUserDialog.h : header file
//

//Standard LandLink Dialog to ask for a choice from the user. 
//
//This may be replaced or enhanced by the client application.

class TSLUserDialog : public CDialog
{
// Construction
public:
	//Method to initialise the dialog members and ask for a 
	//user choice.
	//
	//This method invokes DoModal internally and returns the 
	//button pressed by the user.
    int askForData(const std::string & prompt, const std::string & b1, const std::string & b2,
		   const std::string & b3, const std::string & b4 ) ;

	//Constructor for User Dialog.
    TSLUserDialog(CWnd* pParent = NULL);   // standard constructor
  
	//Temporary storage for button 1 label text.  
	//
	//This is necessary since the button window will not 
	//exist during the dialog box construction.
    std::string lb1 ;
	//Temporary storage for button 2 label text.  
	//
	//This is necessary since the button window will not 
	//exist during the dialog box construction.
    std::string lb2 ;
	//Temporary storage for button 3 label text.  
	//
	//This is necessary since the button window will not 
	//exist during the dialog box construction.
    std::string lb3 ;
	//Temporary storage for button 4 label text.  
	//
	//This is necessary since the button window will not 
	//exist during the dialog box construction.
    std::string lb4 ;
  
    // Dialog Data
    //{{AFX_DATA(TSLUserDialog)
    enum { IDD = IDD_DIALOG };
	//Control attribute allowing access to Button 4.
    CButton	m_button4;
	//Control attribute allowing access to Button 3.
    CButton	m_button3;
	//Control attribute allowing access to Button 2.
    CButton	m_button2;
	//Control attribute allowing access to Button 1.
    CButton	m_button1;
	//Attribute containing prompt to be displayed to the user.
    CString	m_prompt;
    //}}AFX_DATA
  
  
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(TSLUserDialog)
  protected:
	//Dialog data exchange.
	//
	//Automatically generate by DevStudio Class Wizard.
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL
  
    // Implementation
  protected:
  
    // Generated message map functions
    //{{AFX_MSG(TSLUserDialog)
	//Dialog initialisation.
	//
	//Sets the button text for each button, or hides the 
	//button if it is not required.
    virtual BOOL OnInitDialog();
	//Button 1 hander.
	//
	//Exits the dialog indicating that button 1 was pressed.
    afx_msg void OnDialogButton1();
	//Button 2 hander.
	//
	//Exits the dialog indicating that button 2 was pressed.
    afx_msg void OnDialogButton2();
	//Button 3 hander.
	//
	//Exits the dialog indicating that button 3 was pressed.
    afx_msg void OnDialogButton3();
	//Button 4 hander.
	//
	//Exits the dialog indicating that button 4 was pressed.
    afx_msg void OnDialogButton4();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERDIALOG_H__781EAB61_22CD_11D2_8996_00104B556E11__INCLUDED_)
