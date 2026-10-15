// GLScatterGraph.cpp : implementation file
//

#include "stdafx.h"
#include "GLScatterGraph.h"
#include <cfloat>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



// returns the next biggest number from val set to sig significant figures
// use for top label axes
// thus 250=(247,2); 300 = (247,1)
float NextAbove(float val,int sig)
{
	float x=int(log10(fabs(val))+1)-sig;
	float mult = pow( static_cast <float> (10) ,x);
	return ceil(val/mult)*mult;
}

float NextBelow(float val,int sig)
{
	float x=int(log10(fabs(val))+1)-sig;
	float mult = pow( static_cast <float> (10) ,x );
	return floor(val/mult)*mult;
}

/*
This class draws a 3D scattergraph.
Data are passed in using
SetData(int count,COLORREF col,float *pCoords, COLORREF *pColList)
where count is the number of points
col is the default colour used, if pColList==NULL
pCoords are an array of floats count*3 long, containing the 3-D coords
pColList is an array count long, containing the colours for each point separately.

pCoords and pColList have to be maintained by the calling class, 
and must not be deleted or go out of scope while the Scattergraph exists

Axes scales are set using SetMaxX(float x) etc. Only points within the axes box are drawn.
SetAutoScaleX(true/false) etc will scale the axis to the max and min data in that dimension
SetProjection(type) sets the projection as orthogonal or perspective.
SetPtCol(..) sets the default point colour if individual point colours are not specified.
SetSymbolSize(..)specifies the size of the point.
*/


/////////////////////////////////////////////////////////////////////////////
// CGLScatterGraph

CGLScatterGraph::CGLScatterGraph()
{
	m_xMouseRotation=10.0;
	m_yMouseRotation=-15.0;
	m_ProjType=0;
	m_SymbolSize=3;

	m_MaxX=1;
	m_MaxY=1;
	m_MaxZ=1;
	m_MinX=0;
	m_MinY=0;
	m_MinZ=0;
	m_bAutoScaleX=TRUE;
	m_bAutoScaleY=TRUE;
	m_bAutoScaleZ=TRUE;
	m_pColList = 0;

	m_Count = 0;

	m_Colour=RGB(255,0,0);
}

CGLScatterGraph::~CGLScatterGraph()
{
}

void CGLScatterGraph::RenderData()
{
	int i;
	float xW=m_MaxX-m_MinX;
	float yW=m_MaxY-m_MinY;
	float zW=m_MaxZ-m_MinZ;
// need to scale data between -0.5 and 0.5 in all dimensions
	float x,y,z,r,g,b;

// NOTE: z axis goes -ve into screen, so invert z values to make
// like normal 3D graph (z gets bigger going into screen
	if (m_pColList==NULL)	// all the same colour
	{
		r=float(GetRValue(m_Colour))/255;
		g=float(GetGValue(m_Colour))/255;
		b=float(GetBValue(m_Colour))/255;
		glColor3f(r,g,b);
		glBegin(GL_POINTS);
		for (i=0; i<m_Count; i++)
		{
			if (PtWithinAxes(m_pDat[i*3],m_pDat[i*3+1],m_pDat[i*3+2]))
			{
				x=(m_pDat[i*3]-m_MinX)/xW-0.5;
				y=(m_pDat[i*3+1]-m_MinY)/yW-0.5;
				z=(m_pDat[i*3+2]-m_MinZ)/zW-0.5;
				glVertex3f(x,y,-z);
//	TRACE("real world coords in RenderData = %f, %f, %f\n",x,y,z);
			}
		}
		glEnd();
	}
	else // got separate colours for each point
	{
		COLORREF currentCol;
		i=0;
		currentCol=m_pColList[i];
		while (i<m_Count)
		{
			r=float(GetRValue(currentCol))/255;
			g=float(GetGValue(currentCol))/255;
			b=float(GetBValue(currentCol))/255;
			glColor3f(r,g,b);
			glBegin(GL_POINTS);
			while (i<m_Count)
			{
				if (m_pColList[i]!=currentCol)
				{
					currentCol=m_pColList[i];
					glEnd();
					break;
				}
				if (PtWithinAxes(m_pDat[i*3],m_pDat[i*3+1],m_pDat[i*3+2]))
				{
					x=(m_pDat[i*3]-m_MinX)/xW-0.5;
					y=(m_pDat[i*3+1]-m_MinY)/yW-0.5;
					z=(m_pDat[i*3+2]-m_MinZ)/zW-0.5;
					glVertex3f(x,y,-z);
				}
				i++;
//TRACE("real world coords in RenderData = %f, %f, %f\n",x,y,z);
			}
		}
		glEnd();
	}
	Invalidate();
}


BEGIN_MESSAGE_MAP(CGLScatterGraph, CGLMouseRotate)
	//{{AFX_MSG_MAP(CGLScatterGraph)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGLScatterGraph message handlers
void CGLScatterGraph::OnDrawGL()
{
#ifdef _DEBUG
	int rv;
	glGetIntegerv(GL_RENDER_MODE,&rv);
	CString mode;
	if (rv==GL_RENDER)
		mode="Render";
	else if (rv==GL_FEEDBACK)
		mode="Feedback";
	else
		mode="Unknown";
TRACE("In CGLScatterGraph::OnDrawGL, mode = %s\n",mode);
#endif	

	glPushMatrix();	// this saves the modelmatrix settings

	DoMouseRotate();
	m_GraphBox.Draw();	// draw box from display list constructed in OnCreate
	glPointSize(m_SymbolSize);
	RenderData();

	glPopMatrix();	// this restores the modelmatrix settings to before translation etc
}

void CGLScatterGraph::OnDrawGDI(CPaintDC *pDC)
{
TRACE("In CGLScatterGraph::OnDrawGDI\n");	
	CRect r;
	GetClientRect(&r);
	CPen pen(PS_SOLID,0,RGB(255,255,255));
	CPen *oP=pDC->SelectObject(&pen);
//	pDC->MoveTo(r.TopLeft());
//	pDC->LineTo(r.BottomRight());
	pDC->SelectObject(oP);
	;
}
void CGLScatterGraph::OnSizeGL(int cx, int cy)
{
// set correspondence between window and OGL viewport
		glViewport(0,0,cx,cy);

		SetProjection(m_ProjType);
#if 0
// update the camera
 		glPushMatrix();
			glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				if (m_ProjType==0)	// ortho
					glOrtho(-1,1,-1,1,2.0,10.);	// need BIGGER Frustum for ortho
				else
					gluPerspective(25,GetAspectRatio(),1,15.0);
//		glFrustum(-0.5,0.5,-0.5,0.5,2.0,10.);	
				glTranslatef(0.0f,0.0f,-4.f);
			glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
#endif
}

void CGLScatterGraph::OnCreateGL()
{
// following could also be set by calling COpenGLWnd::OnCreateGL();
// perform hidden line/surface removal (enabling Z-Buffer)
	glEnable(GL_DEPTH_TEST);
// set background color to black
	glClearColor(0.f,0.f,0.f,1.0f );
// set clear Z-Buffer value
	glClearDepth(1.0f);

// specific to scattergraph
	MakeFont();
//	RasterFont();	// call to instantiate font bitmaps (maybe should be in base class?)
// make a graph box, since we will need one for all time
	m_GraphBox.StartDef();// <- do not execute list immediately
		glColor3f(0.7f, 0.7f, 0.7f);
		CPoint3D pt;
		CPoint3D orgPt(-0.5,-0.5,0.5);
		glBegin(GL_LINES);
// main X
		pt=orgPt;
		pt.Translate(-0.1f,0.0f,0.0f);
		glVertex3f(pt.x,pt.y,pt.z);
		pt.Translate(1.2f,0.0f,0.0f);
		glVertex3f(pt.x,pt.y,pt.z);
// main Y
		pt=orgPt;
		pt.Translate(0.0f,-0.1f,0.0f);
		glVertex3f(pt.x,pt.y,pt.z);
		pt.Translate(0.0f,1.2f,0.0f);
		glVertex3f(pt.x,pt.y,pt.z);
// main Z
		pt=orgPt;
		pt.Translate(0.0f,0.0f,0.1f);
		glVertex3f(pt.x,pt.y,pt.z);
		pt.Translate(0.0f,0.0f,-1.2f);
		glVertex3f(pt.x,pt.y,pt.z);
		glEnd();

		glLineStipple(2,0xAAAA);
		glEnable(GL_LINE_STIPPLE);
		glBegin(GL_LINE_STRIP);
		pt=orgPt;
		pt.Translate(0.0f,1.0f,0.0f);
		glVertex3f(pt.x,pt.y,pt.z);
		pt.Translate(0.0f,0.0f,-1.0f);
		glVertex3f(pt.x,pt.y,pt.z);
		pt.Translate(0.0f,-1.0f,0.0f);
		glVertex3f(pt.x,pt.y,pt.z);
		glEnd();
		glBegin(GL_LINE_STRIP);
		pt=orgPt;
		pt.Translate(1.0f,0.0f,0.0f);
		glVertex3f(pt.x,pt.y,pt.z);
		pt.Translate(0.0f,0.0f,-1.0f);
		glVertex3f(pt.x,pt.y,pt.z);
		pt.Translate(-1.0f,0.0f,0.0f);
		glVertex3f(pt.x,pt.y,pt.z);
		glEnd();
		glBegin(GL_LINE_STRIP);
		pt=orgPt;
		pt.Translate(0.0f,1.0f,0.0f);
		glVertex3f(pt.x,pt.y,pt.z);
		pt.Translate(1.0f,0.0f,0.0f);
		glVertex3f(pt.x,pt.y,pt.z);
		pt.Translate(0.0f,-1.0f,0.0f);
		glVertex3f(pt.x,pt.y,pt.z);
		glEnd();
		glBegin(GL_LINE_STRIP);
		pt=orgPt;
		pt.Translate(1.0f,1.0f,0.0f);
		glVertex3f(pt.x,pt.y,pt.z);
		pt.Translate(0.0f,0.0f,-1.0f);
		glVertex3f(pt.x,pt.y,pt.z);
		pt.Translate(0.0f,-1.0f,0.0f);
		glVertex3f(pt.x,pt.y,pt.z);
		glEnd();
		glBegin(GL_LINES);
		pt=orgPt;
		pt.Translate(0.0f,1.0f,-1.0f);
		glVertex3f(pt.x,pt.y,pt.z);
		pt.Translate(1.0f,0.0f,0.0f);
		glVertex3f(pt.x,pt.y,pt.z);
		glEnd();
		glDisable(GL_LINE_STIPPLE);

/*
		CPoint3D orgPt(-0.5,-0.5,0.5);
		DrawBox(orgPt,1,1,-1);

		CPoint3D endPt;
		glBegin(GL_LINES);
	// x axis
	//	glColor3f(1.0f, 0.0f, 0.0f);
		endPt=orgPt;
		endPt.Translate(1,0,0);
		glVertex3f(endPt.x,endPt.y,endPt.z);
		endPt.Translate(0.1,0,0);
		glVertex3f(endPt.x,endPt.y,endPt.z);
	// y axis
	//	glColor3f(1.0f, 1.0f, 0.0f);
		endPt=orgPt;
		endPt.Translate(0,1,0);
		glVertex3f(endPt.x,endPt.y,endPt.z);
		endPt.Translate(0,0.1,0);
		glVertex3f(endPt.x,endPt.y,endPt.z);
	// z axis
	//	glColor3f(0.0f, 0.0f, 1.0f);
		endPt=orgPt;
		endPt.Translate(0,0,-1);
		glVertex3f(endPt.x,endPt.y,endPt.z);
		endPt.Translate(0,0,-0.1);
		glVertex3f(endPt.x,endPt.y,endPt.z);
	// origin 
		glVertex3f(orgPt.x, orgPt.y, orgPt.z);
		glVertex3f(orgPt.x-0.1, orgPt.y, orgPt.z);
		glVertex3f(orgPt.x, orgPt.y, orgPt.z);
		glVertex3f(orgPt.x, orgPt.y-0.1, orgPt.z);
		glVertex3f(orgPt.x, orgPt.y, orgPt.z);
		glVertex3f(orgPt.x, orgPt.y, orgPt.z+0.1);
		glEnd();
*/		

		pt=orgPt;
		float endAx=1.13f;
		pt.Translate(endAx,0,0);
		glRasterPos3f(pt.x,pt.y,pt.z);
		PrintString("Longitude");
		pt=orgPt;
		pt.Translate(0,endAx,0);
		glRasterPos3f(pt.x,pt.y,pt.z);
		PrintString("Height/Depth");
		pt=orgPt;
		pt.Translate(0,0,-endAx);
		glRasterPos3f(pt.x,pt.y,pt.z);
		PrintString("Latitude");
	m_GraphBox.EndDef();

	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
//			glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
}

void CGLScatterGraph::SetProjection(int type)
{
	m_ProjType=type;
	BeginGLCommands();
 	glPushMatrix();
		glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			if (m_ProjType==0)	// ortho
				glOrtho(-1,1,-1,1,2.0,10.);	// need BIGGER Frustum for ortho
			else
				gluPerspective(25,GetAspectRatio(),1,15.0);
//		glFrustum(-0.5,0.5,-0.5,0.5,2.0,10.);	
			glTranslatef(0.0f,0.0f,-4.f);
		glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	EndGLCommands();
}

void CGLScatterGraph::SetData(int count,COLORREF col,float *pCoords, COLORREF *pColList)
{
	m_Count=count;
	m_Colour=col;
	m_pDat=pCoords;
	m_pColList=pColList;
	AutoScale();
}

void CGLScatterGraph::AutoScale(int *pDoList)
{
	int i;
// find max & min
	float xMax=-FLT_MAX;
	float xMin=FLT_MAX;
	float yMax=-FLT_MAX;
	float yMin=FLT_MAX;
	float zMax=-FLT_MAX;
	float zMin=FLT_MAX;

	if (m_bAutoScaleX || m_bAutoScaleY || m_bAutoScaleZ)
	{
		for (i=0; i<m_Count; i++)
		{
			if (pDoList!=NULL && pDoList[i]==0)
				continue;
			xMax=max(m_pDat[i*3],xMax);
			xMin=min(m_pDat[i*3],xMin);
			yMax=max(m_pDat[i*3+1],yMax);
			yMin=min(m_pDat[i*3+1],yMin);
			zMax=max(m_pDat[i*3+2],zMax);
			zMin=min(m_pDat[i*3+2],zMin);
		}
		if (m_bAutoScaleX)
		{
			m_MaxX=NextAbove(xMax,5);
			m_MinX=NextBelow(xMin,5);
		}
		if (m_bAutoScaleY)
		{
			m_MaxY=NextAbove(yMax,5);
			m_MinY=NextBelow(yMin,5);
		}
		if (m_bAutoScaleZ)
		{
			m_MaxZ=NextAbove(zMax,5);
			m_MinZ=NextBelow(zMin,5);
		}
	}
	Invalidate();
}

BOOL CGLScatterGraph::PtWithinAxes(float x,float y,float z)
{
	if (x>=m_MinX && x<=m_MaxX &&
		y>=m_MinY && y<=m_MaxY &&
		z>=m_MinZ && z<=m_MaxZ)
		return TRUE;
	return FALSE;
}
