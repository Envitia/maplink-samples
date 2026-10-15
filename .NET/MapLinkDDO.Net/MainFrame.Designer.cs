using System;
using System.Collections.Generic;
using System.Text;

namespace CSharpDDOSample
{
  partial class MapViewerForm
  {
    private System.ComponentModel.IContainer components = null;

    protected override void Dispose(bool disposing)
    {
      if (disposing)
      {
        if (components != null)
        {
          components.Dispose();
        }
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
      System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MapViewerForm));
      this.FileOpenDialog = new System.Windows.Forms.OpenFileDialog();
      this.TheMenu = new System.Windows.Forms.MainMenu(this.components);
      this.FileMenu = new System.Windows.Forms.MenuItem();
      this.FileOpenMenu = new System.Windows.Forms.MenuItem();
      this.MenuSeparator1 = new System.Windows.Forms.MenuItem();
      this.FilePrintMenu = new System.Windows.Forms.MenuItem();
      this.MenuSeparator2 = new System.Windows.Forms.MenuItem();
      this.FileExitMenu = new System.Windows.Forms.MenuItem();
      this.EditMenu = new System.Windows.Forms.MenuItem();
      this.EditCopyMenu = new System.Windows.Forms.MenuItem();
      this.ToolsMenu = new System.Windows.Forms.MenuItem();
      this.ToolsZoomIn = new System.Windows.Forms.MenuItem();
      this.ToolsZoomOut = new System.Windows.Forms.MenuItem();
      this.ToolsReset = new System.Windows.Forms.MenuItem();
      this.MenuSeparator3 = new System.Windows.Forms.MenuItem();
      this.ToolsZoom = new System.Windows.Forms.MenuItem();
      this.ToolsPan = new System.Windows.Forms.MenuItem();
      this.ToolsGrab = new System.Windows.Forms.MenuItem();
      this.MenuSeparator4 = new System.Windows.Forms.MenuItem();
      this.ToolsSavedViewsMenu = new System.Windows.Forms.MenuItem();
      this.ToolsGoto1Menu = new System.Windows.Forms.MenuItem();
      this.ToolsGoto2Menu = new System.Windows.Forms.MenuItem();
      this.ToolsGoto3Menu = new System.Windows.Forms.MenuItem();
      this.MenuSeparator5 = new System.Windows.Forms.MenuItem();
      this.ToolsSet1Menu = new System.Windows.Forms.MenuItem();
      this.ToolsSet2Menu = new System.Windows.Forms.MenuItem();
      this.ToolsSet3Menu = new System.Windows.Forms.MenuItem();
      this.MenuSeparator6 = new System.Windows.Forms.MenuItem();
      this.ToolsViewsResetMenu = new System.Windows.Forms.MenuItem();
      this.ToolsViewStackMenu = new System.Windows.Forms.MenuItem();
      this.ToolsPreviousViewMenu = new System.Windows.Forms.MenuItem();
      this.ToolsNextViewMenu = new System.Windows.Forms.MenuItem();
      this.HelpMenu = new System.Windows.Forms.MenuItem();
      this.HelpAbout = new System.Windows.Forms.MenuItem();
      this.TheToolBar = new System.Windows.Forms.ToolBar();
      this.dumbSeparator = new System.Windows.Forms.ToolBarButton();
      this.FileOpenButton = new System.Windows.Forms.ToolBarButton();
      this.FilePrintButton = new System.Windows.Forms.ToolBarButton();
      this.CopyButton = new System.Windows.Forms.ToolBarButton();
      this.ZoomIn = new System.Windows.Forms.ToolBarButton();
      this.ZoomOut = new System.Windows.Forms.ToolBarButton();
      this.ResetButton = new System.Windows.Forms.ToolBarButton();
      this.ZoomTool = new System.Windows.Forms.ToolBarButton();
      this.PanTool = new System.Windows.Forms.ToolBarButton();
      this.GrabTool = new System.Windows.Forms.ToolBarButton();
      this.previousView = new System.Windows.Forms.ToolBarButton();
      this.nextView = new System.Windows.Forms.ToolBarButton();
      this.savedView1 = new System.Windows.Forms.ToolBarButton();
      this.savedView2 = new System.Windows.Forms.ToolBarButton();
      this.savedView3 = new System.Windows.Forms.ToolBarButton();
      this.theImageList = new System.Windows.Forms.ImageList(this.components);
      this.ViewerPanel = new CSharpDDOSample.MapViewerPanel();
      this.timer1 = new System.Windows.Forms.Timer(this.components);
      this.SuspendLayout();
      // 
      // FileOpenDialog
      // 
      this.FileOpenDialog.DefaultExt = "map";
      this.FileOpenDialog.Filter = "MapLink Maps|*.map";
      this.FileOpenDialog.ReadOnlyChecked = true;
      this.FileOpenDialog.Title = "Open a MapLink Map";
      // 
      // TheMenu
      // 
      this.TheMenu.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.FileMenu,
            this.EditMenu,
            this.ToolsMenu,
            this.HelpMenu});
      // 
      // FileMenu
      // 
      this.FileMenu.Index = 0;
      this.FileMenu.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.FileOpenMenu,
            this.MenuSeparator1,
            this.FilePrintMenu,
            this.MenuSeparator2,
            this.FileExitMenu});
      this.FileMenu.Text = "File";
      // 
      // FileOpenMenu
      // 
      this.FileOpenMenu.Index = 0;
      this.FileOpenMenu.Text = "Open";
      this.FileOpenMenu.Click += new System.EventHandler(this.FileOpenMenu_Click);
      // 
      // MenuSeparator1
      // 
      this.MenuSeparator1.Index = 1;
      this.MenuSeparator1.Text = "-";
      // 
      // FilePrintMenu
      // 
      this.FilePrintMenu.Index = 2;
      this.FilePrintMenu.Text = "Print";
      this.FilePrintMenu.Click += new System.EventHandler(this.FilePrintMenu_Click);
      // 
      // MenuSeparator2
      // 
      this.MenuSeparator2.Index = 3;
      this.MenuSeparator2.Text = "-";
      // 
      // FileExitMenu
      // 
      this.FileExitMenu.Index = 4;
      this.FileExitMenu.Text = "Exit";
      this.FileExitMenu.Click += new System.EventHandler(this.FileExitMenu_Click);
      // 
      // EditMenu
      // 
      this.EditMenu.Index = 1;
      this.EditMenu.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.EditCopyMenu});
      this.EditMenu.Text = "Edit";
      // 
      // EditCopyMenu
      // 
      this.EditCopyMenu.Index = 0;
      this.EditCopyMenu.Text = "Copy";
      this.EditCopyMenu.Click += new System.EventHandler(this.EditCopy_Click);
      // 
      // ToolsMenu
      // 
      this.ToolsMenu.Index = 2;
      this.ToolsMenu.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.ToolsZoomIn,
            this.ToolsZoomOut,
            this.ToolsReset,
            this.MenuSeparator3,
            this.ToolsZoom,
            this.ToolsPan,
            this.ToolsGrab,
            this.MenuSeparator4,
            this.ToolsSavedViewsMenu,
            this.ToolsViewStackMenu});
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
      // MenuSeparator3
      // 
      this.MenuSeparator3.Index = 3;
      this.MenuSeparator3.Text = "-";
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
      // MenuSeparator4
      // 
      this.MenuSeparator4.Index = 7;
      this.MenuSeparator4.Text = "-";
      // 
      // ToolsSavedViewsMenu
      // 
      this.ToolsSavedViewsMenu.Index = 8;
      this.ToolsSavedViewsMenu.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.ToolsGoto1Menu,
            this.ToolsGoto2Menu,
            this.ToolsGoto3Menu,
            this.MenuSeparator5,
            this.ToolsSet1Menu,
            this.ToolsSet2Menu,
            this.ToolsSet3Menu,
            this.MenuSeparator6,
            this.ToolsViewsResetMenu});
      this.ToolsSavedViewsMenu.Text = "Saved Views";
      // 
      // ToolsGoto1Menu
      // 
      this.ToolsGoto1Menu.Index = 0;
      this.ToolsGoto1Menu.Text = "Goto 1";
      this.ToolsGoto1Menu.Click += new System.EventHandler(this.OnSavedViewsMenuClick);
      // 
      // ToolsGoto2Menu
      // 
      this.ToolsGoto2Menu.Index = 1;
      this.ToolsGoto2Menu.Text = "Goto 2";
      this.ToolsGoto2Menu.Click += new System.EventHandler(this.OnSavedViewsMenuClick);
      // 
      // ToolsGoto3Menu
      // 
      this.ToolsGoto3Menu.Index = 2;
      this.ToolsGoto3Menu.Text = "Goto 3";
      this.ToolsGoto3Menu.Click += new System.EventHandler(this.OnSavedViewsMenuClick);
      // 
      // MenuSeparator5
      // 
      this.MenuSeparator5.Index = 3;
      this.MenuSeparator5.Text = "-";
      // 
      // ToolsSet1Menu
      // 
      this.ToolsSet1Menu.Index = 4;
      this.ToolsSet1Menu.Text = "Set 1";
      this.ToolsSet1Menu.Click += new System.EventHandler(this.OnSavedViewsMenuClick);
      // 
      // ToolsSet2Menu
      // 
      this.ToolsSet2Menu.Index = 5;
      this.ToolsSet2Menu.Text = "Set 2";
      this.ToolsSet2Menu.Click += new System.EventHandler(this.OnSavedViewsMenuClick);
      // 
      // ToolsSet3Menu
      // 
      this.ToolsSet3Menu.Index = 6;
      this.ToolsSet3Menu.Text = "Set 3";
      this.ToolsSet3Menu.Click += new System.EventHandler(this.OnSavedViewsMenuClick);
      // 
      // MenuSeparator6
      // 
      this.MenuSeparator6.Index = 7;
      this.MenuSeparator6.Text = "-";
      // 
      // ToolsViewsResetMenu
      // 
      this.ToolsViewsResetMenu.Index = 8;
      this.ToolsViewsResetMenu.Text = "Reset";
      this.ToolsViewsResetMenu.Click += new System.EventHandler(this.OnSavedViewsMenuClick);
      // 
      // ToolsViewStackMenu
      // 
      this.ToolsViewStackMenu.Index = 9;
      this.ToolsViewStackMenu.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.ToolsPreviousViewMenu,
            this.ToolsNextViewMenu});
      this.ToolsViewStackMenu.Text = "View Stack";
      // 
      // ToolsPreviousViewMenu
      // 
      this.ToolsPreviousViewMenu.Index = 0;
      this.ToolsPreviousViewMenu.Text = "Previous View";
      this.ToolsPreviousViewMenu.Click += new System.EventHandler(this.OnViewStackMenuClick);
      // 
      // ToolsNextViewMenu
      // 
      this.ToolsNextViewMenu.Index = 1;
      this.ToolsNextViewMenu.Text = "Next View";
      this.ToolsNextViewMenu.Click += new System.EventHandler(this.OnViewStackMenuClick);
      // 
      // HelpMenu
      // 
      this.HelpMenu.Index = 3;
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
            this.dumbSeparator,
            this.FileOpenButton,
            this.FilePrintButton,
            this.dumbSeparator,
            this.CopyButton,
            this.dumbSeparator,
            this.ZoomIn,
            this.ZoomOut,
            this.ResetButton,
            this.dumbSeparator,
            this.ZoomTool,
            this.PanTool,
            this.GrabTool,
            this.dumbSeparator,
            this.previousView,
            this.nextView,
            this.dumbSeparator,
            this.savedView1,
            this.savedView2,
            this.savedView3});
      this.TheToolBar.ButtonSize = new System.Drawing.Size(24, 24);
      this.TheToolBar.DropDownArrows = true;
      this.TheToolBar.ImageList = this.theImageList;
      this.TheToolBar.Location = new System.Drawing.Point(0, 0);
      this.TheToolBar.Name = "TheToolBar";
      this.TheToolBar.ShowToolTips = true;
      this.TheToolBar.Size = new System.Drawing.Size(592, 36);
      this.TheToolBar.TabIndex = 1;
      this.TheToolBar.ButtonClick += new System.Windows.Forms.ToolBarButtonClickEventHandler(this.TheToolBar_ButtonClick);
      // 
      // dumbSeparator
      // 
      this.dumbSeparator.Name = "dumbSeparator";
      this.dumbSeparator.Style = System.Windows.Forms.ToolBarButtonStyle.Separator;
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
      // CopyButton
      // 
      this.CopyButton.ImageIndex = 2;
      this.CopyButton.Name = "CopyButton";
      // 
      // ZoomIn
      // 
      this.ZoomIn.ImageIndex = 3;
      this.ZoomIn.Name = "ZoomIn";
      // 
      // ZoomOut
      // 
      this.ZoomOut.ImageIndex = 4;
      this.ZoomOut.Name = "ZoomOut";
      // 
      // ResetButton
      // 
      this.ResetButton.ImageIndex = 5;
      this.ResetButton.Name = "ResetButton";
      // 
      // ZoomTool
      // 
      this.ZoomTool.ImageIndex = 6;
      this.ZoomTool.Name = "ZoomTool";
      this.ZoomTool.Pushed = true;
      // 
      // PanTool
      // 
      this.PanTool.ImageIndex = 7;
      this.PanTool.Name = "PanTool";
      // 
      // GrabTool
      // 
      this.GrabTool.ImageIndex = 8;
      this.GrabTool.Name = "GrabTool";
      // 
      // previousView
      // 
      this.previousView.ImageIndex = 9;
      this.previousView.Name = "previousView";
      // 
      // nextView
      // 
      this.nextView.ImageIndex = 10;
      this.nextView.Name = "nextView";
      // 
      // savedView1
      // 
      this.savedView1.ImageIndex = 11;
      this.savedView1.Name = "savedView1";
      // 
      // savedView2
      // 
      this.savedView2.ImageIndex = 12;
      this.savedView2.Name = "savedView2";
      // 
      // savedView3
      // 
      this.savedView3.ImageIndex = 13;
      this.savedView3.Name = "savedView3";
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
      this.theImageList.Images.SetKeyName(9, "");
      this.theImageList.Images.SetKeyName(10, "");
      this.theImageList.Images.SetKeyName(11, "");
      this.theImageList.Images.SetKeyName(12, "");
      this.theImageList.Images.SetKeyName(13, "");
      // 
      // ViewerPanel
      // 
      this.ViewerPanel.Dock = System.Windows.Forms.DockStyle.Fill;
      this.ViewerPanel.Location = new System.Drawing.Point(0, 36);
      this.ViewerPanel.Name = "ViewerPanel";
      this.ViewerPanel.Size = new System.Drawing.Size(592, 373);
      this.ViewerPanel.TabIndex = 2;
      this.ViewerPanel.MouseDown += new System.Windows.Forms.MouseEventHandler(this.OnMouseDown);
      this.ViewerPanel.MouseMove += new System.Windows.Forms.MouseEventHandler(this.OnMouseMove);
      this.ViewerPanel.Resize += new System.EventHandler(this.OnReSize);
      this.ViewerPanel.Paint += new System.Windows.Forms.PaintEventHandler(this.OnPaint);
      this.ViewerPanel.MouseUp += new System.Windows.Forms.MouseEventHandler(this.OnMouseUp);
      // 
      // timer1
      // 
      this.timer1.Interval = 75;
      this.timer1.Tick += new System.EventHandler(this.OnUpdate);
      // 
      // MapViewerForm
      // 
      this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
      this.ClientSize = new System.Drawing.Size(592, 409);
      this.Controls.Add(this.ViewerPanel);
      this.Controls.Add(this.TheToolBar);
      this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
      this.Menu = this.TheMenu;
      this.Name = "MapViewerForm";
      this.Text = "MapLink C# DDO .NET Sample";
      this.MouseWheel += new System.Windows.Forms.MouseEventHandler(this.OnMouseWheel);
      this.KeyUp += new System.Windows.Forms.KeyEventHandler(this.OnKeyUp);
      this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.OnKeyDown);
      this.ResumeLayout(false);
      this.PerformLayout();

    }
    #endregion

    private System.Windows.Forms.MainMenu TheMenu;
    private System.Windows.Forms.MenuItem FileMenu;
    private System.Windows.Forms.MenuItem ToolsMenu;
    private System.Windows.Forms.MenuItem HelpMenu;
    private System.Windows.Forms.MenuItem MenuSeparator1;
    private System.Windows.Forms.MenuItem MenuSeparator2;
    private System.Windows.Forms.MenuItem MenuSeparator3;
    private System.Windows.Forms.MenuItem FileOpenMenu;
    private System.Windows.Forms.MenuItem FilePrintMenu;
    private System.Windows.Forms.MenuItem FileExitMenu;
    private System.Windows.Forms.MenuItem ToolsZoomIn;
    private System.Windows.Forms.MenuItem ToolsZoomOut;
    private System.Windows.Forms.MenuItem ToolsReset;
    private System.Windows.Forms.MenuItem ToolsZoom;
    private System.Windows.Forms.MenuItem ToolsPan;
    private System.Windows.Forms.MenuItem ToolsGrab;
    private System.Windows.Forms.MenuItem HelpAbout;
    private System.Windows.Forms.ToolBar TheToolBar;
    private System.Windows.Forms.ToolBarButton FileOpenButton;
    private System.Windows.Forms.ToolBarButton FilePrintButton;
    private System.Windows.Forms.ToolBarButton dumbSeparator;
    private System.Windows.Forms.ToolBarButton ZoomIn;
    private System.Windows.Forms.ToolBarButton ZoomOut;
    private System.Windows.Forms.ToolBarButton ResetButton;
    private System.Windows.Forms.ToolBarButton ZoomTool;
    private System.Windows.Forms.ToolBarButton PanTool;
    private System.Windows.Forms.ToolBarButton GrabTool;
    private System.Windows.Forms.ToolBarButton CopyButton;
    private System.Windows.Forms.ToolBarButton previousView;
    private System.Windows.Forms.ToolBarButton nextView;
    private System.Windows.Forms.ToolBarButton savedView1;
    private System.Windows.Forms.ToolBarButton savedView2;
    private System.Windows.Forms.ToolBarButton savedView3;
    private CSharpDDOSample.MapViewerPanel ViewerPanel;
    private System.Windows.Forms.ImageList theImageList;
    private System.Windows.Forms.MenuItem EditMenu;
    private System.Windows.Forms.MenuItem EditCopyMenu;
    private System.Windows.Forms.MenuItem MenuSeparator4;
    private System.Windows.Forms.MenuItem ToolsSavedViewsMenu;
    private System.Windows.Forms.MenuItem ToolsGoto1Menu;
    private System.Windows.Forms.MenuItem ToolsGoto2Menu;
    private System.Windows.Forms.MenuItem ToolsGoto3Menu;
    private System.Windows.Forms.MenuItem MenuSeparator5;
    private System.Windows.Forms.MenuItem ToolsSet1Menu;
    private System.Windows.Forms.MenuItem ToolsSet2Menu;
    private System.Windows.Forms.MenuItem ToolsSet3Menu;
    private System.Windows.Forms.MenuItem ToolsViewsResetMenu;
    private System.Windows.Forms.MenuItem MenuSeparator6;
    private System.Windows.Forms.MenuItem ToolsViewStackMenu;
    private System.Windows.Forms.MenuItem ToolsPreviousViewMenu;
    private System.Windows.Forms.MenuItem ToolsNextViewMenu;
    private System.Windows.Forms.OpenFileDialog FileOpenDialog;
    private System.Windows.Forms.Timer timer1;
  }
}
