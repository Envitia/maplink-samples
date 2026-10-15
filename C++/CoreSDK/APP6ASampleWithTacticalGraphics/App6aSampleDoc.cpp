// APP6ASampleDoc.cpp : implementation of the CAPP6ASampleDoc class
//

#include "stdafx.h"
#include "APP6ASample.h"
#include "APP6ASampleDoc.h"
#include "APP6ASampleView.h"
#include "tslgriddatalayer.h"
#include "OpMgr.h"

//missing on NT in math.h
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CAPP6ASampleApp theApp ;

const char * CAPP6ASampleDoc::m_mapLayerName  = "map" ;
const char * CAPP6ASampleDoc::m_editLayerName = "edit" ;
const char * CAPP6ASampleDoc::m_gridLayerName = "grid" ;

/////////////////////////////////////////////////////////////////////////////
// CAPP6ASampleDoc

IMPLEMENT_DYNCREATE(CAPP6ASampleDoc, CDocument)

BEGIN_MESSAGE_MAP(CAPP6ASampleDoc, CDocument)
//{{AFX_MSG_MAP(CAPP6ASampleDoc)
	ON_COMMAND(ID_FILE_LOADMAPUNDERLAY, OnFileLoadmapunderlay)
	ON_COMMAND(ID_VIEW_GRIDLAYER, OnViewGridlayer)
	ON_UPDATE_COMMAND_UI(ID_VIEW_GRIDLAYER, OnUpdateViewGridlayer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAPP6ASampleDoc construction/destruction

CAPP6ASampleDoc::CAPP6ASampleDoc() : m_editLayer( 0 ), 
  m_mapLayer( 0 ), m_pathList( 0 )
{
  // Create the map and edit layers
  m_mapLayer  = new TSLMapDataLayer();

  m_gridLayer = new TSLGridDataLayer ;

  m_editLayer = new TSLStandardDataLayer ;

  // Set up the grid rendering - the units will be set later on
  int gridMajorLineColour, gridMajorLineStyle, gridMajorLineThickness ;
  int gridMinorLineColour, gridMinorLineStyle, gridMinorLineThickness ;
  int gridAxisColour, gridAxisStyle, gridAxisThickness ;
  int gridLayerVisible ;

  TSLProfileHelper::setDefaultSection("grid");

  TSLProfileHelper::lookupProfile("gridScale",   &m_gridScale,   100.0 );
  TSLProfileHelper::lookupProfile("gridXOrigin", &m_gridXOrigin, 0 );
  TSLProfileHelper::lookupProfile("gridYOrigin", &m_gridYOrigin, 0 );
  TSLProfileHelper::lookupProfile("gridLayerVisible", &gridLayerVisible, 0 );
  m_gridLayerVisible = (bool)gridLayerVisible ;

  TSLProfileHelper::lookupProfile("gridMajorLineColour",   &gridMajorLineColour, 7 );
  TSLProfileHelper::lookupProfile("gridMajorLineStyle"    ,&gridMajorLineStyle, 1 );
  TSLProfileHelper::lookupProfile("gridMajorLineThickness",&gridMajorLineThickness, 1 );
  TSLProfileHelper::lookupProfile("gridMinorLineColour",   &gridMinorLineColour, 7 );
  TSLProfileHelper::lookupProfile("gridMinorLineStyle"    ,&gridMinorLineStyle, 1 );
  TSLProfileHelper::lookupProfile("gridMinorLineThickness",&gridMinorLineThickness, 1 );
  TSLProfileHelper::lookupProfile("gridAxisColour",   &gridAxisColour, 7 );
  TSLProfileHelper::lookupProfile("gridAxisStyle"    ,&gridAxisStyle, 1 );
  TSLProfileHelper::lookupProfile("gridAxisThickness",&gridAxisThickness, 1 );

  m_gridLayer->setFeatureRendering("xmajor", 0, TSLRenderingAttributeEdgeColour, gridMajorLineColour);
  m_gridLayer->setFeatureRendering("xmajor", 0, TSLRenderingAttributeEdgeStyle, gridMajorLineStyle);
  m_gridLayer->setFeatureRendering("xmajor", 0, TSLRenderingAttributeEdgeThickness, (double)gridMajorLineThickness);
  m_gridLayer->setFeatureRendering("ymajor", 0, TSLRenderingAttributeEdgeColour, gridMajorLineColour);
  m_gridLayer->setFeatureRendering("ymajor", 0, TSLRenderingAttributeEdgeStyle, gridMajorLineStyle);
  m_gridLayer->setFeatureRendering("ymajor", 0, TSLRenderingAttributeEdgeThickness, (double)gridMajorLineThickness);
  m_gridLayer->setFeatureRendering("xminor", 0, TSLRenderingAttributeEdgeColour, gridMinorLineColour);
  m_gridLayer->setFeatureRendering("xminor", 0, TSLRenderingAttributeEdgeStyle, gridMinorLineStyle);
  m_gridLayer->setFeatureRendering("xminor", 0, TSLRenderingAttributeEdgeThickness, (double)gridMinorLineThickness);
  m_gridLayer->setFeatureRendering("yminor", 0, TSLRenderingAttributeEdgeColour, gridMinorLineColour);
  m_gridLayer->setFeatureRendering("yminor", 0, TSLRenderingAttributeEdgeStyle, gridMinorLineStyle);
  m_gridLayer->setFeatureRendering("yminor", 0, TSLRenderingAttributeEdgeThickness, (double)gridMinorLineThickness);
  m_gridLayer->setFeatureRendering("xaxis", 0, TSLRenderingAttributeEdgeColour, gridAxisColour);
  m_gridLayer->setFeatureRendering("xaxis", 0, TSLRenderingAttributeEdgeStyle, gridAxisStyle);
  m_gridLayer->setFeatureRendering("xaxis", 0, TSLRenderingAttributeEdgeThickness, (double)gridAxisThickness);
  m_gridLayer->setFeatureRendering("yaxis", 0, TSLRenderingAttributeEdgeColour, gridAxisColour);
  m_gridLayer->setFeatureRendering("yaxis", 0, TSLRenderingAttributeEdgeStyle, gridAxisStyle);
  m_gridLayer->setFeatureRendering("yaxis", 0, TSLRenderingAttributeEdgeThickness, (double)gridAxisThickness);

  m_gridLayer->snapable( false );

  // Get the map from the initialisation file
  TSLEnvelope extent ;
  TSLProfileHelper::setDefaultSection( "paths" ) ;

  // Load a map if one is specified in the ini file
  char mapName[ TSL_MAX_PATH ] = "" ;

  TSLProfileHelper::lookupProfile( "map", mapName, TSL_MAX_PATH, "" ) ;
  
  if ( mapName[0] != '\0' )
  {
    // Firstly check for non-absolute path
    if ( mapName[0] != '\\' && !(strlen( mapName ) > 1 && mapName[1] == ':' ) )
    {
      // At this point, any relative path is relative to the iniFile rootDirectory
      std::string rootDirectory = theApp.rootDirectory() ;
      rootDirectory += mapName ;

      // Now use _fullPath to resolve any relative path specifed
      _fullpath( mapName, rootDirectory.c_str(), MAX_PATH ) ;
    }

    loadMap( mapName ) ;
  }
}

CAPP6ASampleDoc::~CAPP6ASampleDoc()
{
  if ( m_mapLayer )
  {
    m_mapLayer->destroy() ;
    m_mapLayer = 0 ;
  }

  if ( m_editLayer )
  {
    m_editLayer->destroy() ;
    m_editLayer = 0 ;
  }

  if ( m_gridLayer )
  {
    m_gridLayer->destroy() ;
    m_gridLayer = 0 ;
  }

  if ( m_pathList )
  {
    delete m_pathList ;
    m_pathList = 0 ;
  }
}

/////////////////////////////////////////////////////////////////////////////
// CAPP6ASampleDoc diagnostics

#ifdef _DEBUG
void CAPP6ASampleDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void CAPP6ASampleDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAPP6ASampleDoc commands

void CAPP6ASampleDoc::DeleteContents() 
{
  COpMgr::reset() ;

  CDocument::DeleteContents();
}

bool CAPP6ASampleDoc::addToSurface( TSLDrawingSurface * surface )
{
  if ( m_mapLayer && m_editLayer  )
  {
    surface->addDataLayer( m_mapLayer, m_mapLayerName ) ;
    surface->setDataLayerProps( m_mapLayerName, TSLPropertyVisible, true ) ;
    surface->setDataLayerProps( m_mapLayerName, TSLPropertyDetect,  true ) ;
    surface->setDataLayerProps( m_mapLayerName, TSLPropertyBuffered,true ) ;

    surface->addDataLayer( m_gridLayer, m_gridLayerName ) ;
    surface->setDataLayerProps( m_gridLayerName, TSLPropertyVisible, m_gridLayerVisible ) ;
    surface->setDataLayerProps( m_gridLayerName, TSLPropertyDetect,  true ) ;

    surface->addDataLayer( m_editLayer, m_editLayerName ) ;
    surface->setDataLayerProps( m_editLayerName, TSLPropertyVisible, true ) ; 
    surface->setDataLayerProps( m_editLayerName, TSLPropertyDetect,  true ) ;
    surface->setDataLayerProps( m_editLayerName, TSLPropertySelect,  true ) ;
  }

  return true ;
}

bool CAPP6ASampleDoc::detachFromSurface( TSLDrawingSurface * surface )
{
  COpMgr::reset() ;

  if ( m_mapLayer && m_editLayer )
  {
    surface->removeDataLayer( m_mapLayerName ) ;
    surface->removeDataLayer( m_gridLayerName ) ;
    surface->removeDataLayer( m_editLayerName ) ;
  }

  return true ;
}

TSLEnvelope CAPP6ASampleDoc::getResetArea( )
{
  TSLTMC x1, y1, x2, y2 ;

  if ( !m_editLayer->getTMCExtent( &x1, &y1, &x2, &y2 ) )
  {
    if ( !m_mapLayer->getTMCExtent( &x1, &y1, &x2, &y2 ) )
    {
      // If nothing loaded into edit or map layer, reset to 
      // some default canvas
      x1 = y1 = -(LONG_MAX / 4) ;
      x2 = y2 =  (LONG_MAX / 4) ;
    }
  }

  return TSLEnvelope( x1, y1, x2, y2 ) ;
}

BOOL CAPP6ASampleDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
  if (!CDocument::OnOpenDocument(lpszPathName))
    return FALSE;

  TCHAR ext[_MAX_EXT];

  _tsplitpath( lpszPathName, NULL, NULL, NULL, ext );

  if ( _tcscmp( ext, _T(".tmf") ) )
    return FALSE ;

  COpMgr::reset() ;

  if ( !m_editLayer->loadData( TSLUTF8Encoder(lpszPathName) ) )
    return FALSE ;

  COpMgr::dataChanged() ;

  // Now reset the view
  POSITION pos = GetFirstViewPosition();
  CView* pFirstView = GetNextView( pos );
  CAPP6ASampleView* vw = (CAPP6ASampleView*)pFirstView;
  vw->OnToolsReset();

  return TRUE;
}

BOOL CAPP6ASampleDoc::OnNewDocument() 
{
  if ( m_editLayer )
  {
    COpMgr::reset() ;

    m_editLayer->removeData() ;

    COpMgr::dataChanged() ;
  }
  return CDocument::OnNewDocument();
}

BOOL CAPP6ASampleDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
  COpMgr::reset() ;

  if ( !m_editLayer->saveData( TSLUTF8Encoder(lpszPathName) ) )
    return FALSE ;

  return TRUE;
}

void CAPP6ASampleDoc::OnFileLoadmapunderlay() 
{
  TCHAR szFilter[] = _T("MapLink Maps (*.map)|*.map||") ;

  CFileDialog dlog( TRUE, NULL, NULL, 0, szFilter ) ;
  dlog.m_ofn.lpstrInitialDir = _T(".\\map");
  dlog.m_ofn.Flags |= OFN_NOCHANGEDIR;
  dlog.DoModal( ) ;

  CString lpszPathName = dlog.GetPathName( ) ;

  // Load into the appropriate layer
  if ( lpszPathName.GetLength() )
  { 
    loadMap( TSLUTF8Encoder(lpszPathName) ) ;

    POSITION pos = GetFirstViewPosition();
    CView* pFirstView = GetNextView( pos );
    CAPP6ASampleView* vw = (CAPP6ASampleView*)pFirstView;
    vw->OnToolsReset();
  }
}

void CAPP6ASampleDoc::OnViewGridlayer() 
{
  POSITION pos = GetFirstViewPosition();

  m_gridLayerVisible = !m_gridLayerVisible ;

  while (pos != NULL)
  {
    CAPP6ASampleView * view = (CAPP6ASampleView *)GetNextView( pos ) ;

    if ( view->IsKindOf(RUNTIME_CLASS(CAPP6ASampleView)) )
    {
      TSLDrawingSurface * surface = view->queryDrawingSurface() ;
      surface->setDataLayerProps( m_gridLayerName, TSLPropertyVisible, m_gridLayerVisible ) ;
    }
  }

  UpdateAllViews( 0 ) ;
}

void CAPP6ASampleDoc::OnUpdateViewGridlayer(CCmdUI* pCmdUI) 
{
  pCmdUI->SetCheck( m_gridLayerVisible ? 1 : 0 );
}

bool CAPP6ASampleDoc::loadMap( const char * mapFile )
{
  // Clear the error stack, load the map then check for errors.
  TSLErrorStack::clear() ;

  m_mapLayer->loadData( mapFile ) ;

  // Display any errors that have occurred
  const char * msg = TSLErrorStack::errorString( "Cannot load map\n" ) ;
  if ( msg )
  {
    // The map load failed, so delete the data layer and path list, then exit
    AfxMessageBox( (LPCTSTR)TSLUTF8Decoder(msg), MB_ICONERROR ) ;
    return false ;
  }

  // Map successfully loaded, set the grid spacing
  m_gridLayer->setUnits( m_gridScale * m_mapLayer->TMCperMU(), m_gridXOrigin, m_gridYOrigin ) ;
  m_gridLayer->setSpacing( "xmajor", 1000.0 ) ;
  m_gridLayer->setSpacing( "ymajor", 1000.0 ) ;
  m_gridLayer->setSpacing( "xminor", 100.0 ) ;
  m_gridLayer->setSpacing( "yminor", 100.0 ) ;

  return true ;
}
