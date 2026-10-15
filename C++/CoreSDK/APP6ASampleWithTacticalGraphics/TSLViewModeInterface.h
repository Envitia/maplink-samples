#ifndef _tslviewmodeinterface_h_
#define _tslviewmodeinterface_h_ 1

class TSLMagnifyingGlass ;
class TSLDrawingSurface ;

typedef void (*TSLViewModeResetCB)( void * arg ) ;

//This is a base class for a simple interaction mode handling 
//system.  This allows the various interaction modes such as 
//zoom and pan to be implemented in an encapsulated and 
//operating system independant manner.
//	
//The users application creates an instance of one the 
//TSLViewMode derived classes and passes all mouse actions to 
//it.  These methods will return true if the view and changed 
//and needs to be redrawn, or false if the view has not 
//changed.  Once the interaction is no longer required, the 
//object should be deleted.  
//
//Operating system dependant code such as echo rectangle 
//drawing is encapsulated in the objects derived from this 
//virtual base class.  The application should create the 
//appropriate one of these objects and pass it when creating 
//the TSLViewModeXxxx objects.
//
//The mode implementations may request various services of 
//this interface class.  At present, the only services 
//implemented are an echo rectangle and mouse capture.
class TSLViewModeInterface 
{
public:
  //Service function to indicate that the current mode 
  //wishes to exit.  This will trigger a callback method 
  //supplied by the application via the setResetCB method.
  void resetMode();


  //Constructor for view mode interface base class
  //
  //The derived classes will specify additional Operating System specific arguments.
  //
  //Argument list description:
  //
  //ds: A handle to the TSLDrawingSurface to be used by the modes.
  //
  TSLViewModeInterface( TSLDrawingSurface * ds ) ;

  //Destructor for view mode interface class
  //
  //Does nothing at present.
  //
  virtual ~TSLViewModeInterface();

  //Service function to set echo rectangle corners.
  //
  //This erases any existing echo rectangle and draws the new one.
  //
  //Argument display list.
  // 
  //(x1,y1): A corner of the rectangle.
  //
  //(x2,y2): Opposite corner of the rectangle.
  //
  void setEchoRectangle( long x1, long y1, long x2, long y2 ) ;

  //Service function to clear echo rectangle corners.
  //
  //This erases any existing echo rectangle.
  //
  void clearEchoRectangle( ) ;

  //Service function to draw any existing echo rectangle.
  //
  //This method must be overridden by the Operating System 
  //specific version of this interface class.  It will only 
  //draw the echo rectangle if one has been requested by 
  //the interaction mode.
  //
  //This method may be called by the setEchoRectangle and 
  //clearEchoRectangle methods and should also be called in 
  //the applications OnDraw handler so that any echo 
  //rectangle is drawn properly during expose events.
  virtual void drawEchoRectangle( ) = 0 ;

  //
  //Service function to set echo line points
  //
  //This erases any existing echo line and draws the new one.
  //
  //Argument display list.
  // 
  //(x1,y1): Point 1.
  //(x2,y2): Point 2.
  void setEchoLine( long x1, long y1, long x2, long y2 ) ;

  //
  // Service function to clear echo line.
  //
  // This erases any existing echo line.
  void clearEchoLine( ) ;

  //
  // Service function to draw any existing echo line.
  //
  // This method must be overridden by the Operating System specific version
  // of this interface class.  It will only draw the echo line if one
  // has been requested by the interaction mode.
  //
  // This method may be called by the setEchoLine and clearEchoLine
  // methods and should also be called in the applications OnDraw handler
  // so that any echo rectangle is drawn properly during expose events.
  virtual void drawEchoLine( ) = 0 ;

  //Service function to capture the mouse.
  //
  //This method must be overridden by the Operating System 
  //specific version of this interface class.
  virtual void captureMouse( ) = 0 ;

  //Service function to capture the mouse.
  //
  //This method must be overridden by the Operating System 
  //specific version of this interface class.
  virtual void releaseMouse( ) = 0 ;

  //Query the TSLDrawingSurface that this is attached to.
  TSLDrawingSurface * drawingSurface(){ return m_drawingSurface ; }

  //Service function to allow an application to provide a 
  //reset callback.  This allows a mode to reset to the 
  //default mode.
  void setResetCB( TSLViewModeResetCB cb, void *arg ) ;

  //Service function to switch buffering on.
  //This method must be overridden by the Operating System specific version
  //of this interface class.  
  virtual void bufferingOn() = 0 ;

  //Service function to switch buffering off.
  //
  //This method must be overridden by the Operating System specific version
  //of this interface class.  
  virtual void bufferingOff() = 0 ;

  //Service function to inform of a window resize.
  //
  //This method must be overridden by the Operating System specific version
  //of this interface class.  
  virtual void onSize( long cx, long cy ) = 0 ;


  //Service function to initialise bitmaps, etc.
  //
  //This method must be overridden by the Operating System specific version
  //of this interface class.
  //
  //Argument display list.
  // 
  //(width,height): The extent of the surface.
  virtual void setupBackgroundBuffer( long width, long height ) = 0;

  //Service function to draw a map with a magnifying glass.
  //
  //This method must be overridden by the Operating System specific version
  //of this interface class.
  //
  //Argument display list.
  // 
  //magnifier: The magnifying glass.
  //
  //restoreBackground: true, if background is to be restored.
  virtual void magnify( TSLMagnifyingGlass* magnifier, bool restoreBackground ) = 0;

  //Service function to draw magnifying glass drawing surface to supplied buffer.
  //
  //This method must be overridden by the Operating System specific version
  //of this interface class.
  //
  //Argument display list.
  // 
  // buffer: the buffer to draw into.
  // (x1,y1): Old origin of buffer.
  // (x2,y2): New origin of buffer.
  // (width,height): The extent of the buffer.
  // restoreBackground: true, if background is to be restored.
  virtual void drawMagnifierToBuffer( HDC buffer, long x1, long y1, long x2, long y2, long width, long height, bool restoreBackground,
    long boundaryThickess, long boundaryRed, long boundaryGreen, long boundaryBlue ) = 0;

  virtual TSLViewModeInterface * clone( ) = 0 ;

protected:

  //Callback function to be called when a mode requests a 
  //reset.  This should reset to the default mode.
  TSLViewModeResetCB m_resetCB;

  //Argument passed to the m_resetCB callback function to 
  //be called when a mode requests a reset.  This should 
  //reset to the default mode.
  void* m_resetArg;


  //The TSLDrawingSurface to control.
  TSLDrawingSurface * m_drawingSurface ;

  //Corners of echo rectangle in device coordinates.
  //Sorted so that x1,y1 < x2,y2.
  //This is usually top left and bottom right corners.
  long m_x1 ;
  long m_y1 ;
  long m_x2 ;
  long m_y2 ;

  //Flags to indicate that the echo rectangle or line has been set 
  //and we should draw it when required.
  bool m_drawRect ;
  bool m_drawLine ;
};

#endif
