//## begin module%1.10%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.10%.codegen_version

//## begin module%419A241402F5.cm preserve=no
//	********************************************************
//	*           Source Control Information
//	********************************************************
//	$Archive: $
//	$Author: $
//	$Date: $
//	$Revision: $
//	********************************************************
//## end module%419A241402F5.cm

//## begin module%419A241402F5.cp preserve=no
//  ********************************************************
//  *  Copyright (c) 1998 to 2011 by Envitia Group PLC
//  ********************************************************
//  MODULE      : interactionmodes\InteractionModeTracks.cpp
//  SUBSYSTEM   : Interaction Modes
//  ********************************************************
//## end module%419A241402F5.cp

//## Module: InteractionModeTracks%419A241402F5; Pseudo Package body
//## Source file: E:\NightlyBuilds\SDK\CoreSDK\interactionmodes\InteractionModeTracks.cpp

//## begin module%419A241402F5.additionalIncludes preserve=no
#ifdef WINNT
# define WIN32_LEAN_AND_MEAN
# define VC_EXTRALEAN
# include "windows.h"
#endif

#include "MapLink.h"
#include "MapLinkIMode.h"
//## end module%419A241402F5.additionalIncludes

//## begin module%419A241402F5.includes preserve=yes
#include "InteractionModeTracks.h"
#include "application.h"
#include <new>
//## end module%419A241402F5.includes

//## begin module%419A241402F5.additionalDeclarations preserve=yes
//## end module%419A241402F5.additionalDeclarations


// Class InteractionModeTracks 

InteractionModeTracks::InteractionModeTracks(Application* parent, int modeID, const char *prompt, bool middleButtonPansToPoint)
//## begin InteractionModeTracks::InteractionModeTracks%E4F0DF0CFEED.hasinit preserve=no
  : m_parent(parent)
  , m_prompt(0)
  //## end InteractionModeTracks::InteractionModeTracks%E4F0DF0CFEED.hasinit
  //## begin InteractionModeTracks::InteractionModeTracks%E4F0DF0CFEED.initialization preserve=yes
  , TSLInteractionMode(modeID, middleButtonPansToPoint)
  //## end InteractionModeTracks::InteractionModeTracks%E4F0DF0CFEED.initialization
{
  //## begin InteractionModeTracks::InteractionModeTracks%E4F0DF0CFEED.body preserve=yes
  if (prompt)
  {
    m_prompt = new TSL_NO_THROW_NEW char[strlen(prompt) + 1];
    if (m_prompt)
      strcpy(m_prompt, prompt);
  }
  //## end InteractionModeTracks::InteractionModeTracks%E4F0DF0CFEED.body
}


InteractionModeTracks::~InteractionModeTracks()
{
  //## begin InteractionModeTracks::~InteractionModeTracks%4473BFC3FEED.body preserve=yes
  if (m_prompt)
  {
    delete[] m_prompt;
    m_prompt = 0;
  }
  //## end InteractionModeTracks::~InteractionModeTracks%4473BFC3FEED.body
}



//## Other Operations (implementation)
void InteractionModeTracks::activate()
{
  //## begin InteractionModeTracks::activate%419B6476036B.body preserve=yes
  //## end InteractionModeTracks::activate%419B6476036B.body
}

void InteractionModeTracks::deactivate()
{
  //## begin InteractionModeTracks::deactivate%419B6476036C.body preserve=yes
  //## end InteractionModeTracks::deactivate%419B6476036C.body
}

bool InteractionModeTracks::onLButtonUp(TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control)
{
  //## begin InteractionModeTracks::onLButtonDown%42869A75FEED.body preserve=yes
  //! handles clicking left click in the tracks mode.
  m_parent->onTracksModeLeftClick(x, y);

  return false;
  //## end InteractionModeTracks::onLButtonDown%42869A75FEED.body
}

bool InteractionModeTracks::onRButtonUp(TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control)
{
  //## begin InteractionModeTracks::onRButtonUp%938C25CAFEED.body preserve=yes

  // Reset the current interaction mode to edit
  m_display->resetMode(TSLButtonRight, x, y);
  return false;

  //## end InteractionModeTracks::onRButtonUp%938C25CAFEED.body
}

TSLCursorStyle InteractionModeTracks::queryCursor()
{
  //## begin InteractionModeTracks::queryCursor%419D1D4D013D.body preserve=yes
  return TSLCursorStylePanToPoint;
  //## end InteractionModeTracks::queryCursor%419D1D4D013D.body
}

const char* InteractionModeTracks::queryPrompt()
{
  //## begin InteractionModeTracks::queryPrompt%588666BEFEED.body preserve=yes
  return m_prompt ? m_prompt : "";
  //## end InteractionModeTracks::queryPrompt%588666BEFEED.body
}

// Additional Declarations
  //## begin InteractionModeTracks%419A241402F5.declarations preserve=yes
void * InteractionModeTracks::operator new (size_t size) TSL_NO_THROW
{
  return new TSL_NO_THROW_NEW unsigned char[size];
}

void InteractionModeTracks::operator delete (void *self)
{
  unsigned char *mem = (unsigned char*)self;
  delete[] mem;
}

#ifdef WIN32
void * InteractionModeTracks::operator new (size_t size, char * filename, int line)
{
  return new TSL_NO_THROW_NEW unsigned char[size];
}

#ifdef _MSC_VER
#if _MSC_VER >= 1200
void InteractionModeTracks::operator delete (void * pMem, char * filename, int line)
{
  unsigned char *mem = (unsigned char*)pMem;
  delete[] mem;
}
#endif
#endif
#endif
//## end InteractionModeTracks%419A241402F5.declarations

//## begin module%419A241402F5.epilog preserve=yes
//## end module%419A241402F5.epilog
