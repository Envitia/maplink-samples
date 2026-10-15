// TSLViewModeSelect.h: interface for the TSLViewModeSelect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TSLVIEWMODESELECT_H__C5E20404_4DC1_11D5_BD31_00104BB05CAE__INCLUDED_)
#define AFX_TSLVIEWMODESELECT_H__C5E20404_4DC1_11D5_BD31_00104BB05CAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tslviewmode.h"

class COwnShipTrack;

class TSLViewModeSelect : public TSLViewMode  
{
public:
	TSLViewModeSelect( TSLViewModeInterface * iViewMode, unsigned long modeID, const char * objectLayerName );
	virtual ~TSLViewModeSelect();

	virtual bool OnLButtonDown  ( long x, long y, bool shift, bool control ) ;
	virtual bool OnLButtonUp    ( long x, long y, bool shift, bool control ) ;
	virtual bool OnMouseMove    ( long x, long y, bool shift, bool control ) ;

protected:
	bool m_pressed ;	    // Set in OnLButtonDown
	bool m_dragging ;	    // Set in OnMouseMove
	
	long m_startX, m_startY ;   // Used to remember the where the mouse was before a possible drag action

	long m_originalID ;	    // Temporary store for the old m_id when a drag operation is started
	
	COwnShipTrack * m_ownShipTrack ;
	char * m_objectLayerName ;

};

#endif // !defined(AFX_TSLVIEWMODESELECT_H__C5E20404_4DC1_11D5_BD31_00104BB05CAE__INCLUDED_)
