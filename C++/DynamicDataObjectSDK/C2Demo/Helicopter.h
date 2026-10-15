// Helicopter.h: interface for the CHelicopter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HELICOPTER_H__D61627D4_5972_4A29_AA4C_C4D21B740D3B__INCLUDED_)
#define AFX_HELICOPTER_H__D61627D4_5972_4A29_AA4C_C4D21B740D3B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Track.h"

class CHelicopter : public CTrack  
{
public:
  CHelicopter();
  CHelicopter( const CTrack & other );
  virtual ~CHelicopter();

  TSLDisplayObject * instantiateDO( TSLDisplayType key, int dsID = 0 ) const ;
  TRACK_TYPE         trackType() const ;     
};

#endif // !defined(AFX_HELICOPTER_H__D61627D4_5972_4A29_AA4C_C4D21B740D3B__INCLUDED_)
