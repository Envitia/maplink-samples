// App6aViewerView.cpp : implementation of the CApp6aViewerView class
//

#include "stdafx.h"
#include "App6aViewer.h"

#include "TSLApp6aSymbolHelper.h"
#include "App6aViewerDoc.h"
#include "App6aViewerView.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern char * display_str ;
CApp6aViewerView * g_mainView = NULL ;

/////////////////////////////////////////////////////////////////////////////
// CApp6aViewerView

IMPLEMENT_DYNCREATE(CApp6aViewerView, CView)

  BEGIN_MESSAGE_MAP(CApp6aViewerView, CView)
    //{{AFX_MSG_MAP(CApp6aViewerView)
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
    ON_COMMAND(ID_BUTTONUP, OnButtonUp)
    ON_COMMAND(ID_BUTTONDOWN, OnButtonDown)
    ON_COMMAND(ID_TOOLS_ZOOM, OnToolsZoom)
    ON_COMMAND(ID_TOOLS_ZOOMPAN, OnToolsZoompan)
    ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
    ON_WM_LBUTTONDBLCLK()
    ON_WM_RBUTTONDBLCLK()
    ON_WM_ERASEBKGND()
    ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOMIN,  OnUpdateToolsAction)
    ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOMPAN, OnUpdateToolsMode)
    ON_COMMAND(ID_TOOLS_GRAB, OnToolsGrab)
    ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
    ON_COMMAND(ID_BUTTONREFRESH, OnRefresh)
    //}}AFX_MSG_MAP

    // Our common handles can update multiple items.
    ON_UPDATE_COMMAND_UI(ID_EDIT_COPY,     OnUpdateToolsAction)
    ON_UPDATE_COMMAND_UI(ID_FILE_PRINT,    OnUpdateToolsAction)
    ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOMOUT, OnUpdateToolsAction)
    ON_UPDATE_COMMAND_UI(ID_TOOLS_PAN,     OnUpdateToolsMode)
    ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOM,    OnUpdateToolsMode)
    ON_UPDATE_COMMAND_UI(ID_TOOLS_GRAB,    OnUpdateToolsMode)
    ON_UPDATE_COMMAND_UI(ID_BUTTONUP,      OnUpdateToolsMode)
    ON_UPDATE_COMMAND_UI(ID_BUTTONDOWN,    OnUpdateToolsMode)

    // Add the middle button handlers since DevStudio doesn't 
    // give us the option.
    ON_WM_MBUTTONDOWN()
    ON_WM_MBUTTONDBLCLK()
    ON_WM_MBUTTONUP()

  END_MESSAGE_MAP()

  /////////////////////////////////////////////////////////////////////////////
  // CApp6aViewerView construction/destruction

  CApp6aViewerView::CApp6aViewerView()
  {
    // Clear my internal variables
    m_drawingSurface = 0 ; 
    m_currentMode    = 0 ;
    m_modeInterface  = 0 ;
    m_mapLoaded = false ;
    g_mainView = this ;
    m_stdDataLayer = 0 ;

  }


  CApp6aViewerView::~CApp6aViewerView()
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
  }

  /////////////////////////////////////////////////////////////////////////////
  // CApp6aViewerView drawing

  void CApp6aViewerView::OnDraw(CDC* pDC)
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
    // pDC->TextOut(20,20,display_str) ;
  }

  /////////////////////////////////////////////////////////////////////////////
  // CApp6aViewerView printing
  BOOL CApp6aViewerView::OnPreparePrinting(CPrintInfo* pInfo)
  {
    // default preparation
    return DoPreparePrinting(pInfo);
  }

  /////////////////////////////////////////////////////////////////////////////
  // CApp6aViewerView diagnostics

#ifdef _DEBUG
  void CApp6aViewerView::AssertValid() const
  {
    CView::AssertValid();
  }

  void CApp6aViewerView::Dump(CDumpContext& dc) const
  {
    CView::Dump(dc);
  }

  CApp6aViewerDoc* CApp6aViewerView::GetDocument() // non-debug version is inline
  {
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CApp6aViewerDoc)));
    return (CApp6aViewerDoc*)m_pDocument;
  }
#endif //_DEBUG

  /////////////////////////////////////////////////////////////////////////////
  // CApp6aViewerView message handlers

  void CApp6aViewerView::OnInitialUpdate() 
  {
    CView::OnInitialUpdate();
    checked_count = 0;

    char DIRECTORY[ 200 ] = "" ;
    sprintf( DIRECTORY, "%s\\config", TSLUtilityFunctions::getMapLinkHome() ) ;
    char filename [1000] ;
    sprintf (filename, "%s\\%s", DIRECTORY,APP6A_CONFIG_FILE_NAME ) ;

    //GetDocument()->symbolHelper->Load(filename);



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
    m_mapLoaded = 1 ;


    if ( m_currentMode )
    {
      delete m_currentMode ;
      m_currentMode = 0 ;
    }


#ifdef XXXXX

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

#endif

  }

  void CApp6aViewerView::OnToolsZoomin() 
  {
    if ( m_drawingSurface )
    {
      // Zoom in by 30%, and request a window redraw
      m_drawingSurface->zoom( 30, true, false ) ;
      InvalidateRect( 0, false ) ;
    }
  }

  void CApp6aViewerView::OnToolsZoomout() 
  {
    if ( m_drawingSurface )
    {
      // Zoom out by 30%, and request a window redraw
      m_drawingSurface->zoom( 30, false, false ) ;
      InvalidateRect( 0, false ) ;
    }
  }

  void CApp6aViewerView::OnToolsReset() 
  {
    if ( m_drawingSurface )
    {
      // Reset the window to display the entire map, and request a window redraw.
      m_drawingSurface->reset( false ) ;
      m_drawingSurface->zoom( 9000, true, false ) ;

      InvalidateRect( 0, false ) ;
    }
  }

  void CApp6aViewerView::OnSize(UINT nType, int cx, int cy) 
  {
    CView::OnSize(nType, cx, cy);




    if ( m_drawingSurface )
    {

      // Inform the drawing surface of the new window size,
      // attempting to keep the top left corner the same.
      // Do not ask for an automatic redraw since we will get an OnDraw call

      m_drawingSurface->wndResize( 0, 0, cx, cy, false, 
        TSLResizeActionMaintainTopLeft ) ;

      drawSymbols();


    }
  }

  void CApp6aViewerView::OnMouseMove(UINT nFlags, CPoint point) 
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

  void CApp6aViewerView::OnLButtonDown(UINT nFlags, CPoint point) 
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

  void CApp6aViewerView::OnMButtonDown(UINT nFlags, CPoint point) 
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

  void CApp6aViewerView::OnRButtonDown(UINT nFlags, CPoint point) 
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

  void CApp6aViewerView::OnLButtonUp(UINT nFlags, CPoint point) 
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

  void CApp6aViewerView::OnMButtonUp(UINT nFlags, CPoint point) 
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

  void CApp6aViewerView::OnRButtonUp(UINT nFlags, CPoint point) 
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

  void CApp6aViewerView::OnToolsPan() 
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

  void CApp6aViewerView::OnToolsZoom() 
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

  void CApp6aViewerView::OnToolsZoompan() 
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

  void CApp6aViewerView::OnUpdateToolsAction(CCmdUI* pCmdUI) 
  {
    // Common update handler to all items that are dependant upon the map
    // being loaded.
    pCmdUI->Enable( m_mapLoaded ? true : false ) ;
  }

  void CApp6aViewerView::OnUpdateToolsMode(CCmdUI* pCmdUI) 
  {
    // Common update handler to all items that are dependant upon the map
    // being loaded, and act as radios dependant upon the current mode.
    pCmdUI->Enable( m_mapLoaded ? true : false  ) ;
    pCmdUI->SetCheck( m_currentMode && m_currentMode->id() == pCmdUI->m_nID ) ;
  }

  void CApp6aViewerView::OnEditCopy() 
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

  void CApp6aViewerView::OnLButtonDblClk(UINT nFlags, CPoint point) 
  {
    // Just handle like a normal press to avoid losing the second press
    OnLButtonDown( nFlags, point ) ;
    CView::OnLButtonDblClk(nFlags, point);
  }

  void CApp6aViewerView::OnRButtonDblClk(UINT nFlags, CPoint point) 
  {
    // Just handle like a normal press to avoid losing the second press
    OnRButtonDown( nFlags, point ) ;
    CView::OnRButtonDblClk(nFlags, point);
  }
  void CApp6aViewerView::OnMButtonDblClk(UINT nFlags, CPoint point) 
  {
    // Just handle like a normal press to avoid losing the second press
    OnMButtonDown( nFlags, point ) ;
    CView::OnMButtonDblClk(nFlags, point);
  }

  void CApp6aViewerView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
  {
    // Attempt to print using the PrintHelper, using the standard templates.
    if ( m_drawingSurface ) 
    {
      TSLEntitySet * tmp = 0 ;
      TSLSimpleString templateFilePath;
      char* templateFile = NULL;

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
      // Note : Use the extent captured in the OnFilePrint, not the current extent.  This is
      //        because the extent may have changed to to MFC Print Preview taking control
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
      //    tmp->destroy() ;

      pInfo->m_bContinuePrinting = false ;
    }
  }

  BOOL CApp6aViewerView::OnEraseBkgnd(CDC* pDC) 
  {
    // Since the drawing surface is double buffered, we don't need to erase the
    // background.
    return true;
  }

  void CApp6aViewerView::OnToolsGrab() 
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


  void CApp6aViewerView::OnFilePrint() 
  {
    // We should not query the extent to print during the OnPrint, since the window may change
    // size, especially in MFC print-preview
    if ( m_drawingSurface )
      m_drawingSurface->getTMCExtent( m_extentToPrint ) ;
    CView::OnFilePrint() ;
  }

  void CApp6aViewerView::OnFilePrintPreview() 
  {
    // We should not query the extent to print during the OnPrint, since the window may change
    // size, especially in MFC print-preview
    if ( m_drawingSurface )
      m_drawingSurface->getTMCExtent( m_extentToPrint ) ;
    CView::OnFilePrintPreview() ;
  }


  void CApp6aViewerView::OnButtonUp()
  {

    checked_count -= 1;
    if (checked_count < 0)
    {
      checked_count = 0;
    }
    // m_drawingSurface->removeDataLayer("Layer");
    drawSymbols();

  }

  void CApp6aViewerView::OnButtonDown()
  {

    checked_count += 1;
    //m_drawingSurface->removeDataLayer("Layer");
    drawSymbols();
  }

  void CApp6aViewerView::drawSymbols()
  {

    if (!m_drawingSurface)
      return ;
    CApp6aViewerDoc * doc = (CApp6aViewerDoc*) GetDocument() ;

    // Create a map data layer
    // -----------------------

    if (!m_stdDataLayer)
    {
      m_stdDataLayer = new TSLStandardDataLayer();
      m_drawingSurface->addDataLayer( m_stdDataLayer, "Layer" ) ;
    }
    //  g_stdDataLayer = m_stdDataLayer ;

    m_stdDataLayer->removeData() ;
    TSLEntitySet * entitySet = m_stdDataLayer->entitySet() ;

    // Clear the error stack, load the map then check for errors.

    int count   = 100000;
    int c_count = 100000;
    int o_count = 0;
    int num_symbols = doc->m_symbolHelper->Size();

    // For all symbols loaded
    // ----------------------

    // First item in tree is the root. Ignore it.
    HTREEITEM this_item = GetDocument()->m_treeView->GetFirstItem(); // Root item

    for (int k=0; k < num_symbols; k++)
    {

      char *name = "";
      doc->m_symbolHelper->GetName(k,&name);


      char* desc_id   ; 
      char* desc_name ; 

      doc->m_symbolHelper->GetId(k,&desc_id);
      doc->m_symbolHelper->GetName(k,&desc_name);

      this_item = GetDocument()->m_treeView->GetNextItem(this_item);     

      if (GetDocument()->m_treeView->IsItemChecked(this_item))
      {
        o_count++;
      }

      if (GetDocument()->m_treeView->IsItemChecked(this_item)&&(o_count > checked_count*14))
      {

        // Draw Symbol Label
        // -----------------        

        TSLText   * label    = entitySet->createText(0,13000,((100000 - c_count + 100000) * 1500)+300, desc_name, 500) ;
        TSLText   * label2   = entitySet->createText(0,13000,((100000 - c_count + 100000) * 1500)-300, desc_id/*finalnum*/, 500) ;

        label->setRendering(TSLRenderingAttributeTextColour,1) ;
        label->setRendering(TSLRenderingAttributeTextFont,1) ;
        label->setRendering(TSLRenderingAttributeTextMinPixelHeight , 5);
        label->setRendering(TSLRenderingAttributeTextMaxPixelHeight,  15);
        label->setRendering(TSLRenderingAttributeTextHorizontalAlignment,0); 
        label2->setRendering(TSLRenderingAttributeTextColour,5) ;
        label2->setRendering(TSLRenderingAttributeTextFont,1) ;
        label2->setRendering(TSLRenderingAttributeTextMinPixelHeight , 2);
        label2->setRendering(TSLRenderingAttributeTextMaxPixelHeight,  15);
        label2->setRendering(TSLRenderingAttributeTextHorizontalAlignment,0); 


        TSLRectangle *rect1 = entitySet->createRectangle(0, TSLCoord(12750,((100000 - c_count + 100000) * 1500) + 750), TSLCoord(25000,((100000 - c_count + 100000) * 1500) - 750), 0.0);
        rect1->setRendering(TSLRenderingAttributeExteriorEdgeThickness, 1);
        rect1->setRendering(TSLRenderingAttributeExteriorEdgeStyle, 1);
        rect1->setRendering(TSLRenderingAttributeExteriorEdgeColour, 1);

        for (int j = 0; j <= 8; j++)
        { 

          TSLEntitySet *es =  const_cast<TSLEntitySet *>(GetDocument()->m_symbolHelper->CreateSymbol(k, j * 1500, (100000 - c_count + 100000) * 1500, 1000));


          if (es && GetDocument()->m_symbolHelper->IsApp6a(es))
          {
            TSLEntitySet *es2 = const_cast<TSLEntitySet *>(GetDocument()->m_symbolHelper->SetHostility(es,PENDING + j));


            TSLEntitySet * layer_set = m_stdDataLayer->entitySet() ;

            int layers = es2->size();

            for (int i=layers; i>=0; i--)
            {
              layer_set->insert((*es2)[i],i); 
            }
          }


          TSLRectangle *rect2 = entitySet->createRectangle(0,  TSLCoord((j * 1500) - 750,((100000 - c_count + 100000) * 1500) + 750), TSLCoord((j * 1500) + 750,((100000 - c_count + 100000) * 1500) - 750), 0.0);

          rect2->setRendering(TSLRenderingAttributeExteriorEdgeThickness, 1);
          rect2->setRendering(TSLRenderingAttributeExteriorEdgeStyle, 1);
          rect2->setRendering(TSLRenderingAttributeExteriorEdgeColour, 1);

        }  

        c_count++;
        if (c_count > 100000 + 14)
          break;
      }

      count++;
      if (count > num_symbols + 100000)

        break;

    }

    m_drawingSurface->reset( false ) ;
    m_stdDataLayer->notifyChanged() ; 

    Invalidate() ;

  }

  void CApp6aViewerView::OnRefresh()
  {
    checked_count = 0;  
    drawSymbols();

  }

