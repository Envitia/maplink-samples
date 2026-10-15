using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Runtime.InteropServices;
using Envitia.MapLink;
using Envitia.MapLink.ML3D;

namespace CSharp3DSample
{
  public class TSL3DViewModeInterface
  {
    [DllImport("gdi32.dll")] private static extern int
      SetROP2(IntPtr hdc, int capindex);

    [DllImport("gdi32.dll")] private static extern IntPtr 
      SelectObject(IntPtr hdc, IntPtr hgdiobj);

    [DllImport("gdi32.dll")] private static extern IntPtr
      GetStockObject(int fnObject);

    [DllImport("gdi32.dll")] private static extern bool
      Rectangle(IntPtr hdc, int x1, int y1, int x2, int y2);

    public TSL3DViewModeInterface( TSLN3DDrawingSurface drawingSurface, IntPtr hwnd)
    {
      m_drawingSurface = drawingSurface;
      m_draw = false;
      m_x1 = 0;
      m_y1 = 0;
      m_x2 = 0;
      m_y2 = 0;
      m_hwnd = hwnd;
    }

    /*
     * Service function to set echo rectangle corners.
     * 
     * This erases any existing echo rectangle and draws the new one.
     * 
     * Argument display list.
     *  
     * (x1,y1): A corner of the rectangle.
     * 
     * (x2,y2): Opposite corner of the rectangle.
     */
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

    /*
     * Service function to clear echo rectangle corners.
     *
     * This erases any existing echo rectangle.
     */
    public void clearEchoRectangle( ) 
    {
      // Erase any existing rectangle
      drawEchoRectangle();

      // Clear the internal flags and corners.
      m_x1 = m_y1 = m_x2 = m_y2 = 0;
      m_draw = false;
    }

    /*
     * Service function to draw any existing echo rectangle.
     *
     * It will only draw the echo rectangle if one has been requested by the 
     * interaction mode.
     *
     * This method may be called by the setEchoRectangle and clearEchoRectangle
     * methods and should also be called in the applications OnDraw handler
     * so that any echo rectangle is drawn properly during expose events.
     */
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
        Rectangle(hdc, m_x1, m_y1, m_x2, m_y2);

        // Reset back to the prevoius drawing mode.
        SelectObject ( hdc, oldBrush ) ;
        SelectObject ( hdc, oldPen ) ;
        SetROP2 ( hdc, oldMode  ) ;

        g.ReleaseHdc();

      }
    }

    /*
     * Query the TSL3DDrawingSurface that this is attached to.
     */
    public TSLN3DDrawingSurface drawingSurface
    {
      get
      {
        return m_drawingSurface;
      }
    }

    private TSLN3DDrawingSurface m_drawingSurface;

    private int m_x1;
    private int m_y1;
    private int m_x2;
    private int m_y2;

    private bool m_draw;

    private IntPtr m_hwnd;

  }
}
