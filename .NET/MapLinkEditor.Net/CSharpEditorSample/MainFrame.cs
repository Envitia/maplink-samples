using System;
using System.Drawing;
using System.Collections;
using System.Windows.Forms;
using Envitia.MapLink;

namespace CSharpEditorSample
{
  /// <summary>
  /// MapViewerForm
  /// </summary>
  public partial class MapViewerForm : Form
  {
    
    //Usual MapLink items
    private TSLNDrawingSurface m_drawingSurface = null;
    private TSLNMapDataLayer m_mapDataLayer = null;
    private TSLNStandardDataLayer m_editLayer = null;
    private TSLNGridDataLayer m_gridLayer = null;
    private OpManager m_opman = null;

    //Short name of the loaded map
    private string m_shortfilename;
    //Currently loaded TMF filename
    private string m_tmffilename = null;

    private const int FALSE = 0, TRUE = 1;

    private int m_gridLayerVisible = FALSE;
    private double m_gridScale = 100;          // grid units per MU
    private double m_gridMajorInterval = 100;  // grid units
    private double m_gridMinorInterval = 10;   // grid units
    private int m_gridXOrigin = 0;             // TMC
    private int m_gridYOrigin = 0;             // TMC

    private bool m_gridLayerVisible_bool
    {
      get { return m_gridLayerVisible != FALSE; }
      set { m_gridLayerVisible = (value ? TRUE : FALSE); }
    }

    public MapViewerForm()
    {
      //To overwrite the inifile loading path, modify the below line
      //TSLNProfileHelper.setDefaultFilename("");

      string iniFile = TSLNProfileHelper.getDefaultFilename();

      if (iniFile == null)
      {
        // No ini file, use the default MapLink one
        string maplinkHome = TSLNUtilityFunctions.getMapLinkHome();
        if (maplinkHome != null)
        {
          string filename = maplinkHome;
          filename += "\\samples\\nt\\CSharpEditorSample\\CSharpEditorSample.ini";

          if (System.IO.File.Exists(filename))
          {
            TSLNProfileHelper.setDefaultFilename(filename);
          }
        }

        iniFile = TSLNProfileHelper.getDefaultFilename();

        if (iniFile == null)
        {
          MessageBox.Show(this, "No initialisation file specified on the command line eg. '-I CSharpEditorSample.ini'\n", "Add this to the shortcut parameters or Settings->Debug->Program arguments", MessageBoxButtons.OK, MessageBoxIcon.Error);
          TSLNDrawingSurface.cleanup();
          Environment.Exit(-1);
        }
      }

      InitializeComponent();
      this.MouseWheel += new MouseEventHandler(OnMouseWheel);

      m_opman = new OpManager();
      PreferencesPanel.SetOpManager(m_opman);
      AttributePanel.SetOpManager(m_opman);

      //The resource designer has a tendency of rearranging the Toolstrip controls
      //So we will hard code the locations to ensure they look tidy.
      //Need to temporarily remove the controls while doing this, since changing Location of a later toolstrip
      //moves earlier toolstrips (even when layout is suspended).
      toolStripContainer1.SuspendLayout();

      ToolStrip[] desiredOrder = { MainToolStrip, EditToolStrip, ToolsToolStrip, SelectToolStrip, ConstraintsToolStrip };
      if (desiredOrder.Length != this.toolStripContainer1.TopToolStripPanel.Controls.Count)
        throw new InvalidOperationException("top toolbar changed - need to update MapViewerForm c'tor code");
      this.toolStripContainer1.TopToolStripPanel.Controls.Clear();
      int xLocation = 3;
      foreach (ToolStrip strip in desiredOrder)
      {
        strip.Location = new Point(xLocation, 0);
        xLocation += strip.Size.Width;
      }
      foreach (ToolStrip strip in desiredOrder)
      {
        this.toolStripContainer1.TopToolStripPanel.Controls.Add(strip);
      }
            
      desiredOrder = new[] { PrimitiveToolStrip, PointToolStrip, TransformsToolStrip };
      if (desiredOrder.Length != this.toolStripContainer1.RightToolStripPanel.Controls.Count)
        throw new InvalidOperationException("right toolbar changed - need to update MapViewerForm c'tor code");
      this.toolStripContainer1.RightToolStripPanel.Controls.Clear();
      int yLocation = 3;
      foreach (ToolStrip strip in desiredOrder)
      {
        strip.Location = new Point(0, yLocation);
        yLocation += strip.Size.Height;
      }
      foreach (ToolStrip strip in desiredOrder)
      {
        this.toolStripContainer1.RightToolStripPanel.Controls.Add(strip);
      }

      toolStripContainer1.ResumeLayout();
    }

    protected override void OnLoad(EventArgs e)
    {
      TSLNProfileHelper.setDefaultSection("grid");
      TSLNProfileHelper.lookupProfileUsingDefaults("gridScale", out m_gridScale, 100.0);
      TSLNProfileHelper.lookupProfileUsingDefaults("gridMajorInterval", out m_gridMajorInterval, 100.0);
      TSLNProfileHelper.lookupProfileUsingDefaults("gridMinorInterval", out m_gridMinorInterval, 10.0);
      TSLNProfileHelper.lookupProfileUsingDefaults("gridXOrigin", out m_gridXOrigin, 0);
      TSLNProfileHelper.lookupProfileUsingDefaults("gridYOrigin", out m_gridYOrigin, 0);
      // Set up the grid rendering - the units will be set later on
      int gridMajorLineColour = 7, gridMajorLineStyle = 1, gridMajorLineThickness = 1;
      int gridMinorLineColour = 7, gridMinorLineStyle = 1, gridMinorLineThickness = 1;
      int gridAxisColour = 7, gridAxisStyle = 1, gridAxisThickness = 1;
      TSLNProfileHelper.lookupProfileUsingDefaults("gridLayerVisible", out m_gridLayerVisible, FALSE);
      TSLNProfileHelper.lookupProfileUsingDefaults("gridMajorLineColour", out gridMajorLineColour, 7);
      TSLNProfileHelper.lookupProfileUsingDefaults("gridMajorLineStyle", out gridMajorLineStyle, 1);
      TSLNProfileHelper.lookupProfileUsingDefaults("gridMajorLineThickness", out gridMajorLineThickness, 1);
      TSLNProfileHelper.lookupProfileUsingDefaults("gridMinorLineColour", out gridMinorLineColour, 7);
      TSLNProfileHelper.lookupProfileUsingDefaults("gridMinorLineStyle", out gridMinorLineStyle, 1);
      TSLNProfileHelper.lookupProfileUsingDefaults("gridMinorLineThickness", out gridMinorLineThickness, 1);
      TSLNProfileHelper.lookupProfileUsingDefaults("gridAxisColour", out gridAxisColour, 7);
      TSLNProfileHelper.lookupProfileUsingDefaults("gridAxisStyle", out gridAxisStyle, 1);
      TSLNProfileHelper.lookupProfileUsingDefaults("gridAxisThickness", out gridAxisThickness, 1);

      // Attach the drawing surface to the panel 
      m_drawingSurface = new TSLNDrawingSurface(ViewerPanel.Handle, false);
     
      //Create the mapdata layer
      m_mapDataLayer = new TSLNMapDataLayer();
      //Create edit layer 
      m_editLayer = new TSLNStandardDataLayer();
      //Create the grid layer
      m_gridLayer = new TSLNGridDataLayer();

      m_drawingSurface.wndResize(ViewerPanel.DisplayRectangle.Left, ViewerPanel.DisplayRectangle.Top, ViewerPanel.DisplayRectangle.Right, ViewerPanel.DisplayRectangle.Bottom, false, TSLNResizeActionEnum.TSLNResizeActionMaintainCentre);
      
      // TSLNPropertyDetect allows editor operations to snap to this layer's entities (needed by e.g. lasso operation).
      // TSLNPropertySelect allows user to select/deselect this layer's entities.

      m_drawingSurface.addDataLayer(m_mapDataLayer, "map");
      m_drawingSurface.setDataLayerProps("map", TSLNPropertyEnum.TSLNPropertyBuffered, TRUE);
      m_drawingSurface.setDataLayerProps("map", TSLNPropertyEnum.TSLNPropertyDetect, TRUE);

      m_drawingSurface.addDataLayer(m_gridLayer, "grid");
      m_drawingSurface.setDataLayerProps("grid", TSLNPropertyEnum.TSLNPropertyVisible, m_gridLayerVisible);
      m_drawingSurface.setDataLayerProps("grid", TSLNPropertyEnum.TSLNPropertyDetect, TRUE);

      m_drawingSurface.addDataLayer(m_editLayer, "std");
      m_drawingSurface.setDataLayerProps("std", TSLNPropertyEnum.TSLNPropertyVisible, TRUE);
      m_drawingSurface.setDataLayerProps("std", TSLNPropertyEnum.TSLNPropertyDetect,  TRUE);
      m_drawingSurface.setDataLayerProps("std", TSLNPropertyEnum.TSLNPropertySelect,  TRUE);

      m_drawingSurface.setOption(TSLNOptionEnum.TSLNOptionDoubleBuffered, true);

      m_drawingSurface.reset(false);

      CreateImodes(m_opman);

      TSLNRenderingAttributes majorAttr = new TSLNRenderingAttributes
      {
        edgeColour = gridMajorLineColour,
        edgeStyle = gridMajorLineStyle,
        edgeThickness = gridMajorLineThickness,
      };
      TSLNRenderingAttributes minorAttr = new TSLNRenderingAttributes
      {
        edgeColour = gridMinorLineColour,
        edgeStyle = gridMinorLineStyle,
        edgeThickness = gridMinorLineThickness,
      };
      TSLNRenderingAttributes gridAxisAttr = new TSLNRenderingAttributes
      {
        edgeColour = gridAxisColour,
        edgeStyle = gridAxisStyle,
        edgeThickness = gridAxisThickness,
      };

      m_gridLayer.setFeatureRendering("xmajor", 0, majorAttr);
      m_gridLayer.setFeatureRendering("xminor", 0, majorAttr);
      m_gridLayer.setFeatureRendering("xminor", 0, minorAttr);
      m_gridLayer.setFeatureRendering("yminor", 0, minorAttr);
      m_gridLayer.setFeatureRendering("xaxis", 0, gridAxisAttr);
      m_gridLayer.setFeatureRendering("yaxis", 0, gridAxisAttr);
      m_gridLayer.snapable = false;


      TSLNProfileHelper.setDefaultSection( "paths" ) ;

      // Load a map if one is specified in the ini file
      string mapName ;

      TSLNProfileHelper.lookupProfileUsingDefaults("map", out mapName, "", -1, 256);
      
      if ( mapName.Length > 0 )
      {
        // Firstly check for non-absolute path
        if ( !System.IO.Path.IsPathRooted(mapName) )
        {
          string inifilename = TSLNProfileHelper.getDefaultFilename();
          string rootDir = System.IO.Path.GetDirectoryName(inifilename);
          mapName = System.IO.Path.Combine(rootDir, mapName);
        }

        LoadMap( mapName ) ;
      }

      base.OnLoad(e);
    }

    //Paint of the ViewerPanel
    private void OnPaint(object sender, System.Windows.Forms.PaintEventArgs e)
    {
      if (m_drawingSurface != null)
      {
        m_drawingSurface.drawDU(e.ClipRectangle.Left, e.ClipRectangle.Top, e.ClipRectangle.Right, e.ClipRectangle.Bottom, false, false);
      }
      if (m_currentMode != null)
      {
        m_currentMode.OnDraw(e.ClipRectangle.Left, e.ClipRectangle.Top, e.ClipRectangle.Right, e.ClipRectangle.Bottom);
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

    private void FileOpenMap(object sender, System.EventArgs e)
    {
      DialogResult dr = FileOpenMapDialog.ShowDialog();
      string mapFile = FileOpenMapDialog.FileName;

      if (mapFile == "" || dr != DialogResult.OK)
      {
        return;
      }

      LoadMap(mapFile);
    }

    private void LoadMap(string mapFile)
    {
      int indexOfLastSlash = mapFile.LastIndexOf('\\') + 1;
      m_shortfilename = mapFile.Substring(indexOfLastSlash, mapFile.Length - indexOfLastSlash);

      this.Text = "C# .NET Editor Sample - " + m_shortfilename;

      if (!m_mapDataLayer.loadData(mapFile))
      {
        string error = TSLNErrorStack.errorString("Errors:\n", TSLNErrorCategory.TSLNErrorCategoryError | TSLNErrorCategory.TSLNErrorCategoryFatal);
        if (error != "")
        {
          MessageBox.Show(this, error, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }
      }

      // Map successfully loaded, set the grid spacing
      m_gridLayer.setUnits((int)(m_gridScale * m_mapDataLayer.TMCperMU), m_gridXOrigin, m_gridYOrigin);
      m_gridLayer.setSpacing("xmajor", m_gridMajorInterval);
      m_gridLayer.setSpacing("ymajor", m_gridMajorInterval);
      m_gridLayer.setSpacing("xminor", m_gridMinorInterval);
      m_gridLayer.setSpacing("yminor", m_gridMinorInterval);

      m_drawingSurface.reset(true);
    }

    private void FileOpenGeometry(object sender, EventArgs e)
    {
      DialogResult dr = FileOpenTMFDialog.ShowDialog();
      string tmfFile = FileOpenTMFDialog.FileName;

      if (tmfFile == "" || dr != DialogResult.OK)
      {
        return;
      }

      m_opman.reset();

      if (!m_editLayer.loadData(tmfFile))
      {
        string error = TSLNErrorStack.errorString("Errors:\n", TSLNErrorCategory.TSLNErrorCategoryError | TSLNErrorCategory.TSLNErrorCategoryFatal);
        if (error != "")
        {
          MessageBox.Show(this, error, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }
      }
      m_tmffilename = tmfFile;

      m_opman.dataChanged();
      m_drawingSurface.reset(true);

    }

    private void FileNewGeometry(object sender, EventArgs e)
    {
      m_tmffilename = null;
      m_opman.reset();
      if (!m_editLayer.removeData())
      {
        string error = TSLNErrorStack.errorString("Errors:\n", TSLNErrorCategory.TSLNErrorCategoryError | TSLNErrorCategory.TSLNErrorCategoryFatal);
        if (error != "")
        {
          MessageBox.Show(this, error, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }
      }
      m_opman.dataChanged();
      m_drawingSurface.reset(true);
    }

    private void FileSaveGeometry(object sender, EventArgs e)
    {
      if (m_tmffilename == null)
      {
        FileSaveAsGeometry(sender, e);
        return;
      }

      if (!m_editLayer.saveData(m_tmffilename))
      {
        string error = TSLNErrorStack.errorString("Errors:\n", TSLNErrorCategory.TSLNErrorCategoryError | TSLNErrorCategory.TSLNErrorCategoryFatal);
        if (error != "")
        {
          MessageBox.Show(this, error, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }
      }
    }

    private void FileSaveAsGeometry(object sender, EventArgs e)
    {
      FileSaveTMFDialog.FileName = m_tmffilename == null ? "" : m_tmffilename;
      DialogResult dr = FileSaveTMFDialog.ShowDialog();
      m_tmffilename = FileSaveTMFDialog.FileName;

      if (m_tmffilename == "" || dr != DialogResult.OK)
      {
        m_tmffilename = null;
        return;
      }

      FileSaveGeometry(sender, e);
    }

    private void FilePrintPreview(object sender, EventArgs e)
    {
      print(true);
    }

    //called when File->Print is clicked from the menu
    private void FilePrintMenu_Click(object sender, System.EventArgs e)
    {
      print(false);
    }

    private void print ( bool previewOnly )
    {
      TSLNPrintItem[] printItems = (TSLNPrintItem[])System.Array.CreateInstance(typeof(TSLNPrintItem), 1);
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
      int tmcPerMetre = printItem.tmcPerMetre;
      printItem.drawingSurface = m_drawingSurface;
      printItem.extent(envelope);
      printItem.borderColour = Color.Black;
      printItem.borderStyle = 1;
      printItem.borderThickness = 2;
      printItem.templateName = "map";

      System.DateTime dt = System.DateTime.Now;
      string[] textMappings = new string[4];
      textMappings[0] = "MapLink Pro";
      textMappings[1] = m_shortfilename;
      textMappings[2] = "date";
      textMappings[3] = dt.ToShortDateString();

      try
      {
        TSLNPrintDocument pd = new TSLNPrintDocument(m_drawingSurface, entitySet, printItems, textMappings, isLandscape);
        if (previewOnly)
        {
          PrintPreviewDialog dlg = new PrintPreviewDialog();
          dlg.Document = pd;
          DialogResult result = dlg.ShowDialog(this);
          if (result == DialogResult.OK)
          {
            pd.Print();
          }
        }
        else
        {
          PrintDialog dlg = new PrintDialog();
          dlg.Document = pd;
          DialogResult result = dlg.ShowDialog(this);
          if (result == DialogResult.OK)
          {
            pd.Print();
          }
        }
      }
      catch (Exception ex)
      {
        MessageBox.Show(this, "An error occurred attempting to print -  " + ex.Message);
      }
    }

    private void FileExitMenu_Click(object sender, EventArgs e)
    {
      this.Close();
    }

    private void menu_View_AttributesPanel_Click(object sender, EventArgs e)
    {
      menu_View_AttributesPanel.Checked = !menu_View_AttributesPanel.Checked;
      AttributesToolStrip.Visible = menu_View_AttributesPanel.Checked;
    }

    private void menu_View_PreferencesPanel_Click(object sender, EventArgs e)
    {
      menu_View_PreferencesPanel.Checked = !menu_View_PreferencesPanel.Checked;
      PreferencesToolStrip.Visible = menu_View_PreferencesPanel.Checked;
    }

    private void menu_View_Statusbar_Click(object sender, EventArgs e)
    {
      menu_View_Statusbar.Checked = !menu_View_Statusbar.Checked;
      TheStatusStrip.Visible = menu_View_Statusbar.Checked;
    }

    private void menu_View_GridLayer_Click(object sender, EventArgs e)
    {
      m_gridLayerVisible_bool = !m_gridLayerVisible_bool;
      menu_View_GridLayer.Checked = m_gridLayerVisible_bool;
      m_drawingSurface.setDataLayerProps("grid", TSLNPropertyEnum.TSLNPropertyVisible, m_gridLayerVisible);
      m_gridLayer.snapable = m_gridLayerVisible_bool;   // if we can see it, we snap to it
      ViewerPanel.Invalidate();
    }

    private void menu_View_ToolbarsMain_Click(object sender, EventArgs e)
    {
      menu_View_ToolbarsMain.Checked = !menu_View_ToolbarsMain.Checked;
      MainToolStrip.Visible = menu_View_ToolbarsMain.Checked;
    }

    private void menu_View_ToolbarsEdit_Click(object sender, EventArgs e)
    {
      menu_View_ToolbarsEdit.Checked = !menu_View_ToolbarsEdit.Checked;
      EditToolStrip.Visible = menu_View_ToolbarsEdit.Checked;
    }

    private void menu_View_ToolbarsSelect_Click(object sender, EventArgs e)
    {
      menu_View_ToolbarsSelect.Checked = !menu_View_ToolbarsSelect.Checked;
      SelectToolStrip.Visible = menu_View_ToolbarsSelect.Checked;
    }

    private void menu_View_ToolbarsTools_Click(object sender, EventArgs e)
    {
      menu_View_ToolbarsTools.Checked = !menu_View_ToolbarsTools.Checked;
      ToolsToolStrip.Visible = menu_View_ToolbarsTools.Checked;
    }

    private void menu_View_ToolbarsConstraints_Click(object sender, EventArgs e)
    {
      menu_View_ToolbarsConstraints.Checked = !menu_View_ToolbarsConstraints.Checked;
      ConstraintsToolStrip.Visible = menu_View_ToolbarsConstraints.Checked;
    }

    private void menu_View_ToolbarsPrimitives_Click(object sender, EventArgs e)
    {
      menu_View_ToolbarsPrimitives.Checked = !menu_View_ToolbarsPrimitives.Checked;
      PrimitiveToolStrip.Visible = menu_View_ToolbarsPrimitives.Checked;
    }

    private void menu_View_ToolbarsTransforms_Click(object sender, EventArgs e)
    {
      menu_View_ToolbarsTransforms.Checked = !menu_View_ToolbarsTransforms.Checked;
      TransformsToolStrip.Visible = menu_View_ToolbarsTransforms.Checked;
    }

    private void menu_View_ToolbarsPointOperations_Click(object sender, EventArgs e)
    {
      menu_View_ToolbarsPointOperations.Checked = !menu_View_ToolbarsPointOperations.Checked;
      PointToolStrip.Visible = menu_View_ToolbarsPointOperations.Checked;
    }

    public string CursorReadout
    {
      set
      {
        MouseReadout.Text = value;
      }
    }

    public string Prompt
    {
      set
      {
        StatusString.Text = value;
      }
    }

    public int SelectedEntityCount
    {
      set
      {
        SelectedEntityCountString.Text = String.Format("Selected: {0}", value);
      }
    }

    private void OnAppAbout(object sender, EventArgs e)
    {
      AboutBox abox = new AboutBox();
      abox.ShowDialog(this);
    }

    private void menu_File_Exit_Click(object sender, EventArgs e)
    {
      this.Close();
    }

    
  }
}
