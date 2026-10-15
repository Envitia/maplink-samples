using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Runtime.InteropServices;
using Envitia.MapLink;
using Envitia.MapLink.InteractionModes;
using Envitia.MapLink.OpenGLSurface;

namespace CSharpWinforms_BasicOpenGL_DotNet_Sample
{
#if false
    using DrawingSurfaceClass = TSLNDrawingSurface;
#else
    using DrawingSurfaceClass = TSLNOpenGLSurface;
#endif

  public class TSLViewModeInterface
  {
    [DllImport("gdi32.dll")] private static extern int
      SetROP2(IntPtr hdc, int capindex);

    [DllImport("gdi32.dll")] private static extern IntPtr 
			SelectObject(IntPtr hdc, IntPtr hgdiobj);

    [DllImport("gdi32.dll")] private static extern IntPtr
      GetStockObject(int fnObject);

    [DllImport("gdi32.dll")] private static extern bool
      Rectangle(IntPtr hdc, int x1, int y1, int x2, int y2);

    public TSLViewModeInterface(DrawingSurfaceClass drawingSurface, IntPtr hwnd)
    {
      m_drawingSurface = drawingSurface;
      m_draw = false;
      m_x1 = 0;
      m_y1 = 0;
      m_x2 = 0;
      m_y2 = 0;
      m_hwnd = hwnd;
    }

    public void setEchoRectangle(int x1, int y1, int x2, int y2)
    {
      // First clear any existing rectangle
      drawEchoRectangle();

      // Now set up the new corners, sorted accordingly.
      m_x1 = x1 < x2 ? x1 : x2;
      m_y1 = y1 < y2 ? y1 : y2;
      m_x2 = x1 < x2 ? x2 : x1;
      m_y2 = y1 < y2 ? y2 : y1;
      m_draw = true;

      // Draw the new rectangle
      drawEchoRectangle();
    }

    public void clearEchoRectangle( ) 
    {
      // Erase any existing rectangle
      drawEchoRectangle();

      // Clear the internal flags and corners.
      m_x1 = m_y1 = m_x2 = m_y2 = 0;
      m_draw = false;
    }

    public void drawEchoRectangle()
    {
      if ( m_draw && m_hwnd != IntPtr.Zero )
      {
        Graphics g = Graphics.FromHwnd(m_hwnd);

        IntPtr hdc = g.GetHdc();

        // Set up a simple Exclusive-Or pen.
        int oldMode = SetROP2 ( hdc, 7 /*R2_XORPEN*/ ) ;
        IntPtr oldBrush = SelectObject ( hdc, GetStockObject( 5 /*NULL_BRUSH*/ ) ) ;
        IntPtr oldPen = SelectObject ( hdc, GetStockObject ( 6 /*WHITE_PEN*/ ) ) ;

        // Draw the rectangle, assuming x1,y1 < x2,y2
        //Rectangle(hdc, m_x1, m_y1, m_x2, m_y2);
        TSLNInteractionModeManagerGeneric m_manager = new TSLNInteractionModeManagerGeneric(null, m_drawingSurface, 5, 5, 30, true);
        TSLNInteractionModeDisplay m_disp = m_manager.display;
        if (m_disp != null)
        {
            m_disp.setEchoRectangle(m_x1, m_y1, m_x2, m_y2);
            m_disp.drawEchoRectangle();
        }
        m_manager.Dispose();

        //s.d
        // Reset back to the prevoius drawing mode.
        SelectObject ( hdc, oldBrush ) ;
        SelectObject ( hdc, oldPen ) ;
        SetROP2 ( hdc, oldMode  ) ;

        g.ReleaseHdc();

      }
    }

    public void captureMouse()
    {
    }

    public void releaseMouse( )
    {
    }

    public DrawingSurfaceClass drawingSurface
    {
      get
      {
        return m_drawingSurface ;
      }
    }

    private DrawingSurfaceClass m_drawingSurface;

    private int m_x1;
    private int m_y1;
    private int m_x2;
    private int m_y2;

    private bool m_draw;

    private IntPtr m_hwnd;

  }
}
