// importexportView.cpp : implementation of the CImportExportView class
//

#include "stdafx.h"
#include "importexport.h"

#include "importexportDoc.h"
#include "importexportView.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CImportExportView

IMPLEMENT_DYNCREATE(CImportExportView, CView)

  BEGIN_MESSAGE_MAP(CImportExportView, CView)
    //{{AFX_MSG_MAP(CImportExportView)
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
    ON_COMMAND(ID_TOOLS_VIEW_ATTS, OnToolsViewAtts)
    ON_UPDATE_COMMAND_UI(ID_TOOLS_VIEW_ATTS, OnUpdateToolsViewAtts)
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
  // CImportExportView construction/destruction

  CImportExportView::CImportExportView()
  {
    // Clear my internal variables
    m_drawingSurface = 0 ;
    m_currentMode    = 0 ;
    m_modeInterface  = 0 ;
    m_mapLoaded = false ;
    m_viewAtts = 0;
  }

  CImportExportView::~CImportExportView()
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
    if ( m_viewAtts )
      delete m_viewAtts;
  }

  /////////////////////////////////////////////////////////////////////////////
  // CImportExportView drawing

  void CImportExportView::OnDraw(CDC* pDC)
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
      m_modeInterface->drawEchoRectangle( ) ;
    }
  }

  /////////////////////////////////////////////////////////////////////////////
  // CImportExportView printing
  BOOL CImportExportView::OnPreparePrinting(CPrintInfo* pInfo)
  {
    // default preparation
    return DoPreparePrinting(pInfo);
  }

  /////////////////////////////////////////////////////////////////////////////
  // CImportExportView diagnostics

#ifdef _DEBUG
  void CImportExportView::AssertValid() const
  {
    CView::AssertValid();
  }

  void CImportExportView::Dump(CDumpContext& dc) const
  {
    CView::Dump(dc);
  }

  CImportExportDoc* CImportExportView::GetDocument() // non-debug version is inline
  {
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImportExportDoc)));
    return (CImportExportDoc*)m_pDocument;
  }
#endif //_DEBUG

  /////////////////////////////////////////////////////////////////////////////
  // CImportExportView message handlers

  void CImportExportView::OnInitialUpdate()
  {
    CView::OnInitialUpdate();

    CImportExportDoc * doc = GetDocument() ;

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

  void CImportExportView::OnToolsZoomin()
  {
    if ( m_drawingSurface )
    {
      // Zoom in by 30%, and request a window redraw
      m_drawingSurface->zoom( 30, true, false ) ;
      InvalidateRect( 0, false ) ;
    }
  }

  void CImportExportView::OnToolsZoomout()
  {
    if ( m_drawingSurface )
    {
      // Zoom out by 30%, and request a window redraw
      m_drawingSurface->zoom( 30, false, false ) ;
      InvalidateRect( 0, false ) ;
    }
  }

  void CImportExportView::OnToolsReset()
  {
    if ( m_drawingSurface )
    {
      // Reset the window to display the entire map, and request a window redraw.
      m_drawingSurface->reset( false ) ;
      InvalidateRect( 0, false ) ;
    }
  }

  void CImportExportView::OnSize(UINT nType, int cx, int cy)
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

  void CImportExportView::OnMouseMove(UINT nFlags, CPoint point)
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

    if ( m_viewAtts )
    {
      // Find the object under the cursor, and if found, query for the attributes

      // First query an aperture around the cursor, remembering that the y coords for DU are upside down
      TSLTMC  x1, y1, x2, y2;
#define APERTURE 3
      m_drawingSurface->DUToTMC( point.x - APERTURE, point.y + APERTURE, &x1, &y1 );
      m_drawingSurface->DUToTMC( point.x + APERTURE, point.y - APERTURE, &x2, &y2 );

      // Now query the standard datalayer
      TSLMapQuery*  res = m_drawingSurface->query( "std", x1, y1, x2, y2, 999 );

      // Now find the nearest item in the collection returned
      if ( res )
      {
        TSLTMC  x, y;
        m_drawingSurface->DUToTMC( point.x, point.y, &x, &y );
        TSLCoord  p( x, y );

        double  dist = TSL_LONG_MAX;
        TSLEntity*  closest = 0;
        for ( int i = 0; i < res->size(); i++ )
        {
          const TSLEntity*  entity = (*res)[i];

          if ( !i )
          {
            dist = entity->distance( p );
            closest = const_cast<TSLEntity*>(entity);
          }
          else
          {
            double  d = entity->distance( p );
            if ( d < dist )
            {
              dist = d;
              closest = const_cast<TSLEntity*>(entity);
            }
          }
        }
        if ( closest && closest->dataSet() )
          m_viewAtts->receiveEntityDataSet( closest->dataSet() );
      }
    }

    CView::OnMouseMove(nFlags, point);
  }

  void CImportExportView::OnLButtonDown(UINT nFlags, CPoint point)
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

  void CImportExportView::OnMButtonDown(UINT nFlags, CPoint point)
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

  void CImportExportView::OnRButtonDown(UINT nFlags, CPoint point)
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

  void CImportExportView::OnLButtonUp(UINT nFlags, CPoint point)
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

  void CImportExportView::OnMButtonUp(UINT nFlags, CPoint point)
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

  void CImportExportView::OnRButtonUp(UINT nFlags, CPoint point)
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

  void CImportExportView::OnToolsPan()
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

  void CImportExportView::OnToolsZoom()
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

  void CImportExportView::OnToolsZoompan()
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

  void CImportExportView::OnUpdateToolsAction(CCmdUI* pCmdUI)
  {
    // Common update handler to all items that are dependant upon the map
    // being loaded.
    pCmdUI->Enable( m_mapLoaded ? true : false ) ;
  }

  void CImportExportView::OnUpdateToolsMode(CCmdUI* pCmdUI)
  {
    // Common update handler to all items that are dependant upon the map
    // being loaded, and act as radios dependant upon the current mode.
    pCmdUI->Enable( m_mapLoaded ? true : false  ) ;
    pCmdUI->SetCheck( m_currentMode && m_currentMode->id() == pCmdUI->m_nID ) ;
  }

  void CImportExportView::OnEditCopy()
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

  void CImportExportView::OnLButtonDblClk(UINT nFlags, CPoint point)
  {
    // Just handle like a normal press to avoid losing the second press
    OnLButtonDown( nFlags, point ) ;
    CView::OnLButtonDblClk(nFlags, point);
  }

  void CImportExportView::OnRButtonDblClk(UINT nFlags, CPoint point)
  {
    // Just handle like a normal press to avoid losing the second press
    OnRButtonDown( nFlags, point ) ;
    CView::OnRButtonDblClk(nFlags, point);
  }
  void CImportExportView::OnMButtonDblClk(UINT nFlags, CPoint point)
  {
    // Just handle like a normal press to avoid losing the second press
    OnMButtonDown( nFlags, point ) ;
    CView::OnMButtonDblClk(nFlags, point);
  }

  void CImportExportView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
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
      TSLDrawingSurface::findFile( templateFile, templateFilePath );

      // load the template
      if ( templateFilePath )
      {
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

  BOOL CImportExportView::OnEraseBkgnd(CDC* pDC)
  {
    // Since the drawing surface is double buffered, we don't need to erase the
    // background.
    return true;
  }

  void CImportExportView::OnToolsGrab()
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

  void CImportExportView::OnToolsViewAtts() 
  {
    if ( m_viewAtts )
    {
      delete m_viewAtts;
      m_viewAtts = 0;
      return;
    }

    m_viewAtts = new CViewAttsDialog;
    m_viewAtts->Create( CViewAttsDialog::IDD, this );
  }

  void CImportExportView::OnUpdateToolsViewAtts(CCmdUI* pCmdUI) 
  {
    pCmdUI->Enable( m_mapLoaded ? true : false  ) ;
    pCmdUI->SetCheck( m_viewAtts ? true : false ) ;
  }

  void CImportExportView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
  {
    if ( m_drawingSurface )
      m_drawingSurface->reset( false );
    InvalidateRect( 0 );
  }
