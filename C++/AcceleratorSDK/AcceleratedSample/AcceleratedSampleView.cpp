// AcceleratedSampleView.cpp : implementation of the AcceleratedSampleView class
//

#include "stdafx.h"
#include "AcceleratedSample.h"

#include "AcceleratedSampleDoc.h"
#include "AcceleratedSampleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static void redrawCallback(void *arg)
{
  // Callback received to notify us that the screen needs up-dating.
  // This will only occur if the tile drawing queue becomes empty.
  AcceleratedSampleView *view = (AcceleratedSampleView*)arg;

  // The callback will be in the context of the background thread
  // so we must not call back into MapLink.
  if (view->m_hWnd != NULL)
  {
    view->InvalidateRect( 0, FALSE );
  }
}

/////////////////////////////////////////////////////////////////////////////
// AcceleratedSampleView

IMPLEMENT_DYNCREATE(AcceleratedSampleView, CView)

BEGIN_MESSAGE_MAP(AcceleratedSampleView, CView)
//{{AFX_MSG_MAP(AcceleratedSampleView)
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
  ON_COMMAND(ID_TOOLS_GRAB, OnToolsGrab)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOMIN,  OnUpdateToolsAction)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOMPAN, OnUpdateToolsMode)
  //ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
  //ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
  //}}AFX_MSG_MAP

  // Our common handlers can update multiple items.
  ON_UPDATE_COMMAND_UI(ID_EDIT_COPY,     OnUpdateToolsAction)
  ON_UPDATE_COMMAND_UI(ID_FILE_PRINT,    OnUpdateToolsAction)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOMOUT, OnUpdateToolsAction)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_PAN,     OnUpdateToolsMode)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOM,    OnUpdateToolsMode)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_GRAB,    OnUpdateToolsMode)

  // Add the middle button handlers since DevStudio doesn't 
  // give us the option.
  ON_WM_MBUTTONDOWN()
  ON_WM_MBUTTONDBLCLK()
  ON_WM_MBUTTONUP()

  //Accelerator SDK specific
  ON_UPDATE_COMMAND_UI(ID_TOOLS_STOPTHREAD, OnUpdateToolsAction)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_RESUMETHREAD, OnUpdateToolsAction)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_SWAPRENDERERS, OnUpdateToolsAction)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_THREADEDRENDERER, OnToolsRendererUpdate)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_BLOCKINGRENDERER, OnToolsRendererUpdate)

  ON_UPDATE_COMMAND_UI(ID_TOOLS_TOGGLENEAREST, OnUpdateToolsAction)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_TOGGLEBORDER, OnUpdateToolsAction)
  ON_UPDATE_COMMAND_UI(ID_LAYER_RASTERBUFFERING, OnUpdateToolsAction)
  ON_UPDATE_COMMAND_UI(ID_LAYER_BACKGROUNDTILESOPAQUE, OnUpdateToolsAction)
  ON_UPDATE_COMMAND_UI(ID_LAYER_BACKLAYERVISABILITY, OnUpdateToolsAction)
  ON_UPDATE_COMMAND_UI(ID_LAYER_FRONTLAYERVISABILITY, OnUpdateToolsAction)

  ON_WM_TIMER()
  ON_WM_KEYUP()
  ON_WM_KEYDOWN()
  ON_COMMAND(ID_TOOLS_STOPTHREAD, OnToolsStopthread)
  ON_COMMAND(ID_TOOLS_RESUMETHREAD, OnToolsResumethread)
  ON_COMMAND(ID_TOOLS_SWAPRENDERERS, OnToolsSwaprenderers)
  ON_COMMAND(ID_TOOLS_THREADEDRENDERER, OnToolsThreadedrenderer)
  ON_COMMAND(ID_TOOLS_BLOCKINGRENDERER, OnToolsBlockingrenderer)
  ON_COMMAND(ID_TOOLS_TOGGLENEAREST, OnToolsTogglenearest)
  ON_COMMAND(ID_TOOLS_TOGGLEBORDER, OnToolsToggleborder)

  ON_COMMAND(ID_LAYER_RASTERBUFFERING, &AcceleratedSampleView::OnLayerRasterbuffering)
  ON_COMMAND(ID_LAYER_BACKGROUNDTILESOPAQUE, &AcceleratedSampleView::OnLayerBackgroundtilesopaque)
  ON_COMMAND(ID_LAYER_BACKLAYERVISABILITY, &AcceleratedSampleView::OnLayerBacklayervisability)
  ON_COMMAND(ID_LAYER_FRONTLAYERVISABILITY, &AcceleratedSampleView::OnLayerFrontlayervisability)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// AcceleratedSampleView construction/destruction

AcceleratedSampleView::AcceleratedSampleView()
{
  // Clear my internal variables
  m_drawingSurface = 0 ; 
  m_modeManager  = 0 ;
  m_mapLoaded = false ;

  m_useThreadedRenderer = true;
  m_mtRenderControl = 0;
  m_threadStopped = true;
  m_blockingRenderControl = 0;

  m_rotate = 0.0 ;
  m_upPressed = false;
  m_downPressed = false;
  m_leftPressed = false;
  m_rightPressed = false;
  m_zPressed = false;
  m_xPressed = false;

  m_toggleNearest = true;
  m_toggleBorder = false;
  m_rasterBuffering = true;
  m_backgroundOpaque = false;
}

AcceleratedSampleView::~AcceleratedSampleView()
{
  if (m_useThreadedRenderer)
  {
    if (m_threadStopped)
    {
      // restart the stopped thread so we can do a clean shutdown.
      if (m_mtRenderControl)
      {
        m_mtRenderControl->resumeThread();
      }
    }
    m_mtRenderControl = 0; // owned by the Surface.
  }
  else
  {
    m_blockingRenderControl = 0; // owned by the Surface.
  }

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
// AcceleratedSampleView drawing

void AcceleratedSampleView::OnDraw(CDC* pDC)
{  
  if ( m_drawingSurface )
  {
    // You have to draw the whole display.
    RECT rect ;
    GetClientRect( &rect ) ;

    m_drawingSurface->drawDU( rect.left, rect.top, rect.right, rect.bottom, true ) ;

    // Don't forget to draw any echo rectangle that may be active.
    if ( m_modeManager )
    {
      m_modeManager->onDraw( rect.left, rect.top, rect.right, rect.bottom ) ;
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
// AcceleratedSampleView printing
BOOL AcceleratedSampleView::OnPreparePrinting(CPrintInfo* pInfo)
{
  // default preparation
  return DoPreparePrinting(pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// AcceleratedSampleView diagnostics

#ifdef _DEBUG
void AcceleratedSampleView::AssertValid() const
{
  CView::AssertValid();
}

void AcceleratedSampleView::Dump(CDumpContext& dc) const
{
  CView::Dump(dc);
}

AcceleratedSampleDoc* AcceleratedSampleView::GetDocument() // non-debug version is inline
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(AcceleratedSampleDoc)));
  return (AcceleratedSampleDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// AcceleratedSampleView message handlers

void AcceleratedSampleView::OnInitialUpdate() 
{
  CView::OnInitialUpdate();

  AcceleratedSampleDoc * doc = GetDocument() ;  

  if (!doc->mapLoaded())
  {
    // If there is no map then don't create the drawing surface.
    //
    // We only do this so the user can decide which renderer to use before loading
    // a map.
    //
    // Once we have created a drawing surface the renderer can not be changed.
    return ;
  }

  // In an SDI application, this method is called every time a document is
  // loaded.  The first time ever, there will be no drawing surface.
  // If we have no drawing surface, create it, otherwise remove the old layer.
  if ( !m_drawingSurface ) 
  {
    // Create the Drawing surface.
    //
    // Note:
    //  1. There is no need to use a Multi-threaded loader with this surface as
    //     map loading occurs in the background render thread.
    //
    //  2. Runtime projection of the maps are not currently supported.
    //
    //  3. Dynamic Arc projection is supported.
    //
    //  4. The Surface is always double buffered.
    //
    // There is an additional Constructor which allows the user to create the
    // DirectX9 configuration themseleves.
    m_drawingSurface = new TSLWGLAcceleratedSurface( m_hWnd, false );

    // Enable dynamic arc map support.
    // Enabling double buffering for layers or drawing surfaces does not make sense
    // for this surface.
    m_drawingSurface->setOption( TSLOptionDynamicArcSupportEnabled, true ) ;

    // Create a multi-threaded render control.
    //
    // We are requesting that the texture map tiles are loaded onto the
    // card in the background thread. You may wish to experiament with this setting.
    //
    // We also want a callback to occur once all the tiles have been loaded so that
    // we can redraw the screen.
    //
    // You may also wish to use a TSLAcceleratedBlockingRenderControl. This particular
    // render control blocks while each tile is rendered rather then use a thread
    // based approach.

    // Add the render control to the surface. This will create an Accelerated
    // Renderer to draw the map tiles.
    //
    if (m_useThreadedRenderer)
    {
      // The first parameter askes the renderer to try and load the texture map tile in the
      // background thread.
      //
      // The second parameter asks the renderer to try and create a display list containing
      // the texture map tile.
      //
      // Peformance will be dependent on your graphics card and TSLAcceleratorConfiguration
      // setup. Please try varying these settings and the tile configuration to obtain
      // the best performance possible for you hardware setup.
      m_mtRenderControl = new TSLAcceleratedMTRenderControl(false, false, redrawCallback, this);
      m_drawingSurface->addRenderControl(m_mtRenderControl);

      // If we are using the Multi-threaded render control then zoom can either
      // occur as blocking or non-blocking.
      //
      // If you require non-blocking zoom then you will need to either provide
      // a redraw and possibly a tile callback method to ensure that the display
      // is updated when the tiles have been rendered.
      //
      // By default zooming blocks.
      m_drawingSurface->setOption(TSLOptionAcceleratorZoomAsynchronous, false);
    }
    else
    {
      m_blockingRenderControl = new TSLAcceleratedBlockingRenderControl(redrawCallback, this);
      m_drawingSurface->addRenderControl(m_blockingRenderControl);
    }

    ////////////////////////////////////////////////////////////////////////
    // Reconfigure the number of tiles and tile size. This should be done before
    // any layers are added. If you do change the settings you should cause the
    // display to be redrawn.
    //
    // The values defined here are the defaults.
    //
    // We may wish to change the defaults to tune for the displayed window size
    // and the graphics card.
    const int tilePixelSize = 512;
    m_configuration.tileSize(tilePixelSize);  // size of tile in pixels (32, 64, 128, 256, 512, 1024 etc...)

    // If we are rotating the surface we need to consider how may tiles we
    // may possibly need to draw the bounding box around the circle scribed by the
    // view area being rotated.
    //
    // In addition panning requirements may also affect the number of tiles 
    // required.
    //
    // The following settings are hints that control the maximum number of tiles
    // that will be held before old tiles are removed.
    // 
    // However if the values are too small to be able to draw the rotated surface
    // the maximum will be increased temporarily. This may affect the pan performance
    // of a rotated surface as tiles just outside the required extent will be removed
    // from the tile cache too early.
    //
    // Calling TSLAcceleratedSurface::getConfiguration will return the number of tiles
    // currently being used if this is greater then the number requested.
    //
    m_configuration.numberTilesX(10);   // number of tiles in the X axis
    m_configuration.numberTilesY(10);   // number of tiles in the Y axis

    // Quality Settings
    m_configuration.textureFilterNearest(true);// GL_NEAREST (default, usually faster, but you may
                                       // lose fine detail) or GL_LINEAR (sometimes slower, but 
                                       // display is usually better). 
    m_configuration.textureBorder(false);// If we are using GL_LINEAR, some graphics cards display
                                       // a thin tile boundary. By setting this to true we add a 
                                       // small boundary to the texture, which lets the tile 
                                       // boundaries blend, eliminating the thin boundary effect.
    m_configuration.dynamicArcTolerance(10.0); // If we have a dynamic arc map the x-axis extent
                                       // changes as you move north and south. This causes the
                                       // cached map tiles to be thrown away too often.
                                       // The tolerance setting allows us to specify how much
                                       // change in the x-axis extent we want to tolerate before
                                       // redrawing the map tiles.
    m_configuration.viewExpansion(12.5); // This allows us to configure the pre-loading of map
                                       // tiles, by specifying a percentage of the current extent
                                       // to look ahead by.
                                       // The value used will be dependent on the max' pan rate
                                       // you wish to support and the effect on performance by
                                       // rendering more tiles (adjust numberTiles* as well).

    //
    // Note: There is a trade off between tile size and speed of display of the tiles.
    //
    //       If you make the tile size too small then the time taken to display all the
    //       tiles becomes greater and will probably impact upon the frame rate you can
    //       achieve.
    //
    //       The value used should be tuned for your map, drawing area and graphics card.
    //
    //       The drawing of the tiles occurs in the background thread, so the number of tiles
    //       generated is less important. It is the actual time to display the tiles once
    //       they have been generated that needs to be considered.
    //
    m_drawingSurface->setConfiguration(m_configuration);

    // Obtain the drawing area size.
    CRect rect ;    
    GetClientRect( &rect ) ;

    // Resize the drawing surface to match the window.
    m_drawingSurface->wndResize( 0, 0, rect.Width(), rect.Height() ) ;

    // Now create and initialse the mode manager and modes
    m_modeManager = new TSLInteractionModeManagerNT( this, m_drawingSurface, m_hWnd ) ;
    m_modeManager->addMode( new TSLInteractionModeZoom( ID_TOOLS_ZOOM ), true ) ;
    m_modeManager->addMode( new TSLInteractionModePan( ID_TOOLS_PAN ), false ) ;
    m_modeManager->addMode( new TSLInteractionModeGrab( ID_TOOLS_GRAB ), false ) ;
    // Magnify does not work with this Surface.
    //  m_modeManager->addMode( new TSLInteractionModeMagnify( ID_TOOLS_MAGNIFY ), false ) ;
  }
  else if ( m_mapLoaded ) 
  {
    // We have a drawing surface with a map loaded into it.
    // Detach the map from the surface
    if ( m_drawingSurface && doc )
    {
      // Never change the contents of a drawing surface while the background
      // thread is running.
      stopBackgroundThread();

      doc->detachFromSurface( m_drawingSurface ) ;

      // We re-start the thread below after we have added the
      // map datalayer.
    }
  }

  // If the document loaded successfully, add it to the drawing surface
  stopBackgroundThread();
  m_mapLoaded = doc->addToSurface( m_drawingSurface ) ;

  // If we are displaying vector maps we may wish to enable the following
  // option.
  //
  // This option extends the view extent for Symbols and Text in the map layer
  // by the percentage specified. This is sometimes necessary to ensure that
  // text is drawn across the background generated map tiles.
  //
  // The value should be changed depending on the size of the text and symbols.
  //
  // If the value is too large performance will be affected.
  //
  // Note:
  // Setting the double buffer option of the map is not required as the
  // map tiles are cached.
  m_drawingSurface->setDataLayerProps("map", TSLPropertyLoadedSymbolsAndTextViewExpansion, 10);

  // Start the background thread now we have added the map data layer to the
  // drawing surface.
  resumeBackgroundThread();


  // clear the surface rotation.
  m_drawingSurface->rotate(0.0);
  m_rotate = 0.0;    // clear the local rotation variable.

  // and reset the current view to display the entire map.
  m_drawingSurface->reset( false ) ;

  // Reset the current interaction mode
  m_modeManager->resetViews( ) ;
  m_modeManager->resetMode( TSLButtonNone, 0, 0 ) ;

  // Get the map extent if one was loaded.
  if (m_mapLoaded)
  {
    doc->getMapExtent(m_tmcExtent);
  }

  // Setup a timer for handling the screen update on keyboard input.
  SetTimer( 0, 1, 0 );
}

void AcceleratedSampleView::OnToolsZoomin() 
{
  if ( m_modeManager )
  {
    if ( m_modeManager->zoomIn( 30 ) )
    {
      // Ideally you should force a redraw now rather then go through
      // windows. This however keeps the example simple.
      InvalidateRect( 0, false ) ;
    }
  }
}

void AcceleratedSampleView::OnToolsZoomout() 
{
  if ( m_modeManager )
  {
    if ( m_modeManager->zoomOut( 30 ) )
      InvalidateRect( 0, false ) ;
  }
}

void AcceleratedSampleView::OnToolsReset() 
{
  if ( m_modeManager )
  {
    if ( m_modeManager->resetToFullExtent( ) )
    {
      // clear the surface rotation.
      m_drawingSurface->rotate(0.0);
      m_rotate = 0.0;    // clear the local rotation variable.

      // Reset the window to display the entire map, and request a window redraw.
      m_drawingSurface->reset( true ) ;
    }
  }
}

void AcceleratedSampleView::OnSize(UINT nType, int cx, int cy) 
{
  CView::OnSize(nType, cx, cy);

  if ( m_drawingSurface )
  {
    // Inform the drawing surface of the new window size,
    // attempting to keep the top left corner the same.
    // Do not ask for an automatic redraw since we will get an OnDraw call
    m_drawingSurface->wndResize( 0, 0, cx, cy, true, TSLResizeActionMaintainTopLeft ) ;
    if ( m_modeManager )
    {
      m_modeManager->onSize( cx, cy ) ;
    }
  }
}

void AcceleratedSampleView::OnMouseMove(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_modeManager )
  {
    TSLButtonType button = TSLButtonNone ;

    if ( nFlags & MK_LBUTTON )
    {
      button = TSLButtonLeft ;
    }
    else if ( nFlags & MK_MBUTTON )
    {
      button = TSLButtonCentre ;
    }
    else if ( nFlags & MK_RBUTTON )
    {
      button = TSLButtonRight ;
    }

    if ( m_modeManager->onMouseMove( button, point.x, point.y,
				     nFlags & MK_SHIFT, nFlags & MK_CONTROL ) )
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( 0, false ) ;
    }
  }

  CView::OnMouseMove(nFlags, point);
}

void AcceleratedSampleView::OnLButtonDown(UINT nFlags, CPoint point) 
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

void AcceleratedSampleView::OnMButtonDown(UINT nFlags, CPoint point) 
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

void AcceleratedSampleView::OnRButtonDown(UINT nFlags, CPoint point) 
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

void AcceleratedSampleView::OnLButtonUp(UINT nFlags, CPoint point) 
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

void AcceleratedSampleView::OnMButtonUp(UINT nFlags, CPoint point) 
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

void AcceleratedSampleView::OnRButtonUp(UINT nFlags, CPoint point) 
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

void AcceleratedSampleView::OnToolsPan() 
{
  if ( m_modeManager )
    m_modeManager->setCurrentMode( ID_TOOLS_PAN ) ;
}

void AcceleratedSampleView::OnToolsZoom() 
{
  if ( m_modeManager )
    m_modeManager->setCurrentMode( ID_TOOLS_ZOOM ) ;
}

void AcceleratedSampleView::OnToolsZoompan() 
{
  if ( m_modeManager )
    m_modeManager->setCurrentMode( ID_TOOLS_ZOOMPAN ) ;
}

void AcceleratedSampleView::OnToolsGrab() 
{
  if ( m_modeManager )
    m_modeManager->setCurrentMode( ID_TOOLS_GRAB ) ;
}

void AcceleratedSampleView::OnUpdateToolsAction(CCmdUI* pCmdUI) 
{
   // Common update handler to all items that are dependant upon the map
  // being loaded.
  pCmdUI->Enable( m_mapLoaded ? true : false ) ;

  if (pCmdUI->m_nID == ID_TOOLS_STOPTHREAD)
  {
    pCmdUI->SetCheck( m_threadStopped ) ;
  }
  if (pCmdUI->m_nID == ID_TOOLS_RESUMETHREAD)
  {
    pCmdUI->SetCheck( !m_threadStopped ) ;
  }
  if (pCmdUI->m_nID == ID_TOOLS_SWAPRENDERERS)
  {
    pCmdUI->Enable( m_mapLoaded ? false : true ) ;
  }
  if (pCmdUI->m_nID == ID_TOOLS_TOGGLENEAREST)
  {
    pCmdUI->SetCheck( !m_toggleNearest ) ;
  }
  if (pCmdUI->m_nID == ID_TOOLS_TOGGLEBORDER)
  {
    pCmdUI->SetCheck( !m_toggleBorder ) ;
  }
  if (pCmdUI->m_nID == ID_LAYER_RASTERBUFFERING)
  {
    pCmdUI->SetCheck( m_rasterBuffering ) ;
  }
  if (pCmdUI->m_nID == ID_LAYER_BACKGROUNDTILESOPAQUE)
  {
    pCmdUI->SetCheck( !m_backgroundOpaque ) ;
  }
  if (pCmdUI->m_nID == ID_LAYER_BACKLAYERVISABILITY)
  {
    AcceleratedSampleDoc* doc = GetDocument();
    pCmdUI->SetCheck( doc->getBackLayerVisibility() ) ;
  }
  if (pCmdUI->m_nID == ID_LAYER_FRONTLAYERVISABILITY)
  {
    AcceleratedSampleDoc* doc = GetDocument();
    pCmdUI->SetCheck( doc->getFrontLayerVisibility() ) ;
  }
}

void AcceleratedSampleView::OnToolsRendererUpdate(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( m_drawingSurface ? false : true ) ;

  if (pCmdUI->m_nID == ID_TOOLS_THREADEDRENDERER)
  {
    pCmdUI->SetCheck( m_useThreadedRenderer ) ;
  }
  if (pCmdUI->m_nID == ID_TOOLS_BLOCKINGRENDERER)
  {
    pCmdUI->SetCheck( !m_useThreadedRenderer ) ;
  }
}

void AcceleratedSampleView::OnUpdateToolsMode(CCmdUI* pCmdUI) 
{
  // Common update handler to all items that are dependant upon the map
  // being loaded, and act as radios dependant upon the current mode.
  pCmdUI->Enable( m_mapLoaded ? true : false  ) ;
  pCmdUI->SetCheck( m_modeManager && m_modeManager->getCurrentMode() == pCmdUI->m_nID ) ;
}

void AcceleratedSampleView::OnEditCopy() 
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

void AcceleratedSampleView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
  // Just handle like a normal press to avoid losing the second press
  OnLButtonDown( nFlags, point ) ;
  CView::OnLButtonDblClk(nFlags, point);
}

void AcceleratedSampleView::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
  // Just handle like a normal press to avoid losing the second press
  OnRButtonDown( nFlags, point ) ;
  CView::OnRButtonDblClk(nFlags, point);
}
void AcceleratedSampleView::OnMButtonDblClk(UINT nFlags, CPoint point) 
{
  // Just handle like a normal press to avoid losing the second press
  OnMButtonDown( nFlags, point ) ;
  CView::OnMButtonDblClk(nFlags, point);
}

void AcceleratedSampleView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
  // printing is not supported for the Accelerator SDK.
}

BOOL AcceleratedSampleView::OnEraseBkgnd(CDC* pDC) 
{
  // Since the drawing surface is double buffered, we don't need to erase the
  // background.
  if (!m_drawingSurface)
  {
    return CView::OnEraseBkgnd(pDC);
  }
  return true;
}

//void AcceleratedSampleView::OnFilePrint() 
//{
//  // Printing is not supported.
//  CView::OnFilePrint() ;
//}

//void AcceleratedSampleView::OnFilePrintPreview() 
//{
//  // Printing is not supported.
//  CView::OnFilePrintPreview() ;
//}

void AcceleratedSampleView::OnTimer(UINT_PTR nIDEvent)
{
  CView::OnTimer(nIDEvent);

  if ( m_drawingSurface && m_mapLoaded )
  {
    long moveX = 0L;
    long moveY = 0L;
    double rotate = 0.0;

    if ( m_upPressed )
    {
      moveY -= PIXEL_SPEED;
    }
    if ( m_downPressed )
    {
      moveY += PIXEL_SPEED;
    }
    if ( m_leftPressed )
    {
      moveX -= PIXEL_SPEED;
    }
    if ( m_rightPressed )
    {
      moveX += PIXEL_SPEED;
    }
    if ( m_zPressed )
    {
      rotate += 2.0;
    }
    if ( m_xPressed )
    {
      rotate -= 2.0;
    }

    if ( ( moveX != 0 ) || ( moveY != 0 ) || ( rotate != 0.0 ) )
    {
      // rotate the drawing surface (no redraw)
      m_rotate += rotate;
      if( !m_drawingSurface->rotate(m_rotate * DEGREES_2_RAD) )
      {
        m_rotate -= rotate;
      }
      if (moveX != 0 || moveY != 0)
      {
        // We need to deal in User Units rather then DU's as we will lose
        // a lot of precision if we don't
        double x1, y1, x2, y2;
        m_drawingSurface->getUUExtent(&x1, &y1, &x2, &y2);
        const double width = x2 - x1;
        const double height = y2 - y1;
        double centerX = x1 + (width/2.0);
        double centerY = y1 + (height/2.0);

        // Use the center of the UU extent to find the center of the DU extent.
        TSLDeviceUnits duCenterX, duCenterY;
        m_drawingSurface->UUToDU(centerX, centerY, &duCenterX, &duCenterY);
        double uuX, uuY;

        // Add on the pixel movement and get the new uu values.
        m_drawingSurface->DUToUU (duCenterX+moveX, duCenterY+moveY, &uuX, &uuY);

        // Work out how far to move.
        // If there is no move in an axis set the move ammount to 0 to eliminate very small
        // floating point errors which will add up over time.
        double moveUUx = moveX == 0 ? 0.0 : (uuX - centerX);
        double moveUUy = moveY == 0 ? 0.0 : (uuY - centerY);

        // pan the drawing surface (with redraw)
        m_drawingSurface->pan(centerX + moveUUx, centerY + moveUUy, true);
      }
      else
      {
        m_drawingSurface->redraw();
      }
    }   
  }
}

void AcceleratedSampleView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  if ( nChar == VK_UP )
  {
    m_upPressed = false;
  }
  else if ( nChar == VK_DOWN )
  {
    m_downPressed = false;
  }
  else if ( nChar == VK_LEFT )
  {
    m_leftPressed = false;
  }
  else if ( nChar == VK_RIGHT )
  {
    m_rightPressed = false;
  }
  else if ( nChar == 'Z' )
  {
    m_zPressed = false;
  }
  else if ( nChar == 'X' )
  {
    m_xPressed = false;
  }
  else
  {
    CView::OnKeyUp(nChar, nRepCnt, nFlags);
  }
}

void AcceleratedSampleView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  if (!m_drawingSurface)
    return ;

  if ( nChar == VK_UP )
  {
    m_upPressed = true;
  }
  else if ( nChar == VK_DOWN )
  {
    m_downPressed = true;
  }
  else if ( nChar == VK_LEFT )
  {
    m_leftPressed = true;
  }
  else if ( nChar == VK_RIGHT )
  {
    m_rightPressed = true;
  }
  else if ( nChar == 'Z' )
  {
    m_zPressed = true;
  }
  else if ( nChar == 'X' )
  {
    m_xPressed = true;
  }
  else if (nChar = 'R' || nChar == 'r')
  {
    m_drawingSurface->redraw();
  }
  else
  {
    CView::OnKeyDown(nChar, nRepCnt, nFlags);
  }
}

void AcceleratedSampleView::OnToolsStopthread()
{
  stopBackgroundThread();
}

void AcceleratedSampleView::OnToolsResumethread()
{
  resumeBackgroundThread();
}

void AcceleratedSampleView::OnToolsSwaprenderers()
{
  if (m_mtRenderControl && m_blockingRenderControl)
  {
    // Swapping the renders will not work as once one has been added
    // it can not be changed.
    m_useThreadedRenderer = !m_useThreadedRenderer;
    if (m_useThreadedRenderer)
    {
      m_drawingSurface->addRenderControl(m_mtRenderControl);
    }
    else
    {
      m_drawingSurface->addRenderControl(m_blockingRenderControl);
    }
    InvalidateRect( 0, false ) ;
  }
}

void AcceleratedSampleView::OnToolsThreadedrenderer()
{
  if (!m_drawingSurface)
  {
    m_useThreadedRenderer = true;
  }
}

void AcceleratedSampleView::OnToolsBlockingrenderer()
{
  if (!m_drawingSurface)
  {
    m_useThreadedRenderer = false;
  }
}

void AcceleratedSampleView::resetMode(TSLInteractionMode * mode, TSLButtonType button, TSLDeviceUnits xDU, TSLDeviceUnits yDU)
{
  // Ignore the override
}

void AcceleratedSampleView::viewChanged(TSLDrawingSurface* drawingSurface)
{
  static int cnt = 1 ;

  TRACE1( "View change %d\n", cnt++ ) ;
}

void AcceleratedSampleView::OnToolsTogglenearest()
{
  if (m_drawingSurface)
  {
    m_configuration.textureFilterNearest( !m_configuration.textureFilterNearest() );
    m_drawingSurface->setConfiguration(m_configuration);
    m_toggleNearest = m_configuration.textureFilterNearest();
    InvalidateRect( 0, false ) ;
  }
}

void AcceleratedSampleView::OnToolsToggleborder()
{
  if (m_drawingSurface)
  {
    m_configuration.textureBorder( !m_configuration.textureBorder() );
    m_drawingSurface->setConfiguration(m_configuration);
    m_toggleBorder = m_configuration.textureBorder();
    InvalidateRect( 0, false ) ;
  }
}

// return active view
AcceleratedSampleView * AcceleratedSampleView::GetView()
{
  CFrameWnd * pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);

  if (!pFrame)
    return NULL;

  CView * pView = pFrame->GetActiveView();

  if ( !pView )
    return NULL;

  // Fail if view is of wrong kind
  // (this could occur with splitter windows, or additional
  // views on a single document
  if ( ! pView->IsKindOf( RUNTIME_CLASS(AcceleratedSampleView) ) )
    return NULL;

  return (AcceleratedSampleView *) pView;
}

void AcceleratedSampleView::stopBackgroundThread()
{
  if (m_mtRenderControl)
  {
    if (!m_threadStopped)
    {
      m_threadStopped = true;
      m_mtRenderControl->stopThread();
    }
  }
}

void AcceleratedSampleView::resumeBackgroundThread()
{
  if (m_mtRenderControl)
  {
    if (m_threadStopped)
    {
      m_threadStopped = false;
      m_mtRenderControl->resumeThread();
    }
  }
}

void AcceleratedSampleView::detachLayers()
{
  stopBackgroundThread();

  AcceleratedSampleDoc * doc = GetDocument() ;  

  doc->detachFromSurface(m_drawingSurface);

  resumeBackgroundThread();
}

void AcceleratedSampleView::OnLayerRasterbuffering()
{
  if (m_drawingSurface)
  {
    AcceleratedSampleDoc * doc = GetDocument() ;  
    doc->rasterBuffering(m_drawingSurface);
    m_rasterBuffering = !m_rasterBuffering;
    InvalidateRect( 0, false ) ;
  }
}

void AcceleratedSampleView::OnLayerBackgroundtilesopaque()
{
  if (m_drawingSurface)
  {
    // not supported
  }
}


void AcceleratedSampleView::redraw()
{
  InvalidateRect( 0, false ) ;
}

void AcceleratedSampleView::OnLayerBacklayervisability()
{
  if (m_drawingSurface)
  {
    AcceleratedSampleDoc * doc = GetDocument() ;  
    doc->changeBackLayerVisibility(m_drawingSurface);
    InvalidateRect( 0, false ) ;
  }
}

void AcceleratedSampleView::OnLayerFrontlayervisability()
{
  if (m_drawingSurface)
  {
    AcceleratedSampleDoc * doc = GetDocument() ;  
    doc->changeFrontLayerVisibility(m_drawingSurface);
    InvalidateRect( 0, false ) ;
  }
}


