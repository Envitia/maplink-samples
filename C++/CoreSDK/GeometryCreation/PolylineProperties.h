#if !defined(AFX_POLYLINEPROPERTIES_H__6C87EA58_11C0_4EA3_AA08_92A85F6B8E6B__INCLUDED_)
#define AFX_POLYLINEPROPERTIES_H__6C87EA58_11C0_4EA3_AA08_92A85F6B8E6B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PolylineProperties.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPolylineProperties dialog

class CPolylineProperties : public CDialog
{
  // Construction
public:
  CPolylineProperties( CGeometryCreationDoc * doc, TSLPolyline * poly, CWnd* pParent = NULL);   // standard constructor
  virtual ~CPolylineProperties()
  {
  }
  
  // Dialog Data
  //{{AFX_DATA(CPolylineProperties)
  enum { IDD = IDD_POLYLINE_DIALOG };
  CString	m_coordinates;
  int		m_edgeColour;
  int		m_edgeStyle;
  int		m_entityID;
  int		m_featureID;
  int		m_renderLevel;
  int		m_thicknessUnits;
  double	m_edgeThickness;
  //}}AFX_DATA
  
  
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CPolylineProperties)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual void PostNcDestroy();
  //}}AFX_VIRTUAL
  
  // Implementation
protected:
  
  // Generated message map functions
  //{{AFX_MSG(CPolylineProperties)
  virtual void OnOK();
  afx_msg void OnDeletePrimitive();
  virtual void OnCancel();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
    
    CGeometryCreationDoc * m_doc ;
  TSLPolyline * m_poly ;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POLYLINEPROPERTIES_H__6C87EA58_11C0_4EA3_AA08_92A85F6B8E6B__INCLUDED_)
