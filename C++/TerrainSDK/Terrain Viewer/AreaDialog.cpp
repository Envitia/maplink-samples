// AreaDialog.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "AreaDialog.h"
#include "GLScatterGraph.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// AreaDialog dialog


AreaDialog::AreaDialog(CWnd* pParent /*=NULL*/)
	: CDialog(AreaDialog::IDD, pParent), m_graph( 0 ), m_data( 0 )
{
	//{{AFX_DATA_INIT(AreaDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
  m_graph = new CGLScatterGraph;
  m_data = NULL;
  m_colours = NULL;
}

AreaDialog::~AreaDialog()
{
  delete m_graph;
  delete []m_data;  
  delete []m_colours;
}

void AreaDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(AreaDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(AreaDialog, CDialog)
	//{{AFX_MSG_MAP(AreaDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// AreaDialog message handlers

BOOL AreaDialog::OnInitDialog() 
{
  CDialog::OnInitDialog();

  CRect rect;
  GetClientRect( rect );

  m_graph->Create( NULL, NULL, WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VISIBLE, rect, this, 0 );
  m_graph->SetAutoScaleY( FALSE );
  m_graph->SetMinY( -10000.0F );
  m_graph->SetMaxY( 10000.0F );
  if ( m_data )
    m_graph->SetData( m_numData / 3, RGB( 0xff, 0x00, 0x00 ), m_data, m_colours );
  return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
}

void AreaDialog::setData( double minLat, double minLon, double maxLat, double maxLon, int numLat, int numLon, double* data )
{
  float* data3D = new float[ numLat * numLon * 3 ];
  delete []m_colours;
  m_colours = new COLORREF[ numLat * numLon ];
  memset( m_colours, 255, numLat * numLon * sizeof( COLORREF ) );
  double diffLat = ( maxLat - minLat ) / (double)numLat;
  double diffLon = ( maxLon - minLon ) / (double)numLon;
  int index = 0;
  int dataIndex = 0;
  for ( int y = 0; y < numLat; y++ )
  {
    double thisLat = minLat + (double)y * diffLat;
    for ( int x = 0; x < numLon; x++ )
    {
      double thisLon = minLon + (double)x * diffLon;
      int col = data[dataIndex] / 3.0;
      bool bNeg = false;
      if ( col <= 0 )
      {
	bNeg = true;
	col = -col;
      }
      if ( col > 255 )
	col = 255;
      if ( !bNeg && ( col < 50 ) )
	col = 50;
      if ( bNeg )
      {
	col = 255 - col;
	if ( col < 150 )
	  col = 150;
      }
      if ( !bNeg )
	m_colours[dataIndex] = RGB( 0, col, 0 );
      else
	m_colours[dataIndex] = RGB( 0, col, col );
      data3D[index++] = thisLon + 180.0;
      data3D[index++] = (float)data[dataIndex++];
      data3D[index++] = thisLat + 90.0;
    }
  }
  delete [] m_data;
  m_data = data3D;  
  m_numData = index;
  m_numLat = numLat;
  m_numLon = numLon;
}
