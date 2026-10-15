// Hello GlobeView.cpp : implementation of the CHelloGlobeView class
//

#include "stdafx.h"
#include "Hello Globe.h"

#include "Hello GlobeDoc.h"
#include "Hello GlobeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHelloGlobeView

IMPLEMENT_DYNCREATE(CHelloGlobeView, CView)

  BEGIN_MESSAGE_MAP(CHelloGlobeView, CView)
    //{{AFX_MSG_MAP(CHelloGlobeView)
    ON_WM_SIZE()
    ON_WM_ERASEBKGND()
    ON_WM_LBUTTONUP()
    ON_WM_RBUTTONUP()
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEMOVE()
    ON_WM_RBUTTONDOWN()
    ON_WM_MOUSEWHEEL()
    //}}AFX_MSG_MAP
    // Standard printing commands
    ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
    ON_WM_MBUTTONUP()
  END_MESSAGE_MAP()

  /////////////////////////////////////////////////////////////////////////////
  // CHelloGlobeView construction/destruction

  CHelloGlobeView::CHelloGlobeView()
    : m_drawingSurface(0), m_grabbed(false), m_checkForGrab(false)
  {
  }

  BOOL CHelloGlobeView::PreCreateWindow(CREATESTRUCT& cs)
  {
    return CView::PreCreateWindow(cs);
  }

  /////////////////////////////////////////////////////////////////////////////
  // CHelloGlobeView drawing

  void CHelloGlobeView::OnDraw(CDC* pDC)
  {
    if ( m_drawingSurface )
    {
      RECT rect ;

      if ( pDC->GetClipBox( &rect ) == NULLREGION )
        GetClientRect( &rect ) ;

      m_drawingSurface->drawDU( rect.left, rect.bottom, rect.right, rect.top, true ) ;
    }
  }

  /////////////////////////////////////////////////////////////////////////////
  // CHelloGlobeView printing

  BOOL CHelloGlobeView::OnPreparePrinting(CPrintInfo* pInfo)
  {
    // default preparation
    return DoPreparePrinting(pInfo);
  }

  void CHelloGlobeView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
  {
  }

  void CHelloGlobeView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
  {
  }

  /////////////////////////////////////////////////////////////////////////////
  // CHelloGlobeView diagnostics

#ifdef _DEBUG
  void CHelloGlobeView::AssertValid() const
  {
    CView::AssertValid();
  }

  void CHelloGlobeView::Dump(CDumpContext& dc) const
  {
    CView::Dump(dc);
  }

  CHelloGlobeDoc* CHelloGlobeView::GetDocument() // non-debug version is inline
  {
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHelloGlobeDoc)));
    return (CHelloGlobeDoc*)m_pDocument;
  }
#endif //_DEBUG

  /////////////////////////////////////////////////////////////////////////////
  // CHelloGlobeView message handlers

  void CHelloGlobeView::OnInitialUpdate() 
  {
    CView::OnInitialUpdate();

    if ( !m_drawingSurface )
    {
      m_drawingSurface = new TSLNTSurface( m_hWnd, false ) ;
      m_drawingSurface->setOption( TSLOptionDoubleBuffered, true ) ;

      RECT cr ;
      GetClientRect( &cr ) ;
      m_drawingSurface->wndResize( cr.left, cr.top, cr.right, cr.bottom, false ) ;
    }

    if ( GetDocument()->addToSurface( m_drawingSurface ) )
      m_drawingSurface->reset( false ) ;
  }

  CHelloGlobeView::~CHelloGlobeView()
  {
    if ( m_drawingSurface )
    {
      delete m_drawingSurface ;
      m_drawingSurface = 0 ;
    }
  }


  void CHelloGlobeView::OnSize(UINT nType, int cx, int cy) 
  {
    CView::OnSize(nType, cx, cy);

    if ( m_drawingSurface )
    {
      m_drawingSurface->wndResize( 0, 0, cx, cy, false, 
        TSLResizeActionMaintainTopLeft );
    }	
  }

  BOOL CHelloGlobeView::OnEraseBkgnd(CDC* pDC) 
  {
    return TRUE ;
  }

  void CHelloGlobeView::OnLButtonUp(UINT nFlags, CPoint point) 
  {
    if ( m_drawingSurface )
    {
      if ( nFlags & MK_CONTROL )
      {
        long x, y ;
        m_drawingSurface->DUToTMC( point.x, point.y, &x, &y ) ;

        if ( GetDocument()->createOverlay( x, y, m_drawingSurface  ) )
          InvalidateRect( 0, FALSE ) ;
      }
      else if ( abs( point.x - m_lmb.x ) <= 3 && abs( point.y - m_lmb.y ) <= 3 )
      { 
        // Pan to point and zoom
        double x, y ;
        if ( m_drawingSurface->DUToUU( point.x, point.y, &x, &y ) )
        {
          if ( m_drawingSurface->pan( x, y, false ) )
          {
            if ( m_drawingSurface->zoom( 25, true, false ) )
              InvalidateRect( 0, FALSE ) ;
          }
        }
      }
      else
      {
        // Zoom to rectangle
        double x1, y1, x2, y2 ;
        if (   m_drawingSurface->DUToUU( point.x, point.y, &x1, &y1 )
          && m_drawingSurface->DUToUU( m_lmb.x, m_lmb.y, &x2, &y2 ) )
        {
          if ( m_drawingSurface->resize( x1, y1, x2, y2, false, true ) )
            InvalidateRect( 0, FALSE ) ;
        }
      }
    }
    CView::OnLButtonUp(nFlags, point);
  }

  void CHelloGlobeView::OnRButtonUp(UINT nFlags, CPoint point) 
  {
    if ( m_drawingSurface && !m_grabbed )
    {
      if ( m_drawingSurface->zoom( 25, false, false ) )
        InvalidateRect( 0, FALSE ) ;
    }
    m_grabbed = m_checkForGrab = false ;
    CView::OnRButtonUp(nFlags, point);
  }

  void CHelloGlobeView::OnLButtonDown(UINT nFlags, CPoint point) 
  {
    m_lmb = point ;
    CView::OnLButtonDown(nFlags, point);
  }

  void CHelloGlobeView::OnMouseMove(UINT nFlags, CPoint point) 
  {
    bool moved = false ;

    if (   m_checkForGrab 
      && ( m_grabbed || abs( point.x - m_rmb.x ) > 3 || abs( point.y - m_rmb.y ) > 3 ) )
    {

      // Calculate offset between the last point and the new point
      long du_offset_x = m_lastGrabPoint.x - point.x ;
      long du_offset_y = point.y - m_lastGrabPoint.y ;

      // Indicate a grab and remember the last grabbed point
      m_lastGrabPoint = point ;
      m_grabbed = true ;

      // Convert the offset into user units
      // No DUPerUU() function so we will take the long way round.
      double tmcPerDUx, tmcPerDUy;
      m_drawingSurface->TMCperDU(tmcPerDUx, tmcPerDUy) ;
      double uu_per_du = tmcPerDUx / m_drawingSurface->TMCperUU() ;

      double uu_offset_x = du_offset_x * uu_per_du ;
      double uu_offset_y = du_offset_y * uu_per_du ;

      // Get the current centre point of the drawing surface
      double x, y, x1, y1, x2, y2 ;

      m_drawingSurface->getUUExtent( &x1, &y1, &x2, &y2 ) ;
      x = ( x1 + x2 ) / 2 ;
      y = ( y1 + y2 ) / 2 ;

      x += uu_offset_x ;
      y += uu_offset_y ;

      // Pan and redraw the drawing surface
      // Request redraw only if pan is successful
      if ( m_drawingSurface->pan( x, y, false ) )
      {
        moved = true ;
        InvalidateRect( 0, FALSE ) ;
      }
    }

    if ( m_drawingSurface && !moved )
    {
      CHelloGlobeDoc * doc = GetDocument() ;

      long x, y ;
      if ( m_drawingSurface->DUToTMC( point.x, point.y, &x, &y ) )
        doc->updateDDOPosition( x, y ) ;
    }

    CView::OnMouseMove(nFlags, point);
  }

  void CHelloGlobeView::OnRButtonDown(UINT nFlags, CPoint point) 
  {
    m_rmb = m_lastGrabPoint = point ;
    m_grabbed = false ;
    m_checkForGrab = true ;
    CView::OnRButtonDown(nFlags, point);
  }

  BOOL CHelloGlobeView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
  {
    ScreenToClient(&pt);

    // Zoom in or out depending upon the wheel direction
    if ( m_drawingSurface && m_drawingSurface->zoom( 30.0, zDelta > 0, false ) )
      InvalidateRect( NULL, FALSE );	

    return CView::OnMouseWheel(nFlags, zDelta, pt);
  }

  void CHelloGlobeView::OnMButtonUp(UINT nFlags, CPoint point) 
  {
    if ( m_drawingSurface )
    {
      double x, y ;
      if ( m_drawingSurface->DUToUU( point.x, point.y, &x, &y ) )
      {
        if ( m_drawingSurface->pan( x, y, false ) )
          InvalidateRect( 0, FALSE ) ;
      }
    }
    CView::OnMButtonUp(nFlags, point);
  }
