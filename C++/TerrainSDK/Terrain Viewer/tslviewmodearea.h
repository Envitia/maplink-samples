//	********************************************************
//	*           Source Control Information
//	********************************************************
//	$Archive: /MapLinkWizard/Template/tslviewmodeArea.h $
//	$Author$
//	$Date$
//	$Revision$
//	********************************************************

#ifndef _tslviewmodeArea_h_
#define _tslviewmodeArea_h_ 1

#include "tslviewmode.h"

class CTerrainViewerDoc;

//
//	This is a simple interaction mode for handling Area and Area to rectangle.
//	
//	The users application creates an instance of one these classes and 
//	passes all mouse actions to it.  The amount by which the
//	class will Area may be specified in the constructor.
//
//	When an LButtonDown event occurs, the mode stores the event position.  
//      If the mouse moves more that 3 pixels from the starting position, then a
//	rubber rectangle is drawn until an LButtonUp event occurs.  If, when the
//      LButtonUp event occurs, the mouse is more then 3 pixels from the starting
//      position, then the mode causes the current view of the TSLDrawingSurface to
//	Area to the specified rectangle.  The aspect ratio is maintained. If the 
//      mouse has moved less than 3 pixels, then the TSLDrawingSurface to
//	Areaed in by the configurable percentage.
//
//	When an RButtonDown event occurs, the mode causes the current 
//	view of the TSLDrawingSurface to Area out by a configurable percentage. 
//	If successful, a redraw request is made by the OnRButtonDown method 
//      returning true.
//
class TSLViewModeArea : public TSLViewMode  
{
  public:
    //
    // Constructor for Area view mode class
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
    // AreaPercentage: This is the percentage passed when Areaing.
    //
    TSLViewModeArea( TSLViewModeInterface * iViewMode, unsigned long modeID, CTerrainViewerDoc * pDoc) ;

    //
    // Destructor for Area pan view mode class
    //
    // Turns off the rubber rectangle if it is currently drawn.
    //
    virtual ~TSLViewModeArea();

    //
    // Mode specific locator handlers this mode
    //
    // When an LButtonDown event occurs, the mode stores the event position.  
    // If the mouse moves more that 3 pixels from the starting position, then a
    // rubber rectangle is drawn until an LButtonUp event occurs.  If, when the
    // LButtonUp event occurs, the mouse is more then 3 pixels from the starting
    // position, then the mode causes the current view of the TSLDrawingSurface to
    // Area to the specified rectangle.  The aspect ratio is maintained. If the 
    // mouse has moved less than 3 pixels, then the TSLDrawingSurface to
    // Areaed in by the configurable percentage.
    //
    // When an RButtonDown event occurs, the mode causes the current 
    // view of the TSLDrawingSurface to Area out by a configurable percentage. 
    // If successful, a redraw request is made by the OnRButtonDown method 
    // returning true.
    //
    // When a MouseMove event occurs, the rubber rectangle is updated.
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
    virtual bool OnLButtonUp( long x, long y, bool shift, bool control );
    virtual bool OnMouseMove( long x, long y, bool shift, bool control );
    virtual bool OnRButtonDown( long x, long y, bool shift, bool control );
    virtual bool OnMButtonDown( long x, long y, bool shift, bool control );
    virtual bool OnMouseWheel(long x, long y, int delta, bool shift, bool control);

  private:

    bool performViewshed(long x, long y);
    bool DUToMU( long x, long y, double & mux, double & muy );
    void getExtent( double mux, double  muy, TSLMUExtent & extent ) ;

    CTerrainViewerDoc * m_doc ;

    std::vector< TSLMUCoord  > m_fixedPositions ;
} ;

#endif 
