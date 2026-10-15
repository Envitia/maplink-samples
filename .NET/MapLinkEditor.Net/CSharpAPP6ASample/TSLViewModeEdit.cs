using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using Envitia.MapLink;
using Envitia.MapLink.Editor;

namespace CSharpAPP6ASample
{
  class TSLViewModeEdit : TSLViewMode
  {
    public TSLViewModeEdit(TSLViewModeInterface iViewMode, long modeID, MapViewerForm parent, OpManager opman)
      : base(iViewMode, modeID)
    {
      m_allowEdit = true;
      m_requestHandler = new EditorRequest(parent, opman);
      m_editor = new TSLNEditor(m_requestHandler);

      if (!m_editor.initialise( TSLNProfileHelper.getDefaultFilename(), "editor"))
      {
        m_editor = null;
        return ;
      }

      TSLNAllOperations.add( m_editor ) ;

      m_editor.attach( m_drawingSurface ) ;
      m_editor.dataChanged( ) ;

      m_editor.reset() ;

      opman.setEditor(m_editor);
    }

    public override bool OnLButtonDown(int x, int y, bool shift, bool control)
    {
      sendLocator(x, y, TSLNButtonType.TSLNButtonLeft, TSLNLocatorType.TSLNLocatorPress, shift, control);
      return false;
    }

    public override bool OnMButtonDown(int x, int y, bool shift, bool control)
    {
      sendLocator(x, y, TSLNButtonType.TSLNButtonCentre, TSLNLocatorType.TSLNLocatorPress, shift, control);
      return false;
    }

    public override bool OnRButtonDown(int x, int y, bool shift, bool control)
    {
      sendLocator(x, y, TSLNButtonType.TSLNButtonRight, TSLNLocatorType.TSLNLocatorPress, shift, control);
      return false;
    }

    public override bool OnLButtonUp(int x, int y, bool shift, bool control)
    {
      sendLocator(x, y, TSLNButtonType.TSLNButtonLeft, TSLNLocatorType.TSLNLocatorRelease, shift, control);
      return false;
    }

    public override bool OnMButtonUp(int x, int y, bool shift, bool control)
    {
      sendLocator(x, y, TSLNButtonType.TSLNButtonCentre, TSLNLocatorType.TSLNLocatorRelease, shift, control);
      return false;
    }

    public override bool OnRButtonUp(int x, int y, bool shift, bool control)
    {
      sendLocator(x, y, TSLNButtonType.TSLNButtonRight, TSLNLocatorType.TSLNLocatorRelease, shift, control);
      return false;
    }

    public override bool OnMouseMove(MouseButtons button, int x, int y, bool shift, bool control)
    {
      TSLNButtonType editorButton = TSLNButtonType.TSLNButtonNone;

      switch (button)
      {
        case MouseButtons.None:   editorButton = TSLNButtonType.TSLNButtonNone; break;
        case MouseButtons.Left:   editorButton = TSLNButtonType.TSLNButtonLeft; break;
        case MouseButtons.Middle: editorButton = TSLNButtonType.TSLNButtonCentre; break;
        case MouseButtons.Right:  editorButton = TSLNButtonType.TSLNButtonRight; break;
      }
      sendLocator(x, y, editorButton, TSLNLocatorType.TSLNLocatorMotion, shift, control);
      return false;
    }

    //This property is used to indicate whther the user is 
    //allowed to edit the model, and hence whether event 
    //should be passed onto the editor.
    public bool AllowEdit
    {
      get
      {
        return m_allowEdit;
      }
      set
      {
        m_allowEdit = value;
      }
    }

    //This attribute is used decide whether events are 
    //propagated to the editor.
    //
    //It shoud be set based upon the current Editing State - 
    //see associated State Diagram
    private bool m_allowEdit;

    //Utility method to send a locator event to the embedded 
    //editor.
    private void sendLocator(int x, int y, TSLNButtonType button, TSLNLocatorType locator, bool shift, bool control)
    {
      if ( m_editor != null && m_allowEdit )
      {
        TSLNLocatorEvent levent ;

        m_drawingSurface.DUToTMC( x, y, out levent.m_x, out levent.m_y ) ;
     
        levent.m_buttonType  = button ;
        levent.m_locatorType = locator ;

        if ( shift && control )
          levent.m_modifiers = TSLNModifierType.TSLNModifierShiftAndControl ;
        else if ( shift )
          levent.m_modifiers = TSLNModifierType.TSLNModifierShift;
        else if ( control )
          levent.m_modifiers = TSLNModifierType.TSLNModifierControl;
        else
          levent.m_modifiers = TSLNModifierType.TSLNModifierNone;

        m_editor.locator(ref levent);
      }
    }

    //Handler class for feedback from the editor.
    private EditorRequest m_requestHandler;

    //This method contains a handle to the TSLEditor class.
    //
    //It is through this interface that the application
    //invokes LandLink editing functionality.
    private TSLNEditor m_editor;
  }
}
