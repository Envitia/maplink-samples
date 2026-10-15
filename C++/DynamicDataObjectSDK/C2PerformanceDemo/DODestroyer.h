// DODestroyer.h: interface for the CDODestroyer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DODESTROYER_H__8A4F954A_5B1F_11D5_BD3E_00104BB05CAE__INCLUDED_)
#define AFX_DODESTROYER_H__8A4F954A_5B1F_11D5_BD3E_00104BB05CAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dotrack.h"

class CDODestroyer : public CDOTrack  
{
public:
	CDODestroyer();
	virtual ~CDODestroyer();

	virtual void drawOutline( HDC surfaceDC ) ;

private:

	POINT m_outline[4] ;
};

#endif // !defined(AFX_DODESTROYER_H__8A4F954A_5B1F_11D5_BD3E_00104BB05CAE__INCLUDED_)
