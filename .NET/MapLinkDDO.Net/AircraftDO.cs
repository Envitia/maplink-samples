using System;
using System.Collections.Generic;
using System.Drawing;
using System.Text;
using System.Runtime.InteropServices;

using Envitia.MapLink;
using Envitia.MapLink.DDO;

namespace CSharpDDOSample
{
  public abstract class AircraftDO : TSLNDisplayObject
  {

    public AircraftDO() : base(true)
    {
      setSize(0, 0, 0, 0);
    }

#if GDI_PLUS_DRAWING
    protected abstract Color foregroundColour
    {
      get;
    }

    protected abstract Color backgroundColour
    {
      get;
    }

    protected abstract Point[] points
    {
      get;
    }

    public override bool draw(TSLNRenderingInterface d_surface, TSLNEnvelope d_extent) 
    {
      TSLNCoord pos = ddo.position;

      int wndX, wndY;

      d_surface.TMCToDU(pos.x, pos.y, out wndX, out wndY);

      Graphics g = d_surface.graphics();

      double theta = ((Aircraft)ddo).heading;
      double sinT = Math.Sin(theta);
      double cosT = Math.Cos(theta);

      Point[] pts = points;

      int num = pts.Length;
      for (int i = 0; i < num; ++i)
      {
        double x = pts[i].X * cosT - pts[i].Y * sinT;
        double y = pts[i].Y * cosT + pts[i].X * sinT;

        pts[i].X = (int)Math.Round(wndX + x);
        pts[i].Y = (int)Math.Round(wndY + y);
      }

      Brush b = new SolidBrush(backgroundColour);
      Pen p = new Pen(foregroundColour);

      g.FillPolygon(b, pts);
      g.DrawPolygon(p, pts);

      return true;
    }

#else
    [DllImport("gdi32.dll")]
    private static extern IntPtr SelectObject(IntPtr hdc, IntPtr hgdiobj);
    [DllImport("gdi32.dll")]
    private static extern bool DeleteObject(IntPtr hObject);
    [DllImport("gdi32.dll")]
    private static extern IntPtr CreatePen(int fnPenStyle, int nWidth, int crColor);
    [DllImport("gdi32")]
    private static extern IntPtr CreateSolidBrush(int crColor);
    [DllImport("gdi32.dll")]
    private static extern bool Polygon(IntPtr hdc, int[] lpPoints, int nCount);

    protected abstract int foregroundColour
    {
      get;
    }

    protected abstract int backgroundColour
    {
      get;
    }

    protected abstract int[] points
    {
      get;
    }

    public override bool draw(TSLNRenderingInterface d_surface, TSLNEnvelope d_extent) 
    {
      TSLNCoord pos = ddo.position;

      int wndX, wndY;

      d_surface.TMCToDU(pos.x, pos.y, out wndX, out wndY);

      IntPtr hdc = d_surface.handleToDrawable();

      // Now draw the symbol
      IntPtr   pen      = CreatePen( 0/*PS_SOLID*/, 0, 0x00f0f0f0 /*OUTLINE_COLOUR*/ );
      IntPtr   brush    = CreateSolidBrush( backgroundColour );

      IntPtr oldPen   = SelectObject( hdc, pen );
      IntPtr oldBrush = SelectObject( hdc, brush );

      int [] pts = points;
      int num = pts.Length / 2;

      double theta = ( (Aircraft)ddo ).heading;
      double sinT = Math.Sin( theta );
      double cosT = Math.Cos( theta );

      for ( int i = 0; i < num; ++i )
      {
        double x = pts[i*2    ]*cosT - pts[i*2 + 1]*sinT;
        double y = pts[i*2 + 1]*cosT + pts[i*2    ]*sinT;

        pts[i*2  ] = wndX + (int)x;
        pts[i*2+1] = wndY + (int)y;
      }
      
      Polygon( hdc, pts, num );

      // Reselect previous attributes and delete these ones
      SelectObject( hdc, oldPen );
      SelectObject( hdc, oldBrush );

      DeleteObject( pen );
      DeleteObject( brush );

      return true;
    }

#endif
  }
}
