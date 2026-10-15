#include "stdafx.h"
#include "tsl3dpickmode.h"

TSL3DPickMode::TSL3DPickMode (long modeID,  const char* prompt ) 
  : TSL3DInteractionMode( modeID ), m_prompt( prompt )
{
}

TSL3DPickMode::~TSL3DPickMode( )
{
}

bool TSL3DPickMode::onLButtonDown(long x, long y, bool shift, bool control)
{
  const TSLPickResultSet * ps = m_drawingSurface->pick( x, y, 2, -1 ) ;
  if ( !ps )
    return false;

  int size = ps->numResults();
  if ( !size )
    return false;

  CString msg ;
  msg.Format( _T("Picked %d objects\n"), size ) ;
  for ( int i = 0 ; i < size ; ++i )
  {
    const TSLPickResultSatellite * prs = TSLPickResultSatellite::isType( (*ps)[i] ) ;
    if ( prs )
    {
      CString sat ;
      TSLUTF8Decoder satelliteName( prs->getSatellite()->satelliteName() );
      sat.Format( _T("\n  Satellite name '%s'"), (TCHAR*)satelliteName ) ;
      msg += sat ;
    }
  }
  ps->destroy() ;

  AfxMessageBox( msg, MB_OK ) ;

  return false ;
}

 TSLCursorStyle TSL3DPickMode::queryCursor ()
{
  return TSLCursorStyleQuery ;
}

const char* TSL3DPickMode::queryPrompt ()
{
  return m_prompt.c_str();
}
