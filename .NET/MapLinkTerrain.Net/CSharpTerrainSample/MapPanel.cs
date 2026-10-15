using System;
using System.ComponentModel;
using System.Collections.Generic;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Windows.Forms.Design;
using Envitia.MapLink;
using Envitia.MapLink.InteractionModes;
using Envitia.MapLink.Terrain;


namespace CSharpTerrainSample
{
  [System.ComponentModel.DesignerCategory("Code")]
  [Designer(typeof(MapViewerPanelDesigner))]
  public class MapPanel : Panel
  {
    private static string m_mapDataLayerName = "map";
    private static string m_stdDataLayerName = "std";
    private static string m_extentDataLayerName = "extent";
    private static string m_customDataLayerName = "custom";

    private TSLNDrawingSurface m_drawingSurface = null;
    private TSLNMapDataLayer m_mapDataLayer = null;
    private TSLNStandardDataLayer m_stdDataLayer = null;
    private TSLNStandardDataLayer m_extentDataLayer = null;
    private TSLNCustomDataLayer m_custDataLayer = null;

    private TerrainCustomClientDataLayer m_customClient = null;

    private TSLNInteractionModeManager m_imodeManager = null;

    private TSLNInteractionModeZoom m_zoomMode = null;
    private TSLNInteractionModePan m_panMode = null;
    private ZoomPanInteractionMode m_zoomPanMode = null;

    private TerrainCrossSectionInteractionMode m_terrainCS = null;
    private TerrainViewShedInteractionMode m_terrainVS = null;
    private TerrainLosInteractionMode m_terrainLOS = null;

    //Interaction Mode IDS
    public static int ID_TOOLS_ZOOM = 1;
    public static int ID_TOOLS_PAN = 2;
    public static int ID_TOOLS_ZOOMPAN = 3;
    public static int ID_TOOLS_CROSS_SECTION = 4;
    public static int ID_TOOLS_LOS = 5;
    public static int ID_TOOLS_VIEW_SHED = 6;

    private bool m_mapLoaded = false;
    private bool m_controlButtonDown = false;
    private bool m_shiftButtonDown = false;
    private bool m_conversionRequired = false;

    private TerrainViewer m_parent = null;
    private TerrainInfo m_infoPanel = null;
    private ToolStripStatusLabel m_statusLabel = null;
    private TerrainOptionsSection m_options = null;

    public MapPanel()
    {
      m_parent = null;
      InitializeComponent();
    }

    public void configure(TerrainViewer parent, TerrainInfo infoPanel, ToolStripStatusLabel statusLabel, TerrainOptionsSection options)
    {
      m_parent = parent;
      m_infoPanel = infoPanel;
      m_statusLabel = statusLabel;
      m_options = options;
    }

    private void InitializeComponent()
    {
      this.SuspendLayout();
      // 
      // MapPanel
      // 
      this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.OnMouseDown);
      this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.OnMouseMove);
      this.MouseUp += new System.Windows.Forms.MouseEventHandler(this.OnMouseUp);
      this.Resize += new System.EventHandler(this.OnResize);
      this.Paint += new System.Windows.Forms.PaintEventHandler(this.OnPaint);
      
      this.ResumeLayout(false);

    }

    public void OnLoad()
    {
      m_drawingSurface = new TSLNDrawingSurface(this.Handle, false);
      m_mapDataLayer = new TSLNMapDataLayer();
      m_stdDataLayer = new TSLNStandardDataLayer();
      m_extentDataLayer = new TSLNStandardDataLayer();
      m_imodeManager = new TSLNInteractionModeManager(null, m_drawingSurface, this.Handle);

      m_zoomMode = new TSLNInteractionModeZoom(ID_TOOLS_ZOOM, 30, "Left button drag zoom to rectangle, Left button click anchored zoom-in, Shift-Left button click anchored zoom-out, Right button click to finish", true);
      m_panMode = new TSLNInteractionModePan(ID_TOOLS_PAN, "Left button click pans to point, Right button click to finish", true);
      m_zoomPanMode = new ZoomPanInteractionMode(ID_TOOLS_ZOOMPAN, 30, "Left button click pans to point and zooms in, Right button click pans to point and zooms out", true);

      m_imodeManager.addMode(m_zoomMode, true); //set zoom as default
      m_imodeManager.addMode(m_panMode, false);
      m_imodeManager.addMode(m_zoomPanMode, false);
      m_imodeManager.setCurrentMode(ID_TOOLS_ZOOM);

      m_drawingSurface.wndResize(this.DisplayRectangle.Left, this.DisplayRectangle.Top, this.DisplayRectangle.Right, this.DisplayRectangle.Bottom, false, TSLNResizeActionEnum.TSLNResizeActionMaintainCentre);
      m_drawingSurface.setOption(TSLNOptionEnum.TSLNOptionDoubleBuffered, true);

      m_drawingSurface.addDataLayer(m_mapDataLayer,    m_mapDataLayerName);
      m_drawingSurface.addDataLayer(m_stdDataLayer,    m_stdDataLayerName);
      m_drawingSurface.addDataLayer(m_extentDataLayer, m_extentDataLayerName);

      m_drawingSurface.setDataLayerProps(m_mapDataLayerName, TSLNPropertyEnum.TSLNPropertyBuffered, 1);
      m_drawingSurface.reset(false);
    }


    protected override void OnPaintBackground(PaintEventArgs pevent)
    {
      // do nothing...
      // we don't want the background to flash over the map
    }

    private void OnPaint(object sender, PaintEventArgs e)
    {
      if (m_drawingSurface != null)
      {
        m_drawingSurface.drawDU(e.ClipRectangle.Left, e.ClipRectangle.Top, e.ClipRectangle.Right, e.ClipRectangle.Bottom, false, false);
      }
      if (m_imodeManager != null)
      {
        m_imodeManager.onDraw(e.ClipRectangle.Left, e.ClipRectangle.Top, e.ClipRectangle.Right, e.ClipRectangle.Bottom);
      }
    }

    private void OnResize(object sender, EventArgs e)
    {
      if (m_drawingSurface != null)
      {
        m_drawingSurface.wndResize(DisplayRectangle.Left, DisplayRectangle.Top, DisplayRectangle.Right, DisplayRectangle.Bottom, false, TSLNResizeActionEnum.TSLNResizeActionMaintainCentre);
      }

      if (m_imodeManager != null)
      {
        m_imodeManager.onSize(DisplayRectangle.Width, DisplayRectangle.Height);
      }

      Invalidate();
    }

    public bool loadMap(string filename, Form parent)
    {
      bool retval = false;
      if (m_drawingSurface != null && m_mapDataLayer != null)
      {
        if (!m_mapDataLayer.loadData(filename))
        {
          string error = TSLNErrorStack.errorString("Errors:", TSLNErrorCategory.TSLNErrorCategoryError | TSLNErrorCategory.TSLNErrorCategoryFatal);
          if (error != "")
          {
            MessageBox.Show(parent, "MapLink has reported an error during loading of the selected map\n" + error, "Error", MessageBoxButtons.OK);
            m_mapLoaded = false;
          }
        }
        else
        {
          m_mapLoaded = true;
          retval = true;
          m_conversionRequired = false;
          TSLNCoordinateSystem mapcs = m_mapDataLayer.coordinateSystem;
          TSLNTerrainDatabase db = m_parent.terrainDatabase;
          TSLNCoordinateSystem terraincs = db != null ? db.coordinateSystem : null;

          if (mapcs != null && terraincs != null)
          {
            m_conversionRequired = mapcs.id != terraincs.id;
          }

          m_drawingSurface.backgroundColour = m_mapDataLayer.backgroundColour;
        }
        m_drawingSurface.reset(false);
        this.Invalidate();
      }
      return retval;
    }

    public bool isMapLoaded()
    {
      return m_mapLoaded;
    }

    public int activeMode()
    {
      if (m_imodeManager == null)
        return 0;
      TSLNInteractionMode mode = null;
      return m_imodeManager.getCurrentMode(out mode);
    }

    private void OnMouseDown(object sender, MouseEventArgs e)
    {
      if (m_imodeManager == null)
      {
        return;
      }

      bool invalidate = false;

      switch (e.Button)
      {
        case MouseButtons.Left:
          {
            invalidate = m_imodeManager.onLButtonDown(e.X, e.Y, m_shiftButtonDown, m_controlButtonDown);
            break;
          }
        case MouseButtons.Middle:
          {
            invalidate = m_imodeManager.onMButtonDown(e.X, e.Y, m_shiftButtonDown, m_controlButtonDown);
            break;
          }
        case MouseButtons.Right:
          {
            invalidate = m_imodeManager.onRButtonDown(e.X, e.Y, m_shiftButtonDown, m_controlButtonDown);
            break;
          }
      }
      if (invalidate)
      {
        Invalidate();
      }
    }

    private void OnMouseUp(object sender, MouseEventArgs e)
    {
      if (m_imodeManager == null)
      {
        return;
      }

      bool invalidate = false;
      switch (e.Button)
      {
        case MouseButtons.Left:
          {
            invalidate = m_imodeManager.onLButtonUp(e.X, e.Y, m_shiftButtonDown, m_controlButtonDown);
            break;
          }
        case MouseButtons.Middle:
          {
            invalidate = m_imodeManager.onMButtonUp(e.X, e.Y, m_shiftButtonDown, m_controlButtonDown);
            break;
          }
        case MouseButtons.Right:
          {
            invalidate = m_imodeManager.onRButtonUp(e.X, e.Y, m_shiftButtonDown, m_controlButtonDown);
            break;
          }
      }
      m_parent.UpdateMenuToolbarButtons();
      if (invalidate)
      {
        Invalidate();
      }
    }

    private void OnMouseMove(object sender, MouseEventArgs e)
    {
      if (m_imodeManager == null)
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

      if (m_imodeManager.onMouseMove(button, e.X, e.Y, m_shiftButtonDown, m_controlButtonDown))
      {
        // Request a redraw if the interaction hander requires it
        Invalidate();
      }

      TSLNTerrainDatabase db = m_parent.terrainDatabase;

      if ( db != null && !db.isOpen )
      {
        double x1, y1, x2, y2;

        if ( !m_conversionRequired) 
        {
          m_drawingSurface.DUToMU( 0, 0, out x1, out y1 );
          m_drawingSurface.DUToMU(  DisplayRectangle.Width - 1, DisplayRectangle.Height - 1, out x2, out y2 );
        }
        else
        {
          double lat1, lat2, lng1, lng2;
          m_drawingSurface.DUToMU(0, 0, out lat1, out lng1);
          db.latLongToMU(lat1, lng1, out x1, out y1, false);

          m_drawingSurface.DUToMU(DisplayRectangle.Width - 1, DisplayRectangle.Height - 1, out lat2, out lng2);
          db.latLongToMU(lat2, lng2, out x2, out y2, false);
        }
        
        db.displayExtent( DisplayRectangle.Width, DisplayRectangle.Height, x1, y1, x2, y2 );
      }

      if (m_drawingSurface != null && db != null && db.isOpen )
      {
        double height;
        double x, y ;

        if (!m_conversionRequired)
        {
          m_drawingSurface.DUToMU(e.X, e.Y, out x, out y);
        }
        else
        {
          double lat, lng;
          m_drawingSurface.DUToLatLong(e.X, e.Y, out lat, out lng, false, true);
          db.latLongToMU(lat, lng, out x, out y, false);
        }
        
        //Terrain Info...
        ValueType divt;
        if (db.query(x, y, out divt, TSLNTerrainData_LEVEL.TSLNTerrainData_All, false, TSLNTerrainInterpolate.TSLNTerrainInterpolate_LINEAR) == TSLNTerrainReturn.TSLNTerrain_OK)
        {
          double lat, lon;
          db.MUToLatLong( x, y, out lat, out lon, false, true );
          TSLNTerrainDataItem di = (TSLNTerrainDataItem)divt;
          height = di.m_z;
          if ( di.m_nearestIsNull )
          {
            m_infoPanel.update(lat, lon, x, y, height, 0.0, 0.0, 0.0, di.m_xResolution, di.m_yResolution);
          }
          else
          {
            m_infoPanel.update(lat, lon, x, y, height, di.m_nearestX, di.m_nearestY, di.m_nearestZ, di.m_xResolution, di.m_yResolution);
          }
          m_statusLabel.Text = String.Format("Latitude: {0,3:F}   Longitude: {1,3:F}   Height: {2,3:F}", lat, lon, height);
        }
        //updateExtentRect();
        
      }
      else
      {
        m_statusLabel.Text = "No Data";
      }
    }

    public void OnMouseWheel(object sender, System.Windows.Forms.MouseEventArgs e)
    {
      if (m_imodeManager == null)
      {
        return;
      }
      if (m_imodeManager.onMouseWheel((short)e.Delta, e.X, e.Y))
      {
        Invalidate();
      }
    }

    public void zoom(bool zoomIn)
    {
      if (m_imodeManager == null)
      {
        return;
      }

      bool invalidate;
      if (zoomIn)
      {
        invalidate = m_imodeManager.zoomIn(30, false);
      }
      else
      {
        invalidate = m_imodeManager.zoomOut(30, false);
      }

      if (invalidate)
      {
        Invalidate();
      }
    }

    public void resetToFullExtent()
    {
      if (m_drawingSurface.reset(false))
      {
        Invalidate();
      }
    }

    public void setImodeByID(int id)
    {
      if (m_imodeManager.setCurrentMode(id))
      {
        m_drawingSurface.redraw();
        //Invalidate();
      }
    }

    public void notifyTerrainChange()
    {
      TSLNTerrainDatabase db = m_parent.terrainDatabase;
      CrossSectionPanel csp = m_parent.terrainPanel;
      if (m_custDataLayer == null)
      {
        m_custDataLayer = new TSLNCustomDataLayer();
        m_customClient = new TerrainCustomClientDataLayer(db, m_drawingSurface, m_mapDataLayer, this, m_options);
        m_custDataLayer.setClientCustomDataLayer(m_customClient);
        m_drawingSurface.addDataLayer(m_custDataLayer, m_customDataLayerName);

        m_terrainVS = new TerrainViewShedInteractionMode(ID_TOOLS_VIEW_SHED, true, "", m_customClient, m_options, this);
        m_terrainLOS = new TerrainLosInteractionMode(ID_TOOLS_LOS, true, "", csp, this);
        m_terrainCS = new TerrainCrossSectionInteractionMode(ID_TOOLS_CROSS_SECTION, true, "", csp, this);

        m_imodeManager.addMode(m_terrainVS, false);
        m_imodeManager.addMode(m_terrainLOS, false);
        m_imodeManager.addMode(m_terrainCS, false);

        m_conversionRequired = false;
        if ( m_mapLoaded )
        {
          TSLNCoordinateSystem mapcs = m_mapDataLayer.coordinateSystem;
          TSLNCoordinateSystem terraincs = db != null ? db.coordinateSystem : null;

          if ( mapcs != null && terraincs != null )
          {
            m_conversionRequired = mapcs.id != terraincs.id;
          }
        }
      }

      double dmed_minlat, dmed_minlon, dmed_maxlat, dmed_maxlon;

      if (m_extentDataLayer != null && db.queryExtent(out dmed_minlon, out dmed_minlat, out dmed_maxlon, out dmed_maxlat) == TSLNTerrainReturn.TSLNTerrain_OK)
      {
        // The query will have returned units in mu
        double muX1 = dmed_minlon;
        double muY1 = dmed_minlat;
        double muX2 = dmed_maxlon;
        double muY2 = dmed_maxlat;

        db.MUToLatLong(muX1, muY1, out dmed_minlat, out dmed_minlon, false, true);
        db.MUToLatLong(muX2, muY2, out dmed_maxlat, out dmed_maxlon, false, true);

        if (dmed_minlon < -180.0)
          dmed_minlon = -180.0;
        if (dmed_maxlon > 180.0)
          dmed_maxlon = 180.0;
        if (dmed_minlat < -90.0)
          dmed_minlat = -90.0;
        if (dmed_maxlat > 90.0)
          dmed_maxlat = 90.0;

        TSLNEntitySet entitySet = m_extentDataLayer.entitySet;
        TSLNCoordSet coords = new TSLNCoordSet();

        int xpos, ypos;

        m_drawingSurface.latLongToTMC(dmed_minlat, dmed_minlon, out xpos, out ypos, false);
        coords.add(xpos, ypos, 0);

        m_drawingSurface.latLongToTMC(dmed_maxlat, dmed_minlon, out xpos, out ypos, false);
        coords.add(xpos, ypos, 0);

        m_drawingSurface.latLongToTMC(dmed_maxlat, dmed_maxlon, out xpos, out ypos, false);
        coords.add(xpos, ypos, 0);

        m_drawingSurface.latLongToTMC(dmed_minlat, dmed_maxlon, out xpos, out ypos, false);
        coords.add(xpos, ypos, 0);

        m_drawingSurface.latLongToTMC(dmed_minlat, dmed_minlon, out xpos, out ypos, false);
        coords.add(xpos, ypos, 0);

        TSLNPolyline ply = entitySet.createPolyline(100, coords);

        // Attach specific colour/style attributes to it.
        ply.setRendering(TSLNRenderingAttributeDouble.TSLNRenderingAttributeEdgeThickness, 1.0);
        ply.setRendering(TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeColour, 181);
        ply.setRendering(TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeStyle, 1);

        m_extentDataLayer.notifyChanged(true);

      }
      m_drawingSurface.reset(false);
      Invalidate();
    }

    public bool isTerrainViewShedActive
    {
      get
      {
        return activeMode() == ID_TOOLS_VIEW_SHED;
      }
    }

    public TSLNStandardDataLayer visDataLayer
    {
      get
      {
        return m_stdDataLayer;
      }
    }

    public bool isConversionRequired
    {
      get
      {
        return m_conversionRequired;
      }
    }

    public void PrintMap(bool preview)
    {
      TSLNPrintItem[] printItems = new TSLNPrintItem[1];
      TSLNPrintItem printItem = new TSLNPrintItem();
      TSLNEnvelope envelope = new TSLNEnvelope();
      printItems[0] = printItem;

      double uux1, uuy1, uux2, uuy2;
      int tmcx1, tmcy1, tmcx2, tmcy2;
      string templateFile = "";
      bool isLandscape;

      if (this.Width > this.Height)
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
        MessageBox.Show(this, "MapLink could not load printer template", "Error", MessageBoxButtons.OK);
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

      string[] textMappings = new string[4];
      textMappings[0] = "MapLink Pro";
      textMappings[1] = ""/*m_shortfilename*/;
      textMappings[2] = "date";
      System.DateTime dt = System.DateTime.Now;
      textMappings[3] = dt.ToShortDateString();

      try
      {
        TSLNPrintDocument pd = new TSLNPrintDocument(m_drawingSurface, entitySet, printItems, textMappings, isLandscape);
        if (preview)
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
  }

  /// <summary>
  /// This designer class forces the background of the MapViewerPanel class to be
  /// drawn when viewing instances of it through the designer.
  /// </summary>
  internal class MapViewerPanelDesigner : ControlDesigner
  {
    protected override void OnPaintAdornments(PaintEventArgs pevent)
    {
      pevent.Graphics.FillRectangle(new SolidBrush(this.Control.BackColor), pevent.ClipRectangle);
    }
  }
}
