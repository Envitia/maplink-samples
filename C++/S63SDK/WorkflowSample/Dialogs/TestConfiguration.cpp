// TestSetup.cpp : implementation file
//
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "../stdafx.h"
#include "../S63/s63workflow.h"
#include "../S63WorkFlowSample.h"
#include "TestConfiguration.h"


// TestConfiguration dialog

IMPLEMENT_DYNAMIC(TestConfiguration, CDialog)

TestConfiguration::TestConfiguration(S63Workflow * workflow, CWnd* pParent /*=NULL*/)
	: CDialog(TestConfiguration::IDD, pParent)
        , m_workFlow(workflow)
        , m_cacheInput()
        , m_cacheSize(32)
        , m_configurationTime(0)
        , m_expiryDateInput()
        , m_hardwareInput()
        , m_hw_id()
        , m_logFile()
        , m_logPermanentWarnings(false)
        , m_permanentWarnings()
        , m_unlockInput()
        , m_unlockKey()
        , m_displayIngestDialog()
        , m_ingestCellCheck()
{
}

TestConfiguration::~TestConfiguration()
{
}

BOOL TestConfiguration::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_permanentWarnings.SetCheck(m_logPermanentWarnings ? BST_CHECKED : BST_UNCHECKED);
  m_ingestCellCheck.SetCheck(m_displayIngestDialog ? BST_CHECKED : BST_UNCHECKED);
  
  if ( m_configurationTime )
  {   
    m_expiryDateInput.SetTime(m_configurationTime);
  }
  else
  {
    const CTime defaultTime = CTime(2004, 8, 24, 12, 0, 0);
    m_expiryDateInput.SetTime(&defaultTime);
  }

  CString cache;
  cache.Format(_T("%d"), m_cacheSize);

  m_cacheInput.SetWindowText(cache);

  m_s57ConfigFile = (LPCTSTR)TSLUTF8Decoder(m_workFlow->s57ConfigFile());

  UpdateData(false);
  return true;
}

void TestConfiguration::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  
  DDX_Text (pDX, IDC_S57_CONFIG_FILE, m_s57ConfigFile );
  DDX_Text (pDX, IDC_LOG_FILE, m_logFile );
  DDX_Text (pDX, IDC_UNLOCK_KEY, m_unlockKey );
  DDX_Text (pDX, IDC_CACHE_SIZE, m_cacheSize );
  DDX_Text (pDX, IDC_HARDWARE_ID, m_hw_id );

  DDX_Control(pDX, IDC_UNLOCK_KEY, m_unlockInput);
  DDX_Control(pDX, IDC_DISABLE_WARNINGS, m_permanentWarnings);
  DDX_Control(pDX, IDC_CACHE_SIZE, m_cacheInput);
  DDX_Control(pDX, IDC_HARDWARE_ID, m_hardwareInput);
  DDX_Control(pDX, IDC_EXPIRY_DATE, m_expiryDateInput);
  DDX_Control(pDX, IDC_DISPLAY_INGEST_DIALOG, m_ingestCellCheck);
}


BEGIN_MESSAGE_MAP(TestConfiguration, CDialog)
  ON_BN_CLICKED(ID_S57_CONFIG_BROWSE, &TestConfiguration::OnBnClickedS57Browse)
  ON_BN_CLICKED(ID_BROWSE, &TestConfiguration::OnBnClickedBrowse)
  ON_BN_CLICKED(IDC_DISABLE_WARNINGS, &TestConfiguration::OnBnClickedDisableWarnings)
  ON_BN_CLICKED(IDC_DISPLAY_INGEST_DIALOG, &TestConfiguration::OnBnClickedDisplayIngest)
  ON_BN_CLICKED(IDOK, &TestConfiguration::OnBnClickedOk)
END_MESSAGE_MAP()


// TestSetup message handlers

void TestConfiguration::OnBnClickedS57Browse()
{
  CFileDialog FileDlg(true, _T(".ini"), NULL, OFN_NOCHANGEDIR, _T("Config Files (*.ini)|*.ini|All Files (*.*)|*.*||"));
  FileDlg.m_pOFN->lpstrTitle = _T("Select S-57 configuration file");

  if( FileDlg.DoModal() == IDOK )
  {
    m_s57ConfigFile.SetString(FileDlg.GetPathName());
  }

  //set the current text values before we update
  m_unlockInput.GetWindowText(m_unlockKey);
  m_hardwareInput.GetWindowText(m_hw_id);

  UpdateData(false);
}

void TestConfiguration::OnBnClickedBrowse()
{
  CFileDialog FileDlg(false, _T(".log"), NULL, OFN_NOCHANGEDIR, _T("Log Files (*.log)|*.log|Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"));
  FileDlg.m_pOFN->lpstrTitle = _T("Select log file");

  if( FileDlg.DoModal() == IDOK )
  {
    m_logFile.SetString(FileDlg.GetPathName());
  }

  //set the current text values before we update
  m_unlockInput.GetWindowText(m_unlockKey);
  m_hardwareInput.GetWindowText(m_hw_id);

  UpdateData(false);
}

void TestConfiguration::OnBnClickedDisableWarnings()
{
  m_logPermanentWarnings = ((m_permanentWarnings.GetState() & 0x0003) != 0);
}

void TestConfiguration::OnBnClickedDisplayIngest()
{
  m_displayIngestDialog = ((m_ingestCellCheck.GetState() & 0x0003) != 0);
}

void TestConfiguration::OnBnClickedOk()
{
  CTime time;
  m_expiryDateInput.GetTime(time);
  m_configurationTime = time.GetTime();

  UpdateData(true);
  string hardwareID = TSLUTF8Encoder(m_hw_id);

  // Update the hardware ID.
  if ( hardwareID.length() != 5 || !m_workFlow->hwid(hardwareID) )
  {
    AfxMessageBox(_T("The provided hardware ID is invalid. Please specify a valid HWID."), MB_ICONERROR);
    return;
  }

  // Update the configuration file
  string s57Config = TSLUTF8Encoder(m_s57ConfigFile);
  m_workFlow->s57ConfigFile( s57Config );

  CDialog::OnOK();
}
