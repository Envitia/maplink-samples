using System;
using System.Collections.Generic;
using System.Text;

namespace CSharpMapViewer
{
  class TSLViewModePan : TSLViewMode
  {
    public TSLViewModePan(TSLViewModeInterface iViewMode, int modeID)
      : base(iViewMode, modeID)
    {
    }

    public override bool OnLButtonDown(int x, int y, bool shift, bool control)
    {
      double uux = 0.0;
      double uuy = 0.0;

      // Get the position pressed in user units
      m_drawingSurface.DUToUU(x, y, out uux, out uuy);

      // Attempt to pan, without auto_redraw enabled.
      // Request redraw only if pan is successful
      return m_drawingSurface.pan(uux, uuy, false);
    }
  }
}
