using System;
using System.Collections.Generic;
using System.Text;
using Envitia.MapLink.ML3D;

namespace CSharp3DSample
{
  class TSL3DViewModeFly : TSL3DViewMode
  {
    const double MIN_ALTITUDE = 100.0;
    const double SCALING_FACTOR = 0.02;
    const TSLN3DCameraMoveAction action = TSLN3DCameraMoveAction.TSLN3DCameraMoveActionMaintainGroundOrientation;

    public TSL3DViewModeFly(TSL3DViewModeInterface iViewMode, int modeID)
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

    public override bool OnLButtonUp(int x, int y, bool shift, bool control)
    {
      m_active = false;
      return false;
    }

    public override bool OnMouseMove(int x, int y, bool shift, bool control)
    {
      int dy = y - m_originY;
      int dx = x - m_originX;

      if ( !m_active || (dy == 0 && dx == 0) )
      {
        return false;
      }

      double latitude, longitude, altitude;
      m_drawingSurface.camera.position( out latitude, out longitude, out altitude );

      if ( altitude < MIN_ALTITUDE )
      {
        altitude = MIN_ALTITUDE;
      }

      double delta = dy*altitude*SCALING_FACTOR;

      double forward  = control ? 0.0 : -delta;
      double upwards  = control ? -delta : 0.0;
      double sideways = dx*altitude*SCALING_FACTOR;

      bool maintainAltitude = !control;

      m_drawingSurface.camera.translate( forward, sideways, upwards, action, maintainAltitude );

      m_originX = x;
      m_originY = y;

      return true;
    }


    private int m_originX;
    private int m_originY;
    private bool m_active;
  }
}
