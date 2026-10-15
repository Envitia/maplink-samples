// LOSView.cpp : implementation file
//

#include "stdafx.h"
#include "terraindemo.h"
#include "LOSView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLOSView

IMPLEMENT_DYNCREATE(CLOSView, CScrollView)

CLOSView::CLOSView()
{
}

CLOSView::~CLOSView()
{
}


BEGIN_MESSAGE_MAP(CLOSView, CScrollView)
	//{{AFX_MSG_MAP(CLOSView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLOSView drawing

void CLOSView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 800;
	SetScrollSizes(MM_TEXT, sizeTotal);
}









void CLOSView::OnDraw(CDC* pDC)
{
	CTerrainDemoDoc* pDoc = (CTerrainDemoDoc *)GetDocument();
  return;


  RECT clientArea;
  GetClientRect(&clientArea);
  int maxWidth = (clientArea.right<clientArea.bottom)?clientArea.right:clientArea.bottom;
  pDoc->m_dataSet.DrawHeightToHDC(pDC->GetSafeHdc(), clientArea.left, clientArea.top, maxWidth, maxWidth, 1);
  pDoc->m_dataSet.DrawLosToHDC(pDC->GetSafeHdc(), clientArea.left, clientArea.top, maxWidth, maxWidth, 1, 0, 0, 0);

/*
  int numColumns = m_dataSet.GetNumberColumns();
  int numRows = m_dataSet.GetNumberRows();
  for (int colInd = 0;colInd<m_num_columns;colInd++)
  {
    int x = colInd*m_cellsize;
    for (int rowInd = 0;rowInd<m_num_rows;rowInd++)
    {
      int y = (m_num_rows-rowInd)*m_cellsize;
      int height = m_values[ARRAY_INDEX(rowInd, colInd)];
      COLORREF col = m_cols[height/10];
      CRect r(x, y, x+m_cellsize, y+m_cellsize);
      pDC->FillSolidRect(r, col);

      CRect r2(r);
      int size = m_cellsize/4.0;
      r2.DeflateRect(size,size,size,size);
      if (!m_los[ARRAY_INDEX(rowInd,colInd)])
        pDC->FillSolidRect(r2, COLORREF(RGB(255, 0, 0)));
      
    
    }
  }
  */
}

/////////////////////////////////////////////////////////////////////////////
// CLOSView diagnostics

#ifdef _DEBUG
void CLOSView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CLOSView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLOSView message handlers
