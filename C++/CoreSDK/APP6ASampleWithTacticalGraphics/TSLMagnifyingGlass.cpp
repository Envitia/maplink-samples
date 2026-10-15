//	********************************************************
//	*           Source Control Information
//	********************************************************
//	$Archive: /Development/Samples/NT/LandLinkSample/TSLMagnifyingGlass.cpp $
//	$Author$
//	$Date$
//	$Revision$
//	********************************************************

#include "stdafx.h"
#include "TSLMagnifyingGlass.h"
#include "TSLViewModeInterface.h"

// Various default values.
double TSLMagnifyingGlass::m_defaultMagFactor	  = 15.0;
double TSLMagnifyingGlass::m_defaultMagSize	  = 15.0;
double TSLMagnifyingGlass::m_defaultMagOffset	  =  1.8;

int TSLMagnifyingGlass::m_boundaryThickness = 2 ;
int TSLMagnifyingGlass::m_boundaryRed   = 219 ;
int TSLMagnifyingGlass::m_boundaryGreen = 112 ;
int TSLMagnifyingGlass::m_boundaryBlue  = 147  ;

bool TSLMagnifyingGlass::m_initialisedDefaults = false ;

TSLMagnifyingGlass::TSLMagnifyingGlass( TSLViewModeInterface* pInterface )
  : m_interface	      ( pInterface ),
    m_drawingSurface  ( pInterface->drawingSurface() ),
    m_leftX	      ( 0 ),
    m_topY	      ( 0 ),
    m_rightX	      ( 0 ),
    m_bottomY	      ( 0 ),
    m_leftOldX	      ( 0 ),
    m_topOldY	      ( 0 ),
    m_rightOldX	      ( 0 ),
    m_bottomOldY      ( 0 )
{
  if ( !m_initialisedDefaults )
  {
    TSLSimpleString section;
    TSLProfileHelper::getDefaultSection( section ) ;

    TSLProfileHelper::setDefaultSection( "magnifier" ) ;

    TSLProfileHelper::lookupProfile( "magFactor",  &m_defaultMagFactor, 15.0 ) ;
    TSLProfileHelper::lookupProfile( "magSize",    &m_defaultMagSize,   15.0 ) ;
    TSLProfileHelper::lookupProfile( "magOffset",  &m_defaultMagOffset, 1.8 ) ;

    TSLProfileHelper::lookupProfile( "boundaryThickness",  &m_boundaryThickness, 2 ) ;
    TSLProfileHelper::lookupProfile( "boundaryRed",        &m_boundaryRed, 219 ) ;
    TSLProfileHelper::lookupProfile( "boundaryGreen",      &m_boundaryGreen, 112 ) ;
    TSLProfileHelper::lookupProfile( "boundaryBlue",       &m_boundaryBlue, 147 ) ;

    TSLProfileHelper::setDefaultSection( section.c_str() ) ;
  }

  m_magFactor = m_defaultMagFactor ;
  m_magSize   = m_defaultMagSize ;
  m_magOffset = m_defaultMagOffset ;

  m_drawingSurface->clearBackgroundColour();
    
  m_interface->bufferingOn();
}

TSLMagnifyingGlass::~TSLMagnifyingGlass()
{
  m_drawingSurface->removeDataLayer( "map" ) ;
  delete m_drawingSurface;
  
  delete m_interface;
}

void TSLMagnifyingGlass::onSize( long cx, long cy )
{
  // Pass on an ON_SIZE message to the view interface.
  m_interface->onSize( cx, cy );
}

bool TSLMagnifyingGlass::draw( HDC buffer, bool restoreBackground )
{
  // Use the associated view interface to draw into the supplied buffer.
  m_interface->drawMagnifierToBuffer( buffer, m_leftOldX, m_topOldY, m_leftX, m_topY, 
				      m_rightX-m_leftX, m_bottomY-m_topY, restoreBackground,
				      m_boundaryThickness, m_boundaryRed, m_boundaryGreen, m_boundaryBlue );

  return true;
}

bool TSLMagnifyingGlass::setWinExtent( int leftX, int topY, int rightX, int bottomY )
{
  // Resize the magnifier's extent.
  m_leftOldX   = m_leftX;
  m_topOldY    = m_topY;
  m_rightOldX  = m_rightX;
  m_bottomOldY = m_bottomY;
  
  m_leftX      = leftX;
  m_topY       = topY;
  m_rightX     = rightX;
  m_bottomY    = bottomY;

  long width   = rightX - leftX;
  long height  = bottomY - topY;

  // Check to see if buffers need updating.
  m_interface->setupBackgroundBuffer( width, height );
  
  m_drawingSurface->wndResize( 0, 0, width, height, false );
  
  return true;
}
    
bool TSLMagnifyingGlass::setMagExtent( double uux1, double uuy1, double uux2, double uuy2 )
{
  // Resize the magnifier's drawing surface.
  m_drawingSurface->resize( uux1, uuy1, uux2, uuy2, false );
  
  return true;
}

void TSLMagnifyingGlass::reset()
{
  // Resets the magnifier to its initial values.
  resetBuffers();

  m_magFactor = m_defaultMagFactor;
  m_magSize   = m_defaultMagSize;
  m_magOffset = m_defaultMagOffset;
}

void TSLMagnifyingGlass::resetBuffers()
{
  // Reinitialises all buffers.
  m_interface->bufferingOff();
  m_interface->bufferingOn();
}


