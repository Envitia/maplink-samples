// C2DemoView.cpp : implementation of the CC2DemoView class
//

#include "stdafx.h"
#include "C2DemoView.h"
#include "C2DemoDoc.h"
#include "resource.h"
#include "TrackManager.h"
#include "Track.h"
#include "Animator.h"

#include "tslviewmodeinterfacent.h"
#include "tslviewmodepan.h"
#include "tslviewmodezoom.h"
#include "tslviewmodezoompan.h"
#include "tslviewmodeselect.h"
#include "tslviewmodegrab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const char * CC2DemoView::m_mapLayerName = "map" ;
const char * CC2DemoView::m_standardLayerName = "standard" ;
const char * CC2DemoView::m_objectLayerName = "object" ;

/////////////////////////////////////////////////////////////////////////////
// CC2DemoView

IMPLEMENT_DYNCREATE(CC2DemoView, CView)

BEGIN_MESSAGE_MAP(CC2DemoView, CView)
      //{{AFX_MSG_MAP(CC2DemoView)
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP

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
// CC2DemoView construction/destruction

CC2DemoView::CC2DemoView()
{
  // Clear my internal variables
  m_drawingSurface  = 0 ; 
  m_currentMode	    = 0 ;
  m_modeInterface   = 0 ;
  m_mapLoaded = false ;
  m_printing  = false ;

  m_zoomCursor	    = AfxGetApp()->LoadCursor( IDC_CURSOR_ZOOM ) ;
  m_panCursor	    = AfxGetApp()->LoadCursor( IDC_CURSOR_PAN ) ;
  m_selectCursor    = AfxGetApp()->LoadCursor( IDC_CURSOR_SELECT ) ;
  m_dragCursor	    = AfxGetApp()->LoadCursor( IDC_CURSOR_DRAG ) ;
  m_grabCursor	    = AfxGetApp()->LoadCursor( IDC_CURSOR_GRAB ) ;
  m_grabbingCursor  = AfxGetApp()->LoadCursor( IDC_CURSOR_GRABBING ) ;

}

CC2DemoView::~CC2DemoView()
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

BOOL CC2DemoView::PreCreateWindow(CREATESTRUCT& cs)
{
  // TODO: Modify the Window class or styles here by modifying
  //  the CREATESTRUCT cs
  
  return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CC2DemoView drawing

void CC2DemoView::OnDraw(CDC* pDC)
{
  ASSERT( m_drawingSurface ) ;

  if ( m_drawingSurface )
  {
    RECT rect ;

    // Map rotation ( if needed )
    if ( CTrackManager::trackHeadingUp() )
      m_drawingSurface->rotate( ( CTrackManager::selectedTrack()->heading() * PI ) / 180 ) ;
    else if ( CTrackManager::northUp() ) 
      m_drawingSurface->rotate( 0.0 ) ;

    // If we are following the selected track pan to keep the track in the middle
    // of the viewing area
    if ( CTrackManager::followSelectedTrack() )
    {
      double uux = 0.0 ;
      double uuy = 0.0 ;
      // Get the position of the selected track
      TSLCoord coord = CTrackManager::selectedTrack()->position() ;
      // Get the position in user units
      m_drawingSurface->TMCToUU( coord.x(), coord.y(), &uux, &uuy ) ;
      // Attempt to pan, without auto_redraw enabled.
      m_drawingSurface->pan( uux, uuy, false ) ;
    }

    // Only draw the exposed bits of the display
    if ( pDC->GetClipBox( &rect ) == NULLREGION )
      GetClientRect( &rect ) ;

    m_drawingSurface->drawDU( rect.left, rect.bottom, rect.right, rect.top, true ) ;

    // Don't forget to draw any echo rectangle that may be active.
    m_modeInterface->drawEchoRectangle( ) ;
  }
}

/////////////////////////////////////////////////////////////////////////////
// CC2DemoView printing

BOOL CC2DemoView::OnPreparePrinting(CPrintInfo* pInfo)
{
  // default preparation
  return DoPreparePrinting(pInfo);
}

void CC2DemoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
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

void CC2DemoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
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
// CC2DemoView diagnostics

#ifdef _DEBUG
void CC2DemoView::AssertValid() const
{
  CView::AssertValid();
}

void CC2DemoView::Dump(CDumpContext& dc) const
{
  CView::Dump(dc);
}

CC2DemoDoc* CC2DemoView::GetDocument() // non-debug version is inline
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CC2DemoDoc)));
  return (CC2DemoDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CC2DemoView message handlers

void CC2DemoView::OnInitialUpdate() 
{
  CView::OnInitialUpdate();
 
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
    // Remove the data layer.
    m_drawingSurface->removeDataLayer( m_mapLayerName ) ;
  }

  m_mapLoaded = false ;

  CC2DemoDoc * doc = GetDocument() ;

  TSLMapDataLayer * mapDataLayer = doc->getMapDataLayer() ;

  if ( mapDataLayer )
  {
    // If the document loaded successfully, add it to the drawing surface
    // and reset the current view to display the entire map.
    m_drawingSurface->addDataLayer( mapDataLayer, m_mapLayerName ) ;
    // Set the map data layer properties
    m_drawingSurface->setDataLayerProps( m_mapLayerName, TSLPropertyBuffered, 1 ) ;
    m_drawingSurface->reset( false ) ;
    m_mapLoaded = true ;
  }

  TSLStandardDataLayer * standardDataLayer = doc->getStandardDataLayer() ;

  if ( standardDataLayer )
  {
    // If the document loaded successfully add the standard data layer
    // to the drawing surface.
    m_drawingSurface->addDataLayer( standardDataLayer, m_standardLayerName ) ;
  }

  TSLObjectDataLayer * objectDataLayer = doc->getObjectDataLayer() ;

  if ( objectDataLayer )
  {
    // If the document loaded successfully add the object data layer
    // to the drawing surface
    m_drawingSurface->addDataLayer( objectDataLayer, m_objectLayerName ) ;
  }

  // Reset the drawing surface
  m_drawingSurface->reset( false ) ;

  // Start the animation
  CAnimator * pAnimator = doc->getAnimator() ;
  
  if ( pAnimator ) 
  {
    pAnimator->startAnimation() ;
  }
    
  // Reset the current interaction mode by deleting it.
  if ( m_currentMode )
  {
    delete m_currentMode ;
    m_currentMode = 0 ;
  }
  setViewMode( ID_TOOLS_TRACKSELECT ) ;
}

void CC2DemoView::OnSize(UINT nType, int cx, int cy) 
{
  CView::OnSize(nType, cx, cy);
  
  if ( m_drawingSurface && !m_printing )
  {
    // Inform the drawing surface of the new window size,
    // attempting to keep the top left corner the same.
    // Do not ask for an automatic redraw since we will get an OnDraw call
    m_drawingSurface->wndResize( 0, 0, cx, cy, false, 
				 TSLResizeActionMaintainTopLeft ) ;
  }
}

void CC2DemoView::OnMouseMove(UINT nFlags, CPoint point) 
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
    // If this is in the middle of a drag operation the cursor 
    // wont be redrawn so we'll force it to set it now
    if ( m_currentMode->id() == ID_TOOLS_TRACKSELECT_DRAG )
      ::SetCursor( m_dragCursor ) ;
  }
    
  CView::OnMouseMove(nFlags, point);
}

void CC2DemoView::OnLButtonDown(UINT nFlags, CPoint point) 
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
    // If we are in track select mode set the mouse capture 
    // in case we want to drag a track onto the Tote
    if ( m_currentMode->id() == ID_TOOLS_TRACKSELECT ) 
      this->SetCapture() ;  
  }
  
  CView::OnLButtonDown(nFlags, point);
}

void CC2DemoView::OnMButtonDown(UINT nFlags, CPoint point) 
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

void CC2DemoView::OnRButtonDown(UINT nFlags, CPoint point) 
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

void CC2DemoView::OnLButtonUp(UINT nFlags, CPoint point) 
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
    // If we are in track select mode reset the mouse capture
    if ( m_currentMode->id() == ID_TOOLS_TRACKSELECT )
      ::ReleaseCapture() ;
  }
	
  CView::OnLButtonUp(nFlags, point);
}

void CC2DemoView::OnMButtonUp(UINT nFlags, CPoint point) 
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

void CC2DemoView::OnRButtonUp(UINT nFlags, CPoint point) 
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

void CC2DemoView::OnEditCopy() 
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

void CC2DemoView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
  // Just handle like a normal press to avoid losing the second press
  OnLButtonDown( nFlags, point ) ;
  CView::OnLButtonDblClk(nFlags, point);
}

void CC2DemoView::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
  // Just handle like a normal press to avoid losing the second press
  OnRButtonDown( nFlags, point ) ;
  CView::OnRButtonDblClk(nFlags, point);
}
void CC2DemoView::OnMButtonDblClk(UINT nFlags, CPoint point) 
{
  // Just handle like a normal press to avoid losing the second press
  OnMButtonDown( nFlags, point ) ;
  CView::OnMButtonDblClk(nFlags, point);
}

void CC2DemoView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
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

BOOL CC2DemoView::OnEraseBkgnd(CDC* pDC) 
{
  // Since the drawing surface is double buffered, we don't need to erase the
  // background.
  return true;
}

void CC2DemoView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
  if ( lHint == HINT_REDRAW_MAP_DISPLAY ) 
  {
    InvalidateRect( NULL, true ) ;
  }
  else if ( lHint == HINT_UPDATE_DECLUTTER_STATUS_ON || lHint == HINT_UPDATE_DECLUTTER_STATUS_OFF )
  {
    const char * feature = m_drawingSurface->featureClass( (long)pHint ) ;

    // Toggle the state
    if ( lHint == HINT_UPDATE_DECLUTTER_STATUS_ON )
    {
      m_drawingSurface->setDeclutterStatus( feature, TSLDeclutterStatusOn ) ;
    }
    else if ( lHint == HINT_UPDATE_DECLUTTER_STATUS_OFF )
    {
      m_drawingSurface->setDeclutterStatus( feature, TSLDeclutterStatusOff ) ;
    }

    m_drawingSurface->redraw() ;
    Invalidate() ;
  }

  else if ( lHint == HINT_UPDATE_VIRTUAL_DECLUTTER_STATUS_ON || lHint == HINT_UPDATE_VIRTUAL_DECLUTTER_STATUS_OFF )
  {

    const char * feature = (char *) pHint ;
    
    // Toggle the state
    if ( lHint == HINT_UPDATE_VIRTUAL_DECLUTTER_STATUS_ON )
    {
      m_drawingSurface->setDeclutterStatus( feature, TSLDeclutterStatusOn ) ;
    }
    else if ( lHint == HINT_UPDATE_VIRTUAL_DECLUTTER_STATUS_OFF )
    {
      m_drawingSurface->setDeclutterStatus( feature, TSLDeclutterStatusOff ) ;
    }

    m_drawingSurface->redraw() ;
    Invalidate() ;
    
  }
  
  CView::OnUpdate( pSender, lHint, pHint ) ;

}

void CC2DemoView::setViewMode( UINT viewModeID )
{
  // If the mode is currently active, then just deactivate it,
  // otherwise deactivate any current mode and activate select mode
  bool active = m_currentMode && m_currentMode->id() == viewModeID ;

  if ( m_currentMode )
  {
    delete m_currentMode ;
    m_currentMode = 0 ;
  }

  // Pass the menu item ID to the mode so that the update handler works
  if ( !active )
  {
    switch( viewModeID )
    {
    case ID_TOOLS_TRACKSELECT :
      m_currentMode = new TSLViewModeSelect( m_modeInterface, ID_TOOLS_TRACKSELECT, m_objectLayerName ) ;
      break ;
    case ID_TOOLS_ZOOMPAN :
      m_currentMode = new TSLViewModeZoomPan( m_modeInterface, ID_TOOLS_ZOOMPAN ) ;
      break ;
    case ID_TOOLS_PAN :
      m_currentMode = new TSLViewModePan( m_modeInterface, ID_TOOLS_PAN ) ;
      break ;
    case ID_TOOLS_ZOOM :
      m_currentMode = new TSLViewModeZoom( m_modeInterface, ID_TOOLS_ZOOM ) ;
      break ;
    case ID_TOOLS_GRAB :
      m_currentMode = new TSLViewModeGrab( m_modeInterface, ID_TOOLS_GRAB ) ;
      break ;
    }
  }
}

// Return the current view mode
long CC2DemoView::getViewMode()
{
  if ( m_currentMode )
    return m_currentMode->id() ;
  else
    return 0 ;
}

BOOL CC2DemoView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
  if ( m_currentMode )
  {
    HCURSOR cursor = 0 ;

    switch ( m_currentMode->id() )
    {
      case ID_TOOLS_ZOOM :
        cursor = m_zoomCursor ; break ;

      case ID_TOOLS_PAN :
        cursor = m_panCursor ; break ;

      case ID_TOOLS_TRACKSELECT :
        cursor =  m_selectCursor ; break ;

      case ID_TOOLS_TRACKSELECT_DRAG :
        cursor =  m_dragCursor ; break ;

      case ID_TOOLS_GRAB :
        cursor =  m_grabCursor ; break ;

      case ID_TOOLS_GRABBING :
        cursor = m_grabbingCursor ; break ;
    }

    if ( cursor )
    {
      ::SetCursor( cursor ) ;
      return TRUE ;
    }
  }

  return CView::OnSetCursor(pWnd, nHitTest, message);
}

bool CC2DemoView::mapLoaded()
{
  return m_mapLoaded ;
}
