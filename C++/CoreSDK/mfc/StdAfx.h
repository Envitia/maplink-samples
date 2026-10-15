// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__CF7BD0C8_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_STDAFX_H__CF7BD0C8_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

// Modify the following defines if you have to target an OS before the ones 
// specified in the following code. See MSDN for the latest information
// about corresponding values for different operating systems.
#ifndef WINVER // http://msdn.microsoft.com/en-us/library/aa383745.aspx		
#define WINVER 0x0601	// Change this to the appropriate value to target 
#endif                     

#ifndef _WIN32_WINNT	
#define _WIN32_WINNT  0x0601	// Change this to the appropriate value to target 
#endif		         			

#ifndef _WIN32_WINDOWS	      
#define _WIN32_WINDOWS 0x0601    // Change this to the appropriate value to target 
#endif			      

#ifndef _WIN32_IE		
#define _WIN32_IE 0x0600   // Change this to the appropriate value to target 
#endif			

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "MapLink.h"
#include "tslutf8decoder.h"
#include "tslutf8encoder.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__CF7BD0C8_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
