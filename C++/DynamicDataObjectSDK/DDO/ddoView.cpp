// ddoView.cpp : implementation of the CDdoView class
//

#include "stdafx.h"
#include "ddo.h"

#include "ddoDoc.h"
#include "ddoView.h"

#include "tslviewmodeinterface.h"
#include "tslviewmodeinterfacent.h"
#include "tslviewmode.h"
#include "tslviewmodepan.h"
#include "tslviewmodezoom.h"
#include "tslviewmodezoompan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const char* CDdoView::m_layerName = "map" ;
const char* CDdoView::m_odlName   = "odl" ;

/////////////////////////////////////////////////////////////////////////////
// CDdoView

IMPLEMENT_DYNCREATE(CDdoView, CView)

BEGIN_MESSAGE_MAP(CDdoView, CView)
      //{{AFX_MSG_MAP(CDdoView)
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
	ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOMIN,  OnUpdateToolsAction)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOMPAN, OnUpdateToolsMode)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
      //}}AFX_MSG_MAP

      // Our common handles can update multiple items.
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY,     OnUpdateToolsAction)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT,    OnUpdateToolsAction)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOMOUT, OnUpdateToolsAction)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_PAN,     OnUpdateToolsMode)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOM,    OnUpdateToolsMode)

      // Add the middle button handlers since DevStudio doesn't 
      // give us the option.
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONDBLCLK()
	ON_WM_MBUTTONUP()

      // Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)

  ON_COMMAND(ID_VIEW_WRAP, &CDdoView::OnViewWrap)
  ON_UPDATE_COMMAND_UI(ID_VIEW_WRAP, &CDdoView::OnUpdateViewWrap)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDdoView construction/destruction

CDdoView::CDdoView()
{
  // Clear my internal variables
  m_drawingSurface = 0 ; 
  m_currentMode    = 0 ;
  m_modeInterface  = 0 ;
  m_mapLoaded = false ;
  m_printing  = false ;
}

CDdoView::~CDdoView()
{
  if ( m_currentMode )
  {
    delete m_currentMode ;
    m_currentMode = NULL ;
  }
  if ( m_modeInterface )
  {
    delete m_modeInterface ;
    m_modeInterface = NULL ;
  }
  if ( m_drawingSurface )
  {
    delete m_drawingSurface ;
    m_drawingSurface = NULL ;
  }

  TSLDrawingSurface::cleanup() ;
}

BOOL CDdoView::PreCreateWindow(CREATESTRUCT& cs)
{
  // TODO: Modify the Window class or styles here by modifying
  //  the CREATESTRUCT cs
  
  return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDdoView drawing

void CDdoView::OnDraw(CDC* pDC)
{
  ASSERT( m_drawingSurface ) ;

  CRect rect ;

  if ( !pDC->GetClipBox( &rect ) )
    GetClientRect( &rect ) ;

  m_drawingSurface->drawDU( rect.left, rect.top, rect.right, rect.bottom, false ) ;

  // Don't forget to draw any echo rectangle that may be active.
  m_modeInterface->drawEchoRectangle( ) ;
}

/////////////////////////////////////////////////////////////////////////////
// CDdoView printing

BOOL CDdoView::OnPreparePrinting(CPrintInfo* pInfo)
{
  // default preparation
  return DoPreparePrinting(pInfo);
}

void CDdoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
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

void CDdoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
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
// CDdoView diagnostics

#ifdef _DEBUG
void CDdoView::AssertValid() const
{
  CView::AssertValid();
}

void CDdoView::Dump(CDumpContext& dc) const
{
  CView::Dump(dc);
}

CDdoDoc* CDdoView::GetDocument() // non-debug version is inline
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDdoDoc)));
  return (CDdoDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDdoView message handlers

void CDdoView::OnInitialUpdate() 
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
    m_drawingSurface->removeDataLayer( m_layerName ) ;

    // Remove the object layer.
    m_drawingSurface->removeDataLayer( m_odlName ) ;
  }

  m_mapLoaded = false ;

  CDdoDoc * doc = GetDocument() ;

  TSLMapDataLayer * dataLayer = doc->getDataLayer() ;

  if ( dataLayer )
  {
    // If the document loaded successfully, add it to the drawing surface
    // and reset the current view to display the entire map.
    m_drawingSurface->addDataLayer( dataLayer, m_layerName ) ;
    m_drawingSurface->reset( false ) ;
    m_mapLoaded = true ;
    m_drawingSurface->setDataLayerProps( m_layerName , tsl_buffered  , 1 ) ;
  }

  TSLObjectDataLayer* objectLayer = doc->getObjectLayer();

  if ( objectLayer )
  {
    m_drawingSurface->addDataLayer( objectLayer, m_odlName ) ;
   // m_drawingSurface->setDataLayerProps( m_odlName , tsl_buffered  , 1 ) ;
  }

  // Reset the current interaction mode by deleting it.
  if ( m_currentMode )
  {
    delete m_currentMode ;
    m_currentMode = NULL ;
  }
}

void CDdoView::OnToolsZoomin() 
{
  if ( m_drawingSurface )
  {
    // Zoom in by 30%, and request a window redraw
    m_drawingSurface->zoom( 30, true, false ) ;
    InvalidateRect( NULL, false ) ;
  }
}

void CDdoView::OnToolsZoomout() 
{
  if ( m_drawingSurface )
  {
    // Zoom out by 30%, and request a window redraw
    m_drawingSurface->zoom( 30, false, false ) ;
    InvalidateRect( NULL, false ) ;
  }
}

void CDdoView::OnToolsReset() 
{
  if ( m_drawingSurface )
  {
    // Reset the window to display the entire map, and request a window redraw.
    m_drawingSurface->reset( false ) ;
    InvalidateRect( NULL, false ) ;
  }
}

void CDdoView::OnSize(UINT nType, int cx, int cy) 
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

void CDdoView::OnMouseMove(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_currentMode )
  {
    if ( m_currentMode->OnMouseMove( point.x, point.y,
		  		     nFlags & MK_SHIFT, nFlags & MK_CONTROL ) )
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( NULL, false ) ;
    }
  }
    
  CView::OnMouseMove(nFlags, point);
}

void CDdoView::OnLButtonDown(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_currentMode )
  {
    if ( m_currentMode->OnLButtonDown( point.x, point.y,
		  		       nFlags & MK_SHIFT, nFlags & MK_CONTROL ) )
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( NULL, false ) ;
    }
  }
  
  CView::OnLButtonDown(nFlags, point);
}

void CDdoView::OnMButtonDown(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_currentMode )
  {
    if ( m_currentMode->OnMButtonDown( point.x, point.y,
		  		       nFlags & MK_SHIFT, nFlags & MK_CONTROL ) )
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( NULL, false ) ;
    }
  }
  
  CView::OnMButtonDown(nFlags, point);
}

void CDdoView::OnRButtonDown(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_currentMode )
  {
    if ( m_currentMode->OnRButtonDown( point.x, point.y,
		  		       nFlags & MK_SHIFT, nFlags & MK_CONTROL ) )
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( NULL, false ) ;
    }
  }
  
  CView::OnRButtonDown(nFlags, point);
}

void CDdoView::OnLButtonUp(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_currentMode )
  {
    if ( m_currentMode->OnLButtonUp( point.x, point.y,
		  		     nFlags & MK_SHIFT, nFlags & MK_CONTROL ) )
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( NULL, false ) ;
    }
  }
	
  CView::OnLButtonUp(nFlags, point);
}

void CDdoView::OnMButtonUp(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_currentMode )
  {
    if ( m_currentMode->OnMButtonUp( point.x, point.y,
		  		     nFlags & MK_SHIFT, nFlags & MK_CONTROL ) )
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( NULL, false ) ;
    }
  }
	

  CView::OnMButtonUp(nFlags, point);
}

void CDdoView::OnRButtonUp(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_currentMode )
  {
    if ( m_currentMode->OnRButtonUp( point.x, point.y,
		  		     nFlags & MK_SHIFT, nFlags & MK_CONTROL ) )
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( NULL, false ) ;
    }
  }
	
	
  CView::OnRButtonUp(nFlags, point);
}

void CDdoView::OnToolsPan() 
{
  // If pan mode is currently active, then just deactivate it,
  // otherwise deactivate any current mode and activate pan mode
  bool active = m_currentMode && m_currentMode->id() == ID_TOOLS_PAN ;

  if ( m_currentMode )
  {
    delete m_currentMode ;
    m_currentMode = NULL ;
  }

  // Pass the menu item ID to the mode so that the update handler works
  if ( !active )
    m_currentMode = new TSLViewModePan( m_modeInterface, ID_TOOLS_PAN ) ;
}

void CDdoView::OnToolsZoom() 
{
  // If zoom mode is currently active, then just deactivate it,
  // otherwise deactivate any current mode and activate zoom mode
  bool active = m_currentMode && m_currentMode->id() == ID_TOOLS_ZOOM ;

  if ( m_currentMode )
  {
    delete m_currentMode ;
    m_currentMode = NULL ;
  }

  // Pass the menu item ID to the mode so that the update handler works
  if ( !active )
    m_currentMode = new TSLViewModeZoom( m_modeInterface, ID_TOOLS_ZOOM ) ;
}

void CDdoView::OnToolsZoompan() 
{
  // If zoom/pan mode is currently active, then just deactivate it,
  // otherwise deactivate any current mode and activate zoom/pan mode
  bool active = m_currentMode && m_currentMode->id() == ID_TOOLS_ZOOMPAN ;

  if ( m_currentMode )
  {
    delete m_currentMode ;
    m_currentMode = NULL ;
  }

  // Pass the menu item ID to the mode so that the update handler works
  if ( !active )
    m_currentMode = new TSLViewModeZoomPan( m_modeInterface, ID_TOOLS_ZOOMPAN ) ;
}

void CDdoView::OnUpdateToolsAction(CCmdUI* pCmdUI) 
{
  // Common update handler to all items that are dependant upon the map
  // being loaded.
  pCmdUI->Enable( m_mapLoaded ? true : false ) ;
}

void CDdoView::OnUpdateToolsMode(CCmdUI* pCmdUI) 
{
  // Common update handler to all items that are dependant upon the map
  // being loaded, and act as radios dependant upon the current mode.
  pCmdUI->Enable( m_mapLoaded ? true : false  ) ;
  pCmdUI->SetCheck( m_currentMode && m_currentMode->id() == pCmdUI->m_nID ) ;
}

void CDdoView::OnEditCopy() 
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

void CDdoView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
  // Just handle like a normal press to avoid losing the second press
  OnLButtonDown( nFlags, point ) ;
  CView::OnLButtonDblClk(nFlags, point);
}

void CDdoView::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
  // Just handle like a normal press to avoid losing the second press
  OnRButtonDown( nFlags, point ) ;
  CView::OnRButtonDblClk(nFlags, point);
}
void CDdoView::OnMButtonDblClk(UINT nFlags, CPoint point) 
{
  // Just handle like a normal press to avoid losing the second press
  OnMButtonDown( nFlags, point ) ;
  CView::OnRButtonDblClk(nFlags, point);
}

void CDdoView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{  
  if ( m_drawingSurface ) 
  {
    TSLEntitySet * tmp = 0 ;
    TSLSimpleString templateFilePath;

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
    long colour    = TSLDrawingSurface::getIDOfNearestColour( 0, 0, 0 ) ;
    long style     = 1 ;
    long thickness = 3 ;
    item.setBorder( colour, style, thickness ) ;
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

    // Now print the items.

    // If we wanted to print more items, for example enlargements, then we could
    // either create a custom template with fixed position items - each with a 
    // different name, or use the TSLPrintHelper::addItemTemplate method to add
    // a dynamically created item.  This allows for the same template to be used
    TSLPrintHelper::printUsingTemplate( context, 1, &item, tmp, 1, mappings ) ;

    // Tidy up the template since we no longer need it.
    tmp->destroy() ;
    
    pInfo->m_bContinuePrinting = false ;
  }
}

void CDdoView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
  // TODO: Add your specialized code here and/or call the base class
  if ( true )
  {
    InvalidateRect( NULL, false );
  }
  else
  {
    CView::OnUpdate(pSender, lHint, pHint);
  }
}

void CDdoView::OnViewWrap()
{
  if (m_drawingSurface == nullptr)
  {
    return;
  }

  switch (m_drawingSurface->wrapAroundMode())
  {
  case TSLDrawingSurface::WrapAroundLongitudinal:
    m_drawingSurface->wrapAroundMode(TSLDrawingSurface::WrapAroundDisabled);
    break;
  default:
    m_drawingSurface->wrapAroundMode(TSLDrawingSurface::WrapAroundLongitudinal);
    break;
  }

  //InvalidateRect(0, false);
}

void CDdoView::OnUpdateViewWrap(CCmdUI* pCmdUI)
{
  if (m_drawingSurface == nullptr)
  {
    pCmdUI->Enable(FALSE);
    pCmdUI->SetCheck(FALSE);
  }
  else
  {
    pCmdUI->Enable(TRUE);
    pCmdUI->SetCheck(m_drawingSurface->wrapAroundMode() == TSLDrawingSurface::WrapAroundLongitudinal);
  }
}
