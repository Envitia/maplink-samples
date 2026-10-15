// DOOwnShip.cpp: implementation of the CDOOwnShip class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DOOwnShip.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDOOwnShip::CDOOwnShip()
: CDOTrack()
{

}

CDOOwnShip::~CDOOwnShip()
{

}

void CDOOwnShip::drawOutline( HDC surfaceDC )
{
  ///////////////////////////////////////////////////////////////
  // Create a brush with the track's hostility colour code
  // and a black outline
  HPEN	  pen	= ::CreatePen( PS_SOLID, 0, OUTLINE_COLOUR ) ;
  HBRUSH  brush = ::CreateSolidBrush( m_fillColour ) ;
  HANDLE  oldPen    = ::SelectObject( surfaceDC, pen ) ;
  HANDLE  oldBrush  = ::SelectObject( surfaceDC, brush ) ;
  ///////////////////////////////////////////////////////////////

  ::Rectangle( surfaceDC, m_xPos - 10, m_yPos - 10, m_xPos + 10, m_yPos + 11 );
  pen = ::CreatePen( PS_SOLID, 0, 0xFFFFFF );
  ::DeleteObject( ::SelectObject( surfaceDC, pen ) );
  ::MoveToEx( surfaceDC, m_xPos - 10, m_yPos - 10, 0 );
  ::LineTo( surfaceDC, m_xPos + 10, m_yPos + 10 );
  ::MoveToEx( surfaceDC, m_xPos - 10, m_yPos + 10, 0 );
  ::LineTo( surfaceDC, m_xPos + 10, m_yPos - 10 );

  ////////////////////////////////////////////////////////
  // Reselect previous attributes and delete these ones
  ::SelectObject( surfaceDC, oldPen ) ;
  ::SelectObject( surfaceDC, oldBrush ) ;
  ::DeleteObject( pen ) ;
  ::DeleteObject( brush ) ;
  ///////////////////////////////////////////////////////
}

void CDOOwnShip::showAsSelectedTrack( HDC surfaceDC )
{
  ::SelectObject( surfaceDC, GetStockObject( NULL_BRUSH ) ) ;
  ::SelectObject( surfaceDC, GetStockObject( BLACK_PEN ) ) ;
  ::Rectangle( surfaceDC, m_xPos - 13, m_yPos - 13, m_xPos + 13, m_yPos + 14 ) ;
}

