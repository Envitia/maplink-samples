//	********************************************************
//	*           Source Control Information
//	********************************************************
//	$Archive: $
//	$Author$
//	$Date$
//	$Revision$
//	********************************************************

#include "stdafx.h"
#include "resource.h"
#include "NetworkDemoDoc.h"
#include "TSLViewModeRoute.h"

TSLViewModeRoute::TSLViewModeRoute(TSLViewModeInterface* iViewMode, unsigned long modeID, CNetworkDemoDoc* doc)
  : TSLViewMode	    ( iViewMode, modeID ),
    m_document	    ( doc ),
    m_currentState  ( TSLRoute_Default ),
    m_previousState ( TSLRoute_Default ),
    m_x		    ( 0 ),
    m_y		    ( 0 ),
    m_pressed	    ( false )
{
}

TSLViewModeRoute::~TSLViewModeRoute()
{
}

bool TSLViewModeRoute::OnLButtonDown(long x, long y, bool shift, bool control)
{
  m_interface->captureMouse() ;

  // Store the location of the mouse click
  m_x = x;
  m_y = y;

  m_pressed = true;

  TSLRouteState oldState = m_previousState;

  switch ( m_currentState )
  {
  case TSLRoute_AddingStartPin:
    m_previousState = m_currentState;
    m_currentState  = TSLRoute_StartPinAdded;

    if ( !addRoutePin() )
    {
      // If this fails, restore previous state.
      m_currentState  = m_previousState;
      m_previousState = oldState;
    }

    break;

  case TSLRoute_AddingEndPin:
    m_previousState = m_currentState;
    m_currentState  = TSLRoute_AbleToRoute;

    if ( !addRoutePin() )
    {
      m_currentState  = m_previousState;
      m_previousState = oldState;
    }

    break;

  case TSLRoute_AddingWayptPin:
    // If way-points are being added after an end point, then the previous state will be TSLRoute_AbleToRoute.
    // In this case, we remain in an 'able to route' state.
    // Otherwise, the current state is marked as 'having had a way-point added'.
    // The previous state is marked as 'in the process of adding a way-point'.
    m_currentState  = ( m_previousState != TSLRoute_AbleToRoute ) ? TSLRoute_WayptPinAdded : TSLRoute_AbleToRoute;
    m_previousState = TSLRoute_AddingWayptPin;

    if ( !addRoutePin() )
    {
      m_currentState  = m_previousState;
      m_previousState = oldState;
    }

    break;
  }

  return false ;	
}

bool TSLViewModeRoute::OnLButtonUp(long x, long y, bool shift, bool control)
{
  m_interface->releaseMouse() ;
  m_pressed = false ;

  return true ;
}

bool TSLViewModeRoute::OnRButtonUp(long x, long y, bool shift, bool control)
{
  // Reset the current view mode to edit
  switch ( m_currentState )
  {
  case TSLRoute_AddingStartPin:
  case TSLRoute_AddingEndPin:
  case TSLRoute_AddingWayptPin:
    m_currentState  = m_previousState;
    m_previousState = TSLRoute_Default;

    break;

  case TSLRoute_StartPinAdded:
  case TSLRoute_WayptPinAdded:
  case TSLRoute_AbleToRoute:
  case TSLRoute_Default:

    break;
  }

  return false ;
}

bool TSLViewModeRoute::OnMouseMove(long x, long y, bool shift, bool control)
{
  if ( !m_pressed )
    return false ;

  // Save the current point
  m_x = x;
  m_y = y;

  return false;
}

void TSLViewModeRoute::reset()
{
  m_currentState  = TSLRoute_Default;
  m_previousState = TSLRoute_Default;

  m_x = 0;
  m_y = 0;

  m_pressed = false;
}

void TSLViewModeRoute::addStartPin()
{
  if ( m_currentState != TSLRoute_AddingStartPin )
  {
    m_previousState = m_currentState;
    m_currentState  = TSLRoute_AddingStartPin;
  }
}

void TSLViewModeRoute::addEndPin()
{
  if ( m_currentState != TSLRoute_AddingEndPin )
  {
    m_previousState = m_currentState;
    m_currentState  = TSLRoute_AddingEndPin;
  }
}

void TSLViewModeRoute::addWayptPin()
{
  if ( m_currentState != TSLRoute_AddingWayptPin )
  {
    m_previousState = m_currentState;
    m_currentState  = TSLRoute_AddingWayptPin;
  }
}

bool TSLViewModeRoute::addRoutePin()
{
  TSLTMC tmcX = 0;
  TSLTMC tmcY = 0;

  // Convert pixels to TMC units.
  m_drawingSurface->DUToTMC( m_x, m_y, &tmcX, &tmcY );
  
  // The size of the pin in pixels.
  static const int PIN_SIZE = 70;

  // Inform document that a pin has been added.
  return m_document->addRoutePin( tmcX, tmcY, m_drawingSurface->TMCperMU(), PIN_SIZE, m_previousState==TSLRoute_AddingEndPin );
}

void TSLViewModeRoute::clearAllRoutes(bool keepFirstPoint)
{
  m_currentState  = keepFirstPoint ? TSLRoute_StartPinAdded : TSLRoute_Default;
  m_previousState = m_currentState;
}
