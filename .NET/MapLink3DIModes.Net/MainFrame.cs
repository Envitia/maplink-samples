using System;
using System.Drawing;
using System.Collections;
using System.Windows.Forms;
using System.Runtime.InteropServices;

using Envitia.MapLink;
using Envitia.MapLink.ML3D;
using Envitia.MapLink.InteractionModes.ML3D;

namespace CSharp3DInteractionSample
{
  /// <summary>
  /// MapViewerForm
  /// </summary>
  public partial class MapViewerForm : Form, TSLN3DInteractionModeRequest
  {
    //Holds information about each interaction mode
    private class InteractionMode3D
    {
      //The id
      public int id;
      //The button on the tool bar that activates this mode
      public ToolStripButton button;
      //The button on the menu bar that activates this mode
      public ToolStripMenuItem menubutton;
    }

    //Interaction Mode IDS
    private static int ID_TOOLS_TRACKBALLEYEPOINT = 1;
    private static int ID_TOOLS_TRACKBALLWORLD = 2;

    //Camera defaults
    private const double DEFAULT_LATITUDE = (50.0);
    private const double DEFAULT_LONGITUDE = (0.0);
    private const double DEFAULT_ALTITUDE = (10000000.0);
    private const double LOOK_AHEAD_OFFSET = (-5.0);

    //Create the usual MapLink Item pointers
    private TSLN3DWinGLSurface m_drawingSurface = null;
    private TSLNMapDataLayer m_mapDataLayer = null;
    private TSLN3DTerrainDataLayer m_terrainDataLayer = null;

    //Create our array pointer of interaction mode information
    private InteractionMode3D[] m_imodeset = null;

    //Create interaction modes' pointers
    private TSLN3DInteractionModeManager m_modeManager = null;

    //Variables to hold if the control/shift button are currently pressed
    private bool m_controlButtonDown = false;
    private bool m_shiftButtonDown = false;

    //Short name of the loaded map
    private string m_shortfilename;

    public MapViewerForm()
    {
      InitializeComponent();
    }

    /// <summary>
    /// The main entry point for the application.
    /// </summary>
    [STAThread]
    static void Main()
    {
      Application.EnableVisualStyles();
      Application.Run(new MapViewerForm());
    }

    protected override void OnLoad(EventArgs e)
    {
      TSLN3DDrawingSurface.loadStandardConfig();

      m_drawingSurface = new TSLN3DWinGLSurface(ViewerPanel.Handle, false);

      m_drawingSurface.setOption(TSLNOptionEnum.TSLNOptionDoubleBuffered, true);
      m_drawingSurface.setOption(TSLNOptionEnum.TSLNOptionDynamicArcSupportEnabled, true);

      m_drawingSurface.wndResize(ViewerPanel.DisplayRectangle.Left, ViewerPanel.DisplayRectangle.Top, ViewerPanel.DisplayRectangle.Right, ViewerPanel.DisplayRectangle.Bottom, false);

      m_drawingSurface.backgroundColour = Color.FromArgb(0, 0, 173);

      string backdrop = TSLNUtilityFunctions.MapLinkHome;
      backdrop += "\\config\\earth.png";

      // Set the bitmap to display over the terrain plus colours for solid-backdrop and wireframe rendering.
      m_drawingSurface.setTerrainRendering(Color.Red, Color.FromArgb(51, 153, 255), backdrop);

      // Reduce the swimming of map textures displayed in the distance.
      m_drawingSurface.anisotropicFilter = true;
      m_drawingSurface.anisotropicFilterLevel = 4.0F;

      // Now create the mode manager
      m_modeManager = new TSLN3DInteractionModeManager(this, m_drawingSurface, ViewerPanel.Handle);

      m_modeManager.addMode(new TSLN3DInteractionModeTrackballWorld(ID_TOOLS_TRACKBALLWORLD, 30), false);
      m_modeManager.addMode(new TSLN3DInteractionModeTrackballEyepoint(ID_TOOLS_TRACKBALLEYEPOINT, 30), true);

      m_imodeset = new InteractionMode3D[2];
      m_imodeset[0] = new InteractionMode3D();
      m_imodeset[0].id = ID_TOOLS_TRACKBALLWORLD;
      m_imodeset[0].button = TrackballWorldButton;
      m_imodeset[0].menubutton = TrackballWorldMenu;
      m_imodeset[1] = new InteractionMode3D();
      m_imodeset[1].id = ID_TOOLS_TRACKBALLEYEPOINT;
      m_imodeset[1].button = TrackballEyepointButton;
      m_imodeset[1].menubutton = TrackballEyepointMenu;

      m_modeManager.onSize(ViewerPanel.DisplayRectangle.Width, ViewerPanel.DisplayRectangle.Height);

      m_mapDataLayer = new TSLNMapDataLayer();

      // In MapLink 5.0+, this will only work AFTER the drawing surface has been attached to a window
      m_drawingSurface.addDataLayer(m_mapDataLayer, "map");

      m_terrainDataLayer = new TSLN3DTerrainDataLayer();

      // In MapLink 5.0+, this will only work AFTER the drawing surface has been attached to a window
      m_drawingSurface.addDataLayer(m_terrainDataLayer, "terrain");


      m_drawingSurface.camera.reset();
      m_drawingSurface.camera.moveTo(DEFAULT_LATITUDE, DEFAULT_LONGITUDE, DEFAULT_ALTITUDE, TSLN3DCameraMoveAction.TSLN3DCameraMoveActionNone);
      m_drawingSurface.camera.lookAt(DEFAULT_LATITUDE, LOOK_AHEAD_OFFSET, 0.0, false);

      m_drawingSurface.setRenderingCallback(new TSLN3DRenderingCallback(renderingCallback), null);

      m_modeManager.resetViews();
      m_modeManager.resetMode(TSLNButtonType.TSLNButtonNone, 0, 0);

      // Store this view as the default.
      m_modeManager.setDefaultView();

      base.OnLoad(e);
    }

    //render callback function passed as a delegate to the drawing surface
    private void renderingCallback(Object arg, int pendingTextures)
    {
      ViewerPanel.Invalidate();
    }

    //TSLN3DInteractionModeRequest override
    public void resetMode(TSLN3DInteractionMode newMode, TSLNButtonType button, int xDU, int yDU)
    {
    }

    //TSLN3DInteractionModeRequest override
    public void viewChanged(TSLN3DDrawingSurface drawingSurface)
    {
    }

    //Paint of the ViewerPanel
    private void OnPaint(object sender, System.Windows.Forms.PaintEventArgs e)
    {
      if (m_drawingSurface != null)
      {
        m_drawingSurface.drawDU(e.ClipRectangle.Left, e.ClipRectangle.Top, e.ClipRectangle.Right, e.ClipRectangle.Bottom, false);
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
        m_drawingSurface.wndResize(ViewerPanel.DisplayRectangle.Left, ViewerPanel.DisplayRectangle.Top, ViewerPanel.DisplayRectangle.Right, ViewerPanel.DisplayRectangle.Bottom, false);
      }
      if (m_modeManager != null)
      {
        m_modeManager.onSize(ViewerPanel.DisplayRectangle.Width, ViewerPanel.DisplayRectangle.Height);
      }

      // Without the invalidate, the paint is only called when the window gets bigger
      ViewerPanel.Invalidate();
      updateLatLonAltReadout();
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
        updateLatLonAltReadout();
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
        ViewerPanel.Invalidate();
        updateLatLonAltReadout();
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
        updateLatLonAltReadout();
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
        updateLatLonAltReadout();
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

    private void OnKeyPress(object sender, KeyPressEventArgs e)
    {
      switch (e.KeyChar)
      {
        case 'w':
        case 'W':
          {
            bool wireframe = m_drawingSurface.get3DOption(TSLN3DOptionEnum.TSLN3DOptionWireframeMode);
            m_drawingSurface.set3DOption(TSLN3DOptionEnum.TSLN3DOptionWireframeMode, !wireframe);

            ViewerPanel.Invalidate();
          }
          break;

        case 'e':
          {
            m_drawingSurface.exaggerateTerrain(1.0, 1.0);
            ViewerPanel.Invalidate();
          }
          break;

        case 'E':
          {
            m_drawingSurface.exaggerateTerrain(100.0, 100.0);
            ViewerPanel.Invalidate();
          }
          break;

        default:
          break;
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

    //When the form closes
    private void OnClosed(object sender, FormClosedEventArgs e)
    {
      //MUST DISPOSE OF THE DRAWINGSURFACE OURSELVES
      m_drawingSurface.Dispose();
      m_drawingSurface = null;
      TSLNDrawingSurface.cleanup();
    }

    //Open a Map
    private void FileOpen_Click(object sender, System.EventArgs e)
    {
      if (FileOpenDialog.ShowDialog() != DialogResult.OK)
        return ;

      string mapFile = FileOpenDialog.FileName;
      if (mapFile == "")
      {
        return;
      }

      int indexOfLastSlash = mapFile.LastIndexOf('\\') + 1;
      m_shortfilename = mapFile.Substring(indexOfLastSlash, mapFile.Length - indexOfLastSlash);

      this.Text = "C# .NET 3D Interaction Modes Sample - " + m_shortfilename;

      if (!m_mapDataLayer.loadData(mapFile))
      {
        string error = TSLNErrorStack.errorString("Errors:\n", TSLNErrorCategory.TSLNErrorCategoryError | TSLNErrorCategory.TSLNErrorCategoryFatal);
        if (error != null && error != "")
        {
          MessageBox.Show(this, "MapLink has reported an error during loading of the selected map", "Error", MessageBoxButtons.OK);
        }
      }
      if (m_modeManager.resetToDefaultView())
      {
        ViewerPanel.Invalidate();
        updateLatLonAltReadout();
      }
    }

    private void FileOpenTerrain_Click(object sender, EventArgs e)
    {
      if (FileOpenTerrainDialog.ShowDialog() != DialogResult.OK)
        return;
      string terrainFile = FileOpenTerrainDialog.FileName;
      if (terrainFile == "")
      {
        return;
      }

      if (!m_terrainDataLayer.loadData(terrainFile))
      {
        string error = TSLNErrorStack.errorString("Errors:\n", TSLNErrorCategory.TSLNErrorCategoryError | TSLNErrorCategory.TSLNErrorCategoryFatal);
        if (error != null && error != "")
        {
          MessageBox.Show(this, "MapLink has reported an error during loading of the selected terrain database:\n" + error, "Error", MessageBoxButtons.OK);
        }
        else
        {
          MessageBox.Show(this, "MapLink has reported an error during loading of the selected terrain database", "Error", MessageBoxButtons.OK);
        }
      }
      if (m_modeManager.resetToDefaultView())
      {
        ViewerPanel.Invalidate();
        updateLatLonAltReadout();
      }
    }

    private void FileExitMenu_Click(object sender, EventArgs e)
    {
      this.Close();
    }

    private void ZoomIn_Click(object sender, EventArgs e)
    {
      if (m_modeManager.zoomIn(30, true))
      {
        ViewerPanel.Invalidate();
        updateLatLonAltReadout();
      }
    }

    private void ZoomOut_Click(object sender, EventArgs e)
    {
      if (m_modeManager.zoomOut(30, true))
      {
        ViewerPanel.Invalidate();
        updateLatLonAltReadout();
      }
    }

    private void Reset_Click(object sender, EventArgs e)
    {
      if (m_modeManager.resetToDefaultView())
      {
        ViewerPanel.Invalidate();
        updateLatLonAltReadout();
      }
    }

    //called when one of the imode menu items/toolbar buttons are clicked
    private void StripImode_Click(object sender, System.EventArgs e)
    {
      if (m_modeManager == null)
      {
        return;
      }

      ToolStripItem itemSender = (ToolStripItem)sender;
      for (int i = 0; i < m_imodeset.Length; ++i)
      {
        if (sender == m_imodeset[i].menubutton ||
            sender == m_imodeset[i].button)
        {
          m_modeManager.setCurrentMode(m_imodeset[i].id);
          break;
        }
      }
      UpdateToolsAndMenuButtons();
    }

    private void GotoPreviewView_Click(object sender, EventArgs e)
    {
      if (m_modeManager == null)
        return;

      if ( m_modeManager.viewStackGotoPrevious() )
        ViewerPanel.Invalidate();
    }

    private void GotoNextView_Click(object sender, EventArgs e)
    {
      if (m_modeManager == null)
        return;

      if (m_modeManager.viewStackGotoNext())
      {
        ViewerPanel.Invalidate();
        updateLatLonAltReadout();
      }
    }
    
    private void GotoSavedView_Click(object sender, EventArgs e)
    {
      ToolStripItem itemSender = (ToolStripItem)sender;
      if (itemSender == null || m_modeManager == null)
        return;

      int savedViewIndex = int.Parse((String)itemSender.Tag);

      if (m_modeManager.savedViewGoto(savedViewIndex))
      {
        ViewerPanel.Invalidate();
        updateLatLonAltReadout();
      }
    }

    private void SaveView_Click(object sender, EventArgs e)
    {
      ToolStripItem itemSender = (ToolStripItem)sender;
      if (itemSender == null || m_modeManager == null)
        return;

      int savedViewIndex = int.Parse((String)itemSender.Tag);
      m_modeManager.savedViewSetToCurrent(savedViewIndex);
    }

    private void ResetSavedViews_Click(object sender, EventArgs e)
    {
      if (m_modeManager == null)
      {
        return;
      }
      m_modeManager.savedViewReset();
    }

    //Show the about box
    private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
    {
      AboutBox abox = new AboutBox();
      abox.ShowDialog(this);
    }

    private void OnLatitudeLabelPaint(object sender, PaintEventArgs e)
    {
      if (m_drawingSurface != null)
      {
        double lat, lon, alt;
        m_drawingSurface.camera.position(out lat, out lon, out alt);

        LatitudeLabel.Text = lat.ToString();
      }
    }

    private void OnLongitudeLabelPaint(object sender, PaintEventArgs e)
    {
      if (m_drawingSurface != null)
      {
        double lat, lon, alt;
        m_drawingSurface.camera.position(out lat, out lon, out alt);

        LongitudeLabel.Text = lon.ToString();
      }
    }

    private void OnAltitudeLabelPaint(object sender, PaintEventArgs e)
    {
      if (m_drawingSurface != null)
      {
        double lat, lon, alt;
        m_drawingSurface.camera.position(out lat, out lon, out alt);

        AltitudeLabel.Text = alt.ToString();
      }
    }

    //called whenever a new imode is selected
    private void UpdateToolsAndMenuButtons()
    {
      long id = m_modeManager.currentModeID;

      for (int i = 0; i < m_imodeset.Length; ++i)
      {
        if (id == m_imodeset[i].id)
        {
          m_imodeset[i].button.CheckState = CheckState.Checked;
          m_imodeset[i].menubutton.Checked = true;
        }
        else
        {
          m_imodeset[i].button.CheckState = CheckState.Unchecked;
          m_imodeset[i].menubutton.Checked = false;
        }
      }
    }

    private void updateLatLonAltReadout()
    {
      LatitudeLabel.Invalidate();
      LongitudeLabel.Invalidate();
      AltitudeLabel.Invalidate();
    }

    
  }
}
