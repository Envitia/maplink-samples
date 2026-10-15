// GeometryCreationView.cpp : implementation of the CGeometryCreationView class
//

#include "stdafx.h"
#include "GeometryCreation.h"

#include "GeometryCreationDoc.h"
#include "GeometryCreationView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

TSLViewModeInterfaceNT CGeometryCreationView::m_modeInterface ;
TSLViewMode * CGeometryCreationView::m_currentMode ;
bool CGeometryCreationView::m_showExtents ;
bool CGeometryCreationView::m_uniqueID ;

/////////////////////////////////////////////////////////////////////////////
// CGeometryCreationView

IMPLEMENT_DYNCREATE(CGeometryCreationView, CView)

BEGIN_MESSAGE_MAP(CGeometryCreationView, CView)
//{{AFX_MSG_MAP(CGeometryCreationView)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_COMMAND(ID_TOOLS_GRAB, OnToolsGrab)
	ON_COMMAND(ID_TOOLS_ZOOMIN, OnToolsZoomin)
	ON_COMMAND(ID_TOOLS_RESET, OnToolsReset)
	ON_COMMAND(ID_TOOLS_ZOOMOUT, OnToolsZoomout)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_TOOLS_PAN, OnToolsPan)
	ON_COMMAND(ID_TOOLS_ZOOM, OnToolsZoom)
	ON_COMMAND(ID_TOOLS_ZOOMPAN, OnToolsZoompan)
	ON_COMMAND(ID_TOOLS_EDITPROPERTIES, OnToolsEditproperties)
	ON_COMMAND(ID_TESTS_SHOWENTITYEXTENTS, OnTestsShowentityextents)
	ON_UPDATE_COMMAND_UI(ID_TESTS_SHOWENTITYEXTENTS, OnUpdateTestsShowentityextents)
	ON_COMMAND(ID_TESTS_UNIQUEIDFOREACHSURFACE, OnTestsUniqueidforeachsurface)
	ON_UPDATE_COMMAND_UI(ID_TESTS_UNIQUEIDFOREACHSURFACE, OnUpdateTestsUniqueidforeachsurface)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOMIN, OnUpdateToolsAction)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOMOUT, OnUpdateToolsAction)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_RESET, OnUpdateToolsAction)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_GRAB, OnUpdateToolsMode)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_PAN,  OnUpdateToolsMode)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOM, OnUpdateToolsMode)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_EDITPROPERTIES, OnUpdateToolsMode)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOMPAN, OnUpdateToolsMode)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGeometryCreationView construction/destruction

CGeometryCreationView::CGeometryCreationView()
{
  // Clear my internal variables
  m_drawingSurface = 0 ; 
}

CGeometryCreationView::~CGeometryCreationView()
{
  if ( m_drawingSurface )
  {
    delete m_drawingSurface ;
    m_drawingSurface = 0 ;
  }

  if ( m_currentMode )
  {
    delete m_currentMode ;
    m_currentMode = 0 ;
  }
}

BOOL CGeometryCreationView::PreCreateWindow(CREATESTRUCT& cs)
{
  // TODO: Modify the Window class or styles here by modifying
  //  the CREATESTRUCT cs
  
  return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CGeometryCreationView drawing

void CGeometryCreationView::OnDraw(CDC* pDC)
{
  ASSERT( m_drawingSurface ) ;
  
  if ( m_drawingSurface )
  {
    RECT rect ;

    // Set up unique id's if required, otherwise all have same id
    if ( m_uniqueID )
      m_drawingSurface->id( (long)this ) ;
    else
      m_drawingSurface->id( -1 ) ;
    
    // Only draw the exposed bits of the display
    if ( pDC->GetClipBox( &rect ) == NULLREGION )
      GetClientRect( &rect ) ;
    
    m_drawingSurface->drawDU( rect.left, rect.bottom, rect.right, rect.top, true ) ;

    // Draw around extent of every object in the layer
    if ( m_showExtents )
    {
      TSLEntitySet * entities = GetDocument()->entities() ;

      pDC->SelectStockObject( HOLLOW_BRUSH ) ;
      pDC->SelectStockObject( BLACK_PEN ) ;

      for ( int i = 0 ; i < entities->size() ; ++i )
      {
        TSLEntity * entity = (*entities)[i] ;
        TSLEnvelope extent = entity->envelope( m_drawingSurface->id() ) ;
        TSLDeviceUnits x1, y1, x2, y2 ;

        m_drawingSurface->TMCToDU( extent.xMin(), extent.yMin(), &x1, &y1 ) ;
        m_drawingSurface->TMCToDU( extent.xMax(), extent.yMax(), &x2, &y2 ) ;
        pDC->Rectangle( x1, y1, x2, y2 ) ;
      }
    }

    // Don't forget to draw any echo rectangle that may be active.
    if ( m_modeInterface.attached( m_drawingSurface ) )
      m_modeInterface.drawEchoRectangle( ) ;
  }
}

/////////////////////////////////////////////////////////////////////////////
// CGeometryCreationView diagnostics

#ifdef _DEBUG
void CGeometryCreationView::AssertValid() const
{
  CView::AssertValid();
}

void CGeometryCreationView::Dump(CDumpContext& dc) const
{
  CView::Dump(dc);
}

CGeometryCreationDoc* CGeometryCreationView::GetDocument() // non-debug version is inline
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGeometryCreationDoc)));
  return (CGeometryCreationDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGeometryCreationView message handlers

void CGeometryCreationView::OnInitialUpdate() 
{
  CView::OnInitialUpdate();
  
  CGeometryCreationDoc * doc = GetDocument() ;
  
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
  }
  
  doc->addToSurface( m_drawingSurface ) ;
  
  // and reset the current view to display the entire file
  m_drawingSurface->reset( false ) ;    
}

BOOL CGeometryCreationView::OnEraseBkgnd(CDC* pDC) 
{
  // Since the drawing surface is double buffered, we don't need to erase the
  // background.
  return true;
}

void CGeometryCreationView::OnSize(UINT nType, int cx, int cy) 
{
  CView::OnSize(nType, cx, cy);
  
  if ( m_drawingSurface )
  {
    // Inform the drawing surface of the new window size,
    // attempting to keep the centre the same.
    // Do not ask for an automatic redraw since we will get an OnDraw call
    m_drawingSurface->wndResize( 0, 0, cx, cy, false, 
				 TSLResizeActionMaintainCentre ) ;
  }
}

void CGeometryCreationView::OnSetFocus(CWnd* pOldWnd) 
{
  CView::OnSetFocus(pOldWnd);

  m_modeInterface.attach( m_drawingSurface, m_hWnd, GetDocument() ) ;
}

void CGeometryCreationView::OnKillFocus(CWnd* pNewWnd) 
{
  CView::OnKillFocus(pNewWnd);

  m_modeInterface.attach( 0, 0, 0 ) ;
}

void CGeometryCreationView::OnUpdateToolsMode(CCmdUI* pCmdUI) 
{
  // Common update handler to all items that are dependant upon the document loaded
  // and act as radios dependent upon the current mode.
  if ( m_modeInterface.attached() )
  {
    pCmdUI->Enable( true ) ;
    pCmdUI->SetCheck( m_currentMode && m_currentMode->id() == pCmdUI->m_nID ) ;
  }
  else
    pCmdUI->Enable( false ) ;
}

void CGeometryCreationView::OnUpdateToolsAction(CCmdUI* pCmdUI) 
{
  // Common update handler to all items that are dependant a tmf file displayed
  pCmdUI->Enable( m_modeInterface.attached() ) ;
}


void CGeometryCreationView::OnToolsGrab() 
{
  // If grab mode is currently active, then just deactivate it,
  // otherwise deactivate any current mode and activate pan mode
  bool active = m_currentMode && m_currentMode->id() == ID_TOOLS_GRAB ;
  
  if ( m_currentMode )
  {
    delete m_currentMode ;
    m_currentMode = 0 ;
  }
  
  // Pass the menu item ID to the mode so that the update handler works
  if ( !active )
    m_currentMode = new TSLViewModeGrab( &m_modeInterface, ID_TOOLS_GRAB ) ;
}

void CGeometryCreationView::OnToolsZoomin() 
{
  if ( m_drawingSurface )
  {
    // Zoom in by 30%, and request a window redraw
    if ( !m_drawingSurface->zoom( 30, true, false ) )
    {
      ::AfxMessageBox( _T("Cannot zoom in any further"), MB_OK ) ;
      return ;
    }
    redraw( ) ;
  }
}

void CGeometryCreationView::OnToolsReset() 
{
  if ( m_drawingSurface )
  {
    // Reset the window to display the entire map, and request a window redraw.
    m_drawingSurface->reset( false ) ;
    redraw( ) ;
  }
}

void CGeometryCreationView::OnToolsZoomout() 
{
  if ( m_drawingSurface )
  {
    // Zoom out by 30%, and request a window redraw
    if ( !m_drawingSurface->zoom( 30, false, false ) )
    {
      ::AfxMessageBox( _T("Cannot zoom out any further"), MB_OK ) ;
      return ;
    }
    redraw( ) ;
  }
}

void CGeometryCreationView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
  // Just handle like a normal press to avoid losing the second press
  OnLButtonDown( nFlags, point ) ;
  CView::OnLButtonDblClk(nFlags, point);
}

void CGeometryCreationView::OnLButtonDown(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_currentMode )
  {
    if ( m_currentMode->OnLButtonDown( point.x, point.y,
				       nFlags & MK_SHIFT, nFlags & MK_CONTROL ) )
    {
      // Request a redraw if the interaction hander requires it
      redraw( ) ;
    }
  }
  
  CView::OnLButtonDown(nFlags, point);
}

void CGeometryCreationView::OnLButtonUp(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_currentMode )
  {
    if ( m_currentMode->OnLButtonUp( point.x, point.y,
			             nFlags & MK_SHIFT, nFlags & MK_CONTROL ) )
    {
      // Request a redraw if the interaction hander requires it
      redraw( ) ;
    }
  }
  
  CView::OnLButtonUp(nFlags, point);
}

void CGeometryCreationView::OnMouseMove(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_currentMode )
  {
    if ( m_currentMode->OnMouseMove( point.x, point.y,
				     nFlags & MK_SHIFT, nFlags & MK_CONTROL ) )
    {
      // Request a redraw if the interaction hander requires it
      redraw( ) ;
    }
  }
  
  CView::OnMouseMove(nFlags, point);
}

BOOL CGeometryCreationView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
  // TODO: Add your message handler code here and/or call default
  
  return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CGeometryCreationView::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
  // Just handle like a normal press to avoid losing the second press
  OnRButtonDown( nFlags, point ) ;
  CView::OnRButtonDblClk(nFlags, point);
}

void CGeometryCreationView::OnRButtonDown(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_currentMode )
  {
    if ( m_currentMode->OnRButtonDown( point.x, point.y,
				       nFlags & MK_SHIFT, nFlags & MK_CONTROL ) )
    {
      // Request a redraw if the interaction hander requires it
      redraw( ) ;
    }
  }
  
  CView::OnRButtonDown(nFlags, point);
}

void CGeometryCreationView::OnRButtonUp(UINT nFlags, CPoint point) 
{

  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_currentMode )
  {
    if ( m_currentMode->OnRButtonUp( point.x, point.y,
				     nFlags & MK_SHIFT, nFlags & MK_CONTROL ) )
    {
      // Request a redraw if the interaction hander requires it
      redraw( ) ;
    }
  }
    
  CView::OnRButtonUp(nFlags, point);
}

void CGeometryCreationView::OnToolsPan() 
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
    m_currentMode = new TSLViewModePan( &m_modeInterface, ID_TOOLS_PAN ) ;
}

void CGeometryCreationView::OnToolsZoom() 
{
  // If pan mode is currently active, then just deactivate it,
  // otherwise deactivate any current mode and activate pan mode
  bool active = m_currentMode && m_currentMode->id() == ID_TOOLS_ZOOM ;
  
  if ( m_currentMode )
  {
    delete m_currentMode ;
    m_currentMode = 0 ;
  }
  
  // Pass the menu item ID to the mode so that the update handler works
  if ( !active )
    m_currentMode = new TSLViewModeZoom( &m_modeInterface, ID_TOOLS_ZOOM ) ;
}

void CGeometryCreationView::OnToolsZoompan() 
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
    m_currentMode = new TSLViewModeZoomPan( &m_modeInterface, ID_TOOLS_ZOOMPAN ) ;
}

void CGeometryCreationView::OnToolsEditproperties() 
{
  // If edit properties mode is currently active, then just deactivate it,
  // otherwise deactivate any current mode and activate edit properties mode
  bool active = m_currentMode && m_currentMode->id() == ID_TOOLS_EDITPROPERTIES ;
  
  if ( m_currentMode )
  {
    delete m_currentMode ;
    m_currentMode = 0 ;
  }
  
  // Pass the menu item ID to the mode so that the update handler works
  if ( !active )
    m_currentMode = new TSLViewModeEditProps( &m_modeInterface, ID_TOOLS_EDITPROPERTIES ) ;
}

void CGeometryCreationView::redraw( )
{
  if ( m_showExtents )
    GetDocument()->UpdateAllViews( 0 ) ;
  else
    InvalidateRect( 0, FALSE ) ;
}

void CGeometryCreationView::updateEntityExtent( TSLEntity * entity, const char *layerName )
{
  if ( entity && m_drawingSurface )
  {
    m_drawingSurface->updateEntityExtent(entity, layerName);
  }
}

void CGeometryCreationView::OnTestsShowentityextents() 
{
  m_showExtents = !m_showExtents ;
  InvalidateRect( 0, FALSE ) ;
}

void CGeometryCreationView::OnUpdateTestsShowentityextents(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( true ) ;
  pCmdUI->SetCheck( m_showExtents ) ;
}

void CGeometryCreationView::OnTestsUniqueidforeachsurface() 
{
  m_uniqueID = !m_uniqueID ;
}

void CGeometryCreationView::OnUpdateTestsUniqueidforeachsurface(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( true ) ;
  pCmdUI->SetCheck( m_uniqueID ) ;
}
