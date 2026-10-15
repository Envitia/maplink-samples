
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;

using Envitia.MapLink;
using Envitia.MapLink.Terrain;

namespace CSharpTerrainSample
{
  public partial class TerrainViewer : Form
  {
    private MRUSection m_mruSectionMap = null;
    private MRUSection m_mruSectionTerrain = null;
    private TerrainOptionsSection m_options = null;
    private TSLNTerrainDatabase m_terrainDB = null;
    private TerrainInfo m_terrainInfo = null;
    private string m_mapFile;
    private string m_terrainDBFile;
    
    public TerrainViewer(MRUSection mruSectionMap, MRUSection mruSectionTerrain, TerrainOptionsSection opts, string mapFile, string terrainDB )
    {
      m_mruSectionMap = mruSectionMap;
      m_mruSectionTerrain = mruSectionTerrain;
      m_options = opts;
      m_terrainInfo = new TerrainInfo(this);
      m_mapFile = mapFile;
      m_terrainDBFile = terrainDB;
      TSLNDrawingSurface.loadStandardConfig();
      InitializeComponent();
      this.tvMapPanel.configure(this, m_terrainInfo, projectionToolStripLabel, m_options);
      this.tvCrossSectionPanel.configure(this, m_options);
      updateRecentMaps();
      updateRecentTerrains();   
    }

    private void OnLoad(object sender, EventArgs e)
    {
      tvMapPanel.OnLoad();
      m_terrainInfo.Visible = false;

      if (m_mapFile.Length > 0)
      {
        tvMapPanel.loadMap(m_mapFile, this);
      }
      if( m_terrainDBFile.Length > 0)
      {
        loadTerrain(m_terrainDBFile);
      }
    }

    private void openToolStripMenuItem_Click(object sender, EventArgs e)
    {
      FileOpenDialog.ShowDialog();
      string mapFile = FileOpenDialog.FileName;

      if (mapFile == "")
      {
        return;
      }

      int indexOfLastSlash = mapFile.LastIndexOf('\\') + 1;
      string shortfilename = mapFile.Substring(indexOfLastSlash, mapFile.Length - indexOfLastSlash);

      if (tvMapPanel.loadMap(mapFile, this))
      {
        this.Text = shortfilename + " - C# Terrain .NET Sample";
        m_mruSectionMap.addPath(mapFile);
        copyToolStripMenuItem.Enabled = true;
        printPreviewToolStripMenuItem.Enabled = true;
        printToolStripButton.Enabled = true;
        printToolStripMenuItem.Enabled = true;
        updateRecentMaps();
      }
      UpdateMenuToolbarButtons();
    }

    public void UpdateMenuToolbarButtons()
    {
      bool mapLoaded = tvMapPanel.isMapLoaded();
      bool terrainLoaded = mapLoaded && m_terrainDB != null ? m_terrainDB.isOpen : false;
      int toolID = tvMapPanel.activeMode();

      printToolStripButton.Enabled = mapLoaded;
      copyToolStripButton.Enabled = mapLoaded;
      zoomInToolStripButton.Enabled = mapLoaded;
      zoomInToolStripMenuItem.Enabled = mapLoaded;
      zoomOutToolStripButton.Enabled = mapLoaded;
      zoomOutToolStripMenuItem.Enabled = mapLoaded;
      resetToolStripButton.Enabled = mapLoaded;
      resetToolStripMenuItem.Enabled = mapLoaded;
      zoomToolStripMenuItem.Enabled = mapLoaded;
      zoomToolStripMenuItem.Checked = toolID == (int)zoomToolStripButton.Tag;
      zoomToolStripButton.Enabled = mapLoaded;
      zoomToolStripButton.Checked = toolID == (int)zoomToolStripButton.Tag;
      panToolStripMenuItem.Enabled = mapLoaded;
      panToolStripMenuItem.Checked = toolID == (int)panToolStripButton.Tag;
      panToolStripButton.Enabled = mapLoaded;
      panToolStripButton.Checked = toolID == (int)panToolStripButton.Tag;
      zoomPanToolStripMenuItem.Enabled = mapLoaded;
      zoomPanToolStripMenuItem.Checked = toolID == (int)zoomPanToolStripButton.Tag;
      zoomPanToolStripButton.Enabled = mapLoaded;
      zoomPanToolStripButton.Checked = toolID == (int)zoomPanToolStripButton.Tag;

      terrainInfoToolStripMenuItem.Enabled = terrainLoaded;
      terrainInfoToolStripMenuItem.Checked = m_terrainInfo.Visible;
      terrainInfoToolStripButton.Enabled = terrainLoaded;
      terrainInfoToolStripButton.Checked = m_terrainInfo.Visible;
      crossSectionToolStripMenuItem.Enabled = terrainLoaded;
      crossSectionToolStripMenuItem.Checked = toolID == (int)crossSectionToolStripButton.Tag;
      crossSectionToolStripButton.Enabled = terrainLoaded;
      crossSectionToolStripButton.Checked = toolID == (int)crossSectionToolStripButton.Tag;
      lineOfSightToolStripMenuItem.Enabled = terrainLoaded;
      lineOfSightToolStripMenuItem.Checked = toolID == (int)losToolStripButton.Tag;
      losToolStripButton.Enabled = terrainLoaded;
      losToolStripButton.Checked = toolID == (int)losToolStripButton.Tag;
      viewShedToolStripMenuItem.Enabled = terrainLoaded;
      viewShedToolStripMenuItem.Checked = toolID == (int)viewShedToolStripButton.Tag;
      viewShedToolStripButton.Enabled = terrainLoaded;
      viewShedToolStripButton.Checked = toolID == (int)viewShedToolStripButton.Tag;
    }

    private void OnMouseWheel(object sender, System.Windows.Forms.MouseEventArgs e)
    {
      if (tvMapPanel.DisplayRectangle.Contains(e.X, e.Y))
      {
        tvMapPanel.OnMouseWheel(sender, e);
      }
    }

    private void tvToolStrip_ButtonClick(object sender, System.EventArgs e)
    {
      if (sender == openToolStripButton ||
          sender == openToolStripMenuItem)
      {
        openToolStripMenuItem_Click(sender, e);
      }
      else if (sender == printToolStripButton ||
               sender == printToolStripMenuItem)
      {
      //  PrintMap();
      }
      else if (sender == copyToolStripButton ||
               sender == copyToolStripMenuItem)
      {
      //  EditCopy();
      }
      else if (sender == zoomInToolStripButton ||
               sender == zoomInToolStripMenuItem)
      {
        tvMapPanel.zoom(true);
      }
      else if (sender == zoomOutToolStripButton ||
               sender == zoomOutToolStripMenuItem)
      {
        tvMapPanel.zoom(false);
      }
      else if (sender == resetToolStripButton ||
          sender == resetToolStripMenuItem)
      {
        tvMapPanel.resetToFullExtent();
      }
      else if (sender == terrainInfoToolStripButton ||
          sender == terrainInfoToolStripMenuItem)
      {
        if (m_terrainInfo.Visible)
        {
          m_terrainInfo.Hide();
        }
        else
        {
          m_terrainInfo.Show(this);
        }
        UpdateMenuToolbarButtons();
      }
      else
      {
        Type buttonType = sender.GetType();
        if ((buttonType == typeof(ToolStripButton) || buttonType == typeof(ToolStripMenuItem)) &&
            ((ToolStripItem)sender).Tag != null)
        {
          tvMapPanel.setImodeByID((int)((ToolStripItem)sender).Tag);
          UpdateMenuToolbarButtons();
        }
      }
    }

    private void updateRecentMaps()
    {
      int numRecentMaps = m_mruSectionMap.Count;
      recentMapsToolStripMenuItem.Enabled = numRecentMaps > 0;
      recentMap1ToolStripMenuItem.Visible = numRecentMaps > 0;
      recentMap2ToolStripMenuItem.Visible = numRecentMaps > 1;
      recentMap3ToolStripMenuItem.Visible = numRecentMaps > 2;
      recentMap4ToolStripMenuItem.Visible = numRecentMaps > 3;
      recentMap1ToolStripMenuItem.Text = numRecentMaps > 0 ? m_mruSectionMap[0] : "";
      recentMap2ToolStripMenuItem.Text = numRecentMaps > 1 ? m_mruSectionMap[1] : "";
      recentMap3ToolStripMenuItem.Text = numRecentMaps > 2 ? m_mruSectionMap[2] : "";
      recentMap4ToolStripMenuItem.Text = numRecentMaps > 3 ? m_mruSectionMap[3] : "";
    }

    private void updateRecentTerrains()
    {
      int numRecentTerrains = m_mruSectionTerrain.Count;
      recentTerrainDBsToolStripMenuItem.Enabled = numRecentTerrains > 0;
      recentTerrain1ToolStripMenuItem.Visible = numRecentTerrains > 0;
      recentTerrain2ToolStripMenuItem.Visible = numRecentTerrains > 1;
      recentTerrain3ToolStripMenuItem.Visible = numRecentTerrains > 2;
      recentTerrain4ToolStripMenuItem.Visible = numRecentTerrains > 3;
      recentTerrain1ToolStripMenuItem.Text = numRecentTerrains > 0 ? m_mruSectionTerrain[0] : "";
      recentTerrain2ToolStripMenuItem.Text = numRecentTerrains > 1 ? m_mruSectionTerrain[1] : "";
      recentTerrain3ToolStripMenuItem.Text = numRecentTerrains > 2 ? m_mruSectionTerrain[2] : "";
      recentTerrain4ToolStripMenuItem.Text = numRecentTerrains > 3 ? m_mruSectionTerrain[3] : "";
    }

    private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
    {
      AboutBox ab = new AboutBox();
      ab.ShowDialog(this);
    }

    private void optionsToolStripMenuItem_Click(object sender, EventArgs e)
    {
      TerrainOptions tod = new TerrainOptions(m_options);
      if (tod.ShowDialog(this) == DialogResult.OK)
      {
        this.tvCrossSectionPanel.configure(this, m_options);
      }
    }

    private void recentMapToolStripMenuItem_Click(object sender, EventArgs e)
    {
      if (sender == null || ((ToolStripMenuItem)sender) == null)
        return;

      int index = (int)((ToolStripMenuItem)sender).Tag;
      --index;

      string mapFile = m_mruSectionMap[index];
      if (!System.IO.File.Exists(mapFile))
      {
        MessageBox.Show(this, "The file \"" + mapFile + "\" no longer exists", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
        m_mruSectionMap.removePath(index);
        return;
      }

      int indexOfLastSlash = mapFile.LastIndexOf('\\') + 1;
      string shortfilename = mapFile.Substring(indexOfLastSlash, mapFile.Length - indexOfLastSlash);

      if (tvMapPanel.loadMap(mapFile, this))
      {
        this.Text = shortfilename + " - MapLink Terrain Viewer .NET";
        copyToolStripMenuItem.Enabled = true;
        m_mruSectionMap.addPath(mapFile);
        printPreviewToolStripMenuItem.Enabled = true;
        printToolStripButton.Enabled = true;
        printToolStripMenuItem.Enabled = true;
        updateRecentMaps();
      }

      UpdateMenuToolbarButtons();
    }

    private void recentTerrainToolStripMenuItem_Click(object sender, EventArgs e)
    {
      if (sender == null || ((ToolStripMenuItem)sender) == null)
        return;

      int index = (int)((ToolStripMenuItem)sender).Tag;
      --index;

      string terrainFile = m_mruSectionTerrain[index];
      if (!System.IO.File.Exists(terrainFile))
      {
        MessageBox.Show(this, "The file \"" + terrainFile + "\" no longer exists", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
        m_mruSectionMap.removePath(index);
        return;
      }
      if (m_terrainDB == null)
      {
        m_terrainDB = new TSLNTerrainDatabase();
      }

      if (m_terrainDB.open(terrainFile, null) == TSLNTerrainReturn.TSLNTerrain_OK)
      {
        m_mruSectionTerrain.addPath(terrainFile);
        tvMapPanel.notifyTerrainChange();
        updateRecentTerrains();
      }
      UpdateMenuToolbarButtons();
    }

    private void loadTerrainDBToolStripMenuItem_Click(object sender, EventArgs e)
    {
      FileOpenTerrainDialog.ShowDialog();
      m_terrainDBFile = FileOpenTerrainDialog.FileName;

      loadTerrain(m_terrainDBFile);
    }
    private void loadTerrain(string terrainFile)
    {
        if (terrainFile == "")
        {
            return;
        }
        if (m_terrainDB == null)
        {
            m_terrainDB = new TSLNTerrainDatabase();
        }

        if (m_terrainDB.open(terrainFile, null) != TSLNTerrainReturn.TSLNTerrain_OK)
        {
            string error = TSLNErrorStack.errorString("Errors:\n", TSLNErrorCategory.TSLNErrorCategoryError | TSLNErrorCategory.TSLNErrorCategoryFatal);
            if (error != "")
            {
                MessageBox.Show(this, "MapLink has reported an error during loading of the selected terrain database\n" + error, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
        }
        else
        {
            m_mruSectionTerrain.addPath(terrainFile);
            tvMapPanel.notifyTerrainChange();
            updateRecentTerrains();
            UpdateMenuToolbarButtons();

        }
    }
    public TSLNTerrainDatabase terrainDatabase
    {
      get
      {
        return m_terrainDB;
      }
    }
    public CrossSectionPanel terrainPanel
    {
      get
      {
        return tvCrossSectionPanel;
      }
    }

    private void exitToolStripMenuItem_Click(object sender, EventArgs e)
    {
      this.Close();
    }

    [DllImport("gdi32.dll")]
    private static extern bool
    BitBlt(IntPtr hdcDest, int nXDest, int nYDest, int nWidth, int nHeight, IntPtr hdcSrc, int nXSrc, int nYSrc, int dwRop);

    private void copyToolStripMenuItem_Click(object sender, EventArgs e)
    {
      Graphics g = tvMapPanel.CreateGraphics();
      Bitmap image = new Bitmap(tvMapPanel.Width, tvMapPanel.Height);
      Graphics dest = Graphics.FromImage(image);
      IntPtr hdc = g.GetHdc();
      IntPtr hdcDest = dest.GetHdc();
      BitBlt(hdcDest, 0, 0, tvMapPanel.Width, tvMapPanel.Height, hdc, 0, 0, 0x00CC0020);
      g.ReleaseHdc(hdc);
      dest.ReleaseHdc(hdcDest);
      Clipboard.SetDataObject(image, true);	
    }

    //called when File->Print is clicked from the menu
    private void FilePrintMenu_Click(object sender, System.EventArgs e)
    {
      this.tvMapPanel.PrintMap(false);
    }

    void printPreviewToolStripMenuItem_Click(object sender, System.EventArgs e)
    {
      this.tvMapPanel.PrintMap(true);
    }
  }
}