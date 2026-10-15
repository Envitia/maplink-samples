// TerrainViewerView.cpp : implementation of the CTerrainViewerView class
//

#include "stdafx.h"
#include "TerrainViewer.h"
#include "TerrainViewerDoc.h"
#include "TerrainViewerView.h"
#include "Mainfrm.h"

#include "tslviewmodeinterfacent.h"
#include "tslviewmodepan.h"
#include "tslviewmodezoom.h"
#include "tslviewmodezoompan.h"
#include "tslviewmodeterrain.h"
#include "tslviewmodearea.h"
#include "tslviewmodescattergraph.h"
#include "tslviewmodelos.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const char * CTerrainViewerView::m_layerName = "map" ;
double m_start_x ;
double m_start_y ;

/////////////////////////////////////////////////////////////////////////////
// CTerrainViewerView

IMPLEMENT_DYNCREATE(CTerrainViewerView, CView)

  BEGIN_MESSAGE_MAP(CTerrainViewerView, CView)
    //{{AFX_MSG_MAP(CTerrainViewerView)
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
    ON_COMMAND(ID_TOOLS_TERRAIN, OnToolsTerrain)
    ON_COMMAND(ID_TOOLS_AREAGRAPH, OnToolsAreagraph)
    ON_COMMAND(ID_VIEW_TERRAINEXTENT, OnViewTerrainExtent)
    ON_UPDATE_COMMAND_UI(ID_VIEW_TERRAINEXTENT, OnUpdateViewTerrainExtent)
    ON_COMMAND(ID_TOOLS_LOS, OnToolsLos)
    ON_COMMAND(ID_VIEW_TERRAININFO, OnViewTerraininfo)
    ON_UPDATE_COMMAND_UI(ID_VIEW_TERRAININFO, OnUpdateViewTerraininfo)
    ON_COMMAND(ID_TOOLS_LOSAREA, OnToolsLosarea)
    ON_UPDATE_COMMAND_UI(ID_TOOLS_LOSAREA, OnUpdateToolsLosarea)
    ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOMIN,  OnUpdateToolsAction)
    ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOMPAN, OnUpdateToolsMode)
    ON_WM_MOUSEWHEEL()
    //}}AFX_MSG_MAP

    // Our common handles can update multiple items.
    ON_UPDATE_COMMAND_UI(ID_EDIT_COPY,     OnUpdateToolsAction)
    ON_UPDATE_COMMAND_UI(ID_FILE_PRINT,    OnUpdateToolsAction)
    ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOMOUT, OnUpdateToolsAction)
    ON_UPDATE_COMMAND_UI(ID_TOOLS_PAN,     OnUpdateToolsMode)
    ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOM,    OnUpdateToolsMode)
    ON_UPDATE_COMMAND_UI(ID_TOOLS_AREAGRAPH,     OnUpdateToolsMode)
    ON_UPDATE_COMMAND_UI(ID_TOOLS_TERRAIN,     OnUpdateToolsMode)
    ON_UPDATE_COMMAND_UI(ID_TOOLS_INTERVISIBILITY,     OnUpdateToolsMode)
    ON_UPDATE_COMMAND_UI(ID_TOOLS_LOS, OnUpdateToolsMode)

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

  /////////////////////////////////////////////////////////////////////////////
  // CTerrainViewerView construction/destruction

  CTerrainViewerView::CTerrainViewerView()
    : m_drawingSurface(NULL) 
    , m_currentMode(0) 
    , m_modeInterface(NULL) 
    , m_mapLoaded(false)
    , m_printing(false)
    , m_viewTerrainExtent(true)
  { 
  }

  CTerrainViewerView::~CTerrainViewerView()
  {
    if ( m_currentMode )
    {
      delete m_currentMode ;
      m_currentMode = 0 ;
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

  BOOL CTerrainViewerView::PreCreateWindow(CREATESTRUCT& cs)
  {
    return CView::PreCreateWindow(cs);
  }

  /////////////////////////////////////////////////////////////////////////////
  // CTerrainViewerView drawing

  void CTerrainViewerView::OnDraw(CDC* pDC)
  {
    ASSERT( m_drawingSurface ) ;

    if ( m_drawingSurface )
    {
      CRect rect;

      CTerrainViewerDoc * doc = GetDocument() ;

      // Only draw the exposed bits of the display
      if ( pDC->GetClipBox( rect ) == NULLREGION )
        GetClientRect( rect ) ;

      m_drawingSurface->drawDU( rect.left, rect.bottom, rect.right, rect.top, true ) ;

      // Don't forget to draw any echo rectangle that may be active.
      m_modeInterface->drawEchoRectangle( ) ;
    }
  }

  /////////////////////////////////////////////////////////////////////////////
  // CTerrainViewerView printing

  BOOL CTerrainViewerView::OnPreparePrinting(CPrintInfo* pInfo)
  {
    // default preparation
    pInfo->SetMaxPage(1);
    return DoPreparePrinting(pInfo);
  }

  void CTerrainViewerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
  {
    // Flag that we are printing and store the window and user unit
    // extents.  This is necessary since the extent of the printer is
    // different from the extent of the window.  We override it in the
    // OnPrint method and put the saved extents back in the OnEndPrinting method.
    m_printing = true ;
    if ( m_drawingSurface )
    {
      m_drawingSurface->getUUExtent( &m_uux1, &m_uuy1, &m_uux2, &m_uuy2 ) ;
      m_drawingSurface->getDUExtent( &m_scrx1, &m_scry1, &m_scrx2, &m_scry2 ) ;
    }
  }

  void CTerrainViewerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
  {
    if ( m_drawingSurface )
    {
      // Put the extents back the way they were before we were printing.
      m_drawingSurface->resize( m_uux1, m_uuy1, m_uux2, m_uuy2, false ) ;
      m_drawingSurface->wndResize( m_scrx1, m_scry1, m_scrx2, m_scry2, false ) ;
    }

    m_printing = false ;
  }

  /////////////////////////////////////////////////////////////////////////////
  // CTerrainViewerView diagnostics

#ifdef _DEBUG
  void CTerrainViewerView::AssertValid() const
  {
    CView::AssertValid();
  }

  void CTerrainViewerView::Dump(CDumpContext& dc) const
  {
    CView::Dump(dc);
  }

  CTerrainViewerDoc* CTerrainViewerView::GetDocument() // non-debug version is inline
  {
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTerrainViewerDoc)));
    return (CTerrainViewerDoc*)m_pDocument;
  }
#endif //_DEBUG

  /////////////////////////////////////////////////////////////////////////////
  // CTerrainViewerView message handlers

  void CTerrainViewerView::OnInitialUpdate() 
  {
    CView::OnInitialUpdate();

    m_info.create( this );

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
      m_drawingSurface->setOption( TSLOptionDynamicArcSupportEnabled, true ) ;
      m_drawingSurface->wndResize( 0, 0, rect.Width(), rect.Height() ) ;
      m_modeInterface = new TSLViewModeInterfaceNT( m_drawingSurface, m_hWnd ) ;
    }
    else if ( m_mapLoaded ) 
    {
      // We have a drawing surface with a map loaded into it.
      // Remove the data layer.
      m_drawingSurface->removeDataLayer( m_layerName ) ;
      m_drawingSurface->removeDataLayer( "VisLayer" ) ;
    }

    m_mapLoaded = false ;

    CTerrainViewerDoc * doc = GetDocument() ;

    TSLMapDataLayer * mapDataLayer = doc->getMapDataLayer() ;

    if ( mapDataLayer )
    {
      // If the document loaded successfully, add it to the drawing surface
      // and reset the current view to display the entire map.
      m_drawingSurface->addDataLayer( mapDataLayer, m_layerName ) ;
      m_drawingSurface->setDataLayerProps(m_layerName, TSLPropertyBuffered, 1);
      m_mapLoaded = true ;
    }

    TSLViewShedDataLayer* viewShedLayer = doc->getViewShedDataLayer();
    if (viewShedLayer)
    {
      // If the document loaded successfully, add it to the drawing surface
      // and reset the current view to display the entire map.
      m_drawingSurface->addDataLayer( viewShedLayer, "ViewShedDataLayer" );
      // Set the viewshed layer to be translucent
      m_drawingSurface->setDataLayerProps("ViewShedDataLayer", TSLPropertyTransparency, 127);
    }

    TSLStandardDataLayer * visDataLayer = doc->getVisDataLayer() ;
    if ( visDataLayer )
    {
      // If the document loaded successfully, add it to the drawing surface
      // and reset the current view to display the entire map.
      m_drawingSurface->addDataLayer( visDataLayer, "VisLayer" ) ;
    }

    TSLStandardDataLayer * extentLayer = doc->getExtentLayer() ;
    if ( extentLayer )
    {
      // If the document loaded successfully, add it to the drawing surface
      // and reset the current view to display the entire map.
      m_drawingSurface->addDataLayer( extentLayer, "overlay" ) ;
    }

    // Reset the current view to display the entire map.
    m_drawingSurface->reset( false ) ;

    // Reset the current interaction mode by deleting it.
    if ( m_currentMode )
    {
      delete m_currentMode ;
      m_currentMode = 0 ;
    }
  }

  void CTerrainViewerView::OnToolsZoomin() 
  {
    if ( m_drawingSurface )
    {
      // Zoom in by 30%, and request a window redraw
      m_drawingSurface->zoom( 30, true, false ) ;
      InvalidateRect( 0, false ) ;
    }
  }

  void CTerrainViewerView::OnToolsZoomout() 
  {
    if ( m_drawingSurface )
    {
      // Zoom out by 30%, and request a window redraw
      m_drawingSurface->zoom( 30, false, false ) ;
      InvalidateRect( 0, false ) ;
    }
  }

  void CTerrainViewerView::OnToolsReset() 
  {
    if ( m_drawingSurface )
    {
      // Reset the window to display the entire map, and request a window redraw.
      m_drawingSurface->reset( false ) ;
      InvalidateRect( 0, false ) ;
    }
  }

  void CTerrainViewerView::OnSize(UINT nType, int cx, int cy) 
  {
    CView::OnSize(nType, cx, cy);

    if ( m_drawingSurface && !m_printing )
    {
      // Inform the drawing surface of the new window size,
      // attempting to keep the top left corner the same.
      // Do not ask for an automatic redraw since we will get an OnDraw call
      m_drawingSurface->wndResize( 0, 0, cx, cy, false, 
        TSLResizeActionMaintainCentre ) ;
    }
  }

  void CTerrainViewerView::OnMouseMove(UINT nFlags, CPoint point) 
  {
    CTerrainViewerDoc * doc = GetDocument() ;

    // Set up the displayExtent so that if any mode uses the terrain database, it has the correct layer
    if (doc->getTerrainDB() &&
        doc->getTerrainDB()->isOpen())
    {
      CRect rect;
      GetClientRect(rect);

      double x1, y1, x2, y2;

      if (!doc->isConversionRequired())
      {
        m_drawingSurface->DUToMU(0, 0, &x1, &y1);
        m_drawingSurface->DUToMU(rect.Width() - 1, rect.Height() - 1, &x2, &y2);
      }
      else
      {
        double lat1, lat2, lng1, lng2;
        m_drawingSurface->DUToLatLong(0, 0, &lat1, &lng1);
        m_drawingSurface->DUToLatLong(rect.Width() - 1, rect.Height() - 1, &lat2, &lng2);

        const TSLCoordinateSystem *terrainCS = doc->terrainCoordinateSystem();
        if (terrainCS)
        {
          doc->getTerrainDB()->latLongToMU(lat1, lng1, &x1, &y1);
          doc->getTerrainDB()->latLongToMU(lat2, lng2, &x2, &y2);
        }
        else
        {
          // The terrain database is already in lat/lon
          x1 = lng1;
          y1 = lat1;
          x2 = lng2;
          y2 = lat2;
        }
      }

      doc->getTerrainDB()->displayExtent(rect.Width(), rect.Height(), x1, y1, x2, y2);
    }

    // If the user is in the middle of an interaction, pass the event onto the handler
    if ( m_currentMode )
    {
      if ( m_currentMode->OnMouseMove( point.x, point.y,
        (nFlags & MK_SHIFT), (nFlags & MK_CONTROL) ) )
      {
        // Request a redraw if the interaction hander requires it
        InvalidateRect( 0, false) ;
      }

    }

    // Display position/Alt in status bar
    // ----------------------------------

    CMainFrame * pFrame = static_cast< CMainFrame * >( AfxGetMainWnd() ) ;
    CStatusBar * pStatus = pFrame->getStatusBar();

    if(pStatus && m_drawingSurface && doc->getTerrainDB() && doc->getTerrainDB()->isOpen() )
    {
      char  tmpstr [200];
      double height;
      TSLTerrainDataItem di;
      double x, y ;	

      if ( !doc->isConversionRequired() ) 
      {
        m_drawingSurface->DUToMU(point.x, point.y, &x, &y ) ;
      }
      else
      {
        double lat, lng ;
        m_drawingSurface->DUToLatLong( point.x, point.y, &lat, &lng );
        const TSLCoordinateSystem *terrainCS = doc->terrainCoordinateSystem();
        if( terrainCS )
        {
          terrainCS->latLongToMU(lat, lng, &x, &y);
        }
        else
        {
          // The terrain database is already in lat/lon
          x = lng;
          y = lat;
        }
      }

      //Terrain Info...
      if ( doc->getTerrainDB()->query( x, y, &di, TSLTerrainData_All, false, TSLTerrainInterpolate_LINEAR ) == TSLTerrain_OK )
      {
        double lat, lon;
        const TSLCoordinateSystem *terrainCS = doc->terrainCoordinateSystem();
        if( terrainCS )
        {
          terrainCS->MUToLatLong ( x, y, &lat, &lon ) ;
        }
        else
        {
          // The terrain database is already in lat/lon
          lat = y;
          lon = x;
        }
        height = di.m_z;
        if( m_info.showing() )
        {
          if ( di.m_nearestIsNull )
          {
            m_info.update( lat, lon, x, y, height, 0.0, 0.0, 0.0, di.m_xResolution, di.m_yResolution );
          }
          else
          {
            m_info.update( lat, lon, x, y, height, di.m_nearestX, di.m_nearestY, di.m_nearestZ, di.m_xResolution, di.m_yResolution );
          }
        }
        _snprintf( tmpstr, sizeof( tmpstr ), "Latitude: %.3lf   Longitude: %.3lf   Height: %lf", lat, lon, height );
        pStatus->SetPaneText(2, (LPCTSTR)TSLUTF8Decoder(tmpstr));
      }
      updateExtentRect();

    }
    else
    {
      pStatus->SetPaneText(2, _T("No Data"));
    }

    CView::OnMouseMove(nFlags, point);
  }

  void CTerrainViewerView::OnLButtonDown(UINT nFlags, CPoint point) 
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

    m_start_x = point.x ;
    m_start_y = point.y ;


    CView::OnLButtonDown(nFlags, point);
  }

  void CTerrainViewerView::OnMButtonDown(UINT nFlags, CPoint point) 
  {

    double x1, y1 ;

    m_drawingSurface->DUToUU( point.x, point.y, &x1, &y1 ) ;
    m_drawingSurface->pan( x1, y1, false ) ;

    InvalidateRect( 0, false ) ;

    CView::OnMButtonDown(nFlags, point);
  }

  void CTerrainViewerView::OnRButtonDown(UINT nFlags, CPoint point) 
  {
    // If the user is in the middle of an interaction, pass the event onto the handler
    if ( m_currentMode )
    {
      if ( m_currentMode->OnRButtonDown( point.x, point.y,
        nFlags & MK_SHIFT, nFlags & MK_CONTROL ) )
      {
        // Request a redraw if the interaction hander requires it
        Invalidate() ;
      }
    }

    CView::OnRButtonDown(nFlags, point);
  }

  BOOL CTerrainViewerView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
  {
    ScreenToClient(&pt);

    if ( m_currentMode )
    {
      if ( m_currentMode->OnMouseWheel( pt.x, pt.y, zDelta
        , nFlags & MK_SHIFT, nFlags & MK_CONTROL ) )
      {
        // Request a redraw if the interaction hander requires it
        Invalidate() ;
      }
    }
    else
    {
      if (zDelta > 0)
      {
        m_drawingSurface->zoom( 30, true, false ) ;
      }
      else
      {
        m_drawingSurface->zoom( 30, false, false ) ;
      }
      InvalidateRect( 0, false ) ;
    }

    return CView::OnMouseWheel(nFlags, zDelta, pt);
  }

  void CTerrainViewerView::OnLButtonUp(UINT nFlags, CPoint point) 
  {
    // If the user is in the middle of an interaction, pass the event onto the handler
    if ( m_currentMode )
    {
      if ( m_currentMode->OnLButtonUp( point.x, point.y,
        nFlags & MK_SHIFT, nFlags & MK_CONTROL ) )
      {
        // Request a redraw if the interaction hander requires it
        Invalidate() ;
      }
    }

    CView::OnLButtonUp(nFlags, point);
  }

  void CTerrainViewerView::OnMButtonUp(UINT nFlags, CPoint point) 
  {
    // If the user is in the middle of an interaction, pass the event onto the handler
    if ( m_currentMode )
    {
      if ( m_currentMode->OnMButtonUp( point.x, point.y,
        nFlags & MK_SHIFT, nFlags & MK_CONTROL ) )
      {
        // Request a redraw if the interaction hander requires it
        Invalidate() ;
      }
    }

    CView::OnMButtonUp(nFlags, point);
  }

  void CTerrainViewerView::OnRButtonUp(UINT nFlags, CPoint point) 
  {
    // If the user is in the middle of an interaction, pass the event onto the handler
    if ( m_currentMode )
    {
      if ( m_currentMode->OnRButtonUp( point.x, point.y,
        nFlags & MK_SHIFT, nFlags & MK_CONTROL ) )
      {
        // Request a redraw if the interaction hander requires it
        Invalidate() ;
      }
    }

    CView::OnRButtonUp(nFlags, point);
  }

  void CTerrainViewerView::OnToolsPan() 
  {
    // If pan mode is currently active, then just deactivate it,
    // otherwise deactivate any current mode and activate pan mode
    bool active = m_currentMode && m_currentMode->id() == ID_TOOLS_PAN ;

    if ( m_currentMode )
    {
      delete m_currentMode ;
      m_currentMode = 0 ;
    }

    // Pass the menu item ID to the mode so that the update handler works
    if ( !active )
      m_currentMode = new TSLViewModePan( m_modeInterface, ID_TOOLS_PAN ) ;
  }

  void CTerrainViewerView::OnToolsZoom() 
  {
    // If zoom mode is currently active, then just deactivate it,
    // otherwise deactivate any current mode and activate zoom mode
    bool active = m_currentMode && m_currentMode->id() == ID_TOOLS_ZOOM ;

    if ( m_currentMode )
    {
      delete m_currentMode ;
      m_currentMode = 0 ;
    }

    // Pass the menu item ID to the mode so that the update handler works
    if ( !active )
      m_currentMode = new TSLViewModeZoom( m_modeInterface, ID_TOOLS_ZOOM ) ;
  }

  void CTerrainViewerView::OnToolsZoompan() 
  {
    // If zoom/pan mode is currently active, then just deactivate it,
    // otherwise deactivate any current mode and activate zoom/pan mode
    bool active = m_currentMode && m_currentMode->id() == ID_TOOLS_ZOOMPAN ;

    if ( m_currentMode )
    {
      delete m_currentMode ;
      m_currentMode = 0 ;
    }

    // Pass the menu item ID to the mode so that the update handler works
    if ( !active )
      m_currentMode = new TSLViewModeZoomPan( m_modeInterface, ID_TOOLS_ZOOMPAN ) ;
  }

  void CTerrainViewerView::OnUpdateToolsAction(CCmdUI* pCmdUI) 
  {
    // Common update handler to all items that are dependant upon the map
    // being loaded.
    pCmdUI->Enable( m_mapLoaded ? true : false ) ;
  }

  void CTerrainViewerView::OnUpdateToolsMode(CCmdUI* pCmdUI) 
  {
    // Common update handler to all items that are dependant upon the map
    // being loaded, and act as radios dependant upon the current mode.
    pCmdUI->Enable( m_mapLoaded ? true : false  ) ;
    pCmdUI->SetCheck( m_currentMode && m_currentMode->id() == pCmdUI->m_nID ) ;
  }

  void CTerrainViewerView::OnEditCopy() 
  {
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

  void CTerrainViewerView::OnLButtonDblClk(UINT nFlags, CPoint point) 
  {
    // Just handle like a normal press to avoid losing the second press
    OnLButtonDown( nFlags, point ) ;
    CView::OnLButtonDblClk(nFlags, point);
  }

  void CTerrainViewerView::OnRButtonDblClk(UINT nFlags, CPoint point) 
  {
    // Just handle like a normal press to avoid losing the second press
    OnRButtonDown( nFlags, point ) ;
    CView::OnRButtonDblClk(nFlags, point);
  }
  void CTerrainViewerView::OnMButtonDblClk(UINT nFlags, CPoint point) 
  {
    // Just handle like a normal press to avoid losing the second press
    OnMButtonDown( nFlags, point ) ;
    CView::OnMButtonDblClk(nFlags, point);
  }

  void CTerrainViewerView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
  {
    HDC printerDC = pDC->GetSafeHdc()  ;

    if ( m_drawingSurface ) 
    {
      // If we are printing the current view, then we need to
      // resize the window according to the paper sizes. 
      CRect prc = pInfo->m_rectDraw ;
      int width  = prc.Width() ;
      int height = prc.Height() ;

      double ratio = ((double)(width))/((double)(height)) ;

      double xrange = m_uux2 - m_uux1 ;
      double yrange = m_uuy2 - m_uuy1 ;

      double m_currentCentreX = m_uux1 + xrange/2 ;
      double m_currentCentreY = m_uuy1 + yrange/2 ;

      if ( ratio == 1 )
      {
        if ( xrange >= yrange )
          yrange = xrange ;
        else
          xrange = yrange ;
      }
      else if ( ratio != (xrange/yrange) )
      {
        if ( ratio > 1 )
          xrange = ratio * yrange ;
        else 
          yrange = xrange / ratio ;
      }

      //calculates extents of the map to be printed
      double xMin = m_currentCentreX - xrange/2 ;
      double yMin = m_currentCentreY - yrange/2 ;
      double xMax = m_currentCentreX + xrange/2 ;
      double yMax = m_currentCentreY + yrange/2 ;

      pDC->Rectangle( prc ) ;

      // Now tell the drawing surface that the window and view have resized,
      // and draw it to the printers DC
      m_drawingSurface->wndResize ( 0, 0, prc.Width(), prc.Height(), false ) ;
      m_drawingSurface->resize(  xMin, yMin, xMax, yMax, false ) ; 	
      m_drawingSurface->drawToHDC( (TSLDeviceContext)printerDC , xMin, yMin, xMax, yMax, false ) ;

      pInfo->m_bContinuePrinting = false ;
    }
  }

  BOOL CTerrainViewerView::OnEraseBkgnd(CDC* pDC) 
  {
    // Since the drawing surface is double buffered, we don't need to erase the
    // background.
    return true;
  }

  void CTerrainViewerView::resetMode()
  {
    if ( m_currentMode )
    {
      delete m_currentMode ;
      m_currentMode = 0 ;
    }
    Invalidate();  
  }


  void CTerrainViewerView::OnToolsTerrain()    // Cross section tool
  {

    bool active = m_currentMode && m_currentMode->id() == ID_TOOLS_TERRAIN ;

    if ( m_currentMode )
    {
      delete m_currentMode ;
      m_currentMode = 0 ;
    }
    Invalidate();

    // Pass the menu item ID to the mode so that the update handler works
    if ( !active )
    {
      CMainFrame * pFrame = static_cast< CMainFrame * >( AfxGetMainWnd() ) ;

      m_currentMode = new TSLViewModeTerrain( m_modeInterface, ID_TOOLS_TERRAIN, pFrame->getTerrainView(), GetDocument() ) ;
    }

  }

  void CTerrainViewerView::OnToolsAreagraph() 
  {
    // If zoom mode is currently active, then just deactivate it,
    // otherwise deactivate any current mode and activate zoom mode
    bool active = m_currentMode && m_currentMode->id() == ID_TOOLS_AREAGRAPH ;

    if ( m_currentMode )
    {
      delete m_currentMode ;
      m_currentMode = 0 ;
    }
    Invalidate();

    // Pass the menu item ID to the mode so that the update handler works
    CTerrainViewerDoc * doc = GetDocument() ;
    if ( !active )
      m_currentMode = new TSLViewModeScatterGraph( m_modeInterface, ID_TOOLS_AREAGRAPH, doc) ;  
  }

  void CTerrainViewerView::OnViewTerrainExtent() 
  {
    CTerrainViewerDoc * doc = GetDocument() ;
    m_viewTerrainExtent = !m_viewTerrainExtent;
    if ( !m_viewTerrainExtent )
    {
      TSLStandardDataLayer * extentLayer = doc->getExtentLayer() ;
      if ( extentLayer && ( extentLayer->entitySet()->size() > 0 ) )
      {
        extentLayer->entitySet()->clear();
        extentLayer->notifyChanged();
      }
    }
    else
    {
      updateExtentRect();
    }
    InvalidateRect( NULL, FALSE );
  }

  void CTerrainViewerView::OnUpdateViewTerrainExtent(CCmdUI* pCmdUI) 
  {
    pCmdUI->SetCheck( m_viewTerrainExtent ? 1 : 0 );	
  }

  void CTerrainViewerView::updateExtentRect()
  {
    CTerrainViewerDoc * doc = GetDocument() ;

    double lat1, long1, lat2, long2, lat3, long3, lat4, long4;
    double muX1, muY1, muX2, muY2;

    TSLStandardDataLayer * extentLayer = doc->getExtentLayer() ;

    if ( m_viewTerrainExtent && m_mapLoaded && extentLayer  && ( extentLayer->entitySet()->size() == 0 ) )
    {
      if ( doc->getTerrainDB() && doc->getTerrainDB()->queryExtent(muX1, muY1, muX2, muY2) == TSLTerrain_OK )
      {
        // The query will have returned units in mu, which could be lat/long
        const TSLCoordinateSystem *terrainCS = doc->terrainCoordinateSystem();
        if( terrainCS )
        {
          // Many not be rectangular in lat/long - do 4 corners
          terrainCS->MUToLatLong(muX1, muY1, &lat1, &long1);
          terrainCS->MUToLatLong(muX2, muY1, &lat2, &long2);
          terrainCS->MUToLatLong(muX2, muY2, &lat3, &long3);
          terrainCS->MUToLatLong(muX1, muY2, &lat4, &long4);
        }
        else
        {
          // The terrain database is already in lat/lon
          lat1 = lat2 = muY1;
          lat3 = lat4 = muY2;
          long1 = long4 = muX1;
          long2 = long3 = muX2;
        }

        // Clamp values
        if (long1 < -180.0)
          long1 = -180.0;
        if (long2 > 180.0)
          long2 = 180.0;
        if (long3 > 180.0)
          long3 = 180.0;
        if (long4 < -180.0)
          long4 = -180.0;

        if (lat1 < -90.0)
          lat1 = -90.0;
        if (lat2 < -90.0)
          lat2 = -90.0;
        if (lat3 > 90.0)
          lat3 = 90.0;
        if (lat4 > 90.0)
          lat4 = 90.0;

        TSLEntitySet * entitySet = extentLayer->entitySet() ;
        TSLCoordSet  * coords    = new TSLCoordSet () ;

        TSLTMC xpos, ypos ;

        m_drawingSurface->latLongToTMC(lat1,long1, &xpos, &ypos, true) ;
        coords->add(xpos, ypos, 0) ;
        m_drawingSurface->latLongToTMC(lat2,long2, &xpos, &ypos, true) ;
        coords->add(xpos, ypos, 0) ;
        m_drawingSurface->latLongToTMC(lat3,long3, &xpos, &ypos, true) ;
        coords->add(xpos, ypos, 0) ;
        m_drawingSurface->latLongToTMC(lat4,long4, &xpos, &ypos, true) ;
        coords->add(xpos, ypos, 0) ;
        m_drawingSurface->latLongToTMC(lat1,long1, &xpos, &ypos, true) ;
        coords->add(xpos, ypos, 0) ;

        TSLPolyline * ply = entitySet->createPolyline(100, coords, true) ;

        // Attach specific colour/style attributes to it.

        ply->setRendering(TSLRenderingAttributeEdgeColour, 181 );
        ply->setRendering(TSLRenderingAttributeEdgeThickness, 1.0);
        ply->setRendering(TSLRenderingAttributeEdgeStyle, 1);   

        extentLayer->notifyChanged();
        Invalidate();

      }
    }
  }

  void CTerrainViewerView::OnToolsLos() 
  {
    bool active = m_currentMode && m_currentMode->id() == ID_TOOLS_LOS;

    if (m_currentMode)
    {
      delete m_currentMode;
      m_currentMode = 0;
    }
    Invalidate();

    CTerrainViewerDoc * doc = GetDocument() ;

    // Pass the menu item ID to the mode so that the update handler works
    if ( !active )
    {
      CMainFrame * pFrame = static_cast< CMainFrame * >( AfxGetMainWnd() ) ;

      m_currentMode = new TSLViewModeLOS(m_modeInterface, ID_TOOLS_LOS, pFrame->getTerrainView(), doc ) ;
      ((TSLViewModeLOS*)m_currentMode)->setCentralMeridian( doc->getMapDataLayer()->queryMapCoordinateSystem()->projectionCentralMeridian() );
    }

  }

  void CTerrainViewerView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
  {
    if ( lHint == HINT_TERRAIN_DATA_LOADED )
    {
      // Force a resize/redraw so that the extent of the terrain data is highlighted
      m_drawingSurface->reset( false ) ;
      Invalidate() ;
    }
  }

  void CTerrainViewerView::OnViewTerraininfo() 
  {
    m_info.show( !m_info.showing() );
  }

  void CTerrainViewerView::OnUpdateViewTerraininfo(CCmdUI* pCmdUI) 
  {
    pCmdUI->SetCheck( m_info.showing() ? 1 : 0 );
  }

  void CTerrainViewerView::OnToolsLosarea() 
  {
    bool active = m_currentMode && m_currentMode->id() == ID_TOOLS_LOSAREA;

    if (m_currentMode)
    {
      delete m_currentMode;
      m_currentMode = 0;
    }
    Invalidate();

    CTerrainViewerDoc * doc = GetDocument() ;

    // Pass the menu item ID to the mode so that the update handler works
    if ( !active )
    {
      m_currentMode = new TSLViewModeArea( m_modeInterface, ID_TOOLS_LOSAREA, doc) ;  
    }
  }

  void CTerrainViewerView::OnUpdateToolsLosarea(CCmdUI* pCmdUI) 
  {
    pCmdUI->Enable( m_mapLoaded ? true : false  ) ;
    pCmdUI->SetCheck( m_currentMode && m_currentMode->id() == pCmdUI->m_nID ) ;	
  }
