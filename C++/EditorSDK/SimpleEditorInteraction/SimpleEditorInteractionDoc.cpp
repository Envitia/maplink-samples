// SimpleEditorInteractionDoc.cpp : implementation of the SimpleEditorInteractionDoc class
//

#include "stdafx.h"
#include "SimpleEditorInteraction.h"
#include "SimpleEditorInteractionDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SimpleEditorInteractionDoc

const char * SimpleEditorInteractionDoc::m_mapLayerName = "map" ;

const char * SimpleEditorInteractionDoc::m_editLayerName = "edit" ;


IMPLEMENT_DYNCREATE(SimpleEditorInteractionDoc, CDocument)

BEGIN_MESSAGE_MAP(SimpleEditorInteractionDoc, CDocument)
//{{AFX_MSG_MAP(SimpleEditorInteractionDoc)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SimpleEditorInteractionDoc construction/destruction

SimpleEditorInteractionDoc::SimpleEditorInteractionDoc()
{
  // Initialise my variables
  m_mapDataLayer = 0 ;
  m_editDataLayer = 0 ;
}

SimpleEditorInteractionDoc::~SimpleEditorInteractionDoc()
{
  if ( m_editDataLayer )
  {
    m_editDataLayer->destroy() ;
    m_editDataLayer = 0 ;
  }
}

/////////////////////////////////////////////////////////////////////////////
// SimpleEditorInteractionDoc diagnostics

#ifdef _DEBUG
void SimpleEditorInteractionDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void SimpleEditorInteractionDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// SimpleEditorInteractionDoc commands

BOOL SimpleEditorInteractionDoc::OnOpenDocument(LPCTSTR lpszPathName) 
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
  else
  {
    if ( !m_editDataLayer )
      m_editDataLayer = new TSLStandardDataLayer ;
  }

  return TRUE;
}

bool SimpleEditorInteractionDoc::addToSurface( TSLDrawingSurface * surface )
{
  if ( !m_mapDataLayer  )
    return false;

  // Add the map to the specified drawing surface,
  surface->addDataLayer( m_mapDataLayer, m_mapLayerName ) ;
  surface->setDataLayerProps( m_mapLayerName, TSLPropertyVisible, true ) ;
  surface->setDataLayerProps( m_mapLayerName, TSLPropertyDetect,  true ) ;
  surface->setDataLayerProps( m_mapLayerName, TSLPropertyBuffered,true ) ;

  surface->addDataLayer( m_editDataLayer, m_editLayerName ) ;
  surface->setDataLayerProps( m_editLayerName, TSLPropertyVisible, true ) ; 
  surface->setDataLayerProps( m_editLayerName, TSLPropertyDetect,  true ) ;
  surface->setDataLayerProps( m_editLayerName, TSLPropertySelect,  true ) ;

  return true ;
}

bool SimpleEditorInteractionDoc::detachFromSurface( TSLDrawingSurface * surface )
{
  if ( !m_mapDataLayer  )
    return false;

  // Remove the map to the specified drawing surface,
  surface->removeDataLayer( m_mapLayerName ) ;

  // Remove the map to the specified drawing surface,
  surface->removeDataLayer( m_editLayerName ) ;

  return true ;
}

void SimpleEditorInteractionDoc::DeleteContents() 
{
  // Clean up by destroying the map and pathlist
  if ( m_mapDataLayer )
  {
    m_mapDataLayer->destroy() ;
    m_mapDataLayer = 0 ;
  }

  if ( m_editDataLayer )
  {
    m_editDataLayer->removeData() ;
  }
	
  CDocument::DeleteContents();
}
