using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace CSharpTerrainSample
{
  public partial class TerrainOptions : Form
  {
    private TerrainOptionsSection m_options;

    public TerrainOptions(TerrainOptionsSection opts)
    {
      m_options = opts;
      InitializeComponent();

      AutoScaleCheck.Checked = m_options.AutoScale;
      BathymetryCheck.Checked = m_options.Bathymetry;
      ManualScaleEdit.Text = m_options.ManualScale.ToString();
      TerrainScaleEdit.Text = m_options.TerrainScale.ToString();
      BathymetryScaleEdit.Text = m_options.BathymetryScale.ToString();
      StartPointHeightEdit.Text = m_options.StartHeight.ToString();
      EndPointHeightEdit.Text = m_options.EndHeight.ToString();
      MaxRangeMUEdit.Text = m_options.LOSAreaRangeMU.ToString();
      GridCellsInLOSCalcEdit.Text = m_options.LOSAreaGridCells.ToString();
      DarkShadedCheck.Checked = m_options.DarkShaded;
      AccurateLOSCheck.Checked = m_options.UseAccurateLos;
      StartHeightAboveGroundCheck.Checked = m_options.StartHeightAboveGround;
      ApplyCurvatureOfEarthCheck.Checked = m_options.CurveOfEarth;
    }

    private void okButton_Click(object sender, EventArgs e)
    {
      m_options.AutoScale = AutoScaleCheck.Checked;
      m_options.Bathymetry = BathymetryCheck.Checked;
      m_options.ManualScale = double.Parse(ManualScaleEdit.Text);
      if (m_options.ManualScale == 0.0)
      {
        m_options.ManualScale = 1.0;
      }

      m_options.TerrainScale = double.Parse(TerrainScaleEdit.Text);
      if (m_options.TerrainScale == 0.0)
      {
        m_options.TerrainScale = 1.0;
      }

      m_options.BathymetryScale = double.Parse(BathymetryScaleEdit.Text);
      if (m_options.BathymetryScale == 0.0)
      {
        m_options.BathymetryScale = 1.0;
      }

      m_options.StartHeight = double.Parse(StartPointHeightEdit.Text);
      m_options.EndHeight = double.Parse(EndPointHeightEdit.Text);
      m_options.LOSAreaRangeMU = double.Parse(MaxRangeMUEdit.Text);

      m_options.LOSAreaGridCells = int.Parse(GridCellsInLOSCalcEdit.Text);

      m_options.DarkShaded = DarkShadedCheck.Checked;
      m_options.UseAccurateLos = AccurateLOSCheck.Checked;
      m_options.StartHeightAboveGround = StartHeightAboveGroundCheck.Checked;
      m_options.CurveOfEarth = ApplyCurvatureOfEarthCheck.Checked;
    }

    private void OnDoubleEditChanged(object sender, EventArgs e)
    {
      TextBox editBox = (TextBox)sender;
      double value;
      try
      {
        value = double.Parse(editBox.Text);
      }
      catch (System.FormatException ex)
      {
        MessageBox.Show(ex.Message);
        editBox.Text = "";
      }
    }

    private void OnIntEditChanged(object sender, EventArgs e)
    {
      TextBox editBox = (TextBox)sender;
      int value;
      try
      {
        value = int.Parse(editBox.Text);
      }
      catch (System.FormatException ex)
      {
        MessageBox.Show(ex.Message);
        editBox.Text = "";
      }
    }

  }
}