namespace CSharpTerrainSample
{
  partial class TerrainViewer
  {
    /// <summary>
    /// Required designer variable.
    /// </summary>
    private System.ComponentModel.IContainer components = null;

    /// <summary>
    /// Clean up any resources being used.
    /// </summary>
    /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
    protected override void Dispose(bool disposing)
    {
      if (disposing && (components != null))
      {
        components.Dispose();
      }
      Envitia.MapLink.TSLNDrawingSurface.cleanup();
      base.Dispose(disposing);
    }

    #region Windows Form Designer generated code

    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    private void InitializeComponent()
    {
      System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
      System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(TerrainViewer));
      System.Windows.Forms.ToolStripSeparator toolStripSeparator8;
      System.Windows.Forms.ToolStripSeparator toolStripSeparator13;
      System.Windows.Forms.ToolStripSeparator toolStripSeparator6;
      System.Windows.Forms.ToolStripSeparator toolStripSeparator9;
      System.Windows.Forms.ToolStripSeparator toolStripSeparator10;
      System.Windows.Forms.ToolStripSeparator toolStripSeparator12;
      System.Windows.Forms.ToolStripSeparator toolStripSeparator;
      System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
      System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
      System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
      System.Windows.Forms.ToolStripSeparator toolStripSeparator5;
      System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
      this.tvMenuStrip = new System.Windows.Forms.MenuStrip();
      this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.loadTerrainDBToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.toolStripSeparator7 = new System.Windows.Forms.ToolStripSeparator();
      this.printToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.printPreviewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.recentMapsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.recentMap1ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.recentMap2ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.recentMap3ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.recentMap4ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.recentTerrainDBsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.recentTerrain1ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.recentTerrain2ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.recentTerrain3ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.recentTerrain4ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.editToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.copyToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.toolsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.zoomInToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.zoomOutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.resetToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.zoomToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.panToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.zoomPanToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.terrainInfoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.crossSectionToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.lineOfSightToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.viewShedToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.optionsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.tvToolStrip = new System.Windows.Forms.ToolStrip();
      this.openToolStripButton = new System.Windows.Forms.ToolStripButton();
      this.printToolStripButton = new System.Windows.Forms.ToolStripButton();
      this.copyToolStripButton = new System.Windows.Forms.ToolStripButton();
      this.zoomInToolStripButton = new System.Windows.Forms.ToolStripButton();
      this.zoomOutToolStripButton = new System.Windows.Forms.ToolStripButton();
      this.resetToolStripButton = new System.Windows.Forms.ToolStripButton();
      this.zoomToolStripButton = new System.Windows.Forms.ToolStripButton();
      this.panToolStripButton = new System.Windows.Forms.ToolStripButton();
      this.zoomPanToolStripButton = new System.Windows.Forms.ToolStripButton();
      this.terrainInfoToolStripButton = new System.Windows.Forms.ToolStripButton();
      this.crossSectionToolStripButton = new System.Windows.Forms.ToolStripButton();
      this.losToolStripButton = new System.Windows.Forms.ToolStripButton();
      this.viewShedToolStripButton = new System.Windows.Forms.ToolStripButton();
      this.aboutToolStripButton = new System.Windows.Forms.ToolStripButton();
      this.tvStatusStrip = new System.Windows.Forms.StatusStrip();
      this.statusToolStripLabel = new System.Windows.Forms.ToolStripStatusLabel();
      this.projectionToolStripLabel = new System.Windows.Forms.ToolStripStatusLabel();
      this.tvSplitContainer = new System.Windows.Forms.SplitContainer();
      this.tvMapPanel = new CSharpTerrainSample.MapPanel();
      this.tvCrossSectionPanel = new CSharpTerrainSample.CrossSectionPanel();
      this.FileOpenDialog = new System.Windows.Forms.OpenFileDialog();
      this.FileOpenTerrainDialog = new System.Windows.Forms.OpenFileDialog();
      fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      toolStripSeparator8 = new System.Windows.Forms.ToolStripSeparator();
      toolStripSeparator13 = new System.Windows.Forms.ToolStripSeparator();
      toolStripSeparator6 = new System.Windows.Forms.ToolStripSeparator();
      toolStripSeparator9 = new System.Windows.Forms.ToolStripSeparator();
      toolStripSeparator10 = new System.Windows.Forms.ToolStripSeparator();
      toolStripSeparator12 = new System.Windows.Forms.ToolStripSeparator();
      toolStripSeparator = new System.Windows.Forms.ToolStripSeparator();
      toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
      toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
      toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
      toolStripSeparator5 = new System.Windows.Forms.ToolStripSeparator();
      toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
      this.tvMenuStrip.SuspendLayout();
      this.tvToolStrip.SuspendLayout();
      this.tvStatusStrip.SuspendLayout();
      this.tvSplitContainer.SuspendLayout();
      this.SuspendLayout();
      // 
      // tvMenuStrip
      // 
      this.tvMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            fileToolStripMenuItem,
            this.editToolStripMenuItem,
            this.toolsToolStripMenuItem,
            this.helpToolStripMenuItem});
      this.tvMenuStrip.Location = new System.Drawing.Point(0, 0);
      this.tvMenuStrip.Name = "tvMenuStrip";
      this.tvMenuStrip.Size = new System.Drawing.Size(913, 24);
      this.tvMenuStrip.TabIndex = 1;
      this.tvMenuStrip.Text = "menuStrip1";
      // 
      // fileToolStripMenuItem
      // 
      fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openToolStripMenuItem,
            this.loadTerrainDBToolStripMenuItem,
            this.toolStripSeparator7,
            this.printToolStripMenuItem,
            this.printPreviewToolStripMenuItem,
            toolStripSeparator8,
            this.recentMapsToolStripMenuItem,
            this.recentTerrainDBsToolStripMenuItem,
            toolStripSeparator13,
            this.exitToolStripMenuItem});
      fileToolStripMenuItem.Name = "fileToolStripMenuItem";
      fileToolStripMenuItem.Size = new System.Drawing.Size(35, 20);
      fileToolStripMenuItem.Text = "&File";
      // 
      // openToolStripMenuItem
      // 
      this.openToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("openToolStripMenuItem.Image")));
      this.openToolStripMenuItem.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.openToolStripMenuItem.Name = "openToolStripMenuItem";
      this.openToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.O)));
      this.openToolStripMenuItem.Size = new System.Drawing.Size(194, 22);
      this.openToolStripMenuItem.Text = "&Open";
      this.openToolStripMenuItem.Click += new System.EventHandler(this.openToolStripMenuItem_Click);
      // 
      // loadTerrainDBToolStripMenuItem
      // 
      this.loadTerrainDBToolStripMenuItem.Name = "loadTerrainDBToolStripMenuItem";
      this.loadTerrainDBToolStripMenuItem.Size = new System.Drawing.Size(194, 22);
      this.loadTerrainDBToolStripMenuItem.Text = "Load Terrain Database";
      this.loadTerrainDBToolStripMenuItem.Click += new System.EventHandler(this.loadTerrainDBToolStripMenuItem_Click);
      // 
      // toolStripSeparator7
      // 
      this.toolStripSeparator7.Name = "toolStripSeparator7";
      this.toolStripSeparator7.Size = new System.Drawing.Size(191, 6);
      // 
      // printToolStripMenuItem
      // 
      this.printToolStripMenuItem.Enabled = false;
      this.printToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("printToolStripMenuItem.Image")));
      this.printToolStripMenuItem.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.printToolStripMenuItem.Name = "printToolStripMenuItem";
      this.printToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.P)));
      this.printToolStripMenuItem.Size = new System.Drawing.Size(194, 22);
      this.printToolStripMenuItem.Text = "&Print";
      this.printToolStripMenuItem.Click += new System.EventHandler(FilePrintMenu_Click);
      // 
      // printPreviewToolStripMenuItem
      // 
      this.printPreviewToolStripMenuItem.Enabled = false;
      this.printPreviewToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("printPreviewToolStripMenuItem.Image")));
      this.printPreviewToolStripMenuItem.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.printPreviewToolStripMenuItem.Name = "printPreviewToolStripMenuItem";
      this.printPreviewToolStripMenuItem.Size = new System.Drawing.Size(194, 22);
      this.printPreviewToolStripMenuItem.Text = "Print Pre&view";
      this.printPreviewToolStripMenuItem.Click += new System.EventHandler(printPreviewToolStripMenuItem_Click);
      // 
      // toolStripSeparator8
      // 
      toolStripSeparator8.Name = "toolStripSeparator8";
      toolStripSeparator8.Size = new System.Drawing.Size(191, 6);
      // 
      // recentMapsToolStripMenuItem
      // 
      this.recentMapsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.recentMap1ToolStripMenuItem,
            this.recentMap2ToolStripMenuItem,
            this.recentMap3ToolStripMenuItem,
            this.recentMap4ToolStripMenuItem});
      this.recentMapsToolStripMenuItem.Enabled = false;
      this.recentMapsToolStripMenuItem.Name = "recentMapsToolStripMenuItem";
      this.recentMapsToolStripMenuItem.Size = new System.Drawing.Size(194, 22);
      this.recentMapsToolStripMenuItem.Text = "Recent Maps";
      // 
      // recentMap1ToolStripMenuItem
      // 
      this.recentMap1ToolStripMenuItem.Name = "recentMap1ToolStripMenuItem";
      this.recentMap1ToolStripMenuItem.Size = new System.Drawing.Size(151, 22);
      this.recentMap1ToolStripMenuItem.Tag = 1;
      this.recentMap1ToolStripMenuItem.Text = "Recent Map 1";
      this.recentMap1ToolStripMenuItem.Visible = false;
      this.recentMap1ToolStripMenuItem.Click += new System.EventHandler(this.recentMapToolStripMenuItem_Click);
      // 
      // recentMap2ToolStripMenuItem
      // 
      this.recentMap2ToolStripMenuItem.Name = "recentMap2ToolStripMenuItem";
      this.recentMap2ToolStripMenuItem.Size = new System.Drawing.Size(151, 22);
      this.recentMap2ToolStripMenuItem.Tag = 2;
      this.recentMap2ToolStripMenuItem.Text = "Recent Map 2";
      this.recentMap2ToolStripMenuItem.Visible = false;
      this.recentMap2ToolStripMenuItem.Click += new System.EventHandler(this.recentMapToolStripMenuItem_Click);
      // 
      // recentMap3ToolStripMenuItem
      // 
      this.recentMap3ToolStripMenuItem.Name = "recentMap3ToolStripMenuItem";
      this.recentMap3ToolStripMenuItem.Size = new System.Drawing.Size(151, 22);
      this.recentMap3ToolStripMenuItem.Tag = 3;
      this.recentMap3ToolStripMenuItem.Text = "Recent Map 3";
      this.recentMap3ToolStripMenuItem.Visible = false;
      this.recentMap3ToolStripMenuItem.Click += new System.EventHandler(this.recentMapToolStripMenuItem_Click);
      // 
      // recentMap4ToolStripMenuItem
      // 
      this.recentMap4ToolStripMenuItem.Name = "recentMap4ToolStripMenuItem";
      this.recentMap4ToolStripMenuItem.Size = new System.Drawing.Size(151, 22);
      this.recentMap4ToolStripMenuItem.Tag = 4;
      this.recentMap4ToolStripMenuItem.Text = "Recent Map 4";
      this.recentMap4ToolStripMenuItem.Visible = false;
      this.recentMap4ToolStripMenuItem.Click += new System.EventHandler(this.recentMapToolStripMenuItem_Click);
      // 
      // recentTerrainDBsToolStripMenuItem
      // 
      this.recentTerrainDBsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.recentTerrain1ToolStripMenuItem,
            this.recentTerrain2ToolStripMenuItem,
            this.recentTerrain3ToolStripMenuItem,
            this.recentTerrain4ToolStripMenuItem});
      this.recentTerrainDBsToolStripMenuItem.Enabled = false;
      this.recentTerrainDBsToolStripMenuItem.Name = "recentTerrainDBsToolStripMenuItem";
      this.recentTerrainDBsToolStripMenuItem.Size = new System.Drawing.Size(194, 22);
      this.recentTerrainDBsToolStripMenuItem.Text = "Recent Terrain DBs";
      // 
      // recentTerrain1ToolStripMenuItem
      // 
      this.recentTerrain1ToolStripMenuItem.Name = "recentTerrain1ToolStripMenuItem";
      this.recentTerrain1ToolStripMenuItem.Size = new System.Drawing.Size(165, 22);
      this.recentTerrain1ToolStripMenuItem.Tag = 1;
      this.recentTerrain1ToolStripMenuItem.Text = "Recent Terrain 1";
      this.recentTerrain1ToolStripMenuItem.Visible = false;
      this.recentTerrain1ToolStripMenuItem.Click += new System.EventHandler(this.recentTerrainToolStripMenuItem_Click);
      // 
      // recentTerrain2ToolStripMenuItem
      // 
      this.recentTerrain2ToolStripMenuItem.Name = "recentTerrain2ToolStripMenuItem";
      this.recentTerrain2ToolStripMenuItem.Size = new System.Drawing.Size(165, 22);
      this.recentTerrain2ToolStripMenuItem.Tag = 2;
      this.recentTerrain2ToolStripMenuItem.Text = "Recent Terrain 2";
      this.recentTerrain2ToolStripMenuItem.Visible = false;
      // 
      // recentTerrain3ToolStripMenuItem
      // 
      this.recentTerrain3ToolStripMenuItem.Name = "recentTerrain3ToolStripMenuItem";
      this.recentTerrain3ToolStripMenuItem.Size = new System.Drawing.Size(165, 22);
      this.recentTerrain3ToolStripMenuItem.Tag = 3;
      this.recentTerrain3ToolStripMenuItem.Text = "Recent Terrain 3";
      this.recentTerrain3ToolStripMenuItem.Visible = false;
      // 
      // recentTerrain4ToolStripMenuItem
      // 
      this.recentTerrain4ToolStripMenuItem.Name = "recentTerrain4ToolStripMenuItem";
      this.recentTerrain4ToolStripMenuItem.Size = new System.Drawing.Size(165, 22);
      this.recentTerrain4ToolStripMenuItem.Tag = 4;
      this.recentTerrain4ToolStripMenuItem.Text = "Recent Terrain 4";
      this.recentTerrain4ToolStripMenuItem.Visible = false;
      // 
      // toolStripSeparator13
      // 
      toolStripSeparator13.Name = "toolStripSeparator13";
      toolStripSeparator13.Size = new System.Drawing.Size(191, 6);
      // 
      // exitToolStripMenuItem
      // 
      this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
      this.exitToolStripMenuItem.Size = new System.Drawing.Size(194, 22);
      this.exitToolStripMenuItem.Text = "E&xit";
      this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
      // 
      // editToolStripMenuItem
      // 
      this.editToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.copyToolStripMenuItem});
      this.editToolStripMenuItem.Name = "editToolStripMenuItem";
      this.editToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
      this.editToolStripMenuItem.Text = "&Edit";
      // 
      // copyToolStripMenuItem
      // 
      this.copyToolStripMenuItem.Enabled = false;
      this.copyToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("copyToolStripMenuItem.Image")));
      this.copyToolStripMenuItem.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.copyToolStripMenuItem.Name = "copyToolStripMenuItem";
      this.copyToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.C)));
      this.copyToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
      this.copyToolStripMenuItem.Text = "&Copy";
      this.copyToolStripMenuItem.Click += new System.EventHandler(this.copyToolStripMenuItem_Click);
      // 
      // toolsToolStripMenuItem
      // 
      this.toolsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.zoomInToolStripMenuItem,
            this.zoomOutToolStripMenuItem,
            this.resetToolStripMenuItem,
            toolStripSeparator6,
            this.zoomToolStripMenuItem,
            this.panToolStripMenuItem,
            this.zoomPanToolStripMenuItem,
            toolStripSeparator9,
            this.terrainInfoToolStripMenuItem,
            toolStripSeparator10,
            this.crossSectionToolStripMenuItem,
            this.lineOfSightToolStripMenuItem,
            this.viewShedToolStripMenuItem,
            toolStripSeparator12,
            this.optionsToolStripMenuItem});
      this.toolsToolStripMenuItem.Name = "toolsToolStripMenuItem";
      this.toolsToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
      this.toolsToolStripMenuItem.Text = "&Tools";
      // 
      // zoomInToolStripMenuItem
      // 
      this.zoomInToolStripMenuItem.Enabled = false;
      this.zoomInToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("zoomInToolStripMenuItem.Image")));
      this.zoomInToolStripMenuItem.Name = "zoomInToolStripMenuItem";
      this.zoomInToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
      this.zoomInToolStripMenuItem.Text = "Zoom In";
      this.zoomInToolStripMenuItem.Click += new System.EventHandler(this.tvToolStrip_ButtonClick);
      // 
      // zoomOutToolStripMenuItem
      // 
      this.zoomOutToolStripMenuItem.Enabled = false;
      this.zoomOutToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("zoomOutToolStripMenuItem.Image")));
      this.zoomOutToolStripMenuItem.Name = "zoomOutToolStripMenuItem";
      this.zoomOutToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
      this.zoomOutToolStripMenuItem.Text = "Zoom Out";
      this.zoomOutToolStripMenuItem.Click += new System.EventHandler(this.tvToolStrip_ButtonClick);
      // 
      // resetToolStripMenuItem
      // 
      this.resetToolStripMenuItem.Enabled = false;
      this.resetToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("resetToolStripMenuItem.Image")));
      this.resetToolStripMenuItem.Name = "resetToolStripMenuItem";
      this.resetToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
      this.resetToolStripMenuItem.Text = "Reset";
      this.resetToolStripMenuItem.Click += new System.EventHandler(this.tvToolStrip_ButtonClick);
      // 
      // toolStripSeparator6
      // 
      toolStripSeparator6.Name = "toolStripSeparator6";
      toolStripSeparator6.Size = new System.Drawing.Size(149, 6);
      // 
      // zoomToolStripMenuItem
      // 
      this.zoomToolStripMenuItem.Enabled = false;
      this.zoomToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("zoomToolStripMenuItem.Image")));
      this.zoomToolStripMenuItem.Name = "zoomToolStripMenuItem";
      this.zoomToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
      this.zoomToolStripMenuItem.Tag = 1;
      this.zoomToolStripMenuItem.Text = "Zoom";
      this.zoomToolStripMenuItem.Click += new System.EventHandler(this.tvToolStrip_ButtonClick);
      // 
      // panToolStripMenuItem
      // 
      this.panToolStripMenuItem.Enabled = false;
      this.panToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("panToolStripMenuItem.Image")));
      this.panToolStripMenuItem.Name = "panToolStripMenuItem";
      this.panToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
      this.panToolStripMenuItem.Tag = 2;
      this.panToolStripMenuItem.Text = "Pan";
      this.panToolStripMenuItem.Click += new System.EventHandler(this.tvToolStrip_ButtonClick);
      // 
      // zoomPanToolStripMenuItem
      // 
      this.zoomPanToolStripMenuItem.Enabled = false;
      this.zoomPanToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("zoomPanToolStripMenuItem.Image")));
      this.zoomPanToolStripMenuItem.Name = "zoomPanToolStripMenuItem";
      this.zoomPanToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
      this.zoomPanToolStripMenuItem.Tag = 3;
      this.zoomPanToolStripMenuItem.Text = "Zoom/Pan";
      this.zoomPanToolStripMenuItem.Click += new System.EventHandler(this.tvToolStrip_ButtonClick);
      // 
      // toolStripSeparator9
      // 
      toolStripSeparator9.Name = "toolStripSeparator9";
      toolStripSeparator9.Size = new System.Drawing.Size(149, 6);
      // 
      // terrainInfoToolStripMenuItem
      // 
      this.terrainInfoToolStripMenuItem.Enabled = false;
      this.terrainInfoToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("terrainInfoToolStripMenuItem.Image")));
      this.terrainInfoToolStripMenuItem.Name = "terrainInfoToolStripMenuItem";
      this.terrainInfoToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
      this.terrainInfoToolStripMenuItem.Text = "Terrain Info";
      this.terrainInfoToolStripMenuItem.Click += new System.EventHandler(this.tvToolStrip_ButtonClick);
      // 
      // toolStripSeparator10
      // 
      toolStripSeparator10.Name = "toolStripSeparator10";
      toolStripSeparator10.Size = new System.Drawing.Size(149, 6);
      // 
      // crossSectionToolStripMenuItem
      // 
      this.crossSectionToolStripMenuItem.Enabled = false;
      this.crossSectionToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("crossSectionToolStripMenuItem.Image")));
      this.crossSectionToolStripMenuItem.Name = "crossSectionToolStripMenuItem";
      this.crossSectionToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
      this.crossSectionToolStripMenuItem.Text = "Cross Section";
      this.crossSectionToolStripMenuItem.Tag = 4;
      this.crossSectionToolStripMenuItem.Click += new System.EventHandler(this.tvToolStrip_ButtonClick);
      // 
      // lineOfSightToolStripMenuItem
      // 
      this.lineOfSightToolStripMenuItem.Enabled = false;
      this.lineOfSightToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("lineOfSightToolStripMenuItem.Image")));
      this.lineOfSightToolStripMenuItem.Name = "lineOfSightToolStripMenuItem";
      this.lineOfSightToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
      this.lineOfSightToolStripMenuItem.Text = "Line Of Sight";
      this.lineOfSightToolStripMenuItem.Tag = 5;
      this.lineOfSightToolStripMenuItem.Click += new System.EventHandler(this.tvToolStrip_ButtonClick);
      // 
      // viewShedToolStripMenuItem
      // 
      this.viewShedToolStripMenuItem.Enabled = false;
      this.viewShedToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("viewShedToolStripMenuItem.Image")));
      this.viewShedToolStripMenuItem.Name = "viewShedToolStripMenuItem";
      this.viewShedToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
      this.viewShedToolStripMenuItem.Text = "View Shed";
      this.viewShedToolStripMenuItem.Tag = 6;
      this.viewShedToolStripMenuItem.Click += new System.EventHandler(this.tvToolStrip_ButtonClick);
      // 
      // toolStripSeparator12
      // 
      toolStripSeparator12.Name = "toolStripSeparator12";
      toolStripSeparator12.Size = new System.Drawing.Size(149, 6);
      // 
      // optionsToolStripMenuItem
      // 
      this.optionsToolStripMenuItem.Name = "optionsToolStripMenuItem";
      this.optionsToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
      this.optionsToolStripMenuItem.Text = "Options";
      this.optionsToolStripMenuItem.Click += new System.EventHandler(this.optionsToolStripMenuItem_Click);
      // 
      // helpToolStripMenuItem
      // 
      this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.aboutToolStripMenuItem});
      this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
      this.helpToolStripMenuItem.Size = new System.Drawing.Size(40, 20);
      this.helpToolStripMenuItem.Text = "&Help";
      // 
      // aboutToolStripMenuItem
      // 
      this.aboutToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("aboutToolStripMenuItem.Image")));
      this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
      this.aboutToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
      this.aboutToolStripMenuItem.Text = "&About...";
      this.aboutToolStripMenuItem.Click += new System.EventHandler(this.aboutToolStripMenuItem_Click);
      // 
      // tvToolStrip
      // 
      this.tvToolStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openToolStripButton,
            this.printToolStripButton,
            toolStripSeparator,
            this.copyToolStripButton,
            toolStripSeparator1,
            this.zoomInToolStripButton,
            this.zoomOutToolStripButton,
            this.resetToolStripButton,
            toolStripSeparator2,
            this.zoomToolStripButton,
            this.panToolStripButton,
            this.zoomPanToolStripButton,
            toolStripSeparator3,
            this.terrainInfoToolStripButton,
            toolStripSeparator5,
            this.crossSectionToolStripButton,
            this.losToolStripButton,
            this.viewShedToolStripButton,
            toolStripSeparator4,
            this.aboutToolStripButton});
      this.tvToolStrip.Location = new System.Drawing.Point(0, 24);
      this.tvToolStrip.Name = "tvToolStrip";
      this.tvToolStrip.Size = new System.Drawing.Size(913, 25);
      this.tvToolStrip.TabIndex = 2;
      this.tvToolStrip.Text = "toolStrip1";
      // 
      // openToolStripButton
      // 
      this.openToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.openToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("openToolStripButton.Image")));
      this.openToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.openToolStripButton.Name = "openToolStripButton";
      this.openToolStripButton.Size = new System.Drawing.Size(23, 22);
      this.openToolStripButton.Text = "&Open";
      this.openToolStripButton.Click += new System.EventHandler(this.tvToolStrip_ButtonClick);
      // 
      // printToolStripButton
      // 
      this.printToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.printToolStripButton.Enabled = false;
      this.printToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("printToolStripButton.Image")));
      this.printToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.printToolStripButton.Name = "printToolStripButton";
      this.printToolStripButton.Size = new System.Drawing.Size(23, 22);
      this.printToolStripButton.Text = "&Print";
      this.printToolStripButton.Click += new System.EventHandler(this.FilePrintMenu_Click);
      // 
      // toolStripSeparator
      // 
      toolStripSeparator.Name = "toolStripSeparator";
      toolStripSeparator.Size = new System.Drawing.Size(6, 25);
      // 
      // copyToolStripButton
      // 
      this.copyToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.copyToolStripButton.Enabled = false;
      this.copyToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("copyToolStripButton.Image")));
      this.copyToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.copyToolStripButton.Name = "copyToolStripButton";
      this.copyToolStripButton.Size = new System.Drawing.Size(23, 22);
      this.copyToolStripButton.Text = "&Copy";
      this.copyToolStripButton.Click += new System.EventHandler(this.tvToolStrip_ButtonClick);
      // 
      // toolStripSeparator1
      // 
      toolStripSeparator1.Name = "toolStripSeparator1";
      toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
      // 
      // zoomInToolStripButton
      // 
      this.zoomInToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.zoomInToolStripButton.Enabled = false;
      this.zoomInToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("zoomInToolStripButton.Image")));
      this.zoomInToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.zoomInToolStripButton.Name = "zoomInToolStripButton";
      this.zoomInToolStripButton.Size = new System.Drawing.Size(23, 22);
      this.zoomInToolStripButton.Text = "Zoom In";
      this.zoomInToolStripButton.Click += new System.EventHandler(this.tvToolStrip_ButtonClick);
      // 
      // zoomOutToolStripButton
      // 
      this.zoomOutToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.zoomOutToolStripButton.Enabled = false;
      this.zoomOutToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("zoomOutToolStripButton.Image")));
      this.zoomOutToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.zoomOutToolStripButton.Name = "zoomOutToolStripButton";
      this.zoomOutToolStripButton.Size = new System.Drawing.Size(23, 22);
      this.zoomOutToolStripButton.Text = "Zoom Out";
      this.zoomOutToolStripButton.Click += new System.EventHandler(this.tvToolStrip_ButtonClick);
      // 
      // resetToolStripButton
      // 
      this.resetToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.resetToolStripButton.Enabled = false;
      this.resetToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("resetToolStripButton.Image")));
      this.resetToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.resetToolStripButton.Name = "resetToolStripButton";
      this.resetToolStripButton.Size = new System.Drawing.Size(23, 22);
      this.resetToolStripButton.Text = "Reset";
      this.resetToolStripButton.Click += new System.EventHandler(this.tvToolStrip_ButtonClick);
      // 
      // toolStripSeparator2
      // 
      toolStripSeparator2.Name = "toolStripSeparator2";
      toolStripSeparator2.Size = new System.Drawing.Size(6, 25);
      // 
      // zoomToolStripButton
      // 
      this.zoomToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.zoomToolStripButton.Enabled = false;
      this.zoomToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("zoomToolStripButton.Image")));
      this.zoomToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.zoomToolStripButton.Name = "zoomToolStripButton";
      this.zoomToolStripButton.Size = new System.Drawing.Size(23, 22);
      this.zoomToolStripButton.Tag = 1;
      this.zoomToolStripButton.Text = "Zoom";
      this.zoomToolStripButton.Click += new System.EventHandler(this.tvToolStrip_ButtonClick);
      // 
      // panToolStripButton
      // 
      this.panToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.panToolStripButton.Enabled = false;
      this.panToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("panToolStripButton.Image")));
      this.panToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.panToolStripButton.Name = "panToolStripButton";
      this.panToolStripButton.Size = new System.Drawing.Size(23, 22);
      this.panToolStripButton.Tag = 2;
      this.panToolStripButton.Text = "Pan";
      this.panToolStripButton.Click += new System.EventHandler(this.tvToolStrip_ButtonClick);
      // 
      // zoomPanToolStripButton
      // 
      this.zoomPanToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.zoomPanToolStripButton.Enabled = false;
      this.zoomPanToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("zoomPanToolStripButton.Image")));
      this.zoomPanToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.zoomPanToolStripButton.Name = "zoomPanToolStripButton";
      this.zoomPanToolStripButton.Size = new System.Drawing.Size(23, 22);
      this.zoomPanToolStripButton.Tag = 3;
      this.zoomPanToolStripButton.Text = "Zoom & Pan";
      this.zoomPanToolStripButton.Click += new System.EventHandler(this.tvToolStrip_ButtonClick);
      // 
      // toolStripSeparator3
      // 
      toolStripSeparator3.Name = "toolStripSeparator3";
      toolStripSeparator3.Size = new System.Drawing.Size(6, 25);
      // 
      // terrainInfoToolStripButton
      // 
      this.terrainInfoToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.terrainInfoToolStripButton.Enabled = false;
      this.terrainInfoToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("terrainInfoToolStripButton.Image")));
      this.terrainInfoToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.terrainInfoToolStripButton.Name = "terrainInfoToolStripButton";
      this.terrainInfoToolStripButton.Size = new System.Drawing.Size(23, 22);
      this.terrainInfoToolStripButton.Text = "Terrain Info";
      this.terrainInfoToolStripButton.Click += new System.EventHandler(this.tvToolStrip_ButtonClick);
      // 
      // toolStripSeparator5
      // 
      toolStripSeparator5.Name = "toolStripSeparator5";
      toolStripSeparator5.Size = new System.Drawing.Size(6, 25);
      // 
      // crossSectionToolStripButton
      // 
      this.crossSectionToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.crossSectionToolStripButton.Enabled = false;
      this.crossSectionToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("crossSectionToolStripButton.Image")));
      this.crossSectionToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.crossSectionToolStripButton.Name = "crossSectionToolStripButton";
      this.crossSectionToolStripButton.Size = new System.Drawing.Size(23, 22);
      this.crossSectionToolStripButton.Tag = 4;
      this.crossSectionToolStripButton.Text = "Display a Cross-Section";
      this.crossSectionToolStripButton.Click += new System.EventHandler(this.tvToolStrip_ButtonClick);
      // 
      // losToolStripButton
      // 
      this.losToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.losToolStripButton.Enabled = false;
      this.losToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("losToolStripButton.Image")));
      this.losToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.losToolStripButton.Name = "losToolStripButton";
      this.losToolStripButton.Size = new System.Drawing.Size(23, 22);
      this.losToolStripButton.Tag = 5;
      this.losToolStripButton.Text = "Line Of Sight";
      this.losToolStripButton.Click += new System.EventHandler(this.tvToolStrip_ButtonClick);
      // 
      // viewShedToolStripButton
      // 
      this.viewShedToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.viewShedToolStripButton.Enabled = false;
      this.viewShedToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("viewShedToolStripButton.Image")));
      this.viewShedToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.viewShedToolStripButton.Name = "viewShedToolStripButton";
      this.viewShedToolStripButton.Size = new System.Drawing.Size(23, 22);
      this.viewShedToolStripButton.Tag = 6;
      this.viewShedToolStripButton.Text = "View Shed";
      this.viewShedToolStripButton.Click += new System.EventHandler(this.tvToolStrip_ButtonClick);
      // 
      // toolStripSeparator4
      // 
      toolStripSeparator4.Name = "toolStripSeparator4";
      toolStripSeparator4.Size = new System.Drawing.Size(6, 25);
      // 
      // aboutToolStripButton
      // 
      this.aboutToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.aboutToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("aboutToolStripButton.Image")));
      this.aboutToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.aboutToolStripButton.Name = "aboutToolStripButton";
      this.aboutToolStripButton.Size = new System.Drawing.Size(23, 22);
      this.aboutToolStripButton.Text = "About...";
      this.aboutToolStripButton.Click += new System.EventHandler(this.aboutToolStripMenuItem_Click);
      // 
      // tvStatusStrip
      // 
      this.tvStatusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.statusToolStripLabel,
            this.projectionToolStripLabel});
      this.tvStatusStrip.Location = new System.Drawing.Point(0, 653);
      this.tvStatusStrip.Name = "tvStatusStrip";
      this.tvStatusStrip.Size = new System.Drawing.Size(913, 22);
      this.tvStatusStrip.TabIndex = 3;
      this.tvStatusStrip.Text = "statusStrip1";
      // 
      // statusToolStripLabel
      // 
      this.statusToolStripLabel.AutoSize = false;
      this.statusToolStripLabel.BorderSides = ((System.Windows.Forms.ToolStripStatusLabelBorderSides)((((System.Windows.Forms.ToolStripStatusLabelBorderSides.Left | System.Windows.Forms.ToolStripStatusLabelBorderSides.Top)
                  | System.Windows.Forms.ToolStripStatusLabelBorderSides.Right)
                  | System.Windows.Forms.ToolStripStatusLabelBorderSides.Bottom)));
      this.statusToolStripLabel.BorderStyle = System.Windows.Forms.Border3DStyle.Sunken;
      this.statusToolStripLabel.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
      this.statusToolStripLabel.Margin = new System.Windows.Forms.Padding(3);
      this.statusToolStripLabel.Name = "statusToolStripLabel";
      this.statusToolStripLabel.Padding = new System.Windows.Forms.Padding(3, 0, 0, 0);
      this.statusToolStripLabel.Size = new System.Drawing.Size(100, 16);
      this.statusToolStripLabel.Text = "Ready";
      this.statusToolStripLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // projectionToolStripLabel
      // 
      this.projectionToolStripLabel.AutoSize = false;
      this.projectionToolStripLabel.BorderSides = ((System.Windows.Forms.ToolStripStatusLabelBorderSides)((((System.Windows.Forms.ToolStripStatusLabelBorderSides.Left | System.Windows.Forms.ToolStripStatusLabelBorderSides.Top)
                  | System.Windows.Forms.ToolStripStatusLabelBorderSides.Right)
                  | System.Windows.Forms.ToolStripStatusLabelBorderSides.Bottom)));
      this.projectionToolStripLabel.BorderStyle = System.Windows.Forms.Border3DStyle.Sunken;
      this.projectionToolStripLabel.Name = "projectionToolStripLabel";
      this.projectionToolStripLabel.Size = new System.Drawing.Size(350, 17);
      // 
      // tvSplitContainer
      // 
      this.tvSplitContainer.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
      this.tvSplitContainer.Dock = System.Windows.Forms.DockStyle.Fill;
      this.tvSplitContainer.Location = new System.Drawing.Point(0, 49);
      this.tvSplitContainer.Name = "tvSplitContainer";
      this.tvSplitContainer.Orientation = System.Windows.Forms.Orientation.Horizontal;
      // 
      // tvSplitContainer.Panel1
      // 
      this.tvSplitContainer.Panel1.Controls.Add(this.tvMapPanel);
      // 
      // tvSplitContainer.Panel2
      // 
      this.tvSplitContainer.Panel2.Controls.Add(this.tvCrossSectionPanel);
      this.tvSplitContainer.Size = new System.Drawing.Size(913, 604);
      this.tvSplitContainer.SplitterDistance = 346;
      this.tvSplitContainer.SplitterWidth = 2;
      this.tvSplitContainer.TabIndex = 4;
      // 
      // tvMapPanel
      // 
      this.tvMapPanel.Dock = System.Windows.Forms.DockStyle.Fill;
      this.tvMapPanel.Location = new System.Drawing.Point(0, 0);
      this.tvMapPanel.Name = "tvMapPanel";
      this.tvMapPanel.Size = new System.Drawing.Size(909, 342);
      this.tvMapPanel.TabIndex = 0;
      // 
      // tvCrossSectionPanel
      // 
      this.tvCrossSectionPanel.Dock = System.Windows.Forms.DockStyle.Fill;
      this.tvCrossSectionPanel.Location = new System.Drawing.Point(0, 0);
      this.tvCrossSectionPanel.Name = "tvCrossSectionPanel";
      this.tvCrossSectionPanel.Size = new System.Drawing.Size(909, 252);
      this.tvCrossSectionPanel.TabIndex = 0;
      // 
      // FileOpenDialog
      // 
      this.FileOpenDialog.DefaultExt = "map";
      this.FileOpenDialog.Filter = "MapLink Maps|*.map;*.mpc|All files|*.*";
      this.FileOpenDialog.Title = "Open a MapLink Map";
      // 
      // FileOpenTerrainDialog
      // 
      this.FileOpenTerrainDialog.DefaultExt = "tdf";
      this.FileOpenTerrainDialog.Filter = "MapLink Terrain Databases|*.tdf|All files|*.*";
      this.FileOpenTerrainDialog.Title = "Open a Terrain Database";
      // 
      // TerrainViewer
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(913, 675);
      this.Controls.Add(this.tvSplitContainer);
      this.Controls.Add(this.tvStatusStrip);
      this.Controls.Add(this.tvToolStrip);
      this.Controls.Add(this.tvMenuStrip);
      this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
      this.MainMenuStrip = this.tvMenuStrip;
      this.Name = "TerrainViewer";
      this.Text = "Untitled - C# Terrain .NET Sample";
      this.MouseWheel += new System.Windows.Forms.MouseEventHandler(this.OnMouseWheel);
      this.Load += new System.EventHandler(this.OnLoad);
      this.tvMenuStrip.ResumeLayout(false);
      this.tvMenuStrip.PerformLayout();
      this.tvToolStrip.ResumeLayout(false);
      this.tvToolStrip.PerformLayout();
      this.tvStatusStrip.ResumeLayout(false);
      this.tvStatusStrip.PerformLayout();
      this.tvSplitContainer.ResumeLayout(false);
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.MenuStrip tvMenuStrip;
    private System.Windows.Forms.ToolStrip tvToolStrip;
    private System.Windows.Forms.StatusStrip tvStatusStrip;
    private System.Windows.Forms.SplitContainer tvSplitContainer;
    private System.Windows.Forms.ToolStripButton openToolStripButton;
    private System.Windows.Forms.ToolStripButton printToolStripButton;
    private System.Windows.Forms.ToolStripButton copyToolStripButton;
    private System.Windows.Forms.ToolStripButton zoomInToolStripButton;
    private System.Windows.Forms.ToolStripButton zoomOutToolStripButton;
    private System.Windows.Forms.ToolStripButton aboutToolStripButton;
    private System.Windows.Forms.ToolStripStatusLabel statusToolStripLabel;
    private System.Windows.Forms.ToolStripButton resetToolStripButton;
    private System.Windows.Forms.ToolStripButton zoomToolStripButton;
    private System.Windows.Forms.ToolStripButton panToolStripButton;
    private System.Windows.Forms.ToolStripButton zoomPanToolStripButton;
    private System.Windows.Forms.ToolStripButton terrainInfoToolStripButton;
    private System.Windows.Forms.ToolStripButton crossSectionToolStripButton;
    private System.Windows.Forms.ToolStripButton losToolStripButton;
    private System.Windows.Forms.ToolStripButton viewShedToolStripButton;
    private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
    private System.Windows.Forms.ToolStripSeparator toolStripSeparator7;
    private System.Windows.Forms.ToolStripMenuItem printToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem printPreviewToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem editToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem copyToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem toolsToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem zoomInToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
    private System.Windows.Forms.ToolStripStatusLabel projectionToolStripLabel;
    private CSharpTerrainSample.MapPanel tvMapPanel;
    private CSharpTerrainSample.CrossSectionPanel tvCrossSectionPanel;
    private System.Windows.Forms.OpenFileDialog FileOpenDialog;
    private System.Windows.Forms.ToolStripMenuItem zoomOutToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem resetToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem zoomToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem panToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem zoomPanToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem terrainInfoToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem crossSectionToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem lineOfSightToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem viewShedToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem optionsToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem loadTerrainDBToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem recentMapsToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem recentTerrainDBsToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem recentMap1ToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem recentMap2ToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem recentMap3ToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem recentMap4ToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem recentTerrain1ToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem recentTerrain2ToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem recentTerrain3ToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem recentTerrain4ToolStripMenuItem;
    private System.Windows.Forms.OpenFileDialog FileOpenTerrainDialog;
  }
}

