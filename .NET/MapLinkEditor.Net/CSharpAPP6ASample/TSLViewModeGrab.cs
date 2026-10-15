using System;
using System.Collections.Generic;
using System.Text;

namespace CSharpAPP6ASample
{
  class TSLViewModeGrab : TSLViewMode
  {
    public TSLViewModeGrab(TSLViewModeInterface iViewMode, long modeID) : base( iViewMode, modeID )
    {
      m_x = 0;
      m_y = 0;
      m_pressed = false;
    }

    public override bool OnLButtonDown(int x, int y, bool shift, bool control)
    {
      m_x = x;
      m_y = y;

      m_pressed = true;

      return false;
    }

    public override bool OnLButtonUp(int x, int y, bool shift, bool control)
    {
      m_pressed = false;
      return false;
    }

    public override bool OnMouseMove(System.Windows.Forms.MouseButtons button, int x, int y, bool shift, bool control)
    {
      if (!m_pressed)
        return false;

      // Calculate offset between the last point and the new point
      long du_offset_x = m_x - x;
      long du_offset_y = y - m_y;

      // Convert the offset into user units

      // No DUPerUU() function so we will take the long way round.
      double tmcPerDUx, tmcPerDUy;
      m_drawingSurface.TMCperDU(out tmcPerDUx, out tmcPerDUy);
      double uu_per_du = tmcPerDUx / m_drawingSurface.TMCperUU;

      double uu_offset_x = du_offset_x * uu_per_du;
      double uu_offset_y = du_offset_y * uu_per_du;

      // Get the current centre point of the drawing surface
      double uu_x = 0.0;
      double uu_y = 0.0;
      double uu_x1 = 0.0;
      double uu_y1 = 0.0;
      double uu_x2 = 0.0;
      double uu_y2 = 0.0;

      m_drawingSurface.getUUExtent( out uu_x1, out uu_y1, out uu_x2, out uu_y2);
      uu_x = (uu_x1 + uu_x2) / 2;
      uu_y = (uu_y1 + uu_y2) / 2;

      uu_x += uu_offset_x;
      uu_y += uu_offset_y;

      // Save the current point

      m_x = x;
      m_y = y;

      // Pan and redraw the drawing surface

      // Attempt to pan, without auto_redraw enabled.
      // Request redraw only if pan is successful
      return m_drawingSurface.pan(uu_x, uu_y, false);
    }

	  private long m_x;
    private long m_y;
    private bool m_pressed;
  }
}
