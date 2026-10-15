// MIProcessingControlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MapLinkStudioClient.h"
#include "MapLinkStudioClientView.h"
#include "MIProcessingControlDlg.h"
#include "MaplinkStudio.h"
#include "MIDataFilePropsDlg.h"

#include <comdef.h>
#include "safearray.h"

#include <algorithm>
#include <cctype>

#ifdef _MSC_VER >= 1900
# include <functional>
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMIProcessingControlDlg dialog


CMIProcessingControlDlg::CMIProcessingControlDlg(CMapLinkStudioClientView* view)
	: CDialog(CMIProcessingControlDlg::IDD)
{
  //{{AFX_DATA_INIT(CMIProcessingControlDlg)
  m_coordSystem = _T("");
  //}}AFX_DATA_INIT
  
  m_view = view;
  m_dataset = "";
  m_layerName = "";
  m_datafileInfo.clear();
  m_datasetInfo.dataset = _T("");
  m_datasetInfo.clipdata = false;;
  m_datasetInfo.tr_lat = 0.0;
  m_datasetInfo.tr_lon = 0.0;
  m_datasetInfo.bl_lat = 0.0;
  m_datasetInfo.bl_lon = 0.0;
  m_maplinkStudio = 0;
}


void CMIProcessingControlDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CMIProcessingControlDlg)
  DDX_Control(pDX, IDC_STATUS, m_status);
  DDX_Control(pDX, ID_MI_ADVANCED, m_bAdvanced);
  DDX_Control(pDX, IDC_MI_DATA_CORNERS_LATLON, m_cornersLatLong);
  DDX_Control(pDX, IDC_MI_DATA_CORNERS_CURRX, m_cornersCurrent);
  DDX_Control(pDX, IDC_MI_REMOVE_DATA, m_bRemove);
  DDX_Control(pDX, IDC_MI_EDIT_DATA, m_bEdit);
  DDX_Control(pDX, IDC_MI_DIR_BROWSER, m_outputBrowse);
  DDX_Control(pDX, ID_GENERATE_MAP, m_bGenerate);
  DDX_Control(pDX, IDC_MI_TEMPLATE_DISPLAY, m_template);
  DDX_Control(pDX, IDC_MI_SRC_TYPE_DISPLAY, m_source);
  DDX_Control(pDX, IDC_MI_OUTPUT_DIR, m_ap_output_dir);
  DDX_Control(pDX, IDC_MI_MAP_TR_LON, m_map_tr_lon);
  DDX_Control(pDX, IDC_MI_MAP_TR_LAT, m_map_tr_lat);
  DDX_Control(pDX, IDC_MI_MAP_BL_LON, m_map_bl_lon);
  DDX_Control(pDX, IDC_MI_MAP_BL_LAT, m_map_bl_lat);
  DDX_Control(pDX, IDC_MI_DATA_LIST, m_mi_data_list);
  DDX_Text(pDX, IDC_COORDSYSTEM, m_coordSystem);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMIProcessingControlDlg, CDialog)
  //{{AFX_MSG_MAP(CMIProcessingControlDlg)
  ON_BN_CLICKED(ID_GENERATE_MAP, OnGenerateMap)
  ON_BN_CLICKED(ID_MI_ADVANCED, OnMiAdvanced)
  ON_BN_CLICKED(IDC_MI_ADD_DATA, OnMiAddData)
  ON_BN_CLICKED(IDC_MI_EDIT_DATA, OnMiEditData)
  ON_BN_CLICKED(IDC_MI_REMOVE_DATA, OnMiRemoveData)
  ON_BN_CLICKED(IDC_MI_DIR_BROWSER, OnMiDirBrowser)
  ON_WM_CREATE()
  ON_LBN_SELCHANGE(IDC_MI_DATA_LIST, OnSelchangeMiDataList)
  ON_BN_CLICKED(IDC_MI_DATA_CORNERS_CURRX, OnMiDataCornersCurrx)
  ON_BN_CLICKED(IDC_MI_DATA_CORNERS_LATLON, OnMiDataCornersLatlon)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMIProcessingControlDlg message handlers

// GET THE LAYER NAME

string CMIProcessingControlDlg::getLayerName()
{
  _variant_t layerNames;
  string layerName = "";
  long numLayers = 0;

  numLayers = m_maplinkStudio->LayerNames(&layerNames);
  if (numLayers > 0)
  {  
    SafeArray<VARIANT, VT_VARIANT> allLayerNames(layerNames.parray);
    
    for (int index = 0; index < numLayers; ++index)
    {
      VARIANT layer = allLayerNames[index];
      
      _bstr_t text(layer.bstrVal);
      layerName = TSLUTF8Encoder((LPCTSTR)text);

      if (layerName.size())
        return layerName;
    }
  }

  return (layerName);
}

// GET LIBRARY NAMES

vector<string> CMIProcessingControlDlg::libraryNames()
{
  _variant_t libraryNames;
  string libraryName = "";
  vector<string> return_vector;
  long numLibraries = 0;

  IVPFPanel* pVPFPanel = new IVPFPanel(m_maplinkStudio->GetVPFPanel());
  
  pVPFPanel->Init();

  numLibraries = pVPFPanel->NumberOfLibraries();
  if (numLibraries > 0)
  {
    pVPFPanel->LibraryNames(&libraryNames);
    
    SafeArray<VARIANT, VT_VARIANT> allLibraryNames(libraryNames.parray);
    
    for (int index = 0; index < numLibraries; ++index)
    {
      VARIANT library = allLibraryNames[index];
      
      _bstr_t text(library.bstrVal);
      libraryName = TSLUTF8Encoder((LPCTSTR)text);

      if (libraryName.size())
      {
        return_vector.push_back(libraryName);
      }
    }
  }

  return (return_vector);
}

// GET RESOLUTION NAMES

vector<string> CMIProcessingControlDlg::resolutionNames()
{
  _variant_t resolutionNames;
  string resolutionName = "";
  vector<string> return_vector;
  long numResolutions = 0;

  IDTMPanelOcean* pDTMPanel = new IDTMPanelOcean(m_maplinkStudio->GetDTMPanelOcean());

  pDTMPanel->Init();

  numResolutions = pDTMPanel->NumberOfResolutions();
  if (numResolutions > 0)
  {  
    pDTMPanel->ResolutionNames(&resolutionNames);
    
    SafeArray<VARIANT, VT_VARIANT> allResolutionNames(resolutionNames.parray);
    
    for (int index = 0; index < numResolutions; ++index)
    {
      VARIANT resolution = allResolutionNames[index];
      
      _bstr_t text(resolution.bstrVal);
      resolutionName = TSLUTF8Encoder((LPCTSTR)text);

      if (resolutionName.size())
      {
        return_vector.push_back(resolutionName);
      }
    }
  }

  return (return_vector);
}

// GENERATE MAP

void CMIProcessingControlDlg::OnGenerateMap() 
{
  m_bGenerate.EnableWindow(FALSE);
  BeginWaitCursor();

  if (!theApp.getCoordinateSystem().size())
    m_maplinkStudio->MapTMCperMU(m_view->getMapTMCperMU());
  
  // first setup the coordinate system

  if (!m_coordSystem.IsEmpty())
    m_maplinkStudio->SetMapCoordinateSystem(m_coordSystem);

  CString output_map;
  m_ap_output_dir.GetWindowText(output_map);
  if (!output_map.IsEmpty())
  {
    if (m_datasetInfo.clipdata)
    {      
      // set the dataset input clipping
  
      m_status.SetWindowText(_T("Extent clipping (please wait).."));
   
      CString text;
      m_map_bl_lon.GetWindowText(text);
      double minX = _tstof(text);

      m_map_bl_lat.GetWindowText(text);
      double minY = _tstof(text);
      
      m_map_tr_lon.GetWindowText(text);
      double maxX = _tstof(text);
      
      m_map_tr_lat.GetWindowText(text);
      double maxY = _tstof(text);
    
      m_maplinkStudio->SetAllDatasetsInputClipping(minX, maxX, minY, maxY);
    }
  
    // generate the map file
  
    m_status.SetWindowText(_T("Creating Map (please wait).."));

    if (m_maplinkStudio->CreateMap(output_map))
    {
      CString map_created = output_map;
     
      m_view->addMapToSurface(output_map);
      map_created += _T(" Created Successfully  ");

      AfxMessageBox(map_created);
    }
    else
    {
      CString error = "ERROR Generating " + output_map  + "  ";
      AfxMessageBox(error);
    }      
  }
  else
  {    
    AfxMessageBox(_T("Please Specify an Output Map"));
  }
  
  m_status.SetWindowText(_T(""));
  m_bGenerate.EnableWindow(TRUE);
  EndWaitCursor();
}

// ADVANCED OPTIONS

void CMIProcessingControlDlg::OnMiAdvanced() 
{
  // allow the use of Maplink Studio
  m_maplinkStudio->SetBatchProcessing(FALSE);
  m_maplinkStudio->SetVisible(TRUE);
  m_bGenerate.EnableWindow(FALSE);
}

// ADD DATA

void CMIProcessingControlDlg::OnMiAddData() 
{
  string filename = theApp.getMaplinkIniFile().c_str();
  string section = theApp.getSourceName().c_str();
  char value[128];

  TSLProfileHelper::lookupProfile(filename.c_str(), section.c_str(), "FILEDIALOGFILTER", value, 128, "");

  string filter = value;
  if (filter.size())
  {
    // Open a multiselect dialog
    CFileDialog dlg(TRUE, NULL, 0, OFN_ALLOWMULTISELECT, (LPCTSTR)TSLUTF8Decoder(filter));
    TCHAR files[8192];
    dlg.m_ofn.lpstrFile = files;
    dlg.m_ofn.nMaxFile = 8192;
  
    CString dialogTitle = _T("Select ");
    dialogTitle += (LPCTSTR)TSLUTF8Decoder(theApp.getSourceName());
    dialogTitle += _T(" Data files to Add");
    dlg.m_ofn.lpstrTitle = dialogTitle;
  
    files[0] = '\0';

    if (dlg.DoModal() != IDOK)
      return;

    // add the files to the project

    BeginWaitCursor();
    m_status.SetWindowText(_T("Loading data (please wait).."));

    POSITION pos;
    pos = dlg.GetStartPosition();
    while (pos)
    { 
      CString datafile = dlg.GetNextPathName(pos);
      string thisFile = TSLUTF8Encoder( datafile );
      m_mi_data_list.AddString(datafile);
      
      // add file to layer

      if (!datafile.IsEmpty())
      {
        char fname[_MAX_FNAME];
        char ext[_MAX_EXT];

        // Automation converts the path to lower case, 
        // we need to do the same for SelectData to work
        transform(thisFile.begin(), thisFile.end(), thisFile.begin(), [](unsigned char c) { return std::tolower(c); });
        _splitpath(thisFile.c_str(), NULL, NULL, fname, ext);
        
        m_maplinkStudio->AddDataToDataset(datafile, (LPCTSTR)TSLUTF8Decoder(m_layerName), (LPCTSTR)TSLUTF8Decoder(m_dataset));

        // Select the Dataset.
        m_maplinkStudio->SelectDataset((LPCTSTR)TSLUTF8Decoder(m_layerName), (LPCTSTR)TSLUTF8Decoder(m_dataset));
        
        // Select the Data - this allows us to query any data that is stored as part of the data configuration.
        string dataName(fname);
        if (strlen(ext) != 0)
        {
          dataName += ".";
          dataName += ext;
        }
        m_maplinkStudio->SelectData((LPCTSTR)TSLUTF8Decoder(m_layerName), (LPCTSTR)TSLUTF8Decoder(m_dataset), (LPCTSTR)TSLUTF8Decoder(dataName));

        // create and add to the datafileInfo vector

        datafileInfo data;
        data.full_path = datafile;
        data.datafile += (LPCTSTR)TSLUTF8Decoder(fname);
        data.datafile += (LPCTSTR)TSLUTF8Decoder(ext);
      
        double* minX = new double(0.0);
        double* minY = new double(0.0);
        double* maxX = new double(0.0);
        double* maxY = new double(0.0);

        m_maplinkStudio->DataInputExtent((LPCTSTR)TSLUTF8Decoder(m_layerName), (LPCTSTR)TSLUTF8Decoder(m_dataset), data.datafile, minX, minY, maxX, maxY);
      
        data.tr_lat = (*maxY);
        data.tr_lon = (*maxX);
        data.bl_lat = (*minY);
        data.bl_lon = (*minX);

        m_datafileInfo.push_back(data);
      }
    }

    EndWaitCursor();

    m_view->drawExtentPolygons(m_datafileInfo);
    
    m_ap_output_dir.EnableWindow(TRUE);
    m_outputBrowse.EnableWindow(TRUE);
  }
  else
  {
    CString message = _T("Could not find FILEDIALOGFILTER in Mapl.ini for ");
    message += (LPCTSTR)TSLUTF8Decoder(theApp.getSourceName());
    AfxMessageBox(message);
  }

  m_status.SetWindowText(_T(""));
}

// EDIT DATA

void CMIProcessingControlDlg::OnMiEditData() 
{
  int curSel = m_mi_data_list.GetCurSel();

  if (curSel != LB_ERR)
  {
    CString filename;
    m_mi_data_list.GetText(curSel, filename);

    // get the datafileInfo structure
    bool found = false;
    datafileInfo fileInfo;
    vector<datafileInfo>::iterator itr;
    for (itr = m_datafileInfo.begin(); itr != m_datafileInfo.end(); itr++)
    {
      if ((*itr).full_path == filename)
      {
        fileInfo = (*itr);
        found = true;
        break;
      }
    }

    if (found)
    {
      vector<string> libNames = libraryNames();
      vector<string> resNames = resolutionNames();
      CMIDataFilePropsDlg* dlg = new CMIDataFilePropsDlg(0, fileInfo, libNames, resNames);
      dlg->DoModal();
    }
  }	
}

// REMOVE DATA

void CMIProcessingControlDlg::OnMiRemoveData() 
{
  int curSel = m_mi_data_list.GetCurSel();

  if (curSel != LB_ERR)
  {
    CString filename;
    m_mi_data_list.GetText(curSel, filename);
    m_mi_data_list.DeleteString(curSel);

    // remove from DatafileInfo vector
    vector<datafileInfo>::iterator itr;
    for (itr = m_datafileInfo.begin(); itr != m_datafileInfo.end(); itr++)
    {
      if ((*itr).full_path == filename)
      {
        m_datafileInfo.erase(itr);
        break;
      }
    }
  }

  m_view->drawExtentPolygons(m_datafileInfo);

  m_bEdit.EnableWindow(FALSE);
  m_bRemove.EnableWindow(FALSE);
}

// BROWSE FOR OUTPUT MAP

void CMIProcessingControlDlg::OnMiDirBrowser() 
{
  string filename = theApp.getMaplinkIniFile();
  string section = theApp.getSourceName();
  char value[128];

  TSLProfileHelper::lookupProfile(filename.c_str(), section.c_str(), "FILEDIALOGFILTER", value, 128, "");

  string filter = value;
  if (filter.size())
  {  
    // Open a file Save As dialogue
    string default_filename = theApp.getTemplateName() + string(".map");
    CFileDialog dlg(FALSE, _T(".map"), (LPCTSTR)TSLUTF8Decoder(default_filename), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Map Files (*.map)|*.map||"));
    dlg.m_ofn.lpstrTitle = _T("Select location for Output Map file");

    if (dlg.DoModal() != IDOK)
      return;

    POSITION pos;
    pos = dlg.GetStartPosition();
    while (pos)
    {
      CString thisFile = dlg.GetNextPathName(pos);
      m_ap_output_dir.SetWindowText(thisFile);
    }

    m_bGenerate.EnableWindow(TRUE);
  }
  else
  {
    CString message = _T("Could not find FILEDIALOGFILTER in Mapl.ini for ");
    message += (LPCTSTR)TSLUTF8Decoder(theApp.getSourceName());
    AfxMessageBox(message);
  }
}

int CMIProcessingControlDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
  if (CDialog::OnCreate(lpCreateStruct) == -1)
    return -1;
		
  return 0;
}

BOOL CMIProcessingControlDlg::OnInitDialog() 
{
  CDialog::OnInitDialog();

  m_dataset = string("[ ") + theApp.getSourceName() + string(" ]");
  m_source.SetWindowText((LPCTSTR)TSLUTF8Decoder(m_dataset));
  m_template.SetWindowText((LPCTSTR)TSLUTF8Decoder(theApp.getTemplateName()));
  m_cornersCurrent.SetCheck(TRUE);
  m_cornersLatLong.SetCheck(FALSE);
  m_layerName = getLayerName();

  if (!m_layerName.size())
  {
    AfxMessageBox(_T("No Layer found in project Template"));
    OnOK();
  }

  m_datasetInfo.dataset = m_dataset.c_str();
  m_datasetInfo.clipdata = false;
  m_datasetInfo.tr_lat = 0.0;
  m_datasetInfo.tr_lon = 0.0;
  m_datasetInfo.bl_lat = 0.0;
  m_datasetInfo.bl_lon = 0.0;
  
  m_bAdvanced.EnableWindow(TRUE);

  UpdateData(FALSE);

  return TRUE;
}

void CMIProcessingControlDlg::StartMaplink()
{
  BeginWaitCursor();

  m_maplinkStudio = new IMapLinkStudio();
  m_maplinkStudio->CreateDispatch(_T("MapLink.Studio.2"));
  m_maplinkStudio->SetBatchProcessing(TRUE);

  m_maplinkStudio->OpenProject((LPCTSTR)TSLUTF8Decoder(theApp.getProjectFile()));

  EndWaitCursor();
}

void CMIProcessingControlDlg::OnOK() 
{
  m_maplinkStudio->Quit();
  m_datafileInfo.clear();
  m_view->drawExtentPolygons(m_datafileInfo);
  delete m_maplinkStudio;
  m_maplinkStudio = 0;
  CDialog::OnOK();
}

void CMIProcessingControlDlg::OnSelchangeMiDataList() 
{
  int curSel = m_mi_data_list.GetCurSel();

  if (curSel != LB_ERR)
  {
    m_bEdit.EnableWindow(TRUE);
    m_bRemove.EnableWindow(TRUE);
  }	
}

void CMIProcessingControlDlg::OnMiDataCornersCurrx() 
{
  m_map_bl_lat.EnableWindow(FALSE);
  m_map_bl_lon.EnableWindow(FALSE);
  m_map_tr_lat.EnableWindow(FALSE);
  m_map_tr_lon.EnableWindow(FALSE);

  m_datasetInfo.clipdata = false;
}

void CMIProcessingControlDlg::OnMiDataCornersLatlon() 
{
  m_map_bl_lat.EnableWindow(TRUE);
  m_map_bl_lon.EnableWindow(TRUE);
  m_map_tr_lat.EnableWindow(TRUE);
  m_map_tr_lon.EnableWindow(TRUE);
  
  m_datasetInfo.clipdata = true;
}

void CMIProcessingControlDlg::SetCoordSystemName(string coordSystemName)
{
  m_coordSystem = coordSystemName.c_str();
}
