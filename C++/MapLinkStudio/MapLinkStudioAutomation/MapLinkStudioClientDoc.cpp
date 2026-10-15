// MapLinkStudioClientDoc.cpp : implementation of the CMapLinkStudioClientDoc class
//

#include "stdafx.h"
#include "MapLinkStudioClient.h"
#include <direct.h>
#include "MapLinkStudioClientDoc.h"
#include "MaplinkStudio.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapLinkStudioClientDoc

const char * CMapLinkStudioClientDoc::m_mapLayerName = "map" ;


IMPLEMENT_DYNCREATE(CMapLinkStudioClientDoc, CDocument)

BEGIN_MESSAGE_MAP(CMapLinkStudioClientDoc, CDocument)
//{{AFX_MSG_MAP(CMapLinkStudioClientDoc)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapLinkStudioClientDoc construction/destruction

CMapLinkStudioClientDoc::CMapLinkStudioClientDoc()
{
  // Initialise my variables
  m_mapDataLayer = 0 ;
}

CMapLinkStudioClientDoc::~CMapLinkStudioClientDoc()
{
}

/////////////////////////////////////////////////////////////////////////////
// CMapLinkStudioClientDoc diagnostics

#ifdef _DEBUG
void CMapLinkStudioClientDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void CMapLinkStudioClientDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMapLinkStudioClientDoc commands

BOOL CMapLinkStudioClientDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
  if (!CDocument::OnOpenDocument(lpszPathName))
    return FALSE;
  
  
  // Create a map data layer
  m_mapDataLayer = new TSLMapDataLayer();
  
  // Clear the error stack, load the map then check for errors.
  TSLErrorStack::clear() ;


  // load the map
  m_mapDataLayer->loadData( TSLUTF8Encoder(lpszPathName) ) ;


  // Display any errors that have occurred
  const char * msg = TSLErrorStack::errorString( "Cannot load map\n" ) ;
  if ( msg )
  {
    // The map load failed, so delete the data layer and path list
    AfxMessageBox( (LPCTSTR)TSLUTF8Decoder(msg), MB_ICONERROR ) ;
    m_mapDataLayer->destroy() ;
    m_mapDataLayer = 0 ;
  }

  
  return TRUE;
}

bool CMapLinkStudioClientDoc::addMapToSurface(LPCTSTR lpszPathName, TSLDrawingSurface* surface)
{
  TSLMapDataLayer* mapDataLayer = new TSLMapDataLayer();

  // Clear the error stack, load the map then check for errors.
  TSLErrorStack::clear() ;

  // load the map
  mapDataLayer->loadData( TSLUTF8Encoder(lpszPathName) ) ;

  // Display any errors that have occurred
  const char * msg = TSLErrorStack::errorString( "Cannot load map\n" ) ;
  if ( msg )
  {
    // The map load failed, so delete the data layer and path list
    AfxMessageBox( (LPCTSTR)TSLUTF8Decoder(msg), MB_ICONERROR ) ;
    mapDataLayer->destroy() ;
    mapDataLayer = 0 ;
    
    return false;
  }

  // Add the map to the specified drawing surface,
  surface->addDataLayer(mapDataLayer, TSLUTF8Encoder(lpszPathName));

  return true;
}

bool CMapLinkStudioClientDoc::addToSurface( TSLDrawingSurface * surface )
{
  if ( !m_mapDataLayer  )
    return false;

  // Add the map to the specified drawing surface,
  surface->addDataLayer( m_mapDataLayer, m_mapLayerName ) ;

  return true ;
}

bool CMapLinkStudioClientDoc::detachFromSurface( TSLDrawingSurface * surface )
{
  if ( !m_mapDataLayer  )
    return false;

  // Remove the map to the specified drawing surface,
  surface->removeDataLayer( m_mapLayerName ) ;

  return true ;
}

void CMapLinkStudioClientDoc::DeleteContents() 
{
  // Clean up by destroying the map and pathlist
  if ( m_mapDataLayer )
  {
    m_mapDataLayer->destroy() ;
    m_mapDataLayer = 0 ;
  }
	
  CDocument::DeleteContents();
}

string CMapLinkStudioClientDoc::getCoordSystemName()
{
  string name = "";
  if (m_mapDataLayer)
  {
    const TSLCoordinateSystem* coordSystem = m_mapDataLayer->queryMapCoordinateSystem();
    name = string(coordSystem->name());
  }

  return (name);
}

double CMapLinkStudioClientDoc::getMapTMCperMU()
{
  if (m_mapDataLayer)
  {
    return (m_mapDataLayer->TMCperMU());
  }

  return 0.0;
}

