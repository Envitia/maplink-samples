// MyCommandLine.cpp: implementation of the MyCommandLine class.
//
//////////////////////////////////////////////////////////////////////

#include "Stdafx_tlt.h"
#include "MyCommandLine.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void MyCommandLine::ParseParam(const TCHAR* pszParam, BOOL bFlag, BOOL bLast)
{
  if ( bFlag )
  {
   if ( 0 == _tcsicmp(pszParam, _T("help")))
    {
      m_next = Next_None;
      ::MessageBox(0, _T("Help:\n")
                      _T("  app /home path_to_install\t(The directory containing the config directory)\n")
                      , _T("Help"), MB_OK);
      m_failed = true;
    }
    else if ( 0 == _tcsicmp( pszParam, _T("home") ) )
    {
      m_isLocalMaplHome = true;
      m_next = Next_MaplHome;
    }
    // if flag is pcache then we start the pcache parameter parsing by preparing to
    // read the file name
    else if (_tcsicmp(pszParam, _T("pcache")) == 0)
    {
      m_next = Next_PCache;
    }
    else
    {
      CCommandLineInfo::ParseParam( pszParam, bFlag, bLast );
      m_next = Next_Default;
    }
  }
  else
  {
    switch ( m_next )
    {
    case Next_MaplHome:
      {
        TSLUTF8Encoder encode(pszParam);
        m_maplHome = encode;
        if ( m_maplHome == "." )
        {
          wchar_t* directory = _wgetcwd( NULL, 0 );
          TSLUTF8Encoder dirEncode( directory );
          m_maplHome = dirEncode;
          m_maplHome += "/../";
          free(directory);
        }
        if (!m_maplHome.empty())
          TSLUtilityFunctions::setMapLinkHome( m_maplHome.c_str(), true );
      }
      m_next = Next_None;
      break;

    case Next_PCache:
      // save persistent cache ini file name
      m_persistentCacheIniFile = TSLUTF8Encoder(pszParam);
      break;

    case Next_Default:
      m_next = Next_None;
      break;

    default:
      m_strFileName = pszParam;
      break;
    }
  }
}