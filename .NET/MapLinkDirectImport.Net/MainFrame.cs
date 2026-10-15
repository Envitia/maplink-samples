using System;
using System.Drawing;
using System.Collections;
using System.Windows.Forms;
using System.IO;
using Envitia.MapLink;
using Envitia.MapLink.InteractionModes;
using Envitia.MapLink.DirectImport;

namespace CSharpDirectImportSample
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
    private static int ID_TOOLS_ZOOM_PAN = 4;
    //Usual MapLink items
    private TSLNDrawingSurface m_drawingSurface = null;
    private TSLNMapDataLayer m_mapDataLayer = null;
    private DirectImportCallbacks m_directImportDataLayerCallbacks = null;
    private TSLNDirectImportDataLayer m_directImportDataLayer = null;
    private TSLNCoordinateSystem m_displayCS = null;
    private TSLNCoordinateSystem m_inputCS = null;

    //Create our array of interaction mode information classes
    private InteractionMode[] m_imodeset = null;

    private TSLNInteractionModeManager m_modeManager = null;
    private TSLNInteractionModeZoom m_zoomMode = null;
    private TSLNInteractionModePan m_panMode = null;
    private TSLNInteractionModeGrab m_grabMode = null;
    private TSLNInteractionMode m_customZoomGrabMode = null;

    private InteractionModeRequestReceiver m_updateReceiver = null;

    //Variables to hold if the control/shift button are currently pressed
    private bool m_controlButtonDown = false;
    private bool m_shiftButtonDown = false;

    //Short name of the loaded map
    private string m_shortfilename;
    private string m_mapFileName;

    private bool m_loaded = false;
    private CustomClassifier m_classifier;

    public TSLNDirectImportDataLayer DirectImportDataLayer { get { return m_directImportDataLayer; } set { m_directImportDataLayer = value; } }
    public TSLNFeatureClassConfig FeatureConfig { get; set; }

    public TSLNCoordinateSystem InputCS { get { return m_inputCS; } }

    public MapViewerForm(string mapFile)
    {
      m_mapFileName = mapFile;
      InitializeComponent();
      this.MouseWheel += new MouseEventHandler(OnMouseWheel);

      m_classifier = new CustomClassifier();

      if (m_classifier == null || !m_classifier.Initialised)
      {
        MessageBox.Show("Cannot load custom rendition file.\n\nCustom vector styling unavailable.");
      }

    }

    protected override void OnLoad(EventArgs e)
    {
      //Initialise MapLink
      if (TSLNDrawingSurface.loadStandardConfig() == false)
      {
        MessageBox.Show(this, "Failed to load standard config. This sample will not function correctly.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
        return;
      }

      if (TSLNCoordinateSystem.loadCoordinateSystems() == false)
      {
        MessageBox.Show(this, "Failed to load coordinate systems. This sample will not function correctly.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
        return;
      }

      // Attach the drawing surface to the panel 
      m_drawingSurface = new TSLNDrawingSurface(ViewerPanel.Handle, false);

      //Create the mapdata layer
      m_mapDataLayer = new TSLNMapDataLayer();
      if (m_mapFileName != "")
      {
        m_mapDataLayer.loadData(m_mapFileName);
      }

      // Interaction mode setup
      m_updateReceiver = new InteractionModeRequestReceiver();
      m_imodeset = new InteractionMode[4];

      TSLNInteractionModeRequest request = (TSLNInteractionModeRequest)m_updateReceiver;

      //Create the interface from the imodes to the surface and panel
      m_modeManager = new TSLNInteractionModeManager(request, m_drawingSurface, ViewerPanel.Handle);

      // Next create our modes
      m_zoomMode = new TSLNInteractionModeZoom(ID_TOOLS_ZOOM, 30, "Left button drag zoom to rectangle, Left button click anchored zoom-in, Shift-Left button click anchored zoom-out, Right button click to finish", true);
      m_panMode = new TSLNInteractionModePan(ID_TOOLS_PAN, "Left button click pans to point, Right button click to finish", true);
      m_grabMode = new TSLNInteractionModeGrab(ID_TOOLS_GRAB, true, "Left button drag move view, Right button click to finish", true);
      m_customZoomGrabMode = new CustomInteractionModeZoomPan(ID_TOOLS_ZOOM_PAN, 30, true, "Left button click pans to point and zooms in, Right button click pans to point and zooms out");

      //Give them to the modeManager
      //Remember we don't own the modes anymore after this point
      //but we must keep a reference to them or c# won't deleted them properly
      m_modeManager.addMode(m_zoomMode, true); //set zoom as default
      m_modeManager.addMode(m_grabMode, false);
      m_modeManager.addMode(m_panMode, false);
      m_modeManager.addMode(m_customZoomGrabMode, false);

      //Create the mode sets. These are use by the toolbar and the menu to 
      //identify the imodes
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
      m_imodeset[3] = new InteractionMode();
      m_imodeset[3].id = ID_TOOLS_ZOOM_PAN;
      m_imodeset[3].button = ZoomPanTool;
      m_imodeset[3].menubutton = ToolsZoomPan;

      m_drawingSurface.wndResize(ViewerPanel.DisplayRectangle.Left, ViewerPanel.DisplayRectangle.Top, ViewerPanel.DisplayRectangle.Right, ViewerPanel.DisplayRectangle.Bottom, false, TSLNResizeActionEnum.TSLNResizeActionMaintainCentre);
      m_modeManager.onSize(ViewerPanel.DisplayRectangle.Width, ViewerPanel.DisplayRectangle.Height);

      // Add the datalayer(s) to the surface
      m_drawingSurface.addDataLayer(m_mapDataLayer, "map");

      m_drawingSurface.setDataLayerProps("map", TSLNPropertyEnum.TSLNPropertyBuffered, 1);
      m_drawingSurface.setOption(TSLNOptionEnum.TSLNOptionDoubleBuffered, true);

      m_drawingSurface.reset(false);

      //activate zoom imode
      m_modeManager.setCurrentMode(ID_TOOLS_ZOOM);

      base.OnLoad(e);
    }

    private void OnClosed(object sender, FormClosedEventArgs e)
    {
      TSLNDrawingSurface.cleanup();
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

    // Create/setup the direct import datalayer
    private bool createDirectImportLayer(string fileName, bool isOpenStreetMap)
    {
      if (m_directImportDataLayer != null)
      {
        return false;
      }

      // The Direct Import layer requires an output coordinate system
      // Use the coordinate system of the map if one is loaded
      if (m_mapDataLayer != null)
      {
        m_displayCS = m_mapDataLayer.coordinateSystem;
      }

      // Otherwise get checked box
      if (m_displayCS == null)
      {
        m_displayCS = getCheckedDisplayCRS();
      }

      m_inputCS = getCheckedInputCRS();

      // Setup the Direct import layer
      m_directImportDataLayer = new TSLNDirectImportDataLayer();
      m_directImportDataLayerCallbacks = new DirectImportCallbacks(m_drawingSurface, m_directImportDataLayer, ViewerPanel, this);
      m_directImportDataLayer.setCallbacks(m_directImportDataLayerCallbacks);
      
      // Display using the data's coordinate system if none is set
      if (m_displayCS == null)
      {
        TSLNDirectImportDataSetList dataSets = m_directImportDataLayer.createDataSets(fileName);
        if( dataSets == null ||
            dataSets.Count == 0 )
        {
          return false;
        }
        TSLNDirectImportDataSet firstDataSet = dataSets[0];

        // The custom classifier provided by this application
        // will set rendering attributes based on 1MU == 1Meter
        // due to the saved settings in the rendition file.
        // If loading OSM we set the output coordinate system
        // to be world mercator in order to get the correct
        // rendering.
        m_displayCS = firstDataSet.coordinateSystem();
        if (m_displayCS == null)
        {
          return false;
        }

        if (isOpenStreetMap && m_displayCS.epsgID == 4326)
        {
          m_displayCS = TSLNCoordinateSystem.findByEPSG(3395);
        }
      }

      // The output coordinate system of the direct import layer must be set
      // prior to creating scale bands
      m_directImportDataLayer.coordinateSystem = m_displayCS;
      
      // Use a single scale band
      // This provides a flexible interface for loading vector and raster data,
      // in situations when nothing is known about the data
      m_directImportDataLayer.addScaleBand(0.0, "Default Scale Band");

      m_drawingSurface.addDataLayer(m_directImportDataLayer, "Direct Import");
      m_drawingSurface.setDataLayerProps("Direct Import", TSLNPropertyEnum.TSLNPropertyProgressiveDisplay, 1);

      return true;
    }

    //Open a MapLink Map or other data
    private void FileOpen()
    {
      FeatureConfig = null;
      FileOpenDialog.FileName = "";
      FileOpenDialog.ShowDialog();
      string fileName = FileOpenDialog.FileName;
      if (fileName == "")
      {
        return;
      }

      string ext = Path.GetExtension(fileName).ToLower();
      bool isOpenStreetMap = (ext == ".pbf" || ext == ".osm");
      if (ext == ".map" ||
          ext == ".mpc")
      {
        // A MapLink Map
        m_mapFileName = fileName;

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
      else
      {
        // Create the datalayer if needed
        if (m_directImportDataLayer == null &&
            !createDirectImportLayer(fileName, isOpenStreetMap))
        {
          string error = TSLNErrorStack.errorString("\nErrors:\n");
          MessageBox.Show(this, "Failed to create Direct Import Data Layer. " + error, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
          return;
        }

        // Other data, attempt to load using the direct import layer
        // This is a fairly simple example of how to load data, more complex scenarios may be found in:
        // - Samples/NT/DirectImportSimpleViewer
        // - Samples/Qt/OpenGLSurface/AdvancedSample

        // Open the data and extract datasets
        TSLNDirectImportDataSetList dataSets = m_directImportDataLayer.createDataSets(fileName);

        if (dataSets == null)
        {
          string error = TSLNErrorStack.errorString("\nErrors:\n");
          MessageBox.Show(this, "Failed to open data/create data sets. " + error, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
          return;
        }

        foreach (TSLNDirectImportDataSet dataSet in dataSets)
        {
          if (dataSet.type() == TSLNDirectImportDataSet.DataSetType.DataSetTypeVector)
          {
            VectorFeatureConfiguration vecConfigfrm = new VectorFeatureConfiguration(this, dataSet, isOpenStreetMap ? m_classifier : null);
            vecConfigfrm.ShowDialog(this);
          }

          // Load the dataset into the datalayer
          if (m_directImportDataLayer.loadData(dataSet, FeatureConfig) == false)
          {
            string error = TSLNErrorStack.errorString("\nErrors:\n");
            MessageBox.Show(this, "Failed to load data: " + dataSet.name() + error, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            return;
          }
        }
      }
      m_loaded = true;
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
        invalidate = m_drawingSurface.reset(false);
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

      string templateFilePath = TSLNDrawingSurface.findFile(templateFile);

      TSLNEntitySet entitySet = TSLNPrintDocument.loadTemplate(templateFilePath, TSLNDrawingSurface.fileLoader, null);

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

    private void InputCRS_Click(object sender, EventArgs e)
    {
      if (m_loaded)
      {
        MessageBox.Show(this, "The input CRS cannot be changed after a dataset is loaded.", "Error", MessageBoxButtons.OK);
        return;
      }
      this.InputCRSBNG.Checked = false;
      this.InputCRS4326.Checked = false;
      this.InputCRSData.Checked = false;
      if (sender == this.InputCRSBNG)
      {
        this.InputCRSBNG.Checked = true;
      }
      else if (sender == this.InputCRS4326)
      {
        this.InputCRS4326.Checked = true;
      }
      else if (sender == this.InputCRSData)
      {
        this.InputCRSData.Checked = true;
      }
      m_inputCS = getCheckedInputCRS();

    }

    private void DisplayCRS_Click(object sender, EventArgs e)
    {
      if (m_loaded)
      {
        MessageBox.Show(this, "The output CRS cannot be changed after a dataset is loaded.", "Error", MessageBoxButtons.OK);
        return;
      }
      this.DisplayCRSData.Checked = false;
      this.DisplayCRSBNG.Checked = false;
      this.DisplayCRS3395.Checked = false;
      this.DisplayCRS4326.Checked = false;
      if( sender == this.DisplayCRSData )
      {
        this.DisplayCRSData.Checked = true;
      }
      else if (sender == this.DisplayCRSBNG)
      {
        this.DisplayCRSBNG.Checked = true;
      }
      else if (sender == this.DisplayCRS3395)
      {
        this.DisplayCRS3395.Checked = true;
      }
      else if (sender == this.DisplayCRS4326)
      {
        this.DisplayCRS4326.Checked = true;
      }
      m_displayCS = getCheckedDisplayCRS();

    }

    private TSLNCoordinateSystem getCheckedDisplayCRS()
    {
      if( this.DisplayCRSData.Checked)
      {
        return null;
      }
      else if (this.DisplayCRSBNG.Checked)
      {
        return TSLNCoordinateSystem.findByEPSG(27700);
      }
      else if (this.DisplayCRS3395.Checked)
      {
        return TSLNCoordinateSystem.findByEPSG(3395);
      }
      else if (this.DisplayCRS4326.Checked)
      {
        return TSLNCoordinateSystem.findByEPSG(4326);
      }
      return TSLNCoordinateSystem.findByEPSG(4326);
    }

    private TSLNCoordinateSystem getCheckedInputCRS()
    {
      if (this.InputCRSBNG.Checked)
      {
        return TSLNCoordinateSystem.findByEPSG(27700);
      }
      else if (this.InputCRSData.Checked)
      {
        return null;
      }
      else if (this.InputCRS4326.Checked)
      {
        return TSLNCoordinateSystem.findByEPSG(4326);
      }
      return TSLNCoordinateSystem.findByEPSG(4326);
    }
  }
}
