
#include "stdafx.h"
#include "TSLViewModeInterface.h"

TSLViewModeInterface::TSLViewModeInterface( TSLDrawingSurface * ds )
  : m_drawingSurface( ds ), m_drawRect( false ), m_drawLine( false ), m_x1( 0 ), m_y1( 0 ),
    m_x2( 0 ), m_y2( 0 )
{
  
}

TSLViewModeInterface::~TSLViewModeInterface()
{

}

void TSLViewModeInterface::clearEchoRectangle()
{
  // Erase any existing rectangle
  drawEchoRectangle( ) ;

  // Clear the internal flags and corners.
  m_x1 = m_y1 = m_x2 = m_y2 = 0 ;
  m_drawRect = false ;
}

void TSLViewModeInterface::setEchoRectangle( long x1, long y1, long x2, long y2 )
{
  // First clear any existing rectangle
  drawEchoRectangle( ) ;

  // Now set up the new corners, sorted accordingly.
  m_x1   = x1 < x2 ? x1 : x2 ;
  m_y1   = y1 < y2 ? y1 : y2 ;
  m_x2   = x1 < x2 ? x2 : x1 ;
  m_y2   = y1 < y2 ? y2 : y1 ;
  m_drawRect = true ;

  // Draw the new rectangle
  drawEchoRectangle( ) ;
}

void TSLViewModeInterface::clearEchoLine()
{
  // Erase any existing rectangle
  drawEchoLine( ) ;

  // Clear the internal flags and corners.
  m_x1 = m_y1 = m_x2 = m_y2 = 0 ;
  m_drawLine = false ;
}

void TSLViewModeInterface::setEchoLine( long x1, long y1, long x2, long y2 )
{
  // First clear any existing rectangle
  drawEchoLine( ) ;

  // Now set up the new corners, sorted accordingly.
  m_x1   = x1;
  m_y1   = y1;
  m_x2   = x2;
  m_y2   = y2;
  m_drawLine = true ;

  // Draw the new rectangle
  drawEchoLine( ) ;
}

void TSLViewModeInterface::resetMode()
{
  if ( m_resetCB )
  {
    m_resetCB( m_resetArg ) ;
  }
}

void TSLViewModeInterface::setResetCB( TSLViewModeResetCB cb, void *arg )
{
  m_resetCB = cb ;
  m_resetArg = arg ;
}
