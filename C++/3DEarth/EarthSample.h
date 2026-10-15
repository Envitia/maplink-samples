
// EarthSample.h : main header file for the EarthSample application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CEarthSampleApp:
// See EarthSample.cpp for the implementation of this class
//

class CEarthSampleApp : public CWinApp
{
public:
	CEarthSampleApp();

protected:
	CSingleDocTemplate* m_pDocTemplate;
public:

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CEarthSampleApp theApp;
