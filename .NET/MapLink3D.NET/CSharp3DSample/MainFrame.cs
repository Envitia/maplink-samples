using System;
using System.Drawing;
using System.Collections;
using System.Windows.Forms;
using Envitia.MapLink;
using Envitia.MapLink.ML3D;

namespace CSharp3DSample
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
      //The view mode
      public TSL3DViewMode mode = null;
    }

    //Interaction Mode IDS
    private const int ID_TOOLS_ROTATE = 1;
    private const int ID_TOOLS_FLY = 2;
    private const int ID_TOOLS_CAMERA = 3;
    private const int ID_TOOLS_ZOOM = 4;

    private const double DEFAULT_LATITUDE   = (50.0);
    private const double DEFAULT_LONGITUDE  = (0.0);
    private const double DEFAULT_ALTITUDE   = (10000000.0);

    //MapLink items
    private TSLN3DWinGLSurface m_drawingSurface = null;
    private TSLNMapDataLayer m_mapDataLayer = null;
    private TSLN3DTerrainDataLayer m_terrainDataLayer = null;
    private TSLN3DStandardDataLayer m_stdDataLayer = null;

    //Create our array of interaction mode information classes
    private InteractionMode[] m_imodeset = null;

    //Create interaction modes interface
    TSL3DViewModeInterface m_modeInterface = null;

    //Reference to the current imode
    private TSL3DViewMode m_current = null;

    //Variables to hold if the control/shift button are currently pressed
    private bool m_controlButtonDown = false;
    private bool m_shiftButtonDown = false;

    //Short name of the loaded map
    private string m_shortfilename;


    public MapViewerForm()
    {
      InitializeComponent();
      this.MouseWheel += new MouseEventHandler(OnMouseWheel);
    }

    protected override void OnLoad(EventArgs e)
    {
      // Attach the drawing surface to the panel 
      m_drawingSurface = new TSLN3DWinGLSurface(ViewerPanel.Handle, false);

      //Create the mapdata layer
      m_mapDataLayer = new TSLNMapDataLayer();

      //Create the terrain data layer
      m_terrainDataLayer = new TSLN3DTerrainDataLayer();

      //Create 3d standard data layer
      m_stdDataLayer = new TSLN3DStandardDataLayer();

      //Create our storage for the imodes
      m_imodeset = new InteractionMode[4];

      //Create the interface from the imodes to the surface and panel
      m_modeInterface = new TSL3DViewModeInterface(m_drawingSurface, ViewerPanel.Handle);

      //Create the mode sets. These are use by the toolbar and the menu to 
      //identify the imodes
      m_imodeset[0] = new InteractionMode();
      m_imodeset[0].id = ID_TOOLS_ROTATE;
      m_imodeset[0].button = RotateTool;
      m_imodeset[0].menubutton = ToolsRotate;
      m_imodeset[0].mode = new TSL3DViewModeRotate(m_modeInterface, ID_TOOLS_ROTATE);
      m_imodeset[1] = new InteractionMode();
      m_imodeset[1].id = ID_TOOLS_FLY;
      m_imodeset[1].button = FlyTool;
      m_imodeset[1].menubutton = ToolsFly;
      m_imodeset[1].mode = new TSL3DViewModeFly(m_modeInterface, ID_TOOLS_FLY);
      m_imodeset[2] = new InteractionMode();
      m_imodeset[2].id = ID_TOOLS_CAMERA;
      m_imodeset[2].button = CameraTool;
      m_imodeset[2].menubutton = ToolsCamera;
      m_imodeset[2].mode = new TSL3DViewModeCamera(m_modeInterface, ID_TOOLS_CAMERA);
      m_imodeset[3] = new InteractionMode();
      m_imodeset[3].id = ID_TOOLS_ZOOM;
      m_imodeset[3].button = ZoomTool;
      m_imodeset[3].menubutton = ToolsZoom;
      m_imodeset[3].mode = new TSL3DViewModeZoom(m_modeInterface, ID_TOOLS_ZOOM);

      m_drawingSurface.wndResize(ViewerPanel.DisplayRectangle.Left, ViewerPanel.DisplayRectangle.Top, ViewerPanel.DisplayRectangle.Right, ViewerPanel.DisplayRectangle.Bottom, false);

      //add the map data layer to the drawing surface
      m_drawingSurface.addDataLayer(m_mapDataLayer, "map");

      //add the terrain data layer to the drawing surface
      m_drawingSurface.addDataLayer(m_terrainDataLayer, "terrain");

      //add the 3d standard data layer to the drawing surface
      m_drawingSurface.addDataLayer(m_stdDataLayer, "std3d");
      
      //turn on double buffering
      m_drawingSurface.setOption(TSLNOptionEnum.TSLNOptionDoubleBuffered, true);

      //set the background colour
      m_drawingSurface.backgroundColour = Color.FromArgb(0, 0, 173) ;

      string backdrop = TSLNUtilityFunctions.MapLinkHome;
      backdrop += "\\config\\earth.png";

      // Set the bitmap to display over the terrain plus colours for solid-backdrop and wireframe rendering.
      m_drawingSurface.setTerrainRendering(Color.Red, Color.FromArgb(51, 153, 255), backdrop);

      //add a rendering callback so that the view gets invalidated when background rendering has
      //completed.
      m_drawingSurface.setRenderingCallback(new TSLN3DRenderingCallback(renderingCallback), null);

      // and reset the current view to display the entire map.
      resetCamera();

      //activate zoom imode
      m_current = m_imodeset[3].mode;

      base.OnLoad(e);
    }

    private void OnClosed(object sender, FormClosedEventArgs e)
    {
      //MUST DISPOSE OF THE DRAWINGSURFACE AND STANDARDDATALAYER OURSELVES
      m_stdDataLayer.Dispose();
      m_stdDataLayer = null;
      m_drawingSurface.Dispose();
      m_drawingSurface = null;
    }

    //Paint of the ViewerPanel
    private void OnPaint(object sender, System.Windows.Forms.PaintEventArgs e)
    {
      if (m_drawingSurface != null)
      {
        m_drawingSurface.drawDU(e.ClipRectangle.Left, e.ClipRectangle.Top, e.ClipRectangle.Right, e.ClipRectangle.Bottom, false);
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
        m_drawingSurface.wndResize(ViewerPanel.DisplayRectangle.Left, ViewerPanel.DisplayRectangle.Top, ViewerPanel.DisplayRectangle.Right, ViewerPanel.DisplayRectangle.Bottom, false);
      }

      // Without the invalidate, the paint is only called when the window gets bigger
      ViewerPanel.Invalidate();
    }

    //Open a Map
    private void FileOpenMap()
    {
      FileOpenMapDialog.ShowDialog();
      string mapFile = FileOpenMapDialog.FileName;

      if (mapFile == "")
      {
        return;
      }

      int indexOfLastSlash = mapFile.LastIndexOf('\\') + 1;
      m_shortfilename = mapFile.Substring(indexOfLastSlash, mapFile.Length - indexOfLastSlash);

      this.Text = "C# 3D .NET Sample - " + m_shortfilename;

      if (!m_mapDataLayer.loadData(mapFile))
      {
        string error = TSLNErrorStack.errorString("Errors:\n", TSLNErrorCategory.TSLNErrorCategoryError | TSLNErrorCategory.TSLNErrorCategoryFatal);
        if (error != null && error != "")
        {
          MessageBox.Show(this, error, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }
      }
      resetCamera();
    }

    private void FileOpenMenu_Click(object sender, System.EventArgs e)
    {
      FileOpenMap();
    }

    //This is called when any of the buttons on the Toolbar is clicked
    private void TheToolBar_ButtonClick(object sender, System.Windows.Forms.ToolBarButtonClickEventArgs e)
    {
      bool invalidate = false;
      if (e.Button == FileOpenButton)
      {
        FileOpenMap();
      }
      else if (e.Button == EditCopyButton)
      {
        CopyToClipboard();
      }
      else if (e.Button == ResetButton)
      {
        resetCamera();
        invalidate = true;
      }
      else
      {
        for (int i = 0; i < m_imodeset.Length; ++i)
        {
          if (e.Button == m_imodeset[i].button)
          {
            m_current = m_imodeset[i].mode;
            invalidate = m_current.activate(this);
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
        updateLatLonAltReadout();
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
        updateLatLonAltReadout();
        ViewerPanel.Invalidate();
      }
    }

    //On mouse wheel on the ViewerPanel
    private void OnMouseWheel(object sender, System.Windows.Forms.MouseEventArgs e)
    {
      if (m_current == null)
        return;
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
          m_current.activate(this);
          break;
        }
      }
      UpdateToolsAndMenuButtons();
    }

    //called whenever one of the base-level non imode menu buttons on the Tools
    //menu are clicked
    private void MenuNonImode_Click(object sender, System.EventArgs e)
    {
      if (sender == this.ToolsReset)
      {
        resetCamera();
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
    private void EditCopyMenu_Click(object sender, System.EventArgs e)
    {
      CopyToClipboard();
    }

    private void FileExitMenu_Click(object sender, EventArgs e)
    {
      this.Close();
    }
    [System.Runtime.InteropServices.DllImport("gdi32.dll")]
    private static extern bool BitBlt(IntPtr hdcDest, 
                                      int nXDest,
                                      int nYDest,
                                      int nWidth,
                                      int nHeight,
                                      IntPtr hdcSrc,
                                      int nXSrc,
                                      int nYSrc,
                                      int dwRop);

    private void CopyToClipboard()
    {
      Bitmap bi = new Bitmap(ViewerPanel.ClientRectangle.Width, ViewerPanel.ClientRectangle.Height);
      Graphics bitmapGraphics = Graphics.FromImage(bi);
      IntPtr bitmapHDC = bitmapGraphics.GetHdc();

      IntPtr wnd = ViewerPanel.Handle;
      Graphics windowGraphics = Graphics.FromHwnd(wnd);
      IntPtr windowHDC = windowGraphics.GetHdc();

      BitBlt(bitmapHDC,
             ViewerPanel.ClientRectangle.Left,
             ViewerPanel.ClientRectangle.Top,
             ViewerPanel.ClientRectangle.Width,
             ViewerPanel.ClientRectangle.Height,
             windowHDC,
             0, 0,
             0x00CC0020 /*SRCCOPY*/);

      windowGraphics.ReleaseHdc(windowHDC);
      bitmapGraphics.ReleaseHdc(bitmapHDC);
      windowGraphics.Dispose();
      bitmapGraphics.Dispose();

      Clipboard.SetImage(bi);
    }

    private void renderingCallback(Object arg, int pendingTextures)
    {
      ViewerPanel.Invalidate();
    }

    private void resetCamera()
    {
      if (m_drawingSurface != null)
      {
        m_drawingSurface.camera.reset();
        m_drawingSurface.camera.moveTo(DEFAULT_LATITUDE, DEFAULT_LONGITUDE, DEFAULT_ALTITUDE, TSLN3DCameraMoveAction.TSLN3DCameraMoveActionNone);
        m_drawingSurface.camera.lookAt(DEFAULT_LATITUDE, TSL3DViewMode.LOOK_AHEAD_OFFSET, 0.0, false);
      }
      updateLatLonAltReadout();
    }

    private void updateLatLonAltReadout()
    {
      LatitudeLabel.Invalidate();
      LongitudeLabel.Invalidate();
      AltitudeLabel.Invalidate();
    }

    private void OnKeyPress(object sender, KeyPressEventArgs e)
    {
      switch ( e.KeyChar )
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
          m_drawingSurface.exaggerateTerrain( 1.0, 1.0 );
          ViewerPanel.Invalidate();
        }
        break;

      case 'E':
        {
          m_drawingSurface.exaggerateTerrain( 10.0, 10.0 );
          ViewerPanel.Invalidate();
        }
        break;

      default:
        break;
      }
    }

    private void FileOpenTerrainMenu_Click(object sender, EventArgs e)
    {
      if (FileOpenTerrainDialog.ShowDialog(this) != DialogResult.OK)
        return;

      m_terrainDataLayer.removeData();

      string terrainFile = FileOpenTerrainDialog.FileName;

      if (terrainFile == "")
      {
        return;
      }

      TSLNErrorStack.clear();

      if (!m_terrainDataLayer.loadData(terrainFile))
      {
        string error = TSLNErrorStack.errorString("Errors:\n", TSLNErrorCategory.TSLNErrorCategoryError | TSLNErrorCategory.TSLNErrorCategoryFatal);
        if (error != null && error != "")
        {
          MessageBox.Show(this, error, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }
      }
      m_terrainDataLayer.notifyChanged();

    }

    private void PrimitiveCreation_Polygon_Click(object sender, EventArgs e)
    {
      TSLN3DEntitySet layerSet = m_stdDataLayer.entitySet;
      TSLN3DCoordSet cs = new TSLN3DCoordSet();

      // Specify the polygon's position in latitude and longitude.
      // The altitude type defaults to TSL3DAltitudeTypeMeanSeaLevel.
      // Any other altitude type needs to be specified for every coordinate to which it should apply.
      cs.add(new TSLN3DCoord(50.0, 50.0, 100000.0));
      cs.add(new TSLN3DCoord(60.5, 50.0, 100000.0));
      cs.add(new TSLN3DCoord(60.5, 60.5, 100000.0));
      cs.add(new TSLN3DCoord(50.5, 60.5, 100000.0));

      TSLN3DPolygon polygon = layerSet.create3DPolygon(0, cs);

      polygon.setRendering(TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeColour, Color.Red.ToArgb());
      polygon.setRendering(TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeStyle, 1);
      polygon.setRendering(TSLNRenderingAttributeDouble.TSLNRenderingAttributeEdgeThickness, 2.0);
      // Note that the edge thickness can be set in map units or in pixels.
      // Setting the edge thickness to be displayed in map units rather than pixels means that the
      // edge thickness changes as the user zooms in and out.  However, there is a performance cost associated
      // with this and where possible, to ensure best performance, TSLDimensionUnitsPixels (the default)
      // should be used.
      // polygon->setRendering(TSLRenderingAttributeEdgeThicknessUnits, TSLDimensionUnitsMapUnits);
      polygon.setRendering(TSLNRenderingAttributeInt.TSLNRenderingAttributeFillStyle, 1);
      polygon.setRendering(TSLNRenderingAttributeInt.TSLNRenderingAttributeFillColour, Color.FromArgb(0, 255,0).ToArgb());

      // create an inner
      cs.clear();
      cs.add(new TSLN3DCoord(55.0, 55.0, 100000.0));
      cs.add(new TSLN3DCoord(57.0, 55.0, 100000.0));
      cs.add(new TSLN3DCoord(57.0, 57.0, 100000.0));
      cs.add(new TSLN3DCoord(55.0, 57.0, 100000.0));
      polygon.addInner(cs);

      m_stdDataLayer.notifyChanged();
      ViewerPanel.Invalidate();
    }

    private void PrimitiveCreation_ExtrudedPolygon_Click(object sender, EventArgs e)
    {
      TSLN3DEntitySet layerSet = m_stdDataLayer.entitySet;
      
      TSLNCoordSet cs2d = new TSLNCoordSet();

      // When creating an extruded primitive specify the vertices in latitude and longitude
      // but use the scaled values to create the 2D coordinate set.
      TSLN3DCoord p1 = new TSLN3DCoord(12.0, 12.0, 100000.0);
      TSLN3DCoord p2 = new TSLN3DCoord(12.0, 20.0, 100000.0);
      TSLN3DCoord p3 = new TSLN3DCoord(20.0, 20.0, 100000.0);
      TSLN3DCoord p4 = new TSLN3DCoord(20.0, 12.0, 100000.0);

      cs2d.add(p1.scaledLongitude, p1.scaledLatitude);
      cs2d.add(p2.scaledLongitude, p2.scaledLatitude);
      cs2d.add(p3.scaledLongitude, p3.scaledLatitude);
      cs2d.add(p4.scaledLongitude, p4.scaledLatitude);

      TSLNPolygon polygon2d = new TSLNPolygon(0, cs2d);

      cs2d.clear();

      TSLN3DCoord p5 = new TSLN3DCoord(15.0, 15.0, 100000.0);
      TSLN3DCoord p6 = new TSLN3DCoord(15.0, 17.0, 100000.0);
      TSLN3DCoord p7 = new TSLN3DCoord(17.0, 17.0, 100000.0);
      TSLN3DCoord p8 = new TSLN3DCoord(17.0, 15.0, 100000.0);

      cs2d.add(p5.scaledLongitude, p5.scaledLatitude);
      cs2d.add(p6.scaledLongitude, p6.scaledLatitude);
      cs2d.add(p7.scaledLongitude, p7.scaledLatitude);
      cs2d.add(p8.scaledLongitude, p8.scaledLatitude);

      polygon2d.addInner(cs2d);

      TSLNExtruded2DPolygon extPolygon = layerSet.createExtruded2DPolygon(0, polygon2d);
      extPolygon.altitude(30000.0);
      extPolygon.extrudedHeight = 10000.0;

      extPolygon.setRendering(TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeColour, Color.Red.ToArgb());
      extPolygon.setRendering(TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeStyle, 1);
      extPolygon.setRendering(TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeThicknessUnits, (int)TSLNDimensionUnits.TSLNDimensionUnitsPixels);
      extPolygon.setRendering(TSLNRenderingAttributeDouble.TSLNRenderingAttributeEdgeThickness, 2.0);
      extPolygon.setRendering(TSLNRenderingAttributeInt.TSLNRenderingAttributeFillStyle, 1);
      extPolygon.setRendering(TSLNRenderingAttributeInt.TSLNRenderingAttributeFillColour, Color.FromArgb(0,153,102).ToArgb());

      m_stdDataLayer.notifyChanged();
      ViewerPanel.Invalidate();
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

  }
}
