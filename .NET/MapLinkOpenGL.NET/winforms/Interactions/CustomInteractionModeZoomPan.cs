using System;
using Envitia.MapLink;
using Envitia.MapLink.InteractionModes;
using Envitia.MapLink.OpenGLSurface;

namespace CSharpWinforms_InteractionOpenGL_DotNet_Sample
{
#if false
    using DrawingSurfaceClass = TSLNDrawingSurface;
#else
    using DrawingSurfaceClass = TSLNOpenGLSurface;
#endif

  /// <summary>
  /// This class demonstrates how to implement a custom interaction mode
  /// </summary>
  public class CustomInteractionModeZoomPan : TSLNInteractionMode
  {
    private int m_myID;
    private int m_myZoomPercentage;
    private bool m_midButtonPansToPoint;
    private string m_myPrompt;

    public CustomInteractionModeZoomPan(int id, int zoomPercentage, bool middleButtonPansToPoint, string prompt)
      : base(id, middleButtonPansToPoint)
    {
      m_myID = id;
      m_myZoomPercentage = zoomPercentage;
      m_midButtonPansToPoint = middleButtonPansToPoint;
      m_myPrompt = prompt;
    }

    public override bool onLButtonDown(int x, int y, bool shift, bool control)
    {
      if (this.display != null && this.display.drawingSurfaceBase != null)
      {
          DrawingSurfaceClass ds = (DrawingSurfaceClass)this.display.drawingSurfaceBase;
        double uux, uuy;
        ds.DUToUU(x, y, out uux, out uuy);

        if (ds.zoom(m_myZoomPercentage, true, false))
          return ds.pan(uux, uuy, false);
      }
      return false;
    }

    public override bool onMButtonDown(int x, int y, bool shift, bool control)
    {
      if (m_midButtonPansToPoint && this.display != null && this.display.drawingSurfaceBase != null)
      {
          DrawingSurfaceClass ds = (DrawingSurfaceClass)this.display.drawingSurfaceBase;
        double uux, uuy;
        if (ds.DUToUU(x, y, out uux, out uuy))
        {
          if (ds.pan(uux, uuy, false))
          {
            this.display.viewChanged(true);
            return true;
          }
        }
      }
      return false;
    }

    public override bool onMouseWheel(short delta, int x, int y, int zoomPercentage)
    {
      if (m_myZoomPercentage != 0 && this.display != null && this.display.drawingSurfaceBase != null)
      {
          DrawingSurfaceClass ds = (DrawingSurfaceClass)this.display.drawingSurfaceBase;
        bool sts = ds.zoom(m_myZoomPercentage, delta > 0, false);
        if (sts)
          this.display.viewChanged(true);
        return sts;
      }
      return false;
    }

    public override bool onRButtonDown(int x, int y, bool shift, bool control)
    {
      if (this.display != null && this.display.drawingSurfaceBase != null)
      {
        DrawingSurfaceClass ds = (DrawingSurfaceClass)this.display.drawingSurfaceBase;
        double uux, uuy;
        ds.DUToUU(x, y, out uux, out uuy);

        if (ds.zoom(m_myZoomPercentage, false, false))
          return ds.pan(uux, uuy, false);
      }
      return false;
    }

    public override Envitia.MapLink.TSLNCursorStyle queryCursor()
    {
      return TSLNCursorStyle.TSLNCursorStyleMagnify;
    }

    public override string queryPrompt()
    {
      return m_myPrompt;
    }

  }
}
