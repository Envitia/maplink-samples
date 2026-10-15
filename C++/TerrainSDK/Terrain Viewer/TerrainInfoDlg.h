#if !defined(AFX_TERRAININFODLG_H__99F00356_C88B_4FDE_8731_C39400DD2A21__INCLUDED_)
#define AFX_TERRAININFODLG_H__99F00356_C88B_4FDE_8731_C39400DD2A21__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TerrainInfoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTerrainInfoDlg dialog

class CTerrainInfoDlg : public CDialog
{
  // Construction
public:
  CTerrainInfoDlg(CWnd* pParent = NULL);   // standard constructor

  void update( double lat, double lon, double x, double y, double z,
    double nearestX, double nearestY, double nearestZ, double xRes, double yRes );

  void show( bool s );
  bool showing();
  void create( CWnd* parent );

  afx_msg void OnClose();

  // Dialog Data
  //{{AFX_DATA(CTerrainInfoDlg)
  enum { IDD = IDD_TERRAIN_INFO };
  CStatic	m_z;
  CStatic	m_yResolution;
  CStatic	m_y;
  CStatic	m_xResolution;
  CStatic	m_x;
  CStatic	m_terrainLongitude;
  CStatic	m_terrainLatitude;
  CStatic	m_nearestZ;
  CStatic	m_nearestY;
  CStatic	m_nearestX;
  //}}AFX_DATA


  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CTerrainInfoDlg)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CTerrainInfoDlg)
  // NOTE: the ClassWizard will add member functions here
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()

private:
  bool	  m_showing;
  bool	  m_created;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TERRAININFODLG_H__99F00356_C88B_4FDE_8731_C39400DD2A21__INCLUDED_)
