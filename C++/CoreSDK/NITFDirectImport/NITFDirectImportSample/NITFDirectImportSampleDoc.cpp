// NITFDirectImportSampleDoc.cpp : implementation of the NITFDirectImportSampleDoc class
//

#include "stdafx.h"
#include "resource.h"
#include "mainfrm.h"
#include ".\nitfdirectimportSampledoc.h"
#include "progressdialog.h"
#include "metadatadlg.h"
#include "displayitemsettings.h"
#include "directoriesdlg.h"
#include "settingsdlg.h"
#include "savedialog.h"
#include "tslfilterprocess.h"
#include "pyramidingsettingsdlg.h"
#include "clippingdlg.h"
#include "geodeticclippingdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NITFDirectImportSampleDoc

const char * NITFDirectImportSampleDoc::m_mapLayerName = "map" ;
const char * NITFDirectImportSampleDoc::m_filterLayerName = "nitf";

CString NITFDirectImportSampleDoc::m_filterDLL = _T("NITFFILTER");
CString NITFDirectImportSampleDoc::m_unlockCode = _T("");
CString NITFDirectImportSampleDoc::m_configFile = _T("");
bool NITFDirectImportSampleDoc::m_alreadyUnlocked = false;

static TCHAR startDirBuffer[MAX_PATH];

IMPLEMENT_DYNCREATE(NITFDirectImportSampleDoc, CDocument)

BEGIN_MESSAGE_MAP(NITFDirectImportSampleDoc, CDocument)
//{{AFX_MSG_MAP(NITFDirectImportSampleDoc)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG_MAP
ON_COMMAND(ID_FILE_SAVE_ALL, OnFileSaveAll)
ON_UPDATE_COMMAND_UI(ID_VIEW_PROGRESS, OnUpdateViewProgress)
ON_COMMAND(ID_VIEW_PROGRESS, OnViewProgress)
ON_COMMAND(ID_VIEW_METADATAINFO, OnViewMetadatainfo)
ON_UPDATE_COMMAND_UI(ID_VIEW_METADATAINFO, OnUpdateViewMetadatainfo)
ON_COMMAND(ID_VIEW_DISPLAYITEMSETTINGSBEFOREPROCESS, OnViewDisplayitemsettingsbeforeprocess)
ON_UPDATE_COMMAND_UI(ID_VIEW_DISPLAYITEMSETTINGSBEFOREPROCESS, OnUpdateViewDisplayitemsettingsbeforeprocess)
ON_COMMAND(ID_TOOLS_DISPLAYITEMSETTINGS, OnToolsDisplayitemsettings)
ON_UPDATE_COMMAND_UI(ID_TOOLS_DISPLAYITEMSETTINGS, OnUpdateToolsDisplayitemsettings)
ON_COMMAND(ID_TOOLS_DIRECTORIES, OnToolsDirectories)
ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_ALL, OnUpdateFileSave)
ON_UPDATE_COMMAND_UI(ID_FILE_SAVEITEM, OnUpdateFileSave)
ON_COMMAND(ID_FILE_SAVEITEM, OnFileSaveitem)
ON_COMMAND(ID_TOOLS_SETTINGS, OnToolsSettings)
ON_UPDATE_COMMAND_UI(ID_TOOLS_SETTINGS, OnUpdateToolsSettings)
ON_COMMAND(ID_SETTINGS_PYRAMIDRASTERS, OnSettingsPyramidrasters)
ON_UPDATE_COMMAND_UI(ID_SETTINGS_PYRAMIDRASTERS, OnUpdateSettingsPyramidrasters)
ON_COMMAND(ID_SETTINGS_PYRAMIDINGSETTINGS, OnSettingsPyramidingsettings)
ON_UPDATE_COMMAND_UI(ID_SETTINGS_PYRAMIDINGSETTINGS, OnUpdateSettingsPyramidingsettings)
ON_COMMAND(ID_CLIPPING_GEODETICCLIPPING, OnClippingGeodeticclipping)
ON_COMMAND(ID_CLIPPING_OUTPUTCLIPPING, OnClippingOutputclipping)
ON_UPDATE_COMMAND_UI(ID_CLIPPING_GEODETICCLIPPING, OnUpdateClipping)
ON_UPDATE_COMMAND_UI(ID_CLIPPING_OUTPUTCLIPPING, OnUpdateClipping)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NITFDirectImportSampleDoc construction/destruction

NITFDirectImportSampleDoc::NITFDirectImportSampleDoc()
: m_mapDataLayer(0)
, m_filterDataLayer(0)
, m_progressDialog(0)
, m_metadatadlg(0)
, m_showMetadataDlg(false)
, m_showProgressBar(false)
, m_showItemSettingsBeforeDraw(false)
, m_pyramidRasters(false)
, m_interpolation(TSLRasterInterpolationBilinear)
, m_rtype(TSLRasterTypeTiff)
, m_multipage(false)
, m_compressionFactor(2)
, m_attachmentsDir("")
, m_outputDir("")
, m_tempDirectory("")
{
  ::GetCurrentDirectory(255, &startDirBuffer[0]);
}

NITFDirectImportSampleDoc::~NITFDirectImportSampleDoc()
{
  if ( m_mapDataLayer )
  {
    m_mapDataLayer->destroy() ;
    m_mapDataLayer = 0 ;
  }
  if ( m_metadatadlg )
  {
    m_metadatadlg->DestroyWindow();
    delete m_metadatadlg;
    m_metadatadlg = 0;
  }
}

/////////////////////////////////////////////////////////////////////////////
// NITFDirectImportSampleDoc diagnostics

#ifdef _DEBUG
void NITFDirectImportSampleDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void NITFDirectImportSampleDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// NITFDirectImportSampleDoc commands

BOOL NITFDirectImportSampleDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
  if (!CDocument::OnOpenDocument(lpszPathName))
    return FALSE;
 
  ::SetCurrentDirectory(&startDirBuffer[0]);

  // Create a map data layer
  TSLUTF8Encoder configFileStr(m_configFile);
  m_filterDataLayer = new TSLNITFFilterDataLayer( TSLUTF8Encoder(m_filterDLL), m_configFile.IsEmpty() ? NULL : (const char*)configFileStr );

  m_alreadyUnlocked = TSLNITFFilterDataLayer::unlockSupport(TSLUTF8Encoder(m_unlockCode));

  // Clear the error stack, load the map then check for errors.
  TSLErrorStack::clear() ;
  
  if ( m_progressDialog )
  {
    m_progressDialog->DestroyWindow();
    delete m_progressDialog;
    m_progressDialog= 0;
  }

  if ( m_showProgressBar )
  {
    m_progressDialog = new CProgressDialog(0);
    m_progressDialog->Create(IDD_PROGRESS_DIALOG, ::AfxGetApp()->GetMainWnd());
    m_progressDialog->ShowWindow(SW_SHOW);
    m_filterDataLayer->setCallback((TSLFilterProgressCallback)&CProgressDialog::staticUserCallback, m_progressDialog);
  }
  else
  {
    m_filterDataLayer->setCallback(0, 0);
  }

  if ( m_pyramidRasters )
  {
    m_filterDataLayer->flags( m_filterDataLayer->flags() | TSLFilterProcessRasterPyramid );
    m_filterDataLayer->rasterPyramidOptions(m_interpolation, m_multipage, m_rtype, m_compressionFactor);
  }
  else
  {
    m_filterDataLayer->flags( m_filterDataLayer->flags() & ~TSLFilterProcessRasterPyramid );
  }
  m_filterDataLayer->setDirectories( TSLUTF8Encoder( m_attachmentsDir ),
                                     TSLUTF8Encoder( m_outputDir ),
                                     TSLUTF8Encoder( m_tempDirectory ) );
  m_filterDataLayer->loadData( TSLUTF8Encoder(lpszPathName) );
  
  // Display any errors that have occurred
  const char * msg = TSLErrorStack::errorString( "Cannot load nitf data\n", TSLErrorCategoryError ) ;
  if ( msg )
  {
    // The map load failed, so delete the data layer and path list
    AfxMessageBox( (LPCTSTR)TSLUTF8Decoder(msg), MB_ICONERROR ) ;
  }

  int noDisItems = m_filterDataLayer->numDisplayItems();
  ((CMainFrame *)(AfxGetMainWnd()))->setNoDisplayItems(noDisItems);

  if ( m_showItemSettingsBeforeDraw )
  {
    CDisplayItemSettings * dlg = new CDisplayItemSettings(m_filterDataLayer, ::AfxGetApp()->GetMainWnd());
    dlg->DoModal();
    delete dlg;
  }
  if ( m_showMetadataDlg )
  {
    m_metadatadlg->updateFromDataLayer(m_filterDataLayer);
  }
  
  msg = TSLErrorStack::errorString( "Cannot load nitf data\n", TSLErrorCategoryError ) ;
  if ( msg )
  {
    // The map load failed, so delete the data layer and path list
    AfxMessageBox( (LPCTSTR)TSLUTF8Decoder(msg), MB_ICONERROR ) ;
  }

  return TRUE;
}

bool NITFDirectImportSampleDoc::addToSurface( TSLDrawingSurface * surface )
{
  if ( !m_metadatadlg )
  {
    m_metadatadlg = new CMetadataDlg(m_showMetadataDlg, 0);
    m_metadatadlg->Create(IDD_METADATA_DLG, ::AfxGetApp()->GetMainWnd());
  }
  if ( !m_mapDataLayer && !m_filterDataLayer  )
    return false;

  //Add the NITF data layer to the drawing surface
  if ( m_filterDataLayer )
  {
    surface->addDataLayer( m_filterDataLayer, m_filterLayerName ) ;
  }

  // Add the map to the specified drawing surface,
  if ( m_mapDataLayer )
  {
    surface->addDataLayer( m_mapDataLayer, m_mapLayerName ) ;
  }

  return true ;
}

bool NITFDirectImportSampleDoc::detachFromSurface( TSLDrawingSurface * surface )
{
  if ( !m_mapDataLayer && !m_filterDataLayer  )
    return false;

  if ( m_filterDataLayer )
  {
    surface->removeDataLayer( m_filterLayerName ) ;
  }

  if ( m_mapDataLayer )
  {
    // Remove the map to the specified drawing surface,
    surface->removeDataLayer( m_mapLayerName ) ;
  }

  return true ;
}

void NITFDirectImportSampleDoc::DeleteContents() 
{
  // Clean up by destroying the map and pathlist
  if ( m_filterDataLayer )
  {
    m_filterDataLayer->destroy();
    m_filterDataLayer = 0 ;
  }
	
  CDocument::DeleteContents();
}

bool NITFDirectImportSampleDoc::openMap(const char* mapPath)
{
  if ( !m_mapDataLayer )
  {
    m_mapDataLayer = new TSLMapDataLayer();
  }

  // Clear the error stack, load the map then check for errors.
  TSLErrorStack::clear() ;

  // Load the map
  // This will automatically load all associated files such as 
  // the .pth and .pal files.  These needed to be loaded
  // directly by the application in earlier versions of MapLink
  m_mapDataLayer->loadData( mapPath ) ;

  // Display any errors that have occurred
  const char * msg = TSLErrorStack::errorString( "Cannot load map\n" ) ;
  if ( msg )
  {
    // The map load failed, so delete the data layer and path list
    AfxMessageBox( (LPCTSTR)TSLUTF8Decoder(msg), MB_ICONERROR ) ;
    m_mapDataLayer->destroy() ;
    m_mapDataLayer = 0 ;
  }

  return true;
}

void NITFDirectImportSampleDoc::OnFileSaveAll()
{
  CSaveDialog * dlg = new CSaveDialog(m_filterDataLayer, false, ::AfxGetApp()->GetMainWnd());
  dlg->DoModal();
  delete dlg;
}


void NITFDirectImportSampleDoc::OnUpdateViewProgress(CCmdUI *pCmdUI)
{
  pCmdUI->Enable();
  pCmdUI->SetCheck(m_showProgressBar);
}

void NITFDirectImportSampleDoc::OnViewProgress()
{
  m_showProgressBar = !m_showProgressBar;
}

void NITFDirectImportSampleDoc::OnViewMetadatainfo()
{
  m_showMetadataDlg = !m_showMetadataDlg;
  if ( m_showMetadataDlg )
  {
    m_metadatadlg->updateFromDataLayer(m_filterDataLayer);
    m_metadatadlg->ShowWindow(SW_SHOW);
  }
  else
  {
    m_metadatadlg->ShowWindow(SW_HIDE);
  } 
}

void NITFDirectImportSampleDoc::OnUpdateViewMetadatainfo(CCmdUI *pCmdUI)
{
  pCmdUI->Enable();
  pCmdUI->SetCheck(m_showMetadataDlg);
}

void NITFDirectImportSampleDoc::OnViewDisplayitemsettingsbeforeprocess()
{
  m_showItemSettingsBeforeDraw = !m_showItemSettingsBeforeDraw;
}

void NITFDirectImportSampleDoc::OnUpdateViewDisplayitemsettingsbeforeprocess(CCmdUI *pCmdUI)
{
  pCmdUI->Enable();
  pCmdUI->SetCheck(m_showItemSettingsBeforeDraw);
}

void NITFDirectImportSampleDoc::OnToolsDisplayitemsettings()
{
  CDisplayItemSettings * dlg = new CDisplayItemSettings(m_filterDataLayer, ::AfxGetApp()->GetMainWnd());
  dlg->DoModal();
  delete dlg;
  ::AfxGetApp()->GetMainWnd()->GetTopWindow()->Invalidate();
}

void NITFDirectImportSampleDoc::OnUpdateToolsDisplayitemsettings(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(m_filterDataLayer != 0);
}

void NITFDirectImportSampleDoc::OnToolsDirectories()
{
  CDirectoriesDlg * dlg = new CDirectoriesDlg(m_attachmentsDir, m_outputDir, m_tempDirectory, ::AfxGetApp()->GetMainWnd());
  dlg->DoModal();
  delete dlg;
}

void NITFDirectImportSampleDoc::OnUpdateFileSave(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(m_filterDataLayer != 0);
}

void NITFDirectImportSampleDoc::OnFileSaveitem()
{
  CSaveDialog * dlg = new CSaveDialog(m_filterDataLayer, true, ::AfxGetApp()->GetMainWnd());
  dlg->DoModal();
  delete dlg;
}

void NITFDirectImportSampleDoc::OnToolsSettings()
{
#ifdef HIDE_UNLOCK_CODE
  bool hideUnlockCode = true;
#else
  bool hideUnlockCode = false;
#endif
  CSettingsDlg * dlg = new CSettingsDlg(m_filterDLL, m_unlockCode, m_configFile, m_alreadyUnlocked, hideUnlockCode, ::AfxGetApp()->GetMainWnd());
  dlg->DoModal();
  delete dlg;
}

void NITFDirectImportSampleDoc::OnUpdateToolsSettings(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(m_filterDataLayer == 0);
}

void NITFDirectImportSampleDoc::OnSettingsPyramidrasters()
{
  m_pyramidRasters = !m_pyramidRasters;
}

void NITFDirectImportSampleDoc::OnUpdateSettingsPyramidrasters(CCmdUI *pCmdUI)
{
  pCmdUI->SetCheck(m_pyramidRasters);
}

void NITFDirectImportSampleDoc::OnSettingsPyramidingsettings()
{
  CPyramidingSettingsDlg * dlg = new CPyramidingSettingsDlg(m_interpolation, m_multipage, m_rtype, m_compressionFactor, ::AfxGetApp()->GetMainWnd());
  dlg->DoModal();
  delete dlg;

}

void NITFDirectImportSampleDoc::OnUpdateSettingsPyramidingsettings(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(m_pyramidRasters);
}

void NITFDirectImportSampleDoc::OnClippingGeodeticclipping()
{
  bool apply;
  double minLat, maxLat, refMer, degEast, degWest;
  m_filterDataLayer->getGeodeticClipping(apply, &minLat, &maxLat, &refMer, &degEast, &degWest);
  if ( !apply )
  {
    minLat = -90.0;
    maxLat = 90.0;
    refMer = 0.0;
    degEast = 180.0;
    degWest = 180.0;
  }
  CGeodeticClippingDlg * dlg = new CGeodeticClippingDlg(apply, minLat, maxLat, refMer, degEast, degWest, ::AfxGetApp()->GetMainWnd());
  dlg->DoModal();
  delete dlg;

  m_filterDataLayer->setGeodeticClipping(apply, minLat, maxLat, refMer, degEast, degWest);
  ::AfxGetApp()->GetMainWnd()->Invalidate();
}

void NITFDirectImportSampleDoc::OnClippingOutputclipping()
{
  bool apply;
  double minX, minY, maxX, maxY;
  m_filterDataLayer->getOutputClipping(apply, &minX, &minY, &maxX, &maxY);
  if ( !apply )
  {
    TSLTMC lminX, lminY, lmaxX, lmaxY;
    m_filterDataLayer->getTMCExtent(&lminX, &lminY, &lmaxX, &lmaxY);
    m_filterDataLayer->getCoordinateSystem()->TMCToMU(lminX, lminY, &minX, &minY);
    m_filterDataLayer->getCoordinateSystem()->TMCToMU(lmaxX, lmaxY, &maxX, &maxY);
  }
  CClippingDlg * dlg = new CClippingDlg(apply, minX, minY, maxX, maxY, ::AfxGetApp()->GetMainWnd());
  dlg->DoModal();
  delete dlg;

  m_filterDataLayer->setOutputClipping(apply, minX, minY, maxX, maxY);
  ::AfxGetApp()->GetMainWnd()->Invalidate();

}

void NITFDirectImportSampleDoc::OnUpdateClipping(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(m_filterDataLayer != 0);
}


