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

namespace CSharpAPP6ASample
{
  public partial class PreferencePanel : UserControl
  {
    private OpManager m_opman = null;
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
        MessageBox.Show(this, "Please enter a valid number", "CSharp .NET APP6A Sample", MessageBoxButtons.OK, MessageBoxIcon.Error);
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
        MessageBox.Show(this, "Please enter a valid number", "CSharp .NET APP6A Sample", MessageBoxButtons.OK, MessageBoxIcon.Error);
        AngleEdit.Text = "0.0";
      }
      m_angle = val;
      m_opman.angular(val);
    }

    private void OnCursorReadoutChanged(object sender, EventArgs e)
    {
      switch ( CursorReadoutCombo.SelectedIndex )
      {
        case 0 :
          m_opman.cursorReadoutType(TSLNCursorMovementType.TSLNCursorMovementTMCAbsolute, "Pos : X %.0f   Y %.0f");
          break ;

        case 1 :
          m_opman.cursorReadoutType(TSLNCursorMovementType.TSLNCursorMovementTMCRelative, "Pos : X %.0f   Y %.0f");
          break ;

        case 2 :
          m_opman.cursorReadoutType(TSLNCursorMovementType.TSLNCursorMovementMUAbsolute, "Pos : X %.0f   Y %.0f");
          break ;

        case 3 :
          m_opman.cursorReadoutType(TSLNCursorMovementType.TSLNCursorMovementMURelative, "Pos : X %.0f   Y %.0f");
          break ;

        case 4 :
          m_opman.cursorReadoutType(TSLNCursorMovementType.TSLNCursorMovementUUAbsolute, "Pos : X %.0f   Y %.0f");
          break ;

        case 5 :
          m_opman.cursorReadoutType(TSLNCursorMovementType.TSLNCursorMovementUURelative, "Pos : X %.0f   Y %.0f");
          break ;

        case 6 :
          m_opman.cursorReadoutType(TSLNCursorMovementType.TSLNCursorMovementLatLongDecimal, "Pos : Lat %.3f   Long %.3f");
          break ;

        case 7 :
          m_opman.cursorReadoutType(TSLNCursorMovementType.TSLNCursorMovementLatLongAngular, "Pos : Lat %s   Long %s");
          break ;

        case 8 :
          m_opman.cursorReadoutType(TSLNCursorMovementType.TSLNCursorMovementAngleTMCDistance, "Pos : Angle %3.3f   Distance %.0f");
          break ;

        case 9 :
          m_opman.cursorReadoutType(TSLNCursorMovementType.TSLNCursorMovementBearingTMCDistance, "Pos : Brg %3.3f   Distance %.0f");
          break ;

        case 10 :
          m_opman.cursorReadoutType(TSLNCursorMovementType.TSLNCursorMovementAngleMUDistance, "Pos : Angle %3.3f   Distance %.0f");
          break ;

        case 11 :
          m_opman.cursorReadoutType(TSLNCursorMovementType.TSLNCursorMovementBearingMUDistance, "Pos : Brg %3.3f   Distance %.0f");
          break ;

        case 12 :
          m_opman.cursorReadoutType(TSLNCursorMovementType.TSLNCursorMovementAngleUUDistance, "Pos : Angle %3.3f   Distance %.0f");
          break ;

        case 13 :
          m_opman.cursorReadoutType(TSLNCursorMovementType.TSLNCursorMovementBearingUUDistance, "Pos : Brg %3.3f   Distance %.0f");
          break ;
      }
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
