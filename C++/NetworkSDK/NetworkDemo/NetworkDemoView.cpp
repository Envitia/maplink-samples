// NetworkDemoView.cpp : implementation of the CNetworkDemoView class
//

#include "stdafx.h"
#include "NetworkDemo.h"

#include "NetworkDemoDoc.h"
#include "NetworkDemoView.h"

#include "tslviewmodeinterfacent.h"
#include "tslviewmodepan.h"
#include "tslviewmodezoom.h"
#include "tslviewmodegrab.h"
#include "tslviewmodezoompan.h"
#include "tslviewmoderoute.h"
#include "tslviewmodespatialquery.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CNetworkDemoView

IMPLEMENT_DYNCREATE(CNetworkDemoView, CView)

BEGIN_MESSAGE_MAP(CNetworkDemoView, CView)
//{{AFX_MSG_MAP(CNetworkDemoView)
  ON_COMMAND(ID_TOOLS_ZOOMIN, OnToolsZoomin)
  ON_COMMAND(ID_TOOLS_ZOOMOUT, OnToolsZoomout)
  ON_COMMAND(ID_TOOLS_RESET, OnToolsReset)
  ON_WM_SIZE()
  ON_WM_MOUSEMOVE()
  ON_WM_LBUTTONDOWN()
  ON_WM_RBUTTONDOWN()
  ON_WM_LBUTTONUP()
  ON_WM_RBUTTONUP()
  ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOM,    OnUpdateToolsMode)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_RESET,   OnUpdateToolsAction)
  ON_COMMAND(ID_TOOLS_PAN, OnToolsPan)
  ON_COMMAND(ID_TOOLS_ZOOM, OnToolsZoom)
  ON_COMMAND(ID_TOOLS_ZOOMPAN, OnToolsZoompan)
  ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
  ON_WM_LBUTTONDBLCLK()
  ON_WM_RBUTTONDBLCLK()
  ON_WM_ERASEBKGND()
  ON_COMMAND(ID_TOOLS_GRAB, OnToolsGrab)
  ON_COMMAND(ID_TOOLS_ROUTE, OnToolsRoute)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_ROUTE, OnUpdateToolsRoute)
  ON_COMMAND(ID_TOOLS_SQ, OnToolsSpatialQuery)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_SQ, OnUpdateToolsSpatialQuery)
  ON_COMMAND(ID_TOOLS_USESPATIALINDEX, OnToolsUseSpatialIndex)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_USESPATIALINDEX, OnUpdateToolsUseSpatialIndex)
  ON_COMMAND(ID_ADDSTARTROUTEPIN, OnAddStartRoutePin)
  ON_UPDATE_COMMAND_UI(ID_ADDSTARTROUTEPIN, OnUpdateAddStartRoutePin)
  ON_COMMAND(ID_ADDENDROUTEPIN, OnAddEndRoutePin)
  ON_UPDATE_COMMAND_UI(ID_ADDENDROUTEPIN, OnUpdateAddEndRoutePin)
  ON_COMMAND(ID_ADDWAYPTROUTEPIN, OnAddWayptRoutePin)
  ON_UPDATE_COMMAND_UI(ID_ADDWAYPTROUTEPIN, OnUpdateAddWayptRoutePin)
  ON_COMMAND(ID_CALCULATE_ROUTE, OnCalculateRoute)
  ON_UPDATE_COMMAND_UI(ID_CALCULATE_ROUTE, OnUpdateCalculateRoute)
  ON_COMMAND(ID_CLEAR_ROUTE_DISPLAY, OnClearRouteDisplay)
  ON_UPDATE_COMMAND_UI(ID_CLEAR_ROUTE_DISPLAY, OnUpdateClearRouteDisplay)
  ON_COMMAND(ID_CLEAR_ROUTES, OnClearRoutes)
  ON_UPDATE_COMMAND_UI(ID_CLEAR_ROUTES, OnUpdateClearRoutes)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOMIN,  OnUpdateToolsAction)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOMPAN, OnUpdateToolsMode)
	ON_COMMAND(ID_QUICKEST, OnQuickest)
	ON_UPDATE_COMMAND_UI(ID_QUICKEST, OnUpdateQuickest)
	ON_COMMAND(ID_SHORTEST, OnShortest)
	ON_UPDATE_COMMAND_UI(ID_SHORTEST, OnUpdateShortest)
	//}}AFX_MSG_MAP

// Our common handles can update multiple items.
ON_UPDATE_COMMAND_UI(ID_EDIT_COPY,     OnUpdateToolsAction)
ON_UPDATE_COMMAND_UI(ID_FILE_PRINT,    OnUpdateToolsAction)
ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOMOUT, OnUpdateToolsAction)
ON_UPDATE_COMMAND_UI(ID_TOOLS_PAN,     OnUpdateToolsMode)
ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOM,    OnUpdateToolsMode)
ON_UPDATE_COMMAND_UI(ID_TOOLS_GRAB,    OnUpdateToolsMode)

// Add the middle button handlers since DevStudio doesn't 
// give us the option.
ON_WM_MBUTTONDOWN()
ON_WM_MBUTTONDBLCLK()
ON_WM_MBUTTONUP()

// Standard printing commands
ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)

END_MESSAGE_MAP()

extern CNetworkDemoApp theApp ;

/////////////////////////////////////////////////////////////////////////////
// CNetworkDemoView construction/destruction

CNetworkDemoView::CNetworkDemoView()
{
  // Clear my internal variables
  m_drawingSurface = 0 ; 
  m_currentMode    = 0 ;
  m_modeInterface  = 0 ;
  m_mapLoaded = false ;

  m_routeMode = 0;
  m_spatialQueryMode = 0;
  m_useSpatialIndex = false;
}

CNetworkDemoView::~CNetworkDemoView()
{
  if ( ( m_currentMode != m_routeMode ) && ( m_currentMode != m_spatialQueryMode ) )
  {
    delete m_currentMode ;
  }
  m_currentMode = 0;

  if ( m_routeMode )
  {
    delete m_routeMode ;
    m_routeMode = 0 ;
  }

  if ( m_spatialQueryMode )
  {
    delete m_spatialQueryMode;
    m_spatialQueryMode = 0;
  }

  if ( m_modeInterface )
  {
    delete m_modeInterface ;
    m_modeInterface = 0 ;
  }
  if ( m_drawingSurface )
  {
    delete m_drawingSurface ;
    m_drawingSurface = 0 ;
    TSLDrawingSurface::cleanup() ;
  }
}

/////////////////////////////////////////////////////////////////////////////
// CNetworkDemoView drawing

void CNetworkDemoView::OnDraw(CDC* pDC)
{
  ASSERT( m_drawingSurface ) ;
  
  if ( m_drawingSurface )
  {
    RECT rect ;
    
    // Only draw the exposed bits of the display
    if ( pDC->GetClipBox( &rect ) == NULLREGION )
      GetClientRect( &rect ) ;
    
    m_drawingSurface->drawDU( rect.left, rect.bottom, rect.right, rect.top, true ) ;
    
    // Don't forget to draw any echo rectangle that may be active.
    m_modeInterface->drawEchoRectangle( ) ;
  }
}

/////////////////////////////////////////////////////////////////////////////
// CNetworkDemoView printing
BOOL CNetworkDemoView::OnPreparePrinting(CPrintInfo* pInfo)
{
  // default preparation
  pInfo->SetMaxPage(1);
  return DoPreparePrinting(pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CNetworkDemoView diagnostics

#ifdef _DEBUG
void CNetworkDemoView::AssertValid() const
{
  CView::AssertValid();
}

void CNetworkDemoView::Dump(CDumpContext& dc) const
{
  CView::Dump(dc);
}

CNetworkDemoDoc* CNetworkDemoView::GetDocument() // non-debug version is inline
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNetworkDemoDoc)));
  return (CNetworkDemoDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNetworkDemoView message handlers

void CNetworkDemoView::OnInitialUpdate() 
{
  CView::OnInitialUpdate();
  
  CNetworkDemoDoc * doc = GetDocument() ;
  
  // In an SDI application, this method is called every time a document is
  // loaded.  The first time ever, there will be no drawing surface.
  // If we have no drawing surface, create it, otherwise remove the old layer.
  if ( !m_drawingSurface ) 
  {
    CRect rect ;
    
    GetClientRect( &rect ) ;
    
    // Create a double buffered drawing surface, and an interaction interface
    // to control it.
    // Set up the initial window extents.
    m_drawingSurface = new TSLNTSurface( m_hWnd, false ) ;
    m_drawingSurface->setOption( TSLOptionDoubleBuffered, true ) ;
    m_drawingSurface->wndResize( 0, 0, rect.Width(), rect.Height() ) ;
    m_modeInterface = new TSLViewModeInterfaceNT( m_drawingSurface, m_hWnd ) ;
  }
  else if ( m_mapLoaded ) 
  {
    // We have a drawing surface with a map loaded into it.
    // Detach the map from the surface
    if ( m_drawingSurface && doc )
      doc->detachFromSurface( m_drawingSurface ) ;
  }
  
  // If the document loaded successfully, add it to the drawing surface
  m_mapLoaded = doc->addToSurface( m_drawingSurface ) ;
  
  // and reset the current view to display the entire map.
  m_drawingSurface->reset( false ) ;
    
  // Reset the current interaction mode by deleting it.
  if ( m_currentMode && (m_currentMode != m_routeMode) && (m_currentMode != m_spatialQueryMode) )
  {
    delete m_currentMode ;
  }
  m_currentMode = 0 ;

  if ( m_modeInterface )
  {
    if ( m_routeMode )
    {
      delete m_routeMode;
      m_routeMode = 0;
    }
    if ( m_spatialQueryMode )
    {
      delete m_spatialQueryMode;
      m_spatialQueryMode = 0;
    }

    // Now initialise the route mode.
    m_routeMode = new TSLViewModeRoute( m_modeInterface, ID_TOOLS_ROUTE, doc );

    // Now initialise the Spatial Query mode.
    m_spatialQueryMode = new TSLViewModeSpatialQuery( m_modeInterface, ID_TOOLS_SQ, doc );
  }
}

void CNetworkDemoView::OnToolsZoomin() 
{
  if ( m_drawingSurface )
  {
    // Zoom in by 30%, and request a window redraw
    m_drawingSurface->zoom( 30, true, false ) ;
    InvalidateRect( 0, false ) ;
  }
}

void CNetworkDemoView::OnToolsZoomout() 
{
  if ( m_drawingSurface )
  {
    // Zoom out by 30%, and request a window redraw
    m_drawingSurface->zoom( 30, false, false ) ;
    InvalidateRect( 0, false ) ;
  }
}

void CNetworkDemoView::OnToolsReset() 
{
  if ( m_drawingSurface )
  {
    // Reset the window to display the entire map, and request a window redraw.
    m_drawingSurface->reset( false ) ;
    InvalidateRect( 0, false ) ;
  }
}

void CNetworkDemoView::OnSize(UINT nType, int cx, int cy) 
{
  CView::OnSize(nType, cx, cy);
  
  if ( m_drawingSurface )
  {
    // Inform the drawing surface of the new window size,
    // attempting to keep the top left corner the same.
    // Do not ask for an automatic redraw since we will get an OnDraw call
    m_drawingSurface->wndResize( 0, 0, cx, cy, false, 
				 TSLResizeActionMaintainTopLeft ) ;
  }
}

void CNetworkDemoView::OnMouseMove(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_currentMode )
  {
    if ( m_currentMode->OnMouseMove( point.x, point.y,
				     nFlags & MK_SHIFT, nFlags & MK_CONTROL ) )
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( 0, false ) ;
    }
  }
  
  CView::OnMouseMove(nFlags, point);
}

void CNetworkDemoView::OnLButtonDown(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_currentMode )
  {
    if ( m_currentMode->OnLButtonDown( point.x, point.y,
				       nFlags & MK_SHIFT, nFlags & MK_CONTROL ) )
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( 0, false ) ;
    }
  }
  
  CView::OnLButtonDown(nFlags, point);
}

void CNetworkDemoView::OnMButtonDown(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_currentMode )
  {
    if ( m_currentMode->OnMButtonDown( point.x, point.y,
				       nFlags & MK_SHIFT, nFlags & MK_CONTROL ) )
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( 0, false ) ;
    }
  }
  
  CView::OnMButtonDown(nFlags, point);
}

void CNetworkDemoView::OnRButtonDown(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_currentMode )
  {
    if ( m_currentMode->OnRButtonDown( point.x, point.y,
				       nFlags & MK_SHIFT, nFlags & MK_CONTROL ) )
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( 0, false ) ;
    }
  }
  
  CView::OnRButtonDown(nFlags, point);
}

void CNetworkDemoView::OnLButtonUp(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_currentMode )
  {
    if ( m_currentMode->OnLButtonUp( point.x, point.y,
			             nFlags & MK_SHIFT, nFlags & MK_CONTROL ) )
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( 0, false ) ;
    }
  }
  
  CView::OnLButtonUp(nFlags, point);
}

void CNetworkDemoView::OnMButtonUp(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_currentMode )
  {
    if ( m_currentMode->OnMButtonUp( point.x, point.y,
				     nFlags & MK_SHIFT, nFlags & MK_CONTROL ) )
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( 0, false ) ;
    }
  }
  
  
  CView::OnMButtonUp(nFlags, point);
}

void CNetworkDemoView::OnRButtonUp(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_currentMode )
  {
    if ( m_currentMode->OnRButtonUp( point.x, point.y,
				     nFlags & MK_SHIFT, nFlags & MK_CONTROL ) )
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( 0, false ) ;
    }
  }
  
  
  CView::OnRButtonUp(nFlags, point);
}

void CNetworkDemoView::OnToolsPan() 
{
  // If pan mode is currently active, then just deactivate it,
  // otherwise deactivate any current mode and activate pan mode
  bool active = m_currentMode && m_currentMode->id() == ID_TOOLS_PAN ;
  
  if ( m_currentMode && m_currentMode != m_routeMode && m_currentMode != m_spatialQueryMode )
  {
    delete m_currentMode ;
  }

  m_currentMode = 0 ;
  
  // Pass the menu item ID to the mode so that the update handler works
  if ( !active )
    m_currentMode = new TSLViewModePan( m_modeInterface, ID_TOOLS_PAN ) ;
}

void CNetworkDemoView::OnToolsZoom() 
{
  // If zoom mode is currently active, then just deactivate it,
  // otherwise deactivate any current mode and activate zoom mode
  bool active = m_currentMode && m_currentMode->id() == ID_TOOLS_ZOOM ;
  
  if ( m_currentMode && m_currentMode != m_routeMode && m_currentMode != m_spatialQueryMode )
  {
    delete m_currentMode ;
  }

  m_currentMode = 0 ;
  
  // Pass the menu item ID to the mode so that the update handler works
  if ( !active )
    m_currentMode = new TSLViewModeZoom( m_modeInterface, ID_TOOLS_ZOOM ) ;
}

void CNetworkDemoView::OnToolsZoompan() 
{
  // If zoom/pan mode is currently active, then just deactivate it,
  // otherwise deactivate any current mode and activate zoom/pan mode
  bool active = m_currentMode && m_currentMode->id() == ID_TOOLS_ZOOMPAN ;
  
  if ( m_currentMode && m_currentMode != m_routeMode && m_currentMode != m_spatialQueryMode )
  {
    delete m_currentMode ;
  }

  m_currentMode = 0 ;
  
  // Pass the menu item ID to the mode so that the update handler works
  if ( !active )
    m_currentMode = new TSLViewModeZoomPan( m_modeInterface, ID_TOOLS_ZOOMPAN ) ;
}

void CNetworkDemoView::OnUpdateToolsAction(CCmdUI* pCmdUI) 
{
  // Common update handler to all items that are dependant upon the map
  // being loaded.
  pCmdUI->Enable( m_mapLoaded ? true : false ) ;
}

void CNetworkDemoView::OnUpdateToolsMode(CCmdUI* pCmdUI) 
{
  // Common update handler to all items that are dependant upon the map
  // being loaded, and act as radios dependant upon the current mode.
  pCmdUI->Enable( m_mapLoaded ? true : false  ) ;
  pCmdUI->SetCheck( m_currentMode && m_currentMode->id() == pCmdUI->m_nID ) ;
}

void CNetworkDemoView::OnEditCopy() 
{
  // Snap shot the current view and place it on the clipboard
  CDC * pDC = GetDC() ;
  
  // Create a bitmap
  RECT rc ;
  GetClientRect ( &rc ) ;
  
  HBITMAP newBitmap = CreateCompatibleBitmap( pDC->m_hDC, rc.right, rc.bottom );
  HDC	  newDC     = CreateCompatibleDC( pDC->m_hDC );
  bool    opened = false ;
  bool    closed = false ;
  
  if ( newBitmap && newDC )
  {
    // Select the bitmap into the DC
    SelectObject( newDC, newBitmap );
    
    // And bit blit the screen to the new bitmap and put it into the clipboard
    if (   !BitBlt( newDC, 0, 0, rc.right, rc.bottom, pDC->m_hDC, 0, 0, SRCCOPY ) 
	|| !(opened = OpenClipboard( ) )
	|| !EmptyClipboard( )
	|| !SetClipboardData( CF_BITMAP, newBitmap ) 
	|| !(closed = CloseClipboard() ) )
    {
      AfxMessageBox( _T("Cannot access clipboard"), MB_ICONERROR );
    }
  }
  else
  {
    AfxMessageBox( _T("Cannot copy map view"), MB_ICONERROR );
  }
  
  if ( opened && !closed )
    CloseClipboard() ;
  
  if ( newBitmap )
    DeleteObject( newBitmap );
  
  if ( newDC )
    DeleteObject( newDC );
}

void CNetworkDemoView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
  // Just handle like a normal press to avoid losing the second press
  OnLButtonDown( nFlags, point ) ;
  CView::OnLButtonDblClk(nFlags, point);
}

void CNetworkDemoView::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
  // Just handle like a normal press to avoid losing the second press
  OnRButtonDown( nFlags, point ) ;
  CView::OnRButtonDblClk(nFlags, point);
}
void CNetworkDemoView::OnMButtonDblClk(UINT nFlags, CPoint point) 
{
  // Just handle like a normal press to avoid losing the second press
  OnMButtonDown( nFlags, point ) ;
  CView::OnMButtonDblClk(nFlags, point);
}

void CNetworkDemoView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
  // Attempt to print using the PrintHelper, using the standard templates.
  if ( m_drawingSurface ) 
  {
    TSLEntitySet * tmp = 0 ;

    char* templateFile;
    
    // Choose a default portrait or landscape template.
    // You can create your own using PrintTemplateStudio;
    if ( pInfo->m_rectDraw.Width() > pInfo->m_rectDraw.Height() )
    {
      templateFile = "landscape.tpt";
    }
    else
    {
      templateFile = "portrait.tpt";
    }

    // search for the file in the drawing surface's pathlists
    TSLSimpleString templateFilePath;
    if ( TSLDrawingSurface::findFile( templateFile, templateFilePath ) )
    {
      // load the template
      tmp = TSLPrintHelper::loadTemplate( templateFilePath.c_str(), TSLDrawingSurface::getLoader(), 0 ) ;
    }

    if ( !tmp || tmp->size() == 0 )
    {
      MessageBox( _T("Cannot find print template") ) ;
      return ;
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

    // Create an MFC specific print context.  This contains information
    // such as the printable area.  It is necessary to use and MFC specific one
    // to cope with the MFC Print Preview facility which tweaks the screen
    // to look like the printer.
    TSLMFCPrintContext context( pDC, pInfo ) ;

    // Inform document that printing is about to begin.
    GetDocument()->prepareForPrinting();

    // Now print the items.

    // If we wanted to print more items, for example enlargements, then we could
    // either create a custom template with fixed position items - each with a 
    // different name, or use the TSLPrintHelper::addItemTemplate method to add
    // a dynamically created item.  This allows for the same template to be used
    TSLPrintHelper::printUsingTemplate( context, 1, &item, tmp, 1, mappings ) ;

    // Inform document that printing has finished.
    GetDocument()->finishedPrinting();

    // Tidy up the template since we no longer need it.
    tmp->destroy() ;
    
    pInfo->m_bContinuePrinting = false ;
  }
}

BOOL CNetworkDemoView::OnEraseBkgnd(CDC* pDC) 
{
  // Since the drawing surface is double buffered, we don't need to erase the
  // background.
  return true;
}

void CNetworkDemoView::OnToolsGrab() 
{
  // If grab mode is currently active, then just deactivate it,
  // otherwise deactivate any current mode and activate pan mode
  bool active = m_currentMode && m_currentMode->id() == ID_TOOLS_GRAB ;
  
  if ( m_currentMode && m_currentMode != m_routeMode && m_currentMode != m_spatialQueryMode )
  {
    delete m_currentMode ;
  }

  m_currentMode = 0 ;
  
  // Pass the menu item ID to the mode so that the update handler works
  if ( !active )
    m_currentMode = new TSLViewModeGrab( m_modeInterface, ID_TOOLS_GRAB ) ;
}

void CNetworkDemoView::OnToolsRoute() 
{
  // If route mode is currently active, then just deactivate it,
  // otherwise deactivate any current mode and activate route mode
  bool active = m_currentMode && m_currentMode->id() == ID_TOOLS_ROUTE;
  
  if ( m_currentMode && m_currentMode != m_routeMode && m_currentMode != m_spatialQueryMode )
  {
    delete m_currentMode ;
  }

  m_currentMode = 0 ;
  
  // Pass the menu item ID to the mode so that the update handler works
  if ( !active )
    m_currentMode = m_routeMode;
}

void CNetworkDemoView::OnUpdateToolsRoute(CCmdUI* pCmdUI) 
{
  // Update handler for routing;  dependant upon the network being loaded.
  CNetworkDemoDoc* doc = GetDocument();

  pCmdUI->Enable( doc->isNetworkLoaded() ? true : false );
  pCmdUI->SetCheck( m_currentMode && m_currentMode->id() == pCmdUI->m_nID );
}

void CNetworkDemoView::OnToolsSpatialQuery() 
{
  // If Spatial Query mode is currently active, then just deactivate it,
  // otherwise deactivate any current mode and activate route mode
  bool active = m_currentMode && m_currentMode->id() == ID_TOOLS_SQ;
  
  if ( !active && m_currentMode != m_routeMode )
  {
    delete m_currentMode ;
  }

  m_currentMode = m_spatialQueryMode;
  
  m_useSpatialIndex = false;
  m_spatialQueryMode->useSpatialIndex( m_useSpatialIndex );
}

void CNetworkDemoView::OnUpdateToolsSpatialQuery(CCmdUI* pCmdUI) 
{
  // Update handler for Spatial Queries,  dependant upon the network being loaded.
  CNetworkDemoDoc* doc = GetDocument();

  pCmdUI->Enable( doc->isNetworkLoaded() ? true : false );
  pCmdUI->SetCheck( m_currentMode && m_currentMode->id() == pCmdUI->m_nID && !m_useSpatialIndex );
}

void CNetworkDemoView::OnToolsUseSpatialIndex() 
{
 // If Spatial Query mode is currently active, then just deactivate it,
  // otherwise deactivate any current mode and activate route mode
  bool active = m_currentMode && m_currentMode->id() == ID_TOOLS_SQ;
  
  if ( !active && m_currentMode != m_routeMode )
  {
    delete m_currentMode ;
  }

  m_currentMode = m_spatialQueryMode;
  
  m_useSpatialIndex = true;
  m_spatialQueryMode->useSpatialIndex( m_useSpatialIndex );
}

void CNetworkDemoView::OnUpdateToolsUseSpatialIndex(CCmdUI* pCmdUI) 
{
  CNetworkDemoDoc* doc = GetDocument();
  pCmdUI->Enable( doc->isNetworkLoaded() ? true : false );
  pCmdUI->SetCheck( m_currentMode && m_currentMode->id() == ID_TOOLS_SQ && m_useSpatialIndex);
}

void CNetworkDemoView::OnAddStartRoutePin() 
{
  m_routeMode->addStartPin();
}

void CNetworkDemoView::OnUpdateAddStartRoutePin(CCmdUI* pCmdUI) 
{
  CNetworkDemoDoc* doc = GetDocument();

  bool enabled = doc->isNetworkLoaded() && m_currentMode == m_routeMode ? true : false;

  if ( enabled )
  {
    switch ( m_routeMode->getCurrentState() )
    {
    case TSLViewModeRoute::TSLRoute_Default:
      pCmdUI->Enable( 1 );
      pCmdUI->SetCheck( 0 );
      break;
      
    case TSLViewModeRoute::TSLRoute_AddingStartPin:
      pCmdUI->Enable( 1 );
      pCmdUI->SetCheck( 1 );
      break;

    default:
      pCmdUI->SetCheck( 0 );
      pCmdUI->Enable( 0 );
      break;
    }
  }
  else
  {
    pCmdUI->Enable( 0 );
  }
}

void CNetworkDemoView::OnAddEndRoutePin() 
{
  m_routeMode->addEndPin();
}

void CNetworkDemoView::OnUpdateAddEndRoutePin(CCmdUI* pCmdUI) 
{
  CNetworkDemoDoc* doc = GetDocument();

  bool enabled = doc->isNetworkLoaded() && m_currentMode == m_routeMode ? true : false;

  if ( enabled )
  {
    switch ( m_routeMode->getCurrentState() )
    {
    case TSLViewModeRoute::TSLRoute_Default:
    case TSLViewModeRoute::TSLRoute_AddingStartPin:
      pCmdUI->Enable( 0 );
      pCmdUI->SetCheck( 0 );
      break;
      
    case TSLViewModeRoute::TSLRoute_StartPinAdded:
      pCmdUI->Enable( 1 );
      pCmdUI->SetCheck( 0 );
      break;
      
    case TSLViewModeRoute::TSLRoute_AddingEndPin:
      pCmdUI->Enable( 1 );
      pCmdUI->SetCheck( 1 );
      break;

    default:
      pCmdUI->SetCheck( 0 );
      pCmdUI->Enable( GetDocument()->endPinAdded() ? 0 : 1 );
      break;
    }
  }
  else
  {
    pCmdUI->Enable( 0 );
  }
}

void CNetworkDemoView::OnAddWayptRoutePin() 
{
  m_routeMode->addWayptPin();
}

void CNetworkDemoView::OnUpdateAddWayptRoutePin(CCmdUI* pCmdUI) 
{
  CNetworkDemoDoc* doc = GetDocument();

  bool enabled = doc->isNetworkLoaded() && m_currentMode == m_routeMode ? true : false;

  if ( enabled )
  {
    switch ( m_routeMode->getCurrentState() )
    {
    case TSLViewModeRoute::TSLRoute_Default:
    case TSLViewModeRoute::TSLRoute_AddingStartPin:
      pCmdUI->Enable( 0 );
      pCmdUI->SetCheck( 0 );
      break;
      
    case TSLViewModeRoute::TSLRoute_StartPinAdded:
    case TSLViewModeRoute::TSLRoute_WayptPinAdded:
    case TSLViewModeRoute::TSLRoute_AddingEndPin:
    case TSLViewModeRoute::TSLRoute_AbleToRoute:
      pCmdUI->Enable( 1 );
      pCmdUI->SetCheck( 0 );
      break;
      
    case TSLViewModeRoute::TSLRoute_AddingWayptPin:
      pCmdUI->Enable( 1 );
      pCmdUI->SetCheck( 1 );
      break;

    default:
      pCmdUI->SetCheck( 0 );
      pCmdUI->Enable( 0 );
      break;
    }
  }
  else
  {
    pCmdUI->Enable( 0 );
  }
}

void CNetworkDemoView::OnCalculateRoute()
{
  GetDocument()->calculateRoute();

  InvalidateRect( 0 );
}

void CNetworkDemoView::OnUpdateCalculateRoute(CCmdUI* pCmdUI)
{
  CNetworkDemoDoc* doc = GetDocument();

  bool enabled = doc->isNetworkLoaded() && m_currentMode == m_routeMode ? true : false;

  if ( enabled )
  {
    pCmdUI->Enable( GetDocument()->endPinAdded() ? 1 : 0 );
  }
  else
  {
    pCmdUI->Enable( 0 );
  }
}

void CNetworkDemoView::OnClearRouteDisplay()
{
  GetDocument()->clearRouteDisplay();

  InvalidateRect( 0 );
}

void CNetworkDemoView::OnUpdateClearRouteDisplay(CCmdUI* pCmdUI)
{
  CNetworkDemoDoc* doc = GetDocument();

  bool enabled = doc->isNetworkLoaded() && m_currentMode == m_routeMode ? true : false;

  if ( enabled )
  {
    pCmdUI->Enable( GetDocument()->isRouteDataLayerEmpty() ? 0 : 1 );
  }
  else
  {
    pCmdUI->Enable( 0 );
  }
}

void CNetworkDemoView::OnClearRoutes()
{
  GetDocument()->clearRoutes();

  m_routeMode->reset();

  InvalidateRect( 0 );
}

void CNetworkDemoView::OnUpdateClearRoutes(CCmdUI* pCmdUI)
{
  CNetworkDemoDoc* doc = GetDocument();

  bool enabled = doc->isNetworkLoaded() && m_currentMode == m_routeMode ? true : false;

  if ( enabled )
  {
    pCmdUI->Enable( m_routeMode->getCurrentState() >= TSLViewModeRoute::TSLRoute_StartPinAdded ? 1 : 0 );
  }
  else
  {
    pCmdUI->Enable( 0 );
  }
}

void CNetworkDemoView::OnQuickest() 
{
  GetDocument()->routingType( QuickestRoute );
}

void CNetworkDemoView::OnUpdateQuickest(CCmdUI* pCmdUI) 
{
  CNetworkDemoDoc* doc = GetDocument();

  bool enabled = doc->isNetworkLoaded() && m_currentMode == m_routeMode ? true : false;

  pCmdUI->Enable( enabled ? 1 : 0 );
  pCmdUI->SetCheck( doc->routingType() == QuickestRoute ? 1 : 0 );
}

void CNetworkDemoView::OnShortest() 
{
  GetDocument()->routingType( ShortestRoute );
}

void CNetworkDemoView::OnUpdateShortest(CCmdUI* pCmdUI) 
{
  CNetworkDemoDoc* doc = GetDocument();

  bool enabled = doc->isNetworkLoaded() && m_currentMode == m_routeMode ? true : false;

  pCmdUI->Enable( enabled ? 1 : 0 );
  pCmdUI->SetCheck( doc->routingType() == ShortestRoute ? 1 : 0 );
}
