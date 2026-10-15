using System;
using System.Collections.Generic;
using System.Text;
using Envitia.MapLink;
using Envitia.MapLink.ML3D;

namespace CSharp3DSample
{
  public abstract class TSL3DViewMode 
  {
    public const double LOOK_AHEAD_OFFSET = (-5.0);

    public TSL3DViewMode(TSL3DViewModeInterface iViewMode, int modeID)
    {
      m_id = modeID;
      m_interface = iViewMode;
      m_drawingSurface = m_interface.drawingSurface; 
    }

    //called when the imode is activated
    public virtual bool activate(System.Windows.Forms.IWin32Window parentWindow)
    {
      return false;
    }

    /*
     * Locator handlers for view mode base class
     *
     * The default base class implementations all do nothing and return false.
     * 
     * NOTE : On Windows NT, an mfc application automatically uses a window 
     *        class that traps double click events and sends them to the
     *        On?ButtonDblClick handler.  It may be necessary to redirect
     *        these to the On?ButtonDown handler to avoid losing some
     *        interactions
     * 
     * Argument list description:
     * 
     * (x,y): Position of locator event, in device unit co-ordinates.
     *  
     * shift: Flag to indicate whether the shift key is pressed. An 
     *        interaction mode may use this to modify its behaviour.
     * 
     * control: Flag to indicate whether the control key is pressed. An 
     *          interaction mode may use this to modify its behaviour.
     * 
     * Returns true if the view needs to be redrawn, false otherwise. The 
     * default base class implementations all return false.
     */
    public virtual bool OnLButtonDown(int x, int y, bool shift, bool control)
    {
      return false;
    }

    public virtual bool OnMButtonDown(int x, int y, bool shift, bool control)
    {
      return false;
    }

    public virtual bool OnRButtonDown(int x, int y, bool shift, bool control)
    {
      return false;
    }

    public virtual bool OnLButtonUp(int x, int y, bool shift, bool control)
    {
      return false;
    }

    public virtual bool OnMButtonUp(int x, int y, bool shift, bool control)
    {
      return false;
    }

    public virtual bool OnRButtonUp(int x, int y, bool shift, bool control)
    {
      return false;
    }

    public virtual bool OnMouseMove(int x, int y, bool shift, bool control)
    {
      return false;
    }

    protected static double degreesToRadians(double deg)
    {
      return (deg * Math.PI) / 180.0;
    }
    
    //
    // Query mode id specified during construction.
    //
    public int id
    {
      get { return m_id; }
    }
    
    protected TSL3DViewModeInterface m_interface ;
    protected TSLN3DDrawingSurface m_drawingSurface;
    int m_id ;
  }
}
