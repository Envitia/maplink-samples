// Frigate.h: interface for the CFrigate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FRIGATE_H__DD4CF2F7_3089_4FC6_9786_8FEA777519EB__INCLUDED_)
#define AFX_FRIGATE_H__DD4CF2F7_3089_4FC6_9786_8FEA777519EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Track.h"

class CFrigate : public CTrack  
{
public:
  CFrigate();
  CFrigate( const CTrack & other );
  virtual ~CFrigate();

  TSLDisplayObject * instantiateDO( TSLDisplayType key, int dsID = 0 ) const ;
  TRACK_TYPE         trackType() const ;   
};

#endif // !defined(AFX_FRIGATE_H__DD4CF2F7_3089_4FC6_9786_8FEA777519EB__INCLUDED_)
