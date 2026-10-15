// DOBomber.h: interface for the CDOBomber class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOBOMBER_H__8A4F9546_5B1F_11D5_BD3E_00104BB05CAE__INCLUDED_)
#define AFX_DOBOMBER_H__8A4F9546_5B1F_11D5_BD3E_00104BB05CAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dotrack.h"

class CDOBomber : public CDOTrack  
{
public:
	CDOBomber();
	virtual ~CDOBomber();
	
	virtual void drawOutline( HDC surfaceDC ) ;

private:

	POINT m_outline[12] ;
};

#endif // !defined(AFX_DOBOMBER_H__8A4F9546_5B1F_11D5_BD3E_00104BB05CAE__INCLUDED_)
