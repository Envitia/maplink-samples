#if !defined(AFX_TEXTPROPERTIES_H__998DA80F_0F93_4B27_9080_1B6BE7DC2E71__INCLUDED_)
#define AFX_TEXTPROPERTIES_H__998DA80F_0F93_4B27_9080_1B6BE7DC2E71__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextProperties.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTextProperties dialog

class CTextProperties : public CDialog
{
  // Construction
public:
  CTextProperties( CGeometryCreationDoc * doc, TSLText * text, CWnd* pParent = NULL);   // standard constructor
  virtual ~CTextProperties()
  {
  }
  
  // Dialog Data
  //{{AFX_DATA(CTextProperties)
  enum { IDD = IDD_TEXT_DIALOG };
  CComboBox	m_verticalAlignment;
  CComboBox	m_horizontalAlignment;
  CComboBox	m_backgroundMode;
  CString	m_position;
  CString	m_string;
  CString	m_offset;
  double	m_sizeFactor;
  int		m_backgroundColour;
  int		m_backgroundEdgeColour;
  int		m_backgroundStyle;
  int		m_colour;
  int		m_entityID;
  int		m_featureID;
  int		m_font;
  int		m_height;
  int		m_maxSize;
  int		m_minSize;
  int		m_renderLevel;
  int		m_rotation;
  int		m_offsetUnits;
  int		m_sizeFactorUnits;
  BOOL          m_fixedSize;
  BOOL          m_rotatable;
  //}}AFX_DATA
  
  
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CTextProperties)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual void PostNcDestroy();
  //}}AFX_VIRTUAL
  
  // Implementation
protected:
  
  // Generated message map functions
  //{{AFX_MSG(CTextProperties)
  virtual void OnOK();
  afx_msg void OnDeletePrimitive();
  virtual void OnCancel();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
    
  CGeometryCreationDoc * m_doc ;
  TSLText * m_text ;
  
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTPROPERTIES_H__998DA80F_0F93_4B27_9080_1B6BE7DC2E71__INCLUDED_)
