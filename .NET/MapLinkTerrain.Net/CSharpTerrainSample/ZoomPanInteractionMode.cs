using System;
using System.Collections.Generic;
using System.Text;
using Envitia.MapLink;
using Envitia.MapLink.InteractionModes;

namespace CSharpTerrainSample
{
  class ZoomPanInteractionMode : TSLNInteractionMode
  {
    private int myID;
    private int myZoomPercentage;
    private bool midButtonPansToPoint;
    private string myPrompt;

    public ZoomPanInteractionMode(int id, int zoomPercentage, string prompt, bool middleButtonPansToPoint)
      : base(id, middleButtonPansToPoint)
    {
      myID = id;
      myZoomPercentage = zoomPercentage;
      midButtonPansToPoint = middleButtonPansToPoint;
      myPrompt = prompt;
    }

    public override bool onLButtonDown(int x, int y, bool shift, bool control)
    {
      if (this.display != null && this.display.drawingSurface != null)
      {
        TSLNDrawingSurface ds = this.display.drawingSurface;
        double uux, uuy;
        ds.DUToUU(x, y, out uux, out uuy);

        if (ds.zoom(myZoomPercentage, true, false))
          return ds.pan(uux, uuy, false);


      }
      return false;
    }

    public override bool onMButtonDown(int x, int y, bool shift, bool control)
    {
      if (midButtonPansToPoint && this.display != null && this.display.drawingSurface != null)
      {
        TSLNDrawingSurface ds = this.display.drawingSurface;
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
      if (myZoomPercentage != 0 && this.display != null && this.display.drawingSurface != null)
      {
        TSLNDrawingSurface ds = this.display.drawingSurface;
        bool sts = ds.zoom(myZoomPercentage, delta > 0, false);
        if (sts)
          this.display.viewChanged(true);
        return sts;
      }
      return false;
    }

    public override bool onRButtonDown(int x, int y, bool shift, bool control)
    {
      if (this.display != null && this.display.drawingSurface != null)
      {
        TSLNDrawingSurface ds = this.display.drawingSurface;
        double uux, uuy;
        ds.DUToUU(x, y, out uux, out uuy);

        if (ds.zoom(myZoomPercentage, false, false))
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
      return myPrompt;
    }
  }
}
