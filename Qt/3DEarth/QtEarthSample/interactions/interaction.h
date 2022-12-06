// *********************************************************************************
// * Copyright (c) 1998 to 2021 by Envitia Group PLC.
// *********************************************************************************

#ifndef INTERACTION_H
#define INTERACTION_H

#include "tslatomic.h"

class CEarthSampleView;
class CEarthSampleDoc;
class InteractionModeRequest;

//! An interaction between the user and the map view
//! Child implementations map actions such as mouse clicks
//! to geometry picking, editing, etc.
class Interaction {
public:
  virtual ~Interaction();

  virtual void activate() = 0;
  virtual void deactivate() = 0;

  //!
	//! Locator handler for interaction modes. 
	//!
	//! The default base implementation returns false.
	//!
	//! @param (x,y) Position of locator event, in device unit coordinates.
	//!
	//! @param shift Flag to indicate whether the shift key is pressed. An interaction
	//! mode may use this to modify its behaviour.
	//!
	//! @param control Flag to indicate whether the control key is pressed. An interaction
	//! mode may use this to modify its behaviour.
	//!
	//! @return true if the view needs to be redrawn, false otherwise.
  virtual bool onLButtonDown(TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control);

  //!
  //! Locator handler for interaction modes. 
  //!
  //! The default base implementation returns false.
  //!
  //! @param (x,y) Position of locator event, in device unit coordinates.
  //!
  //! @param shift Flag to indicate whether the shift key is pressed. An interaction
  //! mode may use this to modify its behaviour.
  //!
  //! @param control Flag to indicate whether the control key is pressed. An interaction
  //! mode may use this to modify its behaviour.
  //!
  //! @return true if the view needs to be redrawn, false otherwise.
  virtual bool onLButtonUp(TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control);

  //!
  //! Locator handler for interaction modes.  
  //!
  //! The default base implementation pans to the click position and returns true
  //! if the pan was successful.  This behaviour occurs if the constructor was
  //! passed the appropriate flag.
  //!
  //! @param (x,y) Position of locator event, in device unit coordinates.
  //!
  //! @param shift Flag to indicate whether the shift key is pressed. An interaction
  //! mode may use this to modify its behaviour.
  //!
  //! @param control Flag to indicate whether the control key is pressed. An interaction
  //! mode may use this to modify its behaviour.
  //!
  //! @return true if the view needs to be redrawn, false otherwise.
  virtual bool onMButtonDown(TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control);

  //!
  //! Locator handler for interaction modes. 
  //!
  //! The default base implementation returns false.
  //!
  //! @param (x,y) Position of locator event, in device unit coordinates.
  //!
  //! @param shift Flag to indicate whether the shift key is pressed. An interaction
  //! mode may use this to modify its behaviour.
  //!
  //! @param control Flag to indicate whether the control key is pressed. An interaction
  //! mode may use this to modify its behaviour.
  //!
  //! @return true if the view needs to be redrawn, false otherwise.
  virtual bool onMButtonUp(TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control);

  //!
  //! Locator handler for interaction modes.
  //!
  //! The default base implementation returns false.
  //!
  //! @param (x,y) Position of locator event, in device unit coordinates.
  //!
  //! @param shift Flag to indicate whether the shift key is pressed. An interaction
  //! mode may use this to modify its behaviour.
  //!
  //! @param control Flag to indicate whether the control key is pressed. An interaction
  //! mode may use this to modify its behaviour.
  //!
  //! @return true if the view needs to be redrawn, false otherwise.
  virtual bool onRButtonUp(TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control);

  //!
  //! Locator handler for interaction modes. 
  //!
  //! The default base implementation returns false.
  //!
  //! @param (x,y) Position of locator event, in device unit coordinates.
  //!
  //! @param shift Flag to indicate whether the shift key is pressed. An interaction
  //! mode may use this to modify its behaviour.
  //!
  //! @param control Flag to indicate whether the control key is pressed. An interaction
  //! mode may use this to modify its behaviour.
  //!
  //! @return true if the view needs to be redrawn, false otherwise.
  virtual bool onRButtonDown(TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control);

  //!
  //! Mouse move handlers for interaction mode base class
  //!
  //! The default base class implementations all do nothing and return false.
  //!
  //! @param button Type of button pressed, if any.
  //!
  //! @param (x,y) Position of locator event, in device unit co-ordinates.
  //!
  //! @param shift Flag to indicate whether the shift key is pressed. An interaction
  //! mode may use this to modify its behaviour.
  //!
  //! @param control Flag to indicate whether the control key is pressed. An interaction
  //! mode may use this to modify its behaviour.
  //!
  //! @return true if the view needs to be redrawn, false otherwise. The default
  //! base class implementations all return false.
  virtual bool onMouseMove(TSLButtonType button, TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control);

  //!
  //! Mouse wheel hander for interaction modes. 
  //!
  //! The default behaviour of this is to zoom in or out by the percentage passed
  //! to the constructor.  A positive value of delta is a zoom-in, a negative
  //! value of delta is a zoom-out.  This behaviour can be overridden by derived
  //! classes.
  //!
  //! @param delta  Delta value of mouse wheel movement. See Windows help for further
  //! information.
  //!
  //! @param (x,y) Position of locator event, in device unit coordinates.
  //!
  //! @param zoomPercentage Percentage of view to zoom in/out
  //!
  //! @param lockCursorLocation  If true, then lock to cursor location at the same place on the screen, otherwise
  //!        zoom in around the centre of the current view.  Requires the specific point to be in client window space
  //!        not in screen space.
  //!
  //! @return true if the view needs to be redrawn, false otherwise.
  virtual bool onMouseWheel(short delta, TSLDeviceUnits x, TSLDeviceUnits y);

  //!
  //! Query the unique id passed in the constructor.
  //!
  int id();

  void setInteractionModeRequest(InteractionModeRequest* requestHandler);
  void setDrawingSurface(envitia::maplink::earth::Surface3D* drawingSurface);

protected:
  Interaction();
  Interaction(int modeID);
  
  InteractionModeRequest* m_requestHandler = nullptr;
  envitia::maplink::earth::Surface3D* m_drawingSurface = nullptr;
  //!
  //! Id of this mode, initialised by the constructor.
  int m_id;
};

#endif