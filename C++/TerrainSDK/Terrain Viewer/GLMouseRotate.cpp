// GLMouseRotate.cpp : implementation file
//

#include "stdafx.h"
#include "GLMouseRotate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
This class is a simple extension that allows the mouse to rotate the
image.
*/
/////////////////////////////////////////////////////////////////////////////
// CGLMouseRotate

CGLMouseRotate::CGLMouseRotate()
{
	m_bAllowMouseRotate=TRUE;
	m_bInMouseRotate=FALSE;

	m_xMouseRotation = 0.0f;
	m_yMouseRotation = 0.0f;

}

CGLMouseRotate::~CGLMouseRotate()
{
}


BEGIN_MESSAGE_MAP(CGLMouseRotate, COpenGLWnd)
	//{{AFX_MSG_MAP(CGLMouseRotate)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGLMouseRotate message handlers

void CGLMouseRotate::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_bAllowMouseRotate)
	{
		m_LeftDownPos = point;
		SetCapture();
		::SetCursor(LoadCursor(NULL, IDC_SIZEALL));
// NOTE: we need a flag, can't just check whether got capture, 
// since capture might be set in a derived class for another reason
		m_bInMouseRotate=TRUE;
	}
	COpenGLWnd::OnLButtonDown(nFlags, point);
}

void CGLMouseRotate::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_LeftDownPos = CPoint(0,0);	// forget where we clicked
	SetMouseCursor(	AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	ReleaseCapture();
	m_bInMouseRotate=FALSE;
	
	COpenGLWnd::OnLButtonUp(nFlags, point);
}

void CGLMouseRotate::OnMouseMove(UINT nFlags, CPoint point) 
{
// check if we are in mouse rotate
// can't just check if got capture, because might be captured in derived class for another reason
	if (m_bInMouseRotate)
	{
		ASSERT(GetCapture()==this);
		::SetCursor(LoadCursor(NULL, IDC_SIZEALL));
		m_yMouseRotation -= (float)(m_LeftDownPos.x - point.x)/3.0f;
		m_xMouseRotation -= (float)(m_LeftDownPos.y - point.y)/3.0f;
		m_LeftDownPos = point;
		InvalidateRect(NULL,FALSE);
	}
	
	COpenGLWnd::OnMouseMove(nFlags, point);
}

void CGLMouseRotate::DoMouseRotate()
{
	glRotatef(m_xMouseRotation, 1.0, 0.0, 0.0);
	glRotatef(m_yMouseRotation, 0.0, 1.0, 0.0);
}

