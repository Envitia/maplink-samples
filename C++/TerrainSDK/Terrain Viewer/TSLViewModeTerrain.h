// TSLViewModeTerrain.h: interface for the TSLViewModeTerrain class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TSLVIEWMODETERRAIN_H__051C52EB_4B84_11D5_93AF_00C04FA10B1D__INCLUDED_)
#define AFX_TSLVIEWMODETERRAIN_H__051C52EB_4B84_11D5_93AF_00C04FA10B1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTerrainView ;
class CTerrainViewerDoc ;

#include "tslviewmode.h"

class TSLViewModeTerrain : public TSLViewMode 
{
public:
	TSLViewModeTerrain( TSLViewModeInterface * iViewMode, unsigned long modeID, CTerrainView * pTerrainView, CTerrainViewerDoc * pDoc );
	virtual ~TSLViewModeTerrain();
    virtual bool OnLButtonDown( long x, long y, bool shift, bool control );
	virtual bool OnMouseMove(long x, long y, bool shift, bool control) ;
	virtual bool OnLButtonUp(long x, long y, bool shift, bool control) ;

private:
	CTerrainView * m_terrain_view;
    long   m_startX ;
    long   m_startY ;
    double m_start_mux  ;
    double m_start_muy  ;
    bool   m_pressed ;
    double m_percentage ;
    CTerrainViewerDoc * m_doc ;
};

#endif // !defined(AFX_TSLVIEWMODETERRAIN_H__051C52EB_4B84_11D5_93AF_00C04FA10B1D__INCLUDED_)
