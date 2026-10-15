//	********************************************************
//	*           Source Control Information
//	********************************************************
//	$Archive: /Samples/NT/DDO/tslviewmodepan.h $
//	$Author$
//	$Date$
//	$Revision$
//	********************************************************

#ifndef _tslviewmodepan_h_
#define _tslviewmodepan_h_ 1

#include "tslviewmode.h"

//
//	This is a simple interaction mode for handling pan to point.
//	
//	The users application creates an instance of one these classes and 
//	passes all mouse actions to it.  This class only specialise the 
//	OnLButtonDown method.  When one of these events occurs, the mode
//	causes the current view of the TSLDrawingSurface to pan to the
//	specified location.  If successful, a redraw request is made by
//	the OnLButtonDown method returning true.
//
class TSLViewModePan : public TSLViewMode
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
    TSLViewModePan( TSLViewModeInterface * iface, long modeID );

    //
    // Destructor for pan view mode class
    //
    // Does nothing
    //
    virtual ~TSLViewModePan();

    //
    // Mode specific locator handlers this mode
    //
    // When one of these events occurs, the mode causes the current view 
    // of the TSLDrawingSurface to pan to the specified location.  
    // If successful, a redraw request is by returning true. 
    //
    // NOTE : On Windows NT, an DDO application automatically uses a window 
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
} ;

#endif
