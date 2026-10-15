using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace CSharpEditorSample
{
  /// <summary>
  /// This part of the partial class defines the interaction modes of the form
  /// </summary>
  public partial class MapViewerForm
  {
    //Holds information about each interaction mode
    private class InteractionMode
    {
      //The id
      public int id;
      //The button on the tool bar that activates this mode
      public System.Windows.Forms.ToolStripButton button;
      //The button on the menu bar that activates this mode
      public System.Windows.Forms.ToolStripMenuItem menubutton;
      //The view mode
      public TSLViewMode mode = null;
    }

    //Interaction Mode IDS
    private static readonly int ID_TOOLS_ZOOM = 1;
    private static readonly int ID_TOOLS_PAN = 2;
    private static readonly int ID_TOOLS_GRAB = 3;
    private static readonly int ID_TOOLS_ZOOM_PAN = 4;
    private static readonly int ID_TOOLS_EDIT = 5;

    //Create our array of interaction mode information classes
    private InteractionMode[] m_imodeset = null;

    //Create interaction modes interface
    TSLViewModeInterface m_modeInterface = null;

    //Reference to the current imode
    private TSLViewMode m_currentMode = null;
    private TSLViewMode m_editMode = null;

    //Variables to hold if the control/shift button are currently pressed
    private bool m_controlButtonDown = false;
    private bool m_shiftButtonDown = false;

    //Called from OnLoad
    private void CreateImodes(OpManager opman)
    {
      //Create our storage for the imodes
      m_imodeset = new InteractionMode[5];

      //Create the interface from the imodes to the surface and panel
      m_modeInterface = new TSLViewModeInterface(m_drawingSurface, ViewerPanel.Handle);
      m_modeInterface.setResetCB(resetCB, this);

      m_editMode = new TSLViewModeEdit(m_modeInterface, ID_TOOLS_EDIT, this, opman);

      //Create the mode sets. These are use by the toolbar and the menu to 
      //identify the imodes
      m_imodeset[0] = new InteractionMode();
      m_imodeset[0].id = ID_TOOLS_ZOOM;
      m_imodeset[0].button = btn_zoom;
      m_imodeset[0].menubutton = menu_Tools_Zoommode;
      m_imodeset[0].mode = new TSLViewModeZoom(m_modeInterface, ID_TOOLS_ZOOM, 30.0);
      m_imodeset[1] = new InteractionMode();
      m_imodeset[1].id = ID_TOOLS_PAN;
      m_imodeset[1].button = btn_pan;
      m_imodeset[1].menubutton = menu_Tools_Panmode;
      m_imodeset[1].mode = new TSLViewModePan(m_modeInterface, ID_TOOLS_PAN);
      m_imodeset[2] = new InteractionMode();
      m_imodeset[2].id = ID_TOOLS_GRAB;
      m_imodeset[2].button = btn_grab;
      m_imodeset[2].menubutton = menu_Tools_Grabmode;
      m_imodeset[2].mode = new TSLViewModeGrab(m_modeInterface, ID_TOOLS_GRAB);
      m_imodeset[3] = new InteractionMode();
      m_imodeset[3].id = ID_TOOLS_ZOOM_PAN;
      m_imodeset[3].button = btn_zoomPan;
      m_imodeset[3].menubutton = menu_Tools_Zoompanmode;
      m_imodeset[3].mode = new TSLViewModeZoomPan(m_modeInterface, ID_TOOLS_ZOOM_PAN, 30.0);
      m_imodeset[4] = new InteractionMode();
      m_imodeset[4].id = ID_TOOLS_EDIT;
      m_imodeset[4].button = btn_edit;
      m_imodeset[4].menubutton = menu_Tools_Editmode;
      m_imodeset[4].mode = m_editMode;

      //activate zoom imode
      m_currentMode = m_imodeset[4].mode;
    }

    //This is called when any of the buttons on the Tools ToolbarStrip or Menu are clicked
    private void Imode_ButtonClick(object sender, EventArgs e)
    {
      bool invalidate = false;
      if (sender == btn_zoomIn ||
          sender == menu_Tools_ZoomIn)
      {
        invalidate = m_drawingSurface.zoom(30, true, false);
      }
      else if (sender == btn_zoomOut ||
               sender == menu_Tools_ZoomOut)
      {
        invalidate = m_drawingSurface.zoom(30, false, false);
      }
      else if (sender == btn_reset ||
               sender == menu_Tools_Reset)
      {
        invalidate = m_drawingSurface.reset(true);
      }
      else
      {
        for (int i = 0; i < m_imodeset.Length; ++i)
        {
          if (sender == m_imodeset[i].button || sender == m_imodeset[i].menubutton)
          {
            m_currentMode = m_imodeset[i].mode;
            break;
          }
        }
        UpdateToolsAndMenuButtons();
      }
      if (invalidate)
      {
        ViewerPanel.Invalidate();
      }
    }

    //On mouse down on the ViewerPanel
    private void OnMouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
    {
      if (m_currentMode == null)
        return;

      bool invalidate = false;

      switch (e.Button)
      {
        case MouseButtons.Left:
          {
            invalidate = m_currentMode.OnLButtonDown(e.X, e.Y, m_shiftButtonDown, m_controlButtonDown);
            break;
          }
        case MouseButtons.Middle:
          {
#if PASS_MIDDLE_BUTTON_TO_MODE
            invalidate = m_currentMode.OnMButtonDown(e.X, e.Y, m_shiftButtonDown, m_controlButtonDown);
#else
            double uux, uuy;
            if (m_drawingSurface.DUToUU(e.X, e.Y, out uux, out uuy))
            {
                if (m_drawingSurface.pan(uux, uuy, false))
                    invalidate = true;
            }
#endif
            break;
          }
        case MouseButtons.Right:
          {
            invalidate = m_currentMode.OnRButtonDown(e.X, e.Y, m_shiftButtonDown, m_controlButtonDown);
            break;
          }
      }
      if (invalidate)
      {
        ViewerPanel.Invalidate();
      }
    }

    //On mouse move on the ViewerPanel
    private void OnMouseMove(object sender, System.Windows.Forms.MouseEventArgs e)
    {
      if (m_currentMode == null)
        return;

      if (m_currentMode.OnMouseMove(e.Button, e.X, e.Y, m_shiftButtonDown, m_controlButtonDown))
      {
        // Request a redraw if the interaction hander requires it
        ViewerPanel.Invalidate();
      }
    }

    //On mouse up on the ViewerPanel
    private void OnMouseUp(object sender, System.Windows.Forms.MouseEventArgs e)
    {
      if (m_currentMode == null)
        return;

      bool invalidate = false;
      switch (e.Button)
      {
        case MouseButtons.Left:
          {
            invalidate = m_currentMode.OnLButtonUp(e.X, e.Y, m_shiftButtonDown, m_controlButtonDown);
            break;
          }
        case MouseButtons.Middle:
          {
            invalidate = m_currentMode.OnMButtonUp(e.X, e.Y, m_shiftButtonDown, m_controlButtonDown);
            break;
          }
        case MouseButtons.Right:
          {
            invalidate = m_currentMode.OnRButtonUp(e.X, e.Y, m_shiftButtonDown, m_controlButtonDown);
            break;
          }
      }
      UpdateToolsAndMenuButtons();
      if (invalidate)
      {
        ViewerPanel.Invalidate();
      }
    }

    //On mouse wheel on the ViewerPanel
    private void OnMouseWheel(object sender, System.Windows.Forms.MouseEventArgs e)
    {
      if (m_currentMode == null)
        return;

      if (m_currentMode.OnMouseWheel(e.Delta, e.X, e.Y))
      {
        ViewerPanel.Invalidate();
      }
    }

    //On key down anywhere in the app
    private void OnKeyDown(object sender, System.Windows.Forms.KeyEventArgs e)
    {
      if (e.Control == true)
      {
        m_controlButtonDown = true;
      }
      else if (e.Shift == true)
      {
        m_shiftButtonDown = true;
      }
    }

    //On key up anywhere in the app
    private void OnKeyUp(object sender, System.Windows.Forms.KeyEventArgs e)
    {
      if (m_controlButtonDown && e.Control == false)
      {
        m_controlButtonDown = false;
      }
      else if (m_shiftButtonDown && e.Shift == false)
      {
        m_shiftButtonDown = false;
      }
    }

    //called whenever a new imode is selected
    private void UpdateToolsAndMenuButtons()
    {
      if (m_currentMode == null)
        return;

      int id = m_currentMode.id;

      for (int i = 0; i < m_imodeset.Length; ++i)
      {
        if (id == m_imodeset[i].id)
        {
          m_imodeset[i].button.Checked = true;
          m_imodeset[i].menubutton.Checked = true;
        }
        else
        {
          m_imodeset[i].button.Checked = false;
          m_imodeset[i].menubutton.Checked = false;
        }
      }
    }

    static void resetCB(Object self)
    {
      // This method is triggered when a drawing mode wants to deactivate itself.
      MapViewerForm pthis = (MapViewerForm)self;

      if (pthis.m_currentMode != pthis.m_editMode)
      {
        pthis.m_currentMode = pthis.m_editMode;
      }
    }
  }
}
