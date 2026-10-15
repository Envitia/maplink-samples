#if !defined(AFX_RECTANGLEPROPERTIES_H__D398D918_E664_4450_877A_12DFB00B1046__INCLUDED_)
#define AFX_RECTANGLEPROPERTIES_H__D398D918_E664_4450_877A_12DFB00B1046__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RectangleProperties.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRectangleProperties dialog

class CRectangleProperties : public CDialog
{
  // Construction
public:
  CRectangleProperties( CGeometryCreationDoc * doc, TSLRectangle * rect, CWnd* pParent = NULL);   // standard constructor
  virtual ~CRectangleProperties()
  {
  }
  
  // Dialog Data
  //{{AFX_DATA(CRectangleProperties)
  enum { IDD = IDD_RECTANGLE_DIALOG };
  CString	m_lowerLeft;
  CString	m_topRight;
  int		m_edgeColour;
  int		m_edgeStyle;
  int		m_entityID;
  int		m_featureID;
  int		m_fillColour;
  int		m_fillStyle;
  int		m_thicknessUnits;
  int		m_renderLevel;
  double	m_rotation;
  double	m_edgeThickness;
	//}}AFX_DATA
  
  
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CRectangleProperties)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual void PostNcDestroy();
	//}}AFX_VIRTUAL
  
  // Implementation
protected:
  
  // Generated message map functions
  //{{AFX_MSG(CRectangleProperties)
  virtual void OnOK();
  afx_msg void OnDeletePrimitive();
  virtual void OnCancel();
	//}}AFX_MSG
  DECLARE_MESSAGE_MAP()

  CGeometryCreationDoc * m_doc ;
  TSLRectangle         * m_rect ;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECTANGLEPROPERTIES_H__D398D918_E664_4450_877A_12DFB00B1046__INCLUDED_)
