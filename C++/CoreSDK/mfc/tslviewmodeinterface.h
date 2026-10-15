//	********************************************************
//	*           Source Control Information
//	********************************************************
//	$Archive: /Development/Samples/NT/mfc/tslviewmodeinterface.h $
//	$Author$
//	$Date$
//	$Revision$
//	********************************************************

#ifndef _tslviewmodeinterface_h_
#define _tslviewmodeinterface_h_ 1

//
//	This is a base class for a simple interaction mode handling
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
class TSLViewModeInterface 
{
  public:
    //
    // Constructor for view mode interface base class
    //
    // The derived classes will specify additional Operating System specific arguments.
    //
    // Argument list description:
    //
    // ds: A handle to the TSLDrawingSurface to be used by the modes.
    // 
    TSLViewModeInterface( TSLDrawingSurface * ds ) ;

    //
    // Destructor for view mode interface class
    //
    // Does nothing at present.
    //
    virtual ~TSLViewModeInterface();

    //
    // Service function to set echo rectangle corners.
    //
    // This erases any existing echo rectangle and draws the new one.
    //
    // Argument display list.
    // 
    // (x1,y1): A corner of the rectangle.
    //
    // (x2,y2): Opposite corner of the rectangle.
    //
    void setEchoRectangle( long x1, long y1, long x2, long y2 ) ;

    //
    // Service function to clear echo rectangle corners.
    //
    // This erases any existing echo rectangle.
    //
    void clearEchoRectangle( ) ;

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
    virtual void drawEchoRectangle( ) = 0 ;

    //
    // Service function to capture the mouse.
    //
    // This method must be overridden by the Operating System specific version
    // of this interface class.  
    //
    virtual void captureMouse( ) = 0 ;

    //
    // Service function to capture the mouse.
    //
    // This method must be overridden by the Operating System specific version
    // of this interface class.  
    //
    virtual void releaseMouse( ) = 0 ;

    //
    // Query the TSLDrawingSurface that this is attached to.
    //
    TSLDrawingSurface * drawingSurface(){ return m_drawingSurface ; }

  protected:

    //
    // The TSLDrawingSurface to control.
    //
    TSLDrawingSurface * m_drawingSurface ;

    // 
    // Corners of echo rectangle in device coordinates.
    // Sorted so that x1,y1 < x2,y2.
    // This is usually top left and bottom right corners.
    //
    long m_x1 ;
    long m_y1 ;
    long m_x2 ;
    long m_y2 ;

    //
    // Flag to indicate that the echo rectangle has been set and we should
    // draw it when required.
    //
    bool m_draw ;
};

#endif
