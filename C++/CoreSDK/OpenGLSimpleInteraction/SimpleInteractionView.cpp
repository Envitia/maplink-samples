// SimpleInteractionView.cpp : implementation of the SimpleInteractionView class
//

#include "stdafx.h"
#include "resource.h"
#include "SimpleInteractionDoc.h"
#include "SimpleInteractionView.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SimpleInteractionView

IMPLEMENT_DYNCREATE(SimpleInteractionView, CView)

BEGIN_MESSAGE_MAP(SimpleInteractionView, CView)
//{{AFX_MSG_MAP(SimpleInteractionView)
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
  ON_WM_CREATE()
    ON_COMMAND(ID_VIEW_WRAP, &SimpleInteractionView::OnViewWrap)
  ON_UPDATE_COMMAND_UI(ID_VIEW_WRAP, &SimpleInteractionView::OnUpdateViewWrap)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SimpleInteractionView construction/destruction

SimpleInteractionView::SimpleInteractionView()
{
  // Clear my internal variables
  m_drawingSurface = 0 ; 
  m_modeManager    = 0 ;
  m_mapLoaded = false ;
}

SimpleInteractionView::~SimpleInteractionView()
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
// SimpleInteractionView drawing

void SimpleInteractionView::OnDraw(CDC* pDC)
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
// SimpleInteractionView printing
BOOL SimpleInteractionView::OnPreparePrinting(CPrintInfo* pInfo)
{
  pInfo->SetMaxPage(1);
  return DoPreparePrinting(pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// SimpleInteractionView diagnostics

#ifdef _DEBUG
void SimpleInteractionView::AssertValid() const
{
  CView::AssertValid();
}

void SimpleInteractionView::Dump(CDumpContext& dc) const
{
  CView::Dump(dc);
}

SimpleInteractionDoc* SimpleInteractionView::GetDocument() // non-debug version is inline
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(SimpleInteractionDoc)));
  return (SimpleInteractionDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// SimpleInteractionView message handlers

void SimpleInteractionView::OnInitialUpdate() 
{
  CView::OnInitialUpdate();
  
  SimpleInteractionDoc * doc = GetDocument() ;
  
  // In an SDI application, this method is called every time a document is
  // loaded.  The first time ever, there will be no drawing surface.
  // If we have no drawing surface, create it, otherwise remove the old layer.
  if ( !m_drawingSurface ) 
  {
    CRect rect ;
    
    GetClientRect( &rect ) ;
    
    /////////////////////////////////////////////////////////
    // This sample is based on the SimpleInteraction Sample.
    //
    // The modifications to use the OpenGL 2D Drawing Surface
    // have been kept to a minimum.
    //
    /*
       // Include the header
       #include "MapLinkOpenGLSurface.h"

       // Replace the Drawing Surface and Interaction Mode Manager	   
	   TSLWGLSurface * m_drawingSurface ;                 // Was TSLNTSurface
	   TSLInteractionModeManagerGeneric * m_modeManager ; // Was TSLInteractionModeManagerNT

       // Update the creation of the Drawing Surface and Interaction modes
       // as shown below.

       // Update the Linker 'Additional Dependencies' to include:
       //   Debug:    MapLinkOpenGLSurfaced.lib;OpenGL32.lib
       //   Release:  MapLinkOpenGLSurface.lib;OpenGL32.lib
       //
       // Don't forget the 64bit linker updates as well.
    */
    ////////////////////////////////////////////////////////

    // Tell the drawing surface whether it will need to perform buffer swaps, or whether
    // it is handled externally. In this sample the drawing surface handles buffer swaps for us.
    TSLOpenGLSurfaceCreationParameters creationOptions;
    creationOptions.swapBuffersManually( false );

    // Force the use of the OpenGL 2.1 profile - use this if the performance is significantly lower
    // than expected. We have found some drivers and hardware are slower with the OpenGL 3.2 renderer
    // than with the OpenGL 2.1 renderer.
    //creationOptions.forceFallbackRenderer(true);

    // Disable vertical sync'. Set this if you want to check maximum fps.
    //creationOptions.useVSync( false );

    m_drawingSurface = new TSLWGLSurface( (TSLWindowHandle)m_hWnd, false, creationOptions);

    // Not required - always Double Buffered.
    //    m_drawingSurface->setOption( TSLOptionDoubleBuffered, true ) ;
    m_drawingSurface->setOption( TSLOptionDynamicArcSupportEnabled, true ) ;

    // initial window extents
    m_drawingSurface->wndResize( 0, 0, rect.Width(), rect.Height() ) ;

    // Now create and initialse the mode manager and modes
    m_modeManager = new TSLInteractionModeManagerGeneric( this, m_drawingSurface, 5, 5, 30, true ) ;
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

void SimpleInteractionView::OnToolsZoomin() 
{
  if ( m_modeManager )
  {
    if ( m_modeManager->zoomIn( 30 ) )
      InvalidateRect( 0, false ) ;
  }
}

void SimpleInteractionView::OnToolsZoomout() 
{
  if ( m_modeManager )
  {
    if ( m_modeManager->zoomOut( 30 ) )
      InvalidateRect( 0, false ) ;
  }
}

void SimpleInteractionView::OnToolsReset() 
{
  if ( m_modeManager )
  {
    if ( m_modeManager->resetToFullExtent( ) )
      InvalidateRect( 0, false ) ;
  }

}

void SimpleInteractionView::OnSize(UINT nType, int cx, int cy) 
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

void SimpleInteractionView::OnMouseMove(UINT nFlags, CPoint point) 
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

void SimpleInteractionView::OnLButtonDown(UINT nFlags, CPoint point) 
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

void SimpleInteractionView::OnMButtonDown(UINT nFlags, CPoint point) 
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

void SimpleInteractionView::OnRButtonDown(UINT nFlags, CPoint point) 
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

void SimpleInteractionView::OnLButtonUp(UINT nFlags, CPoint point) 
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

void SimpleInteractionView::OnMButtonUp(UINT nFlags, CPoint point) 
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

void SimpleInteractionView::OnRButtonUp(UINT nFlags, CPoint point) 
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

void SimpleInteractionView::OnToolsPan() 
{
  if ( m_modeManager )
    m_modeManager->setCurrentMode( ID_TOOLS_PAN ) ;

}

void SimpleInteractionView::OnToolsZoom() 
{
  if ( m_modeManager )
    m_modeManager->setCurrentMode( ID_TOOLS_ZOOM ) ;

}

void SimpleInteractionView::OnToolsGrab() 
{
  if ( m_modeManager )
    m_modeManager->setCurrentMode( ID_TOOLS_GRAB ) ;
}

void SimpleInteractionView::OnUpdateToolsAction(CCmdUI* pCmdUI) 
{
  // Common update handler to all items that are dependant upon the map
  // being loaded.
  pCmdUI->Enable( m_mapLoaded ? true : false ) ;
}

void SimpleInteractionView::OnUpdateToolsMode(CCmdUI* pCmdUI) 
{
  // Common update handler to all items that are dependant upon the map
  // being loaded, and act as radios dependant upon the current mode.
  pCmdUI->Enable( m_mapLoaded ? true : false  ) ;
  pCmdUI->SetCheck( m_mapLoaded && m_modeManager && m_modeManager->getCurrentMode() == (long)pCmdUI->m_nID ) ;
}

void SimpleInteractionView::OnEditCopy() 
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
    DeleteDC( newDC );

  ReleaseDC(pDC);
}

void SimpleInteractionView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
  // Just handle like a normal press to avoid losing the second press
  OnLButtonDown( nFlags, point ) ;
  CView::OnLButtonDblClk(nFlags, point);
}

void SimpleInteractionView::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
  // Just handle like a normal press to avoid losing the second press
  OnRButtonDown( nFlags, point ) ;
  CView::OnRButtonDblClk(nFlags, point);
}
void SimpleInteractionView::OnMButtonDblClk(UINT nFlags, CPoint point) 
{
  // Just handle like a normal press to avoid losing the second press
  OnMButtonDown( nFlags, point ) ;
  CView::OnMButtonDblClk(nFlags, point);
}


BOOL SimpleInteractionView::OnGesturePan(CPoint ptFrom, CPoint ptTo)
{
  int dx = ptTo.x - ptFrom.x;
  int dy = ptTo.y - ptFrom.y;

  if (dx != 0 || dy != 0)
  {
    if ( m_modeManager->onGesturePan( ptFrom.x, ptFrom.y, ptTo.x, ptTo.y ) )
       InvalidateRect( 0, false ) ;
  }
  return TRUE;
}

BOOL SimpleInteractionView::OnGestureZoom(CPoint ptCenter, long lDelta)
{
  if ((m_pCurrentGestureInfo->dwFlags & GF_BEGIN) == GF_BEGIN)
  {
    // NOTE - There is a bug in the Windows Gesture handling, when
    // the gesture occurs on a secondary screen. The initial lDelta argument
    // is meant to be distance between the gesture points, but under these
    // circumstances it is incorrect - too large.  
    // The impact of this is that the subsequent scaling is incorrect
    m_dblZoomRatioStart = m_dblZoomRatioTotal = lDelta;
  }
  else if (lDelta != 0)
  {
    m_dblZoomRatioTotal += lDelta;
    double zoomFactor = m_dblZoomRatioTotal / m_dblZoomRatioStart;

    if (  m_modeManager->onGestureZoom( ptCenter.x, ptCenter.y, zoomFactor ) )
    {
      m_dblZoomRatioStart = m_dblZoomRatioTotal;
      InvalidateRect( 0, false ) ;
    }
  }

  return TRUE;
}

BOOL SimpleInteractionView::OnGestureRotate(CPoint ptCenter, double dblAngle)
{
  if ((m_pCurrentGestureInfo->dwFlags & GF_BEGIN) == GF_BEGIN)
  {
    // Make the first center, the rotating one
    m_ptCenter = ptCenter;
    m_rotationBase = m_drawingSurface->rotation() ;
  }
  else if (dblAngle != 0.0)
  {
    if ( m_modeManager->onGestureRotate( ptCenter.x, ptCenter.y, m_rotationBase + dblAngle ) )
      InvalidateRect( 0, false ) ;
  }

  return TRUE;
}

BOOL SimpleInteractionView::OnGesturePressAndTap(CPoint ptFirstFinger, long lDelta)
{
  if ((m_pCurrentGestureInfo->dwFlags & GF_BEGIN) != 0)
  {
    if ( m_modeManager->onGesturePressAndTap( ptFirstFinger.x, ptFirstFinger.y, lDelta ) )
      InvalidateRect( 0, false ) ;
  }

  return TRUE;
}

BOOL SimpleInteractionView::OnGestureTwoFingerTap(CPoint ptCenter)
{
  if ( m_modeManager->onGestureTwoFingerTap( ptCenter.x, ptCenter.y ) )
    InvalidateRect( 0, false ) ;

  return TRUE;
}

void SimpleInteractionView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
  // Attempt to print using the PrintHelper, using the standard templates.
  if ( m_drawingSurface ) 
  {
    TSLDrawingSurface *drawingSurface = m_drawingSurface;
    bool localSurface = false;
    if (m_drawingSurface->clone() == NULL)
    {
      // The surface can not be cloned.
      //
      // This means that we can not print what is on the drawing surface in the normal way.
      //
      // There are two options to printing in this case;
      //
      //   1. Create a tiled set of raster's using the current drawing surface and draw the rasters
      //      on a printer HDC.
      //
      //      This requires a very large amount of memory if you are to maintain the DPI of the printer.
      //
      //   2. Create a copy of the contents of the current surface in a TSLNTSurface.
      //
      //      This is the simplest approach, but can not replicate all the drawing capabilities of the
      //      OpenGL 2D surface such as alpha lines and anti-aliasing.
      //
      //      This will also not draw anything that is only being drawn using OpenGL, for example;
      //      Custom Data-Layer and Dynamic Data Objects which are drawn using OpenGL rather than
      //      via the TSLRenderingInterface.
      //
      //      You can of course query the Drawing Surface type via the TSLRenderingInterface and provide
      //      a GDI/TSLRenderingInterface version of the drawing.
      //
      localSurface = true;
      drawingSurface = new TSLNTSurface((TSLWindowHandle) m_hWnd);

      // Copy the contents of the OpenGL Surface to the GDI Surface (we don't copy the HWND, HDC etc...
      // information as this does not make sense. 
      TSLDrawingSurface::copy(drawingSurface, m_drawingSurface);
    }

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
    if ( TSLDrawingSurface::findFile( templateFile, templateFilePath ) )
    {
      // load the template
      tmp = TSLPrintHelper::loadTemplate( templateFilePath ) ;
    }

    if ( !tmp || tmp->size() == 0 )
    {
      if (localSurface)
        delete drawingSurface;
      MessageBox( _T("Cannot find print template") ) ;
      return ;
    }

    // Set up a print item with a 3 pixel wide border, displaying the current extent.
    // All visible data layers in the drawing surface are used when printing.
    // To display at a specific scale, use item.centreAndScale rather than item.extent
    // There can be as many items as we need 
    // Note : Use the extent captured in the OnFilePrint, not the current extent.  This is
    //        because the extent may have changed to to SimpleInteraction Print Preview taking control
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

    if (localSurface)
      delete drawingSurface;
  }
}

BOOL SimpleInteractionView::OnEraseBkgnd(CDC* pDC) 
{
  // Since the drawing surface is double buffered, we don't need to erase the
  // background.
  return true;
}


void SimpleInteractionView::OnFilePrint() 
{
  // We should not query the extent to print during the OnPrint, since the window may change
  // size, especially in SimpleInteraction print-preview
  if ( m_drawingSurface )
    m_drawingSurface->getTMCExtent( m_extentToPrint ) ;
  CView::OnFilePrint() ;
}

void SimpleInteractionView::OnFilePrintPreview() 
{
  // We should not query the extent to print during the OnPrint, since the window may change
  // size, especially in SimpleInteraction print-preview
  if ( m_drawingSurface )
    m_drawingSurface->getTMCExtent( m_extentToPrint ) ;
  CView::OnFilePrintPreview() ;
}

void SimpleInteractionView::OnToolsMagnify()
{
  if ( m_modeManager )
    m_modeManager->setCurrentMode( ID_TOOLS_MAGNIFY ) ;
}

void SimpleInteractionView::resetMode(TSLInteractionMode * mode, TSLButtonType button, TSLDeviceUnits xDU, TSLDeviceUnits yDU)
{
  // Ignore the override
}

void SimpleInteractionView::viewChanged(TSLDrawingSurface* drawingSurface)
{
 
}

void SimpleInteractionView::OnSavedviewsGoto1()
{
  if ( m_modeManager )
  {
    if ( m_modeManager->savedViewGoto( 0 ) )
      InvalidateRect( 0, false ) ;
  }
}

void SimpleInteractionView::OnSavedviewsGoto2()
{
  if ( m_modeManager )
  {
    if ( m_modeManager->savedViewGoto( 1 ) )
      InvalidateRect( 0, false ) ;
  }
}

void SimpleInteractionView::OnSavedviewsGoto3()
{
  if ( m_modeManager )
  {
    if ( m_modeManager->savedViewGoto( 2 ) )
      InvalidateRect( 0, false ) ;
  }
}

void SimpleInteractionView::OnSavedviewsReset()
{
  if ( m_modeManager )
    m_modeManager->savedViewReset( ) ;
}

void SimpleInteractionView::OnSavedviewsSet1()
{
  if ( m_modeManager )
    m_modeManager->savedViewSetToCurrent( 0 ) ;
}

void SimpleInteractionView::OnUpdateSavedviewsReset(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( m_mapLoaded ? true : false  ) ;
}

void SimpleInteractionView::OnUpdateSavedviewsGoto3(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( m_mapLoaded && m_modeManager && m_modeManager->savedViewValid( 2 ) ) ;
}

void SimpleInteractionView::OnUpdateSavedviewsGoto2(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( m_mapLoaded && m_modeManager && m_modeManager->savedViewValid( 1 ) ) ;
}

void SimpleInteractionView::OnUpdateSavedviewsGoto1(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( m_mapLoaded && m_modeManager && m_modeManager->savedViewValid( 0 ) ) ;
}

void SimpleInteractionView::OnUpdateSavedviewsSet1(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( m_mapLoaded ? true : false  ) ;
}

void SimpleInteractionView::OnSavedviewsSet2()
{
  if ( m_modeManager )
    m_modeManager->savedViewSetToCurrent( 1 ) ;
}

void SimpleInteractionView::OnUpdateSavedviewsSet2(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( m_mapLoaded ? true : false  ) ;
}

void SimpleInteractionView::OnSavedviewsSet3()
{
  if ( m_modeManager )
    m_modeManager->savedViewSetToCurrent( 2 ) ;
}

void SimpleInteractionView::OnUpdateSavedviewsSet3(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( m_mapLoaded ? true : false  ) ;
}

void SimpleInteractionView::OnViewstackNextview()
{
  if ( m_modeManager )
  {
    if ( m_modeManager->viewStackGotoNext( ) )
      InvalidateRect( 0, false ) ;
  }
}

void SimpleInteractionView::OnUpdateViewstackNextview(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( (m_mapLoaded && m_modeManager && m_modeManager->viewStackNextValid( )) ? true : false ) ;
}

void SimpleInteractionView::OnViewstackPreviousview()
{
  if ( m_modeManager )
  {
    if ( m_modeManager->viewStackGotoPrevious( ) )
      InvalidateRect( 0, false ) ;
  }
}

void SimpleInteractionView::OnUpdateViewstackPreviousview(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( (m_mapLoaded && m_modeManager && m_modeManager->viewStackPreviousValid( )) ? true : false ) ;
}

BOOL SimpleInteractionView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
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


int SimpleInteractionView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (__super::OnCreate(lpCreateStruct) == -1)
    return -1;

  // Set up the gesture configuration
  GetGestureConfig(&m_gestureConfig);

  // Rotation is not enabled by default, so we enable it here
  m_gestureConfig.EnableRotate() ;

  // Configure the pan to be appropriate for a mapping application.  
  // The default behaviour without this will use a 'GUTTER' and bias towards horizontal and vertical scrolls.
  // We also enable the inertia pan - to disnable it, remove GC_PAN_WITH_INERTIA from the flags
  // MSDN doesn't document this very well, see http://answers.oreilly.com/topic/1258-working-with-gestures-in-windows-7/
  m_gestureConfig.EnablePan( TRUE,  
			      GC_PAN | GC_PAN_WITH_SINGLE_FINGER_VERTICALLY 
				    | GC_PAN_WITH_SINGLE_FINGER_HORIZONTALLY | GC_PAN_WITH_INERTIA ) ; 

  SetGestureConfig(&m_gestureConfig);

  return 0;
}

void SimpleInteractionView::OnViewWrap()
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

  InvalidateRect(0, false);
}

void SimpleInteractionView::OnUpdateViewWrap(CCmdUI* pCmdUI)
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
