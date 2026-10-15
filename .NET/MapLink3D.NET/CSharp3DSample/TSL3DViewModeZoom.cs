using System;
using System.Collections.Generic;
using System.Text;
using Envitia.MapLink.ML3D;

namespace CSharp3DSample
{
  class TSL3DViewModeZoom : TSL3DViewMode
  {
    public TSL3DViewModeZoom(TSL3DViewModeInterface iViewMode, int modeID)
      : base(iViewMode, modeID)
    {
      m_originX = 0;
      m_originY = 0;
      m_active = false;
    }

    public override bool OnLButtonDown(int x, int y, bool shift, bool control)
    {
      m_originX = x;
      m_originY = y;
      m_active = true;

      return false;
    }

    public override bool OnMouseMove(int x, int y, bool shift, bool control)
    {
      int dx = m_originX - x;

      if (!m_active || (dx == 0))
        return false;

      double latitude, longitude, altitude;

      m_drawingSurface.camera.position(out latitude, out longitude, out altitude);

      long magnitude = Math.Abs(dx);
      double sign = dx / magnitude;

      for (int i = 0; i < magnitude; ++i)
        altitude += sign * altitude / 100.0;

      m_drawingSurface.camera.moveTo(latitude, longitude, altitude, TSLN3DCameraMoveAction.TSLN3DCameraMoveActionNone);
      m_drawingSurface.camera.lookAt(latitude, longitude + LOOK_AHEAD_OFFSET, 0.0, control);

      m_originX = x;
      m_originY = y;

      return true;
    }

    public override bool OnLButtonUp(int x, int y, bool shift, bool control)
    {
      m_active = false;
      return false;
    }

    private int m_originX;
    private int m_originY;
    private bool m_active;
  }
}
