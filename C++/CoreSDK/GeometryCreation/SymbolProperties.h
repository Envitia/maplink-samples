#if !defined(AFX_SYMBOLPROPERTIES_H__3D878008_4CF3_4FBE_B325_82CA9EBF69B5__INCLUDED_)
#define AFX_SYMBOLPROPERTIES_H__3D878008_4CF3_4FBE_B325_82CA9EBF69B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SymbolProperties.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSymbolProperties dialog

class CSymbolProperties : public CDialog
{
  // Construction
public:
  CSymbolProperties( CGeometryCreationDoc * doc, TSLSymbol * sym, CWnd* pParent = NULL);   // standard constructor
  virtual ~CSymbolProperties()
  {
  }
  
  // Dialog Data
  //{{AFX_DATA(CSymbolProperties)
  enum { IDD = IDD_SYMBOL_DIALOG };
  CString	m_position;
  double	m_sizeFactor;
  int		m_colour;
  int		m_entityID;
  int		m_featureID;
  int		m_height;
  int		m_maxSize;
  int		m_minSize;
  int		m_renderLevel;
  int		m_rotation;
  int		m_style;
  int		m_sizeFactorUnits;
  int		m_rotatable;
  BOOL	        m_fixedSize;
  //}}AFX_DATA
  
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CSymbolProperties)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual void PostNcDestroy();
  //}}AFX_VIRTUAL
  
  // Implementation
protected:
  
  // Generated message map functions
  //{{AFX_MSG(CSymbolProperties)
  virtual void OnOK();
  afx_msg void OnDeletePrimitive();
  virtual void OnCancel();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
    
  CGeometryCreationDoc * m_doc ;
  TSLSymbol * m_symbol ;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYMBOLPROPERTIES_H__3D878008_4CF3_4FBE_B325_82CA9EBF69B5__INCLUDED_)
