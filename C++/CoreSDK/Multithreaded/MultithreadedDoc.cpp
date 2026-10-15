// MultithreadedDoc.cpp : implementation of the CMultithreadedDoc class
//

#include "stdafx_tlt.h"
#include "Multithreaded.h"
#include "MultithreadedDoc.h"
#include "MultithreadedView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMultithreadedDoc


//statics
const char mapLayerName[] = "map" ;  // layer name


IMPLEMENT_DYNCREATE(CMultithreadedDoc, CDocument)

BEGIN_MESSAGE_MAP(CMultithreadedDoc, CDocument)
//{{AFX_MSG_MAP(CMultithreadedDoc)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMultithreadedDoc construction/destruction

CMultithreadedDoc::CMultithreadedDoc()
{
  m_mapDataLayer = 0 ;
}


CMultithreadedDoc::~CMultithreadedDoc()
{
  cleanup();
}

/////////////////////////////////////////////////////////////////////////////
// CMultithreadedDoc diagnostics

#ifdef _DEBUG
void CMultithreadedDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void CMultithreadedDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMultithreadedDoc commands

BOOL CMultithreadedDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
  if (!CDocument::OnOpenDocument(lpszPathName) || !AfxGetMainWnd())
    return FALSE;

  TSLFileLoader* loader = CMultithreadedApp::m_loader;

  // Create a map data layer
  m_mapDataLayer = new TSLMapDataLayer();
  
  // Clear the error stack, load the map then check for errors.
  TSLErrorStack::clear() ;

  // set memory cache size
  m_mapDataLayer->cacheSize ( 10000 ); // 10M

  // add loader to data layer
  m_mapDataLayer->addLoader( loader, 
    CMultithreadedDoc::mapCallbackFunction, 
    AfxGetMainWnd()->m_hWnd ) ;

  // load the map
  if ( !m_mapDataLayer->loadData( TSLUTF8Encoder(lpszPathName) ) )
  {
    AfxMessageBox( _T("Could not find map file.") );
    cleanup();
    return FALSE;
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




/////////////////////////////////////////////////////////////////////////////
// CMultithreadedDoc static callback function
//    note: may be called by a thread that is not our main GUI thread - it
//    is not safe to call other methods or use data unless we add critical sections,
//    but it is safe to send ourself messages.

TSLLoaderCallbackReturn CMultithreadedDoc::mapCallbackFunction( void* arg, const char* filename, TSLEnvelope extent, TSLLoaderStatus status, int percentDone )
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


void CMultithreadedDoc::SuccessfulCallbackAction( )
{
  // invalidate the map data layer
  m_mapDataLayer->notifyChanged();

  // call OnUpdate on all views
  UpdateAllViews ( NULL );
}


void CMultithreadedDoc::cleanup()
{
  if ( m_mapDataLayer )
  {
    m_mapDataLayer->destroy() ;
    m_mapDataLayer = 0 ;
  }
}


void CMultithreadedDoc::DeleteContents() 
{
  cleanup();

  CDocument::DeleteContents();
}


bool CMultithreadedDoc::addToSurface( TSLDrawingSurface * surface )
{
  if ( !m_mapDataLayer  )
    return false;

  surface->addDataLayer( m_mapDataLayer, mapLayerName ) ;

  // enable progressive loading mode, which means that when a detailed area is still loading
  // in the background, less-detailed data can be shown in that area rather than white screen
  surface->setDataLayerProps( mapLayerName, TSLPropertyProgressiveDisplay, true ) ;

  return true ;
}


bool CMultithreadedDoc::detachFromSurface( TSLDrawingSurface * surface )
{
  if ( !m_mapDataLayer  )
    return false;

  surface->removeDataLayer( mapLayerName ) ;

  return true ;
}

