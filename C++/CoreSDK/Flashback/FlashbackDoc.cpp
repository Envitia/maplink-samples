// FlashbackDoc.cpp : implementation of the FlashbackDoc class
//

#include "stdafx.h"
#include "Flashback.h"
#include <direct.h>

#include "FlashbackDoc.h"
#include "ArchiveDialog.h"


#include <io.h>

#include <sstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// FlashbackDoc

const char * FlashbackDoc::m_mapLayerName = "map" ;


static
void getArchiveDirectory(void* arg, const char* layerName, TSLHistoryVersion version, char* directory)
{
  FlashbackDoc* doc = (FlashbackDoc*)arg;
  doc->getArchiveRoot( layerName, version, directory );
}


IMPLEMENT_DYNCREATE(FlashbackDoc, CDocument)

BEGIN_MESSAGE_MAP(FlashbackDoc, CDocument)
//{{AFX_MSG_MAP(FlashbackDoc)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// FlashbackDoc construction/destruction

FlashbackDoc::FlashbackDoc()
{
  // Initialise my variables
  m_mapDataLayer = 0 ;

  m_archiveRoot = "C:\\temp";
  m_archiveInSubdir = true;
}

FlashbackDoc::~FlashbackDoc()
{
}

/////////////////////////////////////////////////////////////////////////////
// FlashbackDoc diagnostics

#ifdef _DEBUG
void FlashbackDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void FlashbackDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// FlashbackDoc commands

BOOL FlashbackDoc::OnOpenDocument(LPCTSTR lpszPathName) 
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

  CWnd* pMainWnd = AfxGetMainWnd();
  if ( m_mapDataLayer && pMainWnd->IsWindowVisible() ) 
  {
    TSLVersionHistorySet const* history = m_mapDataLayer->versionHistory();
    if ( history )
    {
      pMainWnd->SendMessage( UM_AVAILABLE_VERSIONS, 0, (LPARAM)history );

      if ( !history->empty() )
      {
        // Ask the user where the archives for this map are stored.
        CArchiveDialog dlg( m_archiveRoot.c_str(), pMainWnd );
        if ( dlg.DoModal() == IDOK )
        {
          m_archiveRoot = TSLUTF8Encoder(dlg.archiveRoot());
          m_archiveInSubdir = dlg.archiveInSubdir();
        }
        else
        {
          m_archiveRoot.clear();
        }
      }
    }
  }

  return TRUE;
}

bool FlashbackDoc::addToSurface( TSLDrawingSurface * surface )
{
  if ( !m_mapDataLayer  )
    return false;

  // Add the map to the specified drawing surface,
  surface->addDataLayer( m_mapDataLayer, m_mapLayerName ) ;

  return true ;
}

bool FlashbackDoc::detachFromSurface( TSLDrawingSurface * surface )
{
  if ( !m_mapDataLayer  )
    return false;

  // Remove the map to the specified drawing surface,
  surface->removeDataLayer( m_mapLayerName ) ;

  return true ;
}

void FlashbackDoc::DeleteContents() 
{
  // Clean up by destroying the map and pathlist
  if ( m_mapDataLayer )
  {
    m_mapDataLayer->destroy() ;
    m_mapDataLayer = 0 ;
  }
	
  CDocument::DeleteContents();
}

void FlashbackDoc::flashback(TSLHistoryVersion version)
{
  if ( version == (TSLHistoryVersion)0 )
  {
    m_mapDataLayer->cancelFlashback();
  }
  else
  {
    m_mapDataLayer->flashbackToVersion( version, getArchiveDirectory, this );
  }

  m_mapDataLayer->notifyChanged( true );

  UpdateAllViews( 0 );
}

void FlashbackDoc::getArchiveRoot(const char* layerName, TSLHistoryVersion version, char* directory)
{
  if ( m_archiveRoot.empty() )
  {
    return;
  }

  std::string archiveDir;
  std::ostringstream ss;
  ss << m_archiveRoot;
  
  if ( m_archiveInSubdir )
  {
    ss << '\\' << version;
    archiveDir = ss.str();
  }
  else
  {
    archiveDir = m_archiveRoot;
  }

  if( TSLFileHelper::fileExists( archiveDir.c_str() ) )
  {
    strncpy( directory, archiveDir.c_str(), 1024 );
  }
}
