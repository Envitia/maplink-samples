// DORangeRing.cpp: implementation of the CDORangeRing class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DORangeRing.h"
#include "RangeRing.h"
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

COLORREF  CDORangeRing::m_ringColour = 0x00FF9933 ;
HANDLE	  CDORangeRing::m_RRfont = NULL ;
int	  CDORangeRing::m_RRReference = 0 ;
HPEN  CDORangeRing::m_penDot = 0;
HPEN  CDORangeRing::m_penSolid = 0;



CDORangeRing::CDORangeRing()
  : CDOTrack()
{
  m_lastOwnShipX = -1 ;
  m_lastOwnShipY = -1 ;
  if ( ++m_RRReference == 1 )
  {
    m_RRfont = CreateFont( 15, 0, 0, 0, FW_BOLD, 0,0,0, 0,0,0,0,0,_T("Helvetica") ) ;
    m_penDot = ::CreatePen( PS_SOLID, 0, m_ringColour ) ;
    m_penSolid = ::CreatePen( PS_DOT, 0, m_ringColour ) ;
  }
}

CDORangeRing::~CDORangeRing() 
{
  if ( --m_RRReference == 0 )
  {
    DeleteObject( m_RRfont ) ;
    m_RRfont = 0 ;
    ::DeleteObject( m_penDot ) ;
    ::DeleteObject( m_penSolid ) ;
    m_penDot = 0;
    m_penSolid = 0;
  }
}

bool CDORangeRing::getMinMaxDivisor( int &minorDiv, int &majorDiv, double size )
{
  struct RangeRingDivisor
  {
    int range ; // Radius in km
    int minor ; // Minor ring in 10ths of a km
    int major ; // Major ring in 10ths of a km
  } ;

  static RangeRingDivisor divisors[] = 
  {
    { 1, 1,5},
    { 2, 2,10},
    { 5, 10,20},
    { 10, 10,50},
    { 15, 10,50},
    { 20, 20,100},
    { 25, 50,200},
    { 50, 100,200},
    { 100, 100,500},
    { 150, 100,500},
    { 200, 200,1000},
    { 250, 500,1000},
    { 300, 500,1000},
    { 400, 500,2000},
    { 500, 1000,2000},
    { 1000, 1250,2500},
    { 1500, 1000,5000},
    { 2000, 2000,10000},
    { 5000, 5000,20000},
    { 10000, 10000,50000}
  } ;

  double radius = size / 2.0 ;
  int numRanges = sizeof( divisors ) / sizeof( RangeRingDivisor ) ;

  // Find range that is lower than selected

  int i;
  for ( i = 0 ; i < numRanges ; i++ )
  {
    if ( divisors[ i ].range > radius )
      break ;
  }
  if ( radius < (double)divisors[i].minor / 10.0 )
    // Too small
    return false;

  if ( i == numRanges )
    i -= 1 ;			// Get the next lower

  minorDiv = divisors[ i ].minor ;
  majorDiv = divisors[ i ].major ;
  return true;
}

void CDORangeRing::recalculateRings( TSLRenderingInterface *d_surface, int minorDiv, int majorDiv )
{
  CTrack	* ownShip = ((CRangeRing*)m_ddo)->ownShipTrack();

  double tmcPerMetre = CTrackManager::tmcPerMetre() ;
  double minDistance = DBL_MAX;
  double maxDistance = 0.0 ;

  TSLCoord c = m_drawnExtent.centre() ;
  long cx = c.x() ;
  long cy = c.y() ;

  updateDistances( m_drawnExtent.yMin() - ownShip->position().y(), m_drawnExtent.xMin() - ownShip->position().x(), minDistance, maxDistance ) ;
  updateDistances( m_drawnExtent.yMin() - ownShip->position().y(), m_drawnExtent.xMax() - ownShip->position().x(), minDistance, maxDistance ) ;
  updateDistances( m_drawnExtent.yMax() - ownShip->position().y(), m_drawnExtent.xMin() - ownShip->position().x(), minDistance, maxDistance ) ;
  updateDistances( m_drawnExtent.yMax() - ownShip->position().y(), m_drawnExtent.xMax() - ownShip->position().x(), minDistance, maxDistance ) ;

  bool insideHorizontal = false ;
  bool insideVertical   = false ;

  // If need to calculate perpendicular - for example, ownship near one
  // side of SDA, to get minimum, then calculate it.
  // So check to see if ownship is within the horizontal/vertical extents
  if ( ownShip->position().x() > m_drawnExtent.xMin() && ownShip->position().x() < m_drawnExtent.xMax() )
  {
    updateDistances( m_drawnExtent.yMax() - ownShip->position().y(), 0, minDistance, maxDistance );
    updateDistances( m_drawnExtent.yMin() - ownShip->position().y(), 0, minDistance, maxDistance );
    insideHorizontal = true ;
  }

  if ( ownShip->position().y() > m_drawnExtent.yMin() && ownShip->position().y() < m_drawnExtent.yMax() )
  {
    updateDistances( 0, m_drawnExtent.xMin() - ownShip->position().x(), minDistance, maxDistance );
    updateDistances( 0, m_drawnExtent.xMax() - ownShip->position().x(), minDistance, maxDistance );
    insideVertical = true ;
  }

  updateDistances( cx - ownShip->position().x(), cy - ownShip->position().y(), minDistance, maxDistance );

  // Here, min and maxDistance are in TMC and squared 
  // Convert to 10ths of km to match divisor settings
  minDistance = sqrt( minDistance ) / (100.0 * tmcPerMetre) ;
  maxDistance = sqrt( maxDistance ) / (100.0 * tmcPerMetre) ;

  // Round to get start and check to stop zero ring appearing
  minDistance = (int)(minDistance / minorDiv) * minorDiv ;

  // Ensure that we never go smaller than the minorDiv and that we always
  // display rings close to ownship when ownship is on display.
  if ( minDistance < minorDiv || (insideHorizontal && insideVertical) )
    minDistance = minorDiv ;

  // Get ownship position in screen space
  double  ocx = ownShip->position().x();
  double  ocy = ownShip->position().y();
  d_surface->TMCToDuAccurate( ocx, ocy ) ;

  // Get centre of extent in screen space
  double scx = cx, scy = cy;
  d_surface->TMCToDuAccurate( scx, scy ) ;
  scx -= ocx ;
  scy = ocy - scy;

  // Calculate the correct angle for the text - line from ownship to centre
  // of zoomed extent
  double angle ;
  double sinAngle ;
  double cosAngle ;

  angle = (scy ? atan2( (double)scx, (double)scy ) : (-135.0 * PI / 180.0) ) ;
  sinAngle = sin( angle ) ;
  cosAngle = cos( angle ) ;

  angle *= 360.0 / (2.0 * PI) ;

  bool useStraightLines = false ;

  // If arcs would fit inside coordinate space, use them
  // otherwise, approximate using straight lines
#define RING_AS_ARC_MAX   3000
#define RING_AS_ARC_MIN (-3000)

  if (   ocx > RING_AS_ARC_MAX
    || ocx < RING_AS_ARC_MIN
    || ocy > RING_AS_ARC_MAX
    || ocy < RING_AS_ARC_MIN )
  {
    useStraightLines = true ;
  }

  m_minorRings.clear( ) ;
  m_majorRings.clear( ) ;

  int m_gap = 15 ;

  int count = 0;

  for (int range = (int)minDistance ; range <= maxDistance ; range += minorDiv)
  {
    int diff ;

    count++;

    // Range is in 10ths of km, convert to TMC
    double dx = (double)range * 100.0 * tmcPerMetre + ownShip->position().x();
    double dy  = ownShip->position().y();

    d_surface->TMCToDuAccurate( dx, dy ) ;

    dx -= ocx ;
    dy -= ocy ;

    double arcSize = sqrt( dx * dx  + dy * dy ) ;

    long xoffset = arcSize *  sinAngle ;
    long yoffset = arcSize * -cosAngle ;

    vector< RRing > * primBuffer = NULL ;
    RRing ring ;

    if ( (range % majorDiv) == 0 )
    {
      if ( arcSize <= m_gap )
        diff = 90 * 64 ;
      else
        diff = (int)((((double)m_gap / arcSize)
        * (360 * 64)) / ( 2 * PI)) ;

      primBuffer = &m_majorRings ;
      char b[16];
      sprintf( b, "%g", (double)range / 10.0 ) ;
      ring.m_label = b;

      ring.m_tx = ocx + xoffset ;
      ring.m_ty = ocy + yoffset ;
    }
    else
    {
      diff = 0 ;
      primBuffer = &m_minorRings ;
    }

    if ( useStraightLines )
    { 
      ring.m_arc = false ;

      if ( primBuffer == &m_majorRings )
      {
        ring.m_x1 = ocx + xoffset - m_gap * cosAngle ;
        ring.m_y1 = ocy + yoffset - m_gap * sinAngle ;
        ring.m_x2 = ocx + xoffset - (1000 * 2 * cosAngle) ;
        ring.m_y2 = ocy + yoffset - (1000 * 2 * sinAngle) ;

        int xo = -(1000 * 2 + m_gap) * cosAngle ;
        int yo = -(1000 * 2 + m_gap) * sinAngle ;

        ring.m_x3 = ring.m_x1 - xo ;
        ring.m_y3 = ring.m_y1 - yo ;
        ring.m_x4 = ring.m_x2 - xo ;
        ring.m_y4 = ring.m_y2 - yo ;
      }
      else
      {
        int xo = -1000 * 2 * cosAngle ;
        int yo = -1000 * 2 * sinAngle ;

        ring.m_x1 = ocx + xoffset + xo ;
        ring.m_y1 = ocy + yoffset + yo ;
        ring.m_x2 = ocx + xoffset - xo ;
        ring.m_y2 = ocy + yoffset - yo ;
      }
    }
    else
    {
      ring.m_arc = true ;

      ring.m_x1 = ocx - arcSize ;
      ring.m_y1 = ocy - arcSize ;
      ring.m_x2 = ocx + arcSize ;
      ring.m_y2 = ocy + arcSize ;

      ring.m_x3 = ring.m_x4 = ocx + arcSize ; 
      ring.m_y3 = ring.m_y4 = ocy ;

      double a1 = ((angle * 64 + diff) * PI / (64 * 180) ) ;
      double a2 = a1 + ((360 * 64) - (2 * diff)) * PI / (64 * 180) ;

      ring.m_x4 = ocx + sin( a1 ) * arcSize;
      ring.m_y4 = ocy - cos( a1 ) * arcSize;

      ring.m_x3 = ocx + sin( a2 ) * arcSize;
      ring.m_y3 = ocy - cos( a2 ) * arcSize;
    }
    primBuffer->push_back( ring ) ;
  }
}

bool CDORangeRing::draw (TSLRenderingInterface *d_surface, TSLEnvelope *d_extent ) 
{
  TSLEnvelope e  = d_surface->extent() ;
  CRangeRing * track = (CRangeRing *)m_ddo ;

  if ( !track->visible() || !CTrackManager::displayRangeRings() )
    return true ;

  CTrack * ownShip = ((CRangeRing*)m_ddo)->ownShipTrack();
  if (ownShip == NULL)
    return false;

  // set up the maximum window extent
  if ( d_surface->rotationActive() )
  {
    TSLCoord  centre = d_surface->rotationCentre();
    if ( !TSLUtilityFunctions::rotateEnvelope( &e, d_surface->rotation(),
      centre.x(),
      centre.y() ) )
    {
      return true ;
    }
  }


  bool	recalc = false;
  if ( e != m_drawnExtent )
    recalc = true;
  else
  {
    // See if the own ship's position in DU has changed from last time
    TSLTMC  nowX, nowY;
    d_surface->TMCToDU( ownShip->position().x(), ownShip->position().y(), &nowX, &nowY );
    if ( m_lastOwnShipX != nowX || m_lastOwnShipY != nowY )
    {
      recalc = true;
      m_lastOwnShipX = nowX;
      m_lastOwnShipY = nowY;
    }
  }
  if ( recalc )
  {
    m_drawnExtent = e ;

    double tmcPerMetre = CTrackManager::tmcPerMetre() ;

    double widthKm  = ( (double)e.xMax() - (double)e.xMin() ) / ( tmcPerMetre * 1000.0 ) ;
    double heightKm = ( (double)e.yMax() - (double)e.yMin() ) / ( tmcPerMetre * 1000.0 ) ;

    int minDivisor, maxDivisor ;

    if ( !getMinMaxDivisor( minDivisor, maxDivisor, widthKm < heightKm ? heightKm : widthKm ) )
    {
      // Too small to draw arcs - they lock the GDI
      m_majorRings.clear();
      m_minorRings.clear();
    }
    else
    {
      recalculateRings( d_surface, minDivisor, maxDivisor ) ;
    }
  }

  TSLCoord c = track->position();


  TSLTMC wndX = c.x();
  TSLTMC wndY = c.y();
  TSLDeviceUnits	xo, yo;
  d_surface->TMCToDU( wndX, wndY, &xo, &yo );
  wndX = xo;
  wndY = yo;

  HDC snkDC = (HDC)(d_surface->handleToDrawable());

  int oldMode = ::SetBkMode( snkDC, TRANSPARENT ) ;

  HPEN   pen = NULL ;

  if ( m_minorRings.size() )
  {
    HANDLE oldPen = ::SelectObject( snkDC, m_penDot ) ;

    for ( size_t i = 0 ; i < m_minorRings.size() ; i++ )
    {
      RRing & r = m_minorRings[i] ;

      if ( r.m_arc )
      {
        ::Arc( snkDC, r.m_x1, r.m_y1, r.m_x2, r.m_y2, 
          r.m_x3, r.m_y3, r.m_x4, r.m_y4 ) ;
      }
      else
      {
        POINT p[2] ;
        p[0].x = r.m_x1 ;
        p[0].y = r.m_y1 ;
        p[1].x = r.m_x2 ;
        p[1].y = r.m_y2 ;
        ::Polyline( snkDC, p, 2 ) ;
      }
    }
    ::SelectObject( snkDC, oldPen ) ;

    pen = 0 ;
  }

  if ( m_majorRings.size() )
  {
    ::SetTextAlign( snkDC, TA_CENTER | TA_BASELINE ) ;
    ::SetTextColor( snkDC, m_ringColour ) ;

    HANDLE oldPen = ::SelectObject( snkDC, m_penSolid ) ;

    for ( size_t i = 0 ; i < m_majorRings.size() ; i++ )
    {
      RRing & r = m_majorRings[i] ;

      if ( r.m_arc )
      {
        ::Arc( snkDC, r.m_x1, r.m_y1, r.m_x2, r.m_y2, 
          r.m_x3, r.m_y3, r.m_x4, r.m_y4 ) ;
      }
      else
      {
        POINT p[2] ;
        p[0].x = r.m_x1 ;
        p[0].y = r.m_y1 ;
        p[1].x = r.m_x2 ;
        p[1].y = r.m_y2 ;
        ::Polyline( snkDC, p, 2 ) ;
        p[0].x = r.m_x3 ;
        p[0].y = r.m_y3 ;
        p[1].x = r.m_x4 ;
        p[1].y = r.m_y4 ;
        ::Polyline( snkDC, p, 2 ) ;
      }
      HANDLE oldFont = ::SelectObject(snkDC, m_RRfont);      
      ::TextOutA( snkDC, r.m_tx, r.m_ty + 3, r.m_label.c_str(), r.m_label.size() ) ;
      ::SelectObject(snkDC, oldFont);
    }

    ::SelectObject( snkDC, oldPen ) ;
    pen = 0 ;
  }

  ::SetBkMode( snkDC, oldMode ) ;

  return false;
}
