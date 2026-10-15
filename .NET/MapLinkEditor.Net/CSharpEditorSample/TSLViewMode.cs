using System;
using System.Collections.Generic;
using System.Text;
using Envitia.MapLink;

namespace CSharpEditorSample
{
  public abstract class TSLViewMode 
  {
    public enum TSLViewModeButton
    {
      TSLViewModeLeft = 0,
      TSLViewModeMiddle = 1,
      TSLViewModeRight = 2,
      TSLViewModeNone = 3,
    } ;

    public TSLViewMode(TSLViewModeInterface iViewMode, int modeID)
    {
      m_id = modeID;
      m_interface = iViewMode;
      m_drawingSurface = m_interface.drawingSurface; 
    }

    public virtual bool OnLButtonDown(int x, int y, bool shift, bool control)
    {
      return false;
    }

    public virtual bool OnMButtonDown(int x, int y, bool shift, bool control)
    {
      double uux, uuy;
      if (m_drawingSurface.DUToUU(x, y, out uux, out uuy))
      {
        if (m_drawingSurface.pan(uux, uuy, false))
          return true;
      }
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

    public virtual bool OnMouseMove(System.Windows.Forms.MouseButtons button, int x, int y, bool shift, bool control)
    {
      return false;
    }

    public virtual bool OnMouseWheel(int delta, int x, int y)
    {
      return m_drawingSurface.zoom(30.0, delta > 0, false);
    }

    public virtual void OnDraw(int x1, int y1, int x2, int y2)
    {
    }

    //
    // Query mode id specified during construction.
    //
    public int id
    {
      get
      {
        return m_id;
      }
    }
    
    protected TSLViewModeInterface m_interface ;
    protected TSLNDrawingSurface m_drawingSurface;
    int m_id;
  }


}
