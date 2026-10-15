// TerrainView.cpp : implementation file
//

#include "stdafx.h"
#include "TerrainView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CTerrainView

IMPLEMENT_DYNCREATE(CTerrainView, CView)

  CTerrainView::CTerrainView()
  : m_num_values(0)
  , m_range(0)
{
}

void CTerrainView::SetData(int num_values, double * values, double range) 
{
  m_num_values = num_values ;
  assert(m_num_values <= _countof(m_values));

  for (int i = 0 ; i < m_num_values; i++)
  {
    m_values [i] = values[i] ;
  }
  Invalidate(FALSE);

  m_range = range;
}

CTerrainView::~CTerrainView()
{
}



BEGIN_MESSAGE_MAP(CTerrainView, CView)
  //{{AFX_MSG_MAP(CTerrainView)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTerrainView drawing

void CTerrainView::OnDraw(CDC* pDC)
{
  POINT points [1000];

  CRect rect;
  GetClientRect( &rect );

  CBrush white_brush(RGB(255 , 255  ,255)) ;
  HGDIOBJ oldBrush = pDC->SelectObject(white_brush) ;
  pDC->FillRect(rect,NULL) ;
  pDC->SelectObject(oldBrush);

  if (m_num_values < 2)
    return ;


  int baseLine = rect.bottom - 30 ;
  int topLine = rect.top + 30 ;
  int leftLine = rect.left + 30  ;
  int rightLine = rect.right - 30 ;

  double middleLine = ((baseLine - topLine)/2.0) + topLine  ;

  double screenWidth = static_cast <double> (rightLine - leftLine);
  double screenHeight  = static_cast <double> (baseLine - topLine) ;

  TerrainViewerOptions& options = CMainFrame::instance()->options();

  // If we are displaying depth data then use half for that.
  if ( options.m_bathy )
  {
    screenHeight = screenHeight / 2.0 ;
    middleLine = (baseLine / 2.0) + topLine ;
  } 
  else
  {
    middleLine = baseLine ;
  }

  //-------------------------------------------------------------------
  // TERRAIN 
  //-------------------------------------------------------------------

  // Calculate Horizontal Spacing
  // ----------------------------
  // This is the width of the screen divided by the number of points to display.
  double hspacing = screenWidth / (double)(m_num_values - 1) ;

  // Calculate Vertical Spacing
  // --------------------------
  // This is the height of the screen, divide by two (because we need to display
  // depth data) divided the highest point we want to display.
  double vspacing ;
  double maxval = options.m_manual_scale ;
  if (options.m_terrainAutoscale)
  {
    maxval = 0.0 ;

    // Get the highest point from the data.
    for (int i = 0; i < m_num_values; i++)
      maxval = (maxval > m_values[i]) ? maxval : m_values[i] ;

    // Calculate the vertical spacing. 
    if (maxval > 0 )
      vspacing = screenHeight / ( maxval * options.m_terr_scale ) ;
    else
      vspacing = 0 ; 
  }
  else
  {
    // Get the highest point from the config.
    vspacing = screenHeight / options.m_manual_scale ; 
  }


  // Draw it 
  // -------

  // Start from..
  points[0].x = leftLine  ; 
  points[0].y = middleLine  ;

  int j = 0;
  for (; j < m_num_values; ++j)
  {
    points[j+1].x = (int)((j * hspacing) + leftLine) ;

    int val = (int)(m_values[j] * vspacing) ;

    // Terrain must be greater than 0.
    val = (val > 0) ? val : 0 ;

    // Multiply by the terrain scale.
    val = val * options.m_terr_scale;

    // Save the y point.
    points[j+1].y = middleLine - val ;
  }

  // End at..
  points[m_num_values+1].x = rightLine ;
  points[m_num_values+1].y = middleLine ;

  int npts = m_num_values + 2  ;

  CBrush green_brush (RGB(0 , 200,  0)) ;
  oldBrush = pDC->SelectObject(green_brush) ;

  // At finally draw to the device context.
  pDC->Polygon(points, npts) ;

  pDC->SelectObject(oldBrush);

  // ------------------------------------------------
  // Now Draw the Sea ( If the Bathymetry is true )
  // ------------------------------------------------
  double minval = 0.0 - options.m_manual_scale  ;


  if ( options.m_bathy )
  {

    // Select the new brush.
    CBrush blue_brush  (RGB(0, 0, 200)) ;

    // Calculate Vertical Spacing
    if (options.m_terrainAutoscale)
    {
      minval = 0.0 ;

      for (int i = 0; i < m_num_values; i++)
        minval = (minval < m_values[i]) ? minval : m_values[i] ;

      if ( minval < 0 )
        vspacing = screenHeight / ( -minval * options.m_bathy_scale ) ;
      else
        vspacing = 0;

    }
    else
    {
      vspacing = screenHeight / options.m_manual_scale ; 
    }

    // Start From.. 
    points[0].x = leftLine ;
    points[0].y = middleLine ;

    // **********
    for (j = 0; j < m_num_values; j++)
    {
      points[j+1].x = (int)((j * hspacing) + leftLine ) ;

      int val = (int)(m_values[j] * vspacing) ;

      val = (val < 0) ? val : 0 ;

      // Multiply by the bathymetry scale.
      val = val * options.m_bathy_scale ;

      points[j+1].y = middleLine - val ;

    }

    // End at ...
    points[m_num_values+1].x = rightLine ;
    points[m_num_values+1].y = middleLine ;

    npts = m_num_values + 2  ;

    oldBrush = pDC->SelectObject(blue_brush) ;
    pDC->Polygon(points, npts) ;

    pDC->SelectObject(oldBrush);

  } // End Bathymetry

  // If the user has switched on scale factors in the config panel, the 
  // heights of all points will be X times what they really are. So, we
  // must DIVIDE the scale values by X to get a realistic looking scale.
  if ( ! options.m_terrainAutoscale )
  {
    maxval = maxval / options.m_terr_scale ;
    minval = minval / options.m_bathy_scale ;
  }

  CString range;
  if ( m_range < 1000 )
    range.Format( _T("%.3fm"), m_range );
  else
    range.Format( _T("%.3fkm"), m_range / 1000.0 );

  pDC->MoveTo( leftLine, middleLine );
  pDC->LineTo( rightLine, middleLine );
  pDC->TextOut ( rightLine - 70, middleLine - 25, range ) ;

  pDC->MoveTo ( leftLine, middleLine ) ;
  pDC->LineTo ( leftLine, topLine ) ;
  pDC->TextOut ( leftLine - 25, middleLine, "0" ) ;

  if ( maxval > 0 )
  {
    CString Text;

    Text.Format (_T("%d"), (int) maxval ) ;
    pDC->TextOut ( leftLine - 25, topLine, Text ) ;
  }

  if ( options.m_bathy && options.m_terrainAutoscale)
  {

    CString Text;

    pDC->MoveTo ( leftLine, middleLine ) ;
    pDC->LineTo ( leftLine, baseLine ) ;

    if ( minval < 0 )
    {

      Text.Format (_T("%d"), (int) minval ) ;
      pDC->TextOut ( leftLine - 25, baseLine, Text ) ;
    }
  }

  if ( !options.m_terrainAutoscale ) 
  {

    CString Text;

    pDC->MoveTo ( leftLine, baseLine ) ;
    pDC->LineTo ( leftLine, topLine ) ;
    pDC->TextOut ( leftLine - 25, middleLine, "0" ) ;
    Text.Format (_T("%d"), (int) maxval ) ;
    pDC->TextOut ( leftLine - 25, topLine, Text ) ;

    if ( options.m_bathy )
    {
      Text.Format (_T("-%d"), (int) minval ) ;
      pDC->TextOut ( leftLine - 25, baseLine, Text ) ;
    }

  }
}

/////////////////////////////////////////////////////////////////////////////
// CTerrainView diagnostics

#ifdef _DEBUG
void CTerrainView::AssertValid() const
{
  CView::AssertValid();
}

void CTerrainView::Dump(CDumpContext& dc) const
{
  CView::Dump(dc);
}
#endif //_DEBUG



