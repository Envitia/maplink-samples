// MISourceSelectionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MapLinkStudioClient.h"
#include "MISourceSelectionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMISourceSelectionDlg dialog


CMISourceSelectionDlg::CMISourceSelectionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMISourceSelectionDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CMISourceSelectionDlg)
  // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
  m_itemImageList = 0;
  m_sources.clear();
}


void CMISourceSelectionDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CMISourceSelectionDlg)
  DDX_Control(pDX, IDOK, m_OK);
  DDX_Control(pDX, IDC_MI_SOURCE_TYPE, m_source_type);
  DDX_Control(pDX, IDC_MI_PROJECT_LIST, m_project_list);
  //}}AFX_DATA_MAP
}



BEGIN_MESSAGE_MAP(CMISourceSelectionDlg, CDialog)
  //{{AFX_MSG_MAP(CMISourceSelectionDlg)
  ON_CBN_SELCHANGE(IDC_MI_SOURCE_TYPE, OnSelchangeMiSourceType)
  ON_NOTIFY(NM_CLICK, IDC_MI_PROJECT_LIST, OnClickMiProjectList)
	ON_NOTIFY(NM_DBLCLK, IDC_MI_PROJECT_LIST, OnDblclkMiProjectList)
	ON_CBN_EDITCHANGE(IDC_MI_SOURCE_TYPE, OnEditchangeMiSourceType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMISourceSelectionDlg message handlers

void CMISourceSelectionDlg::OnOK() 
{
  CString CSsource;
  int curSel = m_source_type.GetCurSel();
  
  if (curSel != LB_ERR)
    m_source_type.GetLBText (curSel, CSsource);

  string source = TSLUTF8Encoder(CSsource);

  if (!source.empty())
  {
    theApp.setSourceName(source);

    POSITION pos = m_project_list.GetFirstSelectedItemPosition();
    int nItem = m_project_list.GetNextSelectedItem(pos);
    CString CSproject = m_project_list.GetItemText(nItem,0);
    string project = TSLUTF8Encoder(CSproject);
    if (project.size())
    {
      theApp.setTemplateName(project);
		
      string projectFile = theApp.getMaplinkHome() + string("\\projects\\AutomationTemplates\\") + project + ".mlp";
      theApp.setProjectFile(projectFile);
    }
    else
    {
      AfxMessageBox(_T("Please select a Valid Template"));
      return;
    }
  }
  else
  {
    AfxMessageBox(_T("Please select a Source Type"));
    return;
  }

  m_itemImageList->DeleteImageList();
  delete m_itemImageList;
  m_itemImageList = 0;
  
  CDialog::OnOK();
}

void CMISourceSelectionDlg::OnSelchangeMiSourceType() 
{
  m_project_list.DeleteAllItems();  

  CString source;
  int curSel = m_source_type.GetCurSel();
  
  if (curSel != LB_ERR)
    m_source_type.GetLBText (curSel, source);

  if (source)
  {
    string buffer = theApp.getMaplinkHome() + "\\projects\\AutomationTemplates\\*.mlp" ;
    
    int index = 0;
    CFileFind finder;
    BOOL bWorking = finder.FindFile((LPCTSTR)TSLUTF8Decoder(buffer));
    while (bWorking)
    {
      bWorking = finder.FindNextFile();
      string filename = TSLUTF8Encoder(finder.GetFilePath());
    
      string dataset = "";
      BOOL success = getDataset(filename, dataset);
      
      TSLUTF8Encoder sourceEncoder( source );
      string utf8Source = sourceEncoder;
      if (success && (dataset == utf8Source))
      {
        index++;
        char file[_MAX_FNAME], ext[_MAX_EXT];
        _splitpath (filename.c_str(), NULL, NULL, file, ext);
        m_project_list.InsertItem(index, (LPCTSTR)TSLUTF8Decoder(file), 0);
      }
    }
  }
}

BOOL CMISourceSelectionDlg::getDataset(string& filename, string& dataset)
{
  char buffer[256];
  string line = "";
  
  // stream in the DATASET name
  TSLUTF8Decoder filenameDecoder( filename );
  std::wstring wFilename = filenameDecoder;
  ifstream stream(wFilename.c_str());

  do
  {
    stream.getline(buffer, 256, ';');
    line = string(buffer);

  } while (!stream.eof() && (line.find("\nDATASET") == -1));

  if (stream.eof())
    return FALSE;

  // version
  stream.getline(buffer, 256, ';');

  // stuff to throw away
  stream.getline(buffer, 256, ';');
  stream.getline(buffer, 256, ';');
  
  // source name
  stream.getline(buffer, 256, ';');
  dataset = string(buffer);

  return TRUE;
}

BOOL CMISourceSelectionDlg::OnInitDialog() 
{
  CDialog::OnInitDialog();

  m_sources.clear();

  // create an image list
  if (m_itemImageList)
    m_itemImageList->DeleteImageList();
  
  m_itemImageList = new CImageList();
  m_itemImageList->Create(30, 32, ILC_COLOR24 | ILC_MASK, 24, 24);

  CBitmap bitmap;
  bitmap.LoadBitmap(IDB_MAPLINK);
  m_itemImageList->Add(&bitmap, RGB(255,255,255));
  bitmap.DeleteObject();

  m_project_list.SetImageList(m_itemImageList, TVSIL_NORMAL);

  // get templates directory
  string buffer = theApp.getMaplinkHome() + "\\projects\\AutomationTemplates\\*.mlp" ;

  int i = 0;
  CFileFind finder;
  BOOL bWorking = finder.FindFile( (LPCTSTR)TSLUTF8Decoder(buffer) );
  while (bWorking)
  {
    bWorking = finder.FindNextFile();
    string filename = TSLUTF8Encoder(finder.GetFilePath());

    string dataset = "";
    bool success = getDataset(filename, dataset);

    if (success)
    {    
      // check for duplicate
      bool exists = false;
      vector<string>::iterator itr;
      for (itr = m_sources.begin(); itr != m_sources.end(); itr++)
      {
        if ((*itr) == dataset)
        {
          exists = true;
          break;
        }
      }

      if (!exists)
      {
        m_sources.push_back(dataset);
        m_source_type.AddString((LPCTSTR)TSLUTF8Decoder(dataset));
        i++;
      }
    }
  }

  if (!i)
  {
    CString message = _T("No Project Templates exist in ");
    message += (LPCTSTR)TSLUTF8Decoder(theApp.getMaplinkHome());
    message += _T("\\projects\\AutomationTemplates  ");
    AfxMessageBox(message, MB_ICONERROR);
    OnCancel();
    //return FALSE;
  }

  m_source_type.SetCurSel(0);
  OnSelchangeMiSourceType();
	
  return TRUE;
}

void CMISourceSelectionDlg::OnClickMiProjectList(NMHDR* pNMHDR, LRESULT* pResult) 
{
  *pResult = 0;
}

void CMISourceSelectionDlg::OnDblclkMiProjectList(NMHDR* pNMHDR, LRESULT* pResult) 
{
  *pResult = 0;
  OnOK();
}

void CMISourceSelectionDlg::OnEditchangeMiSourceType() 
{
  OnSelchangeMiSourceType();	
}
