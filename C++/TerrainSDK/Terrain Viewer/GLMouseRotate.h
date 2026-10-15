#if !defined(AFX_GLMOUSEROTATE_H__D0F03570_94E6_4182_BA37_467E8CF91B1A__INCLUDED_)
#define AFX_GLMOUSEROTATE_H__D0F03570_94E6_4182_BA37_467E8CF91B1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GLMouseRotate.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGLMouseRotate window
#include "OpenGlWnd.h"
class CGLMouseRotate : public COpenGLWnd
{
// Construction
public:
	CGLMouseRotate();

// Attributes
public:

protected:
	BOOL m_bAllowMouseRotate;
	void DoMouseRotate();
	float m_xMouseRotation;
	float m_yMouseRotation;

private:
	BOOL m_bInMouseRotate;
	CPoint m_LeftDownPos;
	
// Operations
public:
// Overridables
//	virtual void OnCreateGL(); // override to set bg color, activate z-buffer, and other global settings
//	virtual void OnSizeGL(int cx, int cy); // override to adapt the viewport to the window
//	virtual void OnDrawGL(); // override to issue drawing functions

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGLMouseRotate)
	//}}AFX_VIRTUAL

// Implementation
public:
	void AllowMouseRotate(BOOL bAllow) {m_bAllowMouseRotate=bAllow;}
	virtual ~CGLMouseRotate();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGLMouseRotate)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GLMOUSEROTATE_H__D0F03570_94E6_4182_BA37_467E8CF91B1A__INCLUDED_)
