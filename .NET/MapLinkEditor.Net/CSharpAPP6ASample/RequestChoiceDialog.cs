﻿﻿using System;
using System.Collections.Generic;
using System.Data;
using System.Windows.Forms;

namespace CSharpAPP6ASample
{
  public partial class RequestChoiceDialog : Form
  {
    private Dictionary<DialogResult, int> m_buttonChoiceToEditorEnteredChoice;

    public RequestChoiceDialog()
    {
      InitializeComponent();

      // Values of DialogResult are arbitrary, they just have to uniquely identify the button.
      m_buttonChoiceToEditorEnteredChoice = new Dictionary<DialogResult, int>
      {
        [DialogResult.Cancel] = -1,
        [DialogResult.Abort] = -1,
        [button1.DialogResult] = 1,
        [button2.DialogResult] = 2,
        [button3.DialogResult] = 3,
        [button4.DialogResult] = 4,
      };
      if (m_buttonChoiceToEditorEnteredChoice.Count != 6)
      {
        throw new ConstraintException("buttons' DialogResult incorrect! (all should be different)");
      }
    }

    private void setupButton(Button button, string label)
    {
      if (!string.IsNullOrEmpty(label))
      {
        button.Text = label;
        button.Visible = true;
      }
      else
      {
        button.Text = "";
        button.Visible = false;
      }
    }

    public int SetupAndShowModal(IWin32Window owner, string prompt, string label1, string label2, string label3, string label4)
    {
      textBox1.Text = prompt;
      setupButton(button1, label1);
      setupButton(button2, label2);
      setupButton(button3, label3);
      setupButton(button4, label4);

      DialogResult result = ShowDialog(owner);
      return m_buttonChoiceToEditorEnteredChoice[result];
    }
  }
}