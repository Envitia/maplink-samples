// *********************************************************************************
// * Copyright (c) 1998 to 2021 by Envitia Group PLC.
// *********************************************************************************

#ifndef interactionmoderequest_3d_h
#define interactionmoderequest_3d_h 1

class Interaction;

//!
//! This is the request handler, used for callbacks to the application.
//!
//! The application should derive from this class, and pass a pointer to the
//! constructor of the TSLInteractionModeManager.
//!
//! @ingroup group_coresdk_interactionmodes apigroup_interaction_modes
//! \{
class InteractionModeRequest
{
//! \}
  public:
      //!
      //! This method is triggered when the mode has gone back to the default - or no
      //! current mode if nothing is the default.
      //!
      //!
      //! @param newMode  This is a pointer to the new active mode.
      //!
      //! @param button  If the reset was caused by a button press, then this is the type of
      //! button pressed.  Can be used to for context menu invocation.
      //!
      //! @param (xDU, yDU)  If the reset was caused by a button press, then this is the
      //! location of the press.  Can be used to for context menu invocation.
      virtual void resetMode (Interaction* newMode, TSLButtonType button, TSLDeviceUnits xDU, TSLDeviceUnits yDU) = 0;

      //!
      //! This is triggered when the view has changed for any reason.
      //!
      //! @param drawingSurface  Pointer to the drawing surface whose view has changed.
      virtual void viewChanged (envitia::maplink::earth::Surface3D* drawingSurface) = 0;


	  virtual void geometryAdded(earth::geometry::Geometry* geom) = 0;
	  virtual void geometryRemoved(earth::geometry::Geometry* geom) = 0;
	  virtual void geometrySelected(earth::geometry::Geometry* geom) = 0;
	  virtual void trackSelected(earth::Track* track) = 0;
	  virtual TSLEnvelope GetWindowEnvelope() = 0;

      virtual ~InteractionModeRequest() {}
};

#endif
