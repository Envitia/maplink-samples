using System;
using System.Collections.Generic;
using System.Text;

namespace CSharpAPP6ASample
{
  class TSLViewModeZoomPan : TSLViewMode
  {
    public TSLViewModeZoomPan(TSLViewModeInterface iViewMode, long modeID, double zoomPercentage)
      : base(iViewMode, modeID)
    {
      m_percentage = zoomPercentage;
    }

    public override bool OnLButtonDown(int x, int y, bool shift, bool control)
    {
      double uux = 0.0;
      double uuy = 0.0;

      // Get the position pressed in user units.
      m_drawingSurface.DUToUU(x, y, out uux, out uuy);

      // Zoom in by required percentage. If successful, pan.
      // Request redraw if both operations are successful
      if (m_drawingSurface.zoom(m_percentage, true, false))
        return m_drawingSurface.pan(uux, uuy, false);

      return false;
    }

    public override bool OnRButtonDown(int x, int y, bool shift, bool control)
    {
      double uux = 0.0;
      double uuy = 0.0;

      // Get the position pressed in user units
      m_drawingSurface.DUToUU(x, y, out uux, out uuy);

      // Zoom out by required percentage. If successful, pan.
      // Request redraw if both operations are successful
      if (m_drawingSurface.zoom(m_percentage, false, false))
        return m_drawingSurface.pan(uux, uuy, false);

      return false;
    }

    private double m_percentage;
  }
}
