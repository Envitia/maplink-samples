// Hello GlobeDoc.cpp : implementation of the CHelloGlobeDoc class
//

#include "stdafx.h"
#include "Hello Globe.h"

#include "Hello GlobeDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHelloGlobeDoc

IMPLEMENT_DYNCREATE(CHelloGlobeDoc, CDocument)

BEGIN_MESSAGE_MAP(CHelloGlobeDoc, CDocument)
	//{{AFX_MSG_MAP(CHelloGlobeDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHelloGlobeDoc construction/destruction

CHelloGlobeDoc::CHelloGlobeDoc()
{
  m_mapDataLayer = 0 ;
}

CHelloGlobeDoc::~CHelloGlobeDoc()
{
}

BOOL CHelloGlobeDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CHelloGlobeDoc serialization

void CHelloGlobeDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}

/////////////////////////////////////////////////////////////////////////////
// CHelloGlobeDoc diagnostics

#ifdef _DEBUG
void CHelloGlobeDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CHelloGlobeDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHelloGlobeDoc commands

BOOL CHelloGlobeDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
  if (!CDocument::OnOpenDocument(lpszPathName))
    return FALSE;
  
  m_mapDataLayer = new TSLMapDataLayer() ;

  if ( !m_mapDataLayer->loadData( TSLUTF8Encoder(lpszPathName) ) )
  {
    const char * msg = TSLErrorStack::errorString( "Cannot load map : \n" ) ;
    if ( msg ) 
      AfxMessageBox( (LPCTSTR)TSLUTF8Decoder(msg), MB_OK ) ;

    m_mapDataLayer->destroy() ;
    m_mapDataLayer = 0 ;

    return FALSE ;
  }
  
  return TRUE;
}

void CHelloGlobeDoc::DeleteContents() 
{
  if ( m_mapDataLayer )
  {
    m_mapDataLayer->destroy() ;
    m_mapDataLayer = 0 ;
  }
	
  CDocument::DeleteContents();
}

bool CHelloGlobeDoc::addToSurface( TSLDrawingSurface * drawingSurface )
{
  if ( !m_mapDataLayer || !drawingSurface )
    return false ;

  return drawingSurface->addDataLayer( m_mapDataLayer, "map" ) ;

}
