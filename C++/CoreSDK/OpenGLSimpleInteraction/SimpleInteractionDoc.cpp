// SimpleInteractionDoc.cpp : implementation of the SimpleInteractionDoc class
//

#include "stdafx.h"
#include "SimpleInteractionDoc.h"
#ifdef REALTIME_REPROJECTION
// TBD
# include "glsurfacebackdoors.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SimpleInteractionDoc

const char * SimpleInteractionDoc::m_mapLayerName = "map" ;


IMPLEMENT_DYNCREATE(SimpleInteractionDoc, CDocument)

BEGIN_MESSAGE_MAP(SimpleInteractionDoc, CDocument)
//{{AFX_MSG_MAP(SimpleInteractionDoc)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SimpleInteractionDoc construction/destruction

SimpleInteractionDoc::SimpleInteractionDoc()
{
  // Initialise my variables
  m_mapDataLayer = 0 ;
}

SimpleInteractionDoc::~SimpleInteractionDoc()
{
}

/////////////////////////////////////////////////////////////////////////////
// SimpleInteractionDoc diagnostics

#ifdef _DEBUG
void SimpleInteractionDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void SimpleInteractionDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// SimpleInteractionDoc commands

BOOL SimpleInteractionDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
  if (!CDocument::OnOpenDocument(lpszPathName))
    return FALSE;
  
  // Create a map data layer
  m_mapDataLayer = new TSLStaticMapDataLayer();

  // Increase the map's cache size to 256Mb in order to improve performance
  m_mapDataLayer->drawCacheSize( 256 * 1024 );
  
  // Clear the error stack, load the map then check for errors.
  TSLErrorStack::clear() ;

  // Load the map
  // This will automatically load all associated files such as 
  // the .pth and .pal files.  These needed to be loaded
  // directly by the application in earlier versions of MapLink
  m_mapDataLayer->loadData( TSLUTF8Encoder(lpszPathName) ) ;

  /***
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
  */
  // Display any errors that have occurred
  const char * msg = TSLErrorStack::errorString( "Cannot load map\n" ) ;
  if ( msg )
  {
    // The map load failed, so delete the data layer and path list
    AfxMessageBox( (LPCTSTR)TSLUTF8Decoder(msg), MB_ICONERROR ) ;
    m_mapDataLayer->destroy() ;
    m_mapDataLayer = 0 ;
    /*
    if ( runtimeCS )
    {
      runtimeCS->destroy();
    }*/
  }

  return TRUE;
}

bool SimpleInteractionDoc::addToSurface( TSLDrawingSurface * surface )
{
  if ( !m_mapDataLayer  )
    return false;

  // Add the map to the specified drawing surface,
  surface->addDataLayer( m_mapDataLayer, m_mapLayerName ) ;

#ifdef REALTIME_REPROJECTION
  GLSurfaceBackdoors::enableRealtimeReprojection( (TSLOpenGLSurface*)surface, m_mapLayerName, true );
#endif

  return true ;
}

bool SimpleInteractionDoc::detachFromSurface( TSLDrawingSurface * surface )
{
  if ( !m_mapDataLayer  )
    return false;

  // Remove the map to the specified drawing surface,
  surface->removeDataLayer( m_mapLayerName ) ;

  return true ;
}

void SimpleInteractionDoc::DeleteContents() 
{
  // Clean up by destroying the map and pathlist
  if ( m_mapDataLayer )
  {
    m_mapDataLayer->destroy() ;
    m_mapDataLayer = 0 ;
  }
	
  CDocument::DeleteContents();
}
