// DOTrack.cpp: implementation of the CDOTrack class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DOTrack.h"
#include "Track.h"
#include "TrackManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

HANDLE	CDOTrack::m_font = NULL ;
int	CDOTrack::m_doReference = 0 ;

CDOTrack::CDOTrack()
{
  if ( ++m_doReference == 1 )
  {
    m_font = CreateFont( 13, 0, 0, 0, 400, 0,0,0, 0,0,0,0,0,_T("Helvetica") ) ;
  }
  setSize( 0,0,0,0 ) ;
}

CDOTrack::~CDOTrack()
{
  if ( --m_doReference == 0 )
  {
    DeleteObject( m_font ) ;
    m_font = 0 ;
  }
}

// Set the fill and text colour member variables depending
// on the track's hostility
bool CDOTrack::setColours( HDC surfaceDC, CTrack * pTrack )
{
  if ( GetDeviceCaps( surfaceDC, NUMCOLORS ) != -1 )
  {
    // 256 colour palette
    switch ( pTrack->hostility() )
    {
      case HOSTILE :
	m_fillColour = HOSTILE_FILL_256 ;
	m_textColour = HOSTILE_TEXT_256 ;
	break ;

      case FRIENDLY :
	m_fillColour = FRIENDLY_FILL_256 ;
	m_textColour = FRIENDLY_TEXT_256 ;
	break ;

      case NEUTRAL :
	m_fillColour = NEUTRAL_FILL_256 ;
	m_textColour = NEUTRAL_TEXT_256 ;
	break ;

      case UNKNOWN :
	m_fillColour = UNKNOWN_FILL_256 ;
	m_textColour = UNKNOWN_TEXT_256 ;
	break ;
    }
  }
  else
  {
    // 16 bit or more palette
    switch ( pTrack->hostility() )
    {
      case HOSTILE :
	m_fillColour = HOSTILE_FILL ;
	m_textColour = HOSTILE_TEXT ;
	break ;

      case FRIENDLY :
	m_fillColour = FRIENDLY_FILL ;
	m_textColour = FRIENDLY_TEXT ;
	break ;

      case NEUTRAL :
	m_fillColour = NEUTRAL_FILL ;
	m_textColour = NEUTRAL_TEXT ;
	break ;

      case UNKNOWN :
	m_fillColour = UNKNOWN_FILL ;
	m_textColour = UNKNOWN_TEXT ;
	break ;
    }
  }

  return true ;  
}

bool CDOTrack::draw ( TSLRenderingInterface *d_surface, TSLEnvelope *d_extent )
{
  // Retrieve a pointer to the associated track
  CTrack * pTrack = (CTrack *)m_ddo ;
  
  if ( !CTrackManager::displayTrackType( pTrack->hostility() ) )
  {
    return false ;
  }

  // Get the current position of the track in terms of pixels
  TSLCoord coord = pTrack->position() ;
  TSLDeviceUnits	xCo, yCo ;
  d_surface->TMCToDU( coord.x(), coord.y(), &xCo, &yCo ) ;
  m_xPos = (double)xCo;
  m_yPos = (double)yCo;

  HDC surfaceDC = (HDC)(d_surface->handleToDrawable()) ;

  int oldMode = ::SetBkMode( surfaceDC, TRANSPARENT ) ;

  // Set the fill and text colours based on the track hostility
  setColours( surfaceDC, pTrack ) ;
 
  if ( CTrackManager::displayHistoryPoints() )
  {
    drawHistoryPoints( d_surface, pTrack ) ;
  }

  if ( CTrackManager::displayVelocityVectors() )
  {
    drawVelocityVector( d_surface, pTrack ) ;
  }

  // Draw the track outline
  drawOutline( surfaceDC ) ;

  if ( CTrackManager::selectedTrack() == pTrack )
  {
    showAsSelectedTrack( surfaceDC ) ;
  }

  // Set the font for labels
  HANDLE oldFont = 0 ;
  
  if ( m_font ) 
    oldFont = ::SelectObject( surfaceDC, m_font ) ;
  
  if ( CTrackManager::displayTrackNumbers() )
  {
    drawTrackNumber( surfaceDC, pTrack ) ;
  }

  if ( CTrackManager::displayPlatformCodes() )
  {
    drawPlatformCode( surfaceDC, pTrack ) ;
  }

  // Now select the old font
  if ( oldFont )
    ::SelectObject( surfaceDC, oldFont ) ;

  oldMode = ::SetBkMode( surfaceDC, oldMode ) ;
 
  return true;
}

// Draw the last (max 10) points that the track has passed through
bool CDOTrack::drawHistoryPoints( TSLRenderingInterface *d_surface, CTrack * pTrack )
{
  //Draw History points
  POINT pts[ 10 ] ;
  vector< pair<long,long> >::const_iterator  h;

  // Get handle to the surface DC
  HDC surfaceDC = (HDC)(d_surface->handleToDrawable()) ;

  // Create a brush and pen with the current fill colour
  HBRUSH brush = ::CreateSolidBrush( m_fillColour ) ;
  HANDLE oldBrush = ::SelectObject( surfaceDC, brush ) ;
  HPEN pen  = ::CreatePen( PS_SOLID, 0, m_fillColour ) ;
  HANDLE oldPen = ::SelectObject( surfaceDC, pen ) ;

  for ( h = pTrack->historyPoints().begin(); h != pTrack->historyPoints().end(); h++ )
  {
    TSLDeviceUnits  hx, hy;
    d_surface->TMCToDU( (*h).first, (*h).second, &hx, &hy );
    pts[0].x = hx - 2;
    pts[0].y = hy - 2;
    pts[1].x = hx - 2;
    pts[1].y = hy + 2;
    pts[2].x = hx + 2;
    pts[2].y = hy + 2;
    pts[3].x = hx + 2;
    pts[3].y = hy - 2;
    pts[4].x = hx - 2;
    pts[4].y = hy - 2;
    ::Polygon( surfaceDC, pts, 5 ) ;
  }

  // Reselect the old pen and brush to the DC
  ::SelectObject( surfaceDC, oldBrush ) ;
  ::DeleteObject( brush ) ;
  ::SelectObject( surfaceDC, oldPen ) ;
  ::DeleteObject( pen ) ;

    return true ;
}

// Draw the platform code on the track: hostility & type
bool CDOTrack::drawPlatformCode( HDC surfaceDC, CTrack * pTrack )
{
  char platformCode[PLATFORM_DESCRIPTION];

  CTrackManager::platformDescription( pTrack, platformCode ) ;

  // Now draw the platform code
  ::SetTextAlign( surfaceDC, TA_CENTER | TA_TOP ) ;
  ::SetTextColor( surfaceDC, m_textColour ) ;
  CString platCode = (LPCTSTR)TSLUTF8Decoder( platformCode );
  ::TextOut( surfaceDC, m_xPos + m_textX, m_yPos + m_textY - 7, platCode, platCode.GetLength() ) ;
  
  return true ;

}

// Draw the track's ID number
bool CDOTrack::drawTrackNumber( HDC surfaceDC, CTrack * track )
{
  char label[PLATFORM_DESCRIPTION] ; label[0] = '\0' ;
  sprintf( label, "%d", track->trackNumber() ) ;

  if ( label[0] )
  {
    // Now draw the track label
    ::SetTextAlign( surfaceDC, TA_CENTER | TA_TOP ) ;
    ::SetTextColor( surfaceDC, RGB( 255, 255, 255 ) ) ;
    CString labl = (LPCTSTR)TSLUTF8Decoder( label );
    ::TextOut( surfaceDC, m_xPos, m_yPos + 11, labl, labl.GetLength() ) ;

    return true ;
  }

  return false ;
}

// Draw a bounding rectangle around the track to indicate the track is selected
bool CDOTrack::showAsSelectedTrack( HDC surfaceDC )
{
  ::SelectObject( surfaceDC, GetStockObject( NULL_BRUSH ) ) ;
  ::SelectObject( surfaceDC, GetStockObject( WHITE_PEN ) ) ;
  ::Rectangle( surfaceDC, m_xPos - 13, m_yPos - 13, m_xPos + 13, m_yPos + 13 ) ;

  return true ;
}

// Draw the vector lines from the front of the track showing the track bearing
// and indicating the velocity of the track
bool CDOTrack::drawVelocityVector( TSLRenderingInterface *d_surface, CTrack *pTrack )
{
  long vX, vY ;
  pTrack->velocityVectorData( vX, vY ) ;

  HDC surfaceDC = (HDC)(d_surface->handleToDrawable()) ;

  // Draw velocity vector
  if ( ( vX || vY ) )
  {
    POINT p[2] ;

    double angle = -d_surface->rotation() ;

    if ( angle )
    {
      double x = vX * cos( angle ) - vY * sin( angle ) ;
      double y = vY * cos( angle ) + vX * sin( angle ) ;
      vX = x ;
      vY = y ;
    }

    p[0].x = m_xPos ;
    p[0].y = m_yPos ;
    p[1].x = m_xPos + vX ;
    p[1].y = m_yPos + vY ;

    HPEN pen = ::CreatePen( PS_SOLID, 0, m_fillColour ) ;
    HANDLE oldPen = ::SelectObject( surfaceDC, pen ) ;

    ::Polyline( surfaceDC, p, 2 ) ;
    ::SelectObject( surfaceDC, oldPen ) ;
    ::DeleteObject( pen ) ;

    return true ;
  }

  return false ;
}