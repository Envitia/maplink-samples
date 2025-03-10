using System;

namespace DrawingSurfacePanel
{
#if USEOPENGLSURFACE
  using DrawingSurfaceClass = Envitia.MapLink.OpenGLSurface.TSLNOpenGLSurface;
#else
  using DrawingSurfaceClass = Envitia.MapLink.TSLNDrawingSurface;
#endif

  /// <summary>
  /// This class demonstrates how to implement a custom interaction mode
  /// </summary>
  internal class CustomInteractionModeZoomPan : Envitia.MapLink.InteractionModes.TSLNInteractionMode
  {
    public int ID { get; private set; }
    public int ZoomPercentage { get; set; }
    public string Prompt { get; set; }
    public bool MiddleButtonPansToPoint { get; private set; }

    public CustomInteractionModeZoomPan(int id, bool middleButtonPansToPoint )
      : base(id, middleButtonPansToPoint)
    {
      ID = id;
      MiddleButtonPansToPoint = middleButtonPansToPoint;
    }   

    public override bool onLButtonDown(int x, int y, bool shift, bool control)
    {
      if (this.display != null && this.display.drawingSurfaceBase != null)
      {
        DrawingSurfaceClass ds = (DrawingSurfaceClass)this.display.drawingSurfaceBase;
        ds.DUToUU(x, y, out double uux, out double uuy);

        if (ds.zoom(ZoomPercentage, true, false))
          return ds.pan(uux, uuy, false);
      }
      return false;
    }

    public override bool onMButtonDown(int x, int y, bool shift, bool control)
    {
      if (MiddleButtonPansToPoint && this.display != null && this.display.drawingSurfaceBase != null)
      {
        DrawingSurfaceClass ds = (DrawingSurfaceClass)this.display.drawingSurfaceBase;
        if (ds.DUToMU(x, y, out double mux, out double muy))
        {
          System.Diagnostics.Trace.WriteLine(mux.ToString() + "," + muy.ToString());
        }
      }
      return false;
    }

    public override bool onMouseWheel(short delta, int x, int y, int zoomPercentage)
    {
      if (ZoomPercentage != 0 && this.display != null && this.display.drawingSurfaceBase != null)
      {
        DrawingSurfaceClass ds = (DrawingSurfaceClass)this.display.drawingSurfaceBase;
        bool sts = ds.zoom(ZoomPercentage, delta > 0, false);
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
        ds.DUToUU(x, y, out double uux, out double uuy);

        if (ds.zoom(ZoomPercentage, false, false))
          return ds.pan(uux, uuy, false);
      }
      return false;
    }

    public override Envitia.MapLink.TSLNCursorStyle queryCursor()
    {
      return Envitia.MapLink.TSLNCursorStyle.TSLNCursorStyleMagnify;
    }

    public override string queryPrompt()
    {
      return Prompt;
    }

  }
}
