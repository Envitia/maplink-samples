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

#include <afxwin.h>         // OverlayOpacity core and standard components
#include <afxext.h>         // OverlayOpacity extensions
#include <afxdisp.h>        // OverlayOpacity Automation classes
#include <afxdtctl.h>		// OverlayOpacity support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// OverlayOpacity support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "MapLink.h"
#include "tslutf8decoder.h"
#include "tslutf8encoder.h"

#include <vector>
#include <string>

using std::string;
using std::vector;

#include "tslviewmodeinterface.h"
#include "tslviewmodeinterfacent.h"
#include "tslviewmode.h"
#include "tslviewmodepan.h"
#include "tslviewmodezoom.h"
#include "tslviewmodegrab.h"
#include "tslviewmodezoompan.h"

#include <math.h>
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


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__CF7BD0C8_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
