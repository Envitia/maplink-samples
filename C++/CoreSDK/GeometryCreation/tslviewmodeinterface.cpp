//	********************************************************
//	*           Source Control Information
//	********************************************************
//	$Archive: /MapLinkWizard/Template/tslviewmodeinterface.cpp $
//	$Author$
//	$Date$
//	$Revision$
//	********************************************************

#include "stdAfx.h"
#include "tslviewmodeinterface.h"

TSLViewModeInterface::TSLViewModeInterface( )
  : m_draw( false ), m_x1( 0 ), m_y1( 0 ),
    m_x2( 0 ), m_y2( 0 ), m_drawingSurface( 0 )
{
  
}

TSLViewModeInterface::~TSLViewModeInterface()
{

}

void TSLViewModeInterface::attach( TSLDrawingSurface * ds )
{
  m_drawingSurface = ds ;
}

bool TSLViewModeInterface::attached( TSLDrawingSurface * ds )
{
  // If asked to check against specific surface then do so
  if ( ds )
    return (m_drawingSurface == ds ) ;

  // Otherwise only bothered if attached to any
  return m_drawingSurface ? true : false ;
}

void TSLViewModeInterface::clearEchoRectangle()
{
  // Erase any existing rectangle
  drawEchoRectangle( ) ;

  // Clear the internal flags and corners.
  m_x1 = m_y1 = m_x2 = m_y2 = 0 ;
  m_draw = false ;
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
  m_draw = true ;

  // Draw the new rectangle
  drawEchoRectangle( ) ;
}
