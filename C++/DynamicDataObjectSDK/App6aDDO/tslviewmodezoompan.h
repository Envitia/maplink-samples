//	********************************************************
//	*           Source Control Information
//	********************************************************
//	$Archive: /Samples/NT/DDO/tslviewmodezoompan.h $
//	$Author$
//	$Date$
//	$Revision$
//	********************************************************

#ifndef _tslviewmodezoompan_h_
#define _tslviewmodezoompan_h_ 1

#include "tslviewmode.h"

//
//	This is a simple interaction mode for handling zoom with pan.
//	
//	The users application creates an instance of one these classes and 
//	passes all mouse actions to it.  This class only specialise the 
//	OnLButtonDown and OnRButtonDown methods.  The amount by which the
//	class will zoom may be specified in the constructor.
//
//	When an LButtonDown event occurs, the mode causes the current 
//	view of the TSLDrawingSurface to pan to the specified location and zoom
//	in by a configurable percentage.  If successful, a redraw request is made by
//	the OnLButtonDown method returning true.
//
//	When an RButtonDown event occurs, the mode causes the current 
//	view of the TSLDrawingSurface to pan to the specified location and zoom
//	out by a configurable percentage.  If successful, a redraw request is made by
//	the OnLButtonDown method returning true.
//
class TSLViewModeZoomPan : public TSLViewMode  
{
  public:
    //
    // Constructor for zoom pan view mode class
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
    // zoomPercentage: This is the percentage passed when zooming.
    //
    TSLViewModeZoomPan( TSLViewModeInterface * iface, long modeID, 
			double zoomPercentage = 30.0 ) ;
    //
    // Destructor for zoom pan view mode class
    //
    // Does nothing
    //
    virtual ~TSLViewModeZoomPan();

    //
    // Mode specific locator handlers this mode
    //
    // When an LButtonDown event occurs, the mode causes the current 
    // view of the TSLDrawingSurface to pan to the specified location and zoom
    // in by a configurable percentage.  If successful, a redraw request is made by
    // the OnLButtonDown method returning true.
    //
    // When an RButtonDown event occurs, the mode causes the current 
    // view of the TSLDrawingSurface to pan to the specified location and zoom
    // out by a configurable percentage.  If successful, a redraw request is made by
    // the OnRButtonDown method returning true.
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
    virtual bool OnRButtonDown( long x, long y, bool shift, bool control );

  protected:

    //
    // Storage for the zoom percentage passed in the constructor
    //
    double m_percentage ;
};

#endif 
