
namespace CSharp3DSample
{
  partial class MapViewerForm
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
      base.Dispose(disposing);
    }


    #region Windows Form Designer generated code
    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    private void InitializeComponent()
    {
      this.components = new System.ComponentModel.Container();
      System.Windows.Forms.ToolBarButton dumbSeparator1;
      System.Windows.Forms.MenuItem MenuSeparator2;
      System.Windows.Forms.MenuItem MenuSeparator3;
      System.Windows.Forms.ToolBarButton dumbSeparator2;
      System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MapViewerForm));
      this.FileOpenMapDialog = new System.Windows.Forms.OpenFileDialog();
      this.TheMenu = new System.Windows.Forms.MainMenu(this.components);
      this.FileMenu = new System.Windows.Forms.MenuItem();
      this.FileOpenMenu = new System.Windows.Forms.MenuItem();
      this.FileOpenTerrainMenu = new System.Windows.Forms.MenuItem();
      this.FileExitMenu = new System.Windows.Forms.MenuItem();
      this.menuItem2 = new System.Windows.Forms.MenuItem();
      this.EditCopyMenu = new System.Windows.Forms.MenuItem();
      this.ToolsMenu = new System.Windows.Forms.MenuItem();
      this.ToolsReset = new System.Windows.Forms.MenuItem();
      this.ToolsZoom = new System.Windows.Forms.MenuItem();
      this.ToolsFly = new System.Windows.Forms.MenuItem();
      this.ToolsRotate = new System.Windows.Forms.MenuItem();
      this.ToolsCamera = new System.Windows.Forms.MenuItem();
      this.menuItem1 = new System.Windows.Forms.MenuItem();
      this.PrimitiveCreation_Polygon = new System.Windows.Forms.MenuItem();
      this.PrimitiveCreation_ExtrudedPolygon = new System.Windows.Forms.MenuItem();
      this.HelpMenu = new System.Windows.Forms.MenuItem();
      this.HelpAbout = new System.Windows.Forms.MenuItem();
      this.TheToolBar = new System.Windows.Forms.ToolBar();
      this.FileOpenButton = new System.Windows.Forms.ToolBarButton();
      this.EditCopyButton = new System.Windows.Forms.ToolBarButton();
      this.ResetButton = new System.Windows.Forms.ToolBarButton();
      this.ZoomTool = new System.Windows.Forms.ToolBarButton();
      this.FlyTool = new System.Windows.Forms.ToolBarButton();
      this.RotateTool = new System.Windows.Forms.ToolBarButton();
      this.CameraTool = new System.Windows.Forms.ToolBarButton();
      this.theImageList = new System.Windows.Forms.ImageList(this.components);
      this.TheStatusStrip = new System.Windows.Forms.StatusStrip();
      this.StatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
      this.LatitudeLabel = new System.Windows.Forms.ToolStripStatusLabel();
      this.LongitudeLabel = new System.Windows.Forms.ToolStripStatusLabel();
      this.AltitudeLabel = new System.Windows.Forms.ToolStripStatusLabel();
      this.FileOpenTerrainDialog = new System.Windows.Forms.OpenFileDialog();
      this.toolStripContainer1 = new System.Windows.Forms.ToolStripContainer();
      this.ViewerPanel = new CSharp3DSample.MapViewerPanel();
      dumbSeparator1 = new System.Windows.Forms.ToolBarButton();
      MenuSeparator2 = new System.Windows.Forms.MenuItem();
      MenuSeparator3 = new System.Windows.Forms.MenuItem();
      dumbSeparator2 = new System.Windows.Forms.ToolBarButton();
      this.TheStatusStrip.SuspendLayout();
      this.toolStripContainer1.BottomToolStripPanel.SuspendLayout();
      this.toolStripContainer1.ContentPanel.SuspendLayout();
      this.toolStripContainer1.SuspendLayout();
      this.SuspendLayout();
      // 
      // dumbSeparator1
      // 
      dumbSeparator1.Name = "dumbSeparator1";
      dumbSeparator1.Style = System.Windows.Forms.ToolBarButtonStyle.Separator;
      // 
      // MenuSeparator2
      // 
      MenuSeparator2.Index = 2;
      MenuSeparator2.Text = "-";
      // 
      // MenuSeparator3
      // 
      MenuSeparator3.Index = 1;
      MenuSeparator3.Text = "-";
      // 
      // dumbSeparator2
      // 
      dumbSeparator2.Name = "dumbSeparator2";
      dumbSeparator2.Style = System.Windows.Forms.ToolBarButtonStyle.Separator;
      // 
      // FileOpenMapDialog
      // 
      this.FileOpenMapDialog.DefaultExt = "map";
      this.FileOpenMapDialog.Filter = "MapLink Maps|*.map;*.mpc|All files|*.*";
      this.FileOpenMapDialog.ReadOnlyChecked = true;
      this.FileOpenMapDialog.RestoreDirectory = true;
      this.FileOpenMapDialog.Title = "Open a MapLink Map";
      // 
      // TheMenu
      // 
      this.TheMenu.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.FileMenu,
            this.menuItem2,
            this.ToolsMenu,
            this.menuItem1,
            this.HelpMenu});
      // 
      // FileMenu
      // 
      this.FileMenu.Index = 0;
      this.FileMenu.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.FileOpenMenu,
            this.FileOpenTerrainMenu,
            MenuSeparator2,
            this.FileExitMenu});
      this.FileMenu.Text = "File";
      // 
      // FileOpenMenu
      // 
      this.FileOpenMenu.Index = 0;
      this.FileOpenMenu.Text = "Open Map";
      this.FileOpenMenu.Click += new System.EventHandler(this.FileOpenMenu_Click);
      // 
      // FileOpenTerrainMenu
      // 
      this.FileOpenTerrainMenu.Index = 1;
      this.FileOpenTerrainMenu.Text = "Open Terrain Database";
      this.FileOpenTerrainMenu.Click += new System.EventHandler(this.FileOpenTerrainMenu_Click);
      // 
      // FileExitMenu
      // 
      this.FileExitMenu.Index = 3;
      this.FileExitMenu.Text = "Exit";
      this.FileExitMenu.Click += new System.EventHandler(this.FileExitMenu_Click);
      // 
      // menuItem2
      // 
      this.menuItem2.Index = 1;
      this.menuItem2.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.EditCopyMenu});
      this.menuItem2.Text = "Edit";
      // 
      // EditCopyMenu
      // 
      this.EditCopyMenu.Index = 0;
      this.EditCopyMenu.Text = "Copy";
      this.EditCopyMenu.Click += new System.EventHandler(this.EditCopyMenu_Click);
      // 
      // ToolsMenu
      // 
      this.ToolsMenu.Index = 2;
      this.ToolsMenu.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.ToolsReset,
            MenuSeparator3,
            this.ToolsZoom,
            this.ToolsFly,
            this.ToolsRotate,
            this.ToolsCamera});
      this.ToolsMenu.Text = "Tools";
      // 
      // ToolsReset
      // 
      this.ToolsReset.Index = 0;
      this.ToolsReset.Text = "Reset";
      this.ToolsReset.Click += new System.EventHandler(this.MenuNonImode_Click);
      // 
      // ToolsZoom
      // 
      this.ToolsZoom.Checked = true;
      this.ToolsZoom.Index = 2;
      this.ToolsZoom.RadioCheck = true;
      this.ToolsZoom.Text = "Zoom";
      this.ToolsZoom.Click += new System.EventHandler(this.MenuImode_Click);
      // 
      // ToolsFly
      // 
      this.ToolsFly.Index = 3;
      this.ToolsFly.RadioCheck = true;
      this.ToolsFly.Text = "Fly";
      this.ToolsFly.Click += new System.EventHandler(this.MenuImode_Click);
      // 
      // ToolsRotate
      // 
      this.ToolsRotate.Index = 4;
      this.ToolsRotate.RadioCheck = true;
      this.ToolsRotate.Text = "Rotate";
      this.ToolsRotate.Click += new System.EventHandler(this.MenuImode_Click);
      // 
      // ToolsCamera
      // 
      this.ToolsCamera.Index = 5;
      this.ToolsCamera.RadioCheck = true;
      this.ToolsCamera.Text = "Camera";
      this.ToolsCamera.Click += new System.EventHandler(this.MenuImode_Click);
      // 
      // menuItem1
      // 
      this.menuItem1.Index = 3;
      this.menuItem1.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.PrimitiveCreation_Polygon,
            this.PrimitiveCreation_ExtrudedPolygon});
      this.menuItem1.Text = "Primitive Creation";
      // 
      // PrimitiveCreation_Polygon
      // 
      this.PrimitiveCreation_Polygon.Index = 0;
      this.PrimitiveCreation_Polygon.Text = "Polygon";
      this.PrimitiveCreation_Polygon.Click += new System.EventHandler(this.PrimitiveCreation_Polygon_Click);
      // 
      // PrimitiveCreation_ExtrudedPolygon
      // 
      this.PrimitiveCreation_ExtrudedPolygon.Index = 1;
      this.PrimitiveCreation_ExtrudedPolygon.Text = "Extruded Polygon";
      this.PrimitiveCreation_ExtrudedPolygon.Click += new System.EventHandler(this.PrimitiveCreation_ExtrudedPolygon_Click);
      // 
      // HelpMenu
      // 
      this.HelpMenu.Index = 4;
      this.HelpMenu.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.HelpAbout});
      this.HelpMenu.Text = "Help";
      // 
      // HelpAbout
      // 
      this.HelpAbout.Index = 0;
      this.HelpAbout.Text = "About";
      this.HelpAbout.Click += new System.EventHandler(this.HelpAboutMenu_Click);
      // 
      // TheToolBar
      // 
      this.TheToolBar.Buttons.AddRange(new System.Windows.Forms.ToolBarButton[] {
            this.FileOpenButton,
            this.EditCopyButton,
            dumbSeparator1,
            this.ResetButton,
            dumbSeparator2,
            this.ZoomTool,
            this.FlyTool,
            this.RotateTool,
            this.CameraTool});
      this.TheToolBar.ButtonSize = new System.Drawing.Size(24, 24);
      this.TheToolBar.DropDownArrows = true;
      this.TheToolBar.ImageList = this.theImageList;
      this.TheToolBar.Location = new System.Drawing.Point(0, 0);
      this.TheToolBar.Name = "TheToolBar";
      this.TheToolBar.ShowToolTips = true;
      this.TheToolBar.Size = new System.Drawing.Size(988, 36);
      this.TheToolBar.TabIndex = 1;
      this.TheToolBar.ButtonClick += new System.Windows.Forms.ToolBarButtonClickEventHandler(this.TheToolBar_ButtonClick);
      // 
      // FileOpenButton
      // 
      this.FileOpenButton.ImageIndex = 0;
      this.FileOpenButton.Name = "FileOpenButton";
      // 
      // FilePrintButton
      // 
      this.EditCopyButton.ImageIndex = 1;
      this.EditCopyButton.Name = "EditCopyButton";
      // 
      // ResetButton
      // 
      this.ResetButton.ImageIndex = 2;
      this.ResetButton.Name = "ResetButton";
      // 
      // ZoomTool
      // 
      this.ZoomTool.ImageIndex = 3;
      this.ZoomTool.Name = "ZoomTool";
      this.ZoomTool.Pushed = true;
      // 
      // FlyTool
      // 
      this.FlyTool.ImageIndex = 4;
      this.FlyTool.Name = "FlyTool";
      // 
      // RotateTool
      // 
      this.RotateTool.ImageIndex = 6;
      this.RotateTool.Name = "RotateTool";
      // 
      // CameraTool
      // 
      this.CameraTool.ImageIndex = 5;
      this.CameraTool.Name = "CameraTool";
      // 
      // theImageList
      // 
      this.theImageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("theImageList.ImageStream")));
      this.theImageList.TransparentColor = System.Drawing.Color.White;
      this.theImageList.Images.SetKeyName(0, "");
      this.theImageList.Images.SetKeyName(1, "copy.png");
      this.theImageList.Images.SetKeyName(2, "");
      this.theImageList.Images.SetKeyName(3, "");
      this.theImageList.Images.SetKeyName(4, "");
      this.theImageList.Images.SetKeyName(5, "");
      this.theImageList.Images.SetKeyName(6, "");
      // 
      // TheStatusStrip
      // 
      this.TheStatusStrip.Dock = System.Windows.Forms.DockStyle.None;
      this.TheStatusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.StatusLabel,
            this.LatitudeLabel,
            this.LongitudeLabel,
            this.AltitudeLabel});
      this.TheStatusStrip.Location = new System.Drawing.Point(0, 0);
      this.TheStatusStrip.Name = "TheStatusStrip";
      this.TheStatusStrip.RightToLeft = System.Windows.Forms.RightToLeft.No;
      this.TheStatusStrip.Size = new System.Drawing.Size(988, 22);
      this.TheStatusStrip.TabIndex = 3;
      this.TheStatusStrip.Text = "TheStatusStrip";
      // 
      // StatusLabel
      // 
      this.StatusLabel.Name = "StatusLabel";
      this.StatusLabel.Size = new System.Drawing.Size(634, 17);
      this.StatusLabel.Spring = true;
      // 
      // LatitudeLabel
      // 
      this.LatitudeLabel.AutoSize = false;
      this.LatitudeLabel.BorderSides = ((System.Windows.Forms.ToolStripStatusLabelBorderSides)((((System.Windows.Forms.ToolStripStatusLabelBorderSides.Left | System.Windows.Forms.ToolStripStatusLabelBorderSides.Top)
                  | System.Windows.Forms.ToolStripStatusLabelBorderSides.Right)
                  | System.Windows.Forms.ToolStripStatusLabelBorderSides.Bottom)));
      this.LatitudeLabel.BorderStyle = System.Windows.Forms.Border3DStyle.SunkenOuter;
      this.LatitudeLabel.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
      this.LatitudeLabel.Name = "LatitudeLabel";
      this.LatitudeLabel.Size = new System.Drawing.Size(113, 17);
      this.LatitudeLabel.Text = "Latitude";
      this.LatitudeLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      this.LatitudeLabel.Paint += new System.Windows.Forms.PaintEventHandler(this.OnLatitudeLabelPaint);
      // 
      // LongitudeLabel
      // 
      this.LongitudeLabel.AutoSize = false;
      this.LongitudeLabel.BorderSides = ((System.Windows.Forms.ToolStripStatusLabelBorderSides)((((System.Windows.Forms.ToolStripStatusLabelBorderSides.Left | System.Windows.Forms.ToolStripStatusLabelBorderSides.Top)
                  | System.Windows.Forms.ToolStripStatusLabelBorderSides.Right)
                  | System.Windows.Forms.ToolStripStatusLabelBorderSides.Bottom)));
      this.LongitudeLabel.BorderStyle = System.Windows.Forms.Border3DStyle.SunkenOuter;
      this.LongitudeLabel.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
      this.LongitudeLabel.Name = "LongitudeLabel";
      this.LongitudeLabel.Size = new System.Drawing.Size(113, 17);
      this.LongitudeLabel.Text = "Longitude";
      this.LongitudeLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      this.LongitudeLabel.Paint += new System.Windows.Forms.PaintEventHandler(this.OnLongitudeLabelPaint);
      // 
      // AltitudeLabel
      // 
      this.AltitudeLabel.AutoSize = false;
      this.AltitudeLabel.BorderSides = ((System.Windows.Forms.ToolStripStatusLabelBorderSides)((((System.Windows.Forms.ToolStripStatusLabelBorderSides.Left | System.Windows.Forms.ToolStripStatusLabelBorderSides.Top)
                  | System.Windows.Forms.ToolStripStatusLabelBorderSides.Right)
                  | System.Windows.Forms.ToolStripStatusLabelBorderSides.Bottom)));
      this.AltitudeLabel.BorderStyle = System.Windows.Forms.Border3DStyle.SunkenOuter;
      this.AltitudeLabel.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
      this.AltitudeLabel.Name = "AltitudeLabel";
      this.AltitudeLabel.Size = new System.Drawing.Size(113, 17);
      this.AltitudeLabel.Text = "Altitude";
      this.AltitudeLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      this.AltitudeLabel.Paint += new System.Windows.Forms.PaintEventHandler(this.OnAltitudeLabelPaint);
      // 
      // FileOpenTerrainDialog
      // 
      this.FileOpenTerrainDialog.DefaultExt = "tdf";
      this.FileOpenTerrainDialog.Filter = "MapLink Terrain Database Files|*.tdf|All files|*.*";
      this.FileOpenTerrainDialog.ReadOnlyChecked = true;
      this.FileOpenTerrainDialog.RestoreDirectory = true;
      this.FileOpenTerrainDialog.Title = "Open a MapLink Terrain Database";
      // 
      // toolStripContainer1
      // 
      // 
      // toolStripContainer1.BottomToolStripPanel
      // 
      this.toolStripContainer1.BottomToolStripPanel.Controls.Add(this.TheStatusStrip);
      // 
      // toolStripContainer1.ContentPanel
      // 
      this.toolStripContainer1.ContentPanel.Controls.Add(this.ViewerPanel);
      this.toolStripContainer1.ContentPanel.Size = new System.Drawing.Size(988, 554);
      this.toolStripContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
      this.toolStripContainer1.LeftToolStripPanelVisible = false;
      this.toolStripContainer1.Location = new System.Drawing.Point(0, 36);
      this.toolStripContainer1.Name = "toolStripContainer1";
      this.toolStripContainer1.RightToolStripPanelVisible = false;
      this.toolStripContainer1.Size = new System.Drawing.Size(988, 576);
      this.toolStripContainer1.TabIndex = 4;
      this.toolStripContainer1.Text = "toolStripContainer1";
      this.toolStripContainer1.TopToolStripPanelVisible = false;
      // 
      // ViewerPanel
      // 
      this.ViewerPanel.Dock = System.Windows.Forms.DockStyle.Fill;
      this.ViewerPanel.Location = new System.Drawing.Point(0, 0);
      this.ViewerPanel.Name = "ViewerPanel";
      this.ViewerPanel.Size = new System.Drawing.Size(988, 554);
      this.ViewerPanel.TabIndex = 2;
      this.ViewerPanel.Paint += new System.Windows.Forms.PaintEventHandler(this.OnPaint);
      this.ViewerPanel.MouseMove += new System.Windows.Forms.MouseEventHandler(this.OnMouseMove);
      this.ViewerPanel.MouseDown += new System.Windows.Forms.MouseEventHandler(this.OnMouseDown);
      this.ViewerPanel.Resize += new System.EventHandler(this.OnReSize);
      this.ViewerPanel.MouseUp += new System.Windows.Forms.MouseEventHandler(this.OnMouseUp);
      // 
      // MapViewerForm
      // 
      this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
      this.ClientSize = new System.Drawing.Size(988, 612);
      this.Controls.Add(this.toolStripContainer1);
      this.Controls.Add(this.TheToolBar);
      this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
      this.Menu = this.TheMenu;
      this.Name = "MapViewerForm";
      this.Text = "C# 3D .NET Sample";
      this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.OnClosed);
      this.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.OnKeyPress);
      this.KeyUp += new System.Windows.Forms.KeyEventHandler(this.OnKeyUp);
      this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.OnKeyDown);
      this.TheStatusStrip.ResumeLayout(false);
      this.TheStatusStrip.PerformLayout();
      this.toolStripContainer1.BottomToolStripPanel.ResumeLayout(false);
      this.toolStripContainer1.BottomToolStripPanel.PerformLayout();
      this.toolStripContainer1.ContentPanel.ResumeLayout(false);
      this.toolStripContainer1.ResumeLayout(false);
      this.toolStripContainer1.PerformLayout();
      this.ResumeLayout(false);
      this.PerformLayout();

    }
    #endregion

    private System.Windows.Forms.OpenFileDialog FileOpenMapDialog;
    private System.Windows.Forms.MainMenu TheMenu;
    private System.Windows.Forms.MenuItem FileMenu;
    private System.Windows.Forms.MenuItem ToolsMenu;
    private System.Windows.Forms.MenuItem HelpMenu;
    private System.Windows.Forms.MenuItem FileOpenMenu;
    private System.Windows.Forms.MenuItem FileExitMenu;
    private System.Windows.Forms.MenuItem ToolsReset;
    private System.Windows.Forms.MenuItem ToolsZoom;
    private System.Windows.Forms.MenuItem ToolsFly;
    private System.Windows.Forms.MenuItem ToolsRotate;
    private System.Windows.Forms.MenuItem ToolsCamera;
    private System.Windows.Forms.MenuItem HelpAbout;
    private System.Windows.Forms.ToolBar TheToolBar;
    private System.Windows.Forms.ToolBarButton FileOpenButton;
    private System.Windows.Forms.ToolBarButton EditCopyButton;
    private System.Windows.Forms.ToolBarButton ResetButton;
    private System.Windows.Forms.ToolBarButton ZoomTool;
    private System.Windows.Forms.ToolBarButton FlyTool;
    private System.Windows.Forms.ToolBarButton RotateTool;
    private System.Windows.Forms.ToolBarButton CameraTool;
    private System.Windows.Forms.ImageList theImageList;
    private CSharp3DSample.MapViewerPanel ViewerPanel;
    private System.Windows.Forms.StatusStrip TheStatusStrip;
    private System.Windows.Forms.ToolStripStatusLabel LatitudeLabel;
    private System.Windows.Forms.ToolStripStatusLabel LongitudeLabel;
    private System.Windows.Forms.ToolStripStatusLabel AltitudeLabel;
    private System.Windows.Forms.ToolStripStatusLabel StatusLabel;
    private System.Windows.Forms.MenuItem FileOpenTerrainMenu;
    private System.Windows.Forms.OpenFileDialog FileOpenTerrainDialog;
    private System.Windows.Forms.MenuItem menuItem1;
    private System.Windows.Forms.MenuItem PrimitiveCreation_Polygon;
    private System.Windows.Forms.MenuItem PrimitiveCreation_ExtrudedPolygon;
    private System.Windows.Forms.MenuItem menuItem2;
    private System.Windows.Forms.MenuItem EditCopyMenu;
    private System.Windows.Forms.ToolStripContainer toolStripContainer1;
  }
}
