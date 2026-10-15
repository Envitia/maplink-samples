#include "stdafx.h"
#include "resource.h"
#include "NetworkDemoDoc.h"
#include "TSLViewModeSpatialQuery.h"
#include "Utility.h"

TSLViewModeSpatialQuery::TSLViewModeSpatialQuery(TSLViewModeInterface * iViewMode, unsigned long modeID , CNetworkDemoDoc* doc)
  : TSLViewMode	    ( iViewMode, modeID ),
    m_document	    ( doc ),
    m_x		          ( 0 ),
    m_y		          ( 0 ),
    m_searchPtTMCx ( 0 ),
    m_searchPtTMCy ( 0 ),
    m_pressed	    ( false ),
    m_useSpatialIndex( false )
{
}

TSLViewModeSpatialQuery::~TSLViewModeSpatialQuery(void)
{
}

bool TSLViewModeSpatialQuery::OnLButtonDown(long x, long y, bool shift, bool control)
{
  // Check state
  if ( !m_pressed )
  {
    // Ensure clear
    m_document->clearSpatialQueryData();

    // Initiate Query, i.e. this action defines the Search Point
    m_interface->captureMouse() ;

    // Store the location of the mouse click
    m_x = x;
    m_y = y;

    // Convert pixels to TMC units.
    m_drawingSurface->DUToTMC( m_x, m_y, &m_searchPtTMCx, &m_searchPtTMCy );

    // The size of the pin in pixels.
    static const int PIN_SIZE = 70;

    m_document->addSpatialQuerySearchPointPin( m_searchPtTMCx, 
                                               m_searchPtTMCy, 
                                               m_drawingSurface->TMCperMU(), 
                                               PIN_SIZE );

    m_pressed = true;
  }
  else
  {
    // This should trigger the dialog that checks the data
    m_interface->releaseMouse() ;
    m_pressed = false ;

    // Clear the echo rectangle
    m_interface->clearEchoRectangle( ) ;

    // Convert pixels to TMC units.
    TSLTMC endTMCx;
    TSLTMC endTMCy;
    m_drawingSurface->DUToTMC( x, y, &endTMCx, &endTMCy );

    // Calculate the absolute difference in X & Y
    TSLTMC diffX = abs(abs(m_searchPtTMCx) - abs(endTMCx));
    TSLTMC diffY = abs(abs(m_searchPtTMCy) - abs(endTMCy));

    // Search range is the larger of the two
    TSLTMC searchRange = diffX > diffY ? diffX : diffY;


    TSLCoord endPt( endTMCx, endTMCy );

    // Create an extent using the search point as the centre and the range as the aperture 
    TSLEnvelope searchExtent;
    searchExtent.expand( searchRange );
    searchExtent.translate( m_searchPtTMCx, m_searchPtTMCy );

    // Display Data
    m_document->displaySpatialQueryOrientationRange( searchExtent, endPt,
                                                     m_drawingSurface->TMCperMU() );
    // Calculate Orientation
    TSLCoord searchPt( m_searchPtTMCx, m_searchPtTMCy);
    double orientation = Utility::baseLineAngle( searchPt, endPt );

    // Find Links
    std::string displayResult;
    m_document->performSpatialQuery( searchExtent, searchRange, orientation, m_useSpatialIndex, displayResult );

    // Force a redraw and display the string result
    m_drawingSurface->redraw();
    AfxMessageBox( (LPCTSTR)TSLUTF8Decoder(displayResult), MB_OK | MB_ICONINFORMATION ) ;

  }

  return true ;	
}

bool TSLViewModeSpatialQuery::OnLButtonUp(long x, long y, bool shift, bool control)
{
  if ( m_pressed )
  {
    m_interface->clearLine();
  }

  return true ;
}

bool TSLViewModeSpatialQuery::OnRButtonDown(long x, long y, bool shift, bool control)
{
  m_interface->releaseMouse() ;
  m_pressed = false ;

  m_document->clearSpatialQueryData();

  return true ;
}

bool TSLViewModeSpatialQuery::OnMouseMove(long x, long y, bool shift, bool control)
{
  if ( !m_pressed )
    return false ;

  // Calculate the absolute difference in X & Y
  long diffX = abs(abs(m_x) - abs(x));
  long diffY = abs(abs(m_y) - abs(y));

  // Search range is the larger of the two
  long searchRange = diffX > diffY ? diffX : diffY;

  // Create an extent using the seach point as the centre and the range as the aperture 
  TSLEnvelope searchExtent;
  searchExtent.expand( searchRange );
  searchExtent.translate( m_x, m_y );

  if ( abs( searchRange ) < 3 )
  {
    // If we haven't moved more than 3 pixels, then assume its just mouse wobble.
    m_interface->clearEchoRectangle( ) ;
  }
  else
  {
    // We have moved more than 3 pixels from the start position.
    // Set the position of the echo rectangle. Automatically causes it to be drawn
    m_interface->setEchoRectangle( searchExtent.bottomLeft().m_x,
                                   searchExtent.bottomLeft().m_y,
                                   searchExtent.topRight().m_x,
                                   searchExtent.topRight().m_y );
  }

  // Draw Orientation line
  m_interface->clearLine();
  m_interface->setLine( m_x, m_y, x, y );

  return false;
}

void TSLViewModeSpatialQuery::useSpatialIndex( bool state )
{
  m_useSpatialIndex = state;
}
