// Simple3DInteractionView.cpp : implementation of the Simple3DInteractionView class
//

#include "stdafx.h"
#include "Simple3DInteraction.h"
#include "Simple3DInteractionDoc.h"
#include "Simple3DInteractionView.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Simple3DInteractionView

IMPLEMENT_DYNCREATE(Simple3DInteractionView, CView)

BEGIN_MESSAGE_MAP(Simple3DInteractionView, CView)
  //{{AFX_MSG_MAP(Simple3DInteractionView)
  ON_COMMAND(ID_TOOLS_ZOOMIN, OnToolsZoomin)
  ON_COMMAND(ID_TOOLS_ZOOMOUT, OnToolsZoomout)
  ON_COMMAND(ID_TOOLS_RESET, OnToolsReset)
  ON_WM_SIZE()
  ON_WM_MOUSEMOVE()
  ON_WM_LBUTTONDOWN()
  ON_WM_RBUTTONDOWN()
  ON_WM_LBUTTONUP()
  ON_WM_RBUTTONUP()
  ON_WM_LBUTTONDBLCLK()
  ON_WM_RBUTTONDBLCLK()
  ON_COMMAND(ID_TOOLS_TRACKBALLWORLD, OnToolsTrackballWorld)
  ON_COMMAND(ID_TOOLS_TRACKBALLEYEPOINT, OnToolsTrackballEyepoint)
  ON_WM_ERASEBKGND()
  //}}AFX_MSG_MAP

  // Our common handlers can update multiple items.
  ON_UPDATE_COMMAND_UI(ID_TOOLS_TRACKBALLWORLD, OnUpdateToolsMode)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_TRACKBALLEYEPOINT, OnUpdateToolsMode)


  // update status bar
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_LAT, OnUpdateLat)
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_LON, OnUpdateLon )
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_ALT, OnUpdateAlt )

  // Add the middle button handlers since DevStudio doesn't 
  // give us the option.
  ON_WM_MBUTTONDOWN()
  ON_WM_MBUTTONDBLCLK()
  ON_WM_MBUTTONUP()
  ON_WM_MOUSEWHEEL()
  ON_WM_CHAR()

  ON_COMMAND(ID_SAVEDVIEWS_GOTO1, OnSavedviewsGoto1)
  ON_COMMAND(ID_SAVEDVIEWS_GOTO2, OnSavedviewsGoto2)
  ON_COMMAND(ID_SAVEDVIEWS_GOTO3, OnSavedviewsGoto3)
  ON_COMMAND(ID_SAVEDVIEWS_RESET, OnSavedviewsReset)
  ON_COMMAND(ID_SAVEDVIEWS_SET1, OnSavedviewsSet1)
  ON_COMMAND(ID_SAVEDVIEWS_SET2, OnSavedviewsSet2)
  ON_COMMAND(ID_SAVEDVIEWS_SET3, OnSavedviewsSet3)
  ON_COMMAND(ID_VIEWSTACK_NEXTVIEW, OnViewstackNextview)
  ON_COMMAND(ID_VIEWSTACK_PREVIOUSVIEW, OnViewstackPreviousview)
  ON_UPDATE_COMMAND_UI(ID_SAVEDVIEWS_GOTO1, OnUpdateSavedviewsGoto1)
  ON_UPDATE_COMMAND_UI(ID_SAVEDVIEWS_GOTO2, OnUpdateSavedviewsGoto2)
  ON_UPDATE_COMMAND_UI(ID_SAVEDVIEWS_GOTO3, OnUpdateSavedviewsGoto3)
  ON_UPDATE_COMMAND_UI(ID_SAVEDVIEWS_RESET, OnUpdateSavedviewsReset)
  ON_UPDATE_COMMAND_UI(ID_VIEWSTACK_NEXTVIEW, OnUpdateViewstackNextview)
  ON_UPDATE_COMMAND_UI(ID_VIEWSTACK_PREVIOUSVIEW, OnUpdateViewstackPreviousview)
  ON_COMMAND(ID_TOOLS_EXAGGERATETERRAIN, OnToolsExaggerateterrain)
  ON_COMMAND(ID_TOOLS_WIREFRAME, OnToolsWireframe)
  ON_COMMAND(ID_TOOLS_LIMITCAMERA, OnToolsLimitcamera)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Simple3DInteractionView construction/destruction

Simple3DInteractionView::Simple3DInteractionView()
{
  // Clear my internal variables
  m_drawingSurface = 0 ; 
  m_modeManager    = 0 ;
  m_exaggerateTerrain = false;
  m_limitCameraHeight = false;
}

Simple3DInteractionView::~Simple3DInteractionView()
{
  if ( m_modeManager )
  {
    delete m_modeManager ;
    m_modeManager = 0 ;
  }

  if ( m_drawingSurface )
  {
    delete m_drawingSurface ;
    m_drawingSurface = 0 ;
  }
  TSLDrawingSurface::cleanup() ;
}

/////////////////////////////////////////////////////////////////////////////
// Simple3DInteractionView drawing

void Simple3DInteractionView::OnDraw(CDC* pDC)
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
    if ( m_modeManager )
      m_modeManager->onDraw( rect.left, rect.bottom, rect.right, rect.top ) ;
  }
}

/////////////////////////////////////////////////////////////////////////////
// Simple3DInteractionView diagnostics

#ifdef _DEBUG
void Simple3DInteractionView::AssertValid() const
{
  CView::AssertValid();
}

void Simple3DInteractionView::Dump(CDumpContext& dc) const
{
  CView::Dump(dc);
}

Simple3DInteractionDoc* Simple3DInteractionView::GetDocument() // non-debug version is inline
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Simple3DInteractionDoc)));
  return (Simple3DInteractionDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// Simple3DInteractionView message handlers

void Simple3DInteractionView::OnInitialUpdate() 
{
  CView::OnInitialUpdate();

  Simple3DInteractionDoc * doc = GetDocument() ;

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
    m_drawingSurface->setOption( TSLOptionDoubleBuffered, true ) ;
    m_drawingSurface->setOption( TSLOptionDynamicArcSupportEnabled, true ) ;
    m_drawingSurface->wndResize( 0, 0, rect.Width(), rect.Height() ) ;

    // Give the 'sky' a colour!
    static const int skyColourIndex( TSLDrawingSurface::getIDOfNearestColour( 0, 0, 128 ) );
    static int const wireframeColourIndex(  TSLDrawingSurface::getIDOfNearestColour( 255, 0, 0 )  );
    static int const solidColourIndex( TSLDrawingSurface::getIDOfNearestColour( 51, 153, 255 ) ) ;

    m_drawingSurface->setBackgroundColour(  skyColourIndex );

    m_backdrop = TSLUtilityFunctions::getMapLinkHome();
    m_backdrop += "\\config\\earth.png";

    // Set the bitmap to display over the terrain plus colours for solid-backdrop and wireframe rendering.
    m_drawingSurface->setTerrainRendering( wireframeColourIndex, solidColourIndex, TSLUTF8Encoder(m_backdrop) );

    // Reduce the swimming of map textures displayed in the distance.
    m_drawingSurface->setAnisotropicFilter(true);
    m_drawingSurface->setAnisotropicFilterLevel(4.0);

    // Now create and initialse the mode manager and modes
    m_modeManager = new TSL3DInteractionModeManagerNT( this, m_drawingSurface, m_hWnd ) ;
    m_modeManager->addMode( new TSL3DInteractionModeTrackballWorld( ID_TOOLS_TRACKBALLWORLD, 30 ), false ) ;
    m_modeManager->addMode( new TSL3DInteractionModeTrackballEyepoint( ID_TOOLS_TRACKBALLEYEPOINT, 30 ), true ) ;
  }
  else
  {
    // We have a drawing surface with a map loaded into it.
    // Detach the map from the surface
    if ( m_drawingSurface && doc )
      doc->detachFromSurface( m_drawingSurface ) ;
  }

  // If the document loaded successfully, add it to the drawing surface
  doc->addToSurface( m_drawingSurface ) ;

  // and reset the current view to display the entire map.
  // This is the value by which to look ahead.
#define DEFAULT_LATITUDE   (50.0)
#define DEFAULT_LONGITUDE  (0.0)
#define DEFAULT_ALTITUDE   (8000000.0)

#define LOOK_AHEAD_OFFSET (0.0)

  m_drawingSurface->camera()->reset();
  m_drawingSurface->camera()->moveTo( DEFAULT_LATITUDE, DEFAULT_LONGITUDE, DEFAULT_ALTITUDE, TSL3DCameraMoveActionNone ) ;
  m_drawingSurface->camera()->lookAt( DEFAULT_LATITUDE, LOOK_AHEAD_OFFSET, 0.0, false ) ;

  // Set my rendering callback to trigger an update
  m_drawingSurface->setRenderingCallback( renderingCallback, this ) ;

  m_modeManager->resetViews( ) ;
  m_modeManager->resetMode( TSLButtonNone, 0, 0 ) ;

  // Store this view as the default.
  m_modeManager->setDefaultView( ) ;
}

void Simple3DInteractionView::OnToolsZoomin() 
{
  if ( m_modeManager )
  {
    if ( m_modeManager->zoomIn( 30 ) )
      InvalidateRect( 0, false ) ;
  }
}

void Simple3DInteractionView::OnToolsZoomout() 
{
  if ( m_modeManager )
  {
    if ( m_modeManager->zoomOut( 30 ) )
      InvalidateRect( 0, false ) ;
  }
}

void Simple3DInteractionView::OnToolsReset() 
{
  if ( m_modeManager )
  {
    if ( m_modeManager->resetToDefaultView( ) )
      InvalidateRect( 0, false ) ;
  }

}

void Simple3DInteractionView::OnSize(UINT nType, int cx, int cy) 
{
  CView::OnSize(nType, cx, cy);

  if ( m_drawingSurface )
  {
    // Inform the drawing surface of the new window size,
    // attempting to keep the top left corner the same.
    // Do not ask for an automatic redraw since we will get an OnDraw call
    m_drawingSurface->wndResize( 0, 0, cx, cy, false ) ;

    if ( m_modeManager )
      m_modeManager->onSize( cx, cy ) ;
  }
}

void Simple3DInteractionView::OnMouseMove(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_modeManager )
  {
    TSLButtonType button = TSLButtonNone ;

    if ( nFlags & MK_LBUTTON )
      button = TSLButtonLeft ;
    else if ( nFlags & MK_MBUTTON )
      button = TSLButtonCentre ;
    else if ( nFlags & MK_RBUTTON )
      button = TSLButtonRight ;

    if ( m_modeManager->onMouseMove( button, point.x, point.y,
      nFlags & MK_SHIFT, nFlags & MK_CONTROL ) )
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( 0, false ) ;
    }
  }  
  CView::OnMouseMove(nFlags, point);
}

void Simple3DInteractionView::OnLButtonDown(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_modeManager )
  {
    if ( m_modeManager->onLButtonDown( point.x, point.y,
      nFlags & MK_SHIFT, nFlags & MK_CONTROL ) )
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( 0, false ) ;
    }
  }

  CView::OnLButtonDown(nFlags, point);
}

void Simple3DInteractionView::OnMButtonDown(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_modeManager )
  {
    if ( m_modeManager->onMButtonDown( point.x, point.y,
      nFlags & MK_SHIFT, nFlags & MK_CONTROL ) )
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( 0, false ) ;
    }
  }

  CView::OnMButtonDown(nFlags, point);
}

void Simple3DInteractionView::OnRButtonDown(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_modeManager )
  {
    if ( m_modeManager->onRButtonDown( point.x, point.y,
      nFlags & MK_SHIFT, nFlags & MK_CONTROL ) )
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( 0, false ) ;
    }
  }

  CView::OnRButtonDown(nFlags, point);
}

void Simple3DInteractionView::OnLButtonUp(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_modeManager )
  {
    if ( m_modeManager->onLButtonUp( point.x, point.y,
      nFlags & MK_SHIFT, nFlags & MK_CONTROL ) )
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( 0, false ) ;
    }
  }

  CView::OnLButtonUp(nFlags, point);
}

void Simple3DInteractionView::OnMButtonUp(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_modeManager )
  {
    if ( m_modeManager->onMButtonUp( point.x, point.y,
      nFlags & MK_SHIFT, nFlags & MK_CONTROL ) )
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( 0, false ) ;
    }
  }


  CView::OnMButtonUp(nFlags, point);
}

void Simple3DInteractionView::OnRButtonUp(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_modeManager )
  {
    if ( m_modeManager->onRButtonUp( point.x, point.y,
      nFlags & MK_SHIFT, nFlags & MK_CONTROL ) )
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( 0, false ) ;
    }
  }


  CView::OnRButtonUp(nFlags, point);
}

void Simple3DInteractionView::OnToolsTrackballWorld() 
{
  if ( m_modeManager )
    m_modeManager->setCurrentMode( ID_TOOLS_TRACKBALLWORLD ) ;

}

void Simple3DInteractionView::OnToolsTrackballEyepoint() 
{
  if ( m_modeManager )
    m_modeManager->setCurrentMode( ID_TOOLS_TRACKBALLEYEPOINT ) ;

}

void Simple3DInteractionView::OnUpdateToolsMode(CCmdUI* pCmdUI) 
{
  // Common update handler to all items that are dependant upon the map
  // being loaded, and act as radios dependant upon the current mode.
  pCmdUI->SetCheck( m_modeManager && m_modeManager->getCurrentMode() == pCmdUI->m_nID ) ;
}

void Simple3DInteractionView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
  // Just handle like a normal press to avoid losing the second press
  OnLButtonDown( nFlags, point ) ;
  CView::OnLButtonDblClk(nFlags, point);
}

void Simple3DInteractionView::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
  // Just handle like a normal press to avoid losing the second press
  OnRButtonDown( nFlags, point ) ;
  CView::OnRButtonDblClk(nFlags, point);
}
void Simple3DInteractionView::OnMButtonDblClk(UINT nFlags, CPoint point) 
{
  // Just handle like a normal press to avoid losing the second press
  OnMButtonDown( nFlags, point ) ;
  CView::OnMButtonDblClk(nFlags, point);
}

BOOL Simple3DInteractionView::OnEraseBkgnd(CDC* pDC) 
{
  // Since the drawing surface is double buffered, we don't need to erase the
  // background.
  return true;
}

void Simple3DInteractionView::resetMode(TSL3DInteractionMode * mode, TSLButtonType button, TSLDeviceUnits xDU, TSLDeviceUnits yDU)
{
  // Ignore the override
}

void Simple3DInteractionView::viewChanged(TSL3DDrawingSurface* drawingSurface)
{
  static int cnt = 1 ;

  // TRACE1( "View change %d\n", cnt++ ) ;
}

void Simple3DInteractionView::OnSavedviewsGoto1()
{
  if ( m_modeManager )
  {
    if ( m_modeManager->savedViewGoto( 0 ) )
      InvalidateRect( 0, false ) ;
  }
}

void Simple3DInteractionView::OnSavedviewsGoto2()
{
  if ( m_modeManager )
  {
    if ( m_modeManager->savedViewGoto( 1 ) )
      InvalidateRect( 0, false ) ;
  }
}

void Simple3DInteractionView::OnSavedviewsGoto3()
{
  if ( m_modeManager )
  {
    if ( m_modeManager->savedViewGoto( 2 ) )
      InvalidateRect( 0, false ) ;
  }
}

void Simple3DInteractionView::OnSavedviewsReset()
{
  if ( m_modeManager )
    m_modeManager->savedViewReset( ) ;
}

void Simple3DInteractionView::OnSavedviewsSet1()
{
  if ( m_modeManager )
    m_modeManager->savedViewSetToCurrent( 0 ) ;
}

void Simple3DInteractionView::OnUpdateSavedviewsReset(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( true ) ;
}

void Simple3DInteractionView::OnUpdateSavedviewsGoto3(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( m_modeManager && m_modeManager->savedViewValid( 2 ) ) ;
}

void Simple3DInteractionView::OnUpdateSavedviewsGoto2(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( m_modeManager && m_modeManager->savedViewValid( 1 ) ) ;
}

void Simple3DInteractionView::OnUpdateSavedviewsGoto1(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( m_modeManager && m_modeManager->savedViewValid( 0 ) ) ;
}

void Simple3DInteractionView::OnSavedviewsSet2()
{
  if ( m_modeManager )
    m_modeManager->savedViewSetToCurrent( 1 ) ;
}

void Simple3DInteractionView::OnSavedviewsSet3()
{
  if ( m_modeManager )
    m_modeManager->savedViewSetToCurrent( 2 ) ;
}

void Simple3DInteractionView::OnViewstackNextview()
{
  if ( m_modeManager )
  {
    if ( m_modeManager->viewStackGotoNext( ) )
      InvalidateRect( 0, false ) ;
  }
}

void Simple3DInteractionView::OnUpdateViewstackNextview(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( (m_modeManager && m_modeManager->viewStackNextValid( )) ? true : false ) ;
}

void Simple3DInteractionView::OnViewstackPreviousview()
{
  if ( m_modeManager )
  {
    if ( m_modeManager->viewStackGotoPrevious( ) )
      InvalidateRect( 0, false ) ;
  }
}

void Simple3DInteractionView::OnUpdateViewstackPreviousview(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( (m_modeManager && m_modeManager->viewStackPreviousValid( )) ? true : false ) ;
}

BOOL Simple3DInteractionView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
  if ( m_modeManager )
  {
    if ( m_modeManager->onMouseWheel( zDelta, pt.x, pt.y ) )
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( 0, false ) ;
    }
  }

  return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void Simple3DInteractionView::renderingCallback(void * arg, int pendingTextures)
{
  Simple3DInteractionView * view = (Simple3DInteractionView *)arg ;

  if ( view->m_hWnd )
  {
    view->Invalidate() ;
  }
}

const char * Simple3DInteractionView::queryPrompt( )
{
  return m_modeManager ? m_modeManager->queryPrompt( ) : "" ;
}

void Simple3DInteractionView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
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

void Simple3DInteractionView::OnUpdateLat(CCmdUI* pCmdUI)
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

void Simple3DInteractionView::OnUpdateLon(CCmdUI* pCmdUI)
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

void Simple3DInteractionView::OnUpdateAlt(CCmdUI* pCmdUI)
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

void Simple3DInteractionView::OnToolsExaggerateterrain()
{
  m_exaggerateTerrain = !m_exaggerateTerrain;
  if (m_exaggerateTerrain)
  {
    m_drawingSurface->exaggerateTerrain( 10.0, 10.0 );
  }
  else
  {
    m_drawingSurface->exaggerateTerrain( 1.0, 1.0 );
  }
  InvalidateRect( 0, false );
}

void Simple3DInteractionView::OnToolsWireframe()
{
  bool wireframe = m_drawingSurface->get3DOption( TSL3DOptionWireframeMode );
  m_drawingSurface->set3DOption( TSL3DOptionWireframeMode, !wireframe );

  InvalidateRect( 0, false );
}

void Simple3DInteractionView::OnToolsLimitcamera()
{
  // TODO: Add your command handler code here
  TSL3DCamera *camera = m_drawingSurface->camera();
  if (camera)
  {
    m_limitCameraHeight = !m_limitCameraHeight;
    camera->limitHeightAboveEarth (m_limitCameraHeight, 100.0, true);
  }
}
