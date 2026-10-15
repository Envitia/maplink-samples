#if !defined(AFX_POLYGONPROPERTIES_H__07F33B67_FC5B_4FE7_800D_7CFAE779C5D6__INCLUDED_)
#define AFX_POLYGONPROPERTIES_H__07F33B67_FC5B_4FE7_800D_7CFAE779C5D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PolygonProperties.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPolygonProperties dialog

class CPolygonProperties : public CDialog
{
  // Construction
public:
  CPolygonProperties( CGeometryCreationDoc * doc, TSLPolygon * poly, CWnd* pParent = NULL);   // standard constructor
  virtual ~CPolygonProperties()
  {
  }
  
  // Dialog Data
  //{{AFX_DATA(CPolygonProperties)
  enum { IDD = IDD_POLYGON_DIALOG };
  CString	m_coordinates;
  int		m_edgeColour;
  int		m_edgeStyle;
  int		m_entityID;
  int		m_featureID;
  int		m_fillColour;
  int		m_fillStyle;
  int		m_renderLevel;
  int		m_thicknessUnits;
  double	m_edgeThickness;
  //}}AFX_DATA
  
  
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CPolygonProperties)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual void PostNcDestroy();
  //}}AFX_VIRTUAL
  
  // Implementation
protected:
  
  // Generated message map functions
  //{{AFX_MSG(CPolygonProperties)
  afx_msg void OnOk();
  afx_msg void OnDeletePrimitive();
  virtual void OnCancel();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()

  CGeometryCreationDoc * m_doc ;
  TSLPolygon           * m_poly ;
  
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POLYGONPROPERTIES_H__07F33B67_FC5B_4FE7_800D_7CFAE779C5D6__INCLUDED_)
