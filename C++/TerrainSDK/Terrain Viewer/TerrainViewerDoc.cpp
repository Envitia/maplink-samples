// TerrainViewerDoc.cpp : implementation of the CTerrainViewerDoc class
//

#include "stdafx.h"
#include "TerrainViewer.h"
#include "MainFrm.h"
#include "TerrainViewerView.h"

#include "TerrainViewerDoc.h"
#include "TerrainViewerOptions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CTerrainViewerDoc* CTerrainViewerDoc::m_thisDocument = 0;

/////////////////////////////////////////////////////////////////////////////
// CTerrainViewerDoc

IMPLEMENT_DYNCREATE(CTerrainViewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CTerrainViewerDoc, CDocument)
//{{AFX_MSG_MAP(CTerrainViewerDoc)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
ON_COMMAND(ID_IMPORT_TERRAIN, OnImportTerrain)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTerrainViewerDoc construction/destruction

CTerrainViewerDoc::CTerrainViewerDoc()
  : m_mapDataLayer(NULL)
  , m_visDataLayer(NULL)
  , m_extentLayer(NULL)
  , m_viewShedDataLayer(NULL)
  , m_conversionRequired(false)
{
  m_thisDocument = this;
}

CTerrainViewerDoc::~CTerrainViewerDoc()
{
  if ( m_mapDataLayer )
  {
    m_mapDataLayer->destroy() ;
    m_mapDataLayer = 0 ;
  }
  if( m_viewShedDataLayer )
  {
    m_viewShedDataLayer->destroy();
    m_viewShedDataLayer = NULL;
  }
  if ( m_visDataLayer )
  {
    m_visDataLayer->destroy() ;
    m_visDataLayer = 0 ;
  }
  if ( m_extentLayer )
  {
    m_extentLayer->destroy() ;
    m_extentLayer = 0 ;
  }
}

/////////////////////////////////////////////////////////////////////////////
// CTerrainViewerDoc diagnostics

#ifdef _DEBUG
void CTerrainViewerDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void CTerrainViewerDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTerrainViewerDoc commands

BOOL CTerrainViewerDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
  if (!CDocument::OnOpenDocument(lpszPathName))
    return FALSE;

  // Clear the error stack, load the map then check for errors.
  TSLErrorStack::clear() ;

  if ( m_mapDataLayer )
  {
    m_mapDataLayer->destroy() ;
    m_mapDataLayer = 0 ;
  }
  if (m_viewShedDataLayer)
  {
    m_viewShedDataLayer->destroy();
    m_viewShedDataLayer = NULL;
  }
  if ( m_visDataLayer )
  {
    m_visDataLayer->destroy() ;
    m_visDataLayer = 0 ;
  }
  if ( m_extentLayer )
  {
    m_extentLayer->destroy() ;
    m_extentLayer = 0 ;
  }

  m_mapDataLayer = new TSLMapDataLayer() ;
  m_extentLayer = new TSLStandardDataLayer() ;
  m_visDataLayer = new TSLStandardDataLayer() ;

  // This sample uses the terrain database within the viewshed datalayer to 
  // avoid loading the terrain data twice.
  // TSLTerrainDatabase and TSLDTEDTerrainDatabase may also be used directly
  // if viewshed capability is not required by the application.
  m_viewShedDataLayer = new TSLViewShedDataLayer();
  // Increase the default limits on the viewshed calculation
  // The TSLTerrainVSInputTerrainDatabase caches data for better performance
  // 512MB should be more than enough for more viewsheds
  m_viewShedDataLayer->viewShedInputCacheLimit(512);
  // The TSLViewShedDataLayer limits the size of the calculation 
  // Double the default to allow the sample to perform larger calculations
  // at the cost of some performance
  m_viewShedDataLayer->maxOutputSize(32768);
  // By default the layer will perform calculations asynchronously
  // This sample has not yet been updated for this case
  m_viewShedDataLayer->setSynchronousLoading(true);

  m_mapDataLayer->loadData( TSLUTF8Encoder(lpszPathName) ) ;

  // Display any errors that have occurred
  const char * msg = TSLErrorStack::errorString( "Cannot load map\n" ) ;
  if ( msg )
  {
    // The map load failed, so delete the data layer and path list
    AfxMessageBox( (LPCTSTR)TSLUTF8Decoder(msg), MB_ICONERROR ) ;
    m_mapDataLayer->destroy() ;
    m_viewShedDataLayer->destroy();
    m_visDataLayer->destroy();
    m_extentLayer->destroy();
    m_mapDataLayer = 0 ;
    m_viewShedDataLayer = NULL;
    m_visDataLayer = 0;
    m_extentLayer = 0;
  }
  else
  {
    // Find the coordinate system of the map
    m_mapCoordinateSystem = "";
    
    CString text;

    const TSLCoordinateSystem* cs = m_mapDataLayer->queryCoordinateSystem();
    
    // Check if future coordinate system conversions will be required. 
    if ( m_terrainCoordinateSystem.GetLength() > 0 )
    {
      const TSLCoordinateSystem * terrCS = terrainCoordinateSystem();
      if ( terrCS && cs->id() != terrCS->id() )
        m_conversionRequired = true ;
      else
        m_conversionRequired = false ;
    }

    if ( cs )
    {
      m_mapCoordinateSystem = cs->name();
      if ( ( m_mapCoordinateSystem.GetLength() > 0 ) && ( m_terrainCoordinateSystem.GetLength() > 0 ) )
      {
        text.Format( _T("Map: %s   Terrain: %s"), m_mapCoordinateSystem, m_terrainCoordinateSystem );
      }
      else if ( m_mapCoordinateSystem.GetLength() > 0 )
      {
        text.Format( _T("Map: %s"), m_mapCoordinateSystem );
      }
      else if ( m_terrainCoordinateSystem.GetLength() > 0 )
      {
        text.Format( _T("Terrain: %s"), m_terrainCoordinateSystem );
      }
    }
    CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd() ;
    if ( pFrame )
    {
      CStatusBar *pStatus = pFrame->getStatusBar();
      pStatus->SetPaneText( 1, text );
    }
  }

  POSITION pos = GetFirstViewPosition() ;
  GetNextView( pos )->Invalidate() ;

  return TRUE;
}

void CTerrainViewerDoc::DeleteContents() 
{
  if ( m_mapDataLayer )
  {
    m_mapDataLayer->destroy() ;
    m_mapDataLayer = 0 ;
  }
  if (m_viewShedDataLayer)
  {
    m_viewShedDataLayer->destroy();
    m_viewShedDataLayer = NULL;
  }
  if ( m_visDataLayer )
  {
    m_visDataLayer->destroy() ;
    m_visDataLayer = 0 ;
  }
  if ( m_extentLayer )
  {
    m_extentLayer->destroy() ;
    m_extentLayer = 0 ;
  }
	
  CDocument::DeleteContents();
}

void CTerrainViewerDoc::OnImportTerrain() 
{
  CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, _T("All Terrain Formats|*.tdf;dmed|Terrain Control Files (*.tdf)|*.tdf|DMED Files (dmed)|dmed|All Files (*.*)|*.*||") );
  
  if ( dlg.DoModal() == IDOK )
  {
    openTerrain( (LPCTSTR)dlg.GetPathName() ) ;
  }
}

void CTerrainViewerDoc::openTerrain( LPCTSTR path )
{
  if( path == NULL )
  {
    return;
  }
  if( _tcslen(path) == 0 )
  {
    return;
  }
  if (!m_viewShedDataLayer)
  {
    return;
  }

  if (!m_viewShedDataLayer->loadData(TSLUTF8Encoder(path)))
  {
    return;
  }

  m_terrainCoordinateSystem = "";
  
  const TSLCoordinateSystem * cs = terrainCoordinateSystem();
  
  // If no coordinate system is found for this data, which will happen if the
  // data was produced with a early version of MapLink then we need to exit.
  if ( !cs )
  {
    m_conversionRequired = true ;

    if ( m_extentLayer )
    {
      m_extentLayer->entitySet()->clear() ;
    }

    return ;	
  }

  // Check if future coordinate system conversions will be required. 
  if ( m_mapCoordinateSystem.GetLength() > 0 )
  {
    const TSLCoordinateSystem * mapCS = m_mapDataLayer->queryCoordinateSystem () ;
    if ( cs->id() != mapCS->id() )
      m_conversionRequired = true ;
    else
      m_conversionRequired = false ;
  }
  
  CString text;
  
  if ( cs )
  {
    m_terrainCoordinateSystem = cs->name();
    if ( ( m_mapCoordinateSystem.GetLength() > 0 ) && ( m_terrainCoordinateSystem.GetLength() > 0 ) )
    {
      text.Format( _T("Map: %s   Terrain: %s"), m_mapCoordinateSystem, m_terrainCoordinateSystem );
    }
    else if ( m_mapCoordinateSystem.GetLength() > 0 )
    {
      text.Format( _T("Map: %s"), m_mapCoordinateSystem );
    }
    else if ( m_terrainCoordinateSystem.GetLength() > 0 )
    {
      text.Format( _T("Terrain: %s"), m_terrainCoordinateSystem );
    }
  }
  
  CMainFrame *pFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
  
  if ( pFrame )
  {
    CStatusBar *pStatus = pFrame->getStatusBar();
    pStatus->SetPaneText( 1, text );
  }

  // This forces the view to repopulate the entity set
  if ( m_extentLayer )
  {
    m_extentLayer->entitySet()->clear() ;
  }

  UpdateAllViews( NULL, HINT_TERRAIN_DATA_LOADED );
}

bool CTerrainViewerDoc::calculateViewshed(double startX, double startY, const vector< TSLMUCoord > & fixedPositions, const TSLMUExtent& displayExtent, TSLDeviceUnits displayWidth, TSLDeviceUnits displayHeight)
{
  if (!m_viewShedDataLayer)
  {
    return false;
  }


  TerrainViewerOptions& options = CMainFrame::instance()->options();

  /* Provide the viewshed layer with the current view extent
   * in both Map Units (terrain CS) and Display Units.
   * This allows the TSLTerrainVSInputTerrainDatabase to select an appropriate layer and
   * resolution of data from the terrain database.
   * The viewshed calculation will be performed at the terrain data resolution. Depending on the
   * structure of the database, this may mean the calculation is performed at a much higher/lower resolution
   * than the current display.
   */



  m_viewShedDataLayer->setDisplayParameters(displayExtent, displayWidth, displayHeight);

  // Setup the datalayer with the various viewshed parameters
  // Most of the options are handled by TSLViewShedDataLayer::Observer
  // These may be different for each observer, however this sample does
  // not do this

  // Earth curvature corrections
  switch( options.m_viewShedCurvatureBoxIndex )
  {
    case 1:
      m_viewShedDataLayer->earthCurvatureCorrectionEnabled(true);
      m_viewShedDataLayer->earthCurvatureCorrectionType(TSLTerrainVSInputEarthCurvature::LOSTypeVisual);
      break;
    case 2:
      m_viewShedDataLayer->earthCurvatureCorrectionEnabled(true);
      m_viewShedDataLayer->earthCurvatureCorrectionType(TSLTerrainVSInputEarthCurvature::LOSTypeRadar);
      break;
    case 0:
    default:
      m_viewShedDataLayer->earthCurvatureCorrectionEnabled(false);
      break;
  }

  /* Perform the viewshed calculation, using the selected start point, and the various viewshed options.
   * Terrain data will be read from TSLViewShedDataLayer::viewShedInput
   * Interpretation of the results is determined by TSLViewShedDataLayer::visualisation
   * In this case the visibility of each point in the calculation will be displayed as the provided colour
   *
   * By default this calculation will be multithreaded. The number of threads may be limited by calling
   * TSLTerrainVSAlgorithm::maxThreads(numThreads);
   *
   * The TSLViewShedDataLayer may perform this calculation synchronously, or asynchronously
   * This sample has only been setup to perform calculations synchronously
   */

  // First setup the Observers for the viewshed calculation
  // If multiple observers are present the calculations will be merged into a single output,
  // showing the visibility of each point from all of the observers
  m_viewShedDataLayer->removeAllObservers();
  // One observer for the mouse position, and one observer for each fixed position
  for (size_t i(0); i < fixedPositions.size() + 1; ++i)
  {
    TSLMUCoord observerPos;
    if( i == fixedPositions.size() )
    {
      observerPos.x(startX);
      observerPos.y(startY);
    }
    else
    {
      observerPos = fixedPositions[i];
    }
    TSLViewShedDataLayer::Observer observer( 
      observerPos,
      options.m_viewShedStartHeight,
      options.m_viewShedStartRelativeToGround,
      options.m_viewShedMaximumRadius 
     );
     observer.endHeight( options.m_viewShedEndHeight );
     observer.endHeightRelativeToGround( options.m_viewShedEndRelativeToGround );

    m_viewShedDataLayer->addObserver(observer);
  }

  // Set the visualisation for the viewshed
  TSLViewShedDataLayer::VisualisationVisibility vis( options.viewShedColourVisible(), options.viewShedColourNotVisible(), options.viewShedColourNoData() );
  m_viewShedDataLayer->visualisation( &vis );

  // If calculations are being performed asynchronously this will schedule
  // a calculation and return immediately
  // If calculations are synchronous it will be performed immediately in the current thread
  return m_viewShedDataLayer->calculateViewShed();
}

TSLTerrainDatabase * CTerrainViewerDoc::getTerrainDB()
{
  // For this sample we re-use the terrain database within the viewshed layer
  // This avoids needing to load the data twice for general terrain queries
  // and viewshed calculations
  if (!m_viewShedDataLayer)
  {
    return NULL;
  }
  return m_viewShedDataLayer->terrainDatabase();
}

const TSLCoordinateSystem* CTerrainViewerDoc::terrainCoordinateSystem()
{
  if (!m_viewShedDataLayer)
  {
    return NULL;
  }
  return m_viewShedDataLayer->queryCoordinateSystem();
}