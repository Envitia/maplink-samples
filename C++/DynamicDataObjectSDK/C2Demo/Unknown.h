// Unknown.h: interface for the CUnknown class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UNKNOWN_H__CE5DF058_4B49_4776_8A40_9B8D65D182EA__INCLUDED_)
#define AFX_UNKNOWN_H__CE5DF058_4B49_4776_8A40_9B8D65D182EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Track.h"

class CUnknown : public CTrack  
{
  public:
    CUnknown();
    CUnknown( const CTrack & other );
    virtual ~CUnknown();

    TSLDisplayObject * instantiateDO( TSLDisplayType key, int dsID = 0 ) const ;
    TRACK_TYPE         trackType() const ;       
};

#endif // !defined(AFX_UNKNOWN_H__CE5DF058_4B49_4776_8A40_9B8D65D182EA__INCLUDED_)
