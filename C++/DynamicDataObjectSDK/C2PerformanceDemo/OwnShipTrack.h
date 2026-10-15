// OwnShipTrack.h: interface for the COwnShipTrack class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OWNSHIPTRACK_H__67E4AB8A_490B_11D5_BD2D_00104BB05CAE__INCLUDED_)
#define AFX_OWNSHIPTRACK_H__67E4AB8A_490B_11D5_BD2D_00104BB05CAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Track.h"

class COwnShipTrack : public CTrack  
{
public:
  COwnShipTrack();
  virtual ~COwnShipTrack();
  
  virtual TSLDisplayObject * instantiateDO( TSLDisplayType key, int dsID = 0 ) const ;
  
  virtual void translate( TSLDataLayer * pLayer ) ;  // Move the Track, update the headings and history
  TRACK_TYPE         trackType() const ;      
  
};

#endif // !defined(AFX_OWNSHIPTRACK_H__67E4AB8A_490B_11D5_BD2D_00104BB05CAE__INCLUDED_)
