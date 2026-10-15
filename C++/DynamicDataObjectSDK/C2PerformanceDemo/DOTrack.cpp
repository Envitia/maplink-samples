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

//
// Creating GDI objects and deleting them are expensive.
// We create them on first use.
//
HBRUSH CDOTrack::friendlyFill = NULL;
HPEN   CDOTrack::friendlyText = NULL;
HBRUSH CDOTrack::neutralFill = NULL;
HPEN   CDOTrack::neutralText = NULL;
HBRUSH CDOTrack::unknownFill = NULL;
HPEN   CDOTrack::unknownText = NULL;
HBRUSH CDOTrack::hostileFill = NULL;
HPEN   CDOTrack::hostileText = NULL;

class Tidy
{
public:
  Tidy() {}
  ~Tidy()
  {
    ::DeleteObject( CDOTrack::friendlyFill ) ;
    ::DeleteObject( CDOTrack::friendlyText ) ;
    ::DeleteObject( CDOTrack::neutralFill ) ;
    ::DeleteObject( CDOTrack::neutralText ) ;
    ::DeleteObject( CDOTrack::unknownFill ) ;
    ::DeleteObject( CDOTrack::unknownText ) ;
    ::DeleteObject( CDOTrack::hostileFill ) ;
    ::DeleteObject( CDOTrack::hostileText ) ;
  }
};

Tidy tidy;

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
    case ASSUMED_FRIEND:
    case FRIENDLY :
      m_fillColour = FRIENDLY_FILL_256 ;
      m_textColour = FRIENDLY_TEXT_256 ;
      if (!friendlyFill)
        friendlyFill = ::CreateSolidBrush( m_fillColour );
      if (!friendlyText)
        friendlyText = ::CreatePen( PS_SOLID, 0, m_fillColour ) ;
      m_brush = friendlyFill;
      m_pen = friendlyText ;
      break ;

    case NEUTRAL :
      m_fillColour = NEUTRAL_FILL_256 ;
      m_textColour = NEUTRAL_TEXT_256 ;
      if (!neutralFill)
        neutralFill = ::CreateSolidBrush( m_fillColour );
      if (!neutralText)
        neutralText = ::CreatePen( PS_SOLID, 0, m_fillColour ) ;
      m_brush = neutralFill;
      m_pen = neutralText ;
      break ;

    case PENDING:
    case UNKNOWN :
      m_fillColour = UNKNOWN_FILL_256 ;
      m_textColour = UNKNOWN_TEXT_256 ;
      if (!unknownFill)
        unknownFill = ::CreateSolidBrush( m_fillColour );
      if (!unknownText)
        unknownText = ::CreatePen( PS_SOLID, 0, m_fillColour ) ;
      m_brush = unknownFill;
      m_pen = unknownText ;
      break ;

    case HOSTILE :
    default:
      m_fillColour = HOSTILE_FILL_256 ;
      m_textColour = HOSTILE_TEXT_256 ;
      if (!hostileFill)
        hostileFill = ::CreateSolidBrush( m_fillColour );
      if (!hostileText)
        hostileText = ::CreatePen( PS_SOLID, 0, m_fillColour ) ;
      m_brush = hostileFill;
      m_pen = hostileText ;
      break ;
    }
  }
  else
  {
    // 16 bit or more palette
    switch ( pTrack->hostility() )
    {
    case ASSUMED_FRIEND:
    case FRIENDLY :
      m_fillColour = FRIENDLY_FILL ;
      m_textColour = FRIENDLY_TEXT ;
      if (!friendlyFill)
        friendlyFill = ::CreateSolidBrush( m_fillColour );
      if (!friendlyText)
        friendlyText = ::CreatePen( PS_SOLID, 0, m_fillColour ) ;
      m_brush = friendlyFill;
      m_pen = friendlyText ;
      break ;

    case NEUTRAL :
      m_fillColour = NEUTRAL_FILL ;
      m_textColour = NEUTRAL_TEXT ;
      if (!neutralFill)
        neutralFill = ::CreateSolidBrush( m_fillColour );
      if (!neutralText)
        neutralText = ::CreatePen( PS_SOLID, 0, m_fillColour ) ;
      m_brush = neutralFill;
      m_pen = neutralText ;
      break ;

    case PENDING:
    case UNKNOWN :
      m_fillColour = UNKNOWN_FILL ;
      m_textColour = UNKNOWN_TEXT ;
      if (!unknownFill)
        unknownFill = ::CreateSolidBrush( m_fillColour );
      if (!unknownText)
        unknownText = ::CreatePen( PS_SOLID, 0, m_fillColour ) ;
      m_brush = unknownFill;
      m_pen = unknownText ;
      break ;

    case HOSTILE :
    default:
      m_fillColour = HOSTILE_FILL ;
      m_textColour = HOSTILE_TEXT ;
      if (!hostileFill)
        hostileFill = ::CreateSolidBrush( m_fillColour );
      if (!hostileText)
        hostileText = ::CreatePen( PS_SOLID, 0, m_fillColour ) ;
      m_brush = hostileFill;
      m_pen = hostileText ;
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
  TSLTMC	xCo, yCo ;
  d_surface->TMCToDU( coord.x(), coord.y(), &xCo, &yCo ) ;
  m_xPos = xCo;
  m_yPos = yCo;

  HDC surfaceDC = (HDC)(d_surface->handleToDrawable()) ;

  int oldMode = ::SetBkMode( surfaceDC, TRANSPARENT ) ;

  // Set the fill and text colours based on the track hostility
  setColours( surfaceDC, pTrack ) ;

  // We would suggest that the history, and velocity vectors could be
  // draw by a pre-layer render callback. This is to avoid swapping
  // pens and brushes.
  //
  // Sorted into hostility the history could be drawn first, then velocity vectors.
  // the resulting display would be clearer to read from a track point of 
  // view.
  //
  // For the OpenGL surface you should use TSLEntitySet's or TSLEntity for
  // drawing geometry. Caching the entities is required for performance.
  //
  // See TSLDataLayerDrawingCallback.
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
  ::SetTextAlign( surfaceDC, TA_CENTER | TA_TOP ) ;

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

  // Get handle to the surface DC
  HDC surfaceDC = (HDC)(d_surface->handleToDrawable()) ;

  // Create a brush with the current fill colour

  HGDIOBJ old = ::SelectObject(surfaceDC,(HGDIOBJ) m_brush);
  //HDC memDC = CTrackManager::historyDC();

  vector<pair<long,long> >::const_iterator h( pTrack->historyPoints().begin() );
  vector<pair<long,long> >::const_iterator hE( pTrack->historyPoints().end() );

  for ( ; h != hE; ++h )
  {
    pair<long,long> const& p = *h;
    TSLTMC hx, hy;
    d_surface->TMCToDU( p.first, p.second, &hx, &hy );
    ::Rectangle(surfaceDC, hx-BMP_DX/2, hy-BMP_DY/2, hx+BMP_DX/2, hy+BMP_DY/2);
  }

  SelectObject(surfaceDC, old);

  return true ;
}

// Draw the platform code on the track: hostility & type
void CDOTrack::drawPlatformCode( HDC surfaceDC, CTrack * pTrack )
{
  char platformCode[PLATFORM_DESCRIPTION];

  CTrackManager::platformDescription( pTrack, platformCode ) ;

  // Now draw the platform code
  ::SetTextColor( surfaceDC, m_textColour ) ;
  ::TextOutA( surfaceDC, m_xPos + m_textX, m_yPos + m_textY - 7, platformCode, 3 ) ;
}

// Draw the track's ID number
void CDOTrack::drawTrackNumber( HDC surfaceDC, CTrack * track )
{
  char const* label = track->trackLabel();

  if ( label )
  {
    ::SetTextColor( surfaceDC, RGB( 255, 255, 255 ) ) ;
    ::TextOutA( surfaceDC, m_xPos, m_yPos + 11, label, track->trackLabelLen() ) ;
  }
}

// Draw a bounding rectangle around the track to indicate the track is selected
void CDOTrack::showAsSelectedTrack( HDC surfaceDC )
{
  ::SelectObject( surfaceDC, GetStockObject( NULL_BRUSH ) ) ;
  ::SelectObject( surfaceDC, GetStockObject( WHITE_PEN ) ) ;
  ::Rectangle( surfaceDC, m_xPos - 13, m_yPos - 13, m_xPos + 13, m_yPos + 13 ) ;
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
      double cosAngle = cos( angle );
      double sinAngle = sin( angle );
      double x = vX * cosAngle - vY * sinAngle ;
      double y = vY * cosAngle + vX * sinAngle ;
      vX = x ;
      vY = y ;
    }

    p[0].x = m_xPos ;
    p[0].y = m_yPos ;
    p[1].x = m_xPos + vX ;
    p[1].y = m_yPos + vY ;

    HANDLE oldPen = ::SelectObject( surfaceDC, m_pen ) ;

    ::Polyline( surfaceDC, p, 2 ) ;

    ::SelectObject( surfaceDC, oldPen ) ;
    return true ;
  }

  return false ;
}