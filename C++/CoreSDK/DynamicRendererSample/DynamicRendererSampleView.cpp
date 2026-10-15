// DynamicRendererSampleView.cpp : implementation of the DynamicRendererSampleView class
//

#include "stdafx.h"
#include "DynamicRendererSample.h"
#include "DynamicRendererSampleDoc.h"
#include "DynamicRendererSampleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DynamicRendererSampleView

IMPLEMENT_DYNCREATE(DynamicRendererSampleView, CView)

BEGIN_MESSAGE_MAP(DynamicRendererSampleView, CView)
//{{AFX_MSG_MAP(DynamicRendererSampleView)
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
  ON_COMMAND(ID_DYNAMICRENDER_DRAWINGSURFACE, &DynamicRendererSampleView::OnDynamicrenderDrawingsurface)
  ON_UPDATE_COMMAND_UI(ID_DYNAMICRENDER_DRAWINGSURFACE, &DynamicRendererSampleView::OnUpdateDynamicrenderDrawingsurface)
  ON_COMMAND(ID_DYNAMICRENDER_DRAWINGSURFACE_LAND, &DynamicRendererSampleView::OnDynamicrenderDrawingsurfaceLand)
  ON_UPDATE_COMMAND_UI(ID_DYNAMICRENDER_DRAWINGSURFACE_LAND, &DynamicRendererSampleView::OnUpdateDynamicrenderDrawingsurfaceLand)
  ON_COMMAND(ID_DYNAMICRENDER_MAPLAYER, &DynamicRendererSampleView::OnDynamicrenderMaplayer)
  ON_UPDATE_COMMAND_UI(ID_DYNAMICRENDER_MAPLAYER, &DynamicRendererSampleView::OnUpdateDynamicrenderMaplayer)
  ON_COMMAND(ID_DYNAMICRENDER_MAPLAYER_ROADS, &DynamicRendererSampleView::OnDynamicrenderMaplayerRoads)
  ON_UPDATE_COMMAND_UI(ID_DYNAMICRENDER_MAPLAYER_ROADS, &DynamicRendererSampleView::OnUpdateDynamicrenderMaplayerRoads)
  ON_COMMAND(ID_DYNAMICRENDER_NORMALRENDERING, &DynamicRendererSampleView::OnDynamicrenderNormalrendering)
  ON_UPDATE_COMMAND_UI(ID_DYNAMICRENDER_NORMALRENDERING, &DynamicRendererSampleView::OnUpdateDynamicrenderNormalrendering)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DynamicRendererSampleView construction/destruction

DynamicRendererSampleView::DynamicRendererSampleView()
{
  // Clear my internal variables
  m_drawingSurface = 0 ; 
  m_modeManager    = 0 ;
  m_overlay = 0 ;
  m_mapLoaded = false ;
}

DynamicRendererSampleView::~DynamicRendererSampleView()
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

  if ( m_overlay )
  {
    m_overlay->destroy();
    m_overlay = 0 ;
  }
  TSLDrawingSurface::cleanup() ;
}

/////////////////////////////////////////////////////////////////////////////
// DynamicRendererSampleView drawing

void DynamicRendererSampleView::OnDraw(CDC* pDC)
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
// DynamicRendererSampleView printing
BOOL DynamicRendererSampleView::OnPreparePrinting(CPrintInfo* pInfo)
{
  pInfo->SetMaxPage(1);
  return DoPreparePrinting(pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// DynamicRendererSampleView diagnostics

#ifdef _DEBUG
void DynamicRendererSampleView::AssertValid() const
{
  CView::AssertValid();
}

void DynamicRendererSampleView::Dump(CDumpContext& dc) const
{
  CView::Dump(dc);
}

DynamicRendererSampleDoc* DynamicRendererSampleView::GetDocument() // non-debug version is inline
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(DynamicRendererSampleDoc)));
  return (DynamicRendererSampleDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// DynamicRendererSampleView message handlers

void DynamicRendererSampleView::OnInitialUpdate() 
{
  CView::OnInitialUpdate();
  
  DynamicRendererSampleDoc * doc = GetDocument() ;
  
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
    m_drawingSurface->setOption( TSLOptionDynamicArcSupportEnabled, true ) ;
    m_drawingSurface->wndResize( 0, 0, rect.Width(), rect.Height() ) ;

    // Now create and initialse the mode manager and modes
    m_modeManager = new TSLInteractionModeManagerNT( this, m_drawingSurface, m_hWnd ) ;
    m_modeManager->addMode( new TSLInteractionModeZoom( ID_TOOLS_ZOOM ), true ) ;
    m_modeManager->addMode( new TSLInteractionModePan( ID_TOOLS_PAN ), false ) ;
    m_modeManager->addMode( new TSLInteractionModeGrab( ID_TOOLS_GRAB ), false ) ;
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

  // If we've loaded the map, then get the relevant names and feature codes
  if ( m_mapLoaded )
  {
    m_mapLayerName = doc->m_mapLayerName ;
    TSLDataLayer * layer = m_drawingSurface->getDataLayer( m_mapLayerName ) ;
    const TSLFeatureClassList * list = layer->featureList() ;
    
    // Since we're loading the Dorset map for this sample, we'll hard-code
    // the feature names
    m_landFeatureCode = list->getID( "NetMap Low.coast1" ) ;
    m_roadFeatureCode = list->getID( "NetMap Low.aroad1" ) ;

    // Now add a standard data layer with some primitives in, so that we can
    // see whether the layer/drawing surface split is working correctly
    TSLTMC x1, y1, x2, y2 ;
    layer->getTMCExtent( &x1, &y1, &x2, &y2 ) ;
    TSLEnvelope extent( x1, y1, x2, y2 ) ;

    if ( !m_overlay )
    {
      m_overlay = new TSLStandardDataLayer ;

      TSLEntitySet * es = m_overlay->entitySet() ;

      TSLCoordSet * coords = new TSLCoordSet ;
      coords->add( x1, y1 ) ;
      coords->add( x2, y2 ) ;

      // Add other primitives to test the layer specific renderings
      long black = TSLDrawingSurface::getIDOfNearestColour( 0, 0, 0 ) ;
      long white = TSLDrawingSurface::getIDOfNearestColour( 255, 255, 255 ) ;

      TSLEntity * entity ;
      // Firstly, a polyline with the road feature code
      // This shouldn't change when the 'layer road' dynamic rendering is enabled
      entity = es->createPolyline( m_roadFeatureCode, coords ) ;
      entity->setRendering( TSLRenderingAttributeEdgeStyle, 1 ) ;
      entity->setRendering( TSLRenderingAttributeEdgeColour, black ) ;
      entity->setRendering( TSLRenderingAttributeEdgeThickness, 5 ) ;
      entity->setRendering( TSLRenderingAttributeEdgeThicknessUnits, TSLDimensionUnitsPixels ) ;
      coords->clear() ;

      // Next a polygon with the land feature code
      // This should change when the 'ds land' dynamic rendering is enabled, but not when the
      // layer rendering is enabled
      TSLEnvelope poly = extent ;
      poly.expand( (TSLTMC)(poly.width() / -3.0), (TSLTMC)(poly.height() / -3.0)) ;
      poly.translate( (TSLTMC)(-(poly.width() / 1.5)), (TSLTMC)(poly.height() / 1.5) ) ;
      coords->add( poly.xMin(), poly.yMin() ) ;
      coords->add( poly.xMin(), poly.yMax() ) ;
      coords->add( poly.xMax(), poly.yMax() ) ;
      coords->add( poly.xMax(), poly.yMin() ) ;
      
      entity = es->createPolygon( m_landFeatureCode, coords ) ;
      entity->setRendering( TSLRenderingAttributeEdgeStyle, 1 ) ;
      entity->setRendering( TSLRenderingAttributeEdgeColour, black ) ;
      entity->setRendering( TSLRenderingAttributeEdgeThickness, 5 ) ;
      entity->setRendering( TSLRenderingAttributeEdgeThicknessUnits, TSLDimensionUnitsPixels ) ;
      entity->setRendering( TSLRenderingAttributeFillStyle, 4 ) ;
      entity->setRendering( TSLRenderingAttributeFillColour, black ) ;

      // Next an arc
      entity = es->createArc( 0, 0.0, (TSLTMC)(3.1415 * 3.0 / 2.0), extent.centre().x(), extent.centre().y(), (TSLTMC)(extent.height() / 2.0) ) ;
      entity->setRendering( TSLRenderingAttributeEdgeStyle, 1 ) ;
      entity->setRendering( TSLRenderingAttributeEdgeColour, black ) ;
      entity->setRendering( TSLRenderingAttributeEdgeThickness, 5 ) ;
      entity->setRendering( TSLRenderingAttributeEdgeThicknessUnits, TSLDimensionUnitsPixels ) ;

      // Next an ellipse
      entity = es->createEllipse( 0, (TSLTMC)(extent.centre().x() + (extent.width() / 4.0)), 
				  (TSLTMC)(extent.centre().y() - (extent.height() / 4.0)), (TSLTMC)(extent.height() / 4.0) ) ;
      entity->setRendering( TSLRenderingAttributeEdgeStyle, 1 ) ;
      entity->setRendering( TSLRenderingAttributeEdgeColour, black ) ;
      entity->setRendering( TSLRenderingAttributeEdgeThickness, 5 ) ;
      entity->setRendering( TSLRenderingAttributeEdgeThicknessUnits, TSLDimensionUnitsPixels ) ;
      entity->setRendering( TSLRenderingAttributeFillStyle, 4 ) ;
      entity->setRendering( TSLRenderingAttributeFillColour, black ) ;

      // Next some text
      entity = es->createText( 0, (TSLTMC)(extent.centre().x() - (extent.width() / 3.0)), 
			       (TSLTMC)(extent.centre().y() - (extent.height() / 3.0)), 
			       "The chunky black primitives\nare in an overlay layer" ) ;
      entity->setRendering( TSLRenderingAttributeTextFont, 1 ) ;
      entity->setRendering( TSLRenderingAttributeTextColour, white ) ;
      entity->setRendering( TSLRenderingAttributeTextSizeFactor, 25.0 ) ;
      entity->setRendering( TSLRenderingAttributeTextSizeFactorUnits, TSLDimensionUnitsPixels ) ;
      entity->setRendering( TSLRenderingAttributeTextBackgroundMode, TSLTextBackgroundModeHalo ) ;
      entity->setRendering( TSLRenderingAttributeTextBackgroundColour, black ) ;

      coords->destroy() ;
    }
    m_drawingSurface->addDataLayer( m_overlay, "overlay" ) ;
  }

  // and reset the current view to display the entire map.
  m_drawingSurface->reset( false ) ;

  m_modeManager->resetViews( ) ;
  m_modeManager->resetMode( TSLButtonNone, 0, 0 ) ;
}

void DynamicRendererSampleView::OnToolsZoomin() 
{
  if ( m_modeManager )
  {
    if ( m_modeManager->zoomIn( 30 ) )
      InvalidateRect( 0, false ) ;
  }
}

void DynamicRendererSampleView::OnToolsZoomout() 
{
  if ( m_modeManager )
  {
    if ( m_modeManager->zoomOut( 30 ) )
      InvalidateRect( 0, false ) ;
  }
}

void DynamicRendererSampleView::OnToolsReset() 
{
  if ( m_modeManager )
  {
    if ( m_modeManager->resetToFullExtent( ) )
      InvalidateRect( 0, false ) ;
  }

}

void DynamicRendererSampleView::OnSize(UINT nType, int cx, int cy) 
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

void DynamicRendererSampleView::OnMouseMove(UINT nFlags, CPoint point) 
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
				     (nFlags & MK_SHIFT) == MK_SHIFT, (nFlags & MK_CONTROL) == MK_CONTROL ))
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( 0, false ) ;
    }
  }
  
  CView::OnMouseMove(nFlags, point);
}

void DynamicRendererSampleView::OnLButtonDown(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_modeManager )
  {
    if ( m_modeManager->onLButtonDown( point.x, point.y,
				       (nFlags & MK_SHIFT) == MK_SHIFT, (nFlags & MK_CONTROL) == MK_CONTROL )) 
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( 0, false ) ;
    }
  }
  
  CView::OnLButtonDown(nFlags, point);
}

void DynamicRendererSampleView::OnMButtonDown(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_modeManager )
  {
    if ( m_modeManager->onMButtonDown( point.x, point.y,
				       (nFlags & MK_SHIFT) == MK_SHIFT, (nFlags & MK_CONTROL) == MK_CONTROL )) 
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( 0, false ) ;
    }
  }
  
  CView::OnMButtonDown(nFlags, point);
}

void DynamicRendererSampleView::OnRButtonDown(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_modeManager )
  {
    if ( m_modeManager->onRButtonDown( point.x, point.y,
				       (nFlags & MK_SHIFT) == MK_SHIFT, (nFlags & MK_CONTROL) == MK_CONTROL )) 
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( 0, false ) ;
    }
  }
  
  CView::OnRButtonDown(nFlags, point);
}

void DynamicRendererSampleView::OnLButtonUp(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_modeManager )
  {
    if ( m_modeManager->onLButtonUp( point.x, point.y,
			             (nFlags & MK_SHIFT) == MK_SHIFT, (nFlags & MK_CONTROL) == MK_CONTROL )) 
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( 0, false ) ;
    }
  }
  
  CView::OnLButtonUp(nFlags, point);
}

void DynamicRendererSampleView::OnMButtonUp(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_modeManager )
  {
    if ( m_modeManager->onMButtonUp( point.x, point.y,
				     (nFlags & MK_SHIFT) == MK_SHIFT, (nFlags & MK_CONTROL) == MK_CONTROL )) 
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( 0, false ) ;
    }
  }
  
  
  CView::OnMButtonUp(nFlags, point);
}

void DynamicRendererSampleView::OnRButtonUp(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_modeManager )
  {
    if ( m_modeManager->onRButtonUp( point.x, point.y,
				     (nFlags & MK_SHIFT) == MK_SHIFT, (nFlags & MK_CONTROL) == MK_CONTROL )) 
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( 0, false ) ;
    }
  }
  
  
  CView::OnRButtonUp(nFlags, point);
}

void DynamicRendererSampleView::OnToolsPan() 
{
  if ( m_modeManager )
    m_modeManager->setCurrentMode( ID_TOOLS_PAN ) ;

}

void DynamicRendererSampleView::OnToolsZoom() 
{
  if ( m_modeManager )
    m_modeManager->setCurrentMode( ID_TOOLS_ZOOM ) ;

}

void DynamicRendererSampleView::OnToolsGrab() 
{
  if ( m_modeManager )
    m_modeManager->setCurrentMode( ID_TOOLS_GRAB ) ;
}

void DynamicRendererSampleView::OnUpdateToolsAction(CCmdUI* pCmdUI) 
{
  // Common update handler to all items that are dependant upon the map
  // being loaded.
  pCmdUI->Enable( m_mapLoaded ? true : false ) ;
}

void DynamicRendererSampleView::OnUpdateToolsMode(CCmdUI* pCmdUI) 
{
  // Common update handler to all items that are dependant upon the map
  // being loaded, and act as radios dependant upon the current mode.
  pCmdUI->Enable( m_mapLoaded ? true : false  ) ;
  pCmdUI->SetCheck( m_mapLoaded && m_modeManager && m_modeManager->getCurrentMode() == pCmdUI->m_nID ) ;
}

void DynamicRendererSampleView::OnEditCopy() 
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

void DynamicRendererSampleView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
  // Just handle like a normal press to avoid losing the second press
  OnLButtonDown( nFlags, point ) ;
  CView::OnLButtonDblClk(nFlags, point);
}

void DynamicRendererSampleView::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
  // Just handle like a normal press to avoid losing the second press
  OnRButtonDown( nFlags, point ) ;
  CView::OnRButtonDblClk(nFlags, point);
}
void DynamicRendererSampleView::OnMButtonDblClk(UINT nFlags, CPoint point) 
{
  // Just handle like a normal press to avoid losing the second press
  OnMButtonDown( nFlags, point ) ;
  CView::OnMButtonDblClk(nFlags, point);
}

void DynamicRendererSampleView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
  // Attempt to print using the PrintHelper, using the standard templates.
  //HDC printerDC = pDC->GetSafeHdc()  ;

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
    TSLDrawingSurface::findFile( templateFile, templateFilePath );

    // load the template
    if ( templateFilePath )
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
    //        because the extent may have changed to to DynamicRendererSample Print Preview taking control
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

BOOL DynamicRendererSampleView::OnEraseBkgnd(CDC* pDC) 
{
  // Since the drawing surface is double buffered, we don't need to erase the
  // background.
  return true;
}


void DynamicRendererSampleView::OnFilePrint() 
{
  // We should not query the extent to print during the OnPrint, since the window may change
  // size, especially in DynamicRendererSample print-preview
  if ( m_drawingSurface )
    m_drawingSurface->getTMCExtent( m_extentToPrint ) ;
  CView::OnFilePrint() ;
}

void DynamicRendererSampleView::OnFilePrintPreview() 
{
  // We should not query the extent to print during the OnPrint, since the window may change
  // size, especially in DynamicRendererSample print-preview
  if ( m_drawingSurface )
    m_drawingSurface->getTMCExtent( m_extentToPrint ) ;
  CView::OnFilePrintPreview() ;
}

void DynamicRendererSampleView::OnToolsMagnify()
{
  if ( m_modeManager )
    m_modeManager->setCurrentMode( ID_TOOLS_MAGNIFY ) ;
}

void DynamicRendererSampleView::resetMode(TSLInteractionMode * mode, TSLButtonType button, TSLDeviceUnits xDU, TSLDeviceUnits yDU)
{
  // Ignore the override
}

void DynamicRendererSampleView::viewChanged(TSLDrawingSurface* drawingSurface)
{
}

void DynamicRendererSampleView::OnSavedviewsGoto1()
{
  if ( m_modeManager )
  {
    if ( m_modeManager->savedViewGoto( 0 ) )
      InvalidateRect( 0, false ) ;
  }
}

void DynamicRendererSampleView::OnSavedviewsGoto2()
{
  if ( m_modeManager )
  {
    if ( m_modeManager->savedViewGoto( 1 ) )
      InvalidateRect( 0, false ) ;
  }
}

void DynamicRendererSampleView::OnSavedviewsGoto3()
{
  if ( m_modeManager )
  {
    if ( m_modeManager->savedViewGoto( 2 ) )
      InvalidateRect( 0, false ) ;
  }
}

void DynamicRendererSampleView::OnSavedviewsReset()
{
  if ( m_modeManager )
    m_modeManager->savedViewReset( ) ;
}

void DynamicRendererSampleView::OnSavedviewsSet1()
{
  if ( m_modeManager )
    m_modeManager->savedViewSetToCurrent( 0 ) ;
}

void DynamicRendererSampleView::OnUpdateSavedviewsReset(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( m_mapLoaded ? true : false  ) ;
}

void DynamicRendererSampleView::OnUpdateSavedviewsGoto3(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( m_mapLoaded && m_modeManager && m_modeManager->savedViewValid( 2 ) ) ;
}

void DynamicRendererSampleView::OnUpdateSavedviewsGoto2(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( m_mapLoaded && m_modeManager && m_modeManager->savedViewValid( 1 ) ) ;
}

void DynamicRendererSampleView::OnUpdateSavedviewsGoto1(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( m_mapLoaded && m_modeManager && m_modeManager->savedViewValid( 0 ) ) ;
}

void DynamicRendererSampleView::OnUpdateSavedviewsSet1(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( m_mapLoaded ? true : false  ) ;
}

void DynamicRendererSampleView::OnSavedviewsSet2()
{
  if ( m_modeManager )
    m_modeManager->savedViewSetToCurrent( 1 ) ;
}

void DynamicRendererSampleView::OnUpdateSavedviewsSet2(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( m_mapLoaded ? true : false  ) ;
}

void DynamicRendererSampleView::OnSavedviewsSet3()
{
  if ( m_modeManager )
    m_modeManager->savedViewSetToCurrent( 2 ) ;
}

void DynamicRendererSampleView::OnUpdateSavedviewsSet3(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( m_mapLoaded ? true : false  ) ;
}

void DynamicRendererSampleView::OnViewstackNextview()
{
  if ( m_modeManager )
  {
    if ( m_modeManager->viewStackGotoNext( ) )
      InvalidateRect( 0, false ) ;
  }
}

void DynamicRendererSampleView::OnUpdateViewstackNextview(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( (m_mapLoaded && m_modeManager && m_modeManager->viewStackNextValid( )) ? true : false ) ;
}

void DynamicRendererSampleView::OnViewstackPreviousview()
{
  if ( m_modeManager )
  {
    if ( m_modeManager->viewStackGotoPrevious( ) )
      InvalidateRect( 0, false ) ;
  }
}

void DynamicRendererSampleView::OnUpdateViewstackPreviousview(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( (m_mapLoaded && m_modeManager && m_modeManager->viewStackPreviousValid( )) ? true : false ) ;
}

BOOL DynamicRendererSampleView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
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

void DynamicRendererSampleView::OnDynamicrenderDrawingsurface()
{
  // Clear down any existing rendering
  OnDynamicrenderNormalrendering( ) ;

  // Now add the drawing surface one
  TSLDynamicRenderer * renderer = TSLDynamicRendererFactory::create( "myRenderer" ) ;

  if ( !renderer )
  {
    AfxMessageBox( _T("Failed to create  dynamic renderer") ) ;
    return ;
  }

  if ( !m_drawingSurface->addDynamicRenderer( renderer ) )
  {
    AfxMessageBox( _T("Failed to add dynamic renderer to surface") ) ;
    return ;
  }
  // The drawing surface now owns the renderer

  InvalidateRect( 0, false ) ;
}

void DynamicRendererSampleView::OnUpdateDynamicrenderDrawingsurface(CCmdUI *pCmdUI)
{
  if ( !m_mapLoaded )
    return;

  TSLDynamicRenderer * stored = m_drawingSurface->getDynamicRenderer( ) ;
  pCmdUI->SetCheck( stored ? 1 : 0 ) ;
}

void DynamicRendererSampleView::OnDynamicrenderDrawingsurfaceLand()
{
  // Clear down any existing rendering
  OnDynamicrenderNormalrendering( ) ;

  // Now add the drawing surface one
  TSLDynamicRenderer * renderer = TSLDynamicRendererFactory::create( "myRenderer" ) ;

  if ( !renderer )
  {
    AfxMessageBox( _T("Failed to create  dynamic renderer") ) ;
    return ;
  }

  if ( !m_drawingSurface->addDynamicRenderer( renderer, m_landFeatureCode ) )
  {
    AfxMessageBox( _T("Failed to add dynamic renderer to surface") ) ;
    return ;
  }
  // The drawing surface now owns the renderer

  InvalidateRect( 0, false ) ;
}

void DynamicRendererSampleView::OnUpdateDynamicrenderDrawingsurfaceLand(CCmdUI *pCmdUI)
{
  if ( !m_mapLoaded )
    return;

  TSLDynamicRenderer * stored = m_drawingSurface->getDynamicRenderer( m_landFeatureCode ) ;
  pCmdUI->SetCheck( stored ? 1 : 0 ) ;
}

void DynamicRendererSampleView::OnDynamicrenderMaplayer()
{
  // Clear down any existing rendering
  OnDynamicrenderNormalrendering( ) ;

  // Now add the drawing surface one
  TSLDynamicRenderer * renderer = TSLDynamicRendererFactory::create( "myRenderer" ) ;

  if ( !renderer )
  {
    AfxMessageBox( _T("Failed to create  dynamic renderer") ) ;
    return ;
  }

  if ( !m_drawingSurface->addDynamicRenderer( renderer, -1, m_mapLayerName ) )
  {
    AfxMessageBox( _T("Failed to add dynamic renderer to surface") ) ;
    return ;
  }
  // The drawing surface now owns the renderer

  InvalidateRect( 0, false ) ;

}

void DynamicRendererSampleView::OnUpdateDynamicrenderMaplayer(CCmdUI *pCmdUI)
{
  if ( !m_mapLoaded )
    return;

  TSLDynamicRenderer * stored = m_drawingSurface->getDynamicRenderer( -1, m_mapLayerName ) ;
  pCmdUI->SetCheck( stored ? 1 : 0 ) ;
}

void DynamicRendererSampleView::OnDynamicrenderMaplayerRoads()
{
  // Clear down any existing rendering
  OnDynamicrenderNormalrendering( ) ;

  // Now add the drawing surface one
  TSLDynamicRenderer * renderer = TSLDynamicRendererFactory::create( "myRenderer" ) ;

  if ( !renderer )
  {
    AfxMessageBox( _T("Failed to create  dynamic renderer") ) ;
    return ;
  }

  if ( !m_drawingSurface->addDynamicRenderer( renderer, m_roadFeatureCode, m_mapLayerName ) )
  {
    AfxMessageBox( _T("Failed to add dynamic renderer to surface") ) ;
    return ;
  }
  // The drawing surface now owns the renderer

  InvalidateRect( 0, false ) ;
}

void DynamicRendererSampleView::OnUpdateDynamicrenderMaplayerRoads(CCmdUI *pCmdUI)
{
  if ( !m_mapLoaded )
    return;

  TSLDynamicRenderer * stored = m_drawingSurface->getDynamicRenderer( m_roadFeatureCode, m_mapLayerName ) ;
  pCmdUI->SetCheck( stored ? 1 : 0 ) ;
}

void DynamicRendererSampleView::OnDynamicrenderNormalrendering()
{
  m_drawingSurface->clearDynamicRenderer( ) ;
  m_drawingSurface->clearDynamicRenderer( m_landFeatureCode ) ;
  m_drawingSurface->clearDynamicRenderer( -1, m_mapLayerName ) ;
  m_drawingSurface->clearDynamicRenderer( m_roadFeatureCode, m_mapLayerName ) ;
  InvalidateRect( 0, false ) ;
}

void DynamicRendererSampleView::OnUpdateDynamicrenderNormalrendering(CCmdUI *pCmdUI)
{
  if ( !m_mapLoaded )
    return;

  bool any =   m_drawingSurface->getDynamicRenderer(  )
            || m_drawingSurface->getDynamicRenderer( m_landFeatureCode )
            || m_drawingSurface->getDynamicRenderer( -1, m_mapLayerName )
            || m_drawingSurface->getDynamicRenderer( m_roadFeatureCode, m_mapLayerName ) ;

  pCmdUI->SetCheck( any ? 0 : 1 ) ;
}
