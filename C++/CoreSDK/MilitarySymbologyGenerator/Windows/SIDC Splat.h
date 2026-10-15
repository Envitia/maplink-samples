#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

class CSidcSplatApp : public CWinAppEx
{
public:
	CSidcSplatApp() noexcept;

public:
	BOOL InitInstance() override;

	BOOL  m_bHiColorIcons;

	void PreLoadState() override;
	void LoadCustomState() override;
	void SaveCustomState() override;

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CSidcSplatApp theApp;
