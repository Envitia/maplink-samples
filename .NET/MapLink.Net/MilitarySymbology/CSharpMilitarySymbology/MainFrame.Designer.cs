namespace CSharpMapViewer
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
            System.Windows.Forms.ToolStripSeparator dumbSeparator1;
            System.Windows.Forms.ToolStripSeparator MenuSeparator1;
            System.Windows.Forms.ToolStripSeparator MenuSeparator2;
            System.Windows.Forms.ToolStripSeparator MenuSeparator3;
            System.Windows.Forms.ToolStripSeparator dumbSeparator2;
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MapViewerForm));
            this.FileOpenDialog = new System.Windows.Forms.OpenFileDialog();
            this.TheMenu = new System.Windows.Forms.MenuStrip();
            this.FileMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.FileOpenMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.FilePrintMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.FileExitMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.ToolsMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.ToolsZoomIn = new System.Windows.Forms.ToolStripMenuItem();
            this.ToolsZoomOut = new System.Windows.Forms.ToolStripMenuItem();
            this.ToolsReset = new System.Windows.Forms.ToolStripMenuItem();
            this.ToolsZoom = new System.Windows.Forms.ToolStripMenuItem();
            this.ToolsPan = new System.Windows.Forms.ToolStripMenuItem();
            this.ToolsGrab = new System.Windows.Forms.ToolStripMenuItem();
            this.ToolsZoomPan = new System.Windows.Forms.ToolStripMenuItem();
            this.ToolsRotate = new System.Windows.Forms.ToolStripMenuItem();
            this.ToolsSymbolPlot = new System.Windows.Forms.ToolStripMenuItem();
            this.HelpMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.HelpAbout = new System.Windows.Forms.ToolStripMenuItem();
            this.TheToolBar = new System.Windows.Forms.ToolStrip();
            this.FileOpenButton = new System.Windows.Forms.ToolStripButton();
            this.FilePrintButton = new System.Windows.Forms.ToolStripButton();
            this.ZoomIn = new System.Windows.Forms.ToolStripButton();
            this.ZoomOut = new System.Windows.Forms.ToolStripButton();
            this.ResetButton = new System.Windows.Forms.ToolStripButton();
            this.ZoomTool = new System.Windows.Forms.ToolStripButton();
            this.PanTool = new System.Windows.Forms.ToolStripButton();
            this.GrabTool = new System.Windows.Forms.ToolStripButton();
            this.ZoomPanTool = new System.Windows.Forms.ToolStripButton();
            this.RotateTool = new System.Windows.Forms.ToolStripButton();
            this.theImageList = new System.Windows.Forms.ImageList(this.components);
            this.ViewerPanel = new CSharpMapViewer.MapViewerPanel();
            dumbSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            MenuSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            MenuSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            MenuSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            dumbSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.SuspendLayout();
            // 
            // dumbSeparator1
            // 
            dumbSeparator1.Name = "dumbSeparator1";
            dumbSeparator1.Size = new System.Drawing.Size(6, 25);
            // 
            // MenuSeparator1
            // 
            MenuSeparator1.Name = "MenuSeparator1";
            MenuSeparator1.Size = new System.Drawing.Size(6, 25);
            // 
            // MenuSeparator2
            // 
            MenuSeparator2.Name = "MenuSeparator2";
            MenuSeparator2.Size = new System.Drawing.Size(6, 25);
            // 
            // MenuSeparator3
            // 
            MenuSeparator3.Name = "MenuSeparator3";
            MenuSeparator3.Size = new System.Drawing.Size(6, 25);
            // 
            // dumbSeparator2
            // 
            dumbSeparator2.Name = "dumbSeparator2";
            dumbSeparator2.Size = new System.Drawing.Size(6, 25);
            // 
            // FileOpenDialog
            // 
            this.FileOpenDialog.DefaultExt = "map";
            this.FileOpenDialog.Filter = "MapLink Maps|*.map;*.mpc|All files|*.*";
            this.FileOpenDialog.ReadOnlyChecked = true;
            this.FileOpenDialog.Title = "Open a MapLink Map";
            // 
            // TheMenu
            // 
            this.TheMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.FileMenu,
            this.ToolsMenu,
            this.HelpMenu});
            // 
            // FileMenu
            // 
            this.FileMenu.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.FileOpenMenu,
            MenuSeparator1,
            this.FilePrintMenu,
            MenuSeparator2,
            this.FileExitMenu});
            this.FileMenu.Text = "File";
            // 
            // FileOpenMenu
            // 
            this.FileOpenMenu.Text = "Open";
            this.FileOpenMenu.Click += new System.EventHandler(this.FileOpenMenu_Click);
            // 
            // FilePrintMenu
            // 
            this.FilePrintMenu.Text = "Print";
            this.FilePrintMenu.Click += new System.EventHandler(this.FilePrintMenu_Click);
            // 
            // FileExitMenu
            // 
            this.FileExitMenu.Text = "Exit";
            this.FileExitMenu.Click += new System.EventHandler(this.FileExitMenu_Click);
            // 
            // ToolsMenu
            // 
            this.ToolsMenu.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ToolsZoomIn,
            this.ToolsZoomOut,
            this.ToolsReset,
            MenuSeparator3,
            this.ToolsZoom,
            this.ToolsPan,
            this.ToolsGrab,
            this.ToolsZoomPan,
            this.ToolsRotate,
            this.ToolsSymbolPlot});
            this.ToolsMenu.Text = "Tools";
            // 
            // ToolsZoomIn
            // 
            this.ToolsZoomIn.Text = "Zoom In";
            this.ToolsZoomIn.Click += new System.EventHandler(this.MenuNonImode_Click);
            // 
            // ToolsZoomOut
            // 
            this.ToolsZoomOut.Text = "Zoom Out";
            this.ToolsZoomOut.Click += new System.EventHandler(this.MenuNonImode_Click);
            // 
            // ToolsReset
            // 
            this.ToolsReset.Text = "Reset";
            this.ToolsReset.Click += new System.EventHandler(this.MenuNonImode_Click);
            // 
            // ToolsZoom
            // 
            this.ToolsZoom.Checked = true;
            this.ToolsZoom.CheckOnClick = true;
            this.ToolsZoom.Text = "Zoom";
            this.ToolsZoom.Click += new System.EventHandler(this.MenuImode_Click);
            // 
            // ToolsPan
            // 
            this.ToolsPan.CheckOnClick = true;
            this.ToolsPan.Text = "Pan";
            this.ToolsPan.Click += new System.EventHandler(this.MenuImode_Click);
            // 
            // ToolsGrab
            // 
            this.ToolsGrab.CheckOnClick = true;
            this.ToolsGrab.Text = "Grab";
            this.ToolsGrab.Click += new System.EventHandler(this.MenuImode_Click);
            // 
            // ToolsZoomPan
            // 
            this.ToolsZoomPan.CheckOnClick = true;
            this.ToolsZoomPan.Text = "Zoom / Pan";
            this.ToolsZoomPan.Click += new System.EventHandler(this.MenuImode_Click);
            // 
            // ToolsRotate
            // 
            this.ToolsRotate.CheckOnClick = true;
            this.ToolsRotate.Text = "Rotate";
            // 
            // ToolsSymbolPlot
            // 
            this.ToolsSymbolPlot.Name = "ToolsSymbolPlot";
            this.ToolsSymbolPlot.Text = "Symbol Plot";
            this.ToolsSymbolPlot.Click += new System.EventHandler(this.ToolsSymbolPlot_Click);
            // 
            // HelpMenu
            // 
            this.HelpMenu.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.HelpAbout});
            this.HelpMenu.Text = "Help";
            // 
            // HelpAbout
            // 
            this.HelpAbout.Text = "About";
            this.HelpAbout.Click += new System.EventHandler(this.HelpAboutMenu_Click);
            // 
            // TheToolBar
            // 
            this.TheToolBar.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.FileOpenButton,
            this.FilePrintButton,
            dumbSeparator1,
            this.ZoomIn,
            this.ZoomOut,
            this.ResetButton,
            dumbSeparator2,
            this.ZoomTool,
            this.PanTool,
            this.GrabTool,
            this.ZoomPanTool,
            this.RotateTool});
            this.TheToolBar.ImageList = this.theImageList;
            this.TheToolBar.Location = new System.Drawing.Point(0, 0);
            this.TheToolBar.Name = "TheToolBar";
            this.TheToolBar.Size = new System.Drawing.Size(738, 25);
            this.TheToolBar.TabIndex = 1;
            this.TheToolBar.ItemClicked += new System.Windows.Forms.ToolStripItemClickedEventHandler(this.TheToolBar_ButtonClick);
            // 
            // FileOpenButton
            // 
            this.FileOpenButton.ImageIndex = 0;
            this.FileOpenButton.Name = "FileOpenButton";
            // 
            // FilePrintButton
            // 
            this.FilePrintButton.ImageIndex = 1;
            this.FilePrintButton.Name = "FilePrintButton";
            // 
            // ZoomIn
            // 
            this.ZoomIn.ImageIndex = 2;
            this.ZoomIn.Name = "ZoomIn";
            // 
            // ZoomOut
            // 
            this.ZoomOut.ImageIndex = 3;
            this.ZoomOut.Name = "ZoomOut";
            // 
            // ResetButton
            // 
            this.ResetButton.ImageIndex = 4;
            this.ResetButton.Name = "ResetButton";
            // 
            // ZoomTool
            // 
            this.ZoomTool.ImageIndex = 5;
            this.ZoomTool.Name = "ZoomTool";
            this.ZoomTool.Checked = true;
            // 
            // PanTool
            // 
            this.PanTool.ImageIndex = 6;
            this.PanTool.Name = "PanTool";
            // 
            // GrabTool
            // 
            this.GrabTool.ImageIndex = 7;
            this.GrabTool.Name = "GrabTool";
            // 
            // ZoomPanTool
            // 
            this.ZoomPanTool.ImageIndex = 8;
            this.ZoomPanTool.Name = "ZoomPanTool";
            // 
            // RotateTool
            // 
            this.RotateTool.ImageIndex = 9;
            this.RotateTool.Name = "RotateTool";
            // 
            // theImageList
            // 
            this.theImageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("theImageList.ImageStream")));
            this.theImageList.TransparentColor = System.Drawing.Color.Silver;
            this.theImageList.Images.SetKeyName(0, "");
            this.theImageList.Images.SetKeyName(1, "");
            this.theImageList.Images.SetKeyName(2, "");
            this.theImageList.Images.SetKeyName(3, "");
            this.theImageList.Images.SetKeyName(4, "");
            this.theImageList.Images.SetKeyName(5, "");
            this.theImageList.Images.SetKeyName(6, "");
            this.theImageList.Images.SetKeyName(7, "");
            this.theImageList.Images.SetKeyName(8, "");
            this.theImageList.Images.SetKeyName(9, "rotate.bmp");
            // 
            // ViewerPanel
            // 
            this.ViewerPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ViewerPanel.Location = new System.Drawing.Point(0, 36);
            this.ViewerPanel.Name = "ViewerPanel";
            this.ViewerPanel.Size = new System.Drawing.Size(738, 191);
            this.ViewerPanel.TabIndex = 2;
            this.ViewerPanel.Paint += new System.Windows.Forms.PaintEventHandler(this.OnPaint);
            this.ViewerPanel.MouseDown += new System.Windows.Forms.MouseEventHandler(this.OnMouseDown);
            this.ViewerPanel.MouseMove += new System.Windows.Forms.MouseEventHandler(this.OnMouseMove);
            this.ViewerPanel.MouseUp += new System.Windows.Forms.MouseEventHandler(this.OnMouseUp);
            this.ViewerPanel.Resize += new System.EventHandler(this.OnReSize);
            // 
            // MapViewerForm
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
            this.ClientSize = new System.Drawing.Size(738, 227);
            this.Controls.Add(this.ViewerPanel);
            this.Controls.Add(this.TheToolBar);
            this.Controls.Add(this.TheMenu);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.TheMenu;
            this.Name = "MapViewerForm";
            this.Text = "C# .NET Sample";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.OnClosed);
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.OnKeyDown);
            this.KeyUp += new System.Windows.Forms.KeyEventHandler(this.OnKeyUp);
            this.ResumeLayout(false);
            this.PerformLayout();

    }
    #endregion

    private System.Windows.Forms.OpenFileDialog FileOpenDialog;
    private System.Windows.Forms.MenuStrip TheMenu;
    private System.Windows.Forms.ToolStripMenuItem FileMenu;
    private System.Windows.Forms.ToolStripMenuItem FileOpenMenu;
    private System.Windows.Forms.ToolStripMenuItem FilePrintMenu;
    private System.Windows.Forms.ToolStripMenuItem FileExitMenu;
    private System.Windows.Forms.ToolStripMenuItem ToolsMenu;
    private System.Windows.Forms.ToolStripMenuItem ToolsZoomIn;
    private System.Windows.Forms.ToolStripMenuItem ToolsZoomOut;
    private System.Windows.Forms.ToolStripMenuItem ToolsReset;
    private System.Windows.Forms.ToolStripMenuItem ToolsZoom;
    private System.Windows.Forms.ToolStripMenuItem ToolsPan;
    private System.Windows.Forms.ToolStripMenuItem ToolsZoomPan;
    private System.Windows.Forms.ToolStripMenuItem ToolsGrab;
    private System.Windows.Forms.ToolStripMenuItem HelpMenu;
    private System.Windows.Forms.ToolStripMenuItem HelpAbout;
    private System.Windows.Forms.ToolStrip TheToolBar;
    private System.Windows.Forms.ToolStripButton FileOpenButton;
    private System.Windows.Forms.ToolStripButton FilePrintButton;
    private System.Windows.Forms.ToolStripButton ZoomIn;
    private System.Windows.Forms.ToolStripButton ZoomOut;
    private System.Windows.Forms.ToolStripButton ResetButton;
    private System.Windows.Forms.ToolStripButton ZoomTool;
    private System.Windows.Forms.ToolStripButton PanTool;
    private System.Windows.Forms.ToolStripButton GrabTool;
    private System.Windows.Forms.ToolStripButton ZoomPanTool;
    private System.Windows.Forms.ToolStripButton RotateTool;
    private System.Windows.Forms.ImageList theImageList;
    private CSharpMapViewer.MapViewerPanel ViewerPanel;
    private System.Windows.Forms.ToolStripMenuItem ToolsRotate;
    private System.Windows.Forms.ToolStripMenuItem ToolsSymbolPlot;
  }
}
