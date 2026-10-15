// WMSClientSampleView.cpp : implementation of the WMSClientSampleView class
//

#include "stdafx.h"
#include "WMSClientSample.h"

#include "WMSClientSampleDoc.h"
#include "WMSClientSampleView.h"
#include <string>

#include "optionsdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// WMSClientSampleView

IMPLEMENT_DYNCREATE(WMSClientSampleView, CView)

BEGIN_MESSAGE_MAP(WMSClientSampleView, CView)
//{{AFX_MSG_MAP(WMSClientSampleView)
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
  ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
  ON_WM_LBUTTONDBLCLK()
  ON_WM_RBUTTONDBLCLK()
  ON_WM_ERASEBKGND()
  ON_COMMAND(ID_TOOLS_GRAB, OnToolsGrab)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOMIN,  OnUpdateToolsAction)
  ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
  //}}AFX_MSG_MAP

  // Our common handlers can update multiple items.
  ON_UPDATE_COMMAND_UI(ID_EDIT_COPY,     OnUpdateToolsAction)
  ON_UPDATE_COMMAND_UI(ID_FILE_PRINT,    OnUpdateToolsAction)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOMOUT, OnUpdateToolsAction)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_PAN,     OnUpdateToolsMode)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOM,    OnUpdateToolsMode)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_GRAB,    OnUpdateToolsMode)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_MAGNIFY, OnUpdateToolsMode)

  // Add the middle button handlers since DevStudio doesn't 
  // give us the option.
  ON_WM_MBUTTONDOWN()
  ON_WM_MBUTTONDBLCLK()
  ON_WM_MBUTTONUP()

  ON_COMMAND(ID_TOOLS_MAGNIFY, OnToolsMagnify)
  ON_COMMAND(ID_TOOLS_OPTIONS, OnToolsOptions)
  ON_COMMAND(ID_SAVEDVIEWS_GOTO1, OnSavedviewsGoto1)
  ON_COMMAND(ID_SAVEDVIEWS_GOTO2, OnSavedviewsGoto2)
  ON_COMMAND(ID_SAVEDVIEWS_GOTO3, OnSavedviewsGoto3)
  ON_COMMAND(ID_SAVEDVIEWS_RESET, OnSavedviewsReset)
  ON_COMMAND(ID_SAVEDVIEWS_SET1, OnSavedviewsSet1)
  ON_UPDATE_COMMAND_UI(ID_SAVEDVIEWS_RESET, OnUpdateSavedviewsReset)
  ON_UPDATE_COMMAND_UI(ID_SAVEDVIEWS_GOTO3, OnUpdateSavedviewsGoto3)
  ON_UPDATE_COMMAND_UI(ID_SAVEDVIEWS_GOTO2, OnUpdateSavedviewsGoto2)
  ON_UPDATE_COMMAND_UI(ID_SAVEDVIEWS_GOTO1, OnUpdateSavedviewsGoto1)
  ON_UPDATE_COMMAND_UI(ID_SAVEDVIEWS_SET1, OnUpdateSavedviewsSet1)
  ON_COMMAND(ID_SAVEDVIEWS_SET2, OnSavedviewsSet2)
  ON_UPDATE_COMMAND_UI(ID_SAVEDVIEWS_SET2, OnUpdateSavedviewsSet2)
  ON_COMMAND(ID_SAVEDVIEWS_SET3, OnSavedviewsSet3)
  ON_UPDATE_COMMAND_UI(ID_SAVEDVIEWS_SET3, OnUpdateSavedviewsSet3)
  ON_COMMAND(ID_VIEWSTACK_NEXTVIEW, OnViewstackNextview)
  ON_UPDATE_COMMAND_UI(ID_VIEWSTACK_NEXTVIEW, OnUpdateViewstackNextview)
  ON_COMMAND(ID_VIEWSTACK_PREVIOUSVIEW, OnViewstackPreviousview)
  ON_UPDATE_COMMAND_UI(ID_VIEWSTACK_PREVIOUSVIEW, OnUpdateViewstackPreviousview)
  ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// WMSClientSampleView construction/destruction

WMSClientSampleView::WMSClientSampleView()
{
  // Clear my internal variables
  m_drawingSurface = 0 ; 
  m_modeManager    = 0 ;
  m_mapLoaded = false ;
}

WMSClientSampleView::~WMSClientSampleView()
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
}

/////////////////////////////////////////////////////////////////////////////
// WMSClientSampleView drawing

void WMSClientSampleView::OnDraw(CDC* pDC)
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
// WMSClientSampleView printing
BOOL WMSClientSampleView::OnPreparePrinting(CPrintInfo* pInfo)
{
  pInfo->SetMaxPage(1);
  return DoPreparePrinting(pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// WMSClientSampleView diagnostics

#ifdef _DEBUG
void WMSClientSampleView::AssertValid() const
{
  CView::AssertValid();
}

void WMSClientSampleView::Dump(CDumpContext& dc) const
{
  CView::Dump(dc);
}

WMSClientSampleDoc* WMSClientSampleView::GetDocument() // non-debug version is inline
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(WMSClientSampleDoc)));
  return (WMSClientSampleDoc*)m_pDocument;
}
#endif //_DEBUG

void WMSClientSampleView::addLayer(NamedDataLayer* layerToAdd)
{
  if (!m_mapLoaded)
  {
    OnInitialUpdate();
    return;
  }

  m_drawingSurface->addDataLayer( layerToAdd->dataLayer(), layerToAdd->genMapLinkLayerName().c_str() );
  Invalidate();
}

void WMSClientSampleView::addLayerToBackAndReset(NamedDataLayer* layerToAdd)
{
  if (!m_mapLoaded)
  {
    OnInitialUpdate();
    return;
  }

  string name = layerToAdd->genMapLinkLayerName();
  m_drawingSurface->addDataLayer( layerToAdd->dataLayer(), name.c_str() );
  m_drawingSurface->sendToBack( name.c_str() );
  if ( m_modeManager )
  {
    m_modeManager->resetToFullExtent( );
  }
  Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// WMSClientSampleView message handlers

void WMSClientSampleView::OnInitialUpdate() 
{
  CView::OnInitialUpdate();
  
  WMSClientSampleDoc * doc = GetDocument() ;
  
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

    m_drawingSurface->wrapAroundMode(TSLDrawingSurface::WrapAroundLongitudinal);

    // Now create and initialse the mode manager and modes
    m_modeManager = new TSLInteractionModeManagerNT( this, m_drawingSurface, m_hWnd, 5, 5, 30, true ) ;
    m_modeManager->addMode( new TSLInteractionModeZoom( ID_TOOLS_ZOOM ), false ) ;
    m_modeManager->addMode( new TSLInteractionModePan( ID_TOOLS_PAN ), false ) ;
    m_modeManager->addMode( new TSLInteractionModeGrab( ID_TOOLS_GRAB ), true ) ;
    m_modeManager->addMode( new TSLInteractionModeMagnify( ID_TOOLS_MAGNIFY ), false ) ;

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

  m_modeManager->resetViews( ) ;
  m_modeManager->resetMode( TSLButtonNone, 0, 0 ) ;
}

void WMSClientSampleView::OnToolsZoomin() 
{
  if ( m_modeManager )
  {
    if ( m_modeManager->zoomIn( 30 ) )
      InvalidateRect( 0, false ) ;
  }
}

void WMSClientSampleView::OnToolsZoomout() 
{
  if ( m_modeManager )
  {
    if ( m_modeManager->zoomOut( 30 ) )
      InvalidateRect( 0, false ) ;
  }
}

void WMSClientSampleView::OnToolsReset() 
{
  if ( m_modeManager )
  {
    if ( m_modeManager->resetToFullExtent( ) )
      InvalidateRect( 0, false ) ;
  }

}

void WMSClientSampleView::OnSize(UINT nType, int cx, int cy) 
{
  CView::OnSize(nType, cx, cy);
  
  if ( m_drawingSurface )
  {
    // Inform the drawing surface of the new window size,
    // attempting to keep the top left corner the same.
    // Do not ask for an automatic redraw since we will get an OnDraw call
    m_drawingSurface->wndResize( 0, 0, cx, cy, false, 
				 TSLResizeActionMaintainTopLeft) ;

    if ( m_modeManager )
      m_modeManager->onSize( cx, cy ) ;
  }
}

void WMSClientSampleView::OnMouseMove(UINT nFlags, CPoint point) 
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
                                     (nFlags & MK_SHIFT) != 0, (nFlags & MK_CONTROL) != 0 ) )
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( 0, false ) ;
    }
  }
  
  CView::OnMouseMove(nFlags, point);
}

void WMSClientSampleView::OnLButtonDown(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_modeManager )
  {
    if ( m_modeManager->onLButtonDown( point.x, point.y,
                                      (nFlags & MK_SHIFT) != 0, (nFlags & MK_CONTROL) != 0 ) )
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( 0, false ) ;
    }
  }
  
  CView::OnLButtonDown(nFlags, point);
}

void WMSClientSampleView::OnMButtonDown(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_modeManager )
  {
    if ( m_modeManager->onMButtonDown( point.x, point.y,
                                      (nFlags & MK_SHIFT) != 0, (nFlags & MK_CONTROL) != 0 ) )
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( 0, false ) ;
    }
  }
  
  CView::OnMButtonDown(nFlags, point);
}

void WMSClientSampleView::OnRButtonDown(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_modeManager )
  {
    if ( m_modeManager->onRButtonDown( point.x, point.y,
                                     (nFlags & MK_SHIFT) != 0, (nFlags & MK_CONTROL) != 0 ) )
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( 0, false ) ;
    }
  }
  
  CView::OnRButtonDown(nFlags, point);
}

void WMSClientSampleView::OnLButtonUp(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_modeManager )
  {
    if ( m_modeManager->onLButtonUp( point.x, point.y,
                                     (nFlags & MK_SHIFT) != 0, (nFlags & MK_CONTROL) != 0 ) )
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( 0, false ) ;
    }
  }
  
  CView::OnLButtonUp(nFlags, point);
}

void WMSClientSampleView::OnMButtonUp(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_modeManager )
  {
    if ( m_modeManager->onMButtonUp( point.x, point.y,
                                     (nFlags & MK_SHIFT) != 0, (nFlags & MK_CONTROL) != 0 ) )
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( 0, false ) ;
    }
  }
  
  
  CView::OnMButtonUp(nFlags, point);
}

void WMSClientSampleView::OnRButtonUp(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_modeManager )
  {
    if ( m_modeManager->onRButtonUp( point.x, point.y,
                                     (nFlags & MK_SHIFT) != 0, (nFlags & MK_CONTROL) != 0 ) )
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( 0, false ) ;
    }
  }
  
  
  CView::OnRButtonUp(nFlags, point);
}

void WMSClientSampleView::OnToolsPan() 
{
  if ( m_modeManager )
    m_modeManager->setCurrentMode( ID_TOOLS_PAN ) ;

}

void WMSClientSampleView::OnToolsZoom() 
{
  if ( m_modeManager )
    m_modeManager->setCurrentMode( ID_TOOLS_ZOOM ) ;

}

void WMSClientSampleView::OnToolsGrab() 
{
  if ( m_modeManager )
    m_modeManager->setCurrentMode( ID_TOOLS_GRAB ) ;
}

void WMSClientSampleView::OnUpdateToolsAction(CCmdUI* pCmdUI) 
{
  // Common update handler to all items that are dependant upon the map
  // being loaded.
  pCmdUI->Enable( m_mapLoaded ? true : false ) ;
}

void WMSClientSampleView::OnUpdateToolsMode(CCmdUI* pCmdUI) 
{
  // Common update handler to all items that are dependant upon the map
  // being loaded, and act as radios dependant upon the current mode.
  pCmdUI->Enable( m_mapLoaded ? true : false  ) ;
  pCmdUI->SetCheck( m_mapLoaded && m_modeManager && m_modeManager->getCurrentMode() == (long)pCmdUI->m_nID ) ;
}

void WMSClientSampleView::OnEditCopy() 
{
  // Snap shot the current view and place it on the clipboard
  CDC * pDC = GetDC() ;
  
  // Create a bitmap
  RECT rc ;
  GetClientRect ( &rc ) ;
  
  HBITMAP newBitmap = CreateCompatibleBitmap( pDC->m_hDC, rc.right, rc.bottom );
  HDC	  newDC     = CreateCompatibleDC( pDC->m_hDC );
  BOOL    opened = FALSE ;
  BOOL    closed = FALSE ;
  
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
    DeleteDC( newDC );

  ReleaseDC(pDC);
}

void WMSClientSampleView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
  // Just handle like a normal press to avoid losing the second press
  OnLButtonDown( nFlags, point ) ;
  CView::OnLButtonDblClk(nFlags, point);
}

void WMSClientSampleView::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
  // Just handle like a normal press to avoid losing the second press
  OnRButtonDown( nFlags, point ) ;
  CView::OnRButtonDblClk(nFlags, point);
}
void WMSClientSampleView::OnMButtonDblClk(UINT nFlags, CPoint point) 
{
  // Just handle like a normal press to avoid losing the second press
  OnMButtonDown( nFlags, point ) ;
  CView::OnMButtonDblClk(nFlags, point);
}

void WMSClientSampleView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
  // Attempt to print using the PrintHelper, using the standard templates.
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

    // Search for the file in the drawing surface's pathlist.
    // These files will usually be in the config directory alongside the
    // tslcolours.dat etc.
    // If found, load the template
    if ( TSLDrawingSurface::findFile( templateFile, templateFilePath ) )
    {
      tmp = TSLPrintHelper::loadTemplate( templateFilePath.c_str() ) ;
    }

    if ( !tmp || tmp->size() == 0 )
    {
      MessageBox( _T("Cannot find print template") ) ;
      return ;
    }

    // Set up a print item with a 3 pixel wide border, displaying the current extent.
    // All visible data layers in the drawing surface are used when printing.
    // To display at a specific scale, use item.centreAndScale rather than item.extent
    // There can be as many items as we need 
    // Note : Use the extent captured in the OnFilePrint, not the current extent.  This is
    //        because the extent may have changed to to WMSClientSample Print Preview taking control
    //        of the window and resizing it.
    TSLPrintItem item( m_drawingSurface ) ; // Use the default print template item name 'map'
    long colour    = TSLDrawingSurface::getIDOfNearestColour( 0, 0, 0 ) ;
    long style     = 1 ;
    long thickness = 3 ;
    item.setBorder( colour, style, thickness ) ;
    item.extent( m_extentToPrint ) ;

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

BOOL WMSClientSampleView::OnEraseBkgnd(CDC* pDC) 
{
  // Since the drawing surface is double buffered, we don't need to erase the
  // background.
  return true;
}


void WMSClientSampleView::OnFilePrint() 
{
  // We should not query the extent to print during the OnPrint, since the window may change
  // size, especially in WMSClientSample print-preview
  if ( m_drawingSurface )
    m_drawingSurface->getTMCExtent( m_extentToPrint ) ;
  CView::OnFilePrint() ;
}

void WMSClientSampleView::OnFilePrintPreview() 
{
  // We should not query the extent to print during the OnPrint, since the window may change
  // size, especially in WMSClientSample print-preview
  if ( m_drawingSurface )
    m_drawingSurface->getTMCExtent( m_extentToPrint ) ;
  CView::OnFilePrintPreview() ;
}

void WMSClientSampleView::OnToolsMagnify()
{
  if ( m_modeManager )
    m_modeManager->setCurrentMode( ID_TOOLS_MAGNIFY ) ;
}

void WMSClientSampleView::OnToolsOptions()
{
  COptionsDlg dlg(this);
  if ( dlg.DoModal() == IDOK )
  {
    WMSClientSampleDoc * doc = GetDocument() ;
    doc->refreshAllSettings();
  }
}

void WMSClientSampleView::resetMode(TSLInteractionMode * mode, TSLButtonType button, TSLDeviceUnits xDU, TSLDeviceUnits yDU)
{
  // Ignore the override
}

void WMSClientSampleView::viewChanged(TSLDrawingSurface* drawingSurface)
{
 
}

void WMSClientSampleView::OnSavedviewsGoto1()
{
  if ( m_modeManager )
  {
    if ( m_modeManager->savedViewGoto( 0 ) )
      InvalidateRect( 0, false ) ;
  }
}

void WMSClientSampleView::OnSavedviewsGoto2()
{
  if ( m_modeManager )
  {
    if ( m_modeManager->savedViewGoto( 1 ) )
      InvalidateRect( 0, false ) ;
  }
}

void WMSClientSampleView::OnSavedviewsGoto3()
{
  if ( m_modeManager )
  {
    if ( m_modeManager->savedViewGoto( 2 ) )
      InvalidateRect( 0, false ) ;
  }
}

void WMSClientSampleView::OnSavedviewsReset()
{
  if ( m_modeManager )
    m_modeManager->savedViewReset( ) ;
}

void WMSClientSampleView::OnSavedviewsSet1()
{
  if ( m_modeManager )
    m_modeManager->savedViewSetToCurrent( 0 ) ;
}

void WMSClientSampleView::OnUpdateSavedviewsReset(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( m_mapLoaded ? true : false  ) ;
}

void WMSClientSampleView::OnUpdateSavedviewsGoto3(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( m_mapLoaded && m_modeManager && m_modeManager->savedViewValid( 2 ) ) ;
}

void WMSClientSampleView::OnUpdateSavedviewsGoto2(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( m_mapLoaded && m_modeManager && m_modeManager->savedViewValid( 1 ) ) ;
}

void WMSClientSampleView::OnUpdateSavedviewsGoto1(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( m_mapLoaded && m_modeManager && m_modeManager->savedViewValid( 0 ) ) ;
}

void WMSClientSampleView::OnUpdateSavedviewsSet1(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( m_mapLoaded ? true : false  ) ;
}

void WMSClientSampleView::OnSavedviewsSet2()
{
  if ( m_modeManager )
    m_modeManager->savedViewSetToCurrent( 1 ) ;
}

void WMSClientSampleView::OnUpdateSavedviewsSet2(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( m_mapLoaded ? true : false  ) ;
}

void WMSClientSampleView::OnSavedviewsSet3()
{
  if ( m_modeManager )
    m_modeManager->savedViewSetToCurrent( 2 ) ;
}

void WMSClientSampleView::OnUpdateSavedviewsSet3(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( m_mapLoaded ? true : false  ) ;
}

void WMSClientSampleView::OnViewstackNextview()
{
  if ( m_modeManager )
  {
    if ( m_modeManager->viewStackGotoNext( ) )
      InvalidateRect( 0, false ) ;
  }
}

void WMSClientSampleView::OnUpdateViewstackNextview(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( (m_mapLoaded && m_modeManager && m_modeManager->viewStackNextValid( )) ? true : false ) ;
}

void WMSClientSampleView::OnViewstackPreviousview()
{
  if ( m_modeManager )
  {
    if ( m_modeManager->viewStackGotoPrevious( ) )
      InvalidateRect( 0, false ) ;
  }
}

void WMSClientSampleView::OnUpdateViewstackPreviousview(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( (m_mapLoaded && m_modeManager && m_modeManager->viewStackPreviousValid( )) ? true : false ) ;
}

BOOL WMSClientSampleView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
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

