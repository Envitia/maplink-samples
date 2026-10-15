using System;
using System.Drawing;
using System.Collections;
using System.Windows.Forms;
using System.Runtime.InteropServices;

using Envitia.MapLink;
using Envitia.MapLink.InteractionModes;
using Envitia.MapLink.DDO;

namespace CSharpDDOSample
{
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
    private static int ID_TOOLS_GRAB = 3;

    //Create the usual MapLink Item pointers
    private TSLNDrawingSurface m_drawingSurface = null;
    private TSLNMapDataLayer m_mapDataLayer = null;
    private TSLNObjectDataLayer m_ddoDataLayer = null;

    //Create our array pointer of interaction mode information
    private InteractionMode[] m_imodeset = null;

    //Create interaction modes' pointers
    private TSLNInteractionModeManager m_modeManager = null;
    private TSLNInteractionModeZoom m_zoomMode = null;
    private TSLNInteractionModePan m_panMode = null;
    private TSLNInteractionModeGrab m_grabMode = null;

    //Create out request receiver pointer
    private InteractionModeRequestReceiver m_updateReceiver = null;

    //Variables to hold if the control/shift button are currently pressed
    private bool m_controlButtonDown = false;
    private bool m_shiftButtonDown = false;

    private string m_shortfilename;
    private string m_mapFileName;

    public MapViewerForm(string mapFile)
    {
      m_mapFileName = mapFile;
      InitializeComponent();
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
      TSLNDrawingSurface.cleanup();
    }

    protected override void OnLoad(EventArgs e)
    {
      bool status = TSLNDrawingSurface.loadStandardConfig();

      m_drawingSurface = new TSLNDrawingSurface((IntPtr)ViewerPanel.Handle, false);
      m_mapDataLayer = new TSLNMapDataLayer();
      m_ddoDataLayer = new TSLNObjectDataLayer();

      m_updateReceiver = new InteractionModeRequestReceiver();
      m_imodeset = new InteractionMode[3];
      
      // Create our user defined request receiver
      TSLNInteractionModeRequest request = (TSLNInteractionModeRequest)m_updateReceiver;

      // Now create the mode manager
      m_modeManager = new TSLNInteractionModeManager(request, m_drawingSurface, ViewerPanel.Handle);

      // Next create our modes
      m_zoomMode = new TSLNInteractionModeZoom(ID_TOOLS_ZOOM, 30, "Left button drag zoom to rectangle, Left button click anchored zoom-in, Shift-Left button click anchored zoom-out, Right button click to finish", true);
      m_panMode = new TSLNInteractionModePan(ID_TOOLS_PAN, "Left button click pans to point, Right button click to finish", true);
      m_grabMode = new TSLNInteractionModeGrab(ID_TOOLS_GRAB, true, "Left button drag move view, Right button click to finish", true);

      //Give them to the modeManager
      //Remember we don't own the modes anymore after this point
      //but we must keep a reference to them or c# won't deleted them properly
      m_modeManager.addMode(m_zoomMode, true); //set zoom as default
      m_modeManager.addMode(m_grabMode, false);
      m_modeManager.addMode(m_panMode, false);

      //Create the mode sets. These are use by the toolbar and the menu to identify the
      //modes
      m_imodeset[0] = new InteractionMode();
      m_imodeset[0].id = ID_TOOLS_ZOOM;
      m_imodeset[0].button = ZoomTool;
      m_imodeset[0].menubutton = ToolsZoom;
      m_imodeset[1] = new InteractionMode();
      m_imodeset[1].id = ID_TOOLS_PAN;
      m_imodeset[1].button = PanTool;
      m_imodeset[1].menubutton = ToolsPan;
      m_imodeset[2] = new InteractionMode();
      m_imodeset[2].id = ID_TOOLS_GRAB;
      m_imodeset[2].button = GrabTool;
      m_imodeset[2].menubutton = ToolsGrab;

      m_drawingSurface.wndResize(ViewerPanel.DisplayRectangle.Left, ViewerPanel.DisplayRectangle.Top, ViewerPanel.DisplayRectangle.Right, ViewerPanel.DisplayRectangle.Bottom, false, TSLNResizeActionEnum.TSLNResizeActionMaintainCentre);
      m_modeManager.onSize(ViewerPanel.DisplayRectangle.Width, ViewerPanel.DisplayRectangle.Height);

      // In MapLink 5.0+, this will only work AFTER the drawing surface has been attached to a window
      status = m_drawingSurface.addDataLayer(m_mapDataLayer, "map");
      status = m_drawingSurface.addDataLayer(m_ddoDataLayer, "ddo");

      m_drawingSurface.setDataLayerProps("map", TSLNPropertyEnum.TSLNPropertyBuffered, 1);
      m_drawingSurface.setOption(TSLNOptionEnum.TSLNOptionDoubleBuffered, true);

      m_drawingSurface.reset(false);

      //activate zoom imode
      m_modeManager.setCurrentMode(ID_TOOLS_ZOOM);

      FileOpen(false);

      base.OnLoad(e);
    }

    //Paint of the ViewerPanel
    private void OnPaint(object sender, System.Windows.Forms.PaintEventArgs e)
    {
      if (m_drawingSurface != null)
      {
        m_drawingSurface.drawDU(e.ClipRectangle.Left, e.ClipRectangle.Top, e.ClipRectangle.Right, e.ClipRectangle.Bottom, true, false);
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
        if (FileOpenDialog.ShowDialog() != DialogResult.OK)
        {
          return;
        }
        m_mapFileName = FileOpenDialog.FileName;
      }

      if (m_mapFileName == "")
      {
        return;
      }

      m_ddoDataLayer.removeData();

      int indexOfLastSlash = m_mapFileName.LastIndexOf('\\') + 1;
      m_shortfilename = m_mapFileName.Substring(indexOfLastSlash, m_mapFileName.Length - indexOfLastSlash);

      this.Text = "MapLink C# .NET DDO Sample - " + m_shortfilename;

      if (!m_mapDataLayer.loadData(m_mapFileName))
      {
        string error = TSLNErrorStack.errorString("Errors:\n", TSLNErrorCategory.TSLNErrorCategoryError | TSLNErrorCategory.TSLNErrorCategoryFatal);
        if (error != "")
        {
          MessageBox.Show(this, error, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
          return;
        }
      }
      m_drawingSurface.backgroundColour = m_mapDataLayer.backgroundColour;

      // Calculate how many TMCS per metre (map/user unit)
      double x1, y1, x2, y2;

      //get the extent of this newly loaded map in map units and tmcs
      m_mapDataLayer.getMUExtent(out x1, out y1, out x2, out y2);

      int tx1, ty1, tx2, ty2;
      m_mapDataLayer.getTMCExtent(out tx1, out ty1, out tx2, out ty2, -1);

      double tpm = ((double)tx2 - (double)tx1) / (x2 - x1);

      Aircraft.tmcPerMeter = tpm;

      // Lat/Long coordinates (approx.) of various destinations.
      double[,] airports = {
                            {   51.4667,   -0.4500 },    // London
                            {   40.6667,  -74.0833 },    // New York
                            {   52.5333,   13.4167 },    // Berlin
                            {   49.0167,    2.5167 },    // Paris
                            {   35.6667,  139.7500 },    // Tokyo
                            {  -33.9167,  151.1667 },    // Sydney
                            {  -31.9667,  115.8167 },    // Perth
                            {   37.7500, -122.4500 },    // San Francisco
                            {  -22.8833,  -43.2833 },    // Rio de Janeiro
                            {   55.7500,   37.7000 },    // Moscow
                            {   28.6167,   77.2167 },    // New Delhi
                            {   39.9167,  116.4333 },    // Beijing
                            {   33.3333,   44.4333 },    // Baghdad
                            {  -33.5000,  -70.6667 },    // Santiago
                            {   64.7167, -157.0000 },    // Alaska (Galena)
                            {   71.9833,  102.5167 },    // Russian Fed. (Khatanga)
                            {  -26.1667,   28.0333 }     // Johannesberg
                           };

      int numberOfAirports = airports.GetLength(0);
      // Instantiate some aircraft
      const int Nplanes = 350;
      for (int i = 0; i < Nplanes; ++i)
      {
        Aircraft plane = new Aircraft((Aircraft.AircraftClass)Aircraft.random((int)Aircraft.AircraftClass.TMS_747, (int)Aircraft.AircraftClass.TMS_Cessna));

        long dep = Aircraft.random(0, numberOfAirports - 1);
        long arr = Aircraft.random(0, numberOfAirports - 1);

        int x, y;

        m_mapDataLayer.latLongToTMC(airports[dep, 0], airports[dep, 1], out x, out y, false);

        plane.move(x, y, false);

        m_mapDataLayer.latLongToTMC(airports[arr, 0], airports[arr, 1], out x, out y, false);

        plane.destination(x, y);
        plane.velocity = Aircraft.random(200, 800);

        m_ddoDataLayer.addDDO(plane);
      }

      timer1.Start();

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

      string templateFilePath = TSLNDrawingSurface.findFile(templateFile);

      TSLNEntitySet entitySet = TSLNPrintDocument.loadTemplate(templateFilePath, TSLNDrawingSurface.fileLoader, null);

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
      else if (sender == ToolsGoto2Menu)
      {
        invalidate = GotoSavedView(1);
      }
      else if (sender == ToolsGoto3Menu)
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

    private void OnUpdate(object sender, EventArgs e)
    {
      double rate = 10000.0 / Aircraft.tmcPerMeter;

      if (m_ddoDataLayer != null) 
      {
        TSLNDynamicDataObject[] listDDO = m_ddoDataLayer.getDDOList();

        foreach (TSLNDynamicDataObject ddo in listDDO)
        {
          Aircraft plane = (Aircraft)ddo;
          plane.update(rate);
        }

        m_ddoDataLayer.notifyChanged(true);
      }

      ViewerPanel.Invalidate();
    }

    private void FileExitMenu_Click(object sender, EventArgs e)
    {
      this.Close();
    }

  }
}
