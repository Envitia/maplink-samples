//	********************************************************
//	*           Source Control Information
//	********************************************************
//	$Archive: /MapLinkWizard/Template/tslviewmodeinterfacent.cpp $
//	$Author$
//	$Date$
//	$Revision$
//	********************************************************

#include "stdafx.h"
#include "tslviewmodeinterfacent.h"

TSLViewModeInterfaceNT::TSLViewModeInterfaceNT( TSLDrawingSurface * drawingSurface, HWND hwnd )
  : TSLViewModeInterface( drawingSurface ), m_hwnd( hwnd )
{

}

TSLViewModeInterfaceNT::~TSLViewModeInterfaceNT()
{

}

void TSLViewModeInterfaceNT::drawEchoRectangle( )
{
  if ( m_draw && m_hwnd )
  {
    // Do not use MFC, so we can work in Win32 as well.
    HDC hdc = ::GetDC( m_hwnd ) ;

    // Set up a simple Exclusive-Or pen.
    int oldMode = ::SetROP2 ( hdc, R2_XORPEN ) ;
    ::SelectObject ( hdc, ::GetStockObject (NULL_BRUSH ) ) ;
    ::SelectObject ( hdc, ::GetStockObject ( WHITE_PEN ) ) ;

    // Draw the rectangle, assuming x1,y1 < x2,y2
    ::Rectangle( hdc, m_x1, m_y1, m_x2, m_y2 ) ;

    // Reset back to the prevoius drawing mode.
    ::SetROP2 ( hdc, oldMode  ) ;

    ::ReleaseDC( m_hwnd, hdc ) ;
  }
}

void TSLViewModeInterfaceNT::drawEchoLine( )
{
  if ( m_draw && m_hwnd )
  {
    // Do not use MFC, so we can work in Win32 as well.
    HDC hdc = ::GetDC( m_hwnd ) ;

    // Set up a simple Exclusive-Or pen.
    int oldMode = ::SetROP2 ( hdc, R2_XORPEN ) ;
    ::SelectObject ( hdc, ::GetStockObject (NULL_BRUSH ) ) ;
    ::SelectObject ( hdc, ::GetStockObject ( WHITE_PEN ) ) ;

	POINT ps [2] ;
	ps[0].x = m_x1 ;
	ps[0].y = m_y1 ;
	ps[1].x = m_x2 ;
	ps[1].y = m_y2 ;


    // Draw the rectangle, assuming x1,y1 < x2,y2
	::Polyline(hdc,ps,2) ;
    // Reset back to the prevoius drawing mode.
    ::SetROP2 ( hdc, oldMode  ) ;

    ::ReleaseDC( m_hwnd, hdc ) ;
  }
}

void TSLViewModeInterfaceNT::captureMouse( ) 
{
  // Redirect all mouse events to my window.
  SetCapture( m_hwnd ) ;
}

void TSLViewModeInterfaceNT::releaseMouse( )
{
  // Cancel mouse event redirection.
  ReleaseCapture( ) ;
}
