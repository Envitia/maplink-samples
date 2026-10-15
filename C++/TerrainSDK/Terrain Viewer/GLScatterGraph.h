#if !defined(AFX_GLSCATTERGRAPH_H__821AAEEA_B0C6_45D6_87B8_98000CA2F0A6__INCLUDED_)
#define AFX_GLSCATTERGRAPH_H__821AAEEA_B0C6_45D6_87B8_98000CA2F0A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GLScatterGraph.h : header file
//
#include "glMouseRotate.h"

extern float NextAbove(float val,int sig);
extern float NextBelow(float val,int sig);

/////////////////////////////////////////////////////////////////////////////
// CGLScatterGraph window

class CGLScatterGraph : public CGLMouseRotate
{
// Construction
public:
	CGLScatterGraph();

// Attributes
public:

protected:
	int m_ProjType;	// 0 = ortho, 1 = perspective

	float m_MaxX;
	float m_MaxY;
	float m_MaxZ;
	float m_MinX;
	float m_MinY;
	float m_MinZ;
	BOOL m_bAutoScaleX;
	BOOL m_bAutoScaleY;
	BOOL m_bAutoScaleZ;

	int m_Count;
	float *m_pDat;
	COLORREF m_Colour;
	COLORREF *m_pColList;
// Operations
public:
	void SetPtCol(COLORREF col) {m_Colour=col; Invalidate();}
	CGLDispList m_GraphBox;

/* 
count = number of points
col = colour of points, used if pColList=NULL
pCoords = x,y,z coords of each point, lenght = count*3
pColList = pointer to count COLORREFs, each point has that colour if not NULL
*/
	void SetData(int count,COLORREF col,float *pCoords, COLORREF *pColList=NULL);
	void SetMaxX(float x) {m_MaxX=x; Invalidate();}
	void SetMaxY(float y) {m_MaxY=y;Invalidate();}
	void SetMaxZ(float z) {m_MaxZ=z;Invalidate();}
	void SetMinX(float x) {m_MinX=x;Invalidate();}
	void SetMinY(float y) {m_MinY=y;Invalidate();}
	void SetMinZ(float z) {m_MinZ=z;Invalidate();}
	float GetMaxX() {return m_MaxX;}
	float GetMaxY() {return m_MaxY;}
	float GetMaxZ() {return m_MaxZ;}
	float GetMinX() {return m_MinX;}
	float GetMinY() {return m_MinY;}
	float GetMinZ() {return m_MinZ;}
	void SetAutoScaleX(BOOL scl) {m_bAutoScaleX=scl; AutoScale();}
	void SetAutoScaleY(BOOL scl) {m_bAutoScaleY=scl; AutoScale();}
	void SetAutoScaleZ(BOOL scl) {m_bAutoScaleZ=scl; AutoScale();}

	void RenderData();

	void SetProjection(int type);
	void SetSymbolSize(float size) {m_SymbolSize=size; Invalidate();}

	virtual void OnCreateGL(); // override to set bg color, activate z-buffer, and other global settings
	virtual void OnSizeGL(int cx, int cy); // override to adapt the viewport to the window
	virtual void OnDrawGL(); // override to issue drawing functions
	virtual void OnDrawGDI(CPaintDC *pDC); // override to issue GDI drawing functions

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGLScatterGraph)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGLScatterGraph();

	// Generated message map functions
protected:
	BOOL PtWithinAxes(float x,float y,float z);
	void AutoScale(int *pDoList=NULL);
	float m_SymbolSize;
	//{{AFX_MSG(CGLScatterGraph)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GLSCATTERGRAPH_H__821AAEEA_B0C6_45D6_87B8_98000CA2F0A6__INCLUDED_)
