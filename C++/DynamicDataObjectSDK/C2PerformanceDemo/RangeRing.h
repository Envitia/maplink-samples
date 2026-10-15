// RangeRing.h: interface for the CRangeRing class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RANGERING_H__FAF2F6D2_4A96_11D5_BD2F_00104BB05CAE__INCLUDED_)
#define AFX_RANGERING_H__FAF2F6D2_4A96_11D5_BD2F_00104BB05CAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Track.h"

class CRangeRing : public CTrack  
{

public:

    CRangeRing( );

    virtual ~CRangeRing ();

    void ownShipTrack( CTrack* ownShip )
    {
      m_ownShipTrack = ownShip;
    }
    
    CTrack* ownShipTrack( ) const 
    {
      return m_ownShipTrack;
    }
    
    // To update the history when we swap from vector to CADRG and vice versa
    virtual void recalculatePositionsFromLatLong( TSLDrawingSurface* ds );

    TSLDisplayObject * instantiateDO( TSLDisplayType key, int dsID = 0 ) const ;
    TRACK_TYPE         trackType() const ;     

private:
    
    CTrack * m_ownShipTrack;

};

#endif // !defined(AFX_RANGERING_H__FAF2F6D2_4A96_11D5_BD2F_00104BB05CAE__INCLUDED_)
