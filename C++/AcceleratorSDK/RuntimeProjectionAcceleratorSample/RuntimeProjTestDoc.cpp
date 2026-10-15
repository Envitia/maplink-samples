// RuntimeProjTestDoc.cpp : implementation of the CRuntimeProjTestDoc class
//

#include "stdafx_tlt.h"
#include "RuntimeProjTest.h"
//#include <direct.h>

#include "RuntimeProjTestDoc.h"
#include "RuntimeProjTestView.h"
#include "NewProjectionPropertiesDialog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif







/////////////////////////////////////////////////////////////////////////////
// CRuntimeProjTestDoc


//statics
const char mapLayerName[] = "map" ;  // layer name


IMPLEMENT_DYNCREATE(CRuntimeProjTestDoc, CDocument)

BEGIN_MESSAGE_MAP(CRuntimeProjTestDoc, CDocument)
//{{AFX_MSG_MAP(CRuntimeProjTestDoc)
	ON_UPDATE_COMMAND_UI(ID_REPROJECT, OnUpdateReproject)
	ON_COMMAND(ID_REPROJECT, OnReproject)
	ON_COMMAND(ID_OPTIONS_BEEPWHENALLTILESLOADED, OnOptionsBeepwhenalltilesloaded)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_BEEPWHENALLTILESLOADED, OnUpdateOptionsBeepwhenalltilesloaded)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRuntimeProjTestDoc construction/destruction

CRuntimeProjTestDoc::CRuntimeProjTestDoc()
  : m_mapDataLayer(0),
    m_mapLoaded( false ),
    m_beepOnAllLoaded( false ),
    m_lastGreatCircleDistanceSpecified( 0.0 ),
    m_mapFileLoader(NULL)
{
  // load coordinate system definitions from maplink home.
  // we will use the default datums but explicitly set the map projections
  if ( !TSLCoordinateSystem::loadCoordinateSystems( ) )
  {
    AfxMessageBox( _T("Error: Couldn't initialise coordinate systems from configuration files") );
  }
}


CRuntimeProjTestDoc::~CRuntimeProjTestDoc()
{
}

/////////////////////////////////////////////////////////////////////////////
// CRuntimeProjTestDoc diagnostics

#ifdef _DEBUG
void CRuntimeProjTestDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void CRuntimeProjTestDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRuntimeProjTestDoc commands

BOOL CRuntimeProjTestDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
  if (!CDocument::OnOpenDocument(lpszPathName))
    return FALSE;

  // Clear the error stack, load the map then check for errors.
  TSLErrorStack::clear() ;

  m_mapDataLayer = new TSLMapDataLayer() ;

  // set memory cache size
  m_mapDataLayer->cacheSize ( 32 * 1024 );  // 32MB

  // We need to use a blocking loader as the re-projection is done in the back ground thread
  if (!m_mapFileLoader)
  {
    m_mapFileLoader = new TSLFileLoaderBlocking;
  }
  // add loader to data layer
  m_mapDataLayer->addLoader( m_mapFileLoader,
                            CRuntimeProjTestDoc::mapCallbackFunction, 
                             AfxGetMainWnd()->m_hWnd,
                             CRuntimeProjTestDoc::allLoadedCallbackFunction,
                             AfxGetMainWnd()->m_hWnd ) ;


  TSLCoordinateSystem* runtimeCS = 0;

  // Clear the runtime projection - set it again below.
  // It's done this way because if the new map has a different datum to the runtime datum,
  // the map data layer would find that there's no suitable datum transform set up and fail.
  // By waiting till after the map has loaded we can set up the datum transform.
  TSLCoordinateSystem* oldRuntimeCS = m_mapDataLayer->queryRuntimeCoordinateSystem();
  if ( oldRuntimeCS )
  {
    runtimeCS = oldRuntimeCS->clone( 1001 );
    m_mapDataLayer->clearRuntimeProjection();
  }

  // load the map
  if ( !m_mapDataLayer->loadData( TSLUTF8Encoder(lpszPathName) ) )
  {
    AfxMessageBox( _T("Could not find map file."));
    if ( runtimeCS )
    {
      runtimeCS->destroy();
    }
    cleanup();
    return FALSE;
  }

  if ( runtimeCS )
  {
    // reset the runtime projection if already set up
    if ( !setRuntimeProjection( runtimeCS ) )
    {
      AfxMessageBox( _T("Failed to reset runtime projection") );
      runtimeCS->destroy();
      cleanup();
      return FALSE;
    }

    runtimeCS->destroy();
  }

  // Display any errors that have occurred
  const char * msg = TSLErrorStack::errorString( "Cannot load map\n" ) ;
  if ( msg )
  {
    // The map load failed, so delete the data
    AfxMessageBox( (LPCTSTR)TSLUTF8Decoder(msg), MB_ICONERROR ) ;
    cleanup();
    return FALSE;
  }

  m_mapLoaded = true;
  
  return TRUE;
}




/////////////////////////////////////////////////////////////////////////////
// CRuntimeProjTestDoc static callback function
//    note: may be called by a thread that is not our main GUI thread - it
//    is not safe to call other methods or use data unless we add critical sections,
//    but it is safe to send ourself messages.

TSLLoaderCallbackReturn CRuntimeProjTestDoc::mapCallbackFunction( void* arg, const char* filename, TSLEnvelope extent, TSLLoaderStatus status, int percentDone )
{
  if ( ( status == TSLLoadingOK ) && ( percentDone == 100 ) )
  {
    // tile has been loaded, send the mainframe a message
    // the message is sent to the mainframe because you can't send a message to a document
    // it just calls us straight back, calling SuccessfulCallbackAction
    ::PostMessage( (HWND)arg, PWM_CALLBACK, NULL, NULL );
 }

  return TSLContinue;
}


/////////////////////////////////////////////////////////////////////////////
// CRuntimeProjTestDoc static allLoaded callback function
//    note: may be called by a thread that is not our main GUI thread - it
//    is not safe to call other methods or use data unless we add critical sections,
//    but it is safe to send ourself messages.
void CRuntimeProjTestDoc::allLoadedCallbackFunction( void* arg2 )
{
  // main frame will call us back with AllLoadedCallbackAction in main thread
  ::PostMessage( (HWND)arg2, PWM_ALLLOADED, NULL, NULL );
}


void CRuntimeProjTestDoc::runtimeProjectionChangedCallback( void* arg, TSLRuntimeProjectionChangedEnum reason )
{
  // another callback from a worker thread - send a message to the main frame, which calls back into the document

  // only expected for projection change; origin change has a different callback, in TSLViewModeRecentreProjection
  ASSERT ( reason == TSLRuntimeProjectionChangeProjectionChanged );

  // main frame will call us back with projectionChangedCallbackAction in main thread
  ::PostMessage( (HWND)arg, PWM_PROJCHANGED, NULL, NULL );
}


void CRuntimeProjTestDoc::SuccessfulCallbackAction( )
{
  // This call is made (in the main thread) when a tile has completed loading.

  // invalidate the map data layer
  m_mapDataLayer->notifyChanged();

  // call OnUpdate on all views
  UpdateAllViews ( NULL );
}


void CRuntimeProjTestDoc::AllLoadedCallbackAction( )
{
  // This call is made (in the main thread) when tiles needed loading following
  // a draw, and the last one has just loaded

  if ( m_beepOnAllLoaded )
  {
    Beep( 1000, 200 );
  }
}


void CRuntimeProjTestDoc::projectionChangedCallbackAction( )
{
  // invalidate the map data layer
  m_mapDataLayer->notifyChanged( true );
  
  // force a reset
  POSITION pos = GetFirstViewPosition();
  if (pos == NULL)
    return;
  CRuntimeProjTestView* pView = (CRuntimeProjTestView*)GetNextView(pos);
  pView->PostMessage( WM_COMMAND, ID_TOOLS_RESET, 0 );
}




void CRuntimeProjTestDoc::cleanup()
{
  if ( m_mapDataLayer )
    m_mapDataLayer->removeData();
  
  m_mapLoaded = false;
}


void CRuntimeProjTestDoc::DeleteContents() 
{
  cleanup();
  if (m_mapDataLayer)
    m_mapDataLayer->destroy() ;
  m_mapDataLayer = NULL;

  CDocument::DeleteContents();
}


bool CRuntimeProjTestDoc::addToSurface( TSLDrawingSurface * surface )
{
  if ( !m_mapDataLayer )
  {
    return false ;
  }

  surface->addDataLayer( m_mapDataLayer, mapLayerName ) ;

  // enable progressive moading load, which means that when a detailed area is still loading
  // in the background, less-detailed data can be shown in that area rather than white screen
  surface->setDataLayerProps( mapLayerName, TSLPropertyProgressiveDisplay,true ) ;

  // The layer must be marked as double buffered for it to be pushed to the background thread.
  surface->setDataLayerProps( mapLayerName, TSLPropertyBuffered, true ) ;

  return true ;
}


bool CRuntimeProjTestDoc::detachFromSurface( TSLDrawingSurface * surface )
{
  if ( !m_mapDataLayer )
    return false ;

  surface->removeDataLayer( mapLayerName ) ;

  return true ;
}


void CRuntimeProjTestDoc::OnReproject() 
{
  NewProjectionPropertiesDialog dlg2( AfxGetMainWnd() );

  // set initial origin to centre of map
  double latd, longtd;
  if ( getMapCentre( &latd, &longtd ) )
  {
    // fill values into dialog, rounded to 0.01 degrees
    dlg2.m_initialLat = floor( ( 100.0 * latd ) + 0.5 ) * 0.01;
    dlg2.m_initialLong = floor( ( 100.0 * longtd ) + 0.5 ) * 0.01;
  }

  if ( dlg2.DoModal() != IDOK )
  {
    // make sure display is cleared
    UpdateAllViews ( NULL );
    return;
  }

  setProjection(dlg2.m_projectionChoice, dlg2.m_maxDist, dlg2.m_initialLat, dlg2.m_initialLong);
}

void CRuntimeProjTestDoc::initialProjection()
{
  double latd = 0.0, longtd = 0.0;
  if ( getMapCentre( &latd, &longtd ) )
  {
    // fill values into dialog, rounded to 0.01 degrees
    latd = floor( ( 100.0 * latd ) + 0.5 ) * 0.01;
    longtd = floor( ( 100.0 * longtd ) + 0.5 ) * 0.01;
  }
  setProjection(0, 9704000.0, latd, longtd);
}

void CRuntimeProjTestDoc::setProjection(int choice, double maxDistance, double lat, double lon)
{
  TSLCoordinateSystem* cs = 0;
  bool setOK = false;
  double tilt = 0.0;
  char* datumName;

  // could instantiate a projection by listing those in the coordinate system registry and picking one, 
  // in which case there would be no need to code the WGS84 parameters etc. as the ellipsioid would be 
  // inferred in the coordinate system.  I've hard coded these to keep the number of options down.

  m_lastGreatCircleDistanceSpecified = maxDistance;

  switch( choice )
  {
  case 0:
    // Orthographic tilted
    datumName = "Spherical Earth: Mean Radius";  // this CS needs spherical datum
    cs = TSLCoordinateSystem::createByDatumName( 10001, "CS", datumName );  
    if ( cs )
      setOK = cs->setProjectionOrthographic( lat, lon );
    tilt = -23.5;
    break;

  case 1:
    // Azimuthal Equidistant
    datumName = "Spherical Earth: Mean Radius";  // this CS needs spherical datum
    cs = TSLCoordinateSystem::createByDatumName( 10001, "CS", datumName );  
    if ( cs )
      setOK = cs->setProjectionAzimuthalEquidistant( lat, lon );
    break;

  case 2:
    // Stereographic
    datumName = "World Geodetic System 1984";
    cs = TSLCoordinateSystem::createByDatumName( 10001, "CS", datumName );  
    if ( cs )
      setOK = cs->setProjectionStereographic( lat, lon, 1.0 );
    break;

  case 3:
    // Gnomonic
    datumName = "Spherical Earth: Mean Radius";  // this CS needs spherical datum
    cs = TSLCoordinateSystem::createByDatumName( 10001, "CS", datumName );  
    if ( cs )
      setOK = cs->setProjectionGnomonic(  lat, lon );
    break;

  case 4:
    // Transverse Mercator
    datumName = "World Geodetic System 1984";
    cs = TSLCoordinateSystem::createByDatumName( 10001, "CS", datumName );  
    if ( cs )
      setOK = cs->setProjectionTransverseMercator( lat, lon, 1.0, TSLCoordinateSystem::TSLTransverseMercatorFormulaUSGS );
    break;

  case 5:
    // Mercator
    datumName = "World Geodetic System 1984";
    cs = TSLCoordinateSystem::createByDatumName( 10001, "CS", datumName );  
    if ( cs )
      setOK = cs->setProjectionMercator( lon );
    break;

  case 6:
    // Unprojected, rotated 45 degrees
    datumName = "World Geodetic System 1984";
    cs = TSLCoordinateSystem::createByDatumName( 10001, "CS", datumName );  
    if ( cs )
      cs->setProjectionOrigin( lat, lon );
    tilt = 45.0;
    setOK = ( cs != 0 );
    break;
  }


  if ( setOK )
  {
    // set rotation if needed
    if ( tilt != 0.0 )
    {
      setOK = cs->setLinearRotationTransform( tilt );
    }
  }

  if ( setOK )
  {
    // set the projection
    setOK = setRuntimeProjection( cs );
  }

  if ( !setOK )
  {
    AfxMessageBox( _T("Failed to set runtime projection") );
  }

  if ( cs )
    cs->destroy();

  // make sure display is updated
  UpdateAllViews ( NULL );
}

void CRuntimeProjTestDoc::OnUpdateReproject(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( m_mapDataLayer != 0 );
}

bool CRuntimeProjTestDoc::runtimeProjectionSet() const
{
  return (m_mapDataLayer->queryMapCoordinateSystem() != 0 );
}

void CRuntimeProjTestDoc::OnOptionsBeepwhenalltilesloaded() 
{
  m_beepOnAllLoaded = !m_beepOnAllLoaded;
}

void CRuntimeProjTestDoc::OnUpdateOptionsBeepwhenalltilesloaded(CCmdUI* pCmdUI) 
{
  pCmdUI->SetCheck( m_beepOnAllLoaded ? 1 : 0 );
}


bool CRuntimeProjTestDoc::setRuntimeProjection( TSLCoordinateSystem* runtimeCS )
{
  // set the projection, 
  if ( CRuntimeProjTestApp::m_useCallbacksForProjectionChanges )
  {
    // in background thread, callback made when done
    if ( !m_mapDataLayer->setRuntimeProjection( runtimeCS, m_lastGreatCircleDistanceSpecified, runtimeProjectionChangedCallback, AfxGetMainWnd()->m_hWnd ) )
    {
      return false;
    }
  }
  else
  {
    // synchronously
    if ( !m_mapDataLayer->setRuntimeProjection( runtimeCS, m_lastGreatCircleDistanceSpecified, 0, 0 ) )
    {
      return false;
    }

    // synchronously complete the redraw
    projectionChangedCallbackAction();
  }

  return true;
}


bool CRuntimeProjTestDoc::getMapCentre( double* latd, double* longtd )
{
  // to find the centre of the map in degrees we need to temporarily set up a runtime
  // projection so that the map's extent can be queried

  if ( !m_mapDataLayer )
    return false;

  // remember old settings
  bool oldOption = CRuntimeProjTestApp::m_useCallbacksForProjectionChanges;


  // set do-nothing runtime projection
  char* datumName = "World Geodetic System 1984";
  TSLCoordinateSystem* cs = TSLCoordinateSystem::createByDatumName( 10001, "CS", datumName );
  if ( !cs )
    return false;
  cs->setProjectionOrigin( 0.0, 0.0 );

  // turn off callbacks to avoid complications
  CRuntimeProjTestApp::m_useCallbacksForProjectionChanges = false;

  // apply the do-nothing projection
  m_mapDataLayer->setRuntimeProjection( cs, 100.0, 0, 0 );
  cs->destroy();
  
  // query the map extent; map is unprojected so MU will be in long,lat
  double x1, y1, x2, y2;
  if ( !m_mapDataLayer->getMUExtent( &x1, &y1, &x2, &y2 ) )
  {
    m_mapDataLayer->clearRuntimeProjection( );
    CRuntimeProjTestApp::m_useCallbacksForProjectionChanges = oldOption;
    return false;
  }

  // take means to give centre
  *latd = ( y1 + y2 ) * 0.5;
  *longtd = ( x1 + x2 ) * 0.5;

  // clean up
  m_mapDataLayer->clearRuntimeProjection( );
  CRuntimeProjTestApp::m_useCallbacksForProjectionChanges = oldOption;

  return true;
}
