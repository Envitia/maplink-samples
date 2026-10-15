using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace CSharpTerrainSample
{
  public partial class TerrainInfo : Form
  {
    private TerrainViewer m_parent;

    public TerrainInfo(TerrainViewer parent)
    {
      m_parent = parent;
      InitializeComponent();
    }

    public void update( double lat, double lon, double x, double y, double z,
	          double nearestX, double nearestY, double nearestZ, double xRes, double yRes )
    {

      if ( this.Visible )
      {
        TerrainLat.Text = String.Format("{0,5:f}", lat);
        TerrainLong.Text = String.Format("{0,5:f}", lon);
        TX.Text = String.Format("{0}", x);
        TY.Text = String.Format("{0}", y);
        TZ.Text = String.Format("{0}", z);
        NearestX.Text = String.Format("{0}", nearestX);
        NearestY.Text = String.Format("{0}", nearestY);
        NearestZ.Text = String.Format("{0}", nearestZ);
        ResX.Text = String.Format("{0,5:F}", xRes);
        ResY.Text = String.Format("{0,5:F}", yRes);
      }
    }

    private void OnClosing(object sender, FormClosingEventArgs e)
    {
      switch (e.CloseReason)
      {
        case CloseReason.UserClosing:
          this.Visible = false;
          m_parent.UpdateMenuToolbarButtons();
          e.Cancel = true;
          break;
        default:
          break;
      }
    }
  }
}