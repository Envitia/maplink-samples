//	********************************************************
//	*           Source Control Information
//	********************************************************
//	$Archive: /Development/Samples/NT/ActiveXCPP/tslviewmodegrab.h $
//	$Author$
//	$Date$
//	$Revision$
//	********************************************************

#ifndef _tslviewmodegrab_h_
#define _tslviewmodegrab_h_ 1

#include "tslviewmode.h"

//
//	This is a simple interaction mode for handling pan by drag.
//	
//	The users application creates an instance of one these classes and 
//	passes all mouse actions to it.  This class specialises the 
//	OnLButtonDown/Up and MouseMove method.  
//	On button down remember the start position, whilst the move
//	does the actual pan and causes the current view of the 
//	TSLDrawingSurface to redraw by returning true
//
class TSLViewModeGrab : public TSLViewMode
{
  public:
  
    //
    // Constructor for grab view mode class
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
    TSLViewModeGrab(TSLViewModeInterface* iViewMode, long modeID, TSLViewModeCursor upCursor = 0, TSLViewModeCursor downCursor = 0 ) ;
  
    virtual bool OnLButtonDown(long x, long y, bool shift, bool control);
  
    //Completes the grab operation.
    virtual bool OnLButtonUp(long x, long y, bool shift, bool control);
  
    //If the left mouse button is pressed, calculates the 
    //offset between the new and last position and pans the 
    //map accordingly.
    virtual bool OnMouseMove(long x, long y, bool shift, bool control);
  
  private:
    long m_x;
    long m_y;
    bool m_pressed;
};

#endif 

