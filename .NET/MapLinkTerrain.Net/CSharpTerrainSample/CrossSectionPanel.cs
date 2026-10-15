//#define DESIGNER

using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using Envitia.MapLink.Terrain;

namespace CSharpTerrainSample
{
  public class CrossSectionPanel : Panel
  {
    private int m_num_values ;
    private double [] m_values = new double[100]  ;
    private double m_range;
    private double m_manual_scale ;
    private double m_terr_scale;
    private double m_bathy_scale;
    public double m_startheight;
    public double m_endheight;
    private bool m_terrainAutoscale;
    private bool m_bathy;
    private TerrainViewer m_parent;

    public CrossSectionPanel()
    {
      m_parent = null;
      m_num_values = 0;
      m_range = 0.0;

      m_manual_scale = 2000.0;
      m_terrainAutoscale = false;
      m_terr_scale = 1.0;
      m_bathy_scale = 1.0;
      m_bathy = true;
      m_startheight = 1.0;
      m_endheight = 1.0;

      InitializeComponent();
    }

    public void configure(TerrainViewer parent, TerrainOptionsSection options)
    {
      m_parent = parent;
      m_manual_scale = options.ManualScale;
      m_terrainAutoscale = options.AutoScale;
      m_terr_scale = options.TerrainScale;
      m_bathy_scale = options.BathymetryScale;
      m_bathy = options.Bathymetry;
      m_startheight = options.StartHeight;
      m_endheight = options.EndHeight;
    }

    private void InitializeComponent()
    {
      this.SuspendLayout();
      // 
      // CrossSectionPanel
      // 
      this.Paint += new System.Windows.Forms.PaintEventHandler(this.OnPaint);
      this.ResumeLayout(false);

    }


    public TSLNTerrainDatabase terrainDB
    {
      get
      {
        if (m_parent == null)
          return null;
        return m_parent.terrainDatabase;
      }
    }

    public void SetData(int num_values, double[] values, double range)
    {
      m_num_values = num_values;

      for (int i = 0; i < m_num_values; i++)
      {
        m_values[i] = values[i];
      }
      Invalidate(false);

      m_range = range;
    }

    protected override void OnPaintBackground(PaintEventArgs pevent)
    {
      // do nothing...
      // we don't want the background to flash over the map
    }

    private void OnPaint(object sender, PaintEventArgs e)
    {
      BufferedGraphicsContext currentContext = BufferedGraphicsManager.Current;
      BufferedGraphics myBuffer = currentContext.Allocate(e.Graphics, DisplayRectangle);

      myBuffer.Graphics.FillRectangle(new SolidBrush(Color.White), DisplayRectangle);

      if (m_num_values < 2)
      {
        myBuffer.Render();
        myBuffer.Dispose();
        return;
      }

      int baseLine = DisplayRectangle.Bottom - 30;
      int topLine = DisplayRectangle.Top + 30;
      int leftLine = DisplayRectangle.Left + 30;
      int rightLine = DisplayRectangle.Right - 30;

      double middleLine = ((baseLine - topLine) / 2.0) + topLine;

      double screenWidth = (double)(rightLine - leftLine);
      double screenHeight = (double)(baseLine - topLine);

      // If we displaying depth data then use half for that.
      if (m_bathy)
      {
        screenHeight = screenHeight / 2.0;
        middleLine = (baseLine / 2.0) + topLine;
      }
      else
      {
        middleLine = baseLine;
      }

      //-------------------------------------------------------------------
      // TERRAIN 
      //-------------------------------------------------------------------

      // Calculate Horizontal Spacing
      // ----------------------------
      // This is the width of the screen divided by the number of points to display.
      double hspacing = screenWidth / (double)(m_num_values - 1);

      // Calculate Vertical Spacing
      // --------------------------
      // This is the height of the screen, divide by two (because we need to display
      // depth data) divided the highest point we want to display.
      double vspacing;
      double maxval = m_manual_scale;
      if (m_terrainAutoscale)
      {
        maxval = 0.0;

        // Get the highest point from the data.
        for (int i = 0; i < m_num_values; i++)
        {
          maxval = (maxval > m_values[i]) ? maxval : m_values[i];
        }

        // Calculate the vertical spacing. 
        if (maxval > 0)
        {
          vspacing = screenHeight / (maxval * m_terr_scale);
        }
        else
        {
          vspacing = 0;
        }

      }
      else
      {
        // Get the highest point from the config.
        vspacing = screenHeight / m_manual_scale;
      }

      // Draw it 
      // -------

      Point[] points = new Point[m_num_values + 2];

      // Start from..
      points[0].X = (int)leftLine;
      points[0].Y = (int)middleLine;

      int j = 0;
      for (; j < m_num_values; ++j)
      {
        points[j + 1].X = (int)((j * hspacing) + leftLine);

        int val = (int)(m_values[j] * vspacing);

        // Terrain must be greater than 0.
        val = (val > 0) ? val : 0;

        // Multiply by the terrain scale.
        val = (int)(val * m_terr_scale);

        // Save the y point.
        points[j + 1].Y = (int)(middleLine - val);
      }

      // End at..
      points[m_num_values + 1].X = (int)rightLine;
      points[m_num_values + 1].Y = (int)middleLine;

      myBuffer.Graphics.FillPolygon(new SolidBrush(Color.FromArgb(0, 200, 0)), points);
      myBuffer.Graphics.DrawPolygon(new Pen(Color.Black), points);

      // ------------------------------------------------
      // Now Draw the Sea ( If the Bathymetry is true )
      // ------------------------------------------------
      double minval = 0.0 - m_manual_scale;

      if (m_bathy)
      {
        // Calculate Vertical Spacing
        if (m_terrainAutoscale)
        {
          minval = 0.0;

          for (int i = 0; i < m_num_values; i++)
          {
            minval = (minval < m_values[i]) ? minval : m_values[i];
          }

          if (minval < 0)
            vspacing = screenHeight / (-minval * m_bathy_scale);
          else
            vspacing = 0;

        }
        else
        {
          vspacing = screenHeight / m_manual_scale;
        }

        // Start From.. 
        points[0].X = (int)leftLine;
        points[0].Y = (int)middleLine;

        // **********
        for (j = 0; j < m_num_values; j++)
        {
          points[j + 1].X = (int)((j * hspacing) + leftLine);

          int val = (int)(m_values[j] * vspacing);

          val = (val < 0) ? val : 0;

          // Multiple by the batymetry scale.
          val = (int)(val * m_bathy_scale);

          points[j + 1].Y = (int)(middleLine - val);

        }

        // End at ...
        points[m_num_values + 1].X = (int)rightLine;
        points[m_num_values + 1].Y = (int)middleLine;

        myBuffer.Graphics.FillPolygon(new SolidBrush(Color.FromArgb(0, 0, 200)), points);
        myBuffer.Graphics.DrawPolygon(new Pen(Color.Black), points);

      } // End Bathymetry


      // If the user has switched on scale factors in the config panel, the 
      // heights of all points will be X times what they really are. So, we
      // must DIVIDE the scale values by X to get a realistic looking scale.
      if (!m_terrainAutoscale)
      {
        maxval = maxval / m_terr_scale;
        minval = minval / m_bathy_scale;
      }

      String range;
      if ((int)(m_range / 1000) != 0.0)
      {
        range = String.Format("{0:F3}km", (m_range / 1000));
      }
      else
      {
        range = String.Format("{0:F3}m", m_range);
      }

      myBuffer.Graphics.DrawLine(new Pen(Color.Black), (int)leftLine, (int)middleLine, (int)rightLine, (int)middleLine);
      Font f = new Font(FontFamily.GenericSansSerif, (float)10.0);
      SizeF tSize = myBuffer.Graphics.MeasureString(range, f);
      myBuffer.Graphics.FillRectangle(new SolidBrush(Color.White), (float)rightLine - 70, (float)middleLine - 25, tSize.Width, tSize.Height);
      myBuffer.Graphics.DrawString(range, f, new SolidBrush(Color.Black), (float)rightLine - 70, (float)middleLine - 25);

      if (maxval > 0)
      {
        String text = String.Format("{0}", (int)maxval);
        myBuffer.Graphics.DrawLine(new Pen(Color.Black), (int)leftLine, (int)middleLine, (int)leftLine, (int)topLine);
        myBuffer.Graphics.DrawString("0", new Font(FontFamily.GenericSansSerif, (float)10.0), new SolidBrush(Color.Black), (float)leftLine - 25, (float)middleLine);
        myBuffer.Graphics.DrawString(text, new Font(FontFamily.GenericSansSerif, (float)10.0), new SolidBrush(Color.Black), (float)leftLine - 25, (float)topLine);
      }

      if (m_bathy && m_terrainAutoscale)
      {
        String text = String.Format("{0}", (int)minval);
        myBuffer.Graphics.DrawLine(new Pen(Color.Black), (int)leftLine, (int)middleLine, (int)leftLine, (int)baseLine);
        myBuffer.Graphics.DrawString(text, new Font(FontFamily.GenericSansSerif, (float)10.0), new SolidBrush(Color.Black), (float)leftLine - 25, (float)baseLine);
      }

      if (!m_terrainAutoscale)
      {
        String text = String.Format("{0}", (int)maxval);
        myBuffer.Graphics.DrawLine(new Pen(Color.Black), (int)leftLine, (int)baseLine, (int)leftLine, (int)topLine);
        myBuffer.Graphics.DrawString("0", new Font(FontFamily.GenericSansSerif, (float)10.0), new SolidBrush(Color.Black), (float)leftLine - 25, (float)middleLine);
        myBuffer.Graphics.DrawString(text, new Font(FontFamily.GenericSansSerif, (float)10.0), new SolidBrush(Color.Black), (float)leftLine - 25, (float)topLine);

        if (m_bathy)
        {
          text = String.Format("{0}", (int)minval);
          myBuffer.Graphics.DrawString(text, new Font(FontFamily.GenericSansSerif, (float)10.0), new SolidBrush(Color.Black), (float)leftLine - 25, (float)baseLine);
        }

      }
      myBuffer.Render();
      myBuffer.Dispose();
    }
    
  }
}
