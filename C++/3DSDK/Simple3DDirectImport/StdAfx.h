////////////////////////////////////////////////////////
// Copyright (c) 1998 to 2010 by Envitia Group PLC
////////////////////////////////////////////////////////
// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__CF7BD0C8_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_STDAFX_H__CF7BD0C8_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define WINVER 0x0601

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include "afxmt.h"

#include "MapLink.h"
#include "MapLink3D.h"
#include "MapLink3DIMode.h"

#include <map>
#include <string>
#include <vector>
using namespace std;

#define WM_CAPABILITIES_LOADED    (WM_USER+1)
#define WM_CAPABILITIES_LOAD_FAIL (WM_USER+2)
#define WM_CAPABILITIES_COMPLETE  (WM_USER+3)
#define WM_SHOW_DS_DLG            (WM_USER+4)
#define WM_NO_COMMON_CRS          (WM_USER+5)
#define WM_NO_SUPPORTED_CRS       (WM_USER+6)
#define WM_NEW_TILES_AVAILABLE    (WM_USER+7)

#include "tslutf8encoder.h"
#include "tslutf8decoder.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__CF7BD0C8_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
