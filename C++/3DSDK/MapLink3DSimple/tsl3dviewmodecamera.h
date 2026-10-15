//	********************************************************
//	*           Source Control Information
//	********************************************************
//	$Archive:$
//	$Author$
//	$Date$
//	$Revision$
//	********************************************************

#ifndef _tsl3dviewmodecamera_h_
#define _tsl3dviewmodecamera_h_ 1

#include "tsl3dviewmode.h"

//
//	This is a simple interaction mode for handling camera orientation.
//	
//	The users application creates an instance of one these classes and 
//	passes all mouse actions to it.  This class only specialise the 
//	OnLButtonDown method.  When one of these events occurs, the mode
//	causes the current view of the TSLDrawingSurface to pan to the
//	specified location.  If successful, a redraw request is made by
//	the OnLButtonDown method returning true.
//
class TSL3DViewModeCamera : public TSL3DViewMode
{
  public:
    //
    // Constructor for pan view mode class
    //
    // Argument list description:
    //
    // interface: An instance of the operating system specific interface 
    //	          class.  This provides OS specific features such as 
    //		  drawing echo rectangles.
    // 
    // modeID: This ID is not used internally but is stored and may be
    //         queried by the user application, typically in the
    //	       User Interface update handlers
    //
    TSL3DViewModeCamera( TSL3DViewModeInterface * iViewMode, unsigned long modeID );

    //
    // Destructor for pan view mode class
    //
    // Does nothing
    //
    virtual ~TSL3DViewModeCamera();

    //
    // Mode specific locator handlers for this mode
    //
    // When one of these events occurs, the mode causes the current view 
    // of the TSL3DDrawingSurface to rotate by dragging.  
    // If successful, a redraw request is by returning true. 
    //
    // NOTE : On Windows NT, an mfc application automatically uses a window 
    //        class that traps double click events and sends them to the
    //	      On?ButtonDblClick handler.  It may be necessary to redirect
    //	      these to the On?ButtonDown handler to avoid losing some
    //        interactions
    //
    // Argument list description:
    //
    // (x,y): Position of locator event, in device unit co-ordinates.
    // 
    // shift: Flag to indicate whether the shift key is pressed. An 
    //	      interaction mode may use this to modify its behaviour.
    //
    // control: Flag to indicate whether the control key is pressed. An 
    //	        interaction mode may use this to modify its behaviour.
    //
    // Returns true if the view needs to be redrawn, false otherwise. The 
    // default base class implementations all return false.
    //
    virtual bool OnLButtonDown( long x, long y, bool shift, bool control );
    virtual bool OnMouseMove( long x, long y, bool shift, bool control );
    virtual bool OnLButtonUp( long x, long y, bool shift, bool control );

    void orientation(double x, double y, double z, double angle);
    void fov(double angle);

private:
  long m_originX ;
  long m_originY ;
  bool m_active ;
} ;

#endif
