// EditorSample.h : main header file for the EDITORSAMPLE application
//

#if !defined(AFX_MFC_H__CF7BD0C6_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_MFC_H__CF7BD0C6_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

//Top level application class for the LandLink Sample.
//
//Initialises the application and provides hooks for MFC 
//access.

class CEditorSampleApp : public CWinApp
{
private:
	//This attribute contains the root directory for 
	//configuration file paths.  It is the directory that 
	//contains the configuration file.  This is used in the 
	//'resolveConfigPath' method by classes that need to read 
	//configuration information.
	std::string m_rootDirectory;


public:
	//This method resolves the specified directory path.  
	//
	//If the specified path is relative, then it is prefixed 
	//with the rootDirectory.  If it is absolute, then it is 
	//passed back unchanged.
        //
	//Returns specified path, updated if relative
	const std::string resolveConfigPath(const char * path);

	//This method queryies the root directory for searches.  
	//This is  the directory containing the initialisation 
	//file.
	//
	//Returns directory containing initialisation file.
	const char * rootDirectory();

	//Constructor for the application. 
	CEditorSampleApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditorSampleApp)
	public:
	//Initialisation method for the application.
	//
	//Loads MapLink and LandLink configuration files.  Asks 
	//for user login and determines user status.
	virtual BOOL InitInstance();
	//Called when the application exits.
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CEditorSampleApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFC_H__CF7BD0C6_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
