// ActiveXCPPCtrl.cpp : Implementation of CActiveXCPPCtrl

#include "stdafx.h"
#include "ActiveXCPP.h"
#include "ActiveXCPPCtrl.h"
#include <locale>

#include "tslviewmode.h"
#include "tslviewmodezoom.h"
#include "tslviewmodepan.h"
#include "tslviewmodegrab.h"
#include "tslviewmodeinterfacent.h"

// We need this declared to clip the region properly
#ifndef SYSRGN
#define SYSRGN 4
extern "C" WINGDIAPI int WINAPI GetRandomRgn (IN HDC, IN HRGN, IN INT);
#endif

using std::string;

const char * CActiveXCPPCtrl::m_mapLayerName = "map" ;

/////////////////////////////////////////////////////////////////////////////
// CActiveXCPPCtrl
CActiveXCPPCtrl::CActiveXCPPCtrl()
: m_drawingSurface(0), m_mapDataLayer(0), m_remoteFileLoader(0),
  m_modeInterface(0), m_currentMode(0), m_zoomPercentage(30), m_memoryCacheSize(32*1024)
{
  m_bWindowOnly = TRUE;

  m_hZoomCursor     = LoadCursor(_Module.m_hInst, MAKEINTRESOURCE(IDC_ZOOM) );
  m_hPanCursor      = LoadCursor(_Module.m_hInst, MAKEINTRESOURCE(IDC_PAN) );
  m_hGrabUpCursor   = LoadCursor(_Module.m_hInst, MAKEINTRESOURCE(IDC_GRAB_UP) );
  m_hGrabDownCursor = LoadCursor(_Module.m_hInst, MAKEINTRESOURCE(IDC_GRAB_DOWN) );
}

CActiveXCPPCtrl::~CActiveXCPPCtrl()
{
  delete m_modeInterface;
  delete m_currentMode;
}

STDMETHODIMP CActiveXCPPCtrl::get_mapName(BSTR *pVal)
{
  *pVal = _bstr_t(m_mapFileName.c_str()).copy();

  return S_OK;
}

STDMETHODIMP CActiveXCPPCtrl::put_mapName(BSTR newVal)
{
  m_mapFileName = _bstr_t(newVal,true);
  
  return S_OK;
}

STDMETHODIMP CActiveXCPPCtrl::get_serverMapPath(BSTR *pVal)
{
  *pVal = _bstr_t(m_serverMapPath.c_str()).copy();
  
  return S_OK;
}

STDMETHODIMP CActiveXCPPCtrl::put_serverMapPath(BSTR newVal)
{
  m_serverMapPath = _bstr_t(newVal,true);

  return S_OK;
}


STDMETHODIMP CActiveXCPPCtrl::get_serverConfigPath(BSTR *pVal)
{
  *pVal = _bstr_t(m_serverConfigPath.c_str()).copy();
  
  return S_OK;
}

STDMETHODIMP CActiveXCPPCtrl::put_serverConfigPath(BSTR newVal)
{
  m_serverConfigPath = _bstr_t(newVal, true);
 
  return S_OK;
}

TSLLoaderCallbackReturn CActiveXCPPCtrl::staticLoaderCallback( void* arg, const char* filename, TSLEnvelope extent, TSLLoaderStatus status, int percentDone )
{
  HWND hwnd = reinterpret_cast<HWND>(arg);

  if( status == TSLLoadingOK && percentDone == 100 )
  {
    TSLEnvelope *pEnv = new TSLEnvelope(extent);

    ::PostMessage(hwnd, AM_LOADER_CALLBACK, 0, reinterpret_cast<long>(pEnv) );
  }

  return TSLContinue;
}

LRESULT CActiveXCPPCtrl::OnLoaderCallback(UINT uMsg, WPARAM wParamUnused, LPARAM pEnv, BOOL& bHandled)
{
  m_mapDataLayer->notifyChanged();

  // Since there can be issues with overlapping text, we usually redraw the entire
  // view for each tile - to get a more visually pleasing display.
#ifdef REDRAW_TILE_EXTENT_ONLY

  TSLEnvelope *envelope = reinterpret_cast<TSLEnvelope*>(pEnv);

  // map to du's 
  RECT rc;
  m_drawingSurface->TMCToDU ( envelope->xMin(), envelope->yMin(), &rc.left, &rc.bottom );
  m_drawingSurface->TMCToDU ( envelope->xMax(), envelope->yMax(), &rc.right, &rc.top );

  InvalidateRect(&rc, FALSE);

  delete envelope;
#else

  InvalidateRect( NULL, FALSE);

#endif

  return TRUE;
}

STDMETHODIMP CActiveXCPPCtrl::get_interactionMode(BSTR *pVal)
{
  *pVal = _bstr_t(m_interactionModeName.c_str()).copy();

  return S_OK;
}

STDMETHODIMP CActiveXCPPCtrl::put_interactionMode(BSTR newVal)
{
  m_interactionModeName = _bstr_t(newVal,true);

  // make lower case
  std::locale local("English_UK");

#if ( _MSC_VER < 1300 )
  using std::use_facet ;
  _USE( local, std::ctype<char> ).tolower( &m_interactionModeName[0], &m_interactionModeName[ m_interactionModeName.size() ] ) ;
#else
  std::use_facet< std::ctype< char > > (local).tolower(&m_interactionModeName[0], &m_interactionModeName[m_interactionModeName.size()]);
#endif

  if( m_interactionModeName == "zoom" )
  {
    delete m_currentMode;
    m_currentMode = new TSLViewModeZoom( m_modeInterface, 0, m_zoomPercentage, m_hZoomCursor) ;
  }
  else if( m_interactionModeName == "pan" )
  {
    delete m_currentMode;
    m_currentMode = new TSLViewModePan( m_modeInterface, 0, m_hPanCursor) ;
  }
  else if( m_interactionModeName == "grab" )
  {
    delete m_currentMode;
    m_currentMode = new TSLViewModeGrab( m_modeInterface, 0, m_hGrabUpCursor, m_hGrabDownCursor) ;
  }
  else
  {
    delete m_currentMode;
  }

  return S_OK;
}

HRESULT CActiveXCPPCtrl::OnDraw(ATL_DRAWINFO& di)
{
  RECT& rc = *(RECT*)di.prcBounds;
  
  BOOL userMode = TRUE;

  if ( FAILED(GetAmbientUserMode( userMode ) ) )
  {
    // more likely to fail in run-mode that design mode (???)
    userMode = TRUE;
  }

  if ( userMode )
  {
    // Run mode

    if ( m_errors.size() == 0 )
    {
      // No errors and a map loaded ok.
      // Query the system clip region
      HRGN rgn = CreateRectRgn(0,0,0,0);
      int result = GetRandomRgn( di.hdcDraw, rgn, SYSRGN);
      if( result > 0)
      {
	GetRgnBox(rgn, &rc);
	
	if( !m_bIsWindows9X ) // on 95/98 the returned value is already in client coordinates
	  ScreenToClient(&rc);
      }
      DeleteObject(rgn);
      
      // redraw the map. Since we're double buffered, we don't need to clear
      // the background first...
      m_drawingSurface->drawDU( rc.left, rc.top, rc.right, rc.bottom, true )  ;
      
      m_modeInterface->drawEchoRectangle( ) ;
    }
    else
    {
      // We've had some errors during initialisation
      // Draw them instead of the map.
      Rectangle(di.hdcDraw, rc.left, rc.top, rc.right, rc.bottom);
      
      InflateRect(&rc, -2, -2);
      for(size_t i=0; i<m_errors.size(); ++i)
      {
        TSLUTF8Decoder text( m_errors[i].c_str() );
        int height = DrawText(di.hdcDraw, (LPCTSTR)text, lstrlen((LPCTSTR)text), &rc, DT_LEFT | DT_TOP| DT_WORDBREAK);
	rc.top += height;
      }
    }
  }
  else
  {
    // Design mode
    Rectangle(di.hdcDraw, rc.left, rc.top, rc.right, rc.bottom) ;
    LPCTSTR str = _T( "MapLink ActiveX Sample Control" ) ;
    DrawText( di.hdcDraw, str, lstrlen(str), &rc, DT_CENTER | DT_VCENTER) ;
  }
  
  return S_OK;
}

LRESULT CActiveXCPPCtrl::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  // NB: We only get this if we're windowed
  LPCREATESTRUCT lpCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
  
  // Save the operating system type, used in OnDraw since the system region
  // query is different on each platform.
  getOSType();

  // clear errors from previous runs (e.g. if internet explorer page refreshed)
  TSLErrorStack::clear();

  // Initialise drawing surface, attaching to my window 
  m_drawingSurface = new TSLNTSurface( m_hWndCD, 0 );
  
  // Turn on double buffering
  m_drawingSurface->setOption( TSLOptionDoubleBuffered, true);
  
  // create our file loader and add it to the drawing surface so we can
  // read files from web servers
  m_remoteFileLoader = new TSLFileLoaderRemote;
  TSLDrawingSurface::addLoader(m_remoteFileLoader);
  

  // Initialise the drawing surface data files.
  if ( !TSLDrawingSurface::loadStandardConfig( m_serverConfigPath.c_str() ) )
  {
    string text("Unable to load config (.dat) files. serverMapPath is:" + m_serverMapPath +
      " serverConfigPath is: " + m_serverConfigPath );
    
    m_errors.push_back(text);
    
    const char * msg = TSLErrorStack::errorString( "Cannot setup drawing surface\n" ) ;
    if ( msg )
    {
      m_errors.push_back( msg );
    }
  }

  
  // initialise and load map
  // Create a map data layer
  m_mapDataLayer = new TSLMapDataLayer();

  // Add the remote loader to it as well, giving a callback function so we can redraw the screen
  // when data has asynchronously loaded
  m_mapDataLayer->addLoader( m_remoteFileLoader, staticLoaderCallback, m_hWndCD);

  // m_memoryCacheSize defaults to 32MB - which is the default for a mapdatalayer
  // if not specified at all
  m_mapDataLayer->cacheSize ( m_memoryCacheSize );

  // connect datat layer to drawing surface
  m_drawingSurface->addDataLayer(m_mapDataLayer, m_mapLayerName );

  // enable progressive loading, which shows a higher-level map in places where low-level detail
  // is not yet loaded
  m_drawingSurface->setDataLayerProps( m_mapLayerName, TSLPropertyProgressiveDisplay, 1 ) ;

  // concatenate path and map
  std::string mapPathAndName( m_serverMapPath + m_mapFileName);
  
  // load map
  if ( !m_mapDataLayer->loadData( mapPathAndName.c_str() ) )
  {
    string text("Unable to load map data from file: " + mapPathAndName );
    m_errors.push_back(text);
  }
  
  // set up the persistent cache
  if ( m_persistentCacheDirectory.length() && m_persistentCacheMaxSize > 0 )
  {
    TSLPersistentCacheData *settings = new TSLPersistentCacheData();

    settings->directory( m_persistentCacheDirectory.c_str());
    settings->maxSizeMB(m_persistentCacheMaxSize);

    m_mapDataLayer->configurePersistentCache(settings);
    settings->destroy();

    // clear the cache if the map data has changed
    checkForCacheClear();
  }

  
  // resize drawing surface to fit the control's window size  
  m_drawingSurface->wndResize(0,0, lpCreateStruct->cx, lpCreateStruct->cy, false, TSLResizeActionMaintainCentre );
  
  // redraw
  m_drawingSurface->reset(false);

  // initialise mouse handlers
  m_modeInterface = new TSLViewModeInterfaceNT( m_drawingSurface, m_hWndCD ) ;

  // report any errors
  const char * msg = TSLErrorStack::errorString( "Cannot initialise map\n" ) ;
  if ( msg )
  {
    m_errors.push_back( msg );
  }

  return 0;
}


void CActiveXCPPCtrl::checkForCacheClear()
{
  char oldDate[20] = "";
  char newDate[20] = "";

  TSLifstream instream;
  TSLLoaderStatus sts; 
  
  // Read the local date.dat
  string localFilename = m_persistentCacheDirectory + "\\date.dat";
  sts = m_remoteFileLoader->loadImmediate(0, localFilename.c_str(), &instream);
  if ( sts == TSLLoadingOK )
  {
    instream.getline( oldDate, 20);
  }

  // Read the remote date.dat
  TSLSimpleString remoteFilename ;
  if ( m_mapDataLayer->findFile( "date.dat", remoteFilename ) )
  {
    sts = m_remoteFileLoader->loadImmediate(0, remoteFilename, &instream);
    if ( sts == TSLLoadingOK )
    {
      instream.getline( newDate, 20);
    }
  }
    
  if ( strcmp (newDate, oldDate) != 0 )
  {
    // Dates are different, clear the cache and write the new date

    m_mapDataLayer->clearCache();  // clears both memory and persistent cache

    TSLofstream outstream( localFilename.c_str() );
    outstream.write ( newDate, strlen(newDate)+1 );
  }
}


LRESULT CActiveXCPPCtrl::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  // clean up our objects
  delete m_drawingSurface;
  m_mapDataLayer->destroy();
  m_remoteFileLoader->destroy();

  return 0;
}

// mouse interactions

LRESULT CActiveXCPPCtrl::OnLButtonDown(UINT uMsg, WPARAM nFlags, LPARAM pt, BOOL& bHandled)
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_currentMode )
  {
    if ( m_currentMode->OnLButtonDown( LOWORD(pt), HIWORD(pt),
		  		       nFlags & MK_SHIFT, nFlags & MK_CONTROL ) )
    {
      // Request a redraw if the interaction hander requires it
      Invalidate(FALSE) ;
    }
  }

  return 0;
}

LRESULT CActiveXCPPCtrl::OnLButtonUp(UINT uMsg, WPARAM nFlags, LPARAM pt, BOOL& bHandled)
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_currentMode )
  {
    if ( m_currentMode->OnLButtonUp( LOWORD(pt), HIWORD(pt),
		  		     nFlags & MK_SHIFT, nFlags & MK_CONTROL ) )
    {
      // Request a redraw if the interaction hander requires it
      Invalidate(FALSE) ;
    }
  }

  return 0;
}

LRESULT CActiveXCPPCtrl::OnMouseMove(UINT uMsg, WPARAM nFlags, LPARAM pt, BOOL& bHandled)
{
  if ( m_currentMode )
  {
    if ( m_currentMode->OnMouseMove( (short)LOWORD(pt), (short)HIWORD(pt),
		  		     nFlags & MK_SHIFT, nFlags & MK_CONTROL ) )
    {
      // Request a redraw if the interaction hander requires it
      Invalidate(FALSE) ;
    }
  }

  return 0;
}

LRESULT CActiveXCPPCtrl::OnRButtonDown(UINT uMsg, WPARAM nFlags, LPARAM pt, BOOL& bHandled)
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_currentMode )
  {
    if ( m_currentMode->OnRButtonDown( LOWORD(pt), HIWORD(pt),
		  		       nFlags & MK_SHIFT, nFlags & MK_CONTROL ) )
    {
      // Request a redraw if the interaction hander requires it
      Invalidate(FALSE) ;
    }
  }

  return 0;
}

LRESULT CActiveXCPPCtrl::OnRButtonUp(UINT uMsg, WPARAM nFlags, LPARAM pt, BOOL& bHandled)
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_currentMode )
  {
    if ( m_currentMode->OnRButtonUp( LOWORD(pt), HIWORD(pt),
		  		     nFlags & MK_SHIFT, nFlags & MK_CONTROL ) )
    {
      // Request a redraw if the interaction hander requires it
      Invalidate(FALSE) ;
    }
  }

  return 0;
}

LRESULT CActiveXCPPCtrl::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  m_drawingSurface->wndResize(0,0, LOWORD(lParam), HIWORD(lParam), false, TSLResizeActionMaintainCentre);

  return 0;
}

STDMETHODIMP CActiveXCPPCtrl::get_zoomPercentage(short *pVal)
{
  *pVal = m_zoomPercentage;
  
  return S_OK;
}

STDMETHODIMP CActiveXCPPCtrl::put_zoomPercentage(short newVal)
{
  m_zoomPercentage = newVal;
  
  return S_OK;
}

STDMETHODIMP CActiveXCPPCtrl::resetView(VARIANT_BOOL *rslt)
{
  *rslt = m_drawingSurface->reset(false);

  Invalidate(FALSE);

  return S_OK;
}

STDMETHODIMP CActiveXCPPCtrl::zoomIn(VARIANT_BOOL *rslt)
{
  *rslt = m_drawingSurface->zoom( m_zoomPercentage, true, false);

  Invalidate(FALSE);
  
  return S_OK;
}

STDMETHODIMP CActiveXCPPCtrl::zoomOut(VARIANT_BOOL *rslt)
{
  *rslt = m_drawingSurface->zoom( m_zoomPercentage, false, false);

  Invalidate(FALSE);
  
  return S_OK;
}

STDMETHODIMP CActiveXCPPCtrl::setView(double x, double y, double width, VARIANT_BOOL *rslt)
{
  double x1,y1,x2,y2;

  m_drawingSurface->getUUExtent(&x1, &y1, &x2, &y2);

  double requiredMapHeight = width * (m_rcPos.bottom - m_rcPos.top) / (m_rcPos.right - m_rcPos.left);

  m_drawingSurface->MUToUU(x - width/2, y - requiredMapHeight/2, &x1, &y1);
  m_drawingSurface->MUToUU(x + width/2, y + requiredMapHeight/2, &x2, &y2);
  *rslt = m_drawingSurface->resize( x1, y1, x2, y2, false, true);

  Invalidate(FALSE);
  
  return S_OK;
}

STDMETHODIMP CActiveXCPPCtrl::get_memoryCacheSize(long *pVal)
{
  *pVal = m_memoryCacheSize;
  return S_OK;
}

STDMETHODIMP CActiveXCPPCtrl::put_memoryCacheSize(long newVal)
{
  m_memoryCacheSize = newVal;

  // This property can be set well before the control has had 
  // a chance to initialise itself properly, so check if the 
  // the mapDataLayer is actually there. When constructed, it 
  // will use the value of m_memoryCacheSize to initialise.
  if( m_mapDataLayer)
    m_mapDataLayer->cacheSize( newVal ) ;
  
  return S_OK;
}

void CActiveXCPPCtrl::getOSType()
{
  OSVERSIONINFO osvi;
  osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  GetVersionEx(&osvi);

  m_bIsWindows9X = ( osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS);
}

STDMETHODIMP CActiveXCPPCtrl::get_drawingSurfacePtr(long *pVal)
{
  *pVal = reinterpret_cast<long>(m_drawingSurface);
  
  return S_OK;
}

STDMETHODIMP CActiveXCPPCtrl::get_mapLayerName(BSTR *pVal)
{
  *pVal = _bstr_t( m_mapLayerName ).copy();
  
  return S_OK;
}

LRESULT CActiveXCPPCtrl::OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  // if in a mode, the mode will have set the cursor in it's constructor  
  if ( m_currentMode )
  {
    long mouseMessage = HIWORD( lParam ) ;
    bool down = (   mouseMessage == WM_LBUTTONDOWN || mouseMessage == WM_RBUTTONDOWN 
                 || mouseMessage == WM_MBUTTONDOWN ) ;

    m_currentMode->OnSetCursor( down ) ;
  }
  else
  {
    SetCursor( LoadCursor( NULL, IDC_ARROW ) ) ;
  }

  return 0 ;
}

STDMETHODIMP CActiveXCPPCtrl::get_persistentCacheDirectory(BSTR *pVal)
{
  *pVal = _bstr_t(m_persistentCacheDirectory.c_str()).copy();
  
  return S_OK;
}

STDMETHODIMP CActiveXCPPCtrl::put_persistentCacheDirectory(BSTR newVal)
{
  m_persistentCacheDirectory = _bstr_t(newVal,true);
  
  return S_OK;
}

STDMETHODIMP CActiveXCPPCtrl::get_persistentCacheMaxSize(long *pVal)
{
  *pVal = m_persistentCacheMaxSize;
  
  return S_OK;
}

STDMETHODIMP CActiveXCPPCtrl::put_persistentCacheMaxSize(long newVal)
{
  m_persistentCacheMaxSize = newVal;
  
  return S_OK;
}


LRESULT CActiveXCPPCtrl::OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  // do nothing, so the background is not erased, to prevent flicker 
  // when the window is resized
  return 0;
}


STDMETHODIMP CActiveXCPPCtrl::print()
{
  // Ask the user which printer they want to use
  PRINTDLG printinfo;
  memset( &printinfo, 0, sizeof(printinfo) );
  printinfo.lStructSize = sizeof(printinfo);
  printinfo.hwndOwner = m_hWnd;
  printinfo.Flags = PD_RETURNDC | PD_NOPAGENUMS | PD_NOSELECTION;

  if ( !PrintDlg(&printinfo) )
    return S_OK;

  HDC printerDC = printinfo.hDC;

  // get height and width so we can tell if landscape or portrait
  long width = GetDeviceCaps( printerDC, HORZRES );
  long height = GetDeviceCaps( printerDC, VERTRES );

 
  TSLEntitySet * tmp = 0 ;
  TSLSimpleString templateFilePath;

  char* templateFile;
  
  // Choose a default portrait or landscape template.
  // You can create your own using PrintTemplateStudio;
  if ( width > height )
  {
    templateFile = "landscape.tpt";
  }
  else
  {
    templateFile = "portrait.tpt";
  }

  // search for the file in the drawing surface's pathlists
  if ( TSLDrawingSurface::findFile( templateFile, templateFilePath ) )
  {
    // load the template
    tmp = TSLPrintHelper::loadTemplate( templateFilePath, TSLDrawingSurface::getLoader(), 0 ) ;
  }

  if ( !tmp || tmp->size() == 0 )
  {
    m_errors.push_back( "Cannot find print template" ) ;
    Invalidate();
    return S_OK;
  }

  // Get the TMC extent of the current view of the drawingSurface ;
  TSLEnvelope extent ;
  m_drawingSurface->getTMCExtent( extent ) ;

  // Set up a print item with a 3 pixel wide border, displaying the current extent.
  // All visible data layers in the drawing surface are used when printing.
  // To display at a specific scale, use item.centreAndScale rather than item.extent
  // There can be as many items as we need 
  TSLPrintItem item( m_drawingSurface ) ; // Use the default print template item name 'map'
  item.setBorder( 1, 1,3 ) ;
  item.extent( extent ) ;

  // Add a mapping for the 'date' text object in the template to the current date
  // In the template, if there is a text string 'date', this is changed to the
  // current date and time when printed.
  const char * mappings[ ][ 2 ] = { "date", 0 } ;

  time_t ltime;
  time( &ltime );

  std::string s = ctime( &ltime ) ;

  // ctime always adds a carriage return on the end, so take it off
  size_t last = s.find_last_of( '\n' ) ;
  if ( last != std::string::npos )
    s[ last ] = '\0' ;
  
  mappings[0][1] = s.c_str() ;

  // Now print the items.

  TSLWin32PrintContext context( printerDC );

  DOCINFO info;
  memset( &info, 0, sizeof(info) );
  info.cbSize = sizeof(DOCINFO);
  info.lpszDocName = _T("MapLink ActiveXCPP Sample");

  StartDoc( printerDC, &info );

  // ensure that the maps are fully loaded before printing. 
  m_mapDataLayer->setSynchronousLoading(true);

  // If we wanted to print more items, for example enlargements, then we could
  // either create a custom template with fixed position items - each with a 
  // different name, or use the TSLPrintHelper::addItemTemplate method to add
  // a dynamically created item.  This allows for the same template to be used
  TSLPrintHelper::printUsingTemplate( context, 1, &item, tmp, 1, mappings, true, false ) ;

  m_mapDataLayer->setSynchronousLoading(false);

  // Tidy up the template since we no longer need it.
  tmp->destroy() ;

  EndDoc( printerDC );
  
  DeleteDC( printerDC );
  
  return S_OK;
}
