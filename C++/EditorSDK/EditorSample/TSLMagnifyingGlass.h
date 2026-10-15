//	********************************************************
//	*           Source Control Information
//	********************************************************
//	$Archive: /Development/Samples/NT/LandLinkSample/TSLMagnifyingGlass.h $
//	$Author$
//	$Date$
//	$Revision$
//	********************************************************

#ifndef _tslmagnifyingglass_h_
#define _tslmagnifyingglass_h_ 1

class TSLViewModeInterface ;
class TSLDrawingSurface ;

//
//	This is the famous magnifying glass.
//	
//	The users application creates an object of this type.  in the app's draw()
//	method, this classes draw() method should be called at the end.
//
class TSLMagnifyingGlass
{
  public:
    //
    // Constructor for magnifying glass
    //
    // Argument list description:
    //
    // interface: An instance of the operating system specific interface 
    //	          class.  This provides OS specific features such as 
    //		  drawing echo rectangles.
    // 
    TSLMagnifyingGlass( TSLViewModeInterface* pInterface );
    //
    // Destructor for magnifying glass.
    //
    virtual ~TSLMagnifyingGlass();

    //
    // onSize method.
    //
    // Argument list description:
    //
    // (cx,cy): New size of window.
    // 
    virtual void onSize( long cx, long cy );

    //
    // Draw method.
    //
    // Argument list description:
    //
    // buffer: The buffer to draw into.
    // 
    // restoreBackground: true, if background is to be restored.
    //
    // Returns true if the view needs to be redrawn, false otherwise. The 
    // default base class implementations all return false.
    //
    virtual bool draw( HDC buffer, bool restoreBackground );

    //
    // Sets the extent of the magnifying window in device units.
    //
    // Argument list description:
    //
    // (leftX, topY): Top LH corner of extent.
    // (rightX, bottomY): Bottom RH corner of extent.
    // 
    bool setWinExtent( int leftX, int topY, int rightX, int bottomY );

    //
    // Sets the area of the magnifying window in user units.
    //
    // Argument list description:
    //
    // (uux1,uuy1): A corner of the extent.
    // (uux2,uuy2): Opposite corner of the extent.
    //
    bool setMagExtent( double uux1, double uuy1, double uux2, double uuy2 );

    //
    // Query methods for the magnifier's:
    //
    //    --- magnification
    //    --- size
    //    --- offset
    //
    double magnification() const { return m_magFactor; }
    double size() const { return m_magSize; }
    double offset() const { return m_magOffset; }
   
    //
    // Service methods to reset the magnifier to its initial values.
    //
    void reset();
    
  private:
    //
    void resetBuffers();
    
    //
    TSLViewModeInterface*   m_interface;
    TSLDrawingSurface*	    m_drawingSurface;

    // Magnification factor.
    // Size of glass as a % of window size.
    // Offset from pointer as a fraction of size.
    static double	    m_defaultMagFactor;
    static double	    m_defaultMagSize;
    static double	    m_defaultMagOffset;

    double		    m_magFactor;
    double		    m_magSize;
    double		    m_magOffset;

    // Boundary circle attributes
    static int m_boundaryThickness ;
    static int m_boundaryRed ;
    static int m_boundaryGreen ;
    static int m_boundaryBlue ;

    // Flag to indicate whether read defaults from config file
    static bool             m_initialisedDefaults ;

    // Extent in terms of device units
    int			    m_leftX;
    int			    m_topY;
    int			    m_rightX;
    int			    m_bottomY;
    
    int			    m_leftOldX;
    int			    m_topOldY;
    int			    m_rightOldX;
    int			    m_bottomOldY;
} ;

#endif

