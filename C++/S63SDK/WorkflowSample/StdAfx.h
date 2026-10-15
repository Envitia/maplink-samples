// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__CF7BD0C8_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_STDAFX_H__CF7BD0C8_4693_11D3_8F16_0020AFC8854E__INCLUDED_
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#pragma warning(disable : 4189)
#pragma warning(disable : 4800)
#pragma warning(disable : 4706)

#define WINVER 0x0601

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#ifdef UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#else
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#ifdef WINNT
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>
#endif

#include "MapLink.h"
#include "MapLinkIMode.h"
#include "MapLinkS63.h"
#include "MapLinkS52.h"
#include "MapLinkOpenGLSurface.h"

#include "tslutf8encoder.h"
#include "tslutf8decoder.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#include <string>
#include <vector>
#include <map>
#include <list>
#include <fstream>
#include <sys/stat.h> 
#include <direct.h>

using namespace std;

// layer names
#define NO_DATA_LAYER_NAME "S52_NO_DAT"
#define MAP_LAYER_NAME     "map"
#define S63_LAYER_NAME     "s63"

// Map cache size
#define CACHE_SIZE         500

// initial zoom to data scale.
#define INITIAL_SCALE      3225524

// Default Scale band values
#define OVERVIEW_LOWER 1500000.0
#define OVERVIEW_UPPER 999999999.0
#define GENERAL_LOWER 350000.0
#define GENERAL_UPPER OVERVIEW_LOWER
#define COASTAL_LOWER 100000.0
#define COASTAL_UPPER GENERAL_LOWER
#define APPROACH_LOWER 20000.0
#define APPROACH_UPPER COASTAL_LOWER
#define HARBOUR_LOWER 4000.0
#define HARBOUR_UPPER APPROACH_LOWER
#define BERTHING_LOWER 1.0
#define BERTHING_UPPER HARBOUR_LOWER

// TMC per Map Units - value for world wide coverage with ~ 2cm accuracy.
#define TMC_PER_MU 50.0

// log methods
extern void addLogLine(const char* line);
extern void addLogLine(const std::string &line);
extern bool logFile(const std::string &logFile);

// Finds the MapLink Configuration directory.
inline bool getMapLinkConfigDirectory(string &configDir)
{
  const char *maplHome = TSLUtilityFunctions::getMapLinkHome();
 
  if (maplHome)
  {
    // We have a MapLink installation so see if the S52 configuration directory
    // exists.
    string dir(maplHome);
    dir += "/config";

    string mapLinkConfigDir = dir + "/s52/";

    if (TSLFileHelper::directoryExists(mapLinkConfigDir.c_str()))
    {
      configDir = dir;
      return true;
    }
  }
  // MapLink not installed or the S52 configuration does not exist.
  // see if we can find the config directory relative to where we were run from.
  wchar_t* currentWorkingDirectoryW = _wgetcwd( NULL, 0 );
  TSLUTF8Encoder currentWorkingDirectory( currentWorkingDirectoryW );
  free( currentWorkingDirectoryW );

  string dir(currentWorkingDirectory);
  string mapLinkConfigDir = dir + "/../config/s52/";
  if (TSLFileHelper::directoryExists(mapLinkConfigDir.c_str()))
  {
    configDir = dir + "/../config/";
    return true;
  }
  configDir = "";
  return false;
}

#endif // !defined(AFX_STDAFX_H__CF7BD0C8_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
