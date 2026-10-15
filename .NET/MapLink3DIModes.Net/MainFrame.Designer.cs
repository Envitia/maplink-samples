using System;
using System.Collections.Generic;
using System.Text;

namespace CSharp3DInteractionSample
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
      System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
      System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
      System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
      System.Windows.Forms.ToolStripSeparator toolStripSeparator5;
      System.Windows.Forms.ToolStripSeparator toolStripSeparator11;
      System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MapViewerForm));
      this.FileOpenDialog = new System.Windows.Forms.OpenFileDialog();
      this.TheStatusStrip = new System.Windows.Forms.StatusStrip();
      this.StatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
      this.LatitudeLabel = new System.Windows.Forms.ToolStripStatusLabel();
      this.LongitudeLabel = new System.Windows.Forms.ToolStripStatusLabel();
      this.AltitudeLabel = new System.Windows.Forms.ToolStripStatusLabel();
      this.TheToolStripContainer = new System.Windows.Forms.ToolStripContainer();
      this.ViewerPanel = new CSharp3DInteractionSample.MapViewerPanel();
      this.TheToolStrip = new System.Windows.Forms.ToolStrip();
      this.openToolStripButton = new System.Windows.Forms.ToolStripButton();
      this.ZoomInButton = new System.Windows.Forms.ToolStripButton();
      this.ZoomOutButton = new System.Windows.Forms.ToolStripButton();
      this.ResetButton = new System.Windows.Forms.ToolStripButton();
      this.TrackballEyepointButton = new System.Windows.Forms.ToolStripButton();
      this.TrackballWorldButton = new System.Windows.Forms.ToolStripButton();
      this.previousViewButton = new System.Windows.Forms.ToolStripButton();
      this.nextViewButton = new System.Windows.Forms.ToolStripButton();
      this.savedViewButton1 = new System.Windows.Forms.ToolStripButton();
      this.savedViewButton2 = new System.Windows.Forms.ToolStripButton();
      this.savedViewButton3 = new System.Windows.Forms.ToolStripButton();
      this.helpToolStripButton = new System.Windows.Forms.ToolStripButton();
      this.TheMenuStrip = new System.Windows.Forms.MenuStrip();
      this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.FileOpenMenu = new System.Windows.Forms.ToolStripMenuItem();
      this.FileOpenTerrainMenu = new System.Windows.Forms.ToolStripMenuItem();
      this.toolStripSeparator7 = new System.Windows.Forms.ToolStripSeparator();
      this.FileExit = new System.Windows.Forms.ToolStripMenuItem();
      this.toolsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.ZoomInMenu = new System.Windows.Forms.ToolStripMenuItem();
      this.ZoomOutMenu = new System.Windows.Forms.ToolStripMenuItem();
      this.ResetMenu = new System.Windows.Forms.ToolStripMenuItem();
      this.toolStripSeparator6 = new System.Windows.Forms.ToolStripSeparator();
      this.TrackballEyepointMenu = new System.Windows.Forms.ToolStripMenuItem();
      this.TrackballWorldMenu = new System.Windows.Forms.ToolStripMenuItem();
      this.toolStripSeparator8 = new System.Windows.Forms.ToolStripSeparator();
      this.savedViewsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.goto1ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.goto2ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.goto3ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.toolStripSeparator9 = new System.Windows.Forms.ToolStripSeparator();
      this.set1ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.set2ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.set3ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.toolStripSeparator10 = new System.Windows.Forms.ToolStripSeparator();
      this.resetToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
      this.viewStackToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.previousViewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.nextViewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.FileOpenTerrainDialog = new System.Windows.Forms.OpenFileDialog();
      toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
      toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
      toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
      toolStripSeparator5 = new System.Windows.Forms.ToolStripSeparator();
      toolStripSeparator11 = new System.Windows.Forms.ToolStripSeparator();
      this.TheStatusStrip.SuspendLayout();
      this.TheToolStripContainer.BottomToolStripPanel.SuspendLayout();
      this.TheToolStripContainer.ContentPanel.SuspendLayout();
      this.TheToolStripContainer.TopToolStripPanel.SuspendLayout();
      this.TheToolStripContainer.SuspendLayout();
      this.TheToolStrip.SuspendLayout();
      this.TheMenuStrip.SuspendLayout();
      this.SuspendLayout();
      // 
      // toolStripSeparator1
      // 
      toolStripSeparator1.Name = "toolStripSeparator1";
      toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
      // 
      // toolStripSeparator3
      // 
      toolStripSeparator3.Name = "toolStripSeparator3";
      toolStripSeparator3.Size = new System.Drawing.Size(6, 25);
      // 
      // toolStripSeparator4
      // 
      toolStripSeparator4.Name = "toolStripSeparator4";
      toolStripSeparator4.Size = new System.Drawing.Size(6, 25);
      // 
      // toolStripSeparator5
      // 
      toolStripSeparator5.Name = "toolStripSeparator5";
      toolStripSeparator5.Size = new System.Drawing.Size(6, 25);
      // 
      // toolStripSeparator11
      // 
      toolStripSeparator11.Name = "toolStripSeparator11";
      toolStripSeparator11.Size = new System.Drawing.Size(6, 25);
      // 
      // FileOpenDialog
      // 
      this.FileOpenDialog.DefaultExt = "map";
      this.FileOpenDialog.Filter = "MapLink Maps|*.map;*.mpc|All files|*.*";
      this.FileOpenDialog.ReadOnlyChecked = true;
      this.FileOpenDialog.Title = "Open a MapLink Map";
      // 
      // TheStatusStrip
      // 
      this.TheStatusStrip.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
      this.TheStatusStrip.Dock = System.Windows.Forms.DockStyle.None;
      this.TheStatusStrip.GripStyle = System.Windows.Forms.ToolStripGripStyle.Visible;
      this.TheStatusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.StatusLabel,
            this.LatitudeLabel,
            this.LongitudeLabel,
            this.AltitudeLabel});
      this.TheStatusStrip.Location = new System.Drawing.Point(0, 0);
      this.TheStatusStrip.Name = "TheStatusStrip";
      this.TheStatusStrip.RightToLeft = System.Windows.Forms.RightToLeft.No;
      this.TheStatusStrip.Size = new System.Drawing.Size(944, 22);
      this.TheStatusStrip.TabIndex = 4;
      this.TheStatusStrip.Text = "TheStatusStrip";
      // 
      // StatusLabel
      // 
      this.StatusLabel.Name = "StatusLabel";
      this.StatusLabel.Size = new System.Drawing.Size(590, 17);
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
      // TheToolStripContainer
      // 
      // 
      // TheToolStripContainer.BottomToolStripPanel
      // 
      this.TheToolStripContainer.BottomToolStripPanel.Controls.Add(this.TheStatusStrip);
      // 
      // TheToolStripContainer.ContentPanel
      // 
      this.TheToolStripContainer.ContentPanel.Controls.Add(this.ViewerPanel);
      this.TheToolStripContainer.ContentPanel.Size = new System.Drawing.Size(944, 705);
      this.TheToolStripContainer.Dock = System.Windows.Forms.DockStyle.Fill;
      this.TheToolStripContainer.Location = new System.Drawing.Point(0, 24);
      this.TheToolStripContainer.Name = "TheToolStripContainer";
      this.TheToolStripContainer.Size = new System.Drawing.Size(944, 752);
      this.TheToolStripContainer.TabIndex = 5;
      this.TheToolStripContainer.Text = "toolStripContainer1";
      // 
      // TheToolStripContainer.TopToolStripPanel
      // 
      this.TheToolStripContainer.TopToolStripPanel.Controls.Add(this.TheToolStrip);
      this.TheToolStripContainer.KeyUp += new System.Windows.Forms.KeyEventHandler(this.OnKeyUp);
      this.TheToolStripContainer.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.OnKeyPress);
      this.TheToolStripContainer.KeyDown += new System.Windows.Forms.KeyEventHandler(this.OnKeyDown);
      // 
      // ViewerPanel
      // 
      this.ViewerPanel.Dock = System.Windows.Forms.DockStyle.Fill;
      this.ViewerPanel.Location = new System.Drawing.Point(0, 0);
      this.ViewerPanel.Name = "ViewerPanel";
      this.ViewerPanel.Size = new System.Drawing.Size(944, 705);
      this.ViewerPanel.TabIndex = 2;
      this.ViewerPanel.Paint += new System.Windows.Forms.PaintEventHandler(this.OnPaint);
      this.ViewerPanel.MouseMove += new System.Windows.Forms.MouseEventHandler(this.OnMouseMove);
      this.ViewerPanel.MouseDown += new System.Windows.Forms.MouseEventHandler(this.OnMouseDown);
      this.ViewerPanel.Resize += new System.EventHandler(this.OnReSize);
      this.ViewerPanel.MouseUp += new System.Windows.Forms.MouseEventHandler(this.OnMouseUp);
      // 
      // TheToolStrip
      // 
      this.TheToolStrip.Dock = System.Windows.Forms.DockStyle.None;
      this.TheToolStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openToolStripButton,
            toolStripSeparator1,
            this.ZoomInButton,
            this.ZoomOutButton,
            this.ResetButton,
            toolStripSeparator3,
            this.TrackballEyepointButton,
            this.TrackballWorldButton,
            toolStripSeparator4,
            this.previousViewButton,
            this.nextViewButton,
            toolStripSeparator5,
            this.savedViewButton1,
            this.savedViewButton2,
            this.savedViewButton3,
            toolStripSeparator11,
            this.helpToolStripButton});
      this.TheToolStrip.Location = new System.Drawing.Point(3, 0);
      this.TheToolStrip.Name = "TheToolStrip";
      this.TheToolStrip.Size = new System.Drawing.Size(318, 25);
      this.TheToolStrip.TabIndex = 3;
      this.TheToolStrip.Text = "toolStrip1";
      // 
      // openToolStripButton
      // 
      this.openToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.openToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("openToolStripButton.Image")));
      this.openToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.openToolStripButton.Name = "openToolStripButton";
      this.openToolStripButton.Size = new System.Drawing.Size(23, 22);
      this.openToolStripButton.Text = "&Open";
      this.openToolStripButton.Click += new System.EventHandler(this.FileOpen_Click);
      // 
      // ZoomInButton
      // 
      this.ZoomInButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.ZoomInButton.Image = ((System.Drawing.Image)(resources.GetObject("ZoomInButton.Image")));
      this.ZoomInButton.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.ZoomInButton.Name = "ZoomInButton";
      this.ZoomInButton.Size = new System.Drawing.Size(23, 22);
      this.ZoomInButton.Text = "Zoom In";
      this.ZoomInButton.ToolTipText = "Zoom In";
      this.ZoomInButton.Click += new System.EventHandler(this.ZoomIn_Click);
      // 
      // ZoomOutButton
      // 
      this.ZoomOutButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.ZoomOutButton.Image = ((System.Drawing.Image)(resources.GetObject("ZoomOutButton.Image")));
      this.ZoomOutButton.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.ZoomOutButton.Name = "ZoomOutButton";
      this.ZoomOutButton.Size = new System.Drawing.Size(23, 22);
      this.ZoomOutButton.Text = "Zoom Out";
      this.ZoomOutButton.ToolTipText = "Zoom Out";
      this.ZoomOutButton.Click += new System.EventHandler(this.ZoomOut_Click);
      // 
      // ResetButton
      // 
      this.ResetButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.ResetButton.Image = ((System.Drawing.Image)(resources.GetObject("ResetButton.Image")));
      this.ResetButton.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.ResetButton.Name = "ResetButton";
      this.ResetButton.Size = new System.Drawing.Size(23, 22);
      this.ResetButton.Text = "Reset";
      this.ResetButton.ToolTipText = "Reset";
      this.ResetButton.Click += new System.EventHandler(this.Reset_Click);
      // 
      // TrackballEyepointButton
      // 
      this.TrackballEyepointButton.Checked = true;
      this.TrackballEyepointButton.CheckState = System.Windows.Forms.CheckState.Checked;
      this.TrackballEyepointButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.TrackballEyepointButton.Image = ((System.Drawing.Image)(resources.GetObject("TrackballEyepointButton.Image")));
      this.TrackballEyepointButton.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.TrackballEyepointButton.Name = "TrackballEyepointButton";
      this.TrackballEyepointButton.Size = new System.Drawing.Size(23, 22);
      this.TrackballEyepointButton.Text = "Trackball Eyepoint Interaction Mode";
      this.TrackballEyepointButton.Click += new System.EventHandler(this.StripImode_Click);
      // 
      // TrackballWorldButton
      // 
      this.TrackballWorldButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.TrackballWorldButton.Image = ((System.Drawing.Image)(resources.GetObject("TrackballWorldButton.Image")));
      this.TrackballWorldButton.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.TrackballWorldButton.Name = "TrackballWorldButton";
      this.TrackballWorldButton.Size = new System.Drawing.Size(23, 22);
      this.TrackballWorldButton.Text = "Trackball World Interaction Mode";
      this.TrackballWorldButton.Click += new System.EventHandler(this.StripImode_Click);
      // 
      // previousViewButton
      // 
      this.previousViewButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.previousViewButton.Image = ((System.Drawing.Image)(resources.GetObject("previousViewButton.Image")));
      this.previousViewButton.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.previousViewButton.Name = "previousViewButton";
      this.previousViewButton.Size = new System.Drawing.Size(23, 22);
      this.previousViewButton.Text = "Previous View";
      this.previousViewButton.ToolTipText = "Previous View";
      this.previousViewButton.Click += new System.EventHandler(this.GotoPreviewView_Click);
      // 
      // nextViewButton
      // 
      this.nextViewButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.nextViewButton.Image = ((System.Drawing.Image)(resources.GetObject("nextViewButton.Image")));
      this.nextViewButton.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.nextViewButton.Name = "nextViewButton";
      this.nextViewButton.Size = new System.Drawing.Size(23, 22);
      this.nextViewButton.ToolTipText = "Next View";
      this.nextViewButton.Click += new System.EventHandler(this.GotoNextView_Click);
      // 
      // savedViewButton1
      // 
      this.savedViewButton1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.savedViewButton1.Image = ((System.Drawing.Image)(resources.GetObject("savedViewButton1.Image")));
      this.savedViewButton1.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.savedViewButton1.Name = "savedViewButton1";
      this.savedViewButton1.Size = new System.Drawing.Size(23, 22);
      this.savedViewButton1.Tag = "1";
      this.savedViewButton1.Click += new System.EventHandler(this.GotoSavedView_Click);
      // 
      // savedViewButton2
      // 
      this.savedViewButton2.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.savedViewButton2.Image = ((System.Drawing.Image)(resources.GetObject("savedViewButton2.Image")));
      this.savedViewButton2.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.savedViewButton2.Name = "savedViewButton2";
      this.savedViewButton2.Size = new System.Drawing.Size(23, 22);
      this.savedViewButton2.Tag = "2";
      this.savedViewButton2.Click += new System.EventHandler(this.GotoSavedView_Click);
      // 
      // savedViewButton3
      // 
      this.savedViewButton3.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.savedViewButton3.Image = ((System.Drawing.Image)(resources.GetObject("savedViewButton3.Image")));
      this.savedViewButton3.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.savedViewButton3.Name = "savedViewButton3";
      this.savedViewButton3.Size = new System.Drawing.Size(23, 22);
      this.savedViewButton3.Tag = "3";
      this.savedViewButton3.Click += new System.EventHandler(this.GotoSavedView_Click);
      // 
      // helpToolStripButton
      // 
      this.helpToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.helpToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("helpToolStripButton.Image")));
      this.helpToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.helpToolStripButton.Name = "helpToolStripButton";
      this.helpToolStripButton.Size = new System.Drawing.Size(23, 22);
      this.helpToolStripButton.Text = "He&lp";
      this.helpToolStripButton.Click += new System.EventHandler(this.aboutToolStripMenuItem_Click);
      // 
      // TheMenuStrip
      // 
      this.TheMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.toolsToolStripMenuItem,
            this.helpToolStripMenuItem});
      this.TheMenuStrip.Location = new System.Drawing.Point(0, 0);
      this.TheMenuStrip.Name = "TheMenuStrip";
      this.TheMenuStrip.Size = new System.Drawing.Size(944, 24);
      this.TheMenuStrip.TabIndex = 6;
      this.TheMenuStrip.Text = "menuStrip1";
      // 
      // fileToolStripMenuItem
      // 
      this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.FileOpenMenu,
            this.FileOpenTerrainMenu,
            this.toolStripSeparator7,
            this.FileExit});
      this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
      this.fileToolStripMenuItem.Size = new System.Drawing.Size(35, 20);
      this.fileToolStripMenuItem.Text = "&File";
      // 
      // FileOpenMenu
      // 
      this.FileOpenMenu.Image = ((System.Drawing.Image)(resources.GetObject("FileOpenMenu.Image")));
      this.FileOpenMenu.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.FileOpenMenu.Name = "FileOpenMenu";
      this.FileOpenMenu.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.O)));
      this.FileOpenMenu.Size = new System.Drawing.Size(235, 22);
      this.FileOpenMenu.Text = "&Open";
      this.FileOpenMenu.Click += new System.EventHandler(this.FileOpen_Click);
      // 
      // FileOpenTerrainMenu
      // 
      this.FileOpenTerrainMenu.Image = ((System.Drawing.Image)(resources.GetObject("FileOpenTerrainMenu.Image")));
      this.FileOpenTerrainMenu.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.FileOpenTerrainMenu.Name = "FileOpenTerrainMenu";
      this.FileOpenTerrainMenu.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.T)));
      this.FileOpenTerrainMenu.Size = new System.Drawing.Size(235, 22);
      this.FileOpenTerrainMenu.Text = "Open &Terrain Database";
      this.FileOpenTerrainMenu.Click += new System.EventHandler(this.FileOpenTerrain_Click);
      // 
      // toolStripSeparator7
      // 
      this.toolStripSeparator7.Name = "toolStripSeparator7";
      this.toolStripSeparator7.Size = new System.Drawing.Size(232, 6);
      // 
      // FileExit
      // 
      this.FileExit.Name = "FileExit";
      this.FileExit.Size = new System.Drawing.Size(235, 22);
      this.FileExit.Text = "E&xit";
      this.FileExit.Click += new System.EventHandler(this.FileExitMenu_Click);
      // 
      // toolsToolStripMenuItem
      // 
      this.toolsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ZoomInMenu,
            this.ZoomOutMenu,
            this.ResetMenu,
            this.toolStripSeparator6,
            this.TrackballEyepointMenu,
            this.TrackballWorldMenu,
            this.toolStripSeparator8,
            this.savedViewsToolStripMenuItem,
            this.viewStackToolStripMenuItem});
      this.toolsToolStripMenuItem.Name = "toolsToolStripMenuItem";
      this.toolsToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
      this.toolsToolStripMenuItem.Text = "&Tools";
      // 
      // ZoomInMenu
      // 
      this.ZoomInMenu.Image = ((System.Drawing.Image)(resources.GetObject("ZoomInMenu.Image")));
      this.ZoomInMenu.Name = "ZoomInMenu";
      this.ZoomInMenu.Size = new System.Drawing.Size(172, 22);
      this.ZoomInMenu.Text = "Zoom In";
      this.ZoomInMenu.Click += new System.EventHandler(this.ZoomIn_Click);
      // 
      // ZoomOutMenu
      // 
      this.ZoomOutMenu.Image = ((System.Drawing.Image)(resources.GetObject("ZoomOutMenu.Image")));
      this.ZoomOutMenu.Name = "ZoomOutMenu";
      this.ZoomOutMenu.Size = new System.Drawing.Size(172, 22);
      this.ZoomOutMenu.Text = "Zoom Out";
      this.ZoomOutMenu.Click += new System.EventHandler(this.ZoomOut_Click);
      // 
      // ResetMenu
      // 
      this.ResetMenu.Image = ((System.Drawing.Image)(resources.GetObject("ResetMenu.Image")));
      this.ResetMenu.Name = "ResetMenu";
      this.ResetMenu.Size = new System.Drawing.Size(172, 22);
      this.ResetMenu.Text = "Reset";
      this.ResetMenu.Click += new System.EventHandler(this.Reset_Click);
      // 
      // toolStripSeparator6
      // 
      this.toolStripSeparator6.Name = "toolStripSeparator6";
      this.toolStripSeparator6.Size = new System.Drawing.Size(169, 6);
      // 
      // TrackballEyepointMenu
      // 
      this.TrackballEyepointMenu.Checked = true;
      this.TrackballEyepointMenu.CheckState = System.Windows.Forms.CheckState.Checked;
      this.TrackballEyepointMenu.Image = ((System.Drawing.Image)(resources.GetObject("TrackballEyepointMenu.Image")));
      this.TrackballEyepointMenu.Name = "TrackballEyepointMenu";
      this.TrackballEyepointMenu.Size = new System.Drawing.Size(172, 22);
      this.TrackballEyepointMenu.Text = "Trackball Eyepoint";
      // 
      // TrackballWorldMenu
      // 
      this.TrackballWorldMenu.Image = ((System.Drawing.Image)(resources.GetObject("TrackballWorldMenu.Image")));
      this.TrackballWorldMenu.Name = "TrackballWorldMenu";
      this.TrackballWorldMenu.Size = new System.Drawing.Size(172, 22);
      this.TrackballWorldMenu.Text = "Trackball World";
      // 
      // toolStripSeparator8
      // 
      this.toolStripSeparator8.Name = "toolStripSeparator8";
      this.toolStripSeparator8.Size = new System.Drawing.Size(169, 6);
      // 
      // savedViewsToolStripMenuItem
      // 
      this.savedViewsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.goto1ToolStripMenuItem,
            this.goto2ToolStripMenuItem,
            this.goto3ToolStripMenuItem,
            this.toolStripSeparator9,
            this.set1ToolStripMenuItem,
            this.set2ToolStripMenuItem,
            this.set3ToolStripMenuItem,
            this.toolStripSeparator10,
            this.resetToolStripMenuItem1});
      this.savedViewsToolStripMenuItem.Name = "savedViewsToolStripMenuItem";
      this.savedViewsToolStripMenuItem.Size = new System.Drawing.Size(172, 22);
      this.savedViewsToolStripMenuItem.Text = "Saved Views";
      // 
      // goto1ToolStripMenuItem
      // 
      this.goto1ToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("goto1ToolStripMenuItem.Image")));
      this.goto1ToolStripMenuItem.Name = "goto1ToolStripMenuItem";
      this.goto1ToolStripMenuItem.Size = new System.Drawing.Size(117, 22);
      this.goto1ToolStripMenuItem.Tag = "1";
      this.goto1ToolStripMenuItem.Text = "Goto 1";
      this.goto1ToolStripMenuItem.Click += new System.EventHandler(this.GotoSavedView_Click);
      // 
      // goto2ToolStripMenuItem
      // 
      this.goto2ToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("goto2ToolStripMenuItem.Image")));
      this.goto2ToolStripMenuItem.Name = "goto2ToolStripMenuItem";
      this.goto2ToolStripMenuItem.Size = new System.Drawing.Size(117, 22);
      this.goto2ToolStripMenuItem.Tag = "2";
      this.goto2ToolStripMenuItem.Text = "Goto 2";
      this.goto2ToolStripMenuItem.Click += new System.EventHandler(this.GotoSavedView_Click);
      // 
      // goto3ToolStripMenuItem
      // 
      this.goto3ToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("goto3ToolStripMenuItem.Image")));
      this.goto3ToolStripMenuItem.Name = "goto3ToolStripMenuItem";
      this.goto3ToolStripMenuItem.Size = new System.Drawing.Size(117, 22);
      this.goto3ToolStripMenuItem.Tag = "3";
      this.goto3ToolStripMenuItem.Text = "Goto 3";
      this.goto3ToolStripMenuItem.Click += new System.EventHandler(this.GotoSavedView_Click);
      // 
      // toolStripSeparator9
      // 
      this.toolStripSeparator9.Name = "toolStripSeparator9";
      this.toolStripSeparator9.Size = new System.Drawing.Size(114, 6);
      // 
      // set1ToolStripMenuItem
      // 
      this.set1ToolStripMenuItem.Name = "set1ToolStripMenuItem";
      this.set1ToolStripMenuItem.Size = new System.Drawing.Size(117, 22);
      this.set1ToolStripMenuItem.Tag = "1";
      this.set1ToolStripMenuItem.Text = "Set 1";
      this.set1ToolStripMenuItem.Click += new System.EventHandler(this.SaveView_Click);
      // 
      // set2ToolStripMenuItem
      // 
      this.set2ToolStripMenuItem.Name = "set2ToolStripMenuItem";
      this.set2ToolStripMenuItem.Size = new System.Drawing.Size(117, 22);
      this.set2ToolStripMenuItem.Tag = "2";
      this.set2ToolStripMenuItem.Text = "Set 2";
      this.set2ToolStripMenuItem.Click += new System.EventHandler(this.SaveView_Click);
      // 
      // set3ToolStripMenuItem
      // 
      this.set3ToolStripMenuItem.Name = "set3ToolStripMenuItem";
      this.set3ToolStripMenuItem.Size = new System.Drawing.Size(117, 22);
      this.set3ToolStripMenuItem.Tag = "3";
      this.set3ToolStripMenuItem.Text = "Set 3";
      this.set3ToolStripMenuItem.Click += new System.EventHandler(this.SaveView_Click);
      // 
      // toolStripSeparator10
      // 
      this.toolStripSeparator10.Name = "toolStripSeparator10";
      this.toolStripSeparator10.Size = new System.Drawing.Size(114, 6);
      // 
      // resetToolStripMenuItem1
      // 
      this.resetToolStripMenuItem1.Name = "resetToolStripMenuItem1";
      this.resetToolStripMenuItem1.Size = new System.Drawing.Size(117, 22);
      this.resetToolStripMenuItem1.Text = "Reset";
      this.resetToolStripMenuItem1.Click += new System.EventHandler(this.ResetSavedViews_Click);
      // 
      // viewStackToolStripMenuItem
      // 
      this.viewStackToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.previousViewToolStripMenuItem,
            this.nextViewToolStripMenuItem});
      this.viewStackToolStripMenuItem.Name = "viewStackToolStripMenuItem";
      this.viewStackToolStripMenuItem.Size = new System.Drawing.Size(172, 22);
      this.viewStackToolStripMenuItem.Text = "View Stack";
      // 
      // previousViewToolStripMenuItem
      // 
      this.previousViewToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("previousViewToolStripMenuItem.Image")));
      this.previousViewToolStripMenuItem.Name = "previousViewToolStripMenuItem";
      this.previousViewToolStripMenuItem.Size = new System.Drawing.Size(151, 22);
      this.previousViewToolStripMenuItem.Text = "Previous View";
      this.previousViewToolStripMenuItem.Click += new System.EventHandler(this.GotoPreviewView_Click);
      // 
      // nextViewToolStripMenuItem
      // 
      this.nextViewToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("nextViewToolStripMenuItem.Image")));
      this.nextViewToolStripMenuItem.Name = "nextViewToolStripMenuItem";
      this.nextViewToolStripMenuItem.Size = new System.Drawing.Size(151, 22);
      this.nextViewToolStripMenuItem.Text = "Next View";
      this.nextViewToolStripMenuItem.Click += new System.EventHandler(this.GotoNextView_Click);
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
      this.aboutToolStripMenuItem.Size = new System.Drawing.Size(126, 22);
      this.aboutToolStripMenuItem.Text = "&About...";
      this.aboutToolStripMenuItem.Click += new System.EventHandler(this.aboutToolStripMenuItem_Click);
      // 
      // FileOpenTerrainDialog
      // 
      this.FileOpenTerrainDialog.DefaultExt = "tdf";
      this.FileOpenTerrainDialog.Filter = "MapLink Terrain Database Files|*.tdf|All files|*.*";
      this.FileOpenTerrainDialog.ReadOnlyChecked = true;
      this.FileOpenTerrainDialog.RestoreDirectory = true;
      this.FileOpenTerrainDialog.Title = "Open a MapLink Terrain Database";
      // 
      // MapViewerForm
      // 
      this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
      this.ClientSize = new System.Drawing.Size(944, 776);
      this.Controls.Add(this.TheToolStripContainer);
      this.Controls.Add(this.TheMenuStrip);
      this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
      this.MainMenuStrip = this.TheMenuStrip;
      this.Name = "MapViewerForm";
      this.Text = "C# .NET 3D Interaction Modes Sample";
      this.MouseWheel += new System.Windows.Forms.MouseEventHandler(this.OnMouseWheel);
      this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.OnClosed);
      this.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.OnKeyPress);
      this.KeyUp += new System.Windows.Forms.KeyEventHandler(this.OnKeyUp);
      this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.OnKeyDown);
      this.TheStatusStrip.ResumeLayout(false);
      this.TheStatusStrip.PerformLayout();
      this.TheToolStripContainer.BottomToolStripPanel.ResumeLayout(false);
      this.TheToolStripContainer.BottomToolStripPanel.PerformLayout();
      this.TheToolStripContainer.ContentPanel.ResumeLayout(false);
      this.TheToolStripContainer.TopToolStripPanel.ResumeLayout(false);
      this.TheToolStripContainer.TopToolStripPanel.PerformLayout();
      this.TheToolStripContainer.ResumeLayout(false);
      this.TheToolStripContainer.PerformLayout();
      this.TheToolStrip.ResumeLayout(false);
      this.TheToolStrip.PerformLayout();
      this.TheMenuStrip.ResumeLayout(false);
      this.TheMenuStrip.PerformLayout();
      this.ResumeLayout(false);
      this.PerformLayout();

    }
    #endregion

    private System.Windows.Forms.OpenFileDialog FileOpenDialog;
    private CSharp3DInteractionSample.MapViewerPanel ViewerPanel;
    private System.Windows.Forms.StatusStrip TheStatusStrip;
    private System.Windows.Forms.ToolStripStatusLabel StatusLabel;
    private System.Windows.Forms.ToolStripStatusLabel LatitudeLabel;
    private System.Windows.Forms.ToolStripStatusLabel LongitudeLabel;
    private System.Windows.Forms.ToolStripStatusLabel AltitudeLabel;
    private System.Windows.Forms.ToolStrip TheToolStrip;
    private System.Windows.Forms.ToolStripContainer TheToolStripContainer;
    private System.Windows.Forms.ToolStripButton ZoomInButton;
    private System.Windows.Forms.ToolStripButton ZoomOutButton;
    private System.Windows.Forms.ToolStripButton ResetButton;
    private System.Windows.Forms.ToolStripButton TrackballEyepointButton;
    private System.Windows.Forms.ToolStripButton TrackballWorldButton;
    private System.Windows.Forms.ToolStripButton previousViewButton;
    private System.Windows.Forms.ToolStripButton nextViewButton;
    private System.Windows.Forms.ToolStripButton savedViewButton1;
    private System.Windows.Forms.ToolStripButton savedViewButton2;
    private System.Windows.Forms.ToolStripButton savedViewButton3;
    private System.Windows.Forms.MenuStrip TheMenuStrip;
    private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem FileOpenTerrainMenu;
    private System.Windows.Forms.ToolStripSeparator toolStripSeparator7;
    private System.Windows.Forms.ToolStripMenuItem FileExit;
    private System.Windows.Forms.ToolStripMenuItem toolsToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem ZoomInMenu;
    private System.Windows.Forms.ToolStripMenuItem ZoomOutMenu;
    private System.Windows.Forms.ToolStripMenuItem ResetMenu;
    private System.Windows.Forms.ToolStripSeparator toolStripSeparator6;
    private System.Windows.Forms.ToolStripMenuItem TrackballEyepointMenu;
    private System.Windows.Forms.ToolStripMenuItem TrackballWorldMenu;
    private System.Windows.Forms.ToolStripSeparator toolStripSeparator8;
    private System.Windows.Forms.ToolStripMenuItem savedViewsToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem goto1ToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem goto2ToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem goto3ToolStripMenuItem;
    private System.Windows.Forms.ToolStripSeparator toolStripSeparator9;
    private System.Windows.Forms.ToolStripMenuItem set1ToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem set2ToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem set3ToolStripMenuItem;
    private System.Windows.Forms.ToolStripSeparator toolStripSeparator10;
    private System.Windows.Forms.ToolStripMenuItem resetToolStripMenuItem1;
    private System.Windows.Forms.ToolStripMenuItem viewStackToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem previousViewToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem nextViewToolStripMenuItem;
    private System.Windows.Forms.ToolStripButton openToolStripButton;
    private System.Windows.Forms.ToolStripButton helpToolStripButton;
    private System.Windows.Forms.ToolStripMenuItem FileOpenMenu;
    private System.Windows.Forms.OpenFileDialog FileOpenTerrainDialog;
  }
}
