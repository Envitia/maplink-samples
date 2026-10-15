using System;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Text;

namespace CSharpAPP6ASample
{
  [System.ComponentModel.DesignerCategory("Code")]
  class NonDropDownComboBox : ComboBox
  {
    protected override void WndProc(ref Message m)
    {
      if (m.Msg != 513 && m.Msg != 515)
      {
        base.WndProc(ref m);
      }
      else
      {
        this.OnDropDown(new EventArgs());
      }
    }
  }
}
