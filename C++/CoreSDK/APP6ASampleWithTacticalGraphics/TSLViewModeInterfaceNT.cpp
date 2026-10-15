//	********************************************************
//	*           Source Control Information
//	********************************************************
//	$Archive: /Development/Samples/NT/LandLinkSample/TSLViewModeInterfaceNT.cpp $
//	$Author$
//	$Date$
//	$Revision$
//	********************************************************

#include "stdafx.h"
#include "TSLViewModeInterfaceNT.h"
#include "TSLMagnifyingGlass.h"

TSLViewModeInterfaceNT::TSLViewModeInterfaceNT( TSLDrawingSurface * drawingSurface, HWND hwnd )
  : TSLViewModeInterface  ( drawingSurface ),
    m_hwnd		  ( hwnd ),
    m_doBuffering	  ( false ),
    m_hBufferDC		  ( 0 ),
    m_hBufferBitmap	  ( 0 ),
    m_hBackgroundDC	  ( 0 ),
    m_hBackgroundBitmap	  ( 0 ),
    m_maskDC		  ( 0 ),
    m_maskBitmap	  ( 0 ),
    m_bgInitialised	  ( false )
{
}

TSLViewModeInterfaceNT::~TSLViewModeInterfaceNT()
{
  if ( m_doBuffering )
  {
    killBuffers();
  }
}

void TSLViewModeInterfaceNT::killBuffers()
{
  if ( m_hBackgroundDC )
  {
    ::DeleteDC( m_hBackgroundDC );

    m_hBackgroundDC = 0;
  }
  
  if ( m_hBackgroundBitmap )
  {
    ::DeleteObject( m_hBackgroundBitmap );

    m_hBackgroundBitmap = 0;
  }
  
  if ( m_hBufferDC )
  {
    ::DeleteDC( m_hBufferDC );

    m_hBufferDC = 0;
  }
  
  if ( m_hBufferBitmap )
  {
    ::DeleteObject( m_hBufferBitmap );

    m_hBufferBitmap = 0;
  }
  
  if ( m_maskDC )
  {
    ::DeleteDC( m_maskDC );

    m_maskDC = 0;
  }
  
  if ( m_maskBitmap )
  {
    ::DeleteObject( m_maskBitmap );

    m_maskBitmap = 0;
  }
}

void TSLViewModeInterfaceNT::drawEchoRectangle( )
{
  if ( m_drawRect && m_hwnd )
  {
    // Do not use MFC, so we can work in Win32 as well.
    HDC hdc = ::GetDC( m_hwnd ) ;

    // Set up a simple Exclusive-Or pen.
    int oldMode = ::SetROP2 ( hdc, R2_XORPEN ) ;
    ::SelectObject ( hdc, ::GetStockObject (NULL_BRUSH ) ) ;
    ::SelectObject ( hdc, ::GetStockObject ( WHITE_PEN ) ) ;

    // Draw the rectangle, assuming x1,y1 < x2,y2
    ::Rectangle( hdc, m_x1, m_y1, m_x2, m_y2 ) ;

    // Reset back to the previous drawing mode.
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

void TSLViewModeInterfaceNT::drawEchoLine( )
{
  if ( m_drawLine && m_hwnd )
  {
    // Do not use MFC, so we can work in Win32 as well.
    HDC hdc = ::GetDC( m_hwnd ) ;

    // Set up a simple Exclusive-Or pen.
    int oldMode = ::SetROP2 ( hdc, R2_XORPEN ) ;
    ::SelectObject ( hdc, ::GetStockObject ( WHITE_PEN ) ) ;

    // Draw the rectangle, assuming x1,y1 < x2,y2
    ::MoveToEx( hdc, m_x1, m_y1, 0 );
    ::LineTo( hdc, m_x2, m_y2 );

    // Reset back to the previous drawing mode.
    ::SetROP2 ( hdc, oldMode  ) ;

    ::ReleaseDC( m_hwnd, hdc ) ;
  }
}

void TSLViewModeInterfaceNT::bufferingOn()
{
  m_doBuffering = true;

  TSLDeviceUnits x1;
  TSLDeviceUnits y1;
  TSLDeviceUnits x2;
  TSLDeviceUnits y2;

  m_drawingSurface->getDUExtent( &x1, &y1, &x2, &y2 );

  onSize( x2-x1, y2-y1 );
}

void TSLViewModeInterfaceNT::bufferingOff()
{
  m_doBuffering = false;

  killBuffers();
}

void TSLViewModeInterfaceNT::onSize( long cx, long cy )
{
  if ( m_doBuffering )
  {
    if ( !m_hBufferDC )
    {
      HDC temp_hdc = ::GetDC( m_hwnd );
      m_hBufferDC  = ::CreateCompatibleDC( temp_hdc );
      ::ReleaseDC( m_hwnd, temp_hdc );
    }
    
    if ( m_hBufferBitmap )
    {
      ::DeleteObject( m_hBufferBitmap );
    }
    
    HDC temp_hdc    = ::GetDC( m_hwnd );
    m_hBufferBitmap = ::CreateCompatibleBitmap( temp_hdc, cx, cy );
    ::ReleaseDC( m_hwnd, temp_hdc );
  }
}

void TSLViewModeInterfaceNT::setupBackgroundBuffer( long width, long height )
{
  if ( m_doBuffering )
  {
    if ( !m_hBackgroundBitmap )
    {
      HDC temp_hdc        = ::GetDC( m_hwnd );
      m_hBackgroundBitmap = ::CreateCompatibleBitmap( temp_hdc, width, height );
      m_hBackgroundDC     = ::CreateCompatibleDC( temp_hdc );
      ::SelectObject( m_hBackgroundDC , m_hBackgroundBitmap );
      
      m_hBufferBitmap = ::CreateCompatibleBitmap( temp_hdc, width, height );
      m_hBufferDC     = ::CreateCompatibleDC( temp_hdc );
      ::SelectObject( m_hBufferDC, m_hBufferBitmap );
      
      ::ReleaseDC( m_hwnd, temp_hdc );
      
      m_maskDC     = ::CreateCompatibleDC( m_hBufferDC );
      m_maskBitmap = ::CreateCompatibleBitmap( m_maskDC, width, height );
    }
  }
}

void TSLViewModeInterfaceNT::magnify( TSLMagnifyingGlass* magnifier, bool restoreBackground )
{
  if ( m_doBuffering )
  {
    TSLDeviceUnits x1;
    TSLDeviceUnits y1;
    TSLDeviceUnits x2;
    TSLDeviceUnits y2;
    
    m_drawingSurface->getDUExtent( &x1, &y1, &x2, &y2 );
    
    HGDIOBJ oldBitmap = ::SelectObject( m_hBufferDC, m_hBufferBitmap );
    HRGN       region = ::CreateRectRgn( 0, 0, x2-x1, y2-y1 );
    HGDIOBJ oldRegion = ::SelectObject( m_hBufferDC, region );
    
    double ux1;
    double uy1;
    double ux2;
    double uy2;
    
    m_drawingSurface->getUUExtent( &ux1, &uy1, &ux2, &uy2 );
    
    ((TSLNTSurface*)m_drawingSurface)->drawToHDC( (TSLDeviceContext)m_hBufferDC, ux1, uy1, ux2, uy2, true );
    
    magnifier->draw( m_hBufferDC, restoreBackground );
    
    HDC hdc = ::GetDC( m_hwnd );
    ::BitBlt( hdc, 0, 0, x2-x1, y2-y1, m_hBufferDC, 0, 0, SRCCOPY );
    ::SelectObject( m_hBufferDC, oldBitmap );
    ::SelectObject( m_hBufferDC, oldRegion );
    ::ReleaseDC( m_hwnd, hdc );
    ::DeleteObject( region );
  }
}

void TSLViewModeInterfaceNT::drawMagnifierToBuffer( HDC buffer, long x1, long y1, long x2, long y2, long width, long height, bool restoreBackground, long boundaryThickness, long boundaryRed, long boundaryGreen, long boundaryBlue  )
{
  if ( m_doBuffering )
  {
    HDC hdc = (HDC)buffer;
    
    if ( hdc )
    {
      if ( m_bgInitialised && restoreBackground )
      {
	// Restore the background
	::BitBlt( hdc, x1, y1, width, height, m_hBackgroundDC, 0, 0, SRCCOPY );
      }
      
      // Grab the current background
      if ( restoreBackground && ::BitBlt( m_hBackgroundDC, 0, 0, width, height, hdc, x2, y2, SRCCOPY ) )
      {
	m_bgInitialised = true;
      }
    
      HRGN    rectRegion = ::CreateRectRgn( 0, 0, width, height );
      HGDIOBJ oldRRegion = ::SelectObject( m_hBufferDC, rectRegion );
      
      ::BitBlt( m_hBufferDC, 0, 0, width, height, hdc, x2, y2, SRCCOPY );
      
      HRGN ellipseRegion = ::CreateEllipticRgn( 0, 0, width, height );
      HGDIOBJ oldERegion = ::SelectObject( m_hBufferDC, ellipseRegion );
      
      double uux1;
      double uux2;
      double uuy1;
      double uuy2;
      
      m_drawingSurface->getUUExtent( &uux1, &uuy1, &uux2, &uuy2 );
      ((TSLNTSurface*)m_drawingSurface)->drawToHDC( (TSLDeviceContext)m_hBufferDC, uux1, uuy1, uux2, uuy2, true );    
      
      //-------------------- Masking stuff --------------------//
      //
      // Need to use a mask since GMS overwrites the clipping region.
      //
      ::SelectObject( m_maskDC, m_maskBitmap );
      
      ::SelectObject( m_maskDC, GetStockObject(BLACK_BRUSH) );
      ::Rectangle( m_maskDC, 0, 0, width, height );
      
      ::SelectObject( m_maskDC, GetStockObject(WHITE_BRUSH) );
      ::Ellipse( m_maskDC, 0, 0, width, height );
      
      ::BitBlt( m_hBufferDC, 0, 0, width, height, m_maskDC, 0, 0, SRCAND );
      ::BitBlt( hdc, x2, y2, width, height, m_maskDC, 0, 0, 0x220326 ); // D & ~S
      
      if ( boundaryThickness )
      {
	HPEN       pen = ::CreatePen( PS_SOLID, boundaryThickness, 
				      RGB( boundaryRed, boundaryGreen, boundaryBlue ) ) ;
	HGDIOBJ oldPen = ::SelectObject( m_hBufferDC, pen );
      
	::SelectObject( m_hBufferDC, ::GetStockObject(NULL_BRUSH) );
	::Ellipse( m_hBufferDC, 2, 2, width-2, height-2 );
	::SelectObject( m_hBufferDC, oldPen );
	::DeleteObject( pen );
      }
      ::BitBlt( hdc, x2, y2, width, height, m_hBufferDC, 0, 0, SRCPAINT );
      //
      //-------------------- Masking stuff --------------------//
      
      ::SelectObject( m_hBufferDC, oldRRegion );
      ::SelectObject( m_hBufferDC, oldERegion );

      ::DeleteObject( ellipseRegion );
      ::DeleteObject( rectRegion );
    }
  }
}

TSLViewModeInterface * TSLViewModeInterfaceNT::clone()
{
  return new TSLViewModeInterfaceNT( m_drawingSurface->clone(), m_hwnd ) ;
}

