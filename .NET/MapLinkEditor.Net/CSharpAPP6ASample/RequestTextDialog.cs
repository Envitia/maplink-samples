using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace CSharpAPP6ASample
{
  public partial class RequestTextDialog : Form
  {
    public RequestTextDialog(string labelText, string initialValue)
    {
      InitializeComponent();
      label1.Text = labelText;
      textBox1.Text = m_value = initialValue;
    }

    public string m_value;

    private void OkButton_Click(object sender, EventArgs e)
    {
      m_value = textBox1.Text;
    }
  }
}