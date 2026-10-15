using System;
using System.Drawing;
using System.Collections;
using System.Windows.Forms;
using Envitia.MapLink;
using Envitia.MapLink.OpenGLSurface;
using CSharpWinforms_BasicOpenGL_DotNet_Sample;

namespace CSharpWinforms_BasicOpenGL_DotNet_Sample
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
      //The view mode
      public TSLViewMode mode = null;
    }

    //Interaction Mode IDS
    private static int ID_TOOLS_ZOOM = 1;
    private static int ID_TOOLS_PAN = 2;
    private static int ID_TOOLS_GRAB = 3;
    private static int ID_TOOLS_ZOOM_PAN = 4;
    private static int ID_TOOLS_ROTATE = 5;
    //Usual MapLink items
    private DrawingSurfaceClass m_drawingSurface = null;
    private TSLNMapDataLayer m_mapDataLayer = null;

    //Create our array of interaction mode information classes
    private InteractionMode[] m_imodeset = null;

    //Create interaction modes interface
    TSLViewModeInterface m_modeInterface = null;

    //Reference to the current imode
    private TSLViewMode m_current = null;

    //Variables to hold if the control/shift button are currently pressed
    private bool m_controlButtonDown = false;
    private bool m_shiftButtonDown = false;

    //Short name of the loaded map
    private string m_shortfilename;
    private string m_mapFileName;
    private bool m_accelerated = false;


    public MapViewerForm( string mapFile, bool accelerated )
    {
      m_mapFileName = mapFile;
      m_accelerated = accelerated;
      InitializeComponent();
      this.MouseWheel += new MouseEventHandler(OnMouseWheel);
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

    protected override void OnLoad(EventArgs e)
    {
      //Initialise MapLink
        DrawingSurfaceClass.loadStandardConfig();

        // Attach the drawing surface to the panel 
#if false
      m_drawingSurface = new DrawingSurfaceClass(ViewerPanel.Handle, false, m_accelerated);
#else
      TSLNOpenGLSurfaceCreationParameters creationOptions = new TSLNOpenGLSurfaceCreationParameters();
      creationOptions.createCoreProfile(true);
      creationOptions.enableDoubleBuffering(true);
      m_drawingSurface = new TSLNWGLSurface(ViewerPanel.Handle, false, creationOptions);
#endif
      //Create the mapdata layer
      m_mapDataLayer = new TSLNMapDataLayer();
      if (m_mapFileName != "")
      {
          m_mapDataLayer.loadData(m_mapFileName);
      }

      //Create our storage for the imodes
      m_imodeset = new InteractionMode[5];

      //Create the interface from the imodes to the surface and panel
      m_modeInterface = new TSLViewModeInterface(m_drawingSurface, ViewerPanel.Handle);

      //Create the mode sets. These are use by the toolbar and the menu to 
      //identify the imodes
      m_imodeset[0] = new InteractionMode();
      m_imodeset[0].id = ID_TOOLS_ZOOM;
      m_imodeset[0].button = ZoomTool;
      m_imodeset[0].menubutton = ToolsZoom;
      m_imodeset[0].mode = new TSLViewModeZoom(m_modeInterface, ID_TOOLS_ZOOM, 30.0);
      m_imodeset[1] = new InteractionMode();
      m_imodeset[1].id = ID_TOOLS_PAN;
      m_imodeset[1].button = PanTool;
      m_imodeset[1].menubutton = ToolsPan;
      m_imodeset[1].mode = new TSLViewModePan(m_modeInterface, ID_TOOLS_PAN);
      m_imodeset[2] = new InteractionMode();
      m_imodeset[2].id = ID_TOOLS_GRAB;
      m_imodeset[2].button = GrabTool;
      m_imodeset[2].menubutton = ToolsGrab;
      m_imodeset[2].mode = new TSLViewModeGrab(m_modeInterface, ID_TOOLS_GRAB);
      m_imodeset[3] = new InteractionMode();
      m_imodeset[3].id = ID_TOOLS_ZOOM_PAN;
      m_imodeset[3].button = ZoomPanTool;
      m_imodeset[3].menubutton = ToolsZoomPan;
      m_imodeset[3].mode = new TSLViewModeZoomPan(m_modeInterface, ID_TOOLS_ZOOM_PAN, 30.0);
      m_imodeset[4] = new InteractionMode(); 
      m_imodeset[4].id = ID_TOOLS_ROTATE;
      m_imodeset[4].button = RotateTool;
      m_imodeset[4].menubutton = ToolsRotate;
      m_imodeset[4].mode = new TSLViewModeRotate(m_modeInterface, ID_TOOLS_ROTATE);

      m_drawingSurface.wndResize(ViewerPanel.DisplayRectangle.Left, ViewerPanel.DisplayRectangle.Top, ViewerPanel.DisplayRectangle.Right, ViewerPanel.DisplayRectangle.Bottom, false, TSLNResizeActionEnum.TSLNResizeActionMaintainCentre);
      m_drawingSurface.addDataLayer(m_mapDataLayer, "map");

      // DVDX
      m_drawingSurface.setDataLayerProps("map", TSLNPropertyEnum.TSLNPropertyBuffered, 1 );

      m_drawingSurface.setOption(TSLNOptionEnum.TSLNOptionDoubleBuffered, true);

      m_drawingSurface.reset(false);

      //activate zoom imode
      m_current = m_imodeset[0].mode;

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
        m_drawingSurface.drawDU(e.ClipRectangle.Left, e.ClipRectangle.Top, e.ClipRectangle.Right, e.ClipRectangle.Bottom, true, false);
      }
      if (m_modeInterface != null)
      {
        m_modeInterface.drawEchoRectangle();
      }
    }

    //Resize of the ViewerPanel
    private void OnReSize(object sender, System.EventArgs e)
    {
      if (m_drawingSurface != null)
      {
        m_drawingSurface.wndResize(ViewerPanel.DisplayRectangle.Left, ViewerPanel.DisplayRectangle.Top, ViewerPanel.DisplayRectangle.Right, ViewerPanel.DisplayRectangle.Bottom, false, TSLNResizeActionEnum.TSLNResizeActionMaintainCentre);
      }

      // Without the invalidate, the paint is only called when the window gets bigger
      ViewerPanel.Invalidate();
    }

    //Open a Map
    private void FileOpen()
    {
      FileOpenDialog.ShowDialog();
      m_mapFileName = FileOpenDialog.FileName;

      if (m_mapFileName == "")
      {
        return;
      }

      int indexOfLastSlash = m_mapFileName.LastIndexOf('\\') + 1;
      m_shortfilename = m_mapFileName.Substring(indexOfLastSlash, m_mapFileName.Length - indexOfLastSlash);

      this.Text = "Winforms BasicOpenGl .Net Sample - " + m_shortfilename;

      if (!m_mapDataLayer.loadData(m_mapFileName))
      {
        string error = TSLNErrorStack.errorString("Errors:\n", TSLNErrorCategory.TSLNErrorCategoryError | TSLNErrorCategory.TSLNErrorCategoryFatal);
        if (error != "")
        {
          MessageBox.Show(this, error, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }
      }
      m_drawingSurface.backgroundColour = m_mapDataLayer.backgroundColour;
      m_drawingSurface.reset(true);
    }

    private void FileOpenMenu_Click(object sender, System.EventArgs e)
    {
      FileOpen();
    }

    //This is called when any of the buttons on the Toolbar is clicked
    private void TheToolBar_ButtonClick(object sender, System.Windows.Forms.ToolBarButtonClickEventArgs e)
    {
      bool invalidate = false;
      if (e.Button == FileOpenButton)
      {
        FileOpen();
      }
      else if (e.Button == FilePrintButton)
      {
        PrintMap();
      }
      else if (e.Button == ZoomIn)
      {
        invalidate = m_drawingSurface.zoom(30, true, false);
      }
      else if (e.Button == ZoomOut)
      {
        invalidate = m_drawingSurface.zoom(30, false, false);
      }
      else if (e.Button == ResetButton)
      {
        m_drawingSurface.rotate(0.0);
        invalidate = m_drawingSurface.reset(true);
      }
      else
      {
        for (int i = 0; i < m_imodeset.Length; ++i)
        {
          if (e.Button == m_imodeset[i].button)
          {
            m_current = m_imodeset[i].mode;
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
      if (m_current == null)
        return;

      bool invalidate = false;

      switch (e.Button)
      {
        case MouseButtons.Left:
          {
            invalidate = m_current.OnLButtonDown(e.X, e.Y, m_shiftButtonDown, m_controlButtonDown);
            break;
          }
        case MouseButtons.Middle:
          {
            invalidate = m_current.OnMButtonDown(e.X, e.Y, m_shiftButtonDown, m_controlButtonDown);
            break;
          }
        case MouseButtons.Right:
          {
            invalidate = m_current.OnRButtonDown(e.X, e.Y, m_shiftButtonDown, m_controlButtonDown);
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
      if (m_current == null)
        return;

      if (m_current.OnMouseMove(e.X, e.Y, m_shiftButtonDown, m_controlButtonDown))
      {
        // Request a redraw if the interaction hander requires it
        ViewerPanel.Invalidate();
      }
    }

    //On mouse up on the ViewerPanel
    private void OnMouseUp(object sender, System.Windows.Forms.MouseEventArgs e)
    {
      if (m_current == null)
        return;

      bool invalidate = false;
      switch (e.Button)
      {
        case MouseButtons.Left:
          {
            invalidate = m_current.OnLButtonUp(e.X, e.Y, m_shiftButtonDown, m_controlButtonDown);
            break;
          }
        case MouseButtons.Middle:
          {
            invalidate = m_current.OnMButtonUp(e.X, e.Y, m_shiftButtonDown, m_controlButtonDown);
            break;
          }
        case MouseButtons.Right:
          {
            invalidate = m_current.OnRButtonUp(e.X, e.Y, m_shiftButtonDown, m_controlButtonDown);
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
      if (m_current == null)
        return;

      if (m_current.OnMouseWheel(e.X, e.Y, e.Delta, m_shiftButtonDown, m_controlButtonDown))
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
      if (m_current == null)
        return;

      int id = m_current.id;

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
      for (int i = 0; i < m_imodeset.Length; ++i)
      {
        if (sender == m_imodeset[i].menubutton)
        {
          m_current = m_imodeset[i].mode;
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
        if (m_drawingSurface.zoom(30, true, false))
          ViewerPanel.Invalidate();
      }
      else if (sender == this.ToolsZoomOut)
      {
        if (m_drawingSurface.zoom(30, true, false))
          ViewerPanel.Invalidate();
      }
      else if (sender == this.ToolsReset)
      {
        m_drawingSurface.rotate( 0.0 ) ; 
        if (m_drawingSurface.reset(false))
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
      printItems[0] = printItem;

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
        MessageBox.Show(ViewerPanel, "Error", "MapLink could not load printer template", MessageBoxButtons.OK);
        return;
      }

      m_drawingSurface.getUUExtent(out uux1, out uuy1, out uux2, out uuy2);
      m_drawingSurface.UUToTMC(uux1, uuy1, out tmcx1, out tmcy1);
      m_drawingSurface.UUToTMC(uux2, uuy2, out tmcx2, out tmcy2);
      envelope.bottomLeft.x = tmcx1;
      envelope.bottomLeft.y = tmcy1;
      envelope.topRight.x = tmcx2;
      envelope.topRight.y = tmcy2;
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
        MessageBox.Show("An error occurred attempting to print -  " + ex.Message);
      }
    }

    private void FileExitMenu_Click(object sender, EventArgs e)
    {
      this.Close();
    }

 


  }
}
