#include "pch.h"
#include "framework.h"

#include "SIDC Splat.h"

#include "SIDC SplatDoc.h"
#include "SIDC SplatView.h"

#include <cassert>



// CSidcSplatView

IMPLEMENT_DYNCREATE(CSidcSplatView, CView)

BEGIN_MESSAGE_MAP(CSidcSplatView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


CSidcSplatView::CSidcSplatView() noexcept = default;
CSidcSplatView::~CSidcSplatView() = default;

BOOL CSidcSplatView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CSidcSplatView drawing

void CSidcSplatView::OnDraw(CDC* pDC)
{
	CSidcSplatDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc || !m_drawingSurface)
		return;

	RECT r;
	if (pDC->GetClipBox(&r) == NULLREGION)
		GetClientRect(&r);
	m_drawingSurface->drawDU(r.left, r.bottom, r.right, r.top, true);
}

void CSidcSplatView::OnRButtonUp(UINT /* nFlags */, CPoint pt)
{
	TSLCoord pos;
	m_drawingSurface->DUToTMC(pt.x, pt.y, &pos.m_x, &pos.m_y);
	GetDocument()->addSymbol(pos);
	InvalidateRect(nullptr, false);
}

void CSidcSplatView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
}


// CSidcSplatView message handlers

void CSidcSplatView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	if (m_drawingSurface)
		return;
	
	m_drawingSurface = std::make_unique<TSLNTSurface>(m_hWnd, false);
	m_drawingSurface->setOption(TSLOptionDoubleBuffered, true);
	RECT cr;
	GetClientRect(&cr);
	m_drawingSurface->wndResize(cr.left, cr.top, cr.right, cr.bottom, false);

	if (GetDocument()->addToSurface(m_drawingSurface.get()))
		m_drawingSurface->reset(false);

	int styles_n, * styles_p, ixs_n, *ixs_p;
	m_drawingSurface->getSymbolStyleIndices(&styles_p, &styles_n);
	m_drawingSurface->getSymbolIndices(&ixs_p, &ixs_n);
	//TSLDrawingSurface::getSymbolListStatic
}

void CSidcSplatView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (!m_drawingSurface)
		return;
	m_drawingSurface->wndResize(0, 0, cx, cy, false, TSLResizeActionMaintainCentre);
}

BOOL CSidcSplatView::OnEraseBkgnd(CDC* pDC)
{
	return true; // Inhibit CView::OnEraseBkgnd(pDC)
}

BOOL CSidcSplatView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	assert(m_drawingSurface);
	ScreenToClient(&pt);
	double uux, uuy;
	m_drawingSurface->DUToUU(pt.x, pt.y, &uux, &uuy);
	m_drawingSurface->zoom(20.0, zDelta > 0, false);
	m_drawingSurface->pan(uux, uuy, pt.x, pt.y, false);

	InvalidateRect(nullptr, false);

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CSidcSplatView::OnLButtonUp(UINT nFlags, CPoint pt)
{
	TSLCoord pos;
	m_drawingSurface->DUToTMC(pt.x, pt.y, &pos.m_x, &pos.m_y);
	GetDocument()->extendPolyline(pos);
	InvalidateRect(nullptr, false);

	CView::OnLButtonUp(nFlags, pt);
}
