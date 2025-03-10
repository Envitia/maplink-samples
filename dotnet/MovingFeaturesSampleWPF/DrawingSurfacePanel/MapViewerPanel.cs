using System;
using System.Linq;

namespace DrawingSurfacePanel
{
  using static System.Windows.Forms.VisualStyles.VisualStyleElement;
#if USEOPENGLSURFACE
  using DrawingSurfaceClass = Envitia.MapLink.OpenGLSurface.TSLNOpenGLSurface;
#else
  using DrawingSurfaceClass = Envitia.MapLink.TSLNDrawingSurface;
#endif

  /// <summary>
  /// Summary description for drawingSurfacePanel.
  /// </summary>
  [System.Runtime.Versioning.SupportedOSPlatform("windows")]
  public class MapViewerPanel : System.Windows.Forms.Panel, IPanel
  {
    public IPanel GetIPanel() { return this; }

    #region Panel_Fields
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

    
    public DrawingSurfaceClass DrawingSurface { get; set; }
    
    //Variables to hold if the control/shift button are currently pressed
    private bool ControlButtonDown { get; set; } = false;
    private bool ShiftButtonDown { get; set; } = false;

    // context menu when right click is clicked
    System.Windows.Forms.ContextMenuStrip PanelContextMenu { get; set; } = null;
    public System.Windows.Forms.ToolTip ToolTip { get; } = new System.Windows.Forms.ToolTip();

    public TooltipTextProvider TooltipTextProvider { get; set; }

    public bool DarkMode { get; set; } = true;
    #endregion

    #region Panel_Contructors

    public MapViewerPanel()
    {
      InitializeComponent();
      //! Set Maplink Home directory here ...
      string maplHome = Envitia.MapLink.TSLNUtilityFunctions.getMapLinkHome();
      if (string.IsNullOrEmpty(maplHome))
      {
        Envitia.MapLink.TSLNUtilityFunctions.setMapLinkHome("../");
      }

      //! Initialise drawing surface
      Envitia.MapLink.TSLNDrawingSurface.loadStandardConfig();

      InitializePanel();
    }

    ~MapViewerPanel()
    {
      CleanUp();
    }
    #endregion

    #region Panel_Controls

    private MapViewerParentPanel ViewerPanel;

    private void InitializeComponent()
    {
      //! define the panel's controls
      this.ViewerPanel = new MapViewerParentPanel();
      this.SuspendLayout();

      // 
      // ViewerPanel
      // 
      this.ViewerPanel.Dock = System.Windows.Forms.DockStyle.Fill;
      this.ViewerPanel.Location = new System.Drawing.Point(0, 35);
      this.ViewerPanel.Name = "ViewerPanel";
      this.ViewerPanel.Size = new System.Drawing.Size(783, 495);
      this.ViewerPanel.TabIndex = 2;
      //! override interaction functions
      this.ViewerPanel.Paint += new System.Windows.Forms.PaintEventHandler(this.OnPaint);
      this.ViewerPanel.Resize += new System.EventHandler(this.OnReSize);
      this.ViewerPanel.HandleDestroyed += new System.EventHandler(this.OnDestroyed);
      this.ViewerPanel.MouseDown += new System.Windows.Forms.MouseEventHandler(this.OnMouseDown);
      this.ViewerPanel.MouseMove += new System.Windows.Forms.MouseEventHandler(this.OnMouseMove);
      this.ViewerPanel.MouseUp += new System.Windows.Forms.MouseEventHandler(this.OnMouseUp);
      this.ViewerPanel.MouseWheel += new System.Windows.Forms.MouseEventHandler(this.OnMouseWheel);

      this.ViewerPanel.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.OnMouseDoubleClick);
      this.ViewerPanel.KeyDown += new System.Windows.Forms.KeyEventHandler(this.OnKeyDown);
      this.ViewerPanel.KeyUp += new System.Windows.Forms.KeyEventHandler(this.OnKeyUp);

      this.ViewerPanel.MouseLeave += ViewerPanel_MouseLeave;
      ToolTip.BackColor = System.Drawing.Color.FromArgb(100, 230, 230, 230);

      //! Add controls to the panel
      this.Controls.Add(this.ViewerPanel);
      this.ResumeLayout(false);
    }

    private void ViewerPanel_MouseLeave(object sender, EventArgs e)
    {
      ToolTip.Hide(ViewerPanel);
    }
    #endregion

    #region Panel_Basic_Functions
    //Paint of the ViewerPanel
    private void OnPaint(object sender, System.Windows.Forms.PaintEventArgs e)
    {
      const int leftOffset = 0;
      const int bottomOffset = 0;

      DrawingSurface?.drawDU(e.ClipRectangle.Left + leftOffset, e.ClipRectangle.Top, e.ClipRectangle.Right, e.ClipRectangle.Bottom - bottomOffset, false, false);

      // Don't forget to draw any echo rectangle that may be active.
      ModeManager?.onDraw(e.ClipRectangle.Left + leftOffset, e.ClipRectangle.Top, e.ClipRectangle.Right, e.ClipRectangle.Bottom - bottomOffset);
    }

    //Resize of the ViewerPanel
    private void OnReSize(object sender, System.EventArgs e)
    {
      if (DrawingSurface != null)
      {
        DrawingSurface.wndResize(ViewerPanel.DisplayRectangle.Left, ViewerPanel.DisplayRectangle.Top, ViewerPanel.DisplayRectangle.Right, ViewerPanel.DisplayRectangle.Bottom, false, Envitia.MapLink.TSLNResizeActionEnum.TSLNResizeActionMaintainCentre);
      }
      if (ModeManager != null)
      {
        ModeManager.onSize(ViewerPanel.DisplayRectangle.Width, ViewerPanel.DisplayRectangle.Height);
      }

      // Without the invalidate, the paint is only called when the window gets bigger
      ViewerPanel.Invalidate();
    }

    private bool UiHandlersLeftClick(bool shiftDown, bool ctrlDown, int x, int y)
    {
      foreach (var handler in UiHandlers)
      {
        if (handler.OnLeftButtonClick(shiftDown, ctrlDown, x, y))
          return true;
      }
      return false;
    }

    private bool UiHandlersRightClick(bool shiftDown, bool ctrlDown, int x, int y)
    {
      foreach (var handler in UiHandlers)
      {
        if (handler.OnRightButtonClick(shiftDown, ctrlDown, x, y))
          return true;
      }
      return false;
    }

    private bool UiHandlersLeftButtonDoubleClick(bool shiftDown, bool ctrlDown, int x, int y)
    {
      foreach (var handler in UiHandlers)
      {
        if (handler.OnLeftButtonDoubleClick(shiftDown, ctrlDown, x, y))
          return true;
      }
      return false;
    }

    //On mouse down on the ViewerPanel
    private void OnMouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
    {
      if (ModeManager == null)
      {
        return;
      }

      bool invalidate = false;
      bool isControlPressed = (System.Windows.Forms.Control.ModifierKeys & System.Windows.Forms.Keys.Control) == System.Windows.Forms.Keys.Control;
      bool isShiftPressed = (System.Windows.Forms.Control.ModifierKeys & System.Windows.Forms.Keys.Shift) == System.Windows.Forms.Keys.Shift;

      switch (e.Button)
      {
        case System.Windows.Forms.MouseButtons.Left:
          {
            if (UiHandlersLeftClick(isShiftPressed, isControlPressed, e.X, e.Y))
            {
              invalidate = true;
              break;
            }
            invalidate = ModeManager.onLButtonDown(e.X, e.Y, isShiftPressed, isControlPressed);
            break;
          }
        case System.Windows.Forms.MouseButtons.Middle:
          { 
            invalidate = ModeManager.onMButtonDown(e.X, e.Y, isShiftPressed, isControlPressed);
            break;
          }
        case System.Windows.Forms.MouseButtons.Right:
          {
            if (UiHandlersRightClick(isShiftPressed, isControlPressed, e.X, e.Y))
            {
              invalidate = true;
              break;
            }
            invalidate = ModeManager.onRButtonDown(e.X, e.Y, isShiftPressed, isControlPressed);
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
      if (ModeManager == null)
      {
        return;
      }

      this.Cursor = System.Windows.Forms.Cursors.Default;

      Envitia.MapLink.TSLNButtonType button = Envitia.MapLink.TSLNButtonType.TSLNButtonNone;

      if (e.Button == System.Windows.Forms.MouseButtons.Left)
        button = Envitia.MapLink.TSLNButtonType.TSLNButtonLeft;
      else if (e.Button == System.Windows.Forms.MouseButtons.Middle)
        button = Envitia.MapLink.TSLNButtonType.TSLNButtonCentre;
      else if (e.Button == System.Windows.Forms.MouseButtons.Right)
        button = Envitia.MapLink.TSLNButtonType.TSLNButtonRight;

      bool isControlPressed = (System.Windows.Forms.Control.ModifierKeys & System.Windows.Forms.Keys.Control) == System.Windows.Forms.Keys.Control;
      bool isShiftPressed = (System.Windows.Forms.Control.ModifierKeys & System.Windows.Forms.Keys.Shift) == System.Windows.Forms.Keys.Shift;

      if (ModeManager.onMouseMove(button, e.X, e.Y, isShiftPressed, isControlPressed))
      {
        // Request a redraw if the interaction hander requires it
        ViewerPanel.Invalidate();
      }

      if (TooltipTextProvider is not null)
      {
        if (TooltipTextProvider.Panel is null)
        {
          TooltipTextProvider.Panel = this;
        }
        if (TooltipTextProvider.ToolTip is null)
        {
          TooltipTextProvider.ToolTip = ToolTip;
        }
        TooltipTextProvider.Start(e.X, e.Y);
      }
    }

    //On mouse up on the ViewerPanel
    private void OnMouseUp(object sender, System.Windows.Forms.MouseEventArgs e)
    {
      if (ModeManager == null)
      {
        return;
      }

      bool isControlPressed = (System.Windows.Forms.Control.ModifierKeys & System.Windows.Forms.Keys.Control) == System.Windows.Forms.Keys.Control;
      bool isShiftPressed = (System.Windows.Forms.Control.ModifierKeys & System.Windows.Forms.Keys.Shift) == System.Windows.Forms.Keys.Shift;

      bool invalidate = false;
      switch (e.Button)
      {
        case System.Windows.Forms.MouseButtons.Left:
          {
            invalidate = ModeManager.onLButtonUp(e.X, e.Y, isShiftPressed, isControlPressed);
            break;
          }
        case System.Windows.Forms.MouseButtons.Middle:
          {
            invalidate = ModeManager.onMButtonUp(e.X, e.Y, isShiftPressed, isControlPressed);
            break;
          }
        case System.Windows.Forms.MouseButtons.Right:
          {
            invalidate = ModeManager.onRButtonUp(e.X, e.Y, isShiftPressed, isControlPressed);
            break;
          }
      }

      if (invalidate)
      {
        ViewerPanel.Invalidate();
      }
    }

    private void OnMouseDoubleClick(object sender, System.Windows.Forms.MouseEventArgs e)
    {
      if (ModeManager == null)
      {
        return;
      }

      bool isControlPressed = (System.Windows.Forms.Control.ModifierKeys & System.Windows.Forms.Keys.Control) == System.Windows.Forms.Keys.Control;
      bool isShiftPressed = (System.Windows.Forms.Control.ModifierKeys & System.Windows.Forms.Keys.Shift) == System.Windows.Forms.Keys.Shift;

      UiHandlersLeftButtonDoubleClick(isShiftPressed, isControlPressed, e.X, e.Y);
    }

    //On mouse wheel on the ViewerPanel
    private void OnMouseWheel(object sender, System.Windows.Forms.MouseEventArgs e)
    {
      if (ModeManager == null)
      {
        return;
      }
      if (ModeManager.onMouseWheel((short)e.Delta, e.X, e.Y))
      {
        ViewerPanel.Invalidate();
      }
    }

    private void OnMouseLeave(object sender, System.Windows.Forms.MouseEventArgs e)
    {
      ToolTip.Hide(ViewerPanel);
    }

    //On key down anywhere in the app
    private void OnKeyDown(object sender, System.Windows.Forms.KeyEventArgs e)
    {
      if (ModeManager == null)
      {
        return;
      }

      if (e.KeyCode == System.Windows.Forms.Keys.ControlKey)
      {
        ControlButtonDown = true;
      }
      else if (e.Shift == true)
      {
        ShiftButtonDown = true;
      }
    }

    //On key up anywhere in the app
    private void OnKeyUp(object sender, System.Windows.Forms.KeyEventArgs e)
    {
      if (ModeManager == null)
      {
        return;
      }

      if (ControlButtonDown && e.KeyCode == System.Windows.Forms.Keys.ControlKey)
      {
        ControlButtonDown = false;
      }
      else if (ShiftButtonDown && e.Shift == false)
      {
        ShiftButtonDown = false;
        // activate grab mode
        SetCurrentMode(InteractionModeEnum.TOOLS_GRAB);
      }
    }

    private void OnDestroyed(object sender, System.EventArgs e)
    {
      CleanUp();
    }

    /// <summary>
    /// handles when an option is chosen from the right click context menu.
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void InteractionModeChanged(InteractionMode newMode)
    {
      if (CurrentInteractionMode != newMode.mode)
      {
        SetCurrentMode(newMode.mode);
      }

    }

    #endregion

    #region Panel_Functions
    public void InitializePanel()
    {
      bool status = DrawingSurfaceClass.loadStandardConfig();
#if USEOPENGLSURFACE
          TSLNOpenGLSurfaceCreationParameters creationOptions = new TSLNOpenGLSurfaceCreationParameters();
          creationOptions.createCoreProfile(true);
          creationOptions.enableDoubleBuffering(true);
          DrawingSurface = new TSLNWGLSurface(ViewerPanel.Handle, false, creationOptions);      
#else
      DrawingSurface = new DrawingSurfaceClass((IntPtr)ViewerPanel.Handle, false);
      DrawingSurface.setOption(Envitia.MapLink.TSLNOptionEnum.TSLNOptionDoubleBuffered, true);
      DrawingSurface.setOption(Envitia.MapLink.TSLNOptionEnum.TSLNOptionAntiAliasMonoRasters, true);
#endif

      UpdateReceiver = new InteractionModeRequestReceiver();
      // Attach the drawing surface to the form and set it's initial size...
      // This must be done before the drawing surface is given to the
      // InteractionModeManager
      DrawingSurface.attach(ViewerPanel.Handle, false);

      // Create our user defined request receiver
      Envitia.MapLink.InteractionModes.TSLNInteractionModeRequest request = (Envitia.MapLink.InteractionModes.TSLNInteractionModeRequest)UpdateReceiver;

      // Now create the mode manager
      ModeManager = new Envitia.MapLink.InteractionModes.TSLNInteractionModeManagerGeneric(request, DrawingSurface, 5, 5, 30, true);

      // Next create our modes
      InitializeInteractionModes();

      //Give them to the interaction mode manager
      //Remember we don't own the modes anymore after this point
      //but we must keep a reference to them or c# won't deleted them properly
      InteractionModes.ForEach(mode =>
      {
        ModeManager.addMode(mode.interactionMode, false);
      });

      DrawingSurface.wndResize(ViewerPanel.DisplayRectangle.Left, ViewerPanel.DisplayRectangle.Top, ViewerPanel.DisplayRectangle.Right, ViewerPanel.DisplayRectangle.Bottom, false, Envitia.MapLink.TSLNResizeActionEnum.TSLNResizeActionMaintainCentre);
      ModeManager.onSize(ViewerPanel.DisplayRectangle.Width, ViewerPanel.DisplayRectangle.Height);

      DrawingSurface.setOption(Envitia.MapLink.TSLNOptionEnum.TSLNOptionDoubleBuffered, true);

      //activate grab imode
      SetCurrentMode(InteractionModeEnum.TOOLS_GRAB);

      DrawingSurface.reset(false);
    }

    private void Clear()
    {
      // Clear any data layers currently on the drawing surface
      while (DrawingSurface.numDataLayers > 0)
      {
        DrawingSurface.getDataLayerInfo(0, out Envitia.MapLink.TSLNDataLayer dataLayer, out string layerName);
        DrawingSurface.removeDataLayer(layerName);
      }
    }

    public DrawingSurfacePanel.IMapLayer FoundationLayer { get; private set; }

    public void SetFoundationLayer(DrawingSurfacePanel.IMapLayer foundationLayer)
    {
      Clear();

      FoundationLayer = foundationLayer;

      DrawingSurface.addDataLayer(foundationLayer.GetDataLayer(), foundationLayer.Identifier());
      DrawingSurface.setCoordinateProvidingLayer(foundationLayer.Identifier());
      foundationLayer.ConfigureMapLayer(DrawingSurface);

      var uiHandlers = foundationLayer.GetUiHandlers(DrawingSurface);
      if (uiHandlers is not null)
      {
        UiHandlers.AddRange(uiHandlers);
      }

      DrawingSurface.reset();
    }

    public string AddGridLines(GridType gridType, Envitia.MapLink.TSLNDataLayer dataLayer)
    {
      return gridType == GridType.GARS ? AddGarsGridLines(dataLayer) : AddLatLonGridLines(dataLayer);
    }

    private string AddGarsGridLines(Envitia.MapLink.TSLNDataLayer mapLayer)
    {
      GarsGridLayer.setDataLayer(mapLayer);
      var lineColour = DarkMode ? System.Drawing.Color.SlateGray.ToArgb() : System.Drawing.Color.LightSlateGray.ToArgb();
      var textColour = DarkMode ? System.Drawing.Color.WhiteSmoke.ToArgb() : System.Drawing.Color.LightSlateGray.ToArgb();
      GarsGridLayer.setFeatureRendering("GARSLonBand", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeColour, lineColour);
      GarsGridLayer.setFeatureRendering("GARSLonQuad", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeColour, lineColour);
      GarsGridLayer.setFeatureRendering("GARSLonArea", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeColour, lineColour);
      GarsGridLayer.setFeatureRendering("GARSLatBand", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeColour, lineColour);
      GarsGridLayer.setFeatureRendering("GARSLatQuad", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeColour, lineColour);
      GarsGridLayer.setFeatureRendering("GARSLatArea", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeColour, lineColour);

      GarsGridLayer.setFeatureRendering("GARSLonBand", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeStyle, 1);
      GarsGridLayer.setFeatureRendering("GARSLonQuad", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeStyle, 2);
      GarsGridLayer.setFeatureRendering("GARSLonArea", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeStyle, 3);
      GarsGridLayer.setFeatureRendering("GARSLatBand", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeStyle, 1);
      GarsGridLayer.setFeatureRendering("GARSLatQuad", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeStyle, 2);
      GarsGridLayer.setFeatureRendering("GARSLatArea", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeStyle, 3);

      GarsGridLayer.setFeatureRendering("GARSLonBand", 0, Envitia.MapLink.TSLNRenderingAttributeDouble.TSLNRenderingAttributeEdgeThickness, 1.0);
      GarsGridLayer.setFeatureRendering("GARSLonQuad", 0, Envitia.MapLink.TSLNRenderingAttributeDouble.TSLNRenderingAttributeEdgeThickness, 1.0);
      GarsGridLayer.setFeatureRendering("GARSLonArea", 0, Envitia.MapLink.TSLNRenderingAttributeDouble.TSLNRenderingAttributeEdgeThickness, 1.0);
      GarsGridLayer.setFeatureRendering("GARSLatBand", 0, Envitia.MapLink.TSLNRenderingAttributeDouble.TSLNRenderingAttributeEdgeThickness, 1.0);
      GarsGridLayer.setFeatureRendering("GARSLatQuad", 0, Envitia.MapLink.TSLNRenderingAttributeDouble.TSLNRenderingAttributeEdgeThickness, 1.0);
      GarsGridLayer.setFeatureRendering("GARSLatArea", 0, Envitia.MapLink.TSLNRenderingAttributeDouble.TSLNRenderingAttributeEdgeThickness, 1.0);

      GarsGridLayer.setFeatureRendering("GARSLonBand", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeThicknessUnits, (int)Envitia.MapLink.TSLNDimensionUnits.TSLNDimensionUnitsPixels);
      GarsGridLayer.setFeatureRendering("GARSLonQuad", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeThicknessUnits, (int)Envitia.MapLink.TSLNDimensionUnits.TSLNDimensionUnitsPixels);
      GarsGridLayer.setFeatureRendering("GARSLonArea", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeThicknessUnits, (int)Envitia.MapLink.TSLNDimensionUnits.TSLNDimensionUnitsPixels);
      GarsGridLayer.setFeatureRendering("GARSLatBand", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeThicknessUnits, (int)Envitia.MapLink.TSLNDimensionUnits.TSLNDimensionUnitsPixels);
      GarsGridLayer.setFeatureRendering("GARSLatQuad", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeThicknessUnits, (int)Envitia.MapLink.TSLNDimensionUnits.TSLNDimensionUnitsPixels);
      GarsGridLayer.setFeatureRendering("GARSLatArea", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeThicknessUnits, (int)Envitia.MapLink.TSLNDimensionUnits.TSLNDimensionUnitsPixels);

      GarsGridLayer.setFeatureRendering("GARSLabel", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeTextFont, 1);
      GarsGridLayer.setFeatureRendering("GARSLabel", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeTextColour, textColour);
      GarsGridLayer.setFeatureRendering("GARSLabel", 0, Envitia.MapLink.TSLNRenderingAttributeDouble.TSLNRenderingAttributeTextSizeFactor, 16);
      GarsGridLayer.setFeatureRendering("GARSLabel", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeTextSizeFactorUnits, (int)Envitia.MapLink.TSLNDimensionUnits.TSLNDimensionUnitsPixels);

      string layerName = "GarsGrid";
      DrawingSurface.addDataLayer(GarsGridLayer, layerName);
      DrawingSurface.setDataLayerProps(layerName, Envitia.MapLink.TSLNPropertyEnum.TSLNPropertyVisible, 1);
      DrawingSurface.bringToFront("GarsGrid");
      DrawingSurface.redraw();

      return layerName;
    }

    private string AddLatLonGridLines(Envitia.MapLink.TSLNDataLayer mapLayer)
    {
      Envitia.MapLink.TSLNLatLongGridDataLayer LatLonGridLayer = new Envitia.MapLink.TSLNLatLongGridDataLayer();

      LatLonGridLayer.setDataLayer(mapLayer);
      var lineColour = DarkMode ? System.Drawing.Color.SlateGray.ToArgb() : System.Drawing.Color.LightSlateGray.ToArgb();
      var textColour = DarkMode ? System.Drawing.Color.Lime.ToArgb() : System.Drawing.Color.LimeGreen.ToArgb();
      LatLonGridLayer.setFeatureRendering("lonMajorDegree", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeColour, lineColour);
      LatLonGridLayer.setFeatureRendering("lonDegree", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeColour, lineColour);
      LatLonGridLayer.setFeatureRendering("lonMinute", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeColour, lineColour);
      LatLonGridLayer.setFeatureRendering("lonTenths", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeColour, lineColour);
      LatLonGridLayer.setFeatureRendering("latMajorDegree", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeColour, lineColour);
      LatLonGridLayer.setFeatureRendering("latDegree", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeColour, lineColour);
      LatLonGridLayer.setFeatureRendering("latMinute", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeColour, lineColour);
      LatLonGridLayer.setFeatureRendering("latTenths", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeColour, lineColour);

      LatLonGridLayer.setFeatureRendering("lonMajorDegree", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeStyle, 1);
      LatLonGridLayer.setFeatureRendering("lonDegree", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeStyle, 2);
      LatLonGridLayer.setFeatureRendering("lonMinute", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeStyle, 3);
      LatLonGridLayer.setFeatureRendering("lonTenths", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeStyle, 0);
      LatLonGridLayer.setFeatureRendering("latMajorDegree", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeStyle, 1);
      LatLonGridLayer.setFeatureRendering("latDegree", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeStyle, 2);
      LatLonGridLayer.setFeatureRendering("latMinute", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeStyle, 3);
      LatLonGridLayer.setFeatureRendering("latTenths", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeStyle, 0);

      LatLonGridLayer.setFeatureRendering("lonMajorDegree", 0, Envitia.MapLink.TSLNRenderingAttributeDouble.TSLNRenderingAttributeEdgeThickness, 1.0);
      LatLonGridLayer.setFeatureRendering("lonDegree", 0, Envitia.MapLink.TSLNRenderingAttributeDouble.TSLNRenderingAttributeEdgeThickness, 1.0);
      LatLonGridLayer.setFeatureRendering("lonMinute", 0, Envitia.MapLink.TSLNRenderingAttributeDouble.TSLNRenderingAttributeEdgeThickness, 1.0);
      LatLonGridLayer.setFeatureRendering("lonTenths", 0, Envitia.MapLink.TSLNRenderingAttributeDouble.TSLNRenderingAttributeEdgeThickness, 1.0);
      LatLonGridLayer.setFeatureRendering("latMajorDegree", 0, Envitia.MapLink.TSLNRenderingAttributeDouble.TSLNRenderingAttributeEdgeThickness, 1.0);
      LatLonGridLayer.setFeatureRendering("latDegree", 0, Envitia.MapLink.TSLNRenderingAttributeDouble.TSLNRenderingAttributeEdgeThickness, 1.0);
      LatLonGridLayer.setFeatureRendering("latMinute", 0, Envitia.MapLink.TSLNRenderingAttributeDouble.TSLNRenderingAttributeEdgeThickness, 1.0);
      LatLonGridLayer.setFeatureRendering("latTenths", 0, Envitia.MapLink.TSLNRenderingAttributeDouble.TSLNRenderingAttributeEdgeThickness, 1.0);

      LatLonGridLayer.setFeatureRendering("lonMajorDegree", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeThicknessUnits, (int)Envitia.MapLink.TSLNDimensionUnits.TSLNDimensionUnitsPixels);
      LatLonGridLayer.setFeatureRendering("lonDegree", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeThicknessUnits, (int)Envitia.MapLink.TSLNDimensionUnits.TSLNDimensionUnitsPixels);
      LatLonGridLayer.setFeatureRendering("lonMinute", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeThicknessUnits, (int)Envitia.MapLink.TSLNDimensionUnits.TSLNDimensionUnitsPixels);
      LatLonGridLayer.setFeatureRendering("lonTenths", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeThicknessUnits, (int)Envitia.MapLink.TSLNDimensionUnits.TSLNDimensionUnitsPixels);
      LatLonGridLayer.setFeatureRendering("latMajorDegree", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeThicknessUnits, (int)Envitia.MapLink.TSLNDimensionUnits.TSLNDimensionUnitsPixels);
      LatLonGridLayer.setFeatureRendering("latDegree", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeThicknessUnits, (int)Envitia.MapLink.TSLNDimensionUnits.TSLNDimensionUnitsPixels);
      LatLonGridLayer.setFeatureRendering("latMinute", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeThicknessUnits, (int)Envitia.MapLink.TSLNDimensionUnits.TSLNDimensionUnitsPixels);
      LatLonGridLayer.setFeatureRendering("latTenths", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeThicknessUnits, (int)Envitia.MapLink.TSLNDimensionUnits.TSLNDimensionUnitsPixels);

      LatLonGridLayer.setFeatureRendering("degreeLineLabel", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeTextFont, 1);
      LatLonGridLayer.setFeatureRendering("degreeLineLabel", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeTextColour, textColour);
      LatLonGridLayer.setFeatureRendering("degreeLineLabel", 0, Envitia.MapLink.TSLNRenderingAttributeDouble.TSLNRenderingAttributeTextSizeFactor, 16);
      LatLonGridLayer.setFeatureRendering("degreeLineLabel", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeTextSizeFactorUnits, (int)Envitia.MapLink.TSLNDimensionUnits.TSLNDimensionUnitsPixels);
      LatLonGridLayer.setFeatureRendering("subgridLineLabel", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeTextFont, 1);
      LatLonGridLayer.setFeatureRendering("subgridLineLabel", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeTextColour, textColour);
      LatLonGridLayer.setFeatureRendering("subgridLineLabel", 0, Envitia.MapLink.TSLNRenderingAttributeDouble.TSLNRenderingAttributeTextSizeFactor, 14);
      LatLonGridLayer.setFeatureRendering("subgridLineLabel", 0, Envitia.MapLink.TSLNRenderingAttributeInt.TSLNRenderingAttributeTextSizeFactorUnits, (int)Envitia.MapLink.TSLNDimensionUnits.TSLNDimensionUnitsPixels);

      string layerName = "LatLonGrid";
      DrawingSurface.addDataLayer(LatLonGridLayer, layerName);
      DrawingSurface.setDataLayerProps(layerName, Envitia.MapLink.TSLNPropertyEnum.TSLNPropertyVisible, 1);
      DrawingSurface.bringToFront("LatLonGrid");
      DrawingSurface.redraw();

      return layerName;
    }

    private void EnsureVisible(Envitia.MapLink.TSLNDataLayer dataLayer, string id)
    {
      var overlay = DrawingSurface.getDataLayer(id);
      if (overlay == null)
      {
        DrawingSurface.addDataLayer(dataLayer, id);
        overlay = dataLayer;
      }

      DrawingSurface.bringToFront(id);
      DrawingSurface.setDataLayerProps(id, Envitia.MapLink.TSLNPropertyEnum.TSLNPropertyVisible, 1);
    }

    public void EnsureGridLineVisible(GridType gridType, string layerName)
    {
      switch (gridType)
      {
        case GridType.GARS:
          EnsureVisible(GarsGridLayer, layerName);
          break;
        case GridType.LATLONG:
          EnsureVisible(LatLongGridLayer, layerName);
          break;
      }
    }

    private void InitializeInteractionModes()
    {
      InteractionModes.Add(new InteractionMode
      {
        interactionMode = new Envitia.MapLink.InteractionModes.TSLNInteractionModeGrab((int)InteractionModeEnum.TOOLS_GRAB, true, "Left button drag move view, Right button click to finish", true),
        mode = InteractionModeEnum.TOOLS_GRAB
      });
    }

    private void CleanUp()
    {
      DrawingSurfaceClass.cleanup();
      DrawingSurface.Dispose();
      DrawingSurface = null;
    }


    [System.Runtime.InteropServices.DllImport("gdi32.dll")]
    private static extern bool
    BitBlt(IntPtr hdcDest, int nXDest, int nYDest, int nWidth, int nHeight, IntPtr hdcSrc, int nXSrc, int nYSrc, int dwRop);

    #endregion

    #region Panel_ZoomPan_Functions
    public bool zoomIn(int zoomPercentage, bool addToViewStack)
    {
      if (ModeManager == null)
      {
        return false;
      }
      bool invalidate = ModeManager.zoomIn(zoomPercentage, addToViewStack);
      if (invalidate)
      {
        ViewerPanel.Invalidate();
      }
      return invalidate;
    }

    public bool zoomOut(int zoomPercentage, bool addToViewStack)
    {
      if (ModeManager == null)
      {
        return false;
      }
      bool invalidate = ModeManager.zoomOut(zoomPercentage, addToViewStack);
      if (invalidate)
      {
        ViewerPanel.Invalidate();
      }
      return invalidate;
    }

    public bool resetToFullExtent(bool addToViewStack)
    {
      if (ModeManager == null)
      {
        return false;
      }
      bool invalidate = ModeManager.resetToFullExtent(addToViewStack);
      if (invalidate)
      {
        ViewerPanel.Invalidate();
      }
      return invalidate;
    }

    public bool SetCurrentMode(InteractionModeEnum mode)
    {
      if (ModeManager == null)
      {
        return false;
      }

      bool invalidate = false;
      var result = InteractionModes.Where(item => item.mode == mode).FirstOrDefault();

      invalidate = ModeManager.setCurrentMode((int)result.mode);

      if (invalidate)
      {
        CurrentInteractionMode = mode;
        ViewerPanel.Invalidate();
      }

      return invalidate;
    }

    public InteractionModeEnum GetCurrentMode()
    {
      if (ModeManager == null)
      {
        return InteractionModeEnum.InValid;
      }

      Envitia.MapLink.InteractionModes.TSLNInteractionMode cmode = null;
      long id = ModeManager.getCurrentMode(out cmode);

      var result = InteractionModes.Where(x => (int)x.mode == id).FirstOrDefault();
      return result.mode;
    }

    #endregion

    #region Panel_StackViews_Functions
    public bool SaveView(int index)
    {
      if (ModeManager == null)
      {
        return false;
      }
      bool invalidate = ModeManager.savedViewSetToCurrent(index);
      return invalidate;
    }

    public bool ViewStackReset()
    {
      if (ModeManager == null)
      {
        return false;
      }
      bool invalidate = ModeManager.savedViewReset();
      return invalidate;
    }

    public bool GotoSavedView(int index)
    {
      if (ModeManager == null)
      {
        return false;
      }
      bool invalidate = ModeManager.savedViewGoto(index);
      if (invalidate)
      {
        ViewerPanel.Invalidate();
      }
      return invalidate;
    }

    public bool ViewStackGotoPrevious()
    {
      if (ModeManager == null)
      {
        return false;
      }
      bool invalidate = ModeManager.viewStackGotoPrevious();
      if (invalidate)
      {
        ViewerPanel.Invalidate();
      }
      return invalidate;
    }

    public bool ViewStackGotoNext()
    {
      if (ModeManager == null)
      {
        return false;
      }
      bool invalidate = ModeManager.viewStackGotoNext();
      if (invalidate)
      {
        ViewerPanel.Invalidate();
      }
      return invalidate;
    }

    #endregion

    public void SafeInvalidate()
    {
      if (this.InvokeRequired)
      {
        Action safeInvoke = delegate { SafeInvalidate(); };
        this.Invoke(safeInvoke);
      }
      else
      {
        Invalidate(true);
      }
    }

    Envitia.MapLink.TSLN2DDrawingSurface IPanel.GetDrawingSurface()
    {
      return DrawingSurface;
    }

    public bool AddLayer(IMapLayer layer, string layerName)
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

    /// <summary>
    /// Removes a given layer from the drawing surface if it is already added to the 
    /// drawing surface
    /// </summary>
    /// <param name="layer"></param>
    /// <param name="layerName"></param>
    /// <returns></returns>
    public bool RemoveLayer(IMapLayer layer, string layerName)
    {
      if (layer is null) return false;

      var loadedLayer = DrawingSurface.getDataLayer(layerName);
      if (loadedLayer != null)
      {
        var uiHandlers = layer.GetUiHandlers(DrawingSurface);
        if (uiHandlers != null)
        {
          UiHandlers.RemoveAll(item => uiHandlers.Any(item2 => (item.Property == item2.Property) && (item.Name == item2.Name)));
        }

        DrawingSurface.removeDataLayer(layerName);
        return true;
      }

      return false;
    }

    public bool HideLayer(IMapLayer layer, string layerName)
    {
      if (layer is null) return false;

      var dataLayer = layer.GetDataLayer();
      if (dataLayer is null) return false;

      var loadedLayer = DrawingSurface.getDataLayer(layerName);
      if (loadedLayer != null)
      {
        var uiHandlers = layer.GetUiHandlers(DrawingSurface);
        if (uiHandlers != null)
        {
          UiHandlers.RemoveAll(item => uiHandlers.Any(item2 => (item.Property == item2.Property) && (item.Name == item2.Name)));
        }
      }

      return true;
    }

    public void RedrawMap()
    {
      DrawingSurface.redraw();
    }
  }
}
