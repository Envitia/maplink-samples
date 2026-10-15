//	********************************************************
//	*           Source Control Information
//	********************************************************
//	$Archive: /Development/Samples/NT/OverlayOpacity/tslviewmode.h $
//	$Author$
//	$Date$
//	$Revision$
//	********************************************************

#ifndef _tslviewmode_h_
#define _tslviewmode_h_ 1

#include "tslviewmodeinterface.h"

//
//	This is the base class for a simple interaction mode handling
//	system.  This allows the various interaction modes such as zoom
//	and pan to be implemented in an encapsulated and operating system
//	independant manner.
//	
//	The users application creates an instance of one the TSLViewMode
//	derived classes and passes all mouse actions to it.  These methods
//	will return true if the view and changed and needs to be redrawn,
//	or false if the view has not changed.  Once the interaction is no
//	longer required, the object should be deleted.  The modeID passed
//	to constructor of these objects is not used by the class itself
//	but may be queried by the user.  This is typically queried in the
//	User Interface update handlers to determine which mode is active.
//	For example, in an OverlayOpacity application, the menu item ID which invoked
//	the mode may be passed when creating the TSLViewModeXxx object and
//	queried in a common OnCommandUpdate handler for the menu. 
//
//	Operating system dependant code such as echo rectangle drawing is
//	encapsulated in the objects derived from the virtual base class
//	TSLViewModeInterface.  The application should create the appropriate
//	one of these objects and pass it when creating the TSLViewModeXxxx
//	objects.
//
//	All locator methods in this base class return false to indicate
//	that no redraw is required.  In the derived classes, only those
//	locator methods that are actually useful need to be overridden.
//
class TSLViewMode  
{
  public:
    //
    // Constructor for view mode base class
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
    TSLViewMode( TSLViewModeInterface * iViewMode, unsigned long modeID ) ;

    //
    // Destructor for view mode base class
    //
    virtual ~TSLViewMode();

    //
    // Locator handlers for view mode base class
    //
    // The default base class implementations all do nothing and return false.
    //
    // NOTE : On Windows NT, an OverlayOpacity application automatically uses a window 
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
    virtual bool OnMButtonDown( long x, long y, bool shift, bool control );
    virtual bool OnRButtonDown( long x, long y, bool shift, bool control );
    virtual bool OnLButtonUp( long x, long y, bool shift, bool control );
    virtual bool OnMButtonUp( long x, long y, bool shift, bool control );
    virtual bool OnRButtonUp( long x, long y, bool shift, bool control );
    virtual bool OnMouseMove( long x, long y, bool shift, bool control );
    
    //
    // Query mode id specified during construction.
    //
    unsigned long id(){ return m_id ; }
    
  protected :

    //
    // These attributes are stored and queried during construction.
    //
    TSLViewModeInterface * m_interface ;
    TSLDrawingSurface    * m_drawingSurface ;
    unsigned long                   m_id ;
} ;

#endif 
