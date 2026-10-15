// SimpleEditorInteractionView.cpp : implementation of the SimpleEditorInteractionView class
//

#include "stdafx.h"
#include "SimpleEditorInteraction.h"
#include "SimpleEditorInteractionDoc.h"
#include "SimpleEditorInteractionView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SimpleEditorInteractionView

IMPLEMENT_DYNCREATE(SimpleEditorInteractionView, CView)

BEGIN_MESSAGE_MAP(SimpleEditorInteractionView, CView)
  //{{AFX_MSG_MAP(SimpleEditorInteractionView)
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
  ON_UPDATE_COMMAND_UI(ID_TOOLS_EDIT,    OnUpdateToolsMode)

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
  ON_COMMAND(ID_TOOLS_EDIT, OnToolsEdit)
  ON_WM_SETCURSOR()
  ON_COMMAND(ID_OPERATIONS_ATTRIBUTES, OnOperationsAttributes)
  ON_COMMAND(ID_OPERATIONS_DELETE, OnOperationsDelete)
  ON_COMMAND(ID_OPERATIONS_POLYGON, OnOperationsPolygon)
  ON_UPDATE_COMMAND_UI(ID_OPERATIONS_ATTRIBUTES, OnUpdateOperationsAttributes)
  ON_UPDATE_COMMAND_UI(ID_OPERATIONS_DELETE, OnUpdateOperationsDelete)
  ON_UPDATE_COMMAND_UI(ID_OPERATIONS_POLYGON, OnUpdateOperationsPolygon)
  ON_COMMAND(ID_OPERATIONS_POLYLINE, OnOperationsPolyline)
  ON_UPDATE_COMMAND_UI(ID_OPERATIONS_POLYLINE, OnUpdateOperationsPolyline)
  ON_COMMAND(ID_OPERATIONS_SELECTALL, OnOperationsSelectall)
  ON_UPDATE_COMMAND_UI(ID_OPERATIONS_SELECTALL, OnUpdateOperationsSelectall)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SimpleEditorInteractionView construction/destruction

SimpleEditorInteractionView::SimpleEditorInteractionView()
{
  // Clear my internal variables
  m_drawingSurface = 0 ; 
  m_modeManager    = 0 ;
  m_editMode       = 0 ;
  m_mapLoaded = false ;
  m_initialiseRendering = true ;
  m_numEntities = 0 ;
  m_selDepth = 0 ;
  m_attributePanel = 0 ;
}

SimpleEditorInteractionView::~SimpleEditorInteractionView()
{
  // Deleting the mode manager also deletes all of the modes
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

  if ( m_attributePanel )
  {
    delete m_attributePanel ;
    m_attributePanel = 0 ;
  }
  TSLDrawingSurface::cleanup() ;
}

/////////////////////////////////////////////////////////////////////////////
// SimpleEditorInteractionView drawing

void SimpleEditorInteractionView::OnDraw(CDC* pDC)
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
// SimpleEditorInteractionView printing
BOOL SimpleEditorInteractionView::OnPreparePrinting(CPrintInfo* pInfo)
{
  // default preparation
  pInfo->SetMaxPage(1);
  return DoPreparePrinting(pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// SimpleEditorInteractionView diagnostics

#ifdef _DEBUG
void SimpleEditorInteractionView::AssertValid() const
{
  CView::AssertValid();
}

void SimpleEditorInteractionView::Dump(CDumpContext& dc) const
{
  CView::Dump(dc);
}

SimpleEditorInteractionDoc* SimpleEditorInteractionView::GetDocument() // non-debug version is inline
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(SimpleEditorInteractionDoc)));
  return (SimpleEditorInteractionDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// SimpleEditorInteractionView message handlers

void SimpleEditorInteractionView::OnInitialUpdate() 
{
  CView::OnInitialUpdate();

  SimpleEditorInteractionDoc * doc = GetDocument() ;

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
    m_modeManager = new TSLInteractionModeManagerNT( this, m_drawingSurface, m_hWnd, 5, 5, 30, true ) ;
    m_modeManager->addMode( new TSLInteractionModeZoom( ID_TOOLS_ZOOM ), false ) ;
    m_modeManager->addMode( new TSLInteractionModePan( ID_TOOLS_PAN ), false ) ;
    m_modeManager->addMode( new TSLInteractionModeGrab( ID_TOOLS_GRAB ), false ) ;
    m_modeManager->addMode( new TSLInteractionModeMagnify( ID_TOOLS_MAGNIFY ), false ) ;

    const char * maplinkHome = TSLUtilityFunctions::getMapLinkHome() ;
    CString iniFile ;
    if ( maplinkHome )
    {
      iniFile.Append( (LPCTSTR)TSLUTF8Decoder(maplinkHome) );
      iniFile.Append( _T("\\samples\\nt\\simpleeditorinteraction\\editor.ini") );
    }
    else
    {
      // Default to a standard installation, assuming running from current project dir
      iniFile = _T("editor.ini");
    }

    m_editMode = new TSLInteractionModeEdit( ID_TOOLS_EDIT, this, this, TSLUTF8Encoder((LPCTSTR)iniFile) ) ;
    m_editMode->allowEditing( false ) ;
    m_modeManager->addMode( m_editMode, true ) ;
  }
  else if ( m_mapLoaded ) 
  {
    // Reset the Editor
    m_editMode->editor()->reset() ;

  // We have a drawing surface with a map loaded into it.
    // Detach the map from the surface
    if ( m_drawingSurface && doc )
      doc->detachFromSurface( m_drawingSurface ) ;
  }

  // If the document loaded successfully, add it to the drawing surface
  m_mapLoaded = doc->addToSurface( m_drawingSurface ) ;

  if ( m_mapLoaded )
  {
    m_editMode->allowEditing( true ) ;

    m_editMode->editor()->dataChanged() ;

    if ( m_initialiseRendering)
    {
      // Set the default attributes
      TSLRenderingAttributes attributes ;

      attributes.m_edgeColour = 181 ;
      attributes.m_edgeStyle = 1 ;
      attributes.m_edgeThickness = 1.0 ;
      attributes.m_edgeThicknessUnits = TSLDimensionUnitsPixels ;
      attributes.m_exteriorEdgeColour = 181 ;
      attributes.m_exteriorEdgeStyle = 1 ;
      attributes.m_exteriorEdgeThickness = 1.0 ;
      attributes.m_exteriorEdgeThicknessUnits = TSLDimensionUnitsPixels ;
      attributes.m_fillColour = 4 ;
      attributes.m_fillStyle = 503 ;

      m_editMode->editor()->activate( "renderingattributes", &attributes ) ;

      if ( !m_attributePanel )
      {
        // We're not interested in bordered polygons here - that's only in the Spatial SDK really
        m_attributePanel = new TSLRenderingAttributePanel( attributePanelCallback, GetSafeHwnd(), false ) ;
        m_attributePanel->setRenderingAttributes( &attributes, this ) ;
        m_attributePanel->showDialog( true ) ;
      }
      m_initialiseRendering = false ;
    } 
  }

  // and reset the current view to display the entire map.
  m_drawingSurface->reset( false ) ;

  m_modeManager->resetViews( ) ;
  m_modeManager->resetMode( TSLButtonNone, 0, 0 ) ;

}

void SimpleEditorInteractionView::OnToolsZoomin() 
{
  if ( m_modeManager )
  {
    if ( m_modeManager->zoomIn( 30 ) )
      InvalidateRect( 0, false ) ;
  }
}

void SimpleEditorInteractionView::OnToolsZoomout() 
{
  if ( m_modeManager )
  {
    if ( m_modeManager->zoomOut( 30 ) )
      InvalidateRect( 0, false ) ;
  }
}

void SimpleEditorInteractionView::OnToolsReset() 
{
  if ( m_modeManager )
  {
    if ( m_modeManager->resetToFullExtent( ) )
      InvalidateRect( 0, false ) ;
  }

}

void SimpleEditorInteractionView::OnSize(UINT nType, int cx, int cy) 
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

void SimpleEditorInteractionView::OnMouseMove(UINT nFlags, CPoint point) 
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

void SimpleEditorInteractionView::OnLButtonDown(UINT nFlags, CPoint point) 
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

void SimpleEditorInteractionView::OnMButtonDown(UINT nFlags, CPoint point) 
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

void SimpleEditorInteractionView::OnRButtonDown(UINT nFlags, CPoint point) 
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

void SimpleEditorInteractionView::OnLButtonUp(UINT nFlags, CPoint point) 
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

void SimpleEditorInteractionView::OnMButtonUp(UINT nFlags, CPoint point) 
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

void SimpleEditorInteractionView::OnRButtonUp(UINT nFlags, CPoint point) 
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

void SimpleEditorInteractionView::OnToolsPan() 
{
  if ( m_modeManager )
    m_modeManager->setCurrentMode( ID_TOOLS_PAN ) ;

}

void SimpleEditorInteractionView::OnToolsZoom() 
{
  if ( m_modeManager )
    m_modeManager->setCurrentMode( ID_TOOLS_ZOOM ) ;

}

void SimpleEditorInteractionView::OnToolsGrab() 
{
  if ( m_modeManager )
    m_modeManager->setCurrentMode( ID_TOOLS_GRAB ) ;
}

void SimpleEditorInteractionView::OnUpdateToolsAction(CCmdUI* pCmdUI) 
{
  // Common update handler to all items that are dependant upon the map
  // being loaded.
  pCmdUI->Enable( m_mapLoaded ? true : false ) ;
}

void SimpleEditorInteractionView::OnUpdateToolsMode(CCmdUI* pCmdUI) 
{
  // Common update handler to all items that are dependant upon the map
  // being loaded, and act as radios dependant upon the current mode.
  pCmdUI->Enable( m_mapLoaded ? true : false  ) ;
  pCmdUI->SetCheck( m_mapLoaded && m_modeManager && m_modeManager->getCurrentMode() == (int)pCmdUI->m_nID ) ;
}

void SimpleEditorInteractionView::OnEditCopy() 
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

void SimpleEditorInteractionView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
  // Just handle like a normal press to avoid losing the second press
  OnLButtonDown( nFlags, point ) ;
  CView::OnLButtonDblClk(nFlags, point);
}

void SimpleEditorInteractionView::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
  // Just handle like a normal press to avoid losing the second press
  OnRButtonDown( nFlags, point ) ;
  CView::OnRButtonDblClk(nFlags, point);
}
void SimpleEditorInteractionView::OnMButtonDblClk(UINT nFlags, CPoint point) 
{
  // Just handle like a normal press to avoid losing the second press
  OnMButtonDown( nFlags, point ) ;
  CView::OnMButtonDblClk(nFlags, point);
}

void SimpleEditorInteractionView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
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
    if ( TSLDrawingSurface::findFile( templateFile, templateFilePath ) )
    {
      // load the template
      tmp = TSLPrintHelper::loadTemplate( templateFilePath ) ;
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
    //        because the extent may have changed to to SimpleEditorInteraction Print Preview taking control
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

BOOL SimpleEditorInteractionView::OnEraseBkgnd(CDC* pDC) 
{
  // Since the drawing surface is double buffered, we don't need to erase the
  // background.
  return true;
}


void SimpleEditorInteractionView::OnFilePrint() 
{
  // We should not query the extent to print during the OnPrint, since the window may change
  // size, especially in SimpleEditorInteraction print-preview
  if ( m_drawingSurface )
    m_drawingSurface->getTMCExtent( m_extentToPrint ) ;
  CView::OnFilePrint() ;
}

void SimpleEditorInteractionView::OnFilePrintPreview() 
{
  // We should not query the extent to print during the OnPrint, since the window may change
  // size, especially in SimpleEditorInteraction print-preview
  if ( m_drawingSurface )
    m_drawingSurface->getTMCExtent( m_extentToPrint ) ;
  CView::OnFilePrintPreview() ;
}

void SimpleEditorInteractionView::OnToolsMagnify()
{
  if ( m_modeManager )
    m_modeManager->setCurrentMode( ID_TOOLS_MAGNIFY ) ;
}

void SimpleEditorInteractionView::resetMode(TSLInteractionMode * mode, TSLButtonType button, TSLDeviceUnits xDU, TSLDeviceUnits yDU)
{
  // Ignore the override
}

void SimpleEditorInteractionView::viewChanged(TSLDrawingSurface* drawingSurface)
{
  static int cnt = 1 ;

  TRACE1( "View change %d\n", cnt++ ) ;
}

void SimpleEditorInteractionView::OnSavedviewsGoto1()
{
  if ( m_modeManager )
  {
    if ( m_modeManager->savedViewGoto( 0 ) )
      InvalidateRect( 0, false ) ;
  }
}

void SimpleEditorInteractionView::OnSavedviewsGoto2()
{
  if ( m_modeManager )
  {
    if ( m_modeManager->savedViewGoto( 1 ) )
      InvalidateRect( 0, false ) ;
  }
}

void SimpleEditorInteractionView::OnSavedviewsGoto3()
{
  if ( m_modeManager )
  {
    if ( m_modeManager->savedViewGoto( 2 ) )
      InvalidateRect( 0, false ) ;
  }
}

void SimpleEditorInteractionView::OnSavedviewsReset()
{
  if ( m_modeManager )
    m_modeManager->savedViewReset( ) ;
}

void SimpleEditorInteractionView::OnSavedviewsSet1()
{
  if ( m_modeManager )
    m_modeManager->savedViewSetToCurrent( 0 ) ;
}

void SimpleEditorInteractionView::OnUpdateSavedviewsReset(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( m_mapLoaded ? true : false  ) ;
}

void SimpleEditorInteractionView::OnUpdateSavedviewsGoto3(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( m_mapLoaded && m_modeManager && m_modeManager->savedViewValid( 2 ) ) ;
}

void SimpleEditorInteractionView::OnUpdateSavedviewsGoto2(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( m_mapLoaded && m_modeManager && m_modeManager->savedViewValid( 1 ) ) ;
}

void SimpleEditorInteractionView::OnUpdateSavedviewsGoto1(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( m_mapLoaded && m_modeManager && m_modeManager->savedViewValid( 0 ) ) ;
}

void SimpleEditorInteractionView::OnUpdateSavedviewsSet1(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( m_mapLoaded ? true : false  ) ;
}

void SimpleEditorInteractionView::OnSavedviewsSet2()
{
  if ( m_modeManager )
    m_modeManager->savedViewSetToCurrent( 1 ) ;
}

void SimpleEditorInteractionView::OnUpdateSavedviewsSet2(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( m_mapLoaded ? true : false  ) ;
}

void SimpleEditorInteractionView::OnSavedviewsSet3()
{
  if ( m_modeManager )
    m_modeManager->savedViewSetToCurrent( 2 ) ;
}

void SimpleEditorInteractionView::OnUpdateSavedviewsSet3(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( m_mapLoaded ? true : false  ) ;
}

void SimpleEditorInteractionView::OnViewstackNextview()
{
  if ( m_modeManager )
  {
    if ( m_modeManager->viewStackGotoNext( ) )
      InvalidateRect( 0, false ) ;
  }
}

void SimpleEditorInteractionView::OnUpdateViewstackNextview(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( (m_mapLoaded && m_modeManager && m_modeManager->viewStackNextValid( )) ? true : false ) ;
}

void SimpleEditorInteractionView::OnViewstackPreviousview()
{
  if ( m_modeManager )
  {
    if ( m_modeManager->viewStackGotoPrevious( ) )
      InvalidateRect( 0, false ) ;
  }
}

void SimpleEditorInteractionView::OnUpdateViewstackPreviousview(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( (m_mapLoaded && m_modeManager && m_modeManager->viewStackPreviousValid( )) ? true : false ) ;
}

BOOL SimpleEditorInteractionView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
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

void SimpleEditorInteractionView::invokeContextMenu (TSLEditor* editor, TSLDeviceUnits xDU, TSLDeviceUnits yDU)
{
}

bool SimpleEditorInteractionView::onSelectionChanged (int numEntities, int depth)
{
  m_numEntities = numEntities ;
  m_selDepth = depth ;

  TSLRenderingAttributes attributes ;

  const char * panelTitle = "" ;
  if ( numEntities )
  {
    m_editMode->editor()->querySelectedAttributes( attributes ) ;
    panelTitle = "Select list attributes" ;
  }
  else
  {
    TSLVariant value;
    if ( m_editMode->editor()->query( "renderingattributes", &value ) )
    {
      if (TSLVariantTypePtr == value.getType())
      {
        void *ptr = NULL;
        if (value.getVal(&ptr))
        {
          if (ptr != NULL)
          {
            TSLRenderingAttributes * defaultAttributes = reinterpret_cast< TSLRenderingAttributes * >(ptr);
            if ( defaultAttributes )
            {
              panelTitle = "Default attributes" ;
              attributes = *defaultAttributes ;
            }
          }
        }
      }
    }    
  }

  if ( m_attributePanel )
  {
    m_attributePanel->setDialogTitle( panelTitle ) ;
    m_attributePanel->setRenderingAttributes( &attributes, this ) ;
  }

  return true ;
}

bool SimpleEditorInteractionView::requestDialog (const char* prompt, const char* label1, const char* label2, const char* label3, const char* label4)
{
  return false ;
}

bool SimpleEditorInteractionView::requestText (const char* prompt, const char* initialValue)
{
  return false ;
}

bool SimpleEditorInteractionView::displayError (const char* message)
{
  AfxMessageBox( (LPCTSTR)TSLUTF8Decoder(message), S_OK ) ;
  return true ;
}

void SimpleEditorInteractionView::OnToolsEdit()
{
  if ( m_modeManager )
    m_modeManager->setCurrentMode( ID_TOOLS_EDIT ) ;
}

int SimpleEditorInteractionView::querySelectList( int & depth )
{
  depth = m_selDepth ;
  return m_numEntities ;
}

const char * SimpleEditorInteractionView::queryPrompt( )
{
  return m_modeManager ? m_modeManager->queryPrompt( ) : "" ;
}

BOOL SimpleEditorInteractionView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
  if ( m_modeManager )
  {
    // Currently only maps to the standard cursors
    TSLCursorStyle style = m_modeManager->queryCursor() ;

    switch ( style )
    {
    case TSLCursorStyleScaleNorth_South:
      SetCursor( AfxGetApp()->LoadStandardCursor(IDC_SIZENS) ) ;
      return TRUE ;

    case TSLCursorStyleScaleEast_West:
      SetCursor( AfxGetApp()->LoadStandardCursor(IDC_SIZEWE) ) ;
      return TRUE ;

    case TSLCursorStyleScaleNorthEast_SouthWest:
      SetCursor( AfxGetApp()->LoadStandardCursor(IDC_SIZENESW) ) ;
      return TRUE ;

    case TSLCursorStyleScaleNorthWest_SouthEast:
      SetCursor( AfxGetApp()->LoadStandardCursor(IDC_SIZENWSE) ) ;
      return TRUE ;

    case TSLCursorStyleScaleEqual:
    case TSLCursorStyleScaleUnequal:
    case TSLCursorStyleMoveEntity:
      SetCursor( AfxGetApp()->LoadStandardCursor(IDC_SIZEALL) ) ;
      return TRUE ;

    case TSLCursorStyleBusy:
      SetCursor( AfxGetApp()->LoadStandardCursor(IDC_WAIT) ) ;
      return TRUE ;

    case TSLCursorStyleMagnify:
      SetCursor( AfxGetApp()->LoadStandardCursor(IDC_CROSS) ) ;
      return TRUE ;

    case TSLCursorStyleRotate:
    case TSLCursorStylePanToPoint:
    case TSLCursorStyleZoomToRectangle:

    case TSLCursorStyleNone:
    case TSLCursorStyleSelectEntity:
    case TSLCursorStyleSelectPosition:
    case TSLCursorStyleSelectPoint:
    case TSLCursorStyleDraw:
    case TSLCursorStyleMovePoint:
    case TSLCursorStyleEnterText:
    case TSLCursorStyleQuery:
    case TSLCursorStyleGrab:
      // These all do the basic
      break ;

    }
  }

  return CView::OnSetCursor(pWnd, nHitTest, message);
}

void SimpleEditorInteractionView::OnOperationsAttributes()
{
  // Set the default attributes 
  TSLRenderingAttributes attributes ;
  const char * title = NULL;
  if ( !m_attributePanel || !m_attributePanel->isShown() )
  {
    // Horrible I know, sorry...
    if ( m_editMode->editor()->numSelected() )
    {
      m_editMode->editor()->querySelectedAttributes( attributes ) ;
      title = "Select list attributes" ;
    }
    else
    {
      TSLVariant value;
      if ( m_editMode->editor()->query( "renderingattributes", &value ) )
      {
        if (TSLVariantTypePtr == value.getType())
        {
          void *ptr = NULL;
          if (value.getVal(&ptr))
          {
            if (ptr != NULL)
            {
              TSLRenderingAttributes * defaultAttributes = reinterpret_cast< TSLRenderingAttributes * >(ptr);
              if ( defaultAttributes )
              {
                title = "Default attributes" ;
                attributes = *defaultAttributes ;
              }
            }
          }
        }
      } 
    }
  }

  if ( m_attributePanel )
  {
    bool isCurrentlyShown = m_attributePanel->isShown() ;

    // We're not interested in bordered polygons here - that's only in the Spatial SDK really
    if ( !isCurrentlyShown )
    {
      m_attributePanel->setRenderingAttributes( &attributes, this ) ;
      m_attributePanel->setDialogTitle( title ) ;
    }
    m_attributePanel->showDialog( !isCurrentlyShown ) ;
  }
  else
  {


    // We're not interested in bordered polygons here - that's only in the Spatial SDK really
    m_attributePanel = new TSLRenderingAttributePanel( attributePanelCallback, GetSafeHwnd(), false ) ;
    m_attributePanel->setRenderingAttributes( &attributes, this ) ;
    m_attributePanel->setDialogTitle( title ) ;
    m_attributePanel->showDialog( true ) ;
  }
}

void SimpleEditorInteractionView::OnOperationsDelete()
{
  m_editMode->editor()->activate( "delete" ) ;
}

void SimpleEditorInteractionView::OnOperationsPolygon()
{
  m_editMode->editor()->activate( "polygon", 0 ) ;
}

void SimpleEditorInteractionView::OnUpdateOperationsAttributes(CCmdUI *pCmdUI)
{
}

void SimpleEditorInteractionView::OnUpdateOperationsDelete(CCmdUI *pCmdUI)
{
  if ( !m_editMode )
  {
    pCmdUI->Enable( FALSE ) ;
    return ;
  }

  pCmdUI->Enable( m_mapLoaded && m_editMode->editor()->activatePossible( "delete" ) ) ;
}

void SimpleEditorInteractionView::OnUpdateOperationsPolygon(CCmdUI *pCmdUI)
{
  if ( !m_editMode )
  {
    pCmdUI->Enable( FALSE ) ;
    return ;
  }

  pCmdUI->Enable( m_mapLoaded && m_editMode->editor()->activatePossible( "polygon" ) ) ;
  pCmdUI->SetCheck( m_mapLoaded && !strcmp( m_editMode->editor()->activeOperation(), "polygon" ) ) ;
}

void SimpleEditorInteractionView::OnOperationsPolyline()
{
  m_editMode->editor()->activate( "polyline", 0 ) ;
}

void SimpleEditorInteractionView::OnUpdateOperationsPolyline(CCmdUI *pCmdUI)
{
  if ( !m_editMode )
  {
    pCmdUI->Enable( FALSE ) ;
    return ;
  }

  pCmdUI->Enable( m_mapLoaded && m_editMode->editor()->activatePossible( "polyline" ) ) ;
  pCmdUI->SetCheck( m_mapLoaded && !strcmp( m_editMode->editor()->activeOperation(), "polyline" ) ) ;
}

void SimpleEditorInteractionView::OnOperationsSelectall()
{
  m_editMode->editor()->activate( "selectall" ) ;
}

void SimpleEditorInteractionView::OnUpdateOperationsSelectall(CCmdUI *pCmdUI)
{
  if ( !m_editMode )
  {
    pCmdUI->Enable( FALSE ) ;
    return ;
  }

  pCmdUI->Enable( m_mapLoaded && m_editMode->editor()->activatePossible( "selectall" ) ) ;
  pCmdUI->SetCheck( m_mapLoaded && !strcmp( m_editMode->editor()->activeOperation(), "selectall" ) ) ;
}

void SimpleEditorInteractionView::attributePanelCallback( void * arg, TSLRenderingAttributes * attribs )
{
  SimpleEditorInteractionView * self = (SimpleEditorInteractionView *)arg ;

  self->m_editMode->editor()->activate( "renderingattributes", attribs ) ;
}

