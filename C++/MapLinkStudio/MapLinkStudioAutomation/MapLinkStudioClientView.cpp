// MapLinkStudioClientView.cpp : implementation of the CMapLinkStudioClientView class
//

#include "stdafx.h"
#include "MapLinkStudioClient.h"

#include "MapLinkStudioClientDoc.h"
#include "MapLinkStudioClientView.h"
#include "MISourceSelectionDlg.h"
#include "MIDataFilePropsDlg.h"
#include "MIProcessingControlDlg.h"

#include "tslviewmodeinterfacent.h"
#include "tslviewmodepan.h"
#include "tslviewmodezoom.h"
#include "tslviewmodegrab.h"
#include "tslviewmodezoompan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapLinkStudioClientView

IMPLEMENT_DYNCREATE(CMapLinkStudioClientView, CView)

BEGIN_MESSAGE_MAP(CMapLinkStudioClientView, CView)
  //{{AFX_MSG_MAP(CMapLinkStudioClientView)
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
  ON_UPDATE_COMMAND_UI(ID_IMPORT_MAP, OnUpdateToolsMode)
  ON_COMMAND(ID_IMPORT_MAP, OnImportMap)
  //}}AFX_MSG_MAP

// Our common handles can update multiple items.
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

// Standard printing commands
ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)

END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMapLinkStudioClientView construction/destruction

CMapLinkStudioClientView::CMapLinkStudioClientView()
{
  // Clear my internal variables
  m_drawingSurface = 0; 
  m_currentMode = 0;
  m_modeInterface = 0;
  m_dataLayer = 0;
  m_mapLoaded = false;
  m_sourceSelectionDialog   = NULL ; 
  m_processingControlDialog = NULL ;
}

CMapLinkStudioClientView::~CMapLinkStudioClientView()
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
  if ( m_dataLayer )
  {
    m_dataLayer->destroy();
    m_dataLayer = 0;
  }
  if ( m_drawingSurface )
  {
    delete m_drawingSurface ;
    m_drawingSurface = 0 ;
    TSLDrawingSurface::cleanup() ;
  }
  if ( m_sourceSelectionDialog )
  {
    delete m_sourceSelectionDialog;
    m_sourceSelectionDialog = 0;
  }
  if ( m_processingControlDialog )
  {
    delete m_processingControlDialog;
    m_processingControlDialog = 0;
  }
}

/////////////////////////////////////////////////////////////////////////////
// CMapLinkStudioClientView drawing

void CMapLinkStudioClientView::OnDraw(CDC* pDC)
{
  ASSERT( m_drawingSurface ) ;
  
  if ( m_drawingSurface )
  {
    RECT rect ;
    
    // Only draw the exposed bits of the display
    if ( pDC->GetClipBox( &rect ) == NULLREGION )
      GetClientRect( &rect ) ;
    
    m_drawingSurface->drawDU( rect.left, rect.bottom, rect.right, rect.top, true, false ) ;
    
    // Don't forget to draw any echo rectangle that may be active.
    m_modeInterface->drawEchoRectangle( ) ;
  }
}

/////////////////////////////////////////////////////////////////////////////
// CMapLinkStudioClientView printing
BOOL CMapLinkStudioClientView::OnPreparePrinting(CPrintInfo* pInfo)
{
  // default preparation
  return DoPreparePrinting(pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CMapLinkStudioClientView diagnostics

#ifdef _DEBUG
void CMapLinkStudioClientView::AssertValid() const
{
  CView::AssertValid();
}

void CMapLinkStudioClientView::Dump(CDumpContext& dc) const
{
  CView::Dump(dc);
}

CMapLinkStudioClientDoc* CMapLinkStudioClientView::GetDocument() // non-debug version is inline
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMapLinkStudioClientDoc)));
  return (CMapLinkStudioClientDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMapLinkStudioClientView message handlers

void CMapLinkStudioClientView::OnInitialUpdate() 
{
  CView::OnInitialUpdate();
  
  CMapLinkStudioClientDoc * doc = GetDocument() ;

  // if we have no datalayer
  if (!m_dataLayer)
  {
    m_dataLayer = new TSLStandardDataLayer();
  }
  
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
    // Detach the map from the surface
    if ( m_drawingSurface && doc )
      doc->detachFromSurface( m_drawingSurface ) ;
  }
  
  // If the document loaded successfully, add it to the drawing surface
  m_mapLoaded = doc->addToSurface( m_drawingSurface ) ;
  
  // and reset the current view to display the entire map.
  m_drawingSurface->reset( false ) ;
    
  // Reset the current interaction mode by deleting it.
  if ( m_currentMode )
  {
    delete m_currentMode ;
    m_currentMode = 0 ;
  }
}

void CMapLinkStudioClientView::OnToolsZoomin() 
{
  if ( m_drawingSurface )
  {
    // Zoom in by 30%, and request a window redraw
    m_drawingSurface->zoom( 30, true, false ) ;
    InvalidateRect( 0, false ) ;
  }
}

void CMapLinkStudioClientView::OnToolsZoomout() 
{
  if ( m_drawingSurface )
  {
    // Zoom out by 30%, and request a window redraw
    m_drawingSurface->zoom( 30, false, false ) ;
    InvalidateRect( 0, false ) ;
  }
}

void CMapLinkStudioClientView::OnToolsReset() 
{
  if ( m_drawingSurface )
  {
    // Reset the window to display the entire map, and request a window redraw.
    m_drawingSurface->reset( false ) ;
    InvalidateRect( 0, false ) ;
  }
}

void CMapLinkStudioClientView::OnSize(UINT nType, int cx, int cy) 
{
  CView::OnSize(nType, cx, cy);
  
  if ( m_drawingSurface )
  {
    // Inform the drawing surface of the new window size,
    // attempting to keep the top left corner the same.
    // Do not ask for an automatic redraw since we will get an OnDraw call
    m_drawingSurface->wndResize( 0, 0, cx, cy, false, 
				 TSLResizeActionMaintainTopLeft ) ;
  }
}

void CMapLinkStudioClientView::OnMouseMove(UINT nFlags, CPoint point) 
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
  }
  
  CView::OnMouseMove(nFlags, point);
}

void CMapLinkStudioClientView::OnLButtonDown(UINT nFlags, CPoint point) 
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

void CMapLinkStudioClientView::OnMButtonDown(UINT nFlags, CPoint point) 
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

void CMapLinkStudioClientView::OnRButtonDown(UINT nFlags, CPoint point) 
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

void CMapLinkStudioClientView::OnLButtonUp(UINT nFlags, CPoint point) 
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

void CMapLinkStudioClientView::OnMButtonUp(UINT nFlags, CPoint point) 
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

void CMapLinkStudioClientView::OnRButtonUp(UINT nFlags, CPoint point) 
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

void CMapLinkStudioClientView::OnToolsPan() 
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
    m_currentMode = new TSLViewModePan( m_modeInterface, ID_TOOLS_PAN ) ;
}

void CMapLinkStudioClientView::OnToolsZoom() 
{
  // If zoom mode is currently active, then just deactivate it,
  // otherwise deactivate any current mode and activate zoom mode
  bool active = m_currentMode && m_currentMode->id() == ID_TOOLS_ZOOM ;
  
  if ( m_currentMode )
  {
    delete m_currentMode ;
    m_currentMode = 0 ;
  }
  
  // Pass the menu item ID to the mode so that the update handler works
  if ( !active )
    m_currentMode = new TSLViewModeZoom( m_modeInterface, ID_TOOLS_ZOOM ) ;
}

void CMapLinkStudioClientView::OnToolsZoompan() 
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
    m_currentMode = new TSLViewModeZoomPan( m_modeInterface, ID_TOOLS_ZOOMPAN ) ;
}

void CMapLinkStudioClientView::OnUpdateToolsAction(CCmdUI* pCmdUI) 
{
  // Common update handler to all items that are dependant upon the map
  // being loaded.
  pCmdUI->Enable( m_mapLoaded ? true : false ) ;
}

void CMapLinkStudioClientView::OnUpdateToolsMode(CCmdUI* pCmdUI) 
{
  // Common update handler to all items that are dependant upon the map
  // being loaded, and act as radios dependant upon the current mode.
  pCmdUI->Enable( m_mapLoaded ? true : false  ) ;
  pCmdUI->SetCheck( m_currentMode && m_currentMode->id() == pCmdUI->m_nID ) ;
  
  if (pCmdUI->m_nID == ID_IMPORT_MAP && theApp.getCoordinateSystem().size())
    pCmdUI->Enable(true);
}

void CMapLinkStudioClientView::OnEditCopy() 
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

void CMapLinkStudioClientView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
  // Just handle like a normal press to avoid losing the second press
  OnLButtonDown( nFlags, point ) ;
  CView::OnLButtonDblClk(nFlags, point);
}

void CMapLinkStudioClientView::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
  // Just handle like a normal press to avoid losing the second press
  OnRButtonDown( nFlags, point ) ;
  CView::OnRButtonDblClk(nFlags, point);
}
void CMapLinkStudioClientView::OnMButtonDblClk(UINT nFlags, CPoint point) 
{
  // Just handle like a normal press to avoid losing the second press
  OnMButtonDown( nFlags, point ) ;
  CView::OnMButtonDblClk(nFlags, point);
}

void CMapLinkStudioClientView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
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

    // search for the file in the drawing surface's pathlists
    if ( TSLDrawingSurface::findFile( templateFile, templateFilePath ) )
    {
      // load the template
      tmp = TSLPrintHelper::loadTemplate( templateFilePath, TSLDrawingSurface::getLoader(), 0 ) ;
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
    item.setBorder( 1, 1,3 ) ;
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
	
BOOL CMapLinkStudioClientView::OnEraseBkgnd(CDC* pDC) 
{
  // Since the drawing surface is double buffered, we don't need to erase the
  // background.
  return true;
}

void CMapLinkStudioClientView::OnToolsGrab() 
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
    m_currentMode = new TSLViewModeGrab( m_modeInterface, ID_TOOLS_GRAB ) ;
}

bool CMapLinkStudioClientView::addMapToSurface(LPCTSTR lpszPathName)
{
  CMapLinkStudioClientDoc* doc = GetDocument();
  doc->addMapToSurface(lpszPathName, m_drawingSurface);
  OnToolsReset();
  
  return true;
}

void CMapLinkStudioClientView::drawExtentPolygons(vector<datafileInfo>& extentInfo)
{
  m_dataLayer->destroy();
  m_dataLayer = new TSLStandardDataLayer();
  
  TSLTMC x = 0;
  TSLTMC y = 0;
  
  vector<datafileInfo>::iterator i;
  for (i = extentInfo.begin(); i != extentInfo.end(); i++)
  {
    TSLCoordSet* coords = new TSLCoordSet;
    
    m_drawingSurface->latLongToTMC((*i).tr_lat, (*i).bl_lon, &x, &y, true);
    coords->add(x, y);

    m_drawingSurface->latLongToTMC((*i).tr_lat, (*i).tr_lon, &x, &y, true);
    coords->add(x, y);

    m_drawingSurface->latLongToTMC((*i).bl_lat, (*i).tr_lon, &x, &y, true);
    coords->add(x, y);

    m_drawingSurface->latLongToTMC((*i).bl_lat, (*i).bl_lon, &x, &y, true);
    coords->add(x, y);

    TSLPolygon* polygon = m_dataLayer->entitySet()->createPolygon(0, coords, true);

    if (polygon)
    {
      polygon->setRendering(TSLRenderingAttributeExteriorEdgeStyle, 1);
      polygon->setRendering(TSLRenderingAttributeExteriorEdgeColour, 181);
      polygon->setRendering(TSLRenderingAttributeExteriorEdgeThickness, 1);
    }
  }

  m_drawingSurface->addDataLayer (m_dataLayer, "Data Extent");
  
  OnToolsReset();
}

void CMapLinkStudioClientView::OnImportMap() 
{
  CMapLinkStudioClientDoc* doc = GetDocument();
  string coordSystemName = doc->getCoordSystemName();
  string coordSystemArgument = theApp.getCoordinateSystem();
  
  if (!m_sourceSelectionDialog)
    m_sourceSelectionDialog = new CMISourceSelectionDlg() ;

  if (m_sourceSelectionDialog->DoModal() == IDOK) 
  {
    if (!m_processingControlDialog)
      m_processingControlDialog = new CMIProcessingControlDlg(this);
    
    m_processingControlDialog->StartMaplink();
    
    if (coordSystemArgument.size())      
      m_processingControlDialog->SetCoordSystemName(coordSystemArgument);
    else
      m_processingControlDialog->SetCoordSystemName(coordSystemName);

    m_processingControlDialog->DoModal();
  }

  // coordinate system is only passed if run from the command line

  if (theApp.getCoordinateSystem().size())
    theApp.close();
}

double CMapLinkStudioClientView::getMapTMCperMU()
{
  CMapLinkStudioClientDoc* doc = GetDocument();
  return (doc->getMapTMCperMU());
}
