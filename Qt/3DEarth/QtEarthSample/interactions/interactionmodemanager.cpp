//  ********************************************************
//  *  Copyright (c) 1998 to 2021 by Envitia Group PLC
//  ********************************************************
//  ********************************************************


#ifdef WINNT
# define WIN32_LEAN_AND_MEAN
# define VC_EXTRALEAN
# include "windows.h"
#endif

#include "MapLink3DIMode.h"

#include <map>
#include <vector>
#include <list>

typedef std::map< int, Interaction *, std::less<Long> > ModeMap ;
typedef std::map< int, Interaction *, std::less<Long> >::iterator ModeMapIter ;

// Class InteractionModeManager 

InteractionModeManager::InteractionModeManager (InteractionModeRequest* requestHandler, envitia::maplink::earth::Surface3D* drawingSurface,
	int mouseWheelZoomPercentage, bool lockCursorOnMouseWheel )
  : m_modes( 0 ), m_currentMode( 0 ), m_defaultMode( 0 ),
    m_requestHandler( requestHandler ), m_drawingSurface(drawingSurface),
    m_mouseWheelPercentage( mouseWheelZoomPercentage ), m_lockCursorOnMouseWheel( lockCursorOnMouseWheel )
{
}


InteractionModeManager::~InteractionModeManager ()
{

  // Return if there are no modes to destroy
  if ( !m_modes )
    return ;

  // Clear down the current mode
  if ( m_currentMode )
  {
    m_currentMode->deactivate() ;
    m_currentMode = 0 ;
  }

  ModeMap * modes ;

  modes = (ModeMap *)m_modes ;

  for ( ModeMapIter match = modes->begin() ; match != modes->end() ; ++match )
  {
    delete match->second ;
    match->second = 0 ;
  }

  delete modes;

  m_defaultMode = 0 ;
}



bool InteractionModeManager::addMode (Interaction* mode, bool isDefault)
{

  // Initialise the mode list if necessary
  ModeMap * modes ;

  if ( !m_modes )
  {
    modes = new TSL_NO_THROW_NEW ModeMap ;
    if ( !modes )
    {
      //TSLErrorStack::addError( IMODE_MODE_MAP_CREATE, "", IMODE_MODE_MAP_CREATE_CATEGORY ) ;
      return false ;
    }
    m_modes = (void *)modes ;
  }
  else
  {
    modes = (ModeMap *)m_modes ;
  }

  // Now validate the mode, and check to ensure it's not already there
  if ( !mode )
  {
    //TSLErrorStack::addError( IMODE_MODE_IS_NULL, "", IMODE_MODE_IS_NULL_CATEGORY ) ;
    return false ;
  }

  int id = mode->id();
  ModeMapIter match = modes->find( id ) ;
  if ( match != modes->end() )
  {
    //TSLErrorStack::addError( IMODE_DUPLICATE_ID, "", IMODE_DUPLICATE_ID_CATEGORY ) ;
    return false ;
  }
  
  // Now we know it's valid, add it to the map
  modes->insert( (std::pair<const int, Interaction *>( (const int)id, mode )) ) ;

  mode->setDrawingSurface(m_drawingSurface);
  mode->setInteractionModeRequest(m_requestHandler);

  if ( isDefault )
    m_defaultMode = mode ;

  return true ;
}

int InteractionModeManager::getCurrentMode (Interaction** mode) const
{
  if ( mode )
    *mode = m_currentMode ;

  return m_currentMode ? m_currentMode->id() : 0 ;
}

int InteractionModeManager::getMode (int nth, Interaction** mode) const
{
  if ( !m_modes )
    return 0;

  ModeMap * modes ;

  modes = (ModeMap *)m_modes ;

  Int i = 0;
  for ( ModeMapIter match = modes->begin() ; match != modes->end() ; ++match )
  {
    if ( i == nth )
    {
      if ( mode )
        *mode = match->second;
      return match->second->id();
    }
    ++i;
  }
  return 0;
}

int InteractionModeManager::numModes () const
{
  if ( !m_modes )
    return 0;

  return ((ModeMap*)m_modes)->size();
}

bool InteractionModeManager::onLButtonDown (TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control)
{
  if ( !m_currentMode )
    return false ;
  return m_currentMode->onLButtonDown(x, y, shift, control);
}

bool InteractionModeManager::onLButtonUp (TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control)
{
  if ( !m_currentMode )
    return false ;
  return m_currentMode->onLButtonUp(x, y, shift, control) ;
}

bool InteractionModeManager::onMButtonDown (TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control)
{
  if ( !m_currentMode )
    return false ;
  return m_currentMode->onMButtonDown(x, y, shift, control);
}

bool InteractionModeManager::onMButtonUp (TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control)
{
  if ( !m_currentMode )
    return false ;
  return m_currentMode->onMButtonUp(x, y, shift, control);
}

bool InteractionModeManager::onMouseMove (TSLButtonType button, TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control)
{
  if ( !m_currentMode )
    return false ;
  return m_currentMode->onMouseMove(button, x, y, shift, control) ;
}

bool InteractionModeManager::onMouseWheel (short delta, TSLDeviceUnits x, TSLDeviceUnits y)
{
  if ( !m_currentMode )
    return false ;
  return m_currentMode->onMouseWheel( delta, x, y ) ;
}

bool InteractionModeManager::onRButtonUp (TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control)
{
  if ( !m_currentMode )
    return false ;
  return m_currentMode->onRButtonUp(x, y, shift, control);
}

bool InteractionModeManager::onRButtonDown (TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control)
{
  if ( !m_currentMode )
    return false ;
  return m_currentMode->onRButtonDown(x, y, shift, control);
}

void InteractionModeManager::onSize (TSLDeviceUnits cx, TSLDeviceUnits cy)
{
  if ( !m_currentMode )
    return ;
  //m_currentMode->onSize( cx, cy ) ;
}

TSLCursorStyle InteractionModeManager::queryCursor ()
{
	return TSLCursorStyleNone;
  //return m_currentMode ? m_currentMode->queryCursor() : TSLCursorStyleNone ;
}

const char * InteractionModeManager::queryPrompt ()
{
	return "";
  //return m_currentMode ? m_currentMode->queryPrompt() : "" ;
}

Interaction* InteractionModeManager::removeMode (int id)
{
  ModeMap * modes ;

  if ( !m_modes )
    return 0 ;

  // Check if we're already in this mode
  if ( m_currentMode &&  m_currentMode->id() == id )
  {
    m_currentMode->deactivate( ) ;
    m_currentMode = 0 ;
  }

  modes = (ModeMap *)m_modes ;

  ModeMapIter match = modes->find( id ) ;
  if ( match == modes->end() )
  {
    // If can't find this mode, then it's an error
    return 0 ;
  }
  
  Interaction * mode = match->second ;

  if ( mode == m_defaultMode )
    m_defaultMode = 0 ;

  modes->erase( match ) ;

  return mode ;
}

void InteractionModeManager::resetMode (TSLButtonType button, TSLDeviceUnits x, TSLDeviceUnits y)
{

  // Do something here, even if we're already in the default mode.
  // The Edit mode needs this in order to attach to the drawing surface neatly

  // Deactivate the current mode
  if ( m_currentMode )
  {
    m_currentMode->deactivate() ;
    m_currentMode = 0 ;
  }

  // And activate any default mode
  if ( m_defaultMode )
  {
    m_currentMode = m_defaultMode ;
    m_currentMode->activate( ) ;
  }

  if ( m_requestHandler )
    m_requestHandler->resetMode( m_currentMode, button, x, y ) ;
}

bool InteractionModeManager::setCurrentMode (int id, bool reactivate)
{
  ModeMap * modes ;

  if ( !m_modes )
    return false ;

  // Check if we're already in this mode
  if ( !reactivate && m_currentMode &&  m_currentMode->id() == id )
    return true ;

  modes = (ModeMap *)m_modes ;

  ModeMapIter match = modes->find( id ) ;
  if ( match == modes->end() )
  {
    // If can't find this mode, then deactivate current mode and go back to the default
    resetMode( TSLButtonNone, 0, 0 ) ;
    return false ;
  }
  
  Interaction * mode = match->second ;

  // Deactivate any current ode
  if ( m_currentMode )
  {
    m_currentMode->deactivate() ;
    m_currentMode = 0 ;
  }

  m_currentMode = mode ;
  m_currentMode->activate() ;

  return true ;
}

bool InteractionModeManager::setDefaultMode (int id)
{
  ModeMap * modes ;

  if ( !m_modes )
    return false ;

  // Check if we're already in this mode
  if ( m_defaultMode &&  m_defaultMode->id() == id )
    return true ;

  modes = (ModeMap *)m_modes ;

  ModeMapIter match = modes->find( id ) ;
  if ( match == modes->end() )
  {
    // If can't find this mode, then it's an error
    return false ;
  }

  m_defaultMode = match->second ;

  return true ;
}
