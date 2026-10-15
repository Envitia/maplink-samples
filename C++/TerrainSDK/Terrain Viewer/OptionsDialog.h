#if !defined(AFX_OPTIONSDIALOG_H__88A34CCD_4BD9_11D5_93B0_00C04FA10B1D__INCLUDED_)
#define AFX_OPTIONSDIALOG_H__88A34CCD_4BD9_11D5_93B0_00C04FA10B1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionsDialog dialog

#include "TerrainViewerOptions.h"
#include "afxwin.h"

class CTerrainView;

class COptionsDialog : public CDialog
{
  // Construction
public:
  COptionsDialog(TerrainViewerOptions& options, CWnd* pParent = NULL);   // standard constructor

private:
  static inline uint32_t swapRedAndBlue(uint32_t colour);
  static inline int LOSTypeToComboBoxIndex(TSLTerrainVSInputEarthCurvature::LOSType type);
  static inline TSLTerrainVSInputEarthCurvature::LOSType ComboBoxIndexToLOSType(int index);
  TerrainViewerOptions& m_options;
  // Dialog Data
  //{{AFX_DATA(COptionsDialog)
  enum { IDD = IDD_OPTIONS_DIALOG };
  BOOL	m_autoscale;
  BOOL	m_bathyon;
  double	m_bathyscale;
  double	m_manualscale;
  double	m_terrainscale;
  double  m_startheight;
  double  m_endheight;

  BOOL m_curveOfEarthSingle;
  BOOL m_seaLevelReference;
  //}}AFX_DATA


  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(COptionsDialog)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:
  virtual BOOL OnInitDialog();

  // Generated message map functions
  //{{AFX_MSG(COptionsDialog)
  afx_msg void OnChangeOptionsLos1();
  afx_msg void OnChangeOptionsLos2();
  virtual void OnOK();
  afx_msg void OnOptionsAutoscale();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()

private:
  double m_viewShedMaximumRadius;
  double m_viewShedStartHeight;
  BOOL m_viewShedStartHeightRelativeToGround;
  double m_viewShedEndHeight;
  BOOL m_viewShedEndHeightRelativeToGround;
public:
  CButton m_visTransparent;
  CButton m_novisTransparent;
  CButton m_nodataTransparent;
};


// The MFC Colour picker uses ABGR.
// CTerrainClientCustomDataLayer uses ARGB.
uint32_t COptionsDialog::swapRedAndBlue(uint32_t colour)
{
  return     
    (colour & 0xff00ff00) |
    (colour & 0x00ff0000) >> 16 |
    (colour & 0x000000ff) << 16;
}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSDIALOG_H__88A34CCD_4BD9_11D5_93B0_00C04FA10B1D__INCLUDED_)
