// EditorSampleView.cpp : implementation of the CEditorSampleView class
//

#include "stdafx.h"
#include "EditorSample.h"
#include "EditorSampleDoc.h"
#include "EditorSampleView.h"

#include "TSLViewModeEdit.h"
#include "TSLViewModePan.h"
#include "TSLViewModeZoom.h"
#include "TSLViewModeMagnify.h"
#include "TSLViewModeInterfaceNT.h"
#include "OpMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditorSampleView

IMPLEMENT_DYNCREATE(CEditorSampleView, CView)

BEGIN_MESSAGE_MAP(CEditorSampleView, CView)
      //{{AFX_MSG_MAP(CEditorSampleView)
	ON_COMMAND(ID_TOOLS_ZOOMIN, OnToolsZoomin)
	ON_COMMAND(ID_TOOLS_ZOOMOUT, OnToolsZoomout)
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_TOOLS_PAN, OnToolsPan)
	ON_COMMAND(ID_TOOLS_ZOOM, OnToolsZoom)
	ON_COMMAND(ID_TOOLS_EDIT, OnToolsEdit)  
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEWHEEL()
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_TOOLS_MAGNIFY, OnToolsMagnify)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_TOOLS_RESET, OnToolsReset)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	//}}AFX_MSG_MAP

      // Our common handles can update multiple items.
	ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOM,    OnUpdateToolsMode)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_PAN,     OnUpdateToolsMode)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_EDIT,    OnUpdateToolsMode)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_MAGNIFY, OnUpdateToolsMode)

      // Add the middle button handlers since DevStudio doesn't 
      // give us the option.
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONDBLCLK()
	ON_WM_MBUTTONUP()

      // Standard printing commands
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditorSampleView construction/destruction

CEditorSampleView::CEditorSampleView() : m_resetRequired(false)
{
  // Clear my internal variables
  m_drawingSurface = 0 ; 
  m_currentMode    = 0 ;
  m_editMode       = 0 ;
  m_modeInterface  = 0 ;
  m_printing  = false ;

  m_zoomCursor = AfxGetApp()->LoadCursor( IDC_ZOOM_CURSOR ) ;
  m_panCursor  = AfxGetApp()->LoadStandardCursor( IDC_SIZEALL ) ;
}

CEditorSampleView::~CEditorSampleView()
{
  CEditorSampleDoc * doc = GetDocument() ;

  if ( m_drawingSurface && doc )
    doc->detachFromSurface( m_drawingSurface ) ;

  if ( m_currentMode && m_currentMode != m_editMode )
    delete m_currentMode ;

  m_currentMode = 0 ;

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
  if ( m_editMode )
  {
    delete m_editMode ;
    m_editMode = 0 ;
  }

  if ( m_zoomCursor )
    ::DestroyCursor( m_zoomCursor ) ;
  if ( m_panCursor )
    ::DestroyCursor( m_panCursor ) ;
}


/////////////////////////////////////////////////////////////////////////////
// CEditorSampleView drawing

void CEditorSampleView::OnDraw(CDC* pDC)
{
  ASSERT( m_drawingSurface ) ;

  if ( m_drawingSurface )
  {
    RECT rect ;

    // If this is the first time we have drawn this, then reset the map.
    if ( m_resetRequired )
    {
      m_drawingSurface->reset( false ) ;
      m_resetRequired = false ;
    }

    if ( pDC->GetClipBox( &rect ) == NULLREGION )
      GetClientRect( &rect ) ;

    m_drawingSurface->drawDU( rect.left, rect.bottom, rect.right, rect.top, true ) ;

    // Don't forget to draw any echo rectangle that may be active.
    if ( m_currentMode )
      m_currentMode->OnDraw( rect.left, rect.bottom, rect.right, rect.top ) ;
  }
}

/////////////////////////////////////////////////////////////////////////////
// CEditorSampleView printing


void CEditorSampleView::OnBeginPrinting(CDC *pDC /*pDC*/, CPrintInfo *pInfo /*pInfo*/)
{
  // Flag that we are printing and store the window and user unit
  // extents.  This is necessary since the extent of the printer is
  // different from the extent of the window.  We override it in the
  // OnPrint method and put the saved extents back in the OnEndPrinting method.
  m_printing = true ;
  if ( m_drawingSurface )
  {
    // Turn off the double buffering to stop huge buffers being created
    m_drawingSurface->setOption( TSLOptionDoubleBuffered, false ) ;
    m_drawingSurface->setDataLayerProps( CEditorSampleDoc::m_mapLayerName, TSLPropertyBuffered, false ) ;
    m_drawingSurface->getUUExtent( &m_uux1, &m_uuy1, &m_uux2, &m_uuy2 ) ;
    m_drawingSurface->getDUExtent( &m_scrx1, &m_scry1, &m_scrx2, &m_scry2 ) ;
  }
}

void CEditorSampleView::OnEndPrinting(CDC *pDC /*pDC*/, CPrintInfo *pInfo /*pInfo*/)
{
  if ( m_drawingSurface )
  {
    // Put the extents back the way they were before we were printing.
    m_drawingSurface->resize( m_uux1, m_uuy1, m_uux2, m_uuy2, false ) ;
    m_drawingSurface->wndResize( m_scrx1, m_scry1, m_scrx2, m_scry2, false ) ;
    m_drawingSurface->setOption( TSLOptionDoubleBuffered, true ) ;
    m_drawingSurface->setDataLayerProps( CEditorSampleDoc::m_mapLayerName, TSLPropertyBuffered, true ) ;
  }
  
  m_printing = false ;
}

/////////////////////////////////////////////////////////////////////////////
// CEditorSampleView diagnostics

#ifdef _DEBUG
void CEditorSampleView::AssertValid() const
{
  CView::AssertValid();
}

void CEditorSampleView::Dump(CDumpContext& dc) const
{
  CView::Dump(dc);
}

CEditorSampleDoc* CEditorSampleView::GetDocument() // non-debug version is inline
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEditorSampleDoc)));
  return (CEditorSampleDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEditorSampleView message handlers

void CEditorSampleView::OnInitialUpdate() 
{
  CView::OnInitialUpdate();
 
  // In an SDI application, this method is called every time a document is
  // loaded.  The first time ever, there will be no drawing surface.
  // If we have no drawing surface, create it, otherwise let the operation manager
  // know that we have changed the data displayed in the edit layer.
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

    CEditorSampleDoc * doc = GetDocument() ;

    if ( doc->addToSurface( m_drawingSurface ) )
    {
      // Set an initial view, but also flag that this must happen again in the OnDraw
      // by settings the m_resetRequired flag. This is necessary since the toolbar
      // positioning negotiation seems to happen after this method has been called.
      m_drawingSurface->reset( false ) ;
    }

    m_modeInterface = new TSLViewModeInterfaceNT( m_drawingSurface, m_hWnd ) ;
    m_modeInterface->setResetCB( resetCB, static_cast<void *>(this) ) ;

    m_editMode = new TSLViewModeEdit( m_modeInterface, ID_TOOLS_EDIT ) ;


    m_resetRequired = true ;

    // Now initialise the attributes
    TSLAttributeData initialAttributes ;

    TSLProfileHelper::setDefaultSection( "initialAttributes" ) ;

    TSLProfileHelper::lookupProfile( "edgeColour",   &initialAttributes.m_lineColour, 1 ) ;
    TSLProfileHelper::lookupProfile( "edgeStyle",    &initialAttributes.m_lineStyle,  1 ) ;
    TSLProfileHelper::lookupProfile( "edgeWidth",    &initialAttributes.m_lineThickness,  1 ) ;
    TSLProfileHelper::lookupProfile( "fillColour",   &initialAttributes.m_fillColour, 1 ) ;
    TSLProfileHelper::lookupProfile( "fillStyle",    &initialAttributes.m_fillStyle,  1 ) ;

    TSLProfileHelper::lookupProfile( "textColour",   &initialAttributes.m_textColour, 1 ) ;
    TSLProfileHelper::lookupProfile( "textFont",     &initialAttributes.m_textFont,   1 ) ;
    TSLProfileHelper::lookupProfile( "textHeight",   &initialAttributes.m_textHeight, 2000 ) ;
    if(initialAttributes.m_textHeight < 100)///< if the height in the config file is not entered with the 100 factor 
      initialAttributes.m_textHeight *= 100;

    int val = 0 ;
    TSLProfileHelper::lookupProfile( "textHorizontalAlignment", &val, TSLHorizontalAlignmentLeft ) ;
    initialAttributes.m_textHAlignment = static_cast< TSLHorizontalAlignment >( val ) ;

    TSLProfileHelper::lookupProfile( "textVerticalAlignment",   &val, TSLVerticalAlignmentMiddle ) ;
    initialAttributes.m_textVAlignment = static_cast< TSLVerticalAlignment >( val ) ;

    TSLProfileHelper::lookupProfile( "textBackgroundMode", &val, -1 ) ;
    initialAttributes.m_textBackgroundMode = static_cast< TSLTextBackgroundMode >( val ) ;
    TSLProfileHelper::lookupProfile( "textBackgroundStyle", &initialAttributes.m_textBackgroundStyle, -1 ) ;
    TSLProfileHelper::lookupProfile( "textBackgroundColour", &initialAttributes.m_textBackgroundColour, -1 ) ;
    TSLProfileHelper::lookupProfile( "textBackgroundEdgeColour", &initialAttributes.m_textBackgroundEdgeColour, 1 ) ;
 
    TSLProfileHelper::lookupProfile( "symbolColour", &initialAttributes.m_symbolColour, 1 ) ;
    TSLProfileHelper::lookupProfile( "symbolStyle",  &initialAttributes.m_symbolStyle,  1 ) ;
    TSLProfileHelper::lookupProfile( "symbolSize",   &initialAttributes.m_symbolSize,   2000 ) ;     

    COpMgr::attributeSet( &initialAttributes ) ;

    COpMgr::equal( ID_CONSTRAINT_EQUAL ) ;
  }
  else
  {
    COpMgr::dataChanged() ;
  }

  // Allow the user to create a drawing, even when there is no map displayed
  allowEdit( true ) ;

  // Reset the current interaction mode by deleting it.
  if ( m_currentMode != m_editMode )
  {
    if ( m_currentMode )
      delete m_currentMode ;
    m_currentMode = m_editMode ;
  }
}

void CEditorSampleView::OnToolsZoomin() 
{
  if ( m_drawingSurface )
  {
    // Zoom in by 30%, and request a window redraw
    m_drawingSurface->zoom( 30, true, false ) ;
    InvalidateRect( 0, false ) ;
  }
}

void CEditorSampleView::OnToolsZoomout() 
{
  if ( m_drawingSurface )
  {
    // Zoom out by 30%, and request a window redraw
    m_drawingSurface->zoom( 30, false, false ) ;
    InvalidateRect( 0, false ) ;
  }
}

void CEditorSampleView::OnToolsReset() 
{
  if ( m_drawingSurface )
  {
    CEditorSampleDoc * doc = GetDocument() ;

    TSLEnvelope extent = doc->getResetArea( ) ;

    double x1, y1, x2, y2 ;

    m_drawingSurface->TMCToUU( extent.xMin(), extent.yMin(), &x1, &y1 ) ;
    m_drawingSurface->TMCToUU( extent.xMax(), extent.yMax(), &x2, &y2 ) ;

    // Reset the window to display the reset area, and request a window redraw.
    m_drawingSurface->resize( x1, y1, x2, y2, false, true ) ;
    InvalidateRect( 0, false ) ;
  }
}

void CEditorSampleView::OnSize(UINT nType, int cx, int cy) 
{
  CView::OnSize(nType, cx, cy);
  
  if ( m_drawingSurface && !m_printing )
  {
    // Inform the drawing surface of the new window size,
    // attempting to keep the top left corner the same.
    // Do not ask for an automatic redraw since we will get an OnDraw call
    m_drawingSurface->wndResize( 0, 0, cx, cy, false, 
				 TSLResizeActionMaintainTopLeft ) ;

    // Let the current mode know that the window has changed size.  This is
    // especially necessary for the magnifying glass.
    if ( m_currentMode )
    {
      m_currentMode->OnSize( cx, cy );
    }
  }
}

void CEditorSampleView::OnMouseMove(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_currentMode )
  {
    TSLViewModeButton button = TSLViewModeNone ;

    if ( nFlags & MK_LBUTTON )
      button = TSLViewModeLeft ;
    else if ( nFlags & MK_MBUTTON )
      button = TSLViewModeMiddle ;
    else if ( nFlags & MK_RBUTTON )
      button = TSLViewModeRight ;

    if ( m_currentMode->OnMouseMove( button, point.x, point.y,
		  		     nFlags & MK_SHIFT, nFlags & MK_CONTROL ) )
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( 0, false ) ;
    }
  }
    
  CView::OnMouseMove(nFlags, point);
}

void CEditorSampleView::OnLButtonDown(UINT nFlags, CPoint point) 
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

void CEditorSampleView::OnMButtonDown(UINT nFlags, CPoint point) 
{
#ifdef PASS_MIDDLE_BUTTON_TO_MODE
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
#else
  double x, y ;
  if ( m_drawingSurface->DUToUU( point.x, point.y, &x, &y ) )
  {
    if ( m_drawingSurface->pan( x, y, false ) )
      InvalidateRect( NULL, FALSE );
  }
#endif
  CView::OnMButtonDown(nFlags, point);
}

void CEditorSampleView::OnRButtonDown(UINT nFlags, CPoint point) 
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

void CEditorSampleView::OnLButtonUp(UINT nFlags, CPoint point) 
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

void CEditorSampleView::OnMButtonUp(UINT nFlags, CPoint point) 
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

void CEditorSampleView::OnRButtonUp(UINT nFlags, CPoint point) 
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

void CEditorSampleView::OnToolsPan() 
{
  // If pan mode is currently active, then just activate edit mode, 
  // otherwise deactivate any current mode and activate pan mode
  bool active = m_currentMode && m_currentMode->id() == ID_TOOLS_PAN ;

  if ( m_currentMode != m_editMode )
  {
    if ( m_currentMode )
      delete m_currentMode ;
    m_currentMode = 0 ;
  }

  // Pass the menu item ID to the mode so that the update handler works
  if ( !active )
    m_currentMode = new TSLViewModePan( m_modeInterface, ID_TOOLS_PAN ) ;
  else
    m_currentMode = m_editMode ;
}

void CEditorSampleView::OnToolsZoom() 
{
  // If zoom mode is currently active, then just deactivate it,
  // otherwise deactivate any current mode and activate zoom mode
  bool active = m_currentMode && m_currentMode->id() == ID_TOOLS_ZOOM ;

  if ( m_currentMode != m_editMode )
  {
    delete m_currentMode ;
    m_currentMode = 0 ;
  }

  // Pass the menu item ID to the mode so that the update handler works
  if ( !active )
    m_currentMode = new TSLViewModeZoom( m_modeInterface, ID_TOOLS_ZOOM ) ;
  else
    m_currentMode = m_editMode ;
}

void CEditorSampleView::OnToolsEdit() 
{
  // If edit mode is not currently active, then just deactivate any 
  // current mode and activate edit mode
  if ( m_currentMode != m_editMode )
  {
    if ( m_currentMode )
      delete m_currentMode ;
    m_currentMode = m_editMode ;
  }
}

void CEditorSampleView::OnUpdateToolsMode(CCmdUI* pCmdUI) 
{
  // Common update handler to all items that act as radios dependant upon the current mode.
  pCmdUI->Enable( true  ) ;
  pCmdUI->SetCheck( m_currentMode && m_currentMode->id() == pCmdUI->m_nID ) ;
}

void CEditorSampleView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
  // Just handle like a normal press to avoid losing the second press
  OnLButtonDown( nFlags, point ) ;
  CView::OnLButtonDblClk(nFlags, point);
}

void CEditorSampleView::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
  // Just handle like a normal press to avoid losing the second press
  OnRButtonDown( nFlags, point ) ;
  CView::OnRButtonDblClk(nFlags, point);
}
void CEditorSampleView::OnMButtonDblClk(UINT nFlags, CPoint point) 
{
  // Just handle like a normal press to avoid losing the second press
  OnMButtonDown( nFlags, point ) ;
  CView::OnMButtonDblClk(nFlags, point);
}

void CEditorSampleView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
  HDC printerDC = pDC->GetSafeHdc()  ;
 
  if ( m_drawingSurface ) 
  {
    // If we are printing the current view, then we need to
    // resize the window according to the paper sizes. 
    CRect prc = pInfo->m_rectDraw ;
    
    // We have two options here, either draw the current view, or draw the entire map layer
#define USE_CURRENT_VIEW
#ifdef USE_CURRENT_VIEW
    //makes the area we zoom to squarred.
    double xrange  = m_uux2 - m_uux1 ;
    double yrange  = m_uuy2 - m_uuy1 ; 
    double centreX = m_uux1 + xrange / 2 ;
    double centreY = m_uuy1 + yrange / 2 ;
#else
    //makes the area we zoom to squarred
    TSLMapDataLayer * map = const_cast<TSLMapDataLayer *>(GetDocument()->getMapLayer()) ;
    double x1, y1, x2, y2 ;
    map->getUUExtent( &x1, &y1, &x2, &y2, m_drawingSurface ) ;
    double xrange  = x2 - x1 ;
    double yrange  = y2 - y1 ; 
    double centreX = x1 + xrange / 2 ;
    double centreY = y1 + yrange / 2 ;
#endif
    double requestedRatio = xrange /yrange ;

    // Ensure keep aspect ratio, obtained from the window extent 
    double wndRatio = fabs( (double)prc.Width() / (double)prc.Height() ) ;
   
    if ( wndRatio < requestedRatio )
      yrange = xrange / wndRatio ;
    else
      xrange = yrange * wndRatio ;
  
    double xMin = centreX - xrange/2 ;
    double yMin = centreY - yrange/2 ;
    double xMax = centreX + xrange/2 ;
    double yMax = centreY + yrange/2 ;
    
    // Now tell the drawing surface that the window and view have resized,
    // and draw it to the printers DC
    m_drawingSurface->wndResize ( 0, 0, prc.Width(), prc.Height(), false ) ;
    m_drawingSurface->resize(  xMin, yMin, xMax, yMax, false ) ; 	
    m_drawingSurface->drawToHDC( (TSLDeviceContext)printerDC , xMin, yMin, xMax, yMax, false ) ;
    
    pInfo->m_bContinuePrinting = false ;
  }
}

BOOL CEditorSampleView::OnEraseBkgnd(CDC* pDC) 
{
  // Override this since the drawing surface is double buffered.
  return true;
}

void CEditorSampleView::resetCB(void * self)
{
  // This method is triggered when a drawing mode wants to deactivate itself.
  CEditorSampleView * view = reinterpret_cast<CEditorSampleView *>(self) ;

  if ( view->m_currentMode != view->m_editMode )
  {
    if ( view->m_currentMode )
      delete view->m_currentMode ;

    view->m_currentMode = view->m_editMode ;
  }
}

BOOL CEditorSampleView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
  ScreenToClient(&pt);

  // Zoom in or out depending upon the wheel direction
  if ( m_drawingSurface->zoom( 30.0, zDelta > 0, false ) )
    InvalidateRect( NULL, FALSE );	
	
  return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CEditorSampleView::allowEdit( bool status )
{
  if ( m_editMode )
  {
    m_editMode->allowEdit( status ) ;
    COpMgr::allowEdit( status ) ;
  }
}

BOOL CEditorSampleView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
  // Set the cursor according to the current interaction mode
  if ( m_currentMode->id() == ID_TOOLS_ZOOM )
  {
    ::SetCursor( m_zoomCursor ) ;
    return TRUE ;
  }

  if ( m_currentMode->id() == ID_TOOLS_PAN )
  {
    ::SetCursor( m_panCursor ) ;
    return TRUE ;
  }

  return CView::OnSetCursor( pWnd, nHitTest, message ) ;
}

const char * CEditorSampleView::queryPrompt( )
{
  return m_currentMode ? m_currentMode->queryPrompt() : "" ;
}


void CEditorSampleView::OnToolsMagnify() 
{
  bool active = m_currentMode && m_currentMode->id() == ID_TOOLS_MAGNIFY ;
	
  if ( m_currentMode )
  {
    if ( m_currentMode != m_editMode )
    {
      if ( m_currentMode )
	delete m_currentMode ;
      m_currentMode = 0 ;
    }
		
    InvalidateRect( 0, false );
  }
	
  // Pass the menu item ID to the mode so that the update handler works
  if ( !active )
    m_currentMode = new TSLViewModeMagnify( m_modeInterface, ID_TOOLS_MAGNIFY ) ;
  else
    m_currentMode = m_editMode ;
}


BOOL CEditorSampleView::OnPreparePrinting(CPrintInfo* pInfo) 
{
  pInfo->SetMaxPage(1);
  return DoPreparePrinting(pInfo);
}

void CEditorSampleView::OnFilePrint() 
{
  m_printing = true ; // Stop OnSize changing aspect ratio when attribute panel disappears	
  CView::OnFilePrint() ;
}

void CEditorSampleView::OnFilePrintPreview() 
{
  m_printing = true ; // Stop OnSize changing aspect ratio when attribute panel disappears
  CView::OnFilePrintPreview() ;
}
