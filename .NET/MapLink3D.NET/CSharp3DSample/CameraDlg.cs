using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Envitia.MapLink.ML3D;

namespace CSharp3DSample
{
  public partial class CameraDlg : Form
  {
    private TSLN3DCamera m_camera;

    public CameraDlg(TSLN3DCamera camera)
    {
      m_camera = camera;
      InitializeComponent();
    }

    private void OnNumericTextChanged(object sender, EventArgs e)
    {
      TextBox inputBox = (TextBox)sender;
      int selectionStart = inputBox.SelectionStart;
      int selectionLength = inputBox.SelectionLength;

      string inputText = inputBox.Text;

      for ( int i = 0; i < inputText.Length; ++i )
      {
        char c = inputText[i];
        if (!char.IsDigit(c) && c != '.')
        {
          inputText = inputText.Remove(i, 1);
          if ( selectionStart == -1 )
            continue;

          if (i <= selectionStart)
          {
            --selectionStart;
          }
          else if (i < selectionStart + selectionLength)
          {
            --selectionLength;
          }
        }
      }

      inputBox.Text = inputText;
      inputBox.SelectionStart = selectionStart;
      inputBox.SelectionLength = selectionLength;
    }

    private void ok_button_Click(object sender, EventArgs e)
    {
      double x, y, z, angle, fov;
      try
      {
        x = double.Parse(xBox.Text);
        y = double.Parse(yBox.Text);
        z = double.Parse(zBox.Text);
        angle = double.Parse(raBox.Text);
        fov = double.Parse(fovBox.Text);
      }
      catch
      {
        MessageBox.Show(this, "One of the parameters entered was not valid", "Error");
        return;
      }
      
      // Northings are given by the y-component.
      // Eastings are given by the x-component.
      m_camera.orientation(y, x, z, angle);
      m_camera.setAngleOfView(fov);

      this.DialogResult = DialogResult.OK;
    }
  }
}