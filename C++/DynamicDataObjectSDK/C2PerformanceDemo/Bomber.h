// Bomber.h: interface for the CBomber class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BOMBER_H__EDE274A9_87AA_4853_8DAF_FC87330BB834__INCLUDED_)
#define AFX_BOMBER_H__EDE274A9_87AA_4853_8DAF_FC87330BB834__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Track.h"

class CBomber : public CTrack  
{
  public:
    CBomber();
    CBomber( const CTrack & other );
    virtual ~CBomber();

    TSLDisplayObject * instantiateDO( TSLDisplayType key, int dsID = 0 ) const ;
    TRACK_TYPE         trackType() const ; 

};

#endif // !defined(AFX_BOMBER_H__EDE274A9_87AA_4853_8DAF_FC87330BB834__INCLUDED_)
