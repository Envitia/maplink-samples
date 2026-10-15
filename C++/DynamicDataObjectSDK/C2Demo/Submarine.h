// Submarine.h: interface for the CSubmarine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUBMARINE_H__003A44FF_D52B_4CB4_BACB_0F07A82ABC6B__INCLUDED_)
#define AFX_SUBMARINE_H__003A44FF_D52B_4CB4_BACB_0F07A82ABC6B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Track.h"

class CSubmarine : public CTrack  
{
public:
  CSubmarine();
  CSubmarine( const CTrack & other );
  virtual ~CSubmarine();

  TSLDisplayObject * instantiateDO( TSLDisplayType key, int dsID = 0 ) const ;
  TRACK_TYPE         trackType() const ;      
};

#endif // !defined(AFX_SUBMARINE_H__003A44FF_D52B_4CB4_BACB_0F07A82ABC6B__INCLUDED_)
