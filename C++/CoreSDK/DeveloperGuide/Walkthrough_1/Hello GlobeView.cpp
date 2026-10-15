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
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHelloGlobeView construction/destruction

CHelloGlobeView::CHelloGlobeView()
{
  m_drawingSurface = 0 ;
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
