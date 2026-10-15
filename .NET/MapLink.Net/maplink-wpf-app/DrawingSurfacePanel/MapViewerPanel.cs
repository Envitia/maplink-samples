//#define USEOPENGLSURFACE

using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;
using System.Runtime.InteropServices;

using Envitia.MapLink;
using Envitia.MapLink.InteractionModes;
using Envitia.MapLink.OpenGLSurface;

namespace DrawingSurfacePanel
{
#if USEOPENGLSURFACE
  using DrawingSurfaceClass = TSLNOpenGLSurface;
#else
  using DrawingSurfaceClass = TSLNDrawingSurface;
#endif

  /// <summary>
  /// Summary description for drawingSurfacePanel.
  /// </summary>
  public class MapViewerPanel : Panel, IPanel
  {
    public IPanel GetIPanel() { return this; }

    #region Panel_Fields
    #region Panel_InteractionMode_Parameters
    public enum InteractionModeEnum
    {
      InValid = -1,

      TOOLS_GRAB,
      TOOLS_DRAW_LINE
    }

    struct contextMenuTool
    {
      public TSLNInteractionMode interactionMode;
      public InteractionModeEnum mode;
      public string text;
      public bool ischecked;
    }

    contextMenuTool[] m_contextMenuTools;

    private InteractionModeEnum m_currentInteractionMode;

    //Create out request receiver pointer
    private InteractionModeRequestReceiver m_updateReceiver = null;

    //Create interaction modes' pointers
    private TSLNInteractionModeManagerGeneric m_modeManager = null;

    public TSLNStandardDataLayer GeometryLayer { get; } = new TSLNStandardDataLayer();

    #endregion

    //Create the usual MapLink Item pointers
    public DrawingSurfaceClass DrawingSurface { get; set; }

    #endregion

    #region Panel_Contructors

    public MapViewerPanel()
    {
      initializeComponent();
      //! Set Maplink Home directory here ...
      string maplHome = TSLNUtilityFunctions.getMapLinkHome();
      if (string.IsNullOrEmpty(maplHome))
      {
        TSLNUtilityFunctions.setMapLinkHome("../");
      }

      //! Initialise drawing surface
      Envitia.MapLink.TSLNDrawingSurface.loadStandardConfig();
      
      InitializePanel();
    }

    ~MapViewerPanel()
    {
      cleanUp();
    }
    #endregion

    #region Panel_Controls

    private MapViewerParentPanel ViewerPanel;

    private void initializeComponent()
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

      //! Add controls to the panel
      this.Controls.Add(this.ViewerPanel);
      this.ResumeLayout(false);
    }
    #endregion

    #region Panel_Basic_Functions
    //Paint of the ViewerPanel
    private void OnPaint(object sender, System.Windows.Forms.PaintEventArgs e)
    {
      const int leftOffset = 0;
      const int bottomOffset = 0;

      if (DrawingSurface != null)
      {
        DrawingSurface.drawDU(e.ClipRectangle.Left + leftOffset, e.ClipRectangle.Top, e.ClipRectangle.Right, e.ClipRectangle.Bottom - bottomOffset, false, false);
      }

      // Don't forget to draw any echo rectangle that may be active.
      if (m_modeManager != null)
      {
        m_modeManager.onDraw(e.ClipRectangle.Left + leftOffset, e.ClipRectangle.Top, e.ClipRectangle.Right, e.ClipRectangle.Bottom - bottomOffset);
      }
    }

    //Resize of the ViewerPanel
    private void OnReSize(object sender, System.EventArgs e)
    {
      if (DrawingSurface != null)
      {
        DrawingSurface.wndResize(ViewerPanel.DisplayRectangle.Left, ViewerPanel.DisplayRectangle.Top, ViewerPanel.DisplayRectangle.Right, ViewerPanel.DisplayRectangle.Bottom, false, TSLNResizeActionEnum.TSLNResizeActionMaintainCentre);
      }
      if (m_modeManager != null)
      {
        m_modeManager.onSize(ViewerPanel.DisplayRectangle.Width, ViewerPanel.DisplayRectangle.Height);
      }

      // Without the invalidate, the paint is only called when the window gets bigger
      ViewerPanel.Invalidate();
    }

    //On mouse down on the ViewerPanel
    private void OnMouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
    {
      if (m_modeManager == null)
      {
        return;
      }

      bool invalidate = false;

      switch (e.Button)
      {
        case MouseButtons.Left:
          {
            invalidate = m_modeManager.onLButtonDown(e.X, e.Y, false, false);
            break;
          }
        case MouseButtons.Middle:
          {
            invalidate = m_modeManager.onMButtonDown(e.X, e.Y, false, false);
            break;
          }
        case MouseButtons.Right:
          {
            invalidate = m_modeManager.onRButtonDown(e.X, e.Y, false, false);
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
      if (m_modeManager == null)
      {
        return;
      }
      TSLNButtonType button = TSLNButtonType.TSLNButtonNone;

      if (e.Button == MouseButtons.Left)
        button = TSLNButtonType.TSLNButtonLeft;
      else if (e.Button == MouseButtons.Middle)
        button = TSLNButtonType.TSLNButtonCentre;
      else if (e.Button == MouseButtons.Right)
        button = TSLNButtonType.TSLNButtonRight;

      if (m_modeManager.onMouseMove(button, e.X, e.Y, false, false))
      {
        // Request a redraw if the interaction hander requires it
        ViewerPanel.Invalidate();
      }
    }

    //On mouse up on the ViewerPanel
    private void OnMouseUp(object sender, System.Windows.Forms.MouseEventArgs e)
    {
      if (m_modeManager == null)
      {
        return;
      }

      bool invalidate = false;
      switch (e.Button)
      {
        case MouseButtons.Left:
          {
            invalidate = m_modeManager.onLButtonUp(e.X, e.Y, false, false);
            break;
          }
        case MouseButtons.Middle:
          {
            invalidate = m_modeManager.onMButtonUp(e.X, e.Y, false, false);
            break;
          }
        case MouseButtons.Right:
          {
            break;
          }
      }

      if (invalidate)
      {
        ViewerPanel.Invalidate();
      }
    }

    //On mouse wheel on the ViewerPanel
    private void OnMouseWheel(object sender, System.Windows.Forms.MouseEventArgs e)
    {
      if (m_modeManager == null)
      {
        return;
      }
      if (m_modeManager.onMouseWheel((short)e.Delta, e.X, e.Y))
      {
        ViewerPanel.Invalidate();
      }
    }

    private void OnDestroyed(object sender, System.EventArgs e)
    {
      cleanUp();
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
      DrawingSurface.setOption(TSLNOptionEnum.TSLNOptionDoubleBuffered, true);
      DrawingSurface.setOption(TSLNOptionEnum.TSLNOptionAntiAliasMonoRasters, true);
#endif

      m_updateReceiver = new InteractionModeRequestReceiver();
      // Attach the drawing surface to the form and set it's initial size...
      // This must be done before the drawingsurface is given to the
      // InteractionModeManager
      DrawingSurface.attach(ViewerPanel.Handle, false);

      // Create our user defined request receiver
      TSLNInteractionModeRequest request = (TSLNInteractionModeRequest)m_updateReceiver;

      // Now create the mode manager
      m_modeManager = new TSLNInteractionModeManagerGeneric(request, DrawingSurface, 5, 5, 30, true);

      // Next create our modes
      InitializeInteractionModes();

      //Give them to the interaction mode manager
      //Remember we don't own the modes anymore after this point
      //but we must keep a reference to them or c# won't deleted them properly
      foreach (var tool in m_contextMenuTools)
      {
        m_modeManager.addMode(tool.interactionMode, tool.ischecked);
      }

      DrawingSurface.wndResize(ViewerPanel.DisplayRectangle.Left, ViewerPanel.DisplayRectangle.Top, ViewerPanel.DisplayRectangle.Right, ViewerPanel.DisplayRectangle.Bottom, false, TSLNResizeActionEnum.TSLNResizeActionMaintainCentre);
      m_modeManager.onSize(ViewerPanel.DisplayRectangle.Width, ViewerPanel.DisplayRectangle.Height);

      DrawingSurface.setOption(TSLNOptionEnum.TSLNOptionDoubleBuffered, true);
      
      //activate grab imode
      setCurrentMode(InteractionModeEnum.TOOLS_GRAB);

      DrawingSurface.reset(false);
    }

    private void Clear()
    {
      // Clear any data layers currently on the drawing surface
      while (DrawingSurface.numDataLayers > 0)
      {
        TSLNDataLayer dataLayer = null;
        string layerName;
        DrawingSurface.getDataLayerInfo(0, out dataLayer, out layerName);
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

      EnsureInteractionLayersVisible();
      DrawingSurface.reset();
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
      DrawingSurface.setDataLayerProps(id, TSLNPropertyEnum.TSLNPropertyVisible, 1);
    }

    public void EnsureInteractionLayersVisible()
    {
      EnsureVisible(GeometryLayer, "Geometry");
    }  

    private void InitializeInteractionModes()
    {
      m_contextMenuTools = new contextMenuTool[]
      {
        new contextMenuTool { mode = InteractionModeEnum.TOOLS_GRAB, text = "Grab Tool", ischecked = false},
      };

      for (int i = 0; i < m_contextMenuTools.Length; ++i)
      {
        switch (m_contextMenuTools[i].text)
        {
          case "Grab Tool":
            m_contextMenuTools[i].interactionMode = new TSLNInteractionModeGrab((int)InteractionModeEnum.TOOLS_GRAB, true, "Left button drag move view, Right button click to finish", true);
            break;
        }
      }
    }

    private void cleanUp()
    {
      DrawingSurfaceClass.cleanup();
      DrawingSurface.Dispose();
      DrawingSurface = null;
    }
    

    [DllImport("gdi32.dll")]
    private static extern bool
    BitBlt(IntPtr hdcDest, int nXDest, int nYDest, int nWidth, int nHeight, IntPtr hdcSrc, int nXSrc, int nYSrc, int dwRop);

    #endregion

    #region Panel_ZoomPan_Functions

    public bool setCurrentMode(InteractionModeEnum mode)
    {
      if (m_modeManager == null)
      {
        return false;
      }

      bool invalidate = false;
      for (int i = 0; i < m_contextMenuTools.Length; ++i)
      {
        if (mode == m_contextMenuTools[i].mode)
        {
          invalidate = m_modeManager.setCurrentMode((int)mode);
          break;
        }
      }

      if (invalidate)
      {
        m_currentInteractionMode = mode;
        ViewerPanel.Invalidate();
      }

      return invalidate;
    }

    #endregion

    #region Panel_StackViews_Functions
    public bool saveView(int index)
    {
      if (m_modeManager == null)
      {
        return false;
      }
      bool invalidate = m_modeManager.savedViewSetToCurrent(index);
      return invalidate;
    }

    public bool viewStackReset()
    {
      if (m_modeManager == null)
      {
        return false;
      }
      bool invalidate = m_modeManager.savedViewReset();
      return invalidate;
    }

    public bool gotoSavedView(int index)
    {
      if (m_modeManager == null)
      {
        return false;
      }
      bool invalidate = m_modeManager.savedViewGoto(index);
      if (invalidate)
      {
        ViewerPanel.Invalidate();
      }
      return invalidate;
    }

    public bool viewStackGotoPrevious()
    {
      if (m_modeManager == null)
      {
        return false;
      }
      bool invalidate = m_modeManager.viewStackGotoPrevious();
      if (invalidate)
      {
        ViewerPanel.Invalidate();
      }
      return invalidate;
    }

    public bool viewStackGotoNext()
    {
      if (m_modeManager == null)
      {
        return false;
      }
      bool invalidate = m_modeManager.viewStackGotoNext();
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

    TSLN2DDrawingSurface IPanel.GetDrawingSurface()
    {
      return DrawingSurface;
    }
  }
}
