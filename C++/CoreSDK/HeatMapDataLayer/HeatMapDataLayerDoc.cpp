// HeatMapDataLayerDoc.cpp : implementation of the HeatMapDataLayerDoc class
//

#include "stdafx.h"
#include "HeatMapDataLayerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// HeatMapDataLayerDoc

const char * HeatMapDataLayerDoc::m_mapLayerName = "map" ;


IMPLEMENT_DYNCREATE(HeatMapDataLayerDoc, CDocument)

BEGIN_MESSAGE_MAP(HeatMapDataLayerDoc, CDocument)
//{{AFX_MSG_MAP(HeatMapDataLayerDoc)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// HeatMapDataLayerDoc construction/destruction

HeatMapDataLayerDoc::HeatMapDataLayerDoc()
{
  // Initialise my variables
  m_mapDataLayer = 0 ;
}

HeatMapDataLayerDoc::~HeatMapDataLayerDoc()
{
}

/////////////////////////////////////////////////////////////////////////////
// HeatMapDataLayerDoc diagnostics

#ifdef _DEBUG
void HeatMapDataLayerDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void HeatMapDataLayerDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// HeatMapDataLayerDoc commands

BOOL HeatMapDataLayerDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
  if (!CDocument::OnOpenDocument(lpszPathName))
    return FALSE;
  
  // Create a map data layer
  m_mapDataLayer = new TSLMapDataLayer();
  
  // Clear the error stack, load the map then check for errors.
  TSLErrorStack::clear() ;

  // Load the map
  // This will automatically load all associated files such as 
  // the .pth and .pal files.  These needed to be loaded
  // directly by the application in earlier versions of MapLink
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

bool HeatMapDataLayerDoc::addToSurface( TSLDrawingSurface * surface )
{
  if ( !m_mapDataLayer  )
    return false;

  // Add the map to the specified drawing surface,
  surface->addDataLayer( m_mapDataLayer, m_mapLayerName ) ;

  return true ;
}

bool HeatMapDataLayerDoc::detachFromSurface( TSLDrawingSurface * surface )
{
  if ( !m_mapDataLayer  )
    return false;

  // Remove the map to the specified drawing surface,
  surface->removeDataLayer( m_mapLayerName ) ;

  return true ;
}

void HeatMapDataLayerDoc::DeleteContents() 
{
  // Clean up by destroying the map and pathlist
  if ( m_mapDataLayer )
  {
    m_mapDataLayer->destroy() ;
    m_mapDataLayer = 0 ;
  }
	
  CDocument::DeleteContents();
}
