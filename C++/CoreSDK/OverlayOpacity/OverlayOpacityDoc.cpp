// OverlayOpacityDoc.cpp : implementation of the COverlayOpacityDoc class
//

#include "stdafx.h"
#include "OverlayOpacityDoc.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COverlayOpacityDoc





IMPLEMENT_DYNCREATE(COverlayOpacityDoc, CDocument)

BEGIN_MESSAGE_MAP(COverlayOpacityDoc, CDocument)
//{{AFX_MSG_MAP(COverlayOpacityDoc)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COverlayOpacityDoc construction/destruction

COverlayOpacityDoc::COverlayOpacityDoc()
{
  // Initialise my variables
  m_mapDataLayers.clear() ;
  m_mapLayerNames.clear() ;

  m_cacheSize = 64;

  // Range of Opacity value is range of TSLPropertyTransparency
  // Between 0 and 255, where 255 is fully opaque and 0 is not visible.
  // Initialise opacity value to fully opaque
  m_opacity = 255;
}

COverlayOpacityDoc::~COverlayOpacityDoc()
{
}

/////////////////////////////////////////////////////////////////////////////
// COverlayOpacityDoc diagnostics

#ifdef _DEBUG
void COverlayOpacityDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void COverlayOpacityDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COverlayOpacityDoc commands

BOOL COverlayOpacityDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
  if (!CDocument::OnOpenDocument(lpszPathName))
    return FALSE;
  
  // This routine is called when the user selects to open a new document.
  // All other open documents are cleared ( DeleteContents ) and the 
  // new map is added to element 0 of the m_mapDataLayers vector. This
  // is done within the addMapDataLayer function.
  return addMapDataLayer( lpszPathName );
}

bool COverlayOpacityDoc::addToSurface(TSLDrawingSurface *surface, int index, bool layerBuffered)
{
  if ( m_mapDataLayers.empty() || m_mapDataLayers.size()-1 < index )
    return false ;
  
  std::string layer_name ;
  getMapName ( layer_name, index ) ;

  // Add to the surface.
  surface->addDataLayer( m_mapDataLayers[index], layer_name.c_str() ) ;
  surface->setDataLayerProps( layer_name.c_str(),  TSLPropertyDetect, 1 ) ;

  // If it's an Overlay set the current Opacity
  if ( index > 0 )
  {
    surface->setDataLayerProps( layer_name.c_str(),  TSLPropertyTransparency, m_opacity ) ;
  }

  if ( layerBuffered )
  {
     surface->setDataLayerProps( layer_name.c_str(),  TSLPropertyBuffered, 1 ) ;
  }

  return true ;
}

bool COverlayOpacityDoc::updateOverlayLayers( TSLDrawingSurface *surface )
{
  if ( m_mapDataLayers.size() < 2 )
    return false ;

  // Number of overlays is one less than the number of mapdatalayers
  // but we're initialising the loop index at 1
  int numOverlays = m_mapDataLayers.size();
  for ( size_t loop = 1; loop < numOverlays; ++loop )
  {
    std::string layer_name ;
    getMapName ( layer_name, loop ) ;
    surface->setDataLayerProps( layer_name.c_str(),  TSLPropertyTransparency, m_opacity ) ;
  }

  return true; 
}


void COverlayOpacityDoc::detachFromSurface( TSLDrawingSurface * surface, int index)
{
  //Loop through and remove the attached surfaces.
  for ( int ii = 0; ii <= index; ++ii ) 
  {
    // Create the map layer name.
    CString layer_name ;
    layer_name.Format ( _T("Layer_%d"), index ) ;
    surface->removeDataLayer( TSLUTF8Encoder(layer_name) ) ; 
  }
}

void COverlayOpacityDoc::DeleteContents() 
{
  // Data layers now stored in a vector so loop through and cleanup.
  for ( size_t loop = 0; loop < m_mapDataLayers.size(); ++loop )
  {
    m_mapDataLayers[loop]->destroy() ;
    m_mapDataLayers[loop] = 0 ;
  }

  // All elements are destroyed so we can clear the vector. 
  m_mapDataLayers.clear() ;
  m_mapLayerNames.clear() ;
  
  CDocument::DeleteContents();
}

bool COverlayOpacityDoc::addMapDataLayer(LPCTSTR lpszPathName)
{

  // This function is used to add all MapDataLayers to the application. Each data
  // layer ( except the first ) checks that it is using the same coordinate system
  // as the first before being saved in the vector.
  
  // Check that the map is not already loaded.
  for ( size_t ii = 0; ii < m_mapLayerNames.size(); ++ii )
  {
    //The lpszPathName should not exist unless the map is loaded.
    if ( _tcscmp( lpszPathName, (LPCTSTR)TSLUTF8Decoder(m_mapLayerNames[ii]) ) == 0 )
      return FALSE ;
  }
  
  // Create the new map data layer.
  TSLMapDataLayer * dataLayer = new TSLMapDataLayer() ;
  dataLayer->cacheSize(1024);

  // Clear the error stack, load the map then check for errors.
  TSLErrorStack::clear() ;

  // load the map
  dataLayer->loadData( TSLUTF8Encoder(lpszPathName) ) ;
  
  // Display any errors that have occurred
  const char * msg = TSLErrorStack::errorString( "Cannot load map\n" ) ;
  if ( msg )
  {
    // Error: The map failed to load.
    AfxMessageBox( (LPCTSTR)TSLUTF8Decoder(msg), MB_ICONERROR ) ;
    return FALSE ;
  }

  // Check the coordinate systems. ( Not the first time, ie empty vector ).
  if ( !m_mapDataLayers.empty() ) 
  {
    const TSLCoordinateSystem * firstCS = m_mapDataLayers[0]->queryCoordinateSystem() ;
    const TSLCoordinateSystem * newCS = dataLayer -> queryCoordinateSystem() ;

    if ( newCS -> id() != firstCS -> id() )
    {
      // Error: The coordinate systems are not the same.
      CString err;
      CString ferr;
      err.LoadString ( IDS_COORD_SYS_NOT_EQUAL );
      ferr.Format ( err, firstCS->name(), newCS->name() ) ;
      AfxMessageBox( ferr, MB_ICONINFORMATION ) ;
      return FALSE ;
    }

    if ( newCS -> getTMCperMU () != firstCS -> getTMCperMU() )
    {
      CString err;
      err.LoadString ( IDS_MAP_UNITS_NOT_EQUAL ) ;
      AfxMessageBox( err, MB_ICONINFORMATION ) ;
      return FALSE ;
    }
      
  }
  
  // Set the cache_size used for tiling.
  dataLayer->cacheSize(m_cacheSize * 1024);

  // Success: Save the layer in the vector.
  m_mapDataLayers.push_back( dataLayer ) ;

  // Save the path name in the vector.
  std::string temp = TSLUTF8Encoder( lpszPathName );
  m_mapLayerNames.push_back( temp ) ;
  
  // Update the main windows text.
  string::size_type len = temp.rfind("\\",temp.length()) + 1 ;
  
  // Get the file name. Could use getMapName although that
  // does not return file suffix.
  temp = temp.substr (len, temp.length() - len ) ;
  
  // Get the current window text.
  CString window_text ; 

  AfxGetMainWnd()->GetWindowText( window_text ) ;
  
  // Insert the new file name before the " - MapLink"
  window_text.Insert ( window_text.Find ( _T(" -"), 0 ) , _T(", ") ) ;
  window_text.Insert ( window_text.Find ( _T(" -"), 0 ) , (LPCTSTR)TSLUTF8Decoder( temp.c_str() ) ) ;
  
  // Update.
  AfxGetMainWnd()->SetWindowText ( window_text ) ;
  
  return TRUE ;

}

void COverlayOpacityDoc::cacheSize(int cs)
{
  
  // This routine is used to update the cache size.
  
  m_cacheSize = cs;
  
  // Loop through each layer and update.
  for ( size_t loop = 0; loop < m_mapDataLayers.size(); ++loop )
    m_mapDataLayers[loop]->cacheSize( m_cacheSize * 1024 ) ;
}

TSLMapDataLayer * COverlayOpacityDoc::getMapDataLayer(int index)
{
  // Overloaded function to return a specific data layer,
  // rather than the first one.
  if ( m_mapDataLayers.empty() || m_mapDataLayers.size()-1 < index )
  {
    return (TSLMapDataLayer *) 0 ;
  }
  else
  {
    return m_mapDataLayers[index] ;
  }
}




void COverlayOpacityDoc::getMapName(std::string &name, int index)
{
  if ( index <= m_mapLayerNames.size() )
  {
    
    //We want the part after the final
    // '/'. IE c:/progs/map.map we want map
    name = m_mapLayerNames[index] ;
    
    string::size_type len ;

    len = name.rfind("\\",name.length()) + 1 ;

    name = name.substr (len, name.length() - len ) ;

    len = name.rfind ( ".", name.length () ) ;

    name = name.substr ( 0, len ) ;

  }
}

void COverlayOpacityDoc::getFilePathName(CString &path)
{
  
  // Returns the file path name of any maps added using
  // the 'Add Map' command off the 'File' menu.
  CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR,
                  _T("MapLink Maps (*.map)|*.map|All Files (*.*)|*.*||") );
  
  if ( dlg.DoModal() == IDOK )
    path = dlg.GetPathName() ;
  else
    path = "";
}

bool COverlayOpacityDoc::opacity( int opacity )
{
  // Range of Opacity value is range of TSLPropertyTransparency
  // i.e. between 0 and 255, where 255 is fully opaque and 0 is not visible.
  if ( opacity < 0 || opacity > 255 )
  {
    return false;
  }

  m_opacity = opacity;

  return true;
}

