// DOOwnShip.h: interface for the CDOOwnShip class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOOWNSHIP_H__8A4F954C_5B1F_11D5_BD3E_00104BB05CAE__INCLUDED_)
#define AFX_DOOWNSHIP_H__8A4F954C_5B1F_11D5_BD3E_00104BB05CAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DOTrack.h"

class CDOOwnShip : public CDOTrack  
{
public:
  CDOOwnShip();
  virtual ~CDOOwnShip();

  virtual bool drawOutline( HDC surfaceDC );

  virtual bool showAsSelectedTrack( HDC surfaceDC ) ;

};

#endif // !defined(AFX_DOOWNSHIP_H__8A4F954C_5B1F_11D5_BD3E_00104BB05CAE__INCLUDED_)
