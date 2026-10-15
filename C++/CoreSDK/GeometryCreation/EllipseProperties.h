#if !defined(AFX_ELLIPSEPROPERTIES_H__47C6D0C8_9EB4_4B55_A97E_997B450600D9__INCLUDED_)
#define AFX_ELLIPSEPROPERTIES_H__47C6D0C8_9EB4_4B55_A97E_997B450600D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EllipseProperties.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEllipseProperties dialog

class CEllipseProperties : public CDialog
{
  // Construction
public:
  CEllipseProperties( CGeometryCreationDoc * doc, TSLEllipse * ellipse, CWnd* pParent = NULL);   // standard constructor
  virtual ~CEllipseProperties()
  {
  }
  
  // Dialog Data
  //{{AFX_DATA(CEllipseProperties)
  enum { IDD = IDD_ELLIPSE_DIALOG };
  CString	m_centre;
  int		m_edgeColour;
  int		m_edgeStyle;
  int		m_entityID;
  int		m_featureID;
  int		m_fillColour;
  int		m_fillStyle;
  int		m_renderLevel;
  int		m_rotation;
  int		m_xRadius;
  int		m_yRadius;
  int		m_thicknessUnits;
  double	m_edgeThickness;
  //}}AFX_DATA
  
  
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CEllipseProperties)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual void PostNcDestroy();
  //}}AFX_VIRTUAL
  
  // Implementation
protected:
  
  // Generated message map functions
  //{{AFX_MSG(CEllipseProperties)
  afx_msg void OnDeletePrimitive();
  virtual void OnOK();
  virtual void OnCancel();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
    
    TSLEllipse * m_ellipse ;
  CGeometryCreationDoc * m_doc ;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELLIPSEPROPERTIES_H__47C6D0C8_9EB4_4B55_A97E_997B450600D9__INCLUDED_)
