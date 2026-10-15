// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__18CA2575_13A7_11D5_BD3A_00C04FA1120D__INCLUDED_)
#define AFX_STDAFX_H__18CA2575_13A7_11D5_BD3A_00C04FA1120D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _ATL_PROP_ENTRY_NO_WARNING

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT  0x0601
#endif
#define _ATL_APARTMENT_THREADED

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>
#include <atlctl.h>

#include <comdef.h>

#include "MapLink.h"

#include <string>
#include <vector>

const int AM_LOADER_CALLBACK = WM_APP + 1;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__18CA2575_13A7_11D5_BD3A_00C04FA1120D__INCLUDED)
