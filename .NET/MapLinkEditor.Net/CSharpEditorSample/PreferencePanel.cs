using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Windows.Forms.Design;
using Envitia.MapLink;
using Envitia.MapLink.Editor;

#pragma warning disable 168

namespace CSharpEditorSample
{
  public partial class PreferencePanel : UserControl
  {
    private IOpManager m_opman = null;
    private double m_distance;
    private double m_angle;

    public PreferencePanel()
    {
      InitializeComponent();
      SetStyle(ControlStyles.SupportsTransparentBackColor, true);
    }

    public void SetOpManager(OpManager opman)
    {
      m_opman = opman;
      this.AngleEdit.Text = "0.0";
      this.DistanceEdit.Text = "0.0";
      CursorReadoutCombo.SelectedIndex = 0;
    }

    private void OnLoad(object sender, EventArgs e)
    {
      this.BackColor = Color.Transparent;
    }

    private void OnDistanceChanged(object sender, EventArgs e)
    {
      double val = 0.0;
      try
      {
        val = double.Parse(DistanceEdit.Text);
      }
      catch (FormatException ex)
      {
        MessageBox.Show(this, "Please enter a valid number", "CSharp .NET Editor Sample", MessageBoxButtons.OK, MessageBoxIcon.Error);
        DistanceEdit.Text = "0.0";
      }
      m_distance = val;
      m_opman.distance(m_distance);
      
    }

    private void OnAngleChanged(object sender, EventArgs e)
    {
      double val = 0.0;
      try
      {
        val = double.Parse(AngleEdit.Text);
      }
      catch (FormatException ex)
      {
        MessageBox.Show(this, "Please enter a valid number", "CSharp .NET Editor Sample", MessageBoxButtons.OK, MessageBoxIcon.Error);
        AngleEdit.Text = "0.0";
      }
      m_angle = val;
      m_opman.angular(val);
    }

    private void OnCursorReadoutChanged(object sender, EventArgs e)
    {
      m_opman.cursorReadoutType(CursorReadoutCombo.Text);
    }

    public double Distance
    {
      get
      {
        return m_distance;
      }
    }

    public double Angle
    {
      get
      {
        return m_angle;
      }
    }
  }

  [ToolStripItemDesignerAvailability(ToolStripItemDesignerAvailability.ToolStrip)]
  public class DockablePreferencesPanel : ToolStripControlHost
  {
    public DockablePreferencesPanel()
      : base(new PreferencePanel(), "Preferences")
    {
    }


    public void SetOpManager(OpManager opman)
    {
      ((PreferencePanel)this.Control).SetOpManager(opman);
    }

    public double Distance
    {
      get
      {
        if (Control == null)
          return 0.0;

        return ((PreferencePanel)this.Control).Distance;
      }
    }

    public double Angle
    {
      get
      {
        if (Control == null)
          return 0.0;

        return ((PreferencePanel)this.Control).Angle;
      }
    }
  }
}
