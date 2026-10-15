using System;
using System.Collections.Generic;
using System.Text;

namespace CSharpMapViewer
{
  class TSLViewModeRotate : TSLViewMode
  {
    public TSLViewModeRotate(TSLViewModeInterface iViewMode, int modeID)
      : base(iViewMode, modeID)
    {
      m_y = 0;
      m_pressed = false;
    }

    public override bool OnLButtonDown(int x, int y, bool shift, bool control)
    {
      m_y = y;
      m_startAngle = m_drawingSurface.rotation;
      m_pressed = true;
      return false;
    }

    public override bool OnLButtonUp(int x, int y, bool shift, bool control)
    {
      m_pressed = false;
      return false;
    }

    public override bool OnMouseMove(int x, int y, bool shift, bool control)
    {
      if (!m_pressed)
        return false;
      int dy = y - m_y;
      return m_drawingSurface.rotate(m_startAngle - (dy * (3.141526 / 180.0)));
    }

    private int m_y;
    private double m_startAngle;
    private bool m_pressed;
  }
}
