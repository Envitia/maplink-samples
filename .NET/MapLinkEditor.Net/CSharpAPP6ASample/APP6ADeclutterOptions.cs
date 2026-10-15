using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace CSharpAPP6ASample
{
  public partial class APP6ADeclutterOptions : Form
  {
    public APP6ADeclutterOptions()
    {
      InitializeComponent();
    }

    

    public bool HideFrame
    {
      get
      {
        return this.frameCheckBox.Checked;
      }
      set
      {
        this.frameCheckBox.Checked = value;
      }
    }

    public bool HideGraphics
    {
      get
      {
        return this.graphicsCheckBox.Checked;
      }
      set
      {
        this.graphicsCheckBox.Checked = value;
      }
    }

    public bool HideText
    {
      get
      {
        return this.textCheckBox.Checked;
      }
      set
      {
        this.textCheckBox.Checked = value;
      }
    }
  }
}