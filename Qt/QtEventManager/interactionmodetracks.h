// *********************************************************************************
// * Copyright (c) 1998 to 2017 by Envitia Group PLC.
// *********************************************************************************

#ifndef tslinteractionmodetracks_h
#define tslinteractionmodetracks_h 1

#include "tslinteractionmode.h"

class Application;
//!
//! This is a simple interaction mode for handling tracks to point.
//!
//! The user's application creates an instance of one these classes and adds it
//! to a TSLInteractionModeManager.
//!
//! When the LButtonUp event occurs,  the mode causes the current view of the
//! TSLDrawingSurface to tracks to the specified point.
//!
//! When an RButtonUp event occurs, the mode resets to the default mode.
//!
//! @ingroup group_coresdk_interactionmodes apigroup_interaction_modes
//! \{
class InteractionModeTracks : public TSLInteractionMode
{
  //! \}
public:
  //!
  //! Constructor for tracks interaction mode class.
  //!
  //! @param modeID This ID is not used by the mode itself, but is used as a unique
  //! identified by the mode manager.
  //!
  //! @param prompt  Defaulted to interaction description.  Prompt to return from queryPrompt
  //! method.
  //!
  //! @param middleButtontrackssToPoint  Defaults to true.  This flag allows modes to
  //! easily provide standard behaviour for pressing of the middle button - typically the
  //! mouse wheel.
  InteractionModeTracks(Application* parent, int modeID, const char *prompt = "Left button click trackss to point, Right button click to finish", bool middleButtontrackssToPoint = true);

  //!
  //! See this class description and base class method description for further
  //! details
  virtual ~InteractionModeTracks();

  //!
  //! See this class description and base class method description for further
  //! details
  void activate();

  //!
  //! See this class description and base class method description for further
  //! details
  void deactivate();

  //!
  //! See this class description and base class method description for further
  //! details
  virtual bool onLButtonUp(TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control);

  //!
  //! See this class description and base class method description for further
  //! details
  virtual bool onRButtonUp(TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control);

  //!
  //! See this class description and base class method description for further
  //! details
  TSLCursorStyle queryCursor();

  //!
  //! See this class description and base class method description for further
  //! details
  virtual const char* queryPrompt();

  void * operator new (size_t size) TSL_NO_THROW;
  void operator delete (void *self);
#ifdef WIN32
  //!
  //! When included in an MFC application in debug mode, the debug new expects this
  //! to be here. Override it and return the same as the normal one.
  //! The library must include it when compiled in release mode, since the user's
  //! application may be in debug mode.
  void * operator new (size_t size, char * filename, int line);
#ifdef _MSC_VER
#if _MSC_VER >= 1200
  void operator delete(void * pMem, char * filename, int line);
#endif
#endif
#endif

private:
  char *m_prompt;
  Application* m_parent;
};

#endif
