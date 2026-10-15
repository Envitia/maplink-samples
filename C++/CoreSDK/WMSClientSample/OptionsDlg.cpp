// OptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WMSClientSample.h"
#include "OptionsDlg.h"


// COptionsDlg dialog

IMPLEMENT_DYNAMIC(COptionsDlg, CDialog)

COptionsDlg::COptionsDlg(CWnd* pParent /*=NULL*/)
: CDialog(COptionsDlg::IDD, pParent)
{
  // Find the number of Core's and set the number of concurrent requests
  // based on this value.
  SYSTEM_INFO sysinfo;
  GetSystemInfo( &sysinfo );

  int numCPU = sysinfo.dwNumberOfProcessors;

  m_numberOfThreads = numCPU;
}

COptionsDlg::~COptionsDlg()
{
}

void COptionsDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_CACHE_SLIDER, m_cacheSlider);
  DDX_Control(pDX, IDC_SCALE_FACTOR_SLIDER, m_sfSlider);
  DDX_Control(pDX, IDC_CERT_CN_INVALID, m_certCNInvalid);
  DDX_Control(pDX, IDC_UNKNOWN_CA, m_unknownCA);
  DDX_Control(pDX, IDC_TILELOADORDER, m_tileLoadOrder);
  DDX_Control(pDX, IDC_TILELEVELS, m_tileLevels);
  DDX_Control(pDX, IDC_INITIALWMSVERSION, m_initalWMSVersion);
  DDX_Text(pDX, IDC_NUMBER_OF_THREADS, m_numberOfThreads);
	DDV_MinMaxInt(pDX, m_numberOfThreads, 2, 32);
}


BEGIN_MESSAGE_MAP(COptionsDlg, CDialog)
END_MESSAGE_MAP()


// COptionsDlg message handlers

BOOL COptionsDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_cacheSlider.SetRange(16, 256);
  m_sfSlider.SetRange(0, 15);

  CWinApp * app = AfxGetApp();
  ASSERT(app);

  int tcs = app->GetProfileInt(OPTIONS_APPSETTING_SECTION, TILE_CACHE_SIZE_APPSETTING, 128);
  m_cacheSlider.SetPos(tcs);

  int tlvl = app->GetProfileInt(OPTIONS_APPSETTING_SECTION, TILE_LEVELS_APPSETTING, (int)(TSLWMSDataLayer::TileLevelStrategyDetect));
  m_tileLevels.SetCurSel(tlvl);
  
  int tlor = app->GetProfileInt(OPTIONS_APPSETTING_SECTION, TILE_LOAD_ORDER_APPSETTING, (int)(TSLWMSDataLayer::ClockwiseSpiral_CentreStart));
  m_tileLoadOrder.SetCurSel(tlor);

  int psf = app->GetProfileInt(OPTIONS_APPSETTING_SECTION, P_SCALE_FACTOR_APPSETTING, 9);
  m_sfSlider.SetPos(psf);

  int ukca = app->GetProfileInt(OPTIONS_APPSETTING_SECTION, UNKNOWN_CA_APPSETTING, 0);
  m_unknownCA.SetCheck(ukca == 1 ? BST_CHECKED : BST_UNCHECKED);
  int certcnin = app->GetProfileInt(OPTIONS_APPSETTING_SECTION, CERT_CN_INVALID_APPSETTING, 0);
  m_certCNInvalid.SetCheck(certcnin == 1 ? BST_CHECKED : BST_UNCHECKED);

  int wivers = app->GetProfileInt(OPTIONS_APPSETTING_SECTION, INITIAL_VERSION_APPSETTING, ((int)TSLWMSVersion_1_3_0)-1); // first WMS version not listed
  m_initalWMSVersion.SetCurSel(wivers);

  int numberOfThreads = app->GetProfileInt(OPTIONS_APPSETTING_SECTION, NUMBER_OF_THREADS_APPSETTING, m_numberOfThreads);
  m_numberOfThreads = numberOfThreads;

  UpdateData(FALSE);
  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionsDlg::OnOK()
{
  UpdateData(TRUE);

  CWinApp * app = AfxGetApp();
  ASSERT(app);

  int tcs = m_cacheSlider.GetPos();
  app->WriteProfileInt(OPTIONS_APPSETTING_SECTION, TILE_CACHE_SIZE_APPSETTING, tcs);

  int tlvl = m_tileLevels.GetCurSel();
  app->WriteProfileInt(OPTIONS_APPSETTING_SECTION, TILE_LEVELS_APPSETTING, tlvl);

  int tlor = m_tileLoadOrder.GetCurSel();
  app->WriteProfileInt(OPTIONS_APPSETTING_SECTION, TILE_LOAD_ORDER_APPSETTING, tlor);

  int psf = m_sfSlider.GetPos();
  app->WriteProfileInt(OPTIONS_APPSETTING_SECTION, P_SCALE_FACTOR_APPSETTING, psf);

  int ukca = m_unknownCA.GetCheck() == BST_CHECKED ? 1 : 0;
  app->WriteProfileInt(OPTIONS_APPSETTING_SECTION, UNKNOWN_CA_APPSETTING, ukca);

  int certcnin = m_certCNInvalid.GetCheck() == BST_CHECKED ? 1 : 0;
  app->WriteProfileInt(OPTIONS_APPSETTING_SECTION, CERT_CN_INVALID_APPSETTING, certcnin);

  int wivers = m_initalWMSVersion.GetCurSel();
  app->WriteProfileInt(OPTIONS_APPSETTING_SECTION, INITIAL_VERSION_APPSETTING, wivers);

  app->WriteProfileInt(OPTIONS_APPSETTING_SECTION, NUMBER_OF_THREADS_APPSETTING, m_numberOfThreads);

  CDialog::OnOK();
}





