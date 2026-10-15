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

#define _CRT_NON_CONFORMING_SWPRINTFS
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include "afxmt.h"

#include "resource.h"
#include "MapLink.h"
#include "MapLinkIMode.h"
#include "MapLinkWMTSDataLayer.h"

#include <map>
#include <string>
#include <vector>
using namespace std;

#define WM_NEW_TILES_AVAILABLE       (WM_USER+1)
#define WM_WMS_MESSAGE_BASE          (WM_USER+20)
#define WM_WMTS_MESSAGE_BASE         (WM_USER+40)


#define MRU_URL_APPSETTING          _T("Recent URL List")
#define MRU_FMD_APPSETTING          _T("Recent Federation Metadata")

#define OPTIONS_APPSETTING_SECTION  _T("Options")
#define TILE_CACHE_SIZE_APPSETTING  _T("TileCacheSize")
#define TILE_LEVELS_APPSETTING      _T("TileLevels")
#define TILE_LOAD_ORDER_APPSETTING  _T("TileLoadOrder")
#define P_SCALE_FACTOR_APPSETTING   _T("PyramidScaleFactor")
#define UNKNOWN_CA_APPSETTING       _T("UnknownCA")
#define CERT_CN_INVALID_APPSETTING  _T("CertCNInvalid")
#define INITIAL_VERSION_APPSETTING  _T("InitialWMSVersion")
#define NUMBER_OF_THREADS_APPSETTING _T("NumberOfThreads")

class CExtentedBitmapButton : public CBitmapButton
{
public:
  BOOL SetBitmaps(HBITMAP pBitmap,
                  HBITMAP pBitmapSel = 0,
                  HBITMAP pBitmapFocus = 0,
                  HBITMAP pBitmapDisabled = 0)
  {
    // delete old bitmaps (if present)
    m_bitmap.DeleteObject();
    m_bitmapSel.DeleteObject();
    m_bitmapFocus.DeleteObject();
    m_bitmapDisabled.DeleteObject();
    m_bitmap.m_hObject = pBitmap;
    m_bitmapSel.m_hObject = pBitmapSel;
    m_bitmapFocus.m_hObject = pBitmapFocus;
    m_bitmapDisabled.m_hObject = pBitmapDisabled;

    return TRUE;
  }
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__CF7BD0C8_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
