// DOGeneralAir.h: interface for the CDOGeneralAir class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOGENERALAIR_H__8A4F9545_5B1F_11D5_BD3E_00104BB05CAE__INCLUDED_)
#define AFX_DOGENERALAIR_H__8A4F9545_5B1F_11D5_BD3E_00104BB05CAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dotrack.h"

class CDOGeneralAir : public CDOTrack 
{
public:
	CDOGeneralAir();
	virtual ~CDOGeneralAir();

	virtual void drawOutline( HDC surfaceDC ) ;

private:

	POINT m_outline[8] ;
};

#endif // !defined(AFX_DOGENERALAIR_H__8A4F9545_5B1F_11D5_BD3E_00104BB05CAE__INCLUDED_)
