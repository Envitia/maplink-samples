using DrawingSurfacePanel;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Text;

namespace MovingFeaturesSample.DrawingSurface
{
  internal class DrawingSurfaceWrapper
  {
    public enum InteractionModeEnum
    {
      InValid = -1,

      TOOLS_GRAB,
    }

    public enum GridType
    {
      GARS,
      LATLONG
    }

    struct InteractionMode
    {
      public Envitia.MapLink.InteractionModes.TSLNInteractionMode interactionMode;
      public InteractionModeEnum mode;
    }

    System.Collections.Generic.List<InteractionMode> InteractionModes { get; set; } = new System.Collections.Generic.List<InteractionMode>();

    private InteractionModeEnum CurrentInteractionMode { get; set; }

    //Create out request receiver pointer
    private InteractionModeRequestReceiver UpdateReceiver { get; set; } = null;

    //Create interaction modes' pointers
    private Envitia.MapLink.InteractionModes.TSLNInteractionModeManagerGeneric ModeManager { get; set; } = null;

    public Envitia.MapLink.TSLNGARSGridDataLayer GarsGridLayer { get; } = new Envitia.MapLink.TSLNGARSGridDataLayer();

    public Envitia.MapLink.TSLNLatLongGridDataLayer LatLongGridLayer { get; } = new Envitia.MapLink.TSLNLatLongGridDataLayer();

    public System.Collections.Generic.List<IUiHandler> UiHandlers { get; } = new System.Collections.Generic.List<IUiHandler>();

    //Variables to hold if the control/shift button are currently pressed
    private bool ControlButtonDown { get; set; } = false;
    private bool ShiftButtonDown { get; set; } = false;

    // context menu when right click is clicked
    System.Windows.Forms.ContextMenuStrip PanelContextMenu { get; set; } = null;
    public System.Windows.Forms.ToolTip ToolTip { get; } = new System.Windows.Forms.ToolTip();

    [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
    public TooltipTextProvider TooltipTextProvider { get; set; }

    [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
    public bool DarkMode { get; set; } = true;

    public Envitia.MapLink.TSLNDrawingSurface DrawingSurface { get; private set; } = null;

    public bool Initialise(Envitia.MapLink.TSLNDrawingSurface drawingSurface)
    {
      if (drawingSurface == null)
        return false;

      DrawingSurface = drawingSurface;

      //Create our request receiver
      UpdateReceiver = new InteractionModeRequestReceiver();

      // Create our user defined request receiver
      Envitia.MapLink.InteractionModes.TSLNInteractionModeRequest request = (Envitia.MapLink.InteractionModes.TSLNInteractionModeRequest)UpdateReceiver;

      // Now create the mode manager
      ModeManager = new Envitia.MapLink.InteractionModes.TSLNInteractionModeManagerGeneric(request, drawingSurface, 5, 5, 30, true);

      // Next create our modes
      InitializeInteractionModes();

      //Give them to the interaction mode manager
      //Remember we don't own the modes anymore after this point
      //but we must keep a reference to them or c# won't deleted them properly
      InteractionModes.ForEach(mode =>
      {
        ModeManager.addMode(mode.interactionMode, false);
      });

      //DrawingSurface.wndResize(ViewerPanel.DisplayRectangle.Left, ViewerPanel.DisplayRectangle.Top, ViewerPanel.DisplayRectangle.Right, ViewerPanel.DisplayRectangle.Bottom, false, Envitia.MapLink.TSLNResizeActionEnum.TSLNResizeActionMaintainCentre);
      //ModeManager.onSize(ViewerPanel.DisplayRectangle.Width, ViewerPanel.DisplayRectangle.Height);

      //DrawingSurface.setOption(Envitia.MapLink.TSLNOptionEnum.TSLNOptionDoubleBuffered, true);

      ////activate grab imode
      //SetCurrentMode(InteractionModeEnum.TOOLS_GRAB);

      //DrawingSurface.reset(false);

      ////Add the interaction modes to the manager
      //InteractionModes.Add(new InteractionMode { interactionMode = new CustomInteractionModeZoomPan((int)InteractionModeEnum.TOOLS_GRAB, true), mode = InteractionModeEnum.TOOLS_GRAB });
      //foreach (var mode in InteractionModes)
      //  ModeManager.addInteractionMode(mode.interactionMode);
      ////Set the default interaction mode
      //CurrentInteractionMode = InteractionModeEnum.TOOLS_GRAB;
      //ModeManager.setCurrentInteractionMode((int)CurrentInteractionMode);
      return true;
    }

    private void InitializeInteractionModes()
    {
      InteractionModes.Add(new InteractionMode
      {
        interactionMode = new Envitia.MapLink.InteractionModes.TSLNInteractionModeGrab((int)InteractionModeEnum.TOOLS_GRAB, true, "Left button drag move view, Right button click to finish", true),
        mode = InteractionModeEnum.TOOLS_GRAB
      });
    }

    public bool AddLayer(MapLayers.MapLayer layer, string layerName)
    {
      if (layer is null) return false;

      var dataLayer = layer.GetDataLayer();
      if (dataLayer is null)
      {
        layer.ConfigureMapLayer(DrawingSurface);
        return false;
      }

      var loadedLayer = DrawingSurface.getDataLayer(layerName);
      if (loadedLayer == null)
      {
        // Lazy load the layer
        if (!DrawingSurface.addDataLayer(layer.GetDataLayer(), layerName))
        {
          return false;
        }
      }

      var uiHandlers = layer.GetUiHandlers(DrawingSurface);
      if (uiHandlers != null)
      {
        UiHandlers.AddRange(uiHandlers);
      }

      return true;
    }
  }
}
