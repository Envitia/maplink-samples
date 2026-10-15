using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using Envitia.MapLink;
using Envitia.MapLink.Editor;

namespace CSharpEditorSample
{
  class EditorRequest : TSLNEditorRequest
  {
    private MapViewerForm m_parent;
    private OpManager m_opman;

    public EditorRequest(MapViewerForm parent, OpManager opman)
    {
      m_parent = parent;
      m_opman = opman;
    }

    public override bool displayError(string message)
    {
      //Show a message box
      MessageBox.Show(m_parent, message, "CSharp Editor Sample", MessageBoxButtons.OK, MessageBoxIcon.Error);
      return true;
    }

    public override bool displayPrompt(string message)
    {
      //Tell the parent form to update its status bar
      if (m_parent != null)
      {
        m_parent.Prompt = message;
      }
      return true;
    }

    public override bool onCursorMovement(string displayString, double val1, double val2)
    {
      //Tell the parent form to update its status bar
      if (m_parent != null)
      {
        m_parent.CursorReadout = displayString;
      }
      return true;
    }

    public override bool onOperationDeactivated()
    {
      //Reactivate the default select operation
      if (m_opman != null)
      {
        m_opman.select();
      }
      //Update the menu and toolstrips 
      if (m_parent != null)
      {
        m_parent.UpdateCheckedEditorButtons();
      }
      return true;
    }

    public override bool onSelectionChanged(int numEntities, int depth)
    {
      //Tell the parent form to update its status bar
      if (m_parent != null)
      {
        m_parent.SelectedEntityCount = numEntities;
      }
      return true;
    }

    public override bool requestDialog(string prompt, string label1, string label2, string label3, string label4)
    {
      //Show a dialog to the user so they can choose an option (1, 2, 3, 4 or -1).
      //This can be triggered from e.g. lasso operation.
      RequestChoiceDialog dlg = new RequestChoiceDialog();
      int choice = dlg.SetupAndShowModal(m_parent, prompt, label1, label2, label3, label4);
      m_opman.dialogEntered(choice);
      return true;
    }

    public override bool requestText(string prompt, string initialValue)
    {
      //Show a dialog to the user so they can enter some text
      RequestTextDialog dlg = new RequestTextDialog(prompt, initialValue);
      bool canceled = dlg.ShowDialog(m_parent) != DialogResult.OK;
      m_opman.textEntered(dlg.m_value, canceled);
      return true;
    }

  }
}
