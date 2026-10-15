// GeneralAir.h: interface for the CGeneralAir class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GENERALAIR_H__6870FB25_886D_4921_8F1E_894C9F4A04FE__INCLUDED_)
#define AFX_GENERALAIR_H__6870FB25_886D_4921_8F1E_894C9F4A04FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Track.h"

class CGeneralAir : public CTrack  
{
public:
  CGeneralAir();
  CGeneralAir( const CTrack & other );
  virtual ~CGeneralAir();

  TSLDisplayObject * instantiateDO( TSLDisplayType key, int dsID = 0 ) const ;
  TRACK_TYPE         trackType() const ;   
};

#endif // !defined(AFX_GENERALAIR_H__6870FB25_886D_4921_8F1E_894C9F4A04FE__INCLUDED_)
