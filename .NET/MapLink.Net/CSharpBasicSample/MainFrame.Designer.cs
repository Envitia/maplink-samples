
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
            System.Windows.Forms.ToolBarButton dumbSeparator1;
            System.Windows.Forms.MenuItem MenuSeparator1;
            System.Windows.Forms.MenuItem MenuSeparator2;
            System.Windows.Forms.MenuItem MenuSeparator3;
            System.Windows.Forms.ToolBarButton dumbSeparator2;
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MapViewerForm));
            this.FileOpenDialog = new System.Windows.Forms.OpenFileDialog();
            this.TheMenu = new System.Windows.Forms.MainMenu(this.components);
            this.FileMenu = new System.Windows.Forms.MenuItem();
            this.FileOpenMenu = new System.Windows.Forms.MenuItem();
            this.FilePrintMenu = new System.Windows.Forms.MenuItem();
            this.FileExitMenu = new System.Windows.Forms.MenuItem();
            this.ToolsMenu = new System.Windows.Forms.MenuItem();
            this.ToolsZoomIn = new System.Windows.Forms.MenuItem();
            this.ToolsZoomOut = new System.Windows.Forms.MenuItem();
            this.ToolsReset = new System.Windows.Forms.MenuItem();
            this.ToolsZoom = new System.Windows.Forms.MenuItem();
            this.ToolsPan = new System.Windows.Forms.MenuItem();
            this.ToolsGrab = new System.Windows.Forms.MenuItem();
            this.ToolsZoomPan = new System.Windows.Forms.MenuItem();
            this.ToolsRotate = new System.Windows.Forms.MenuItem();
            this.HelpMenu = new System.Windows.Forms.MenuItem();
            this.HelpAbout = new System.Windows.Forms.MenuItem();
            this.TheToolBar = new System.Windows.Forms.ToolBar();
            this.FileOpenButton = new System.Windows.Forms.ToolBarButton();
            this.FilePrintButton = new System.Windows.Forms.ToolBarButton();
            this.ZoomIn = new System.Windows.Forms.ToolBarButton();
            this.ZoomOut = new System.Windows.Forms.ToolBarButton();
            this.ResetButton = new System.Windows.Forms.ToolBarButton();
            this.ZoomTool = new System.Windows.Forms.ToolBarButton();
            this.PanTool = new System.Windows.Forms.ToolBarButton();
            this.GrabTool = new System.Windows.Forms.ToolBarButton();
            this.ZoomPanTool = new System.Windows.Forms.ToolBarButton();
            this.RotateTool = new System.Windows.Forms.ToolBarButton();
            this.theImageList = new System.Windows.Forms.ImageList(this.components);
            this.ViewerPanel = new CSharpMapViewer.MapViewerPanel();
            dumbSeparator1 = new System.Windows.Forms.ToolBarButton();
            MenuSeparator1 = new System.Windows.Forms.MenuItem();
            MenuSeparator2 = new System.Windows.Forms.MenuItem();
            MenuSeparator3 = new System.Windows.Forms.MenuItem();
            dumbSeparator2 = new System.Windows.Forms.ToolBarButton();
            this.SuspendLayout();
            // 
            // dumbSeparator1
            // 
            dumbSeparator1.Name = "dumbSeparator1";
            dumbSeparator1.Style = System.Windows.Forms.ToolBarButtonStyle.Separator;
            // 
            // MenuSeparator1
            // 
            MenuSeparator1.Index = 1;
            MenuSeparator1.Text = "-";
            // 
            // MenuSeparator2
            // 
            MenuSeparator2.Index = 3;
            MenuSeparator2.Text = "-";
            // 
            // MenuSeparator3
            // 
            MenuSeparator3.Index = 3;
            MenuSeparator3.Text = "-";
            // 
            // dumbSeparator2
            // 
            dumbSeparator2.Name = "dumbSeparator2";
            dumbSeparator2.Style = System.Windows.Forms.ToolBarButtonStyle.Separator;
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
            this.TheMenu.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.FileMenu,
            this.ToolsMenu,
            this.HelpMenu});
            // 
            // FileMenu
            // 
            this.FileMenu.Index = 0;
            this.FileMenu.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.FileOpenMenu,
            MenuSeparator1,
            this.FilePrintMenu,
            MenuSeparator2,
            this.FileExitMenu});
            this.FileMenu.Text = "File";
            // 
            // FileOpenMenu
            // 
            this.FileOpenMenu.Index = 0;
            this.FileOpenMenu.Text = "Open";
            this.FileOpenMenu.Click += new System.EventHandler(this.FileOpenMenu_Click);
            // 
            // FilePrintMenu
            // 
            this.FilePrintMenu.Index = 2;
            this.FilePrintMenu.Text = "Print";
            this.FilePrintMenu.Click += new System.EventHandler(this.FilePrintMenu_Click);
            // 
            // FileExitMenu
            // 
            this.FileExitMenu.Index = 4;
            this.FileExitMenu.Text = "Exit";
            this.FileExitMenu.Click += new System.EventHandler(this.FileExitMenu_Click);
            // 
            // ToolsMenu
            // 
            this.ToolsMenu.Index = 1;
            this.ToolsMenu.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.ToolsZoomIn,
            this.ToolsZoomOut,
            this.ToolsReset,
            MenuSeparator3,
            this.ToolsZoom,
            this.ToolsPan,
            this.ToolsGrab,
            this.ToolsZoomPan,
            this.ToolsRotate});
            this.ToolsMenu.Text = "Tools";
            // 
            // ToolsZoomIn
            // 
            this.ToolsZoomIn.Index = 0;
            this.ToolsZoomIn.Text = "Zoom In";
            this.ToolsZoomIn.Click += new System.EventHandler(this.MenuNonImode_Click);
            // 
            // ToolsZoomOut
            // 
            this.ToolsZoomOut.Index = 1;
            this.ToolsZoomOut.Text = "Zoom Out";
            this.ToolsZoomOut.Click += new System.EventHandler(this.MenuNonImode_Click);
            // 
            // ToolsReset
            // 
            this.ToolsReset.Index = 2;
            this.ToolsReset.Text = "Reset";
            this.ToolsReset.Click += new System.EventHandler(this.MenuNonImode_Click);
            // 
            // ToolsZoom
            // 
            this.ToolsZoom.Checked = true;
            this.ToolsZoom.Index = 4;
            this.ToolsZoom.RadioCheck = true;
            this.ToolsZoom.Text = "Zoom";
            this.ToolsZoom.Click += new System.EventHandler(this.MenuImode_Click);
            // 
            // ToolsPan
            // 
            this.ToolsPan.Index = 5;
            this.ToolsPan.RadioCheck = true;
            this.ToolsPan.Text = "Pan";
            this.ToolsPan.Click += new System.EventHandler(this.MenuImode_Click);
            // 
            // ToolsGrab
            // 
            this.ToolsGrab.Index = 6;
            this.ToolsGrab.RadioCheck = true;
            this.ToolsGrab.Text = "Grab";
            this.ToolsGrab.Click += new System.EventHandler(this.MenuImode_Click);
            // 
            // ToolsZoomPan
            // 
            this.ToolsZoomPan.Index = 7;
            this.ToolsZoomPan.RadioCheck = true;
            this.ToolsZoomPan.Text = "Zoom / Pan";
            this.ToolsZoomPan.Click += new System.EventHandler(this.MenuImode_Click);
            // 
            // ToolsRotate
            // 
            this.ToolsRotate.Index = 8;
            this.ToolsRotate.RadioCheck = true;
            this.ToolsRotate.Text = "Rotate";
            // 
            // HelpMenu
            // 
            this.HelpMenu.Index = 2;
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
            this.TheToolBar.ButtonSize = new System.Drawing.Size(24, 24);
            this.TheToolBar.DropDownArrows = true;
            this.TheToolBar.ImageList = this.theImageList;
            this.TheToolBar.Location = new System.Drawing.Point(0, 0);
            this.TheToolBar.Name = "TheToolBar";
            this.TheToolBar.ShowToolTips = true;
            this.TheToolBar.Size = new System.Drawing.Size(738, 36);
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
            this.ZoomTool.Pushed = true;
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
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Menu = this.TheMenu;
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
    private System.Windows.Forms.MainMenu TheMenu;
    private System.Windows.Forms.MenuItem FileMenu;
    private System.Windows.Forms.MenuItem ToolsMenu;
    private System.Windows.Forms.MenuItem HelpMenu;
    private System.Windows.Forms.MenuItem FileOpenMenu;
    private System.Windows.Forms.MenuItem FilePrintMenu;
    private System.Windows.Forms.MenuItem FileExitMenu;
    private System.Windows.Forms.MenuItem ToolsZoomIn;
    private System.Windows.Forms.MenuItem ToolsZoomOut;
    private System.Windows.Forms.MenuItem ToolsReset;
    private System.Windows.Forms.MenuItem ToolsZoom;
    private System.Windows.Forms.MenuItem ToolsPan;
    private System.Windows.Forms.MenuItem ToolsZoomPan;
    private System.Windows.Forms.MenuItem ToolsGrab;
    private System.Windows.Forms.MenuItem HelpAbout;
    private System.Windows.Forms.ToolBar TheToolBar;
    private System.Windows.Forms.ToolBarButton FileOpenButton;
    private System.Windows.Forms.ToolBarButton FilePrintButton;
    private System.Windows.Forms.ToolBarButton ZoomIn;
    private System.Windows.Forms.ToolBarButton ZoomOut;
    private System.Windows.Forms.ToolBarButton ResetButton;
    private System.Windows.Forms.ToolBarButton ZoomTool;
    private System.Windows.Forms.ToolBarButton PanTool;
    private System.Windows.Forms.ToolBarButton ZoomPanTool;
    private System.Windows.Forms.ToolBarButton GrabTool;
    private System.Windows.Forms.ImageList theImageList;
    private CSharpMapViewer.MapViewerPanel ViewerPanel;
    private System.Windows.Forms.MenuItem ToolsRotate;
    private System.Windows.Forms.ToolBarButton RotateTool;
  }
}
