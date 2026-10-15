// AircraftDO.cpp: implementation of the AircraftDO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AircraftDO.h"
#include "Aircraft.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define OUTLINE_COLOUR	    0x00f0f0f0

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AircraftDO::AircraftDO()
{
  setSize( 0, 0, 0, 0 );
}

AircraftDO::~AircraftDO()
{
}

bool AircraftDO::draw(TSLRenderingInterface *d_surface, TSLEnvelope *d_extent )
{
  TSLCoord pos = m_ddo->position();

  TSLDeviceUnits wndX = 0;
  TSLDeviceUnits wndY = 0;
  
  d_surface->TMCToDU( pos.x(), pos.y(), &wndX, &wndY );

  HDC destDC = (HDC)(d_surface->handleToDrawable());

  double resolutionX, resolutionY ;
  d_surface->screenResolution(resolutionX, resolutionY);

  // Get the fill and text colours, according to the palette
  COLORREF col, txtcol;

  if ( GetDeviceCaps( destDC, NUMCOLORS ) != -1 )
  {
    // 256 colour palette
    col    = getColours().first;
    txtcol = getColours().second;
  }
  else
  {
    // 16 bit or more palette
    col    = getColours(false).first;
    txtcol = getColours(false).second;
  }

  // Now draw the symbol
  HPEN   pen      = ::CreatePen( PS_SOLID, 0, OUTLINE_COLOUR );
  HBRUSH brush    = ::CreateSolidBrush( col );

  HANDLE oldPen   = ::SelectObject( destDC, pen );
  HANDLE oldBrush = ::SelectObject( destDC, brush );

  POINT* src = 0;
  int num = 0;
  long tx = 0, ty = 0;

  if ( !getSymbolDetails( src, num, tx, ty ) )
  {
    return false;
  }

  double theta = ( (Aircraft*)m_ddo )->heading();
  double sinT = sin( theta );
  double cosT = cos( theta );

  POINT pts[64];
  for ( int i = 0; i < num; ++i )
  {
    double x = src[i].x*cosT - src[i].y*sinT;
    double y = src[i].y*cosT + src[i].x*sinT;

    pts[i].x = wndX + x;
    pts[i].y = wndY + y;
  }
  
  ::Polygon( destDC, pts, num );

  // Reselect previous attributes and delete these ones
  ::SelectObject( destDC, oldPen );
  ::SelectObject( destDC, oldBrush );

  ::DeleteObject( pen );
  ::DeleteObject( brush );

  return true;
}

