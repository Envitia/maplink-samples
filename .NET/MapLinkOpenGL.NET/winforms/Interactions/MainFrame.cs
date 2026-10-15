using System;
using System.Drawing;
using System.Collections;
using System.Windows.Forms;
using System.Runtime.InteropServices;

using Envitia.MapLink;
using Envitia.MapLink.InteractionModes;
using Envitia.MapLink.OpenGLSurface;

namespace CSharpWinforms_InteractionOpenGL_DotNet_Sample
{
#if false
    using DrawingSurfaceClass = TSLNDrawingSurface;
#else
    using DrawingSurfaceClass = TSLNOpenGLSurface;
#endif

  /// <summary>
  /// MapViewerForm
  /// </summary>
  public partial class MapViewerForm : Form
  {
    //Holds information about each interaction mode
    private class InteractionMode
    {
      //The id
      public int id;
      //The button on the tool bar that activates this mode
      public System.Windows.Forms.ToolBarButton button;
      //The button on the menu bar that activates this mode
      public System.Windows.Forms.MenuItem menubutton;
    }

    //Interaction Mode IDS
    private static int ID_TOOLS_ZOOM = 1;
    private static int ID_TOOLS_PAN = 2;
    private static int ID_TOOLS_MAGNIFY = 3;
    private static int ID_TOOLS_GRAB = 4;
    private static int ID_TOOLS_ZOOM_PAN = 5;

    //Create the usual MapLink Item pointers
    private DrawingSurfaceClass m_drawingSurface = null;
    private TSLNMapDataLayer m_mapDataLayer = null;

    //Create our array pointer of interaction mode information
    private InteractionMode[] m_imodeset = null;

    //Create interaction modes' pointers
    private TSLNInteractionModeManagerGeneric m_modeManager = null;
    //private TSLNInteractionModeManager m_modeManager = null;
    private TSLNInteractionModeZoom m_zoomMode = null;
    private TSLNInteractionModePan m_panMode = null;
    private TSLNInteractionModeMagnify m_magMode = null;
    private TSLNInteractionModeGrab m_grabMode = null;
    private TSLNInteractionMode m_myCustomMode = null;

    //Create out request receiver pointer
    private InteractionModeRequestReceiver m_updateReceiver = null;

    //Variables to hold if the control/shift button are currently pressed
    private bool m_controlButtonDown = false;
    private bool m_shiftButtonDown = false;

    //Short name of the loaded map
    private string m_shortfilename;
    private string m_mapFilename;

    public MapViewerForm(string mapFile)
    {
      m_mapFilename = mapFile;
      InitializeComponent();
      this.WindowState = FormWindowState.Maximized;
      //! Set Maplink Home directory here ...
      string maplHome = TSLNUtilityFunctions.getMapLinkHome();
      if (string.IsNullOrEmpty(maplHome))
      {
        TSLNUtilityFunctions.setMapLinkHome("../");
      }

      //! Must be done in order to display maps through the new maplink display helper
      bool load = Envitia.MapLink.TSLNDrawingSurface.loadStandardConfig();
    }

    /// <summary>
    /// The main entry point for the application.
    /// </summary>
    [STAThread]
    static void Main(string[] args)
    {
      string mapFile = "";
      if (args.Length > 0)
      {
        mapFile = args[0];
      }
      Application.Run(new MapViewerForm(mapFile));
    }

    protected override void OnLoad(EventArgs e)
    {
        bool status = DrawingSurfaceClass.loadStandardConfig();
#if false
      m_drawingSurface = new DrawingSurfaceClass((IntPtr)ViewerPanel.Handle, false);
#else
      TSLNOpenGLSurfaceCreationParameters creationOptions = new TSLNOpenGLSurfaceCreationParameters();
      creationOptions.createCoreProfile(true);
      creationOptions.enableDoubleBuffering(true);
      m_drawingSurface = new TSLNWGLSurface(ViewerPanel.Handle, false, creationOptions);
#endif
      
      m_mapDataLayer = new TSLNMapDataLayer();
      m_updateReceiver = new InteractionModeRequestReceiver();
      m_imodeset = new InteractionMode[5];
      // Attach the drawing surface to the form and set it's initial size...
      // This must be done before the drawingsurface is given to the
      // InteractionModeManager
      m_drawingSurface.attach(ViewerPanel.Handle, false);

      // Create our user defined request receiver
      TSLNInteractionModeRequest request = (TSLNInteractionModeRequest)m_updateReceiver;

      // Now create the mode manager
      //m_modeManager = new TSLNInteractionModeManager(request, m_drawingSurface, ViewerPanel.Handle);
      m_modeManager = new TSLNInteractionModeManagerGeneric(request, m_drawingSurface, 5, 5, 30, true);

      // Next create our modes
      m_zoomMode = new TSLNInteractionModeZoom(ID_TOOLS_ZOOM, 30, "Left button drag zoom to rectangle, Left button click anchored zoom-in, Shift-Left button click anchored zoom-out, Right button click to finish", true);
      m_panMode = new TSLNInteractionModePan(ID_TOOLS_PAN, "Left button click pans to point, Right button click to finish", true);
      m_magMode = new TSLNInteractionModeMagnify(ID_TOOLS_MAGNIFY, "Mouse move to change magnified area, Right button click to finish", true);
      m_grabMode = new TSLNInteractionModeGrab(ID_TOOLS_GRAB, true, "Left button drag move view, Right button click to finish", true);
      m_myCustomMode = new CustomInteractionModeZoomPan(ID_TOOLS_ZOOM_PAN, 30, true,  "Left button click pans to point and zooms in, Right button click pans to point and zooms out");

      //Give them to the modeManager
      //Remember we don't own the modes anymore after this point
      //but we must keep a reference to them or c# won't deleted them properly
      m_modeManager.addMode(m_zoomMode, true); //set zoom as default
      m_modeManager.addMode(m_grabMode, false);
      m_modeManager.addMode(m_magMode, false);
      m_modeManager.addMode(m_panMode, false);
      m_modeManager.addMode(m_myCustomMode, false);

      //Create the mode sets. These are use by the toolbar and the menu to identify the
      //modes
      m_imodeset[0] = new InteractionMode();
      m_imodeset[0].id = ID_TOOLS_ZOOM;
      m_imodeset[0].button = ZoomTool;
      m_imodeset[0].menubutton = ToolsZoom;
      m_imodeset[1] = new MapViewerForm.InteractionMode();
      m_imodeset[1].id = ID_TOOLS_PAN;
      m_imodeset[1].button = PanTool;
      m_imodeset[1].menubutton = ToolsPan;
      m_imodeset[2] = new MapViewerForm.InteractionMode();
      m_imodeset[2].id = ID_TOOLS_MAGNIFY;
      m_imodeset[2].button = MagnifyTool;
      m_imodeset[2].menubutton = ToolsMagnify;
      m_imodeset[3] = new MapViewerForm.InteractionMode();
      m_imodeset[3].id = ID_TOOLS_GRAB;
      m_imodeset[3].button = GrabTool;
      m_imodeset[3].menubutton = ToolsGrab;
      m_imodeset[4] = new MapViewerForm.InteractionMode();
      m_imodeset[4].id = ID_TOOLS_ZOOM_PAN;
      m_imodeset[4].button = ZoomPanTool;
      m_imodeset[4].menubutton = ToolsZoomPan;

      m_drawingSurface.wndResize(ViewerPanel.DisplayRectangle.Left, ViewerPanel.DisplayRectangle.Top, ViewerPanel.DisplayRectangle.Right, ViewerPanel.DisplayRectangle.Bottom, false, TSLNResizeActionEnum.TSLNResizeActionMaintainCentre);
      m_modeManager.onSize(ViewerPanel.DisplayRectangle.Width, ViewerPanel.DisplayRectangle.Height);

      // In MapLink 5.0+, this will only work AFTER the drawing surface has been attached to a window
      status = m_drawingSurface.addDataLayer(m_mapDataLayer, "map");
      m_drawingSurface.setDataLayerProps("map", TSLNPropertyEnum.TSLNPropertyBuffered, 1);

      m_drawingSurface.setOption(TSLNOptionEnum.TSLNOptionDoubleBuffered, true);

      m_drawingSurface.reset(false);

      //activate zoom imode
      m_modeManager.setCurrentMode(ID_TOOLS_ZOOM);

      //Open the current map file, if passed on command line
      FileOpen(false);

      base.OnLoad(e);
    }

    private void OnClosed(object sender, FormClosedEventArgs e)
    {
        DrawingSurfaceClass.cleanup();
        m_drawingSurface.Dispose();
        m_drawingSurface = null;
    }

    //Paint of the ViewerPanel
    private void OnPaint(object sender, System.Windows.Forms.PaintEventArgs e)
    {
      if (m_drawingSurface != null)
      {
        m_drawingSurface.drawDU(e.ClipRectangle.Left, e.ClipRectangle.Top, e.ClipRectangle.Right, e.ClipRectangle.Bottom, false, false);
      }

      // Don't forget to draw any echo rectangle that may be active.
      if (m_modeManager != null)
      {
        m_modeManager.onDraw(e.ClipRectangle.Left, e.ClipRectangle.Top, e.ClipRectangle.Right, e.ClipRectangle.Bottom);
      }
    }

    //Resize of the ViewerPanel
    private void OnReSize(object sender, System.EventArgs e)
    {
      if (m_drawingSurface != null)
      {
        m_drawingSurface.wndResize(ViewerPanel.DisplayRectangle.Left, ViewerPanel.DisplayRectangle.Top, ViewerPanel.DisplayRectangle.Right, ViewerPanel.DisplayRectangle.Bottom, false, TSLNResizeActionEnum.TSLNResizeActionMaintainCentre);
      }
      if (m_modeManager != null)
      {
        m_modeManager.onSize(ViewerPanel.DisplayRectangle.Width, ViewerPanel.DisplayRectangle.Height);
      }

      // Without the invalidate, the paint is only called when the window gets bigger
      ViewerPanel.Invalidate();
    }

    //Open a Map
    private void FileOpen(bool showDialog)
    {
      if (showDialog)
      {
        FileOpenDialog.ShowDialog();
        m_mapFilename = FileOpenDialog.FileName;
      }
      if (m_mapFilename == "")
      {
        return;
      }

      int indexOfLastSlash = m_mapFilename.LastIndexOf('\\') + 1;
      m_shortfilename = m_mapFilename.Substring(indexOfLastSlash, m_mapFilename.Length - indexOfLastSlash);

      this.Text = "Winforms InteractionOpenGl .Net Sample - " + m_shortfilename;

      if (!m_mapDataLayer.loadData(m_mapFilename))
      {
        string error = TSLNErrorStack.errorString("Errors:\n", TSLNErrorCategory.TSLNErrorCategoryError | TSLNErrorCategory.TSLNErrorCategoryFatal);
        if (error != "")
        {
          MessageBox.Show(this, "MapLink has reported an error during loading of the selected map", "Error", MessageBoxButtons.OK);
        }
      }
      m_drawingSurface.backgroundColour = m_mapDataLayer.backgroundColour;
      m_drawingSurface.reset(true);
    }

    private void FileOpenMenu_Click(object sender, System.EventArgs e)
    {
      FileOpen(true);
    }

    //This is called when any of the buttons on the Toolbar is clicked
    private void TheToolBar_ButtonClick(object sender, System.Windows.Forms.ToolBarButtonClickEventArgs e)
    {
      if (m_modeManager == null)
      {
        return;
      }

      bool invalidate = false;
      if (e.Button == FileOpenButton)
      {
        FileOpen(true);
      }
      else if (e.Button == FilePrintButton)
      {
        PrintMap();
      }
      else if (e.Button == CopyButton)
      {
        EditCopy();
      }
      else if (e.Button == ZoomIn)
      {
        invalidate = m_modeManager.zoomIn(30, true);
      }
      else if (e.Button == ZoomOut)
      {
        invalidate = m_modeManager.zoomOut(30, true);
      }
      else if (e.Button == ResetButton)
      {
        invalidate = m_modeManager.resetToFullExtent(true);
      }
      else if (e.Button == previousView)
      {
        invalidate = m_modeManager.viewStackGotoPrevious();
      }
      else if (e.Button == nextView)
      {
        invalidate = m_modeManager.viewStackGotoNext();
      }
      else if (e.Button == savedView1)
      {
        invalidate = GotoSavedView(0);
      }
      else if (e.Button == savedView2)
      {
        invalidate = GotoSavedView(1);
      }
      else if (e.Button == savedView3)
      {
        invalidate = GotoSavedView(2);
      }
      else
      {
        for (int i = 0; i < m_imodeset.Length; ++i)
        {
          if (e.Button == m_imodeset[i].button)
          {
            invalidate = m_modeManager.setCurrentMode(m_imodeset[i].id);
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
      if (m_modeManager == null)
      {
        return;
      }

      bool invalidate = false;

      switch (e.Button)
      {
        case MouseButtons.Left:
          {
            invalidate = m_modeManager.onLButtonDown(e.X, e.Y, m_shiftButtonDown, m_controlButtonDown);
            break;
          }
        case MouseButtons.Middle:
          {
            invalidate = m_modeManager.onMButtonDown(e.X, e.Y, m_shiftButtonDown, m_controlButtonDown);
            break;
          }
        case MouseButtons.Right:
          {
            invalidate = m_modeManager.onRButtonDown(e.X, e.Y, m_shiftButtonDown, m_controlButtonDown);
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

      if (m_modeManager.onMouseMove(button, e.X, e.Y, m_shiftButtonDown, m_controlButtonDown))
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
            invalidate = m_modeManager.onLButtonUp(e.X, e.Y, m_shiftButtonDown, m_controlButtonDown);
            break;
          }
        case MouseButtons.Middle:
          {
            invalidate = m_modeManager.onMButtonUp(e.X, e.Y, m_shiftButtonDown, m_controlButtonDown);
            break;
          }
        case MouseButtons.Right:
          {
            invalidate = m_modeManager.onRButtonUp(e.X, e.Y, m_shiftButtonDown, m_controlButtonDown);
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
      if (m_modeManager == null)
      {
        return;
      }
      if (m_modeManager.onMouseWheel((short)e.Delta, e.X, e.Y))
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
      TSLNInteractionMode cmode = null;
      long id = m_modeManager.getCurrentMode(out cmode);

      for (int i = 0; i < m_imodeset.Length; ++i)
      {
        if (id == m_imodeset[i].id)
        {
          m_imodeset[i].button.Pushed = true;
          m_imodeset[i].menubutton.Checked = true;
        }
        else
        {
          m_imodeset[i].button.Pushed = false;
          m_imodeset[i].menubutton.Checked = false;
        }
      }
    }

    //called when one of the imode menu items are clicked
    private void MenuImode_Click(object sender, System.EventArgs e)
    {
      if (m_modeManager == null)
      {
        return;
      }

      for (int i = 0; i < m_imodeset.Length; ++i)
      {
        if (sender == m_imodeset[i].menubutton)
        {
          m_modeManager.setCurrentMode(m_imodeset[i].id);
          break;
        }
      }
      UpdateToolsAndMenuButtons();
    }

    //called whenever one of the base-level non imode menu buttons on the Tools
    //menu are clicked
    private void MenuNonImode_Click(object sender, System.EventArgs e)
    {
      if (sender == this.ToolsZoomIn)
      {
        if (m_modeManager.zoomIn(30, true))
          ViewerPanel.Invalidate();
      }
      else if (sender == this.ToolsZoomOut)
      {
        if (m_modeManager.zoomOut(30, true))
          ViewerPanel.Invalidate();
      }
      else if (sender == this.ToolsReset)
      {
        if (m_modeManager.resetToFullExtent(true))
          ViewerPanel.Invalidate();
      }

    }

    //called when Help->About is clicked from the menu
    private void HelpAboutMenu_Click(object sender, System.EventArgs e)
    {
      AboutBox abox = new AboutBox();
      abox.ShowDialog(this);
    }

    //called when File->Print is clicked from the menu
    private void FilePrintMenu_Click(object sender, System.EventArgs e)
    {
      PrintMap();
    }

    //Called from either the menu or toolbar print buttons
    private void PrintMap()
    {
      TSLNPrintItem[] printItems = new TSLNPrintItem[1];
      TSLNPrintItem printItem = new TSLNPrintItem();
      TSLNEnvelope envelope = new TSLNEnvelope();
      printItems.SetValue(printItem, 0);

      double uux1 = 0, uuy1 = 0, uux2 = 0, uuy2 = 0;
      int tmcx1 = 0, tmcy1 = 0, tmcx2 = 0, tmcy2 = 0;
      string templateFile = "";
      bool isLandscape;

      if (ViewerPanel.Width > ViewerPanel.Height)
      {
        templateFile = "landscape.tpt";
        isLandscape = true;
      }
      else
      {
        templateFile = "portrait.tpt";
        isLandscape = false;
      }

      string templateFilePath = DrawingSurfaceClass.findFile(templateFile);

      TSLNEntitySet entitySet = TSLNPrintDocument.loadTemplate(templateFilePath, DrawingSurfaceClass.fileLoader, null);

      if (entitySet.size == 0)
      {
        MessageBox.Show(ViewerPanel, "MapLink could not load printer template", "Error", MessageBoxButtons.OK);
        return;
      }

      m_drawingSurface.getUUExtent(out uux1, out uuy1, out uux2, out uuy2);
      m_drawingSurface.UUToTMC(uux1, uuy1, out tmcx1, out tmcy1);
      m_drawingSurface.UUToTMC(uux2, uuy2, out tmcx2, out tmcy2);
      envelope.bottomLeft.x = tmcx1;
      envelope.bottomLeft.y = tmcy1;
      envelope.topRight.x = tmcx2;
      envelope.topRight.y = tmcy2;
      long tmcPerMetre = printItem.tmcPerMetre;
      printItem.drawingSurface = m_drawingSurface;
      printItem.extent(envelope);
      printItem.borderColour = Color.Black;
      printItem.borderStyle = 1;
      printItem.borderThickness = 2;
      printItem.templateName = "map";

      string[] textMappings = (string[])System.Array.CreateInstance(typeof(string), 4);
      textMappings.SetValue("MapLink Pro", 0);    // Default
      textMappings.SetValue(m_shortfilename, 1);    // New text
      textMappings.SetValue("date", 2);    // Default
      System.DateTime dt = System.DateTime.Now;
      textMappings.SetValue(dt.ToShortDateString(), 3);    // Default

      try
      {
        TSLNPrintDocument pd = new TSLNPrintDocument(m_drawingSurface, entitySet, printItems, textMappings, isLandscape);
        PrintDialog dlg = new PrintDialog();
        dlg.Document = pd;
        DialogResult result = dlg.ShowDialog();

        if (result == DialogResult.OK)
        {
          pd.Print();
        }
      }
      catch (Exception ex)
      {
        MessageBox.Show(this, "An error occurred attempting to print -  " + ex.Message);
      }
    }

    //called when Edit->Copy is clicked from the menu
    private void EditCopy_Click(object sender, System.EventArgs e)
    {
      EditCopy();
    }

    [DllImport("gdi32.dll")]
    private static extern bool
    BitBlt(IntPtr hdcDest, int nXDest, int nYDest, int nWidth, int nHeight, IntPtr hdcSrc, int nXSrc, int nYSrc, int dwRop);

    //Called from either the menu or toolbar copy buttons
    private void EditCopy()
    {
      Graphics g = ViewerPanel.CreateGraphics();
      Bitmap image = new Bitmap(ViewerPanel.Width, ViewerPanel.Height);
      Graphics dest = Graphics.FromImage(image);
      IntPtr hdc = g.GetHdc();
      IntPtr hdcDest = dest.GetHdc();
      BitBlt(hdcDest, 0, 0, ViewerPanel.Width, ViewerPanel.Height, hdc, 0, 0, 0x00CC0020);
      g.ReleaseHdc(hdc);
      dest.ReleaseHdc(hdcDest);
      Clipboard.SetDataObject(image, true);		
    }

    //called when Tools->Saved Views->* is clicked from the menu
    private void OnSavedViewsMenuClick(object sender, System.EventArgs e)
    {
      if (m_modeManager == null)
      {
        return;
      }

      bool invalidate = false;

      if (sender == ToolsGoto1Menu)
      {
        invalidate = GotoSavedView(0);
      }
      else if (sender == ToolsGoto1Menu)
      {
        invalidate = GotoSavedView(1);
      }
      else if (sender == ToolsGoto1Menu)
      {
        invalidate = GotoSavedView(2);
      }
      else if (sender == ToolsSet1Menu)
      {
        SaveView(0);
      }
      else if (sender == ToolsSet2Menu)
      {
        SaveView(1);
      }
      else if (sender == ToolsSet3Menu)
      {
        SaveView(2);
      }
      else if (sender == ToolsViewsResetMenu)
      {
        ViewStackReset();
      }

      if (invalidate)
      {
        ViewerPanel.Invalidate();
      }

    }

    private bool GotoSavedView(int index)
    {
      if (m_modeManager == null)
      {
        return false;
      }
      return m_modeManager.savedViewGoto(index);
    }

    private void SaveView(int index)
    {
      if (m_modeManager == null)
      {
        return;
      }
      m_modeManager.savedViewSetToCurrent(index);
    }

    private void ViewStackReset()
    {
      if (m_modeManager == null)
      {
        return;
      }
      m_modeManager.savedViewReset();
    }

    private void OnViewStackMenuClick(object sender, System.EventArgs e)
    {
      if (m_modeManager == null)
      {
        return;
      }

      bool invalidate = false;

      if (sender == ToolsPreviousViewMenu)
      {
        invalidate = m_modeManager.viewStackGotoPrevious();
      }
      else if (sender == ToolsNextViewMenu)
      {
        invalidate = m_modeManager.viewStackGotoNext();
      }

      if (invalidate)
      {
        ViewerPanel.Invalidate();
      }

    }

    private void FileExitMenu_Click(object sender, EventArgs e)
    {
      this.Close();
    }

  }
}
