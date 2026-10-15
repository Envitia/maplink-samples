#if !defined(AFX_ARCPROPERTIES_H__72F4A2EF_0964_4C2A_89B9_73A12C8CE31E__INCLUDED_)
#define AFX_ARCPROPERTIES_H__72F4A2EF_0964_4C2A_89B9_73A12C8CE31E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ArcProperties.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CArcProperties dialog

class CArcProperties : public CDialog
{
  // Construction
public:
  CArcProperties( CGeometryCreationDoc * doc, TSLArc * arc, CWnd* pParent = NULL);   // standard constructor
  virtual ~CArcProperties()
  {
  }
  
  // Dialog Data
  //{{AFX_DATA(CArcProperties)
	enum { IDD = IDD_ARC_DIALOG };
  CString	m_centre;
  int		m_edgeColour;
  int		m_edgeStyle;
  double	m_edgeThickness;
  double	m_endAngle;
  int		m_entityID;
  int		m_featureID;
  int		m_renderLevel;
  double	m_rotation;
  double	m_startAngle;
  int		m_xRadius;
  int		m_yRadius;
	int		m_edgeThicknessUnits;
	//}}AFX_DATA
  
  
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CArcProperties)
	protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL
  
  // Implementation
protected:
  
  // Generated message map functions
  //{{AFX_MSG(CArcProperties)
  afx_msg void OnDeletePrimitive();
  virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
  DECLARE_MESSAGE_MAP()

  TSLArc * m_arc ;
  CGeometryCreationDoc * m_doc ;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ARCPROPERTIES_H__72F4A2EF_0964_4C2A_89B9_73A12C8CE31E__INCLUDED_)
