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

// Include MapLink header
#include "MapLink.h"
#include "MapLinkIMode.h"

# ifndef DIRECT3D_VERSION
#  define DIRECT3D_VERSION         0x0900
# endif  //DIRECT3D_VERSION
# include <d3dx9.h>

#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d9.lib")

#include "maplinkaccelerator.h"

#include <direct.h>
#include <cmath>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <string>

#ifndef M_PI
# define M_PI 3.14159265358979323846
#endif
#ifndef TWO_PI
# define TWO_PI (2.0 * M_PI)
#endif
#ifndef DEGREES_2_RAD
# define DEGREES_2_RAD (M_PI / 180.0)
#endif
#ifndef RAD_2_DEGREES
# define RAD_2_DEGREES (180.0 / M_PI);
#endif

#define PIXEL_SPEED 5

#include "tslutf8encoder.h"
#include "tslutf8decoder.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__CF7BD0C8_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
