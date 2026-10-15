// DOHelicopter.h: interface for the CDOHelicopter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOHELICOPTER_H__8A4F9548_5B1F_11D5_BD3E_00104BB05CAE__INCLUDED_)
#define AFX_DOHELICOPTER_H__8A4F9548_5B1F_11D5_BD3E_00104BB05CAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dotrack.h"

class CDOHelicopter : public CDOTrack  
{
public:
	CDOHelicopter();
	virtual ~CDOHelicopter();

	virtual void drawOutline( HDC surfaceDC ) ;

private:

	POINT m_outline[5] ;
};

#endif // !defined(AFX_DOHELICOPTER_H__8A4F9548_5B1F_11D5_BD3E_00104BB05CAE__INCLUDED_)
