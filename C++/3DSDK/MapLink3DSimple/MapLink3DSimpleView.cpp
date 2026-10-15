// MapLink3DSimpleView.cpp : implementation of the MapLink3DSimpleView class
//

#include "stdafx.h"
#include "MapLink3DSimple.h"

#include "MapLink3DSimpleDoc.h"
#include "MapLink3DSimpleView.h"
#include <string>
#include "camera.h"
#include "CameraDlg.h"

#include "tsl3dviewmodeinterfacent.h"
#include "tsl3dviewmodezoom.h"
#include "tsl3dviewmoderotate.h"
#include "tsl3dviewmodefly.h"
#include "tsl3dviewmodecamera.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MapLink3DSimpleView

IMPLEMENT_DYNCREATE(MapLink3DSimpleView, CView)

BEGIN_MESSAGE_MAP(MapLink3DSimpleView, CView)
//{{AFX_MSG_MAP(MapLink3DSimpleView)
  ON_COMMAND(ID_TOOLS_RESET, OnToolsReset)
  ON_WM_SIZE()
  ON_WM_MOUSEMOVE()
  ON_WM_LBUTTONDOWN()
  ON_WM_RBUTTONDOWN()
  ON_WM_LBUTTONUP()
  ON_WM_RBUTTONUP()
  ON_UPDATE_COMMAND_UI(ID_TOOLS_RESET,   OnUpdateToolsAction)
  ON_COMMAND(ID_TOOLS_ZOOM, OnToolsZoom)
  ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
  ON_WM_LBUTTONDBLCLK()
  ON_WM_RBUTTONDBLCLK()
  ON_WM_ERASEBKGND()
  //}}AFX_MSG_MAP

  // Our common handlers can update multiple items.
  ON_UPDATE_COMMAND_UI(ID_EDIT_COPY,     OnUpdateToolsAction)
  ON_UPDATE_COMMAND_UI(ID_FILE_PRINT,    OnUpdateToolsAction)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOM,     OnUpdateToolsMode)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_ROTATE,     OnUpdateToolsMode)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_FLY,     OnUpdateToolsMode)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_CAMERA,     OnUpdateToolsMode)

  // Add the middle button handlers since DevStudio doesn't 
  // give us the option.
  ON_WM_MBUTTONDOWN()
  ON_WM_MBUTTONDBLCLK()
  ON_WM_MBUTTONUP()

  ON_COMMAND(ID_TOOLS_ROTATE, OnToolsRotate)
  ON_COMMAND(ID_TOOLS_FLY, OnToolsFly)
  ON_COMMAND(ID_TOOLS_CAMERA, OnToolsCamera)

  ON_UPDATE_COMMAND_UI(ID_INDICATOR_LAT, OnUpdateLat)
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_LON, OnUpdateLon )
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_ALT, OnUpdateAlt )

  ON_COMMAND(ID_LIGHTING_ENABLED, OnEnableLighting)
  ON_UPDATE_COMMAND_UI(ID_LIGHTING_ENABLED, OnUpdateEnableLighting)

  ON_COMMAND(ID_PRIMITIVECREATION_POLYGON, OnCreatePolygon)
  ON_COMMAND(ID_PRIMITIVECREATION_EXTRUDEDPOLYGON, OnCreateExtrudedPolygon)

  ON_WM_CHAR()
  ON_WM_KEYDOWN()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MapLink3DSimpleView construction/destruction

MapLink3DSimpleView::MapLink3DSimpleView()
{
  // Clear my internal variables
  m_drawingSurface = 0 ; 
  m_currentMode    = 0 ;
  m_modeInterface  = 0 ;
  m_stdLayer	   = 0 ;
  m_lightingEnabled = FALSE;
}

MapLink3DSimpleView::~MapLink3DSimpleView()
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
  }
  TSLDrawingSurface::cleanup() ;
}

/////////////////////////////////////////////////////////////////////////////
// MapLink3DSimpleView drawing

void MapLink3DSimpleView::OnDraw(CDC* pDC)
{
  ASSERT( m_drawingSurface ) ;
  
  if ( m_drawingSurface )
  {
    RECT rect ;
    
    // Only draw the exposed bits of the display
    if ( pDC->GetClipBox( &rect ) == NULLREGION )
      GetClientRect( &rect ) ;
    
    m_drawingSurface->drawDU( rect.left, rect.bottom, rect.right, rect.top, false ) ;
    
    // Don't forget to draw any echo rectangle that may be active.
    m_modeInterface->drawEchoRectangle( ) ;
  }
}

/////////////////////////////////////////////////////////////////////////////
// MapLink3DSimpleView diagnostics

#ifdef _DEBUG
void MapLink3DSimpleView::AssertValid() const
{
  CView::AssertValid();
}

void MapLink3DSimpleView::Dump(CDumpContext& dc) const
{
  CView::Dump(dc);
}

MapLink3DSimpleDoc* MapLink3DSimpleView::GetDocument() // non-debug version is inline
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(MapLink3DSimpleDoc)));
  return (MapLink3DSimpleDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// MapLink3DSimpleView message handlers

void MapLink3DSimpleView::OnInitialUpdate() 
{
  CView::OnInitialUpdate();
  
  MapLink3DSimpleDoc * doc = GetDocument() ;
  bool reset = !m_drawingSurface ;
  
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
    m_drawingSurface = new TSL3DWinGLSurface( m_hWnd, false ) ;

    // Give the 'sky' a colour!
    static const int skyColourIndex( 4 );
    m_drawingSurface->setBackgroundColour( skyColourIndex );

    static int const wireframeColourIndex( 181 );
    static int const solidColourIndex( 60 );

    m_backdrop = (LPCTSTR)TSLUTF8Decoder(TSLUtilityFunctions::getMapLinkHome());
    m_backdrop += _T("\\config\\earth.png");

    // Set the bitmap to display over the terrain plus colours for solid-backdrop and wireframe rendering.
    m_drawingSurface->setTerrainRendering( wireframeColourIndex, solidColourIndex, TSLUTF8Encoder(m_backdrop) );

    m_drawingSurface->setOption( TSLOptionDoubleBuffered, true ) ;
    m_drawingSurface->wndResize( 0, 0, rect.Width(), rect.Height() ) ;

    m_modeInterface = new TSL3DViewModeInterfaceNT( m_drawingSurface, m_hWnd ) ;
    m_drawingSurface->setRenderingCallback( renderingCallback, this ) ;
  }
  else
  {
    // We have a drawing surface, potentially with a map loaded into it.
    // Detach the map from the surface
    if ( m_drawingSurface && doc )
      doc->detachFromSurface( m_drawingSurface ) ;
  }
  
  // If the document loaded successfully, add it to the drawing surface
  doc->addToSurface( m_drawingSurface ) ;
  
  if ( reset )
  {
    // and reset the current view to display the entire map.
    m_drawingSurface->camera()->reset();
    m_drawingSurface->camera()->moveTo( DEFAULT_LATITUDE, DEFAULT_LONGITUDE, DEFAULT_ALTITUDE, TSL3DCameraMoveActionNone ) ;
    m_drawingSurface->camera()->lookAt( DEFAULT_LATITUDE, LOOK_AHEAD_OFFSET, 0.0, false ) ;
  }
  
  // Reset the current interaction mode by deleting it.
  if ( m_currentMode )
  {
    delete m_currentMode ;
    m_currentMode = 0 ;
  }
}

void MapLink3DSimpleView::OnToolsReset() 
{
  if ( m_drawingSurface )
  {
    // Reset the window to display the entire map, and request a window redraw.

    // and reset the current view to display the entire map.
    m_drawingSurface->camera()->reset();
    m_drawingSurface->camera()->moveTo( DEFAULT_LATITUDE, DEFAULT_LONGITUDE, DEFAULT_ALTITUDE, TSL3DCameraMoveActionNone ) ;
    m_drawingSurface->camera()->lookAt( DEFAULT_LATITUDE, LOOK_AHEAD_OFFSET, 0.0, false ) ;

    InvalidateRect( 0, false ) ;
  }
}

void MapLink3DSimpleView::OnSize(UINT nType, int cx, int cy) 
{
  CView::OnSize(nType, cx, cy);
  
  if ( m_drawingSurface )
  {
    // Inform the drawing surface of the new window size,
    // attempting to keep the top left corner the same.
    // Do not ask for an automatic redraw since we will get an OnDraw call
    m_drawingSurface->wndResize( 0, 0, cx, cy, false ) ;
  }
}

void MapLink3DSimpleView::OnMouseMove(UINT nFlags, CPoint point) 
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

void MapLink3DSimpleView::OnLButtonDown(UINT nFlags, CPoint point) 
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

void MapLink3DSimpleView::OnMButtonDown(UINT nFlags, CPoint point) 
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

void MapLink3DSimpleView::OnRButtonDown(UINT nFlags, CPoint point) 
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

void MapLink3DSimpleView::OnLButtonUp(UINT nFlags, CPoint point) 
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

void MapLink3DSimpleView::OnMButtonUp(UINT nFlags, CPoint point) 
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

void MapLink3DSimpleView::OnRButtonUp(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_currentMode )
  {
    if ( m_currentMode->id() == ID_TOOLS_CAMERA )
    {
      setCameraOrientation();
    }
    else
    {
      if ( m_currentMode->OnRButtonUp( point.x, point.y, nFlags & MK_SHIFT, nFlags & MK_CONTROL ) )
      {
        // Request a redraw if the interaction hander requires it
        InvalidateRect( 0, false ) ;
      }
    }
  }
  
  CView::OnRButtonUp(nFlags, point);
}

void MapLink3DSimpleView::OnToolsZoom() 
{
  // If zoom mode is currently active, then just deactivate it,
  // otherwise deactivate any current mode and activate pan mode
  bool active = m_currentMode && m_currentMode->id() == ID_TOOLS_ZOOM;
  
  if ( m_currentMode )
  {
    delete m_currentMode ;
    m_currentMode = 0 ;
  }
  
  // Pass the menu item ID to the mode so that the update handler works
  if ( !active )
    m_currentMode = new TSL3DViewModeZoom( m_modeInterface, ID_TOOLS_ZOOM ) ;
}

void MapLink3DSimpleView::OnUpdateToolsAction(CCmdUI* pCmdUI) 
{
  // Common update handler to all items that are dependant upon the map
  // being loaded.
  pCmdUI->Enable( true ) ;
}

void MapLink3DSimpleView::OnUpdateToolsMode(CCmdUI* pCmdUI) 
{
  // Common update handler to all items that are dependant upon the map
  // being loaded, and act as radios dependant upon the current mode.
  pCmdUI->Enable( true ) ;
  pCmdUI->SetCheck( m_currentMode && m_currentMode->id() == pCmdUI->m_nID ) ;
}

void MapLink3DSimpleView::OnEditCopy() 
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

void MapLink3DSimpleView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
  // Just handle like a normal press to avoid losing the second press
  OnLButtonDown( nFlags, point ) ;
  CView::OnLButtonDblClk(nFlags, point);
}

void MapLink3DSimpleView::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
  // Just handle like a normal press to avoid losing the second press
  OnRButtonDown( nFlags, point ) ;
  CView::OnRButtonDblClk(nFlags, point);
}
void MapLink3DSimpleView::OnMButtonDblClk(UINT nFlags, CPoint point) 
{
  // Just handle like a normal press to avoid losing the second press
  OnMButtonDown( nFlags, point ) ;
  CView::OnMButtonDblClk(nFlags, point);
}


BOOL MapLink3DSimpleView::OnEraseBkgnd(CDC* pDC) 
{
  // Since the drawing surface is double buffered, we don't need to erase the
  // background.
  return true;
}



void MapLink3DSimpleView::OnToolsRotate()
{
  // If zoom mode is currently active, then just deactivate it,
  // otherwise deactivate any current mode and activate pan mode
  bool active = m_currentMode && m_currentMode->id() == ID_TOOLS_ROTATE;
  
  if ( m_currentMode )
  {
    delete m_currentMode ;
    m_currentMode = 0 ;
  }
  
  // Pass the menu item ID to the mode so that the update handler works
  if ( !active )
    m_currentMode = new TSL3DViewModeRotate( m_modeInterface, ID_TOOLS_ROTATE ) ;
}

void MapLink3DSimpleView::renderingCallback(void * arg, int pendingTextures)
{
  MapLink3DSimpleView * view = (MapLink3DSimpleView *)arg ;

  if ( view->m_hWnd )
  {
    view->Invalidate() ;
  }
}

void MapLink3DSimpleView::OnToolsFly()
{
  // If zoom mode is currently active, then just deactivate it,
  // otherwise deactivate any current mode and activate pan mode
  bool active = m_currentMode && m_currentMode->id() == ID_TOOLS_FLY;
  
  if ( m_currentMode )
  {
    delete m_currentMode ;
    m_currentMode = 0 ;
  }
  
  // Pass the menu item ID to the mode so that the update handler works
  if ( !active )
    m_currentMode = new TSL3DViewModeFly( m_modeInterface, ID_TOOLS_FLY ) ;
}

void MapLink3DSimpleView::OnToolsCamera()
{
  // If zoom mode is currently active, then just deactivate it,
  // otherwise deactivate any current mode and activate pan mode
  bool active = m_currentMode && m_currentMode->id() == ID_TOOLS_CAMERA;
  
  if ( m_currentMode )
  {
    delete m_currentMode ;
    m_currentMode = 0 ;
  }
  
  // Pass the menu item ID to the mode so that the update handler works
  if ( !active )
  {
    m_currentMode = new TSL3DViewModeCamera( m_modeInterface, ID_TOOLS_CAMERA );

    setCameraOrientation();
  }
}

void MapLink3DSimpleView::setCameraOrientation()
{
  CCameraDlg dlg;

  if ( dlg.DoModal() == IDOK )
  {
    ((TSL3DViewModeCamera*)m_currentMode)->orientation( dlg.m_xAxis, dlg.m_yAxis, dlg.m_zAxis, dlg.m_angle );

    ((TSL3DViewModeCamera*)m_currentMode)->fov(dlg.m_fov);

    InvalidateRect( 0, false );
  }
}

void MapLink3DSimpleView::OnUpdateLat(CCmdUI* pCmdUI)
{
  double lat;
  double lon;
  double alt;

  m_drawingSurface->camera()->position( &lat, &lon, &alt );
  
  CString pos;
  pos.Format( _T("%f"), lat );

  pCmdUI->Enable( TRUE );
  pCmdUI->SetText( pos );
}

void MapLink3DSimpleView::OnUpdateLon(CCmdUI* pCmdUI)
{
  double lat;
  double lon;
  double alt;

  m_drawingSurface->camera()->position( &lat, &lon, &alt );
  
  CString pos;
  pos.Format( _T("%f"), lon );

  pCmdUI->Enable( TRUE );
  pCmdUI->SetText( pos );
}

void MapLink3DSimpleView::OnUpdateAlt(CCmdUI* pCmdUI)
{
  double lat;
  double lon;
  double alt;

  m_drawingSurface->camera()->position( &lat, &lon, &alt );
  
  CString pos;
  pos.Format( _T("%.2f"), alt );

  pCmdUI->Enable( TRUE );
  pCmdUI->SetText( pos );
}

void MapLink3DSimpleView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  // Quick synopsis:
  // ---------------
  //
  // 'w' / 'W' ... toggle wireframe mode.
  // 'e' / 'E' ... set terrain exaggeration to 1 / 10.
  // 's' / 'S' ... toggle between bitmap backdrop and solid colour backdrop display.

  switch ( (unsigned char)nChar )
  {
  case 'w':
  case 'W':
    {
      bool wireframe = m_drawingSurface->get3DOption( TSL3DOptionWireframeMode );
      m_drawingSurface->set3DOption( TSL3DOptionWireframeMode, !wireframe );

      InvalidateRect( 0, false );
    }

    break;

  case 'e':
    {
      m_drawingSurface->exaggerateTerrain( 1.0, 1.0 );

      InvalidateRect( 0, false );
    }

    break;

  case 'E':
    {
      m_drawingSurface->exaggerateTerrain( 10.0, 10.0 );

      InvalidateRect( 0, false );
    }

    break;

  case 's':
  case 'S':
    {
      int wireframeColour;
      int solidColour;
      char const* backdrop = 0;

      if ( m_drawingSurface->getTerrainRendering( &wireframeColour, &solidColour, &backdrop ) )
      {
        if ( backdrop && strlen( backdrop ) > 0 )
        {
          m_drawingSurface->setTerrainRendering( wireframeColour, solidColour, 0 );
        }
        else
        {
          m_drawingSurface->setTerrainRendering( wireframeColour, solidColour, TSLUTF8Encoder(m_backdrop) );
        }
      }

      InvalidateRect( 0, false );
    }

    break;

  default:
    break;
  }
}

void MapLink3DSimpleView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  static bool maintainAltitude( true );
  static TSL3DCameraMoveAction action( TSL3DCameraMoveActionMaintainGroundOrientation );

  double forward( 0.0 );
  double sideways( 0.0 );
  double upwards( 0.0 );

  static double DELTA( 1.0 );

  bool move( true );

  switch ( nChar )
  {
  case VK_LEFT:
    {
      sideways = -DELTA;
    }

    break;

  case VK_RIGHT:
    {
      sideways = DELTA;
    }

    break;

  case VK_UP:
    {
      forward = DELTA;
    }

    break;

  case VK_DOWN:
    {
      forward = -DELTA;
    }

    break;

  default:
    move = false;

    break;
  }

  if ( move )
  {
    m_drawingSurface->camera()->translate( forward, sideways, upwards, action, maintainAltitude );

    InvalidateRect( 0, false );
  }
}

void MapLink3DSimpleView::OnUpdateEnableLighting(CCmdUI *pCmdUI)
{
  pCmdUI->SetCheck(m_lightingEnabled);
}

void MapLink3DSimpleView::OnEnableLighting()
{
  m_lightingEnabled = !m_lightingEnabled;

  if (m_lightingEnabled)
  {
    // First, the lighting must be enabled, and then the global ambient light can
    // be set (an RGBA colour).  The default colour is (0.2, 0.2, 0.2, 1.0) - a dark grey.
    // Here it is set to a bright blue.
    m_drawingSurface->setLightingOption(TSL3DLightingOptionEnable, true);
    m_drawingSurface->globalAmbientLight(0.0, 0.0, 1.0, 1.0);

    // although this is called every time lighting is switched on the createDefaultPolygon method
    // checks to see if the default polygon already exists and only creates it if not.
    MapLink3DSimpleDoc *doc = GetDocument();
    doc->createDefaultPolygon();
  }
  else
  {
    m_drawingSurface->setLightingOption(TSL3DLightingOptionEnable, false);
  }

  // force a redraw
  Invalidate();
}

void MapLink3DSimpleView::OnCreatePolygon()
{
  MapLink3DSimpleDoc *doc = GetDocument();
  doc->createPolygon();
  Invalidate();
}

void MapLink3DSimpleView::OnCreateExtrudedPolygon()
{
  MapLink3DSimpleDoc *doc = GetDocument();
  doc->createExtrudedPolygon();
  Invalidate();
}
