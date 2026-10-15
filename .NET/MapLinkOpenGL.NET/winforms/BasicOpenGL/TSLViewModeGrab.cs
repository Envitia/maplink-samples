using System;
using System.Collections.Generic;
using System.Text;

namespace CSharpWinforms_BasicOpenGL_DotNet_Sample
{
  class TSLViewModeGrab : TSLViewMode
  {
    public TSLViewModeGrab(TSLViewModeInterface iViewMode, int modeID)
      : base(iViewMode, modeID)
    {
      m_x = 0;
      m_y = 0;
      m_pressed = false;
    }

    public override bool OnLButtonDown(int x, int y, bool shift, bool control)
    {
      m_drawingSurface.DUToUU(x, y, out m_x, out m_y);      
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

      return m_drawingSurface.pan(m_x, m_y, x, y, false);
    }

    private double m_x;
    private double m_y;
    private bool m_pressed;
  }
}
