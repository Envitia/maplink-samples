// DOAircraftCarrier.h: interface for the CDOAircraftCarrier class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOAIRCRAFTCARRIER_H__8A4F9549_5B1F_11D5_BD3E_00104BB05CAE__INCLUDED_)
#define AFX_DOAIRCRAFTCARRIER_H__8A4F9549_5B1F_11D5_BD3E_00104BB05CAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dotrack.h"

class CDOAircraftCarrier : public CDOTrack  
{
public:
	CDOAircraftCarrier();
	virtual ~CDOAircraftCarrier();

	virtual void drawOutline( HDC surfaceDC ) ;

private:

	POINT m_outline[12] ;
};

#endif // !defined(AFX_DOAIRCRAFTCARRIER_H__8A4F9549_5B1F_11D5_BD3E_00104BB05CAE__INCLUDED_)
