// TerrainInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TerrainInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTerrainInfoDlg dialog


CTerrainInfoDlg::CTerrainInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTerrainInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTerrainInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
  m_showing = false;
  m_created = false;
}


void CTerrainInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTerrainInfoDlg)
	DDX_Control(pDX, IDC_Z, m_z);
	DDX_Control(pDX, IDC_Y_RESOLUTION, m_yResolution);
	DDX_Control(pDX, IDC_Y, m_y);
	DDX_Control(pDX, IDC_X_RESOLUTION, m_xResolution);
	DDX_Control(pDX, IDC_X, m_x);
	DDX_Control(pDX, IDC_TERRAIN_LONGITUDE, m_terrainLongitude);
	DDX_Control(pDX, IDC_TERRAIN_LATITUDE, m_terrainLatitude);
	DDX_Control(pDX, IDC_NEAREST_Z, m_nearestZ);
	DDX_Control(pDX, IDC_NEAREST_Y, m_nearestY);
	DDX_Control(pDX, IDC_NEAREST_X, m_nearestX);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTerrainInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CTerrainInfoDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
        ON_WM_CLOSE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTerrainInfoDlg message handlers

void CTerrainInfoDlg::update( double lat, double lon, double x, double y, double z,
	     double nearestX, double nearestY, double nearestZ, double xRes, double yRes )
{
  if ( m_created )
  {
    CString text;
    text.Format( _T("%.3lf"), lat );
    m_terrainLatitude.SetWindowText( text );
    text.Format( _T("%.3lf"), lon );
    m_terrainLongitude.SetWindowText( text );
    text.Format( _T("%.2lf"), x );
    m_x.SetWindowText( text );
    text.Format( _T("%.2lf"), y );
    m_y.SetWindowText( text );
    text.Format( _T("%.0lf"), z );
    m_z.SetWindowText( text );
    text.Format( _T("%.2lf"), nearestX );
    m_nearestX.SetWindowText( text );
    text.Format( _T("%.2lf"), nearestY );
    m_nearestY.SetWindowText( text );
    text.Format( _T("%.0lf"), nearestZ );
    m_nearestZ.SetWindowText( text );
    text.Format( _T("%.3lf"), xRes );
    m_xResolution.SetWindowText( text );
    text.Format( _T("%.3lf"), yRes );
    m_yResolution.SetWindowText( text );
  }
}

void CTerrainInfoDlg::show( bool s )
{
  if ( m_showing == s )
  {
    return;
  }
  m_showing = s;
  ShowWindow( m_showing ? SW_SHOW : SW_HIDE );
}

bool CTerrainInfoDlg::showing()
{
  return m_showing;
}

void CTerrainInfoDlg::create( CWnd* parent )
{
  if ( !m_created )
  {
    if ( Create( IDD_TERRAIN_INFO, parent ) )
    {
      m_created = true;
    }
  }
}
void CTerrainInfoDlg::OnClose()
{
  m_showing = false;
  CDialog::OnClose();
}
