using System;
using System.Collections.Generic;
using System.Text;

namespace CSharpMapViewer
{
  class TSLViewModeZoom : TSLViewMode
  {
    public TSLViewModeZoom(TSLViewModeInterface iViewMode, int modeID, double zoomPercentage)
      : base(iViewMode, modeID)
    {
      m_percentage = zoomPercentage;
      m_startX = 0;
      m_startY = 0;
      m_pressed = false;
    }

    public override bool OnLButtonDown(int x, int y, bool shift, bool control)
    {
      // Remember where the event occurred but wait until the release
      // before doing anything.
      m_startX = x;
      m_startY = y;
      m_pressed = true;

      // Capture the mouse so that we get the Button Up event even if the user leaves
      // the window.
      m_interface.captureMouse();
      return false;
    }

    public override bool OnLButtonUp(int x, int y, bool shift, bool control)
    {
      // This can sometimes be called when the title bar is double clicked
      // and therefore cause an inadvertent zoom
      if (!m_pressed)
        return false;

      // Now the user has released the button, we can deduce what they want to do.


      // Release the mouse capture.
      m_interface.releaseMouse();

      // Firstly clear the flags and any rectangle drawn.
      m_interface.clearEchoRectangle();
      m_pressed = false;

      // Calculate the size of any rectangle dragged
      int dx = x - m_startX;
      int dy = y - m_startY;

      if (Math.Abs(dx) < 3 && Math.Abs(dy) < 3)
      {
        // If we haven't moved more than 3 pixels, then assume its just mouse wobble.
        // Zoom in by the configured percentage and request redraw if successful.
        return m_drawingSurface.zoom(m_percentage, true, false);
      }

      // Here we have a rectangle to zoom to.
      double x1, y1;
      double x2, y2;

      // Calculate the corners in user units.  It doesn't matter which way
      // round they are, since the TSLDrawingSurface will sort them.
      m_drawingSurface.DUToUU(m_startX, m_startY, out x1, out y1);
      m_drawingSurface.DUToUU(x, y, out x2, out y2);

      // Zoom to the requested rectangle without auto redraw. 
      // The final argument requests that the TSLDrawingSurface maintains the existing 
      // aspect ratio, as determined by the window.
      // If the resize is successful, request a redraw.
      return m_drawingSurface.resize(x1, y1, x2, y2, false, true);
    }

    public override bool OnRButtonDown(int x, int y, bool shift, bool control)
    {
      // Zoom out by required percentage
      // If the zoom is successful, request a redraw.
      return m_drawingSurface.zoom(m_percentage, false, false);
    }

    public override bool OnMouseMove(int x, int y, bool shift, bool control)
    {
      // Only do anything if we have pressed
      if (!m_pressed)
        return false;

      // Calculate delta offsets from the press event location
      int dx = x - m_startX;
      int dy = y - m_startY;

      if (Math.Abs(dx) < 3 && Math.Abs(dy) < 3)
      {
        // If we haven't moved more than 3 pixels, then assume its just mouse wobble.
        m_interface.clearEchoRectangle();
      }
      else
      {
        // We have moved more than 3 pixels from the start position.
        // Set the position of the echo rectangle. Automatically causes it to be drawn
        m_interface.setEchoRectangle(m_startX, m_startY, x, y);
      }

      // We never want a redraw here
      return false;
    }

    private int m_startX;
    private int m_startY;
    private bool m_pressed;
    private double m_percentage;

  }
}
