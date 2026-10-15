// AirCraftCarrier.h: interface for the CAirCraftCarrier class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AIRCRAFTCARRIER_H__FEB7B482_711D_4813_85B9_D03A97B4CC19__INCLUDED_)
#define AFX_AIRCRAFTCARRIER_H__FEB7B482_711D_4813_85B9_D03A97B4CC19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Track.h"

class CAircraftCarrier : public CTrack  
{
  public:
    CAircraftCarrier();
    CAircraftCarrier( const CTrack & other ) ;
    virtual ~CAircraftCarrier();

    TSLDisplayObject * instantiateDO( TSLDisplayType key, int dsID = 0 ) const ;
    TRACK_TYPE         trackType() const ; 
};

#endif // !defined(AFX_AIRCRAFTCARRIER_H__FEB7B482_711D_4813_85B9_D03A97B4CC19__INCLUDED_)
