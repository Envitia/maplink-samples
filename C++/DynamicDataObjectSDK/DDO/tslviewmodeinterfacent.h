//	********************************************************
//	*           Source Control Information
//	********************************************************
//	$Archive: /Samples/NT/DDO/tslviewmodeinterfacent.h $
//	$Author$
//	$Date$
//	$Revision$
//	********************************************************

#ifndef _tslviewmodeinterfacent_h_
#define _tslviewmodeinterfacent_h_ 1

// Turn on STRICT for VC6.0 and MFC compatability
#ifndef NO_STRICT
#ifndef STRICT
#define STRICT 1
#endif
#endif

#include "tslviewmodeinterface.h"

//
//	This is a NT specific class for a simple interaction mode handling
//	system.  This allows the various interaction modes such as zoom
//	and pan to be implemented in an encapsulated and operating system
//	independant manner.
//	
//	The users application creates an instance of one the TSLViewMode
//	derived classes and passes all mouse actions to it.  These methods
//	will return true if the view and changed and needs to be redrawn,
//	or false if the view has not changed.  Once the interaction is no
//	longer required, the object should be deleted.  
//
//	Operating system dependant code such as echo rectangle drawing is
//	encapsulated in the objects derived from this virtual base class.
//	The application should create the appropriate one of these objects 
//	and pass it when creating the TSLViewModeXxxx objects.
//
//	The mode implementations may request various services of this interface
//	class.  At present, the only services implemented are an echo rectangle.
//      and mouse capture.
//
class TSLViewModeInterfaceNT : public TSLViewModeInterface
{
  public:
    //
    // Constructor for NT specific view mode interface base class
    //
    // Argument list description:
    //
    // ds: A handle to the TSLDrawingSurface to be used by the modes.
    // 
    // hwnd: Handle to window that echo rectangles should be drawn in.
    //
    TSLViewModeInterfaceNT( TSLDrawingSurface * drawingSurface, HWND hwnd ) ;

    //
    // Destructor for view mode interface class
    //
    // Does nothing at present.
    //
    virtual ~TSLViewModeInterfaceNT() ; 

    //
    // Service function to draw any existing echo rectangle.
    //
    // This method must be overridden by the Operating System specific version
    // of this interface class.  It will only draw the echo rectangle if one
    // has been requested by the interaction mode.
    //
    // This method may be called by the setEchoRectangle and clearEchoRectangle
    // methods and should also be called in the applications OnDraw handler
    // so that any echo rectangle is drawn properly during expose events.
    //
    virtual void drawEchoRectangle( ) ;

    //
    // Service function to capture the mouse.
    //
    // This method must be overridden by the Operating System specific version
    // of this interface class.  
    //
    virtual void captureMouse( ) ;

    //
    // Service function to capture the mouse.
    //
    // This method must be overridden by the Operating System specific version
    // of this interface class.  
    //
    virtual void releaseMouse( ) ;

  private:

    //
    // Window passed in constructor.  This is used for echo rectangle drawing
    // and mouse capture.
    //
    HWND m_hwnd ;
} ;

#endif 
