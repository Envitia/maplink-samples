#ifndef _tslviewmodezoom_h_
#define _tslviewmodezoom_h_ 1

#include "tslviewmode.h"

//This is a simple interaction mode for handling zoom to 
//rectangle.
//	
//The users application creates an instance of one these 
//classes and passes all mouse actions to it.  The amount by 
//which the class will zoom may be specified in the 
//constructor.
//
//When an LButtonDown event occurs, the mode stores the event 
//position.  If the mouse moves more that 3 pixels from the 
//starting position, then a rubber rectangle is drawn until 
//an LButtonUp event occurs.  If, when the LButtonUp event 
//occurs, the mouse is more then 3 pixels from the starting 
//position, then the mode causes the current view of the 
//TSLDrawingSurface to zoom to the specified rectangle.  The 
//aspect ratio is maintained. If the mouse has moved less 
//than 3 pixels, then the press is ignored.
//
//When an RButtonUp event occurs, the mode resets to edit 
//mode.
class TSLViewModeZoom : public TSLViewMode  
{
  public:
	//Constructor for zoom view mode class
	//
	//Argument list description:
	//
	//interface: An instance of the operating system specific 
	//interface class.  This provides OS specific features 
	//such as drawing echo rectangles.
	// 
	//modeID: This ID is not used internally but is stored 
	//and may be queried by the user application, typically 
	//in the User Interface update handlers
    TSLViewModeZoom( TSLViewModeInterface * iViewMode, unsigned long modeID     ) ;

	//Destructor for zoom pan view mode class
	//
	//Turns off the rubber rectangle if it is currently drawn.
    virtual ~TSLViewModeZoom();

	//Mode specific locator handler for this mode
	//
	//When an LButtonDown event occurs, the mode stores the 
	//event position.  
	//
	//If the mouse moves more that 3 pixels from the starting 
	//position, then a rubber rectangle is drawn until an 
	//LButtonUp event occurs.  If, when the LButtonUp event 
	//occurs, the mouse is more then 3 pixels from the 
	//starting position, then the mode causes the current 
	//view of the TSLDrawingSurface to zoom to the specified 
	//rectangle.  
	//The aspect ratio is maintained. 
	//
	//Argument list description:
	//
	//(x,y): Position of locator event, in device unit 
	//co-ordinates.
	// 
	//shift: Flag to indicate whether the shift key is 
	//pressed. An interaction mode may use this to modify its 
	//behaviour.
	//
	//control: Flag to indicate whether the control key is 
	//pressed. An interaction mode may use this to modify its 
	//behaviour.
	//
	//Returns true if the view needs to be redrawn, false 
	//otherwise.
    virtual bool OnLButtonDown( long x, long y, bool shift, bool control );
	//Mode specific locator handler for this mode
	//
	//When an RButtonDown event occurs, the mode causes the 
	//current  view of the TSLDrawingSurface to zoom out by a 
	//configurable percentage.  If successful, a redraw 
	//request is made by the OnRButtonDown method returning 
	//true.
	//
	//When a MouseMove event occurs, the rubber rectangle is 
	//updated.
	//
	//Argument list description:
	//
	//(x,y): Position of locator event, in device unit 
	//co-ordinates.
	// 
	//shift: Flag to indicate whether the shift key is 
	//pressed. An interaction mode may use this to modify its 
	//behaviour.
	//
	//control: Flag to indicate whether the control key is 
	//pressed. An interaction mode may use this to modify its 
	//behaviour.
	//
	//Returns true if the view needs to be redrawn, false 
	//otherwise.
    virtual bool OnRButtonUp( long x, long y, bool shift, bool control );

	//Mode specific locator handler for this mode
	//
	//When an LButtonDown event occurs, the mode stores the 
	//event position.  
	//
	//If the mouse moves more that 3 pixels from the starting 
	//position, then a rubber rectangle is drawn until an 
	//LButtonUp event occurs.  If, when the LButtonUp event 
	//occurs, the mouse is more then 3 pixels from the 
	//starting position, then the mode causes the current 
	//view of the TSLDrawingSurface to zoom to the specified 
	//rectangle.  The aspect ratio is maintained. 
	//
	//Argument list description:
	//
	//(x,y): Position of locator event, in device unit 
	//co-ordinates.
	// 
	//shift: Flag to indicate whether the shift key is 
	//pressed. An interaction mode may use this to modify its 
	//behaviour.
	//
	//control: Flag to indicate whether the control key is 
	//pressed. An interaction mode may use this to modify its 
	//behaviour.
	//
	//Returns true if the view needs to be redrawn, false 
	//otherwise.
    virtual bool OnLButtonUp( long x, long y, bool shift, bool control );
	//Mode specific locator handler for this mode
	//
	//When a MouseMove event occurs, the rubber rectangle is 
	//updated.
	//
	//Argument list description:
	//
	//(x,y): Position of locator event, in device unit 
	//co-ordinates.
	// 
	//shift: Flag to indicate whether the shift key is 
	//pressed. An interaction mode may use this to modify its 
	//behaviour.
	//
	//control: Flag to indicate whether the control key is 
	//pressed. An interaction mode may use this to modify its 
	//behaviour.
	//
	//Returns true if the view needs to be redrawn, false 
	//otherwise.
    virtual bool OnMouseMove( TSLViewModeButton button, long x, long y, bool shift, bool control );

    virtual const char * queryPrompt( ) ;

    virtual void OnDraw( long x1, long y1, long x2, long y2 ) ;

  private:

	//Storage for the zoom to rectangle start position.
    long   m_startX ;
    long   m_startY ;

	//A flag to indicate that a press has occurred, used in 
	//the zoom to rectangle handling.
    bool   m_pressed ;
} ;

#endif 
