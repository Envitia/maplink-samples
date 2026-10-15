// MIDataFilePropsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MapLinkStudioClient.h"
#include "MIDataFilePropsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMIDataFilePropsDlg dialog


CMIDataFilePropsDlg::CMIDataFilePropsDlg(CWnd* pParent, datafileInfo& fileInfo, vector<string>& libraryNames, vector<string>& resolutionNames)
	: CDialog(CMIDataFilePropsDlg::IDD, pParent)
{
  m_datafileInfo = fileInfo;

  //{{AFX_DATA_INIT(CMIDataFilePropsDlg)
  //}}AFX_DATA_INIT

  m_libraryNames = libraryNames;
  m_resolutionNames = resolutionNames;
}


void CMIDataFilePropsDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CMIDataFilePropsDlg)
  DDX_Control(pDX, IDC_MI_DATAFILE_TR_LON, m_datafile_tr_lon);
  DDX_Control(pDX, IDC_MI_DATAFILE_TR_LAT, m_datafile_tr_lat);
  DDX_Control(pDX, IDC_MI_DATAFILE_BL_LON, m_datafile_bl_lon);
  DDX_Control(pDX, IDC_MI_DATAFILE_BL_LAT, m_datafile_bl_lat);
  DDX_Control(pDX, IDC_RESOLUTION, m_resolution);
  DDX_Control(pDX, IDC_LIBRARY, m_library);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMIDataFilePropsDlg, CDialog)
  //{{AFX_MSG_MAP(CMIDataFilePropsDlg)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMIDataFilePropsDlg message handlers

BOOL CMIDataFilePropsDlg::OnInitDialog() 
{
  CDialog::OnInitDialog();
  this->SetWindowText(m_datafileInfo.full_path);

  TCHAR printable[256];

  _stprintf(printable, _T("%g"), m_datafileInfo.tr_lon);
  m_datafile_tr_lon.SetWindowText(printable);

  _stprintf(printable, _T("%g"), m_datafileInfo.tr_lat);
  m_datafile_tr_lat.SetWindowText(printable);

  _stprintf(printable, _T("%g"), m_datafileInfo.bl_lon);
  m_datafile_bl_lon.SetWindowText(printable);

  _stprintf(printable, _T("%g"), m_datafileInfo.bl_lat);
  m_datafile_bl_lat.SetWindowText(printable);

  // setup a list of resolution / library names

  vector<string>::iterator i;
  
  m_resolution.AddString(_T("None"));
  for (i = m_resolutionNames.begin(); i != m_resolutionNames.end(); i++)
    m_resolution.AddString( (LPCTSTR)TSLUTF8Decoder((*i)) );

  m_library.AddString(_T("None"));
  for (i = m_libraryNames.begin(); i != m_libraryNames.end(); i++)
    m_library.AddString( (LPCTSTR)TSLUTF8Decoder((*i)) );

  m_resolution.SetCurSel(0);
  m_library.SetCurSel(0);

  UpdateData(FALSE);
	
  return TRUE;
}

void CMIDataFilePropsDlg::OnOK() 
{
  CDialog::OnOK();

  // set datafile resolution
  
  CString resolution;
  int curSel = m_resolution.GetCurSel();
  
  if (curSel != LB_ERR)
    m_resolution.GetLBText (curSel, resolution);

  if (resolution != _T("None"))
    m_datafileInfo.resolution = resolution;

  // set datafile library
  
  CString library;
  curSel = m_library.GetCurSel();
  
  if (curSel != LB_ERR)
    m_library.GetLBText (curSel, library);

  if (library != _T("None"))
    m_datafileInfo.library = library;  
}
