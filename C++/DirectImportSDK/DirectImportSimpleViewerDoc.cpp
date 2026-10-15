// DirectImportSimpleViewerDoc.cpp : implementation of the DirectImportSimpleViewerDoc class
//

#include "stdafx.h"
#include "DirectImportSimpleViewer.h"
#include <direct.h>
#include <iostream>
#include <sstream>
#include "DirectImportSimpleViewerDoc.h"
#include "DirectImportSimpleViewerView.h"
#include "VectorFeatureConfiguration.h"
#include "AnalysisCallbacks.h"
#include "MyCustomClassifier.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DirectImportSimpleViewerDoc

const char * DirectImportSimpleViewerDoc::m_layerName = "data" ;
const char * DirectImportSimpleViewerDoc::m_mapLayerName = "map";



IMPLEMENT_DYNCREATE(DirectImportSimpleViewerDoc, CDocument)

BEGIN_MESSAGE_MAP(DirectImportSimpleViewerDoc, CDocument)
//{{AFX_MSG_MAP(DirectImportSimpleViewerDoc)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG_MAP
  ON_COMMAND(ID_CRS_DEFAULT_TO_27700, &DirectImportSimpleViewerDoc::OnCrsDefaultTo27700)
  ON_UPDATE_COMMAND_UI(ID_CRS_DEFAULT_TO_4326, &DirectImportSimpleViewerDoc::OnUpdateCrsDefaultTo4326)
  ON_UPDATE_COMMAND_UI(ID_CRS_DEFAULT_TO_27700, &DirectImportSimpleViewerDoc::OnUpdateCrsDefaultTo27700)
  ON_COMMAND(ID_CRS_DEFAULT_TO_4326, &DirectImportSimpleViewerDoc::OnCrsDefaultTo4326)
  ON_COMMAND(ID_CRS_DISPLAY_USING_DATA_CRS, &DirectImportSimpleViewerDoc::OnCrsDisplayUsingDataCrs)
  ON_UPDATE_COMMAND_UI(ID_CRS_DISPLAY_USING_DATA_CRS, &DirectImportSimpleViewerDoc::OnUpdateCrsDisplayUsingDataCrs)
  ON_COMMAND(ID_CRS_DISPLAY_USING_4326, &DirectImportSimpleViewerDoc::OnCrsDisplayUsing4326)
  ON_UPDATE_COMMAND_UI(ID_CRS_DISPLAY_USING_4326, &DirectImportSimpleViewerDoc::OnUpdateCrsDisplayUsing4326)
  ON_COMMAND(ID_CRS_DISPLAY_USING_27700, &DirectImportSimpleViewerDoc::OnCrsDisplayUsing27700)
  ON_UPDATE_COMMAND_UI(ID_CRS_DISPLAY_USING_27700, &DirectImportSimpleViewerDoc::OnUpdateCrsDisplayUsing27700)
  ON_COMMAND(ID_CRS_DISPLAY_USING_3395, &DirectImportSimpleViewerDoc::OnCrsDisplayUsing3395)
  ON_UPDATE_COMMAND_UI(ID_CRS_DISPLAY_USING_3395, &DirectImportSimpleViewerDoc::OnUpdateCrsDisplayUsing3395)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DirectImportSimpleViewerDoc construction/destruction


DirectImportSimpleViewerDoc::DirectImportSimpleViewerDoc()
     : m_defaultInputCRS( 27700 ), m_displayCRS( DISPLAY_AS_DATA_CRS )
     , m_defaultInputCS( NULL )
{
  // Initialise my variables
  m_directImportDataLayer = 0 ;
  m_mapDataLayer = 0;
  const TSLCoordinateSystem * cs = TSLCoordinateSystem::findByEPSG(m_defaultInputCRS) ;
  if (cs)
    m_defaultInputCS = cs->clone(m_defaultInputCRS );

  m_myClassifier = new MyCustomClassifier();

  if (!m_myClassifier || !m_myClassifier->initialised())
  {
    CString msg(_T("Cannot load custom rendition file.\n\nCustom vector styling unavailable."));
    AfxMessageBox(msg);
    if (m_myClassifier)
    {
      delete m_myClassifier;
      m_myClassifier = NULL;
    }
  }
}

DirectImportSimpleViewerDoc::~DirectImportSimpleViewerDoc()
{
  if( m_defaultInputCS )
  {
    m_defaultInputCS->destroy();
  }
  if( m_directImportDataLayer )
  {
    m_directImportDataLayer->setCallbacks( NULL );
    m_directImportDataLayer->destroy();
    m_directImportDataLayer = NULL;

    m_mapDataLayer->destroy();
    m_mapDataLayer = NULL;
  }
  if (m_myClassifier)
  {
    delete m_myClassifier;
    m_myClassifier = NULL;
  }
}

/////////////////////////////////////////////////////////////////////////////
// DirectImportSimpleViewerDoc diagnostics

#ifdef _DEBUG
void DirectImportSimpleViewerDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void DirectImportSimpleViewerDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
//
// DirectImportSimpleViewerDoc commands
//
/////////////////////////////////////////////////////////////////////////////
BOOL DirectImportSimpleViewerDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
  if (!CDocument::OnOpenDocument(lpszPathName))
    return FALSE;

  // Clear the error stack, load the data then check for errors.
  TSLErrorStack::clear() ;
  
  // Need to convert the path to the file to load to UTF-8
  TSLUTF8Encoder path(lpszPathName);

  // Get the extension - OSM rendering typically needs map units not degrees
  TSLSimpleString directory, basename, extension;
  TSLFileHelper::decomposePathname(path, directory, basename, extension);
  bool isOpenStreetMap = (extension == "pbf" || extension == "osm");

  if (extension == "map")
  {
    m_mapDataLayer = new TSLMapDataLayer();
    m_mapDataLayer->loadData(path);

    return TRUE;
  }

  bool firstLoad = false;
  if (!m_directImportDataLayer)
  {
    m_directImportDataLayer = new TSLDirectImportDataLayer();
    m_directImportDataLayer->setCallbacks(this);

    // Set the vector and processing cache to 1GB
    m_directImportDataLayer->maxMemoryCacheSize(1024 * 1024);

    // Set the raster cache size to 1GB - used when drawing raster images.
    // If a raster dataset has constantly flickering tiles at the edge, then
    // this cache size is too small for the data. 
    m_directImportDataLayer->maxRasterDrawCacheSize(1024 * 1024);

    firstLoad = true;
  }

  // Identify the datasets from the data
  TSLvector<TSLDirectImportDataSet*>* dataSets( m_directImportDataLayer->createDataSets( path, TSLDirectImportDriver::OverviewTypeAny ) );
  if (!dataSets || dataSets->empty())
  {
    AfxMessageBox(_T("Failed to create datasets\n"), MB_OK);
    return FALSE;
  }
  if ( dataSets->size() != 1 )
    AfxMessageBox(_T("Only the first dataset will be loaded\n"), MB_OK | MB_ICONWARNING );

  // Get the first dataset that we're using
  TSLDirectImportDataSet * dataSet((*dataSets)[0]);

  if (firstLoad)
  {
    if (!m_mapDataLayer)
    {
      // Setup the output coordinate system - either selected by menu or from the data
      const TSLCoordinateSystem* outputCS = determineOutputCoordinateSystem(isOpenStreetMap, dataSet, m_displayCRS);
      m_directImportDataLayer->setCoordinateSystem(outputCS);
    }
    else
    {
      m_directImportDataLayer->setCoordinateSystem(m_mapDataLayer->getCoordinateSystem());
    }

    m_directImportDataLayer->addScaleBand(0.0, "default"); // Make a single scaleband for everything - must be after CRS set
  }

  // Now handle the data according to specific vector or raster 
  TSLFeatureClassConfig * fcc = 0;

  // If it is vector, then we need a FeatureClassConfiguration
  if (dataSet->dataType() == TSLDirectImportDriver::DataTypeVector)
  {
    // Generate or load feature class config
    fcc = new TSLFeatureClassConfig;
    VectorFeatureConfiguration fccDialog(m_directImportDataLayer, dataSet, fcc, isOpenStreetMap ? m_myClassifier : NULL );
    fccDialog.DoModal();
  }

  // Now load the datasets or display
  // Can destroy them after this since the layer will clone anything it needs
  if (!m_directImportDataLayer->loadData(dataSet, fcc ))
  {
    std::stringstream err;
    err << "Failed to load dataset: " << dataSet->name();
    AfxMessageBox(TSLUTF8Decoder(err.str().c_str()), MB_OK);
  }

  // Datasets and config are finished with and have been cloned by the data layer, no longer necessary
  m_directImportDataLayer->destroyDataSets( dataSets );
  if (fcc)
    delete fcc;

  // Display any errors or warning that have occurred that have occurred
  const char * msg = TSLErrorStack::errorString( "Errors while loading file\n" ) ;
  if ( msg )
    AfxMessageBox( TSLUTF8Decoder(msg), MB_ICONERROR ) ;

  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
// Layer / drawing surface management
//
/////////////////////////////////////////////////////////////////////////////
bool DirectImportSimpleViewerDoc::addToSurface( TSLDrawingSurface * surface )
{
  if (m_mapDataLayer)
  {
    surface->addDataLayer(m_mapDataLayer, m_mapLayerName);
  }

  if (m_directImportDataLayer)
  {
    // Add the data to the specified drawing surface,
    surface->addDataLayer(m_directImportDataLayer, m_layerName);
    surface->setDataLayerProps(m_layerName, TSLPropertyProgressiveDisplay, 1);
  }

  return true ;
}

bool DirectImportSimpleViewerDoc::detachFromSurface( TSLDrawingSurface * surface )
{
  if ( !m_directImportDataLayer  )
    return false;

  // Remove the data from the specified drawing surface,
  surface->removeDataLayer( m_layerName ) ;
  surface->removeDataLayer( m_mapLayerName );

  return true ;
}

void DirectImportSimpleViewerDoc::DeleteContents() 
{
  // Clean up by destroying the map and path list
  if ( m_directImportDataLayer )
  {
    m_directImportDataLayer->setCallbacks(NULL);
    m_directImportDataLayer->destroy() ;
    m_directImportDataLayer = 0 ;
  }
	
  CDocument::DeleteContents();
}

////////////////////////////////////////////////////////////////////////////////
//
// Methods to define the default input and output coordinate systems.
// Some datasets do not have an input CRS that can be determined. e.g OS tif+tfw
// This section includes the GUI callbacks
//
////////////////////////////////////////////////////////////////////////////////
const TSLCoordinateSystem*  DirectImportSimpleViewerDoc::determineOutputCoordinateSystem( bool requiresMapUnits, const TSLDirectImportDataSet * dataSet, int selectedCRS)
{
  int epsgID = selectedCRS;

  if (selectedCRS == DISPLAY_AS_DATA_CRS)
    epsgID = dataSet->coordinateSystem()->epsgID();

  // If our styling requires map units, and we've selected a degree based CRS, then swap to world mercator instead
  if (epsgID == 4326 && requiresMapUnits )
    epsgID = 3395;

  const TSLCoordinateSystem* cs(TSLCoordinateSystem::findByEPSG(epsgID));
  if (!cs)
    AfxMessageBox(_T("Cannot find output coordinate system\n"), MB_OK);
  
  return cs;
}

void DirectImportSimpleViewerDoc::OnCrsDefaultTo27700()
{
  m_defaultInputCRS = 27700 ;
  if( m_defaultInputCS )
  {
    m_defaultInputCS->destroy();
    m_defaultInputCS = NULL;
  }
  const TSLCoordinateSystem* cs( TSLCoordinateSystem::findByEPSG( 27700 ) );
  if( cs )
    m_defaultInputCS = cs->clone(1);
}

void DirectImportSimpleViewerDoc::OnUpdateCrsDefaultTo4326(CCmdUI *pCmdUI)
{
  pCmdUI->Enable() ;
  pCmdUI->SetCheck( m_defaultInputCRS == 4326 ) ;
}

void DirectImportSimpleViewerDoc::OnUpdateCrsDefaultTo27700(CCmdUI *pCmdUI)
{
  pCmdUI->Enable() ;
  pCmdUI->SetCheck( m_defaultInputCRS == 27700 ) ;
}

void DirectImportSimpleViewerDoc::OnCrsDefaultTo4326()
{
  m_defaultInputCRS = 4326 ;
  if( m_defaultInputCS )
  {
    m_defaultInputCS->destroy();
    m_defaultInputCS = NULL;
  }
  const TSLCoordinateSystem* cs( TSLCoordinateSystem::findByEPSG( 4326 ) );
  if( cs )
    m_defaultInputCS = cs->clone(1);
}

void DirectImportSimpleViewerDoc::OnCrsDisplayUsingDataCrs()
{
  m_displayCRS = DISPLAY_AS_DATA_CRS ;
}

void DirectImportSimpleViewerDoc::OnUpdateCrsDisplayUsingDataCrs(CCmdUI *pCmdUI)
{
  pCmdUI->Enable() ;
  pCmdUI->SetCheck( m_displayCRS == DISPLAY_AS_DATA_CRS ) ;
}

void DirectImportSimpleViewerDoc::OnCrsDisplayUsing4326()
{
  m_displayCRS = 4326 ;
}

void DirectImportSimpleViewerDoc::OnUpdateCrsDisplayUsing4326(CCmdUI *pCmdUI)
{
  pCmdUI->Enable() ;
  pCmdUI->SetCheck( m_displayCRS == 4326 ) ;
}

void DirectImportSimpleViewerDoc::OnCrsDisplayUsing27700()
{
  m_displayCRS = 27700 ;
}

void DirectImportSimpleViewerDoc::OnUpdateCrsDisplayUsing27700(CCmdUI *pCmdUI)
{
  pCmdUI->Enable() ;
  pCmdUI->SetCheck( m_displayCRS == 27700 ) ;
}

void DirectImportSimpleViewerDoc::OnCrsDisplayUsing3395()
{
  m_displayCRS = 3395 ;
}

void DirectImportSimpleViewerDoc::OnUpdateCrsDisplayUsing3395(CCmdUI *pCmdUI)
{
  pCmdUI->Enable() ;
  pCmdUI->SetCheck( m_displayCRS == 3395 ) ;
}

////////////////////////////////////////////////////////////////////////////////
//
// Various callbacks to handle the screen redraw etc.
//
////////////////////////////////////////////////////////////////////////////////
void DirectImportSimpleViewerDoc::onDeviceCapabilitiesRequired( TSLDeviceCapabilities& capabilities )
{
  DirectImportSimpleViewerView* view(DirectImportSimpleViewerView::GetView());
  if(!view)
  {
    return;
  }
  TSLDrawingSurface* surface( view->drawingSurface() );
  if( !surface)
  {
    return;
  }
  
  surface->getDeviceCapabilities( capabilities );
}

const TSLCoordinateSystem* DirectImportSimpleViewerDoc::onNoCoordinateSystem( const TSLDirectImportDataSet* dataSet, TSLDirectImportDriver* driver )
{
  return m_defaultInputCS;
}

void DirectImportSimpleViewerDoc::requestRedraw()
{
  POSITION pos = GetFirstViewPosition();
  CView* view(GetNextView(pos));
  if( view )
    view->InvalidateRect( 0, false );
}

void DirectImportSimpleViewerDoc::onTileLoadComplete( const TSLDirectImportDataSet* dataSet, unsigned int numProcessing, unsigned int numProcessingTotal )
{
  // Disable to wait until all tiles are loaded before data is displayed 
  POSITION pos = GetFirstViewPosition();
  CView* view(GetNextView(pos));
  if( view )
  {
    m_directImportDataLayer->notifyChanged(true);
    view->InvalidateRect( 0, false );
  }
}
