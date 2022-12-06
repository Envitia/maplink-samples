// *********************************************************************************
// * Copyright (c) 1998 to 2021 by Envitia Group PLC.
// *********************************************************************************

#ifndef InteractionModeManager_3d_h
#define InteractionModeManager_3d_h 1


class Interaction;
class InteractionModeRequest;

//!
//! This is the base class for a simple interaction mode handling system.  This
//! allows the various interaction modes such as zoom and pan to be managed and
//! implemented in an encapsulated and operating system independent manner.  In
//! addition, it maintains an optional view stack for a history of viewing
//! positions, and an array of optional saved positions which may be set by the
//! user.
//!
//! The user's application creates an instance of one the InteractionModeManager
//! derived classes, passing in a callback handler - derived from
//! InteractionModeRequest.   All mouse, draw and size events must be
//! forwarded to the manager, along with all view modification requests.  These
//! methods will typically return true if the view has changed and needs to be
//! redrawn, or false if the view has not changed.   In addition to this return
//! value, the viewChanged callback will be triggered if the view has changed.
//!
//! Operating system dependent code such as echo rectangle drawing, is
//! encapsulated in the objects derived from the virtual base class
//! InteractionModeManager and InteractionDisplay.  The InteractionModeManager
//! instance will create the appropriate version of the
//! InteractionDisplay class for internal use by the interaction modes.
//!
//! The application should create instances of individual Interaction
//! derived classes and pass these in using the addMode method.  A default mode
//! may be nominated which will be activated when other modes are inactive.
//! This is typically an editing mode.
//!
//! @ingroup group_coresdk_interactionmodes apigroup_interaction_modes
//! \{
class InteractionModeManager
{
//! \}
  public:
      //!
      //! The virtual destructor of the InteractionModeManager.  It destroys all
      //! memory allocated internally and also deletes the mode instance objects.
      virtual ~InteractionModeManager ();

      //!
      //! Adds a mode to the list managed by this InteractionModeManager.
      //!
      //! The InteractionModeManager assumes ownership of the mode, which will be
      //! destroyed when the manager instance is deleted.
      //!
      //! @param mode Instance of mode class to use.  The id property of the mode must be
      //! unique and non-zero.
      //!
      //! @param isDefault  Flag indicating whether this mode is to be used as the default.
      //! The last mode added with this flag becomes the default mode.  The default
      //! mode is activated when no other mode has been chosen.  If no mode is made
      //! the default then any subsequent mouse actions will be ignored until a mode
      //! is activated.
      //!
      //! @return true on success, false otherwise.
      bool addMode (Interaction* mode, bool isDefault);

      //!
      //! Query the id and instance of the currently active mode.
      //!
      //! Returns by value  the id property of the currently active mode, 0 if no mode
      //! is currently active.
      //!
      //! Returns by reference the mode instance.
      int getCurrentMode (Interaction** mode = 0) const;

      //!
      //! Query the id and instance of the nth mode, when nth is a value between 0 and
      //! numModes() - 1.
      //!
      //! Returns by value the id property of the nth mode, 0 on error.
      //!
      //! Returns by reference the mode instance.
      int getMode (int nth, Interaction** mode = 0) const;

      //!
      //! Returns the number of modes currently contained within the manager.
      //!
      int numModes () const;

      //!
      //! Locator handler for interaction modes.  This event will be passed on to the
      //! currently active mode.
      //!
      //! NOTE : On Windows, an mfc application automatically uses a window class that
      //! traps double click events and sends them to the On?ButtonDblClick handler.
      //! It may be necessary to redirect these to the On?ButtonDown handler to avoid
      //! losing some interactions
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
      virtual bool onLButtonDown (TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control);

      //!
      //! Locator handler for interaction modes.  This event will be passed on to the
      //! currently active mode.
      //!
      //! NOTE : On Windows, an mfc application automatically uses a window class that
      //! traps double click events and sends them to the On?ButtonDblClick handler.
      //! It may be necessary to redirect these to the On?ButtonDown handler to avoid
      //! losing some interactions
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
      virtual bool onLButtonUp (TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control);

      //!
      //! Locator handler for interaction modes.  This event will be passed on to the
      //! currently active mode.
      //!
      //! NOTE : On Windows, an mfc application automatically uses a window class that
      //! traps double click events and sends them to the On?ButtonDblClick handler.
      //! It may be necessary to redirect these to the On?ButtonDown handler to avoid
      //! losing some interactions
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
      virtual bool onMButtonDown (TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control);

      //!
      //! Locator handler for interaction modes.  This event will be passed on to the
      //! currently active mode.
      //!
      //! NOTE : On Windows, an mfc application automatically uses a window class that
      //! traps double click events and sends them to the On?ButtonDblClick handler.
      //! It may be necessary to redirect these to the On?ButtonDown handler to avoid
      //! losing some interactions
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
      virtual bool onMButtonUp (TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control);

      //!
      //! Mouse move handler for interaction modes.  This event will be passed on to
      //! the currently active mode.
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
      //! @return true if the view needs to be redrawn, false otherwise.
      virtual bool onMouseMove (TSLButtonType button, TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control);

      //!
      //! Mouse wheel hander for interaction modes.  This event will be passed on to
      //! the currently active mode.
      //!
      //! The default behaviour of this is to zoom in or out by the percentage passed
      //! to the constructor.  A positive value of delta is a zoom-in, a negative
      //! value of delta is a zoom-out.
      //!
      //! @param delta  Delta value of mouse wheel movement. See Windows help for further
      //! information.
      //!
      //! @param (x,y) Position of locator event, in client window device unit coordinates
      //!
      //! @return true if the view needs to be redrawn, false otherwise.
      virtual bool onMouseWheel (short delta, TSLDeviceUnits x, TSLDeviceUnits y);

      //!
      //! Locator handler for interaction modes.  This event will be passed on to the
      //! currently active mode.
      //!
      //! NOTE : On Windows, an mfc application automatically uses a window class that
      //! traps double click events and sends them to the On?ButtonDblClick handler.
      //! It may be necessary to redirect these to the On?ButtonDown handler to avoid
      //! losing some interactions
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
      virtual bool onRButtonUp (TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control);

      //!
      //! Locator handler for interaction modes.  This event will be passed on to the
      //! currently active mode.
      //!
      //! NOTE : On Windows, an mfc application automatically uses a window class that
      //! traps double click events and sends them to the On?ButtonDblClick handler.
      //! It may be necessary to redirect these to the On?ButtonDown handler to avoid
      //! losing some interactions
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
      virtual bool onRButtonDown (TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control);

      //!
      //! Event handler for the size method.  This event will be passed on to the
      //! currently active mode.
      //!
      //! This must be called in the size (OnSize) handler of the application, after
      //! the MapLink drawing surface has been resized.  This is used to draw any
      //! dynamic echo lines or other objects such as the magnifying glass.
      //!
      //! @param cx  New width of drawing surface.
      //!
      //! @param cy  New height of drawing surface.
      virtual void onSize (TSLDeviceUnits cx, TSLDeviceUnits cy);

      //!
      //! Query the cursor style that may be appropriate to the currently active mode.
      //!
      //! Returns TSLCursorStyleNone if no particular cursor style is appropriate,
      //! cursor style otherwise.
      TSLCursorStyle queryCursor ();

      //!
      //! Query prompt to display for currently active mode.
      //!
      const char * queryPrompt ();

      //!
      //! This method removes the specified mode from the manager, returning ownership
      //! back to the application.
      //!
      //! @param id  ID of mode to remove
      //!
      //! @return mode instance, NULL if mode not found.
      Interaction* removeMode (int id);

      //!
      //! Force reset to default mode (if any)
      //!
      //! This method is called by the Interactions when they should
      //! deactivate, but it may also be called by an application to make the manager
      //! revert to the default operation.  The manager will subsequently trigger the
      //! 'resetMode' callback in the InteractionModeRequest object.
      //!
      //! @param button  Button pressed which triggered deactivation (if any)
      //!
      //! @param (x,y)  If button pressed to trigger deactivation, location in device
      //! coordinates of press.
      void resetMode (TSLButtonType button, TSLDeviceUnits x, TSLDeviceUnits y);

      //!
      //! Sets the current mode to that specified.
      //!
      //! If a different mode is active, then it is deactivated before the specified
      //! mode is activated.
      //!
      //! @param id  Id property of the mode to activate.  This is passed in the constructor
      //! of the mode and must be unique.
      //!
      //! @return true if the mode was successfully activated, false otherwise.
      virtual bool setCurrentMode (int id, bool reactivate);

      //!
      //! Allows the application to set the default mode.
      //!
      //! @param id  ID of mode to set as the new default
      //!
      //! @return true if successful, false otherwise.
      bool setDefaultMode (int id);
	  
	  //!
	  //! This is the constructor for the base InteractionModeManager.  It is an
	  //! abstract base class and cannot be instantiated directly.  The arguments to
	  //! this are explained in the derived classes.
	  InteractionModeManager(InteractionModeRequest* requestHandler, envitia::maplink::earth::Surface3D* drawingSurface, int mouseWheelZoomPercentage = 30, bool lockCursorOnMouseWheel = false);

  protected:   
      void *m_modes;

      Interaction *m_currentMode;

      InteractionModeRequest *m_requestHandler;
	  envitia::maplink::earth::Surface3D* m_drawingSurface = nullptr;

      Interaction *m_defaultMode;

  private:
      //!
      //! Percentage to zoom in/out with a mouse wheel notch.
      int m_mouseWheelPercentage;

      bool m_lockCursorOnMouseWheel ;
};

#endif
