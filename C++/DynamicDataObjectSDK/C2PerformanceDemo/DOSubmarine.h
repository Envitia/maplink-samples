// DOSubmarine.h: interface for the CDOSubmarine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOSUBMARINE_H__8A4F9544_5B1F_11D5_BD3E_00104BB05CAE__INCLUDED_)
#define AFX_DOSUBMARINE_H__8A4F9544_5B1F_11D5_BD3E_00104BB05CAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dotrack.h"

class CDOSubmarine : public CDOTrack
{
public:
	CDOSubmarine();
	virtual ~CDOSubmarine();

	virtual void drawOutline( HDC surfaceDC ) ;

private:

	POINT m_outline[12] ;
};

#endif // !defined(AFX_DOSUBMARINE_H__8A4F9544_5B1F_11D5_BD3E_00104BB05CAE__INCLUDED_)
