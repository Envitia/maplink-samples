using System;
using System.Collections.Generic;
using System.Text;
using Envitia.MapLink;
using Envitia.MapLink.OpenGLSurface;

namespace CSharpWinforms_BasicOpenGL_DotNet_Sample
{
#if false
    using DrawingSurfaceClass = TSLNDrawingSurface;
#else
    using DrawingSurfaceClass = TSLNOpenGLSurface;
#endif

  public abstract class TSLViewMode 
  {
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

    public virtual bool OnMouseWheel(int x, int y, int delta, bool shift, bool control)
    {
      return m_drawingSurface.zoom(30.0, delta > 0, false);
    }
    
    //
    // Query mode id specified during construction.
    //
    public int id
    {
      get { return m_id; }
    }
    
    protected TSLViewModeInterface m_interface ;
    protected DrawingSurfaceClass m_drawingSurface;
    int m_id;
  }
}
