// WMSClientSampleDoc.cpp : implementation of the WMSClientSampleDoc class
//

#include "stdafx.h"
#include "MainFrm.h"
#include "WMSClientSample.h"
#include "WMSClientSampleDoc.h"
#include "WMSClientSampleView.h"
#include "OpenServiceDlg.h"
#include "OpenWMTSDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// WMSClientSampleDoc

int NamedDataLayer::m_totalLayersCreated = 0;

IMPLEMENT_DYNCREATE(WMSClientSampleDoc, CDocument)

BEGIN_MESSAGE_MAP(WMSClientSampleDoc, CDocument)
//{{AFX_MSG_MAP(WMSClientSampleDoc)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// WMSClientSampleDoc construction/destruction

WMSClientSampleDoc::WMSClientSampleDoc()
  : m_remoteLoader(NULL)
  , m_wmsCallbacks()
  , m_wmtsCallbacks()
  , m_mapDataLayer(NULL)
  , m_dataLayers()
{
  m_remoteLoader = new TSLFileLoaderRemote();  

  // Find the number of Core's and set the number of concurrent requests
  // based on this value.
  SYSTEM_INFO sysinfo;
  GetSystemInfo( &sysinfo );

  int numCPU = sysinfo.dwNumberOfProcessors;

  // The actual multiplier effect will depend on the load on the WMS server.
  m_remoteLoader->maximumConcurrentRequests(numCPU * 4);
}

WMSClientSampleDoc::~WMSClientSampleDoc()
{
  // order of deletion is important: callbacks should be deleted last
  if ( m_remoteLoader )
  {
    m_remoteLoader->destroy();
    m_remoteLoader = 0;
  }
}

/////////////////////////////////////////////////////////////////////////////
// WMSClientSampleDoc diagnostics

#ifdef _DEBUG
void WMSClientSampleDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void WMSClientSampleDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// WMSClientSampleDoc commands

BOOL WMSClientSampleDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
  //if (!CDocument::OnOpenDocument(lpszPathName))
  //  return FALSE;
  openWMSLayer();
  return TRUE;
}

void WMSClientSampleDoc::openWMSLayer() 
{
  DeleteContents();
  SetTitle(_T("Untitled"));
  addWMSLayer();
}

void WMSClientSampleDoc::openWMTSLayer() 
{
  DeleteContents();
  SetTitle(_T("Untitled"));
  addWMTSLayer();
}

NamedDataLayer* WMSClientSampleDoc::addWMSLayer() 
{
  COpenServiceDlg openDlg(::AfxGetMainWnd());

  // Callbacks to handle the WMS Server Client setup and negotiation.
  m_wmsCallbacks.setOpenDlg(&openDlg);

  // WMS Client layer.
  TSLWMSDataLayer* wmsDataLayer = new TSLWMSDataLayer(&m_wmsCallbacks);

  // If we have a map data-layer we need to match the settings to ensure that
  // all the data lines up.
  setupLinearTransformParameters(wmsDataLayer);

  // sets the basic layer settings 
  refreshLayerSettings(wmsDataLayer);

  void* hWnd = (void*)AfxGetMainWnd()->m_hWnd;
  wmsDataLayer->addLoader(m_remoteLoader, TileLoaderCallback, hWnd, AllTilesLoadedCallback, hWnd);

  // Synchronous loading while negotiating and setting up the WMS client layer.
  wmsDataLayer->synchronousLoadStrategy(false);
  
  openDlg.setDataLayer(wmsDataLayer);
  if ( openDlg.DoModal() != IDOK )
  {
    wmsDataLayer->destroy();
    m_wmsCallbacks.setOpenDlg(0);
    return 0;
  }

  m_dataLayers.push_back( NamedDataLayer(wmsDataLayer, openDlg.chosenURL()) );
  m_wmsCallbacks.setOpenDlg(0);
  return &m_dataLayers.back();
}

NamedDataLayer* WMSClientSampleDoc::addWMTSLayer() 
{
  COpenWMTSDlg openDlg(::AfxGetMainWnd());

  // Callbacks to handle the WMS Server Client setup and negotiation.
  m_wmtsCallbacks.setOpenDlg(&openDlg);

  // WMTS Client layer.
  TSLWMTSDataLayer* wmtsDataLayer = new TSLWMTSDataLayer(&m_wmtsCallbacks);

  // If we have a map data-layer we need to match the settings to ensure that
  // all the data lines up.
  setupLinearTransformParameters(wmtsDataLayer);

  // sets the basic layer settings 
  refreshLayerSettings(wmtsDataLayer);

  void* hWnd = (void*)AfxGetMainWnd()->m_hWnd;
  wmtsDataLayer->addLoader(m_remoteLoader, TileLoaderCallback, hWnd, AllTilesLoadedCallback, hWnd);

  // Synchronous loading while negotiating and setting up the WMS client layer.
  wmtsDataLayer->synchronousLoadStrategy(false);
  
  openDlg.setDataLayer(wmtsDataLayer);
  if ( openDlg.DoModal() != IDOK )
  {
    wmtsDataLayer->destroy();
    m_wmtsCallbacks.setOpenDlg(0);
    return 0;
  }

  m_dataLayers.push_back( NamedDataLayer(wmtsDataLayer, openDlg.chosenURL()) );
  m_wmtsCallbacks.setOpenDlg(0);
  return &m_dataLayers.back();
}

NamedDataLayer* WMSClientSampleDoc::addMapLayer()
{
  CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER | OFN_ENABLESIZING | OFN_NOCHANGEDIR,
                  _T("Map Files (*.map)|*.map|All Files (*.*)|*.*||") );

  if ( dlg.DoModal() != IDOK )
    return 0;

  CString filepath = dlg.GetPathName();

  if (m_mapDataLayer)
  {
    m_mapDataLayer->dataLayer()->destroy();
    delete m_mapDataLayer;
    m_mapDataLayer = 0;
  }

  TSLMapDataLayer* mapDataLayer = new TSLMapDataLayer();
  
  void* hWnd = (void*)AfxGetMainWnd()->m_hWnd;
  mapDataLayer->addLoader(m_remoteLoader, TileLoaderCallback, hWnd, AllTilesLoadedCallback, hWnd);

  TSLErrorStack::clear() ;

  mapDataLayer->loadData( TSLUTF8Encoder(filepath) ) ;

  // Display any errors that have occurred
  const char * msg = TSLErrorStack::errorString( "Cannot load map\n" ) ;
  if ( msg )
  {
    AfxMessageBox( (LPCTSTR)TSLUTF8Decoder(msg), MB_ICONERROR ) ;
    mapDataLayer->destroy() ;
    return 0;
  }

  m_mapDataLayer = new NamedDataLayer( mapDataLayer );
  return m_mapDataLayer;
}

bool WMSClientSampleDoc::addToSurface( TSLDrawingSurface * surface )
{
  // Add the layers to the specified drawing surface

  if (m_mapDataLayer)
  {
    surface->addDataLayer( m_mapDataLayer->dataLayer(), m_mapDataLayer->genMapLinkLayerName().c_str() );
  }

  int n = m_dataLayers.size();
  for( int j = 0; j < n; ++j )
  {
    surface->addDataLayer( m_dataLayers[j].dataLayer(), m_dataLayers[j].genMapLinkLayerName().c_str() );
  }

  return (n > 0) || m_mapDataLayer;
}

bool WMSClientSampleDoc::detachFromSurface( TSLDrawingSurface * surface )
{
  // Remove the layers from the specified drawing surface

  if (m_mapDataLayer)
  {
    surface->removeDataLayer( m_mapDataLayer->genMapLinkLayerName().c_str() );
  }

  int n = m_dataLayers.size();
  for( int j = 0; j < n; ++j )
  {
    surface->removeDataLayer( m_dataLayers[j].genMapLinkLayerName().c_str() );
  }

  return (n > 0) || m_mapDataLayer;
}

void WMSClientSampleDoc::DeleteContents() 
{
  if (m_mapDataLayer)
  {
    m_mapDataLayer->dataLayer()->destroy();
    delete m_mapDataLayer;
    m_mapDataLayer = 0;
  }

  int n = m_dataLayers.size();
  for( int j = 0; j < n; ++j )
  {
    m_dataLayers[j].dataLayer()->destroy();
  }
  m_dataLayers.clear();

  CDocument::DeleteContents();
}


void WMSClientSampleDoc::AllTilesLoadedCallback (void* arg2)
{
  PostMessage((HWND)arg2, WM_NEW_TILES_AVAILABLE, 0, 0);
}

TSLLoaderCallbackReturn WMSClientSampleDoc::TileLoaderCallback( void* arg, const char* filename, const TSLEnvelope extent, TSLLoaderStatus status, Int percentDone )
{
  if ( percentDone == 100 && status == TSLLoadingOK )
  {
    PostMessage((HWND)arg, WM_NEW_TILES_AVAILABLE, 0, 0);
  }
  return TSLContinue;
}

void WMSClientSampleDoc::newTileAvailable()
{
  // invalidate the data layers
  if (m_mapDataLayer)
  {
    m_mapDataLayer->dataLayer()->notifyChanged();
  }
  int n = m_dataLayers.size();
  for( int j = 0; j < n; ++j )
  {
    m_dataLayers[j].dataLayer()->notifyChanged();
  }

  // call OnUpdate on all views
  UpdateAllViews ( NULL );
}

void WMSClientSampleDoc::refreshLayerSettings(TSLDataLayer* layer)
{
  CWinApp * app = AfxGetApp();
  ASSERT(app);

  int tcs = app->GetProfileInt(OPTIONS_APPSETTING_SECTION, TILE_CACHE_SIZE_APPSETTING, 128);
  int tlor = app->GetProfileInt(OPTIONS_APPSETTING_SECTION, TILE_LOAD_ORDER_APPSETTING, (int)(TSLWMSDataLayer::ClockwiseSpiral_CentreStart));
  int psf = app->GetProfileInt(OPTIONS_APPSETTING_SECTION, P_SCALE_FACTOR_APPSETTING, 9);

  switch (layer->layerType())
  {
    case TSLDataLayerTypeWMSDataLayer:
      {
        TSLWMSDataLayer* wms = reinterpret_cast<TSLWMSDataLayer*>(layer);

        int wivers = app->GetProfileInt(OPTIONS_APPSETTING_SECTION, INITIAL_VERSION_APPSETTING, ((int)TSLWMSVersion_1_3_0)-1);
        wivers+=1;
        int tlvl = app->GetProfileInt(OPTIONS_APPSETTING_SECTION, TILE_LEVELS_APPSETTING, (int)(TSLWMSDataLayer::TileLevelStrategyDetect));

        wms->setInitialRequestWMSVersion(TSLWMSVersionEnum(wivers));
        wms->cacheSize(tcs * 1024);
        wms->tileLevelStrategy((TSLWMSDataLayer::TileLevelStrategy)tlvl);
        wms->tileLoadOrder((TSLWMSDataLayer::TileLoadOrderStrategy)tlor);
        wms->pyramidScaleFactor(0.5 + 0.1 * psf);
      }
      break;

    case TSLDataLayerTypeWMTSDataLayer:
      {
        TSLWMTSDataLayer* wmts = reinterpret_cast<TSLWMTSDataLayer*>(layer);

        wmts->cacheSize(tcs * 1024);
        wmts->tileLoadOrder((TSLTileLoadOrderStrategyEnum)tlor);
        wmts->pyramidScaleFactor(0.5 + 0.1 * psf);
      }
      break;

    default:
      break;
  }
}


void WMSClientSampleDoc::refreshAllSettings()
{
  CWinApp * app = AfxGetApp();
  ASSERT(app);

  int n = m_dataLayers.size();
  for( int j = 0; j < n; ++j )
  {
    TSLDataLayer* dl = m_dataLayers[j].dataLayer();
    refreshLayerSettings(dl);
  }

  int ukca = app->GetProfileInt(OPTIONS_APPSETTING_SECTION, UNKNOWN_CA_APPSETTING, 0);
  m_remoteLoader->allowUnknownSSLAuthorities(ukca != 0);
  
  int certcnin = app->GetProfileInt(OPTIONS_APPSETTING_SECTION, CERT_CN_INVALID_APPSETTING, 0);
  m_remoteLoader->allowInvalidSSLCertDomain(certcnin != 0);

  int numberOfThreads = app->GetProfileInt(OPTIONS_APPSETTING_SECTION, NUMBER_OF_THREADS_APPSETTING, 4);
  m_remoteLoader->maximumConcurrentRequests(numberOfThreads);
}

void WMSClientSampleDoc::setupLinearTransformParameters(TSLDataLayer *layer)
{
  // If we have a map data-layer we need to match the settings to ensure that
  // all the data is aligned.
  if (m_mapDataLayer)
  {
    TSLMapDataLayer* mapDataLayer = reinterpret_cast<TSLMapDataLayer*>( m_mapDataLayer->dataLayer() );
    const TSLCoordinateSystem *coordSystem = mapDataLayer->queryMapCoordinateSystem();
    if (coordSystem)
    {
      double xOffset, yOffset;
      coordSystem->getMapOffsets(&xOffset, &yOffset);
      double tmcPerMU = coordSystem->getTMCperMU();

      switch (layer->layerType())
      {
      case TSLDataLayerTypeWMSDataLayer:
        {
          TSLWMSDataLayer* wms = reinterpret_cast<TSLWMSDataLayer*>(layer);
          wms->setLinearTransformParameters(false, xOffset, yOffset, tmcPerMU);
        }
        break;
      case TSLDataLayerTypeWMTSDataLayer:
        {
          TSLWMTSDataLayer* wmts = reinterpret_cast<TSLWMTSDataLayer*>(layer);
          wmts->setLinearTransformParameters(false, xOffset, yOffset, tmcPerMU);
        }
        break;

      default:
        break;
      }
    }
  }
}
