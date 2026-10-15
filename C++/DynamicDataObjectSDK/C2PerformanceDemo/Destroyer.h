// Destroyer.h: interface for the CDestroyer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DESTROYER_H__3E01FFBD_24A6_49A7_AFFD_3D03DBF8F4B3__INCLUDED_)
#define AFX_DESTROYER_H__3E01FFBD_24A6_49A7_AFFD_3D03DBF8F4B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "track.h"

class CDestroyer : public CTrack  
{
  public:
    CDestroyer();
    CDestroyer( const CTrack & other );
    virtual ~CDestroyer();

    TSLDisplayObject * instantiateDO( TSLDisplayType key, int dsID = 0 ) const ;
    TRACK_TYPE         trackType() const ;   
};

#endif // !defined(AFX_DESTROYER_H__3E01FFBD_24A6_49A7_AFFD_3D03DBF8F4B3__INCLUDED_)
