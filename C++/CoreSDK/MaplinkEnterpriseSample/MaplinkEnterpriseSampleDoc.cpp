// MaplinkEnterpriseSampleDoc.cpp : implementation of the CMaplinkEnterpriseSampleDoc class
//

#include "stdafx_tlt.h"
#include "MaplinkEnterpriseSample.h"
#include "MaplinkEnterpriseSampleDoc.h"
#include "MaplinkEnterpriseSampleView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMaplinkEnterpriseSampleDoc

//statics
const char mapLayerName[] = "map" ;  // layer name


IMPLEMENT_DYNCREATE(CMaplinkEnterpriseSampleDoc, CDocument)

BEGIN_MESSAGE_MAP(CMaplinkEnterpriseSampleDoc, CDocument)
//{{AFX_MSG_MAP(CMaplinkEnterpriseSampleDoc)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMaplinkEnterpriseSampleDoc construction/destruction

CMaplinkEnterpriseSampleDoc::CMaplinkEnterpriseSampleDoc()
{
  m_mapDataLayer = 0 ;
}


CMaplinkEnterpriseSampleDoc::~CMaplinkEnterpriseSampleDoc()
{
  cleanup();
}

/////////////////////////////////////////////////////////////////////////////
// CMaplinkEnterpriseSampleDoc diagnostics

#ifdef _DEBUG
void CMaplinkEnterpriseSampleDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void CMaplinkEnterpriseSampleDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMaplinkEnterpriseSampleDoc commands

BOOL CMaplinkEnterpriseSampleDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
  // This is to prevent crashing when a map is passed on the cmd line prior to the main window being constructed
  CWnd* cwnd = AfxGetMainWnd();
  if ( !cwnd )
    return true;

  // Use the correct loader, based on which open dialog was used
  TSLFileLoader* loader = NULL;
  if( CMaplinkEnterpriseSampleApp::m_remote )
  {
    loader = CMaplinkEnterpriseSampleApp::m_loaderRemote;
  }
  else
  {
    loader = CMaplinkEnterpriseSampleApp::m_loaderLocal;
  }

  // Open the file.  Can't use CDocument::OnOpenDocument because it
  // cannot cope with remote files over the internet

  if ( IsModified() )
    TRACE0("Warning: OnOpenDocument replaces an unsaved document.\n");
  
  DeleteContents();
  SetModifiedFlag(FALSE);     // start off with unmodified
  
  // Clear the error stack, load the map then check for errors.
  TSLErrorStack::clear() ;

  // create map data layer
  m_mapDataLayer = new TSLMapDataLayer();

  // set memory cache size
  m_mapDataLayer->cacheSize ( 256000 ); // 256Mb

  // add loader to data layer
  m_mapDataLayer->addLoader( loader, CMaplinkEnterpriseSampleDoc::mapCallbackFunction, cwnd->m_hWnd ) ;

  // load the map
  if ( !m_mapDataLayer->loadData( TSLUTF8Encoder(lpszPathName) ) )
  {
    AfxMessageBox( _T("Could not find map file."));
    cleanup();
    return FALSE;
  }

  if ( CMaplinkEnterpriseSampleApp::m_persistentCacheData )
  {
    {
      m_mapDataLayer->configurePersistentCache( CMaplinkEnterpriseSampleApp::m_persistentCacheData );
      // clear the cache if the map data has changed
      checkForCacheClear( CMaplinkEnterpriseSampleApp::m_persistentCacheData->directory(), loader );
    }
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
  
  return TRUE;
}


void CMaplinkEnterpriseSampleDoc::checkForCacheClear( const char* cacheDir, TSLFileLoader* loader )
{
  char oldDate[20] = "";
  char newDate[20] = "";

  TSLifstream instream;
  TSLLoaderStatus sts; 
  
  // Read the local date.dat
  string localFilename = cacheDir;
  localFilename += "\\date.dat";
  sts = loader->loadImmediate(0, localFilename.c_str(), &instream);
  if ( sts == TSLLoadingOK )
  {
    instream.getline( oldDate, 20);
  }

  // Read the remote date.dat
  TSLSimpleString remoteFilename;
  if ( m_mapDataLayer->findFile( "date.dat", remoteFilename ) )
  {
    sts = loader->loadImmediate(0, remoteFilename, &instream);
    if ( sts == TSLLoadingOK )
    {
      instream.getline( newDate, 20);
    }
  }
    
  if ( strcmp (newDate, oldDate) != 0 )
  {
    // Dates are different, clear the cache and write the new date

    m_mapDataLayer->clearCache();  // clears both memory and persistent cache

    TSLofstream outstream( localFilename.c_str() );
    outstream.write ( newDate, strlen(newDate)+1 );
  }
}


/////////////////////////////////////////////////////////////////////////////
// CMaplinkEnterpriseSampleDoc static callback function
//    note: may be called by a thread that is not our main GUI thread - it
//    is not safe to call other methods or use data unless we add critical sections,
//    but it is safe to send ourself messages.

TSLLoaderCallbackReturn CMaplinkEnterpriseSampleDoc::mapCallbackFunction( void* arg, const char* filename, TSLEnvelope extent, TSLLoaderStatus status, int percentDone )
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


void CMaplinkEnterpriseSampleDoc::SuccessfulCallbackAction( )
{
  // Do not just update all views here, since that will cause an immediate redraw.
  // Instead invalidate the rect, which will cause a windows message to be used
  // That way we minimise the redrawing if tiles are loaded quickly

  // Invalidate the map data layer
  // Must do this here (in the foreground drawing thread) to avoid 
  // some redraw problems with blank spaces being left - usually if surface is double buffered.
  m_mapDataLayer->notifyChanged();

  POSITION pos = GetFirstViewPosition();
  while (pos != NULL)
  {
    CView* pView = GetNextView(pos);
    pView->InvalidateRect( 0, FALSE ) ;
  }
}


void CMaplinkEnterpriseSampleDoc::cleanup()
{
  if ( m_mapDataLayer )
  {
    m_mapDataLayer->destroy() ;
    m_mapDataLayer = 0 ;
  }
}


void CMaplinkEnterpriseSampleDoc::DeleteContents() 
{
  cleanup();

  CDocument::DeleteContents();
}


bool CMaplinkEnterpriseSampleDoc::addToSurface( TSLDrawingSurface * surface )
{
  if ( !m_mapDataLayer  )
    return false;

  surface->addDataLayer( m_mapDataLayer, mapLayerName ) ;

  // enable progressive moading load, which means that when a detailed area is still loading
  // in the background, less-detailed data can be shown in that area rather than white screen
  surface->setDataLayerProps( mapLayerName, TSLPropertyProgressiveDisplay,true ) ;

  return true ;
}


bool CMaplinkEnterpriseSampleDoc::detachFromSurface( TSLDrawingSurface * surface )
{
  if ( !m_mapDataLayer  )
    return false;

  surface->removeDataLayer( mapLayerName ) ;

  return true ;
}


void CMaplinkEnterpriseSampleDoc::SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU)
{
  // This needs to be done with Visual Studio .NET 2003, since the base
  // implementation cannot handle http:// prefixed names and generates
  // an exception which causes the program to exit.

  if ( lstrlen(lpszPathName) >= _MAX_PATH )
  {
    ASSERT(FALSE);
    // MFC requires paths with length < _MAX_PATH
    // No other way to handle the error from a void function
    AfxThrowFileException(CFileException::badPath);
  }

  m_strPathName = lpszPathName;
  m_bEmbedded = FALSE;

  // set the document title based on path name - just do the basename
  TCHAR basename[ _MAX_PATH ], ext[ _MAX_PATH ], szFullPath[ _MAX_PATH ] ;
  _tsplitpath( lpszPathName, 0, 0, basename, ext ) ;
  _tmakepath( szFullPath, 0, 0, basename, ext ) ;
  SetTitle(szFullPath);

  ASSERT_VALID(this);
}

void CMaplinkEnterpriseSampleDoc::setSynchronousLoading(bool synchronous)
{
  m_mapDataLayer->setSynchronousLoading(synchronous);
}