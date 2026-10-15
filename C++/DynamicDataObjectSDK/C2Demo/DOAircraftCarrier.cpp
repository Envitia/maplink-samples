// DOAircraftCarrier.cpp: implementation of the CDOAircraftCarrier class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DOAircraftCarrier.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static POINT outlineInit[12] = { {-10,5}, {-10,-5}, {-7,-5}, {-7,-8}, {7,-8}, {7,-5}, {10,-5}, {10,5}, {7,5}, {7,8}, {-7,8}, {-7,5} } ;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDOAircraftCarrier::CDOAircraftCarrier()
: CDOTrack()
{
  memcpy( m_outline, outlineInit, sizeof( outlineInit ) ) ;
  // Set the positioning for the platform code text
  m_textX = 0 ;	  
  m_textY = 0 ;
}

CDOAircraftCarrier::~CDOAircraftCarrier()
{

}

bool CDOAircraftCarrier::drawOutline( HDC surfaceDC )
{
  ///////////////////////////////////////////////////////////////
  // Create a brush with the track's hostility colour code
  // and a black outline
  HPEN	  pen	    = ::CreatePen( PS_SOLID, 0, OUTLINE_COLOUR ) ;
  HBRUSH  brush	    = ::CreateSolidBrush( m_fillColour ) ;
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

  return true ;
}
