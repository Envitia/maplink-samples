// DOUnknown.cpp: implementation of the CDOUnknown class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DOUnknown.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static POINT outlineInit[12] = { {10,8}, {10,-8}, {8,-8}, {8,-10}, {-8,-10}, {-8,-8}, {-10,-8}, {-10,8}, {-8,8}, {-8,10}, {8,10}, {8,8} } ;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDOUnknown::CDOUnknown()
: CDOTrack()
{
  // Set the outline for the track
  memcpy( m_outline, outlineInit, sizeof( outlineInit ) ) ;
  // Set the positioning for the platform code text
  m_textX = 0 ;	  
  m_textY = 1 ;
}

CDOUnknown::~CDOUnknown()
{

}

void CDOUnknown::drawOutline( HDC surfaceDC )
{
  ///////////////////////////////////////////////////////////////
  // Create a brush with the track's hostility colour code
  // and a black outline
  HPEN	  pen	= ::CreatePen( PS_SOLID, 0, OUTLINE_COLOUR ) ;
  HBRUSH  brush = ::CreateSolidBrush( m_fillColour ) ;
  HANDLE  oldPen    = ::SelectObject( surfaceDC, pen ) ;
  HANDLE  oldBrush  = ::SelectObject( surfaceDC, brush ) ;
  ///////////////////////////////////////////////////////////////

  int elements = sizeof( m_outline ) / sizeof( POINT ) ;
  POINT pts[sizeof( m_outline ) / sizeof( POINT )] ;

  if ( elements > 0 )
  {
    for ( int i = 0 ; i < elements ; i++ )
    {
      pts[i].x = m_xPos + m_outline[i].x ;
      pts[i].y = m_yPos + m_outline[i].y ;
    }

    ::Polygon( surfaceDC, pts, elements ) ;
  }
  else
  {
    ::Ellipse( surfaceDC, m_xPos - 10, m_yPos - 10, m_xPos + 10, m_yPos + 10 ) ;
  }

  ////////////////////////////////////////////////////////
  // Reselect previous attributes and delete these ones
  ::SelectObject( surfaceDC, oldPen ) ;
  ::SelectObject( surfaceDC, oldBrush ) ;
  ::DeleteObject( pen ) ;
  ::DeleteObject( brush ) ;
  ///////////////////////////////////////////////////////
}