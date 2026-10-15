// SatellitePropagator3DView.cpp : implementation of the SatellitePropagator3DView class
//

#include "stdafx.h"
#include "SatellitePropagator3D.h"
#include "SatellitePropagator3DDoc.h"
#include "SatellitePropagator3DView.h"
#include "MainFrm.h"
#include "tsl3dpickmode.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern SatellitePropagator3DApp theApp ;

/////////////////////////////////////////////////////////////////////////////
// SatellitePropagator3DView

IMPLEMENT_DYNCREATE(SatellitePropagator3DView, CView)

BEGIN_MESSAGE_MAP(SatellitePropagator3DView, CView)
//{{AFX_MSG_MAP(SatellitePropagator3DView)
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
  ON_UPDATE_COMMAND_UI(ID_TOOLS_PICK,    OnUpdateToolsMode)

  // Add the middle button handlers since DevStudio doesn't 
  // give us the option.
  ON_WM_MBUTTONDOWN()
  ON_WM_MBUTTONDBLCLK()
  ON_WM_MBUTTONUP()
  ON_WM_MOUSEWHEEL()

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
  ON_COMMAND(ID_TOOLS_PICK, OnToolsPick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SatellitePropagator3DView construction/destruction

SatellitePropagator3DView::SatellitePropagator3DView()
{
  // Clear my internal variables
  m_drawingSurface = 0 ; 
  m_modeManager    = 0 ;
}

SatellitePropagator3DView::~SatellitePropagator3DView()
{
  // Delete the contents first
  SatellitePropagator3DDoc * doc = GetDocument() ;
  doc->DeleteContents() ;

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
// SatellitePropagator3DView drawing

void SatellitePropagator3DView::OnDraw(CDC* pDC)
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
// SatellitePropagator3DView diagnostics

#ifdef _DEBUG
void SatellitePropagator3DView::AssertValid() const
{
  CView::AssertValid();
}

void SatellitePropagator3DView::Dump(CDumpContext& dc) const
{
  CView::Dump(dc);
}

SatellitePropagator3DDoc* SatellitePropagator3DView::GetDocument() // non-debug version is inline
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(SatellitePropagator3DDoc)));
  return (SatellitePropagator3DDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// SatellitePropagator3DView message handlers

void SatellitePropagator3DView::OnInitialUpdate() 
{
  CView::OnInitialUpdate();
  
  SatellitePropagator3DDoc * doc = GetDocument() ;
  
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

    m_backdrop.SetString( (LPCTSTR)TSLUTF8Decoder(TSLUtilityFunctions::getMapLinkHome()) );
    m_backdrop += _T("\\config\\earth.png");

    // Set the bitmap to display over the terrain plus colours for solid-backdrop and wireframe rendering.
    m_drawingSurface->setTerrainRendering( wireframeColourIndex, solidColourIndex, TSLUTF8Encoder(m_backdrop) );

    // Now create and initialse the mode manager and modes
    m_modeManager = new TSL3DInteractionModeManagerNT( this, m_drawingSurface, m_hWnd ) ;
    m_modeManager->addMode( new TSL3DInteractionModeTrackballEyepoint( ID_TOOLS_TRACKBALLEYEPOINT ), true ) ;
    m_modeManager->addMode( new TSL3DInteractionModeTrackballWorld( ID_TOOLS_TRACKBALLWORLD ), false ) ;
    m_modeManager->addMode( new TSL3DPickMode( ID_TOOLS_PICK ), false ) ;
  }
  else
  {
    // We have a drawing surface with a map loaded into it.
    // Detach the map from the surface
    if ( m_drawingSurface && doc )
      doc->detachFromSurface( m_drawingSurface ) ;
  }

  // Get the time server
  CMainFrame * mainFrame = (CMainFrame *)theApp.GetMainWnd() ;
  TSLTimeServer * timeServer = mainFrame->getTimeServer( ) ;

  // If the document loaded successfully, add it to the drawing surface
  doc->addToSurface( m_drawingSurface, timeServer ) ;
 
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

void SatellitePropagator3DView::OnToolsZoomin() 
{
  if ( m_modeManager )
  {
    if ( m_modeManager->zoomIn( 30 ) )
      InvalidateRect( 0, false ) ;
  }
}

void SatellitePropagator3DView::OnToolsZoomout() 
{
  if ( m_modeManager )
  {
    if ( m_modeManager->zoomOut( 30 ) )
      InvalidateRect( 0, false ) ;
  }
}

void SatellitePropagator3DView::OnToolsReset() 
{
  if ( m_modeManager )
  {
    if ( m_modeManager->resetToDefaultView( ) )
      InvalidateRect( 0, false ) ;
  }

}

void SatellitePropagator3DView::OnSize(UINT nType, int cx, int cy) 
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

void SatellitePropagator3DView::OnMouseMove(UINT nFlags, CPoint point) 
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

void SatellitePropagator3DView::OnLButtonDown(UINT nFlags, CPoint point) 
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

void SatellitePropagator3DView::OnMButtonDown(UINT nFlags, CPoint point) 
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

void SatellitePropagator3DView::OnRButtonDown(UINT nFlags, CPoint point) 
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

void SatellitePropagator3DView::OnLButtonUp(UINT nFlags, CPoint point) 
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

void SatellitePropagator3DView::OnMButtonUp(UINT nFlags, CPoint point) 
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

void SatellitePropagator3DView::OnRButtonUp(UINT nFlags, CPoint point) 
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

void SatellitePropagator3DView::OnToolsTrackballWorld() 
{
  if ( m_modeManager )
    m_modeManager->setCurrentMode( ID_TOOLS_TRACKBALLWORLD ) ;

}

void SatellitePropagator3DView::OnToolsTrackballEyepoint() 
{
  if ( m_modeManager )
    m_modeManager->setCurrentMode( ID_TOOLS_TRACKBALLEYEPOINT ) ;

}

void SatellitePropagator3DView::OnUpdateToolsMode(CCmdUI* pCmdUI) 
{
  // Common update handler to all items that are dependant upon the map
  // being loaded, and act as radios dependant upon the current mode.
  pCmdUI->SetCheck( m_modeManager && m_modeManager->getCurrentMode() == pCmdUI->m_nID ) ;
}

void SatellitePropagator3DView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
  // Just handle like a normal press to avoid losing the second press
  OnLButtonDown( nFlags, point ) ;
  CView::OnLButtonDblClk(nFlags, point);
}

void SatellitePropagator3DView::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
  // Just handle like a normal press to avoid losing the second press
  OnRButtonDown( nFlags, point ) ;
  CView::OnRButtonDblClk(nFlags, point);
}
void SatellitePropagator3DView::OnMButtonDblClk(UINT nFlags, CPoint point) 
{
  // Just handle like a normal press to avoid losing the second press
  OnMButtonDown( nFlags, point ) ;
  CView::OnMButtonDblClk(nFlags, point);
}

BOOL SatellitePropagator3DView::OnEraseBkgnd(CDC* pDC) 
{
  // Since the drawing surface is double buffered, we don't need to erase the
  // background.
  return true;
}

void SatellitePropagator3DView::resetMode(TSL3DInteractionMode * mode, TSLButtonType button, TSLDeviceUnits xDU, TSLDeviceUnits yDU)
{
  // Ignore the override
}

void SatellitePropagator3DView::viewChanged(TSL3DDrawingSurface* drawingSurface)
{
  static int cnt = 1 ;

 // TRACE1( "View change %d\n", cnt++ ) ;
}

void SatellitePropagator3DView::OnSavedviewsGoto1()
{
  if ( m_modeManager )
  {
    if ( m_modeManager->savedViewGoto( 0 ) )
      InvalidateRect( 0, false ) ;
  }
}

void SatellitePropagator3DView::OnSavedviewsGoto2()
{
  if ( m_modeManager )
  {
    if ( m_modeManager->savedViewGoto( 1 ) )
      InvalidateRect( 0, false ) ;
  }
}

void SatellitePropagator3DView::OnSavedviewsGoto3()
{
  if ( m_modeManager )
  {
    if ( m_modeManager->savedViewGoto( 2 ) )
      InvalidateRect( 0, false ) ;
  }
}

void SatellitePropagator3DView::OnSavedviewsReset()
{
  if ( m_modeManager )
    m_modeManager->savedViewReset( ) ;
}

void SatellitePropagator3DView::OnSavedviewsSet1()
{
  if ( m_modeManager )
    m_modeManager->savedViewSetToCurrent( 0 ) ;
}

void SatellitePropagator3DView::OnUpdateSavedviewsReset(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( true ) ;
}

void SatellitePropagator3DView::OnUpdateSavedviewsGoto3(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( m_modeManager && m_modeManager->savedViewValid( 2 ) ) ;
}

void SatellitePropagator3DView::OnUpdateSavedviewsGoto2(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( m_modeManager && m_modeManager->savedViewValid( 1 ) ) ;
}

void SatellitePropagator3DView::OnUpdateSavedviewsGoto1(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( m_modeManager && m_modeManager->savedViewValid( 0 ) ) ;
}

void SatellitePropagator3DView::OnSavedviewsSet2()
{
  if ( m_modeManager )
    m_modeManager->savedViewSetToCurrent( 1 ) ;
}

void SatellitePropagator3DView::OnSavedviewsSet3()
{
  if ( m_modeManager )
    m_modeManager->savedViewSetToCurrent( 2 ) ;
}

void SatellitePropagator3DView::OnViewstackNextview()
{
  if ( m_modeManager )
  {
    if ( m_modeManager->viewStackGotoNext( ) )
      InvalidateRect( 0, false ) ;
  }
}

void SatellitePropagator3DView::OnUpdateViewstackNextview(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( (m_modeManager && m_modeManager->viewStackNextValid( )) ? true : false ) ;
}

void SatellitePropagator3DView::OnViewstackPreviousview()
{
  if ( m_modeManager )
  {
    if ( m_modeManager->viewStackGotoPrevious( ) )
      InvalidateRect( 0, false ) ;
  }
}

void SatellitePropagator3DView::OnUpdateViewstackPreviousview(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( (m_modeManager && m_modeManager->viewStackPreviousValid( )) ? true : false ) ;
}

BOOL SatellitePropagator3DView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
  ScreenToClient(&pt);

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

void SatellitePropagator3DView::renderingCallback(void * arg, int pendingTextures)
{
  SatellitePropagator3DView * view = (SatellitePropagator3DView *)arg ;

  if ( view->m_hWnd )
  {
    view->Invalidate() ;
  }
}

const char * SatellitePropagator3DView::queryPrompt( )
{
  return m_modeManager ? m_modeManager->queryPrompt( ) : "" ;
}

void SatellitePropagator3DView::OnToolsPick()
{
 if ( m_modeManager )
    m_modeManager->setCurrentMode( ID_TOOLS_PICK ) ;
}
