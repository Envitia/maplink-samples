//	********************************************************
//	*           Source Control Information
//	********************************************************
//	$Archive: /Development/Samples/NT/LandLinkSample/TSLViewModeMagnify.h $
//	$Author$
//	$Date$
//	$Revision$
//	********************************************************

#ifndef _tslviewmodemagnify_h_
#define _tslviewmodemagnify_h_ 1

#include "tslviewmode.h"

class TSLMagnifyingGlass;


//
//	This is a simple interaction mode for handling the famous magnifying glass.
//	
//	The users application creates an instance of one these classes and 
//	passes all mouse actions to it.  This class only specialise the 
//	OnLButtonDown method.  When one of these events occurs, the mode
//	causes the current view of the TSLDrawingSurface to pan to the
//	specified location.  If successful, a redraw request is made by
//	the OnLButtonDown method returning true.
//
class TSLViewModeMagnify : public TSLViewMode
{
  public:
    //
    // Constructor for magnifying glass view mode class
    //
    // Argument list description:
    //
    // interface: An instance of the operating system specific interface 
    //	          class for the magnifier.  This provides OS specific features such as 
    //		  drawing echo rectangles.
    // 
    // modeID: This ID is not used internally but is stored and may be
    //         queried by the user application, typically in the
    //	       User Interface update handlers
    //
    TSLViewModeMagnify( TSLViewModeInterface* pInterface, unsigned long modeID );

    //
    // Destructor for magnifying glass view mode class.
    // Deletes the magnifier.
    //
    virtual ~TSLViewModeMagnify();

    //
    // Mode specific locator handlers this mode
    //
    // OnMouseMove() moves the magnifying glass & buffers the drawing of the window.
    //
    // OnLButtonUp() increases the area of the magnifying glass
    //    --- with shift increases the offset of the magnifying glass
    //
    // OnRButtonUp() decreases the area of the magnifying glass
    //    --- with shift decreases the offset of the magnifying glass
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
    virtual bool OnMouseMove( TSLViewModeButton button, long x, long y, bool shift, bool control );

    virtual bool OnRButtonUp( long x, long y, bool shift, bool control );

    //
    // Service function to inform of a window resize.
    //
    virtual void OnSize( long cx, long cy );

    virtual void OnDraw( long x1, long y1, long x2, long y2 ) ;

  private:
    //
    void updateMagnifierExtent( long x, long y );
    void updateMagnifiedArea( long x, long y );

    //
    // The magnifying glass.
    TSLMagnifyingGlass*	  m_magnifier;

    //
    // The previous magnifier position.
    int			  m_oldXpt;
    int			  m_oldYpt;

    // The previous mouse position.
    int                   m_mouseX ;
    int                   m_mouseY ;
} ;

#endif
