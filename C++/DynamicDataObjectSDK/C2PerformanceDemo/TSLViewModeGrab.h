// TSLViewModeGrab.h: interface for the TSLViewModeGrab class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TSLVIEWMODEGRAB_H__2753D102_6AD1_11D5_BD4F_00104BB05CAE__INCLUDED_)
#define AFX_TSLVIEWMODEGRAB_H__2753D102_6AD1_11D5_BD4F_00104BB05CAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tslviewmode.h"

class TSLViewModeGrab : public TSLViewMode  
{
public:
	TSLViewModeGrab( TSLViewModeInterface * iViewMode, unsigned long modeID );
	virtual ~TSLViewModeGrab();

	virtual bool OnLButtonDown  ( long x, long y, bool shift, bool control ) ;
	virtual bool OnMouseMove    ( long x, long y, bool shift, bool control ) ;
	virtual bool OnLButtonUp    ( long x, long y, bool shift, bool control ) ;

protected:

	long m_lastDUX, m_lastDUY ;
	double m_lastX, m_lastY ;
	bool m_pressed ;


};

#endif // !defined(AFX_TSLVIEWMODEGRAB_H__2753D102_6AD1_11D5_BD4F_00104BB05CAE__INCLUDED_)
