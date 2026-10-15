// OptionsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TerrainViewer.h"
#include "TerrainViewerDoc.h"
#include "OptionsDialog.h"
#include "terrainview.h"

#include "afxcolorbutton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsDialog dialog


COptionsDialog::COptionsDialog(TerrainViewerOptions& options, CWnd* pParent /*=NULL*/)
  : CDialog(COptionsDialog::IDD, pParent)
  , m_options(options)
  , m_viewShedMaximumRadius(0.0)
  , m_viewShedStartHeight(0)
  , m_viewShedStartHeightRelativeToGround(false)
  , m_viewShedEndHeight(0)
  , m_viewShedEndHeightRelativeToGround(false)
{
  m_options.load();

  // Terrain profile view options
  m_terrainscale = m_options.m_terr_scale;
  m_autoscale = m_options.m_terrainAutoscale;
  m_bathyscale = m_options.m_bathy_scale;
  m_bathyon = m_options.m_bathy;
  m_manualscale = m_options.m_manual_scale;

  // Single LOS options
  m_startheight = m_options.m_losStartHeight;
  m_endheight = m_options.m_losEndHeight;
  m_curveOfEarthSingle = m_options.m_losEarthCurvature;
  m_seaLevelReference = m_options.m_losFromSeaLevel;

  // Area LOS options
  m_viewShedMaximumRadius = m_options.m_viewShedMaximumRadius;
  // Colours are read directly from options object.
  m_viewShedStartHeight = m_options.m_viewShedStartHeight;
  m_viewShedStartHeightRelativeToGround = m_options.m_viewShedStartRelativeToGround;
  m_viewShedEndHeight = m_options.m_viewShedEndHeight;
  m_viewShedEndHeightRelativeToGround = m_options.m_viewShedEndRelativeToGround;
}

BOOL COptionsDialog::OnInitDialog()
{
  CDialog::OnInitDialog();

  CMFCColorButton * visibleColour = (CMFCColorButton*) GetDlgItem( IDC_OPTIONS_VISIBLE_COLOUR );
  visibleColour->SetColor( swapRedAndBlue(m_options.m_viewShedColourVisible ) );
  m_visTransparent.SetCheck( m_options.m_viewShedColourVisibleTransparent ) ;
  
  CMFCColorButton * notVisibleColour = (CMFCColorButton*) GetDlgItem( IDC_OPTIONS_NOT_VISIBLE_COLOUR );
  notVisibleColour->SetColor( swapRedAndBlue(m_options.m_viewShedColourNotVisible) );
  m_novisTransparent.SetCheck( m_options.m_viewShedColourNotVisibleTransparent ) ;

  CMFCColorButton * noDataColour = (CMFCColorButton*) GetDlgItem( IDC_OPTIONS_NO_DATA_COLOUR );
  noDataColour->SetColor( swapRedAndBlue(m_options.m_viewShedColourNoData) );
  m_nodataTransparent.SetCheck( m_options.m_viewShedColourNoDataTransparent ) ;

  CComboBox* areaCurvatureBox = (CComboBox*) GetDlgItem( IDC_OPTIONS_AREA_EARTH_CURVATURE_TYPE );
  areaCurvatureBox->SetCurSel(m_options.m_viewShedCurvatureBoxIndex);

  return TRUE;
}

void COptionsDialog::DoDataExchange(CDataExchange* pDX)
{

  CDialog::DoDataExchange(pDX);

  //{{AFX_DATA_MAP(COptionsDialog)
  DDX_Check(pDX, IDC_OPTIONS_AUTOSCALE, m_autoscale);
  DDX_Check(pDX, IDC_OPTIONS_BATHYON, m_bathyon);
  DDX_Text(pDX, IDC_OPTIONS_BATHYSCALE, m_bathyscale);
  DDV_MinMaxDouble(pDX, m_bathyscale, 0.1, 100.0);
  DDX_Text(pDX, IDC_OPTIONS_MANUALSCALE, m_manualscale);
  DDV_MinMaxDouble(pDX, m_manualscale, 0., 100000.);
  DDX_Text(pDX, IDC_OPTIONS_TERRSCALE, m_terrainscale);
  DDV_MinMaxDouble(pDX, m_terrainscale, 0.1, 100.0);
  DDX_Text(pDX, IDC_OPTIONS_LOS1, m_startheight);
  DDX_Text(pDX, IDC_OPTIONS_LOS2, m_endheight);
  DDX_Check(pDX, IDC_CURVE_OF_EARTH_SINGLE_CHECK, m_curveOfEarthSingle);
  DDX_Check(pDX, IDC_SEA_LEVEL_CHECK, m_seaLevelReference);
  DDX_Text(pDX, IDC_OPTIONS_AREA_MAX_RADIUS, m_viewShedMaximumRadius);
  DDX_Text(pDX, IDC_OPTIONS_AREA_START_HEIGHT, m_viewShedStartHeight);
  DDX_Check(pDX, IDC_OPTIONS_AREA_START_RELATIVE_TO_GROUND, m_viewShedStartHeightRelativeToGround);
  DDX_Text(pDX, IDC_OPTIONS_AREA_END_HEIGHT, m_viewShedEndHeight);
  DDX_Check(pDX, IDC_OPTIONS_END_RELATIVE_TO_GROUND, m_viewShedEndHeightRelativeToGround);
  DDX_Control(pDX, IDC_VIS_TRANSPARENT, m_visTransparent);
  DDX_Control(pDX, IDC_NOVIS_TRANSPARENT, m_novisTransparent);
  DDX_Control(pDX, IDC_NODATA_TRANSPARENT, m_nodataTransparent);
  //}}AFX_DATA_MAP

  // Update when displayed.
  CEdit * manualEdit = (CEdit *) GetDlgItem ( IDC_OPTIONS_MANUALSCALE ) ;
  CEdit * bathScaleEdit = (CEdit *) GetDlgItem ( IDC_OPTIONS_BATHYSCALE ) ;
  CEdit * terrScaleEdit = (CEdit *) GetDlgItem ( IDC_OPTIONS_TERRSCALE ) ;

  manualEdit->SetReadOnly ( m_autoscale ) ;
  bathScaleEdit->SetReadOnly ( m_autoscale ) ;
  terrScaleEdit->SetReadOnly ( m_autoscale ) ;
}

BEGIN_MESSAGE_MAP(COptionsDialog, CDialog)
  //{{AFX_MSG_MAP(COptionsDialog)
  ON_EN_CHANGE(IDC_OPTIONS_LOS1, OnChangeOptionsLos1)
  ON_EN_CHANGE(IDC_OPTIONS_LOS2, OnChangeOptionsLos2)
  ON_BN_CLICKED(IDC_OPTIONS_AUTOSCALE, OnOptionsAutoscale)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsDialog message handlers

void COptionsDialog::OnChangeOptionsLos1() 
{
  // TODO: If this is a RICHEDIT control, the control will not
  // send this notification unless you override the CDialog::OnInitDialog()
  // function and call CRichEditCtrl().SetEventMask()
  // with the ENM_CHANGE flag ORed into the mask.

  // TODO: Add your control notification handler code here

}

void COptionsDialog::OnChangeOptionsLos2() 
{
  // TODO: If this is a RICHEDIT control, the control will not
  // send this notification unless you override the CDialog::OnInitDialog()
  // function and call CRichEditCtrl().SetEventMask()
  // with the ENM_CHANGE flag ORed into the mask.

  // TODO: Add your control notification handler code here

}

void COptionsDialog::OnOK() 
{
  CDialog::OnOK();

  CMFCColorButton * visibleColour = (CMFCColorButton*) GetDlgItem( IDC_OPTIONS_VISIBLE_COLOUR );
  m_options.m_viewShedColourVisible = swapRedAndBlue(visibleColour->GetColor());
  m_options.m_viewShedColourVisibleTransparent = m_visTransparent.GetCheck() ;

  CMFCColorButton * notVisibleColour = (CMFCColorButton*) GetDlgItem( IDC_OPTIONS_NOT_VISIBLE_COLOUR );
  m_options.m_viewShedColourNotVisible = swapRedAndBlue(notVisibleColour->GetColor());
  m_options.m_viewShedColourNotVisibleTransparent = m_novisTransparent.GetCheck() ;

  CMFCColorButton * noDataColour = (CMFCColorButton*) GetDlgItem( IDC_OPTIONS_NO_DATA_COLOUR );
  m_options.m_viewShedColourNoData = swapRedAndBlue(noDataColour->GetColor());
  m_options.m_viewShedColourNoDataTransparent = m_nodataTransparent.GetCheck() ;

  CComboBox* areaCurvatureBox = (CComboBox*) GetDlgItem( IDC_OPTIONS_AREA_EARTH_CURVATURE_TYPE );
  m_options.m_viewShedCurvatureBoxIndex = areaCurvatureBox->GetCurSel();

  // Terrain profile view options
  m_options.m_terr_scale = m_terrainscale;
  m_options.m_terrainAutoscale = m_autoscale;
  m_options.m_bathy_scale = m_bathyscale;
  m_options.m_bathy = m_bathyon;
  m_options.m_manual_scale = m_manualscale;

  // Single LOS options
  m_options.m_losStartHeight = m_startheight;
  m_options.m_losEndHeight = m_endheight;
  m_options.m_losEarthCurvature = m_curveOfEarthSingle;
  m_options.m_losFromSeaLevel = m_seaLevelReference;

  // Area LOS options
  m_options.m_viewShedMaximumRadius = m_viewShedMaximumRadius;
  // Colours are written directly to options object.
  m_options.m_viewShedStartHeight = m_viewShedStartHeight;
  m_options.m_viewShedStartRelativeToGround = m_viewShedStartHeightRelativeToGround;
  m_options.m_viewShedEndHeight = m_viewShedEndHeight;
  m_options.m_viewShedEndRelativeToGround = m_viewShedEndHeightRelativeToGround;

  m_options.save();

}

void COptionsDialog::OnOptionsAutoscale() 
{
  // The user should be able to used the manual scale
  // box only if this is OFF.
  CButton * autoCheck = (CButton *) GetDlgItem( IDC_OPTIONS_AUTOSCALE ) ;
  CEdit * manualEdit = (CEdit *) GetDlgItem ( IDC_OPTIONS_MANUALSCALE ) ;
  CEdit * bathScaleEdit = (CEdit *) GetDlgItem ( IDC_OPTIONS_BATHYSCALE ) ;
  CEdit * terrScaleEdit = (CEdit *) GetDlgItem ( IDC_OPTIONS_TERRSCALE ) ;

  manualEdit->SetReadOnly ( autoCheck->GetCheck () ) ;
  bathScaleEdit->SetReadOnly ( autoCheck->GetCheck () ) ;
  terrScaleEdit->SetReadOnly ( autoCheck->GetCheck () ) ;

}



