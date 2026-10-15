// App6aViewerDoc.cpp : implementation of the CApp6aViewerDoc class
//

#include "stdafx.h"
#include "App6aViewer.h"
#include <direct.h>
#include <string>

#include "App6aViewerDoc.h"
#include "App6aViewerView.h"

using std::string ;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CApp6aViewerView * g_mainView;

/////////////////////////////////////////////////////////////////////////////
// CApp6aViewerDoc

const char * CApp6aViewerDoc::m_stdLayerName = "std" ;

//TSLStandardDataLayer * g_stdDataLayer ; 

IMPLEMENT_DYNCREATE(CApp6aViewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CApp6aViewerDoc, CDocument)
//{{AFX_MSG_MAP(CApp6aViewerDoc)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CApp6aViewerDoc construction/destruction

CApp6aViewerDoc::CApp6aViewerDoc()
{
  // Initialise my variables
  m_stdDataLayer  = 0 ;
  m_symbolHelper = new TSLApp6aSymbolHelper() ;

  char DIRECTORY_sh[ 1024 * 4 ] = "" ;
  sprintf( DIRECTORY_sh, "%s\\config", TSLUtilityFunctions::getMapLinkHome() ) ;
  char filename_sh [1024 * 4] ;

  sprintf (filename_sh, "%s\\%s", DIRECTORY_sh,APP6A_CONFIG_FILE_NAME ) ;

  if (!m_symbolHelper->Load(filename_sh))
  {
     
    AfxMessageBox( _T("Cannot load Symbol Configuration File.\n\n"), MB_ICONERROR )  ;
    
    exit(1) ;
  }
}

CApp6aViewerDoc::~CApp6aViewerDoc()
{
}

/////////////////////////////////////////////////////////////////////////////
// CApp6aViewerDoc diagnostics

#ifdef _DEBUG
void CApp6aViewerDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void CApp6aViewerDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CApp6aViewerDoc commands

BOOL CApp6aViewerDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
  if (!CDocument::OnOpenDocument(lpszPathName))
    return FALSE;


  return TRUE;

}

bool CApp6aViewerDoc::addToSurface( TSLDrawingSurface * surface )
{
  if ( !m_stdDataLayer  )
    return false;

  // Add the symbol layer to the specified drawing surface,
  surface->addDataLayer( m_stdDataLayer, m_stdLayerName ) ;
  return true ;
}

bool CApp6aViewerDoc::detachFromSurface( TSLDrawingSurface * surface )
{
  if ( !m_stdDataLayer  )
    return false;

  // Remove the map to the specified drawing surface,
  surface->removeDataLayer( m_stdLayerName ) ;

  return true ;
}

void CApp6aViewerDoc::DeleteContents() 
{
  // Clean up by destroying the map and pathlist
  if ( m_stdDataLayer )
  {
    m_stdDataLayer->destroy() ;
    m_stdDataLayer = 0 ;
  }
	
  CDocument::DeleteContents();
}

void CApp6aViewerDoc::UpdateMainView() 
{
  g_mainView->drawSymbols() ;
}