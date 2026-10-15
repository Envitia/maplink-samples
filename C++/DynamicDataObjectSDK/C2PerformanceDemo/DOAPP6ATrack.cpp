/****************************************************************************
    Copyright (c) 2000-2017 by Envitia Group PLC.
=============================================================================
MODULE          : 
PACKAGE         : MapLink
SCCS            : 
AUTHOR(s)       : D.Dixon
=============================================================================
DESCRIPTION     : 
****************************************************************************/
#include "stdafx.h"
#include "doapp6atrack.h"
#include "app6atrack.h"
#include "trackmanager.h"


// This defines the background colour to use for the transparent blit of the APP6A symbol bitmap.
static const int Bkg_R = 1;
static const int Bkg_G = 2;
static const int Bkg_B = 3;

static const COLORREF BkgCol = RGB( Bkg_R, Bkg_G, Bkg_B );


// This defines the size of the APP6A symbol bitmap.
// The size will depend on the type of annotations that will be attached.
// In this case, we have arrived at these figures by trial-and-error.
static const int SYMBOL_DX  = 130;
static const int SYMBOL_DY  =  60;
static const int SYMBOL_DX2 = SYMBOL_DX / 2;
static const int SYMBOL_DY2 = SYMBOL_DY / 2;


static void updateEntityExtent(TSLNTSurface& surface, TSLEntitySet* eSet)
{
  Int size = eSet->size();

  for ( int i( 0 ); i < size; ++i )
  {
    TSLEntity* e = eSet->operator[](i);
    TSLEntitySet* grp = TSLEntitySet::isEntitySet( e );
    if ( grp )
    {
      updateEntityExtent( surface, grp );
    }
    else
    {
      surface.updateEntityExtent(e, NULL);
    }
  }
}

DOAPP6ATrack::DOAPP6ATrack() : CDOTrack()
{
  m_py = m_px = 0;
  m_firstDraw = false;

  m_bitmap = 0;
  m_memDC = 0;
}

DOAPP6ATrack::~DOAPP6ATrack()
{
  clearBitmap();
}

void DOAPP6ATrack::clearBitmap()
{
  if ( m_bitmap )
  {
    ::SelectObject( m_memDC, m_bitmapOld );
    ::DeleteObject( m_bitmap );
    m_bitmap = 0;
  }

  if ( m_memDC )
  {
    ::DeleteDC( m_memDC );
    m_memDC = 0;
    m_bitmapOld = 0;
  }
}

bool DOAPP6ATrack::draw ( TSLRenderingInterface *d_surface, TSLEnvelope *d_extent )
{
  // Retrieve a pointer to the associated track
  APP6ATrack * pTrack = (APP6ATrack *)m_ddo ;

  if ( !CTrackManager::displayTrackType( pTrack->hostility() ) )
  {
    return false ;
  }
  
  // Get the current position of the track in terms of pixels
  TSLCoord coord = pTrack->position() ;
  TSLDeviceUnits  xCo, yCo ;
  TSLTMC x = coord.x();
  TSLTMC y = coord.y();

  d_surface->TMCToDU( x, y, &xCo, &yCo ) ;
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

  TSLEntitySet* es = pTrack->getAsEntitySet();

  // If e and es are different, it means that something about the APP6A details has changed.
  // If we are rendering based on bitmaps, then we will need to generate a new bitmap.

  if ( !es )
    return false;

  const long dx = x - m_px;
  const long dy = y - m_py;

  es->translate( dx, dy );

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

  // Now select the old font
  if ( oldFont )
    ::SelectObject( surfaceDC, oldFont ) ;

  if ( CTrackManager::bitmapRendering() )
  {
    TSLEntity* e = pTrack->getEntity();
    // If a change has been made to the APP6A details, then we need to generate a new bitmap.
    bool regenerateBitmap = ( e != es );

    if ( !drawBitmap( surfaceDC, pTrack, xCo, yCo, regenerateBitmap ) )
    {
      return false;
    }
  }
  else
  {
    // If we switch rendering mode during a draw cycle, this call makes sure that the correct bitmap
    // is created next time around.
    clearBitmap();

    // Draw the symbol based on entities.
    d_surface->drawEntity( es );
  }

  m_py = y;
  m_px = x;

  return true;
}

bool DOAPP6ATrack::drawBitmap(HDC hDC, APP6ATrack* track, long x, long y, bool regenerateBitmap)
{
  if ( regenerateBitmap )
  {
    clearBitmap();
  }

  if ( !m_bitmap )
  {
    if ( !getSymbolAsBitmap( hDC, track ) )
    {
      return false;
    }
  }

  ::TransparentBlt( hDC, x-SYMBOL_DX2, y-SYMBOL_DY2, SYMBOL_DX, SYMBOL_DY, m_memDC, 0, 0, SYMBOL_DX, SYMBOL_DY, BkgCol );

  return true;
}

bool DOAPP6ATrack::getSymbolAsBitmap(HDC hDC, APP6ATrack* track)
{
  m_memDC = ::CreateCompatibleDC( hDC );
  if ( !m_memDC )
  {
    return false;
  }

  m_bitmap = ::CreateCompatibleBitmap( hDC, SYMBOL_DX, SYMBOL_DY );
  if ( !m_bitmap )
  {
    clearBitmap();
    return false;
  }

  // Set the background colour such that the background will become transparent.
  RECT rect;
  rect.left   = 0;
  rect.right  = SYMBOL_DX;
  rect.top    = 0;
  rect.bottom = SYMBOL_DY;
  HBRUSH brush = ::CreateSolidBrush( BkgCol );
  HBITMAP bmp = static_cast<HBITMAP>( ::SelectObject( m_memDC, m_bitmap ) );
  ::FillRect( m_memDC, &rect, brush );
  ::DeleteObject( brush );
  ::SelectObject( m_memDC, bmp );

  bool success = track->getAsSymbol( m_bitmap );
  if ( success )
  {
    m_bitmapOld = static_cast<HBITMAP>( ::SelectObject( m_memDC, m_bitmap ) );
  }
  else
  {
    clearBitmap();
  }

  return success;
}

// Draw the track's ID number
void DOAPP6ATrack::drawTrackNumber(  HDC surfaceDC, CTrack * track )
{
  char const* label = track->trackLabel();

  if ( label )
  {
    ::SetTextColor( surfaceDC, RGB( 255, 255, 255 ) ) ;
    ::TextOutA( surfaceDC, m_xPos, m_yPos + 11, label, track->trackLabelLen()) ;
  }
}

// Draw a bounding rectangle around the track to indicate the track is selected
void DOAPP6ATrack::showAsSelectedTrack(  HDC surfaceDC )
{
  ::SelectObject( surfaceDC, GetStockObject( NULL_BRUSH ) ) ;
  ::SelectObject( surfaceDC, GetStockObject( WHITE_PEN ) ) ;
  ::Rectangle( surfaceDC, m_xPos - 22, m_yPos - 18, m_xPos + 24, m_yPos + 24 ) ;
}


void DOAPP6ATrack::drawOutline( HDC surfaceDC )
{
}
