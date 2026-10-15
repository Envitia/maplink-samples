
namespace CSharpAPP6ASample
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
      System.Windows.Forms.ToolStripSeparator toolStripSeparator9;
      System.Windows.Forms.ToolStripSeparator toolStripSeparator10;
      System.Windows.Forms.ToolStripSeparator toolStripSeparator13;
      System.Windows.Forms.ToolStripSeparator toolStripSeparator15;
      System.Windows.Forms.ToolStripSeparator toolStripSeparator5;
      System.Windows.Forms.ToolStripSeparator toolStripSeparator17;
      System.Windows.Forms.ToolStripSeparator toolStripSeparator18;
      System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MapViewerForm));
      this.separator_tools = new System.Windows.Forms.ToolStripSeparator();
      this.separator_primitives1 = new System.Windows.Forms.ToolStripSeparator();
      this.separator_transformation2 = new System.Windows.Forms.ToolStripSeparator();
      this.separator_transformation3 = new System.Windows.Forms.ToolStripSeparator();
      this.separator_transformation1 = new System.Windows.Forms.ToolStripSeparator();
      this.FileOpenMapDialog = new System.Windows.Forms.OpenFileDialog();
      this.toolStripContainer1 = new System.Windows.Forms.ToolStripContainer();
      this.TheStatusStrip = new System.Windows.Forms.StatusStrip();
      this.StatusString = new System.Windows.Forms.ToolStripStatusLabel();
      this.MouseReadout = new System.Windows.Forms.ToolStripStatusLabel();
      this.SelectedEntityCountString = new System.Windows.Forms.ToolStripStatusLabel();
      this.ViewerPanel = new CSharpAPP6ASample.MapViewerPanel();
      this.AttributesToolStrip = new System.Windows.Forms.ToolStrip();
      this.AttributePanel = new CSharpAPP6ASample.DockableAttributePanel();
      this.PreferencesToolStrip = new System.Windows.Forms.ToolStrip();
      this.PreferencesPanel = new CSharpAPP6ASample.DockablePreferencesPanel();
      this.PrimitiveToolStrip = new System.Windows.Forms.ToolStrip();
      this.btn_polygon = new System.Windows.Forms.ToolStripButton();
      this.btn_polyline = new System.Windows.Forms.ToolStripButton();
      this.btn_point = new System.Windows.Forms.ToolStripButton();
      this.btn_text = new System.Windows.Forms.ToolStripButton();
      this.btn_changeText = new System.Windows.Forms.ToolStripButton();
      this.btn_symbolPalette = new System.Windows.Forms.ToolStripButton();
      this.btn_symbolProperties = new System.Windows.Forms.ToolStripButton();
      this.btn_symbolSelector = new System.Windows.Forms.ToolStripButton();
      this.btn_declutterOptions = new System.Windows.Forms.ToolStripButton();
      this.PointToolStrip = new System.Windows.Forms.ToolStrip();
      this.btn_addPoint = new System.Windows.Forms.ToolStripButton();
      this.btn_deletePoint = new System.Windows.Forms.ToolStripButton();
      this.btn_movePoint = new System.Windows.Forms.ToolStripButton();
      this.TransformsToolStrip = new System.Windows.Forms.ToolStrip();
      this.btn_move = new System.Windows.Forms.ToolStripButton();
      this.btn_scale = new System.Windows.Forms.ToolStripButton();
      this.btn_rotate = new System.Windows.Forms.ToolStripButton();
      this.btn_group = new System.Windows.Forms.ToolStripButton();
      this.btn_ungroup = new System.Windows.Forms.ToolStripButton();
      this.btn_bringToFront = new System.Windows.Forms.ToolStripButton();
      this.btn_sendToBack = new System.Windows.Forms.ToolStripButton();
      this.ConstraintsToolStrip = new System.Windows.Forms.ToolStrip();
      this.btn_horizontal = new System.Windows.Forms.ToolStripButton();
      this.btn_vertical = new System.Windows.Forms.ToolStripButton();
      this.btn_equal = new System.Windows.Forms.ToolStripButton();
      this.btn_unequal = new System.Windows.Forms.ToolStripButton();
      this.btn_angular = new System.Windows.Forms.ToolStripButton();
      this.btn_distance = new System.Windows.Forms.ToolStripButton();
      this.SelectToolStrip = new System.Windows.Forms.ToolStrip();
      this.btn_selectAll = new System.Windows.Forms.ToolStripButton();
      this.btn_selectByRectangle = new System.Windows.Forms.ToolStripButton();
      this.btn_selectByPolygon = new System.Windows.Forms.ToolStripButton();
      this.ToolsToolStrip = new System.Windows.Forms.ToolStrip();
      this.btn_zoomIn = new System.Windows.Forms.ToolStripButton();
      this.btn_zoomOut = new System.Windows.Forms.ToolStripButton();
      this.btn_reset = new System.Windows.Forms.ToolStripButton();
      this.btn_zoom = new System.Windows.Forms.ToolStripButton();
      this.btn_pan = new System.Windows.Forms.ToolStripButton();
      this.btn_grab = new System.Windows.Forms.ToolStripButton();
      this.btn_zoomPan = new System.Windows.Forms.ToolStripButton();
      this.btn_edit = new System.Windows.Forms.ToolStripButton();
      this.MainToolStrip = new System.Windows.Forms.ToolStrip();
      this.btn_new = new System.Windows.Forms.ToolStripButton();
      this.btn_open = new System.Windows.Forms.ToolStripSplitButton();
      this.openbuttonGeometryMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.openbuttonMapMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.btn_save = new System.Windows.Forms.ToolStripButton();
      this.btn_print = new System.Windows.Forms.ToolStripButton();
      this.btn_about = new System.Windows.Forms.ToolStripButton();
      this.EditToolStrip = new System.Windows.Forms.ToolStrip();
      this.btn_undo = new System.Windows.Forms.ToolStripButton();
      this.btn_backup = new System.Windows.Forms.ToolStripButton();
      this.toolStripSeparator7 = new System.Windows.Forms.ToolStripSeparator();
      this.btn_copy = new System.Windows.Forms.ToolStripButton();
      this.btn_delete = new System.Windows.Forms.ToolStripButton();
      this.TheMenu = new System.Windows.Forms.MenuStrip();
      this.menu_File = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_File_New = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_File_Open = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_File_OpenGeometry = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_File_OpenMap = new System.Windows.Forms.ToolStripMenuItem();
      this.separator_file1 = new System.Windows.Forms.ToolStripSeparator();
      this.menu_File_Save = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_File_SaveAs = new System.Windows.Forms.ToolStripMenuItem();
      this.separator_file2 = new System.Windows.Forms.ToolStripSeparator();
      this.menu_File_Print = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_File_PrintPreview = new System.Windows.Forms.ToolStripMenuItem();
      this.separator_file3 = new System.Windows.Forms.ToolStripSeparator();
      this.menu_File_Exit = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Edit = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Edit_Undo = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Edit_Backup = new System.Windows.Forms.ToolStripMenuItem();
      this.separator_edit1 = new System.Windows.Forms.ToolStripSeparator();
      this.menu_Edit_Copy = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Edit_Delete = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Edit_Clear = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_View = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_View_AttributesPanel = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_View_PreferencesPanel = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_View_Statusbar = new System.Windows.Forms.ToolStripMenuItem();
      this.separator_view1 = new System.Windows.Forms.ToolStripSeparator();
      this.menu_View_GridLayer = new System.Windows.Forms.ToolStripMenuItem();
      this.separator_view2 = new System.Windows.Forms.ToolStripSeparator();
      this.menu_View_Toolbars = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_View_ToolbarsMain = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_View_ToolbarsEdit = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_View_ToolbarsSelect = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_View_ToolbarsTools = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_View_ToolbarsConstraints = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_View_ToolbarsPrimitives = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_View_ToolbarsTransforms = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_View_ToolbarsPointOperations = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Select = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Select_All = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Select_ByRectangle = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Select_ByPolygon = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Tools = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Tools_ZoomIn = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Tools_ZoomOut = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Tools_Reset = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Tools_Zoommode = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Tools_Panmode = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Tools_Grabmode = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Tools_Zoompanmode = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Tools_Editmode = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_aPP6ATool = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_aPP6ATool_palette = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_aPP6ATool_properties = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_aPP6ATool_selector = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_aPP6ATool_declutterOptions = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Constraints = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Constraints_Horizontal = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Constraints_Vertical = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Constraints_Equal = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Constraints_Unequal = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Constraints_Angular = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Constraints_Distance = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Primitives = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Primitives_Polygon = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Primitives_Polyline = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Primitives_Point = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Primitives_Circle = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Primitives_Text = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Primitives_ChangeText = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Transformation = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Transformation_Move = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Transformation_Scale = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Transformation_Rotate = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Transformation_AddPoint = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Transformation_DeletePoint = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Transformation_MovePoint = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Transformation_Group = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Transformation_Ungroup = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Transformation_Front = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Transformation_Back = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Help = new System.Windows.Forms.ToolStripMenuItem();
      this.menu_Help_About = new System.Windows.Forms.ToolStripMenuItem();
      this.FileOpenTMFDialog = new System.Windows.Forms.OpenFileDialog();
      this.FileSaveTMFDialog = new System.Windows.Forms.SaveFileDialog();
      toolStripSeparator9 = new System.Windows.Forms.ToolStripSeparator();
      toolStripSeparator10 = new System.Windows.Forms.ToolStripSeparator();
      toolStripSeparator13 = new System.Windows.Forms.ToolStripSeparator();
      toolStripSeparator15 = new System.Windows.Forms.ToolStripSeparator();
      toolStripSeparator5 = new System.Windows.Forms.ToolStripSeparator();
      toolStripSeparator17 = new System.Windows.Forms.ToolStripSeparator();
      toolStripSeparator18 = new System.Windows.Forms.ToolStripSeparator();
      this.toolStripContainer1.BottomToolStripPanel.SuspendLayout();
      this.toolStripContainer1.ContentPanel.SuspendLayout();
      this.toolStripContainer1.LeftToolStripPanel.SuspendLayout();
      this.toolStripContainer1.TopToolStripPanel.SuspendLayout();
      this.toolStripContainer1.SuspendLayout();
      this.TheStatusStrip.SuspendLayout();
      this.AttributesToolStrip.SuspendLayout();
      this.PreferencesToolStrip.SuspendLayout();
      this.PrimitiveToolStrip.SuspendLayout();
      this.PointToolStrip.SuspendLayout();
      this.TransformsToolStrip.SuspendLayout();
      this.MainToolStrip.SuspendLayout();
      this.ConstraintsToolStrip.SuspendLayout();
      this.SelectToolStrip.SuspendLayout();
      this.ToolsToolStrip.SuspendLayout();
      this.EditToolStrip.SuspendLayout();
      this.TheMenu.SuspendLayout();
      this.SuspendLayout();
      // 
      // toolStripSeparator9
      // 
      toolStripSeparator9.Name = "toolStripSeparator9";
      toolStripSeparator9.Size = new System.Drawing.Size(6, 25);
      // 
      // toolStripSeparator10
      // 
      toolStripSeparator10.Name = "toolStripSeparator10";
      toolStripSeparator10.Size = new System.Drawing.Size(22, 6);
      // 
      // toolStripSeparator13
      // 
      toolStripSeparator13.Name = "toolStripSeparator13";
      toolStripSeparator13.Size = new System.Drawing.Size(22, 6);
      // 
      // toolStripSeparator15
      // 
      toolStripSeparator15.Name = "toolStripSeparator15";
      toolStripSeparator15.Size = new System.Drawing.Size(22, 6);
      // 
      // toolStripSeparator5
      // 
      toolStripSeparator5.Name = "toolStripSeparator5";
      toolStripSeparator5.Size = new System.Drawing.Size(6, 25);
      // 
      // separator_tools
      // 
      this.separator_tools.Name = "separator_tools";
      this.separator_tools.Size = new System.Drawing.Size(149, 6);
      // 
      // separator_primitives1
      // 
      this.separator_primitives1.Name = "separator_primitives1";
      this.separator_primitives1.Size = new System.Drawing.Size(149, 6);
      // 
      // separator_transformation2
      // 
      this.separator_transformation2.Name = "separator_transformation2";
      this.separator_transformation2.Size = new System.Drawing.Size(149, 6);
      // 
      // separator_transformation3
      // 
      this.separator_transformation3.Name = "separator_transformation3";
      this.separator_transformation3.Size = new System.Drawing.Size(149, 6);
      //
      // separator_transformation1
      //
      this.separator_transformation1.Name = "separator_transformation1";
      this.separator_transformation1.Size = new System.Drawing.Size(149, 6);
      // 
      // toolStripSeparator17
      // 
      toolStripSeparator17.Name = "toolStripSeparator17";
      toolStripSeparator17.Size = new System.Drawing.Size(136, 6);
      // 
      // toolStripSeparator18
      // 
      toolStripSeparator18.Name = "toolStripSeparator18";
      toolStripSeparator18.Size = new System.Drawing.Size(166, 6);
      // 
      // FileOpenMapDialog
      // 
      this.FileOpenMapDialog.DefaultExt = "map";
      this.FileOpenMapDialog.Filter = "MapLink Maps|*.map;*.mpc|All files|*.*";
      this.FileOpenMapDialog.ReadOnlyChecked = true;
      this.FileOpenMapDialog.Title = "Open a MapLink Map";
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
      this.toolStripContainer1.ContentPanel.Size = new System.Drawing.Size(752, 581);
      this.toolStripContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
      // 
      // toolStripContainer1.LeftToolStripPanel
      // 
      this.toolStripContainer1.LeftToolStripPanel.Controls.Add(this.AttributesToolStrip);
      this.toolStripContainer1.LeftToolStripPanel.Controls.Add(this.PreferencesToolStrip);
      this.toolStripContainer1.Location = new System.Drawing.Point(0, 24);
      this.toolStripContainer1.Name = "toolStripContainer1";
      //
      // toolStripContainer1.RightToolStripPanel
      //
      this.toolStripContainer1.RightToolStripPanel.Controls.Add(this.PrimitiveToolStrip);
      this.toolStripContainer1.RightToolStripPanel.Controls.Add(this.PointToolStrip);
      this.toolStripContainer1.RightToolStripPanel.Controls.Add(this.TransformsToolStrip);
      this.toolStripContainer1.Size = new System.Drawing.Size(953, 703);
      this.toolStripContainer1.TabIndex = 3;
      this.toolStripContainer1.Text = "toolStripContainer1";
      // 
      // toolStripContainer1.TopToolStripPanel
      // 
      this.toolStripContainer1.TopToolStripPanel.Controls.Add(this.ConstraintsToolStrip);
      this.toolStripContainer1.TopToolStripPanel.Controls.Add(this.SelectToolStrip);
      this.toolStripContainer1.TopToolStripPanel.Controls.Add(this.ToolsToolStrip);
      this.toolStripContainer1.TopToolStripPanel.Controls.Add(this.MainToolStrip);
      this.toolStripContainer1.TopToolStripPanel.Controls.Add(this.EditToolStrip);
      // 
      // TheStatusStrip
      // 
      this.TheStatusStrip.Dock = System.Windows.Forms.DockStyle.None;
      this.TheStatusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.StatusString,
            this.MouseReadout,
            this.SelectedEntityCountString});
      this.TheStatusStrip.Location = new System.Drawing.Point(0, 0);
      this.TheStatusStrip.Name = "TheStatusStrip";
      this.TheStatusStrip.Size = new System.Drawing.Size(953, 22);
      this.TheStatusStrip.TabIndex = 3;
      this.TheStatusStrip.Text = "TheStatusStrip";
      // 
      // StatusString
      // 
      this.StatusString.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
      this.StatusString.Name = "StatusString";
      this.StatusString.Size = new System.Drawing.Size(638, 17);
      this.StatusString.Spring = true;
      this.StatusString.Text = "Select object or operation";
      this.StatusString.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // MouseReadout
      // 
      this.MouseReadout.AutoSize = false;
      this.MouseReadout.BorderSides = ((System.Windows.Forms.ToolStripStatusLabelBorderSides)((((System.Windows.Forms.ToolStripStatusLabelBorderSides.Left | System.Windows.Forms.ToolStripStatusLabelBorderSides.Top)
              | System.Windows.Forms.ToolStripStatusLabelBorderSides.Right)
              | System.Windows.Forms.ToolStripStatusLabelBorderSides.Bottom)));
      this.MouseReadout.BorderStyle = System.Windows.Forms.Border3DStyle.SunkenOuter;
      this.MouseReadout.Name = "MouseReadout";
      this.MouseReadout.Size = new System.Drawing.Size(200, 17);
      this.MouseReadout.Text = "Pos : X 0  Y 0";
      this.MouseReadout.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // SelectedEntityCountString
      // 
      this.SelectedEntityCountString.AutoSize = false;
      this.SelectedEntityCountString.BorderSides = ((System.Windows.Forms.ToolStripStatusLabelBorderSides)((((System.Windows.Forms.ToolStripStatusLabelBorderSides.Left | System.Windows.Forms.ToolStripStatusLabelBorderSides.Top)
              | System.Windows.Forms.ToolStripStatusLabelBorderSides.Right)
              | System.Windows.Forms.ToolStripStatusLabelBorderSides.Bottom)));
      this.SelectedEntityCountString.BorderStyle = System.Windows.Forms.Border3DStyle.SunkenOuter;
      this.SelectedEntityCountString.Name = "SelectedEntityCountString";
      this.SelectedEntityCountString.Size = new System.Drawing.Size(100, 17);
      this.SelectedEntityCountString.Text = "Select : 0";
      this.SelectedEntityCountString.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // ViewerPanel
      // 
      this.ViewerPanel.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
      this.ViewerPanel.Dock = System.Windows.Forms.DockStyle.Fill;
      this.ViewerPanel.Location = new System.Drawing.Point(0, 0);
      this.ViewerPanel.Name = "ViewerPanel";
      this.ViewerPanel.Size = new System.Drawing.Size(752, 581);
      this.ViewerPanel.TabIndex = 2;
      this.ViewerPanel.Paint += new System.Windows.Forms.PaintEventHandler(this.OnPaint);
      this.ViewerPanel.MouseMove += new System.Windows.Forms.MouseEventHandler(this.OnMouseMove);
      this.ViewerPanel.MouseDown += new System.Windows.Forms.MouseEventHandler(this.OnMouseDown);
      this.ViewerPanel.MouseUp += new System.Windows.Forms.MouseEventHandler(this.OnMouseUp);
      this.ViewerPanel.Resize += new System.EventHandler(this.OnReSize);
      // 
      // AttributesToolStrip
      // 
      this.AttributesToolStrip.CanOverflow = false;
      this.AttributesToolStrip.CausesValidation = true;
      this.AttributesToolStrip.Dock = System.Windows.Forms.DockStyle.None;
      this.AttributesToolStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.AttributePanel});
      this.AttributesToolStrip.Location = new System.Drawing.Point(0, 3);
      this.AttributesToolStrip.Name = "AttributesToolStrip";
      this.AttributesToolStrip.Size = new System.Drawing.Size(169, 484);
      this.AttributesToolStrip.TabIndex = 3;
      this.AttributesToolStrip.Text = "toolStrip1";
      // 
      // AttributePanel
      // 
      this.AttributePanel.BackColor = System.Drawing.Color.Transparent;
      this.AttributePanel.Name = "DockableAttributePanel";
      this.AttributePanel.Size = new System.Drawing.Size(167, 470);
      this.AttributePanel.Text = "AttributePanel";
      // 
      // PreferencesToolStrip
      // 
      this.PreferencesToolStrip.CanOverflow = false;
      this.PreferencesToolStrip.Dock = System.Windows.Forms.DockStyle.None;
      this.PreferencesToolStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.PreferencesPanel});
      this.PreferencesToolStrip.Location = new System.Drawing.Point(0, 487);
      this.PreferencesToolStrip.Name = "PreferencesToolStrip";
      this.PreferencesToolStrip.Size = new System.Drawing.Size(169, 94);
      this.PreferencesToolStrip.TabIndex = 4;
      // 
      // PreferencesPanel
      // 
      this.PreferencesPanel.BackColor = System.Drawing.Color.Transparent;
      this.PreferencesPanel.Name = "Preferences";
      this.PreferencesPanel.Size = new System.Drawing.Size(167, 150);
      this.PreferencesPanel.Text = "PreferencesPanel";
      // 
      // PrimitiveToolStrip
      // 
      this.PrimitiveToolStrip.Dock = System.Windows.Forms.DockStyle.None;
      this.PrimitiveToolStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btn_polygon,
            this.btn_polyline,
            this.btn_point,
            this.btn_text,
            this.btn_changeText,
            toolStripSeparator15,
            this.btn_symbolPalette,
            this.btn_symbolProperties,
            this.btn_symbolSelector,
            this.btn_declutterOptions});
      this.PrimitiveToolStrip.Location = new System.Drawing.Point(0, 9);
      this.PrimitiveToolStrip.Name = "PrimitiveToolStrip";
      this.PrimitiveToolStrip.Size = new System.Drawing.Size(24, 132);
      this.PrimitiveToolStrip.TabIndex = 3;
      this.PrimitiveToolStrip.Text = "toolStrip1";
      // 
      // btn_polygon
      // 
      this.btn_polygon.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_polygon.Image = global::CSharpAPP6ASample.Properties.Resources.Polygon;
      this.btn_polygon.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.btn_polygon.Name = "btn_polygon";
      this.btn_polygon.Size = new System.Drawing.Size(22, 20);
      this.btn_polygon.Text = "Polygon";
      this.btn_polygon.Click += new System.EventHandler(this.menu_Primitives_Polygon_Click);
      // 
      // btn_polyline
      // 
      this.btn_polyline.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_polyline.Image = global::CSharpAPP6ASample.Properties.Resources.Polyline;
      this.btn_polyline.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.btn_polyline.Name = "btn_polyline";
      this.btn_polyline.Size = new System.Drawing.Size(22, 20);
      this.btn_polyline.Text = "Polyline";
      this.btn_polyline.Click += new System.EventHandler(this.menu_Primitives_Polyline_Click);
      // 
      // btn_point
      // 
      this.btn_point.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_point.Image = global::CSharpAPP6ASample.Properties.Resources.Point;
      this.btn_point.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.btn_point.Name = "btn_point";
      this.btn_point.Size = new System.Drawing.Size(22, 20);
      this.btn_point.Text = "Point";
      this.btn_point.Click += new System.EventHandler(this.menu_Primitives_Point_Click);
      // 
      // btn_text
      // 
      this.btn_text.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_text.Image = global::CSharpAPP6ASample.Properties.Resources.Text;
      this.btn_text.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.btn_text.Name = "btn_text";
      this.btn_text.Size = new System.Drawing.Size(22, 20);
      this.btn_text.Text = "Text";
      this.btn_text.Click += new System.EventHandler(this.menu_Primitives_Text_Click);
      // 
      // btn_changeText
      // 
      this.btn_changeText.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_changeText.Image = global::CSharpAPP6ASample.Properties.Resources.Edit_Text;
      this.btn_changeText.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.btn_changeText.Name = "btn_changeText";
      this.btn_changeText.Size = new System.Drawing.Size(22, 20);
      this.btn_changeText.Text = "Edit Text";
      this.btn_changeText.Click += new System.EventHandler(this.menu_Primitives_ChangeText_Click);
      // 
      // btn_symbolPalette
      // 
      this.btn_symbolPalette.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_symbolPalette.Image = ((System.Drawing.Image)(resources.GetObject("btn_symbolPalette.Image")));
      this.btn_symbolPalette.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.btn_symbolPalette.Name = "btn_symbolPalette";
      this.btn_symbolPalette.Size = new System.Drawing.Size(22, 20);
      this.btn_symbolPalette.Text = "Symbol Palette";
      this.btn_symbolPalette.Click += new System.EventHandler(this.menu_aPP6ATool_Palette_Click);
      // 
      // btn_symbolProperties
      // 
      this.btn_symbolProperties.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_symbolProperties.Image = ((System.Drawing.Image)(resources.GetObject("btn_symbolProperties.Image")));
      this.btn_symbolProperties.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.btn_symbolProperties.Name = "btn_symbolProperties";
      this.btn_symbolProperties.Size = new System.Drawing.Size(22, 20);
      this.btn_symbolProperties.Text = "Symbol Properties";
      this.btn_symbolProperties.Click += new System.EventHandler(this.menu_aPP6ATool_Properties_Click);
      // 
      // btn_symbolSelector
      // 
      this.btn_symbolSelector.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_symbolSelector.Image = ((System.Drawing.Image)(resources.GetObject("btn_symbolSelector.Image")));
      this.btn_symbolSelector.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.btn_symbolSelector.Name = "btn_symbolSelector";
      this.btn_symbolSelector.Size = new System.Drawing.Size(22, 20);
      this.btn_symbolSelector.Text = "Symbol Selector";
      this.btn_symbolSelector.Click += new System.EventHandler(this.menu_aPP6ATool_Selector_Click);
      // 
      // btn_declutterOptions
      // 
      this.btn_declutterOptions.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_declutterOptions.Image = ((System.Drawing.Image)(resources.GetObject("btn_declutterOptions.Image")));
      this.btn_declutterOptions.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.btn_declutterOptions.Name = "btn_declutterOptions";
      this.btn_declutterOptions.Size = new System.Drawing.Size(22, 20);
      this.btn_declutterOptions.Text = "Declutter Options";
      this.btn_declutterOptions.Click += new System.EventHandler(this.menu_aPP6ATool_DeclutterOptions_Click);
      // 
      // PointToolStrip
      // 
      this.PointToolStrip.Dock = System.Windows.Forms.DockStyle.None;
      this.PointToolStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btn_addPoint,
            this.btn_deletePoint,
            this.btn_movePoint});
      this.PointToolStrip.Location = new System.Drawing.Point(0, 233);
      this.PointToolStrip.Name = "PointToolStrip";
      this.PointToolStrip.Size = new System.Drawing.Size(32, 80);
      this.PointToolStrip.TabIndex = 3;
      this.PointToolStrip.Text = "toolStrip1";
      // 
      // btn_addPoint
      // 
      this.btn_addPoint.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_addPoint.Image = global::CSharpAPP6ASample.Properties.Resources.Add_Point;
      this.btn_addPoint.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.btn_addPoint.Name = "btn_addPoint";
      this.btn_addPoint.Size = new System.Drawing.Size(22, 20);
      this.btn_addPoint.Text = "Add Point";
      this.btn_addPoint.Click += new System.EventHandler(this.menu_Transformation_AddPoint_Click);
      // 
      // btn_deletePoint
      // 
      this.btn_deletePoint.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_deletePoint.Image = global::CSharpAPP6ASample.Properties.Resources.Delete_Point;
      this.btn_deletePoint.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.btn_deletePoint.Name = "btn_deletePoint";
      this.btn_deletePoint.Size = new System.Drawing.Size(22, 20);
      this.btn_deletePoint.Text = "Delete Point";
      this.btn_deletePoint.Click += new System.EventHandler(this.menu_Transformation_DeletePoint_Click);
      // 
      // btn_movePoint
      // 
      this.btn_movePoint.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_movePoint.Image = global::CSharpAPP6ASample.Properties.Resources.Move_Point;
      this.btn_movePoint.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.btn_movePoint.Name = "btn_movePoint";
      this.btn_movePoint.Size = new System.Drawing.Size(22, 20);
      this.btn_movePoint.Text = "Move Point";
      this.btn_movePoint.Click += new System.EventHandler(this.menu_Transformation_MovePoint_Click);
      // 
      // TransformsToolStrip
      // 
      this.TransformsToolStrip.Dock = System.Windows.Forms.DockStyle.None;
      this.TransformsToolStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btn_move,
            this.btn_scale,
            this.btn_rotate,
            toolStripSeparator10,
            this.btn_group,
            this.btn_ungroup,
            toolStripSeparator13,
            this.btn_bringToFront,
            this.btn_sendToBack});
      this.TransformsToolStrip.Location = new System.Drawing.Point(0, 354);
      this.TransformsToolStrip.Name = "TransformsToolStrip";
      this.TransformsToolStrip.Size = new System.Drawing.Size(24, 184);
      this.TransformsToolStrip.TabIndex = 8;
      // 
      // btn_move
      // 
      this.btn_move.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_move.Image = global::CSharpAPP6ASample.Properties.Resources.Move;
      this.btn_move.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.btn_move.Name = "btn_move";
      this.btn_move.Size = new System.Drawing.Size(22, 20);
      this.btn_move.Text = "Move";
      this.btn_move.Click += new System.EventHandler(this.menu_Transformation_Move_Click);
      // 
      // btn_scale
      // 
      this.btn_scale.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_scale.Image = global::CSharpAPP6ASample.Properties.Resources.Scale;
      this.btn_scale.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.btn_scale.Name = "btn_scale";
      this.btn_scale.Size = new System.Drawing.Size(22, 20);
      this.btn_scale.Text = "Scale";
      this.btn_scale.Click += new System.EventHandler(this.menu_Transformation_Scale_Click);
      // 
      // btn_rotate
      // 
      this.btn_rotate.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_rotate.Image = global::CSharpAPP6ASample.Properties.Resources.Rotate;
      this.btn_rotate.ImageTransparentColor = System.Drawing.Color.White;
      this.btn_rotate.Name = "btn_rotate";
      this.btn_rotate.Size = new System.Drawing.Size(22, 20);
      this.btn_rotate.Text = "Rotate";
      this.btn_rotate.Click += new System.EventHandler(this.menu_Transformation_Rotate_Click);
      // 
      // btn_group
      // 
      this.btn_group.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_group.Image = global::CSharpAPP6ASample.Properties.Resources.Group;
      this.btn_group.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.btn_group.Name = "btn_group";
      this.btn_group.Size = new System.Drawing.Size(22, 20);
      this.btn_group.Text = "Group";
      this.btn_group.ToolTipText = "Group a set of entities";
      this.btn_group.Click += new System.EventHandler(this.menu_Transformation_Group_Click);
      // 
      // btn_ungroup
      // 
      this.btn_ungroup.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_ungroup.Image = global::CSharpAPP6ASample.Properties.Resources.Ungroup;
      this.btn_ungroup.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.btn_ungroup.Name = "btn_ungroup";
      this.btn_ungroup.Size = new System.Drawing.Size(22, 20);
      this.btn_ungroup.Text = "Ungroup";
      this.btn_ungroup.ToolTipText = "Ungroup a set of entities";
      this.btn_ungroup.Click += new System.EventHandler(this.menu_Transformation_Ungroup_Click);
      // 
      // btn_bringToFront
      // 
      this.btn_bringToFront.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_bringToFront.Image = global::CSharpAPP6ASample.Properties.Resources.Bring_to_Front;
      this.btn_bringToFront.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.btn_bringToFront.Name = "btn_bringToFront";
      this.btn_bringToFront.Size = new System.Drawing.Size(22, 20);
      this.btn_bringToFront.Text = "Bring To Front";
      this.btn_bringToFront.ToolTipText = "Bring an entity to the front";
      this.btn_bringToFront.Click += new System.EventHandler(this.menu_Transformation_Front_Click);
      // 
      // btn_sendToBack
      // 
      this.btn_sendToBack.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_sendToBack.Image = global::CSharpAPP6ASample.Properties.Resources.Send_to_Back;
      this.btn_sendToBack.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.btn_sendToBack.Name = "btn_sendToBack";
      this.btn_sendToBack.Size = new System.Drawing.Size(22, 20);
      this.btn_sendToBack.Text = "Send To Back";
      this.btn_sendToBack.ToolTipText = "Send an entity to the back";
      this.btn_sendToBack.Click += new System.EventHandler(this.menu_Transformation_Back_Click);
      // 
      // ConstraintsToolStrip
      // 
      this.ConstraintsToolStrip.Dock = System.Windows.Forms.DockStyle.None;
      this.ConstraintsToolStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btn_horizontal,
            this.btn_vertical,
            this.btn_equal,
            this.btn_unequal,
            this.btn_angular,
            this.btn_distance});
      this.ConstraintsToolStrip.Location = new System.Drawing.Point(3, 0);
      this.ConstraintsToolStrip.Name = "ConstraintsToolStrip";
      this.ConstraintsToolStrip.Size = new System.Drawing.Size(150, 25);
      this.ConstraintsToolStrip.TabIndex = 8;
      // 
      // btn_horizontal
      // 
      this.btn_horizontal.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_horizontal.Image = global::CSharpAPP6ASample.Properties.Resources.Constraint_Horizontal;
      this.btn_horizontal.ImageTransparentColor = System.Drawing.Color.Silver;
      this.btn_horizontal.Name = "btn_horizontal";
      this.btn_horizontal.Size = new System.Drawing.Size(23, 22);
      this.btn_horizontal.Text = "Horizontal Constaint";
      this.btn_horizontal.Click += new System.EventHandler(this.btn_horizontal_Click);
      // 
      // btn_vertical
      // 
      this.btn_vertical.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_vertical.Image = global::CSharpAPP6ASample.Properties.Resources.Constraint_Vertical;
      this.btn_vertical.ImageTransparentColor = System.Drawing.Color.Silver;
      this.btn_vertical.Name = "btn_vertical";
      this.btn_vertical.Size = new System.Drawing.Size(23, 22);
      this.btn_vertical.Text = "Vertical Constaint";
      this.btn_vertical.Click += new System.EventHandler(this.btn_vertical_Click);
      // 
      // btn_equal
      // 
      this.btn_equal.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_equal.Image = global::CSharpAPP6ASample.Properties.Resources.Constraint_Equal;
      this.btn_equal.ImageTransparentColor = System.Drawing.Color.Silver;
      this.btn_equal.Name = "btn_equal";
      this.btn_equal.Size = new System.Drawing.Size(23, 22);
      this.btn_equal.Text = "Equal Constaint";
      this.btn_equal.Click += new System.EventHandler(this.btn_equal_Click);
      // 
      // btn_unequal
      // 
      this.btn_unequal.Checked = true;
      this.btn_unequal.CheckState = System.Windows.Forms.CheckState.Checked;
      this.btn_unequal.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_unequal.Image = global::CSharpAPP6ASample.Properties.Resources.Constraint_Unequal;
      this.btn_unequal.ImageTransparentColor = System.Drawing.Color.Silver;
      this.btn_unequal.Name = "btn_unequal";
      this.btn_unequal.Size = new System.Drawing.Size(23, 22);
      this.btn_unequal.Text = "Unequal Constaint";
      this.btn_unequal.Click += new System.EventHandler(this.btn_unequal_Click);
      // 
      // btn_angular
      // 
      this.btn_angular.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_angular.Image = global::CSharpAPP6ASample.Properties.Resources.Constraint_Angular;
      this.btn_angular.ImageTransparentColor = System.Drawing.Color.Silver;
      this.btn_angular.Name = "btn_angular";
      this.btn_angular.Size = new System.Drawing.Size(23, 22);
      this.btn_angular.Text = "Angular Constaint";
      this.btn_angular.Click += new System.EventHandler(this.btn_angular_Click);
      // 
      // btn_distance
      // 
      this.btn_distance.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_distance.Image = global::CSharpAPP6ASample.Properties.Resources.Constraint_Distance;
      this.btn_distance.ImageTransparentColor = System.Drawing.Color.Silver;
      this.btn_distance.Name = "btn_distance";
      this.btn_distance.Size = new System.Drawing.Size(23, 22);
      this.btn_distance.Text = "Distance Constaint";
      this.btn_distance.Click += new System.EventHandler(this.btn_distance_Click);
      // 
      // SelectToolStrip
      // 
      this.SelectToolStrip.Dock = System.Windows.Forms.DockStyle.None;
      this.SelectToolStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btn_selectAll,
            this.btn_selectByRectangle,
            this.btn_selectByPolygon});
      this.SelectToolStrip.Location = new System.Drawing.Point(69, 25);
      this.SelectToolStrip.Name = "SelectToolStrip";
      this.SelectToolStrip.Size = new System.Drawing.Size(81, 25);
      this.SelectToolStrip.TabIndex = 7;
      // 
      // btn_selectAll
      // 
      this.btn_selectAll.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_selectAll.Image = global::CSharpAPP6ASample.Properties.Resources.Select_All;
      this.btn_selectAll.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.btn_selectAll.Name = "btn_selectAll";
      this.btn_selectAll.Size = new System.Drawing.Size(23, 22);
      this.btn_selectAll.Text = "Select All";
      this.btn_selectAll.Click += new System.EventHandler(this.menu_Select_All_Click);
      // 
      // btn_selectByRectangle
      // 
      this.btn_selectByRectangle.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_selectByRectangle.Image = global::CSharpAPP6ASample.Properties.Resources.Select_by_Rectangle;
      this.btn_selectByRectangle.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.btn_selectByRectangle.Name = "btn_selectByRectangle";
      this.btn_selectByRectangle.Size = new System.Drawing.Size(23, 22);
      this.btn_selectByRectangle.Text = "Select By Rectangle";
      this.btn_selectByRectangle.Click += new System.EventHandler(this.menu_Select_ByRectangle_Click);
      // 
      // btn_selectByPolygon
      // 
      this.btn_selectByPolygon.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_selectByPolygon.Image = global::CSharpAPP6ASample.Properties.Resources.Select_by_Polygon;
      this.btn_selectByPolygon.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.btn_selectByPolygon.Name = "btn_selectByPolygon";
      this.btn_selectByPolygon.Size = new System.Drawing.Size(23, 22);
      this.btn_selectByPolygon.Text = "Select By Polygon";
      this.btn_selectByPolygon.Click += new System.EventHandler(this.menu_Select_ByPolygon_Click);
      // 
      // ToolsToolStrip
      // 
      this.ToolsToolStrip.Dock = System.Windows.Forms.DockStyle.None;
      this.ToolsToolStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btn_zoomIn,
            this.btn_zoomOut,
            this.btn_reset,
            toolStripSeparator9,
            this.btn_zoom,
            this.btn_pan,
            this.btn_grab,
            this.btn_zoomPan,
            this.btn_edit});
      this.ToolsToolStrip.Location = new System.Drawing.Point(3, 50);
      this.ToolsToolStrip.Name = "ToolsToolStrip";
      this.ToolsToolStrip.Size = new System.Drawing.Size(233, 25);
      this.ToolsToolStrip.TabIndex = 6;
      // 
      // btn_zoomIn
      // 
      this.btn_zoomIn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_zoomIn.Image = global::CSharpAPP6ASample.Properties.Resources.zoom_in;
      this.btn_zoomIn.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.btn_zoomIn.Name = "btn_zoomIn";
      this.btn_zoomIn.Size = new System.Drawing.Size(23, 22);
      this.btn_zoomIn.Text = "Zoom In";
      this.btn_zoomIn.Click += new System.EventHandler(this.Imode_ButtonClick);
      // 
      // btn_zoomOut
      // 
      this.btn_zoomOut.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_zoomOut.Image = global::CSharpAPP6ASample.Properties.Resources.zoom_out;
      this.btn_zoomOut.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.btn_zoomOut.Name = "btn_zoomOut";
      this.btn_zoomOut.Size = new System.Drawing.Size(23, 22);
      this.btn_zoomOut.Text = "Zoom Out";
      this.btn_zoomOut.Click += new System.EventHandler(this.Imode_ButtonClick);
      // 
      // btn_reset
      // 
      this.btn_reset.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_reset.Image = global::CSharpAPP6ASample.Properties.Resources.Reset;
      this.btn_reset.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.btn_reset.Name = "btn_reset";
      this.btn_reset.Size = new System.Drawing.Size(23, 22);
      this.btn_reset.Text = "Reset";
      this.btn_reset.Click += new System.EventHandler(this.Imode_ButtonClick);
      // 
      // btn_zoom
      // 
      this.btn_zoom.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_zoom.Image = global::CSharpAPP6ASample.Properties.Resources.Zoom;
      this.btn_zoom.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.btn_zoom.Name = "btn_zoom";
      this.btn_zoom.Size = new System.Drawing.Size(23, 22);
      this.btn_zoom.Tag = 1;
      this.btn_zoom.Text = "Zoom";
      this.btn_zoom.Click += new System.EventHandler(this.Imode_ButtonClick);
      // 
      // btn_pan
      // 
      this.btn_pan.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_pan.Image = global::CSharpAPP6ASample.Properties.Resources.Pan;
      this.btn_pan.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.btn_pan.Name = "btn_pan";
      this.btn_pan.Size = new System.Drawing.Size(23, 22);
      this.btn_pan.Tag = 2;
      this.btn_pan.Text = "Pan";
      this.btn_pan.Click += new System.EventHandler(this.Imode_ButtonClick);
      // 
      // btn_grab
      // 
      this.btn_grab.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_grab.Image = global::CSharpAPP6ASample.Properties.Resources.Grab;
      this.btn_grab.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.btn_grab.Name = "btn_grab";
      this.btn_grab.Size = new System.Drawing.Size(23, 22);
      this.btn_grab.Tag = 2;
      this.btn_grab.Text = "Grab";
      this.btn_grab.Click += new System.EventHandler(this.Imode_ButtonClick);
      // 
      // btn_zoomPan
      // 
      this.btn_zoomPan.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_zoomPan.Image = global::CSharpAPP6ASample.Properties.Resources.Zoom___Pan;
      this.btn_zoomPan.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.btn_zoomPan.Name = "btn_zoomPan";
      this.btn_zoomPan.Size = new System.Drawing.Size(23, 22);
      this.btn_zoomPan.Tag = 3;
      this.btn_zoomPan.Text = "Zoom and Pan";
      this.btn_zoomPan.Click += new System.EventHandler(this.Imode_ButtonClick);
      // 
      // btn_edit
      // 
      this.btn_edit.Checked = true;
      this.btn_edit.CheckState = System.Windows.Forms.CheckState.Checked;
      this.btn_edit.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_edit.Image = global::CSharpAPP6ASample.Properties.Resources.Edit_Mode;
      this.btn_edit.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.btn_edit.Name = "btn_edit";
      this.btn_edit.Size = new System.Drawing.Size(23, 22);
      this.btn_edit.Text = "&Edit";
      this.btn_edit.Click += new System.EventHandler(this.Imode_ButtonClick);
      // 
      // MainToolStrip
      // 
      this.MainToolStrip.CausesValidation = true;
      this.MainToolStrip.Dock = System.Windows.Forms.DockStyle.None;
      this.MainToolStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btn_new,
            this.btn_open,
            this.btn_save,
            this.btn_print,
            toolStripSeparator5,
            this.btn_about});
      this.MainToolStrip.Location = new System.Drawing.Point(3, 75);
      this.MainToolStrip.Name = "MainToolStrip";
      this.MainToolStrip.Size = new System.Drawing.Size(173, 25);
      this.MainToolStrip.TabIndex = 4;
      this.MainToolStrip.Text = "MainToolStrip";
      // 
      // btn_new
      // 
      this.btn_new.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_new.Image = ((System.Drawing.Image)(resources.GetObject("newToolStripButton.Image")));
      this.btn_new.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.btn_new.Name = "btn_new";
      this.btn_new.Size = new System.Drawing.Size(23, 22);
      this.btn_new.Text = "&New";
      this.btn_new.Click += new System.EventHandler(this.FileNewGeometry);
      // 
      // btn_open
      // 
      this.btn_open.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_open.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openbuttonGeometryMenuItem,
            this.openbuttonMapMenuItem});
      this.btn_open.Image = ((System.Drawing.Image)(resources.GetObject("openToolStripButton.Image")));
      this.btn_open.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.btn_open.Name = "btn_open";
      this.btn_open.Size = new System.Drawing.Size(32, 22);
      this.btn_open.Text = "&Open";
      // 
      // openbuttonGeometryMenuItem
      // 
      this.openbuttonGeometryMenuItem.Name = "openbuttonGeometryMenuItem";
      this.openbuttonGeometryMenuItem.Size = new System.Drawing.Size(152, 22);
      this.openbuttonGeometryMenuItem.Text = "Geometry...";
      this.openbuttonGeometryMenuItem.Click += new System.EventHandler(this.FileOpenGeometry);
      // 
      // openbuttonMapMenuItem
      // 
      this.openbuttonMapMenuItem.Name = "openbuttonMapMenuItem";
      this.openbuttonMapMenuItem.Size = new System.Drawing.Size(152, 22);
      this.openbuttonMapMenuItem.Text = "Map...";
      this.openbuttonMapMenuItem.Click += new System.EventHandler(this.FileOpenMap);
      // 
      // btn_save
      // 
      this.btn_save.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_save.Image = ((System.Drawing.Image)(resources.GetObject("saveToolStripButton.Image")));
      this.btn_save.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.btn_save.Name = "btn_save";
      this.btn_save.Size = new System.Drawing.Size(23, 22);
      this.btn_save.Text = "&Save";
      this.btn_save.Click += new System.EventHandler(this.FileSaveGeometry);
      // 
      // btn_print
      // 
      this.btn_print.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_print.Image = ((System.Drawing.Image)(resources.GetObject("printToolStripButton.Image")));
      this.btn_print.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.btn_print.Name = "btn_print";
      this.btn_print.Size = new System.Drawing.Size(23, 22);
      this.btn_print.Text = "&Print";
      this.btn_print.Click += new System.EventHandler(this.FilePrintMenu_Click);
      // 
      // btn_about
      // 
      this.btn_about.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_about.Image = ((System.Drawing.Image)(resources.GetObject("aboutToolStripButton.Image")));
      this.btn_about.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.btn_about.Name = "btn_about";
      this.btn_about.Size = new System.Drawing.Size(23, 22);
      this.btn_about.Text = "About";
      this.btn_about.Click += new System.EventHandler(this.OnAppAbout);
      // 
      // EditToolStrip
      // 
      this.EditToolStrip.Dock = System.Windows.Forms.DockStyle.None;
      this.EditToolStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btn_undo,
            this.btn_backup,
            this.toolStripSeparator7,
            this.btn_copy,
            this.btn_delete});
            this.EditToolStrip.Location = new System.Drawing.Point(176, 75);
      this.EditToolStrip.Name = "EditToolStrip";
      this.EditToolStrip.Size = new System.Drawing.Size(110, 25);
      this.EditToolStrip.TabIndex = 5;
      this.EditToolStrip.Text = "EditToolStrip";
      // 
      // btn_undo
      // 
      this.btn_undo.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_undo.Image = global::CSharpAPP6ASample.Properties.Resources.undo;
      this.btn_undo.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.btn_undo.Name = "btn_undo";
      this.btn_undo.Size = new System.Drawing.Size(23, 22);
      this.btn_undo.Text = "&Undo";
      this.btn_undo.Click += new System.EventHandler(this.menu_Edit_Undo_Click);
      // 
      // btn_backup
      // 
      this.btn_backup.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_backup.Image = global::CSharpAPP6ASample.Properties.Resources.Backup;
      this.btn_backup.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.btn_backup.Name = "btn_backup";
      this.btn_backup.Size = new System.Drawing.Size(23, 22);
      this.btn_backup.Text = "&Backup";
      this.btn_backup.Click += new System.EventHandler(this.menu_Edit_Backup_Click);
      // 
      // toolStripSeparator7
      // 
      this.toolStripSeparator7.Name = "toolStripSeparator7";
      this.toolStripSeparator7.Size = new System.Drawing.Size(6, 25);
      // 
      // btn_copy
      // 
      this.btn_copy.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_copy.Image = ((System.Drawing.Image)(resources.GetObject("copyToolStripButton.Image")));
      this.btn_copy.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.btn_copy.Name = "btn_copy";
      this.btn_copy.Size = new System.Drawing.Size(23, 22);
      this.btn_copy.Text = "&Copy";
      this.btn_copy.Click += new System.EventHandler(this.menu_Edit_Copy_Click);
      // 
      // btn_delete
      // 
      this.btn_delete.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.btn_delete.Image = global::CSharpAPP6ASample.Properties.Resources.Delete;
      this.btn_delete.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.btn_delete.Name = "btn_delete";
      this.btn_delete.Size = new System.Drawing.Size(23, 22);
      this.btn_delete.Text = "Delete";
      this.btn_delete.Click += new System.EventHandler(this.menu_Edit_Delete_Click);
      // 
      // TheMenu
      // 
      this.TheMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menu_File,
            this.menu_Edit,
            this.menu_View,
            this.menu_Select,
            this.menu_Tools,
            this.menu_Constraints,
            this.menu_Primitives,
            this.menu_Transformation,
            this.menu_Help});
      this.TheMenu.Location = new System.Drawing.Point(0, 0);
      this.TheMenu.Name = "TheMenu";
      this.TheMenu.Size = new System.Drawing.Size(953, 24);
      this.TheMenu.TabIndex = 4;
      this.TheMenu.Text = "menuStrip1";
      // 
      // fileMenu
      // 
      this.menu_File.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menu_File_New,
            this.menu_File_Open,
            this.separator_file1,
            this.menu_File_Save,
            this.menu_File_SaveAs,
            this.separator_file2,
            this.menu_File_Print,
            this.menu_File_PrintPreview,
            this.separator_file3,
            this.menu_File_Exit});
      this.menu_File.Name = "fileMenu";
      this.menu_File.Size = new System.Drawing.Size(37, 20);
      this.menu_File.Text = "&File";
      // 
      // menu_File_New
      // 
      this.menu_File_New.Image = ((System.Drawing.Image)(resources.GetObject("newToolStripMenuItem.Image")));
      this.menu_File_New.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.menu_File_New.Name = "menu_File_New";
      this.menu_File_New.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.N)));
      this.menu_File_New.Size = new System.Drawing.Size(152, 22);
      this.menu_File_New.Text = "&New";
      this.menu_File_New.Click += new System.EventHandler(this.FileNewGeometry);
      // 
      // menu_File_Open
      // 
      this.menu_File_Open.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menu_File_OpenGeometry,
            this.menu_File_OpenMap});
      this.menu_File_Open.Image = ((System.Drawing.Image)(resources.GetObject("openToolStripMenuItem.Image")));
      this.menu_File_Open.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.menu_File_Open.Name = "menu_File_Open";
      this.menu_File_Open.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.O)));
      this.menu_File_Open.Size = new System.Drawing.Size(152, 22);
      this.menu_File_Open.Text = "&Open";
      // 
      // menu_File_OpenGeometry
      // 
      this.menu_File_OpenGeometry.Name = "menu_File_OpenGeometry";
      this.menu_File_OpenGeometry.Size = new System.Drawing.Size(135, 22);
      this.menu_File_OpenGeometry.Text = "Geometry...";
      this.menu_File_OpenGeometry.Click += new System.EventHandler(this.FileOpenGeometry);
      // 
      // menu_File_OpenMap
      // 
      this.menu_File_OpenMap.Name = "menu_File_OpenMap";
      this.menu_File_OpenMap.Size = new System.Drawing.Size(135, 22);
      this.menu_File_OpenMap.Text = "Map...";
      this.menu_File_OpenMap.Click += new System.EventHandler(this.FileOpenMap);
      // 
      // separator_file1
      // 
      this.separator_file1.Name = "separator_file1";
      this.separator_file1.Size = new System.Drawing.Size(149, 6);
      // 
      // menu_File_Save
      // 
      this.menu_File_Save.Image = ((System.Drawing.Image)(resources.GetObject("saveToolStripMenuItem.Image")));
      this.menu_File_Save.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.menu_File_Save.Name = "menu_File_Save";
      this.menu_File_Save.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.S)));
      this.menu_File_Save.Size = new System.Drawing.Size(152, 22);
      this.menu_File_Save.Text = "&Save";
      this.menu_File_Save.Click += new System.EventHandler(this.FileSaveGeometry);
      // 
      // menu_File_SaveAs
      // 
      this.menu_File_SaveAs.Name = "menu_File_SaveAs";
      this.menu_File_SaveAs.Size = new System.Drawing.Size(152, 22);
      this.menu_File_SaveAs.Text = "Save &As...";
      this.menu_File_SaveAs.Click += new System.EventHandler(this.FileSaveAsGeometry);
      // 
      // separator_file2
      // 
      this.separator_file2.Name = "separator_file2";
      this.separator_file2.Size = new System.Drawing.Size(149, 6);
      // 
      // menu_File_Print
      // 
      this.menu_File_Print.Image = ((System.Drawing.Image)(resources.GetObject("printToolStripMenuItem.Image")));
      this.menu_File_Print.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.menu_File_Print.Name = "menu_File_Print";
      this.menu_File_Print.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.P)));
      this.menu_File_Print.Size = new System.Drawing.Size(152, 22);
      this.menu_File_Print.Text = "&Print...";
      this.menu_File_Print.Click += new System.EventHandler(this.FilePrintMenu_Click);
      // 
      // menu_File_PrintPreview
      // 
      this.menu_File_PrintPreview.Image = ((System.Drawing.Image)(resources.GetObject("printPreviewToolStripMenuItem.Image")));
            this.menu_File_PrintPreview.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.menu_File_PrintPreview.Name = "menu_File_PrintPreview";
      this.menu_File_PrintPreview.Size = new System.Drawing.Size(152, 22);
      this.menu_File_PrintPreview.Text = "Print Pre&view...";
      this.menu_File_PrintPreview.Click += new System.EventHandler(this.FilePrintPreview);
      // 
      // separator_file3
      // 
      this.separator_file3.Name = "separator_file3";
      this.separator_file3.Size = new System.Drawing.Size(149, 6);
      // 
      // menu_File_Exit
      // 
      this.menu_File_Exit.Name = "menu_File_Exit";
      this.menu_File_Exit.Size = new System.Drawing.Size(152, 22);
      this.menu_File_Exit.Text = "E&xit";
      this.menu_File_Exit.Click += new System.EventHandler(this.menu_File_Exit_Click);
      // 
      // editMenu
      // 
      this.menu_Edit.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menu_Edit_Undo,
            this.menu_Edit_Backup,
            this.separator_edit1,
            this.menu_Edit_Copy,
            this.menu_Edit_Delete,
            this.menu_Edit_Clear});
      this.menu_Edit.Name = "editMenu";
      this.menu_Edit.Size = new System.Drawing.Size(39, 20);
      this.menu_Edit.Text = "&Edit";
      // 
      // menu_Edit_Undo
      // 
      this.menu_Edit_Undo.Image = global::CSharpAPP6ASample.Properties.Resources.undo;
      this.menu_Edit_Undo.Name = "menu_Edit_Undo";
      this.menu_Edit_Undo.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Z)));
      this.menu_Edit_Undo.Size = new System.Drawing.Size(144, 22);
      this.menu_Edit_Undo.Text = "&Undo";
      this.menu_Edit_Undo.Click += new System.EventHandler(this.menu_Edit_Undo_Click);
      // 
      // menu_Edit_Backup
      // 
      this.menu_Edit_Backup.Image = global::CSharpAPP6ASample.Properties.Resources.Backup;
      this.menu_Edit_Backup.Name = "menu_Edit_Backup";
      this.menu_Edit_Backup.Size = new System.Drawing.Size(144, 22);
      this.menu_Edit_Backup.Text = "Backup";
      this.menu_Edit_Backup.Click += new System.EventHandler(this.menu_Edit_Backup_Click);
      // 
      // separator_edit1
      // 
      this.separator_edit1.Name = "separator_edit1";
      this.separator_edit1.Size = new System.Drawing.Size(149, 6);
      // 
      // menu_Edit_Copy
      // 
      this.menu_Edit_Copy.Image = ((System.Drawing.Image)(resources.GetObject("copyToolStripMenuItem.Image")));
      this.menu_Edit_Copy.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.menu_Edit_Copy.Name = "menu_Edit_Copy";
      this.menu_Edit_Copy.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.C)));
      this.menu_Edit_Copy.Size = new System.Drawing.Size(144, 22);
      this.menu_Edit_Copy.Text = "&Copy";
      this.menu_Edit_Copy.Click += new System.EventHandler(this.menu_Edit_Copy_Click);
      // 
      // menu_Edit_Delete
      // 
      this.menu_Edit_Delete.Image = global::CSharpAPP6ASample.Properties.Resources.Delete;
      this.menu_Edit_Delete.Name = "menu_Edit_Delete";
      this.menu_Edit_Delete.ShortcutKeys = System.Windows.Forms.Keys.Delete;
      this.menu_Edit_Delete.Size = new System.Drawing.Size(144, 22);
      this.menu_Edit_Delete.Text = "Delete";
      this.menu_Edit_Delete.Click += new System.EventHandler(this.menu_Edit_Delete_Click);
      // 
      // menu_Edit_Clear
      // 
      this.menu_Edit_Clear.Name = "menu_Edit_Clear";
      this.menu_Edit_Clear.Size = new System.Drawing.Size(144, 22);
      this.menu_Edit_Clear.Text = "Clear";
      this.menu_Edit_Clear.Click += new System.EventHandler(this.menu_Edit_Clear_Click);
      // 
      // viewMenu
      // 
      this.menu_View.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menu_View_AttributesPanel,
            this.menu_View_PreferencesPanel,
            this.menu_View_Statusbar,
            this.separator_view1,
            this.menu_View_GridLayer,
            this.separator_view2,
            this.menu_View_Toolbars});
      this.menu_View.Name = "viewMenu";
      this.menu_View.Size = new System.Drawing.Size(44, 20);
      this.menu_View.Text = "&View";
      // 
      // menu_View_AttributesPanel
      // 
      this.menu_View_AttributesPanel.Checked = true;
      this.menu_View_AttributesPanel.CheckState = System.Windows.Forms.CheckState.Checked;
      this.menu_View_AttributesPanel.Name = "menu_View_AttributesPanel";
      this.menu_View_AttributesPanel.Size = new System.Drawing.Size(167, 22);
      this.menu_View_AttributesPanel.Text = "Attributes Panel";
      this.menu_View_AttributesPanel.Click += new System.EventHandler(this.menu_View_AttributesPanel_Click);
      // 
      // menu_View_PreferencesPanel
      // 
      this.menu_View_PreferencesPanel.Checked = true;
      this.menu_View_PreferencesPanel.CheckState = System.Windows.Forms.CheckState.Checked;
      this.menu_View_PreferencesPanel.Name = "menu_View_PreferencesPanel";
      this.menu_View_PreferencesPanel.Size = new System.Drawing.Size(167, 22);
      this.menu_View_PreferencesPanel.Text = "Preferences Panel";
      this.menu_View_PreferencesPanel.Click += new System.EventHandler(this.menu_View_PreferencesPanel_Click);
      // 
      // menu_View_Statusbar
      // 
      this.menu_View_Statusbar.Checked = true;
      this.menu_View_Statusbar.CheckState = System.Windows.Forms.CheckState.Checked;
      this.menu_View_Statusbar.Name = "menu_View_Statusbar";
      this.menu_View_Statusbar.Size = new System.Drawing.Size(167, 22);
      this.menu_View_Statusbar.Text = "Status Bar";
      this.menu_View_Statusbar.Click += new System.EventHandler(this.menu_View_Statusbar_Click);
      // 
      // separator_view1
      // 
      this.separator_view1.Name = "separator_view1";
      this.separator_view1.Size = new System.Drawing.Size(164, 6);
      // 
      // menu_View_GridLayer
      // 
      this.menu_View_GridLayer.Name = "menu_View_GridLayer";
      this.menu_View_GridLayer.Size = new System.Drawing.Size(167, 22);
      this.menu_View_GridLayer.Text = "Grid Layer";
      this.menu_View_GridLayer.Click += new System.EventHandler(this.menu_View_GridLayer_Click);
      // 
      // separator_view2
      // 
      this.separator_view2.Name = "separator_view2";
      this.separator_view2.Size = new System.Drawing.Size(164, 6);
      // 
      // menu_View_Toolbars
      // 
      this.menu_View_Toolbars.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menu_View_ToolbarsMain,
            this.menu_View_ToolbarsEdit,
            this.menu_View_ToolbarsSelect,
            this.menu_View_ToolbarsTools,
            this.menu_View_ToolbarsConstraints,
            this.menu_View_ToolbarsPrimitives,
            this.menu_View_ToolbarsTransforms,
            this.menu_View_ToolbarsPointOperations});
      this.menu_View_Toolbars.Name = "menu_View_Toolbars";
      this.menu_View_Toolbars.Size = new System.Drawing.Size(167, 22);
      this.menu_View_Toolbars.Text = "Toolbars";
      // 
      // menu_View_ToolbarsMain
      // 
      this.menu_View_ToolbarsMain.Checked = true;
      this.menu_View_ToolbarsMain.CheckState = System.Windows.Forms.CheckState.Checked;
      this.menu_View_ToolbarsMain.Name = "menu_View_ToolbarsMain";
      this.menu_View_ToolbarsMain.Size = new System.Drawing.Size(163, 22);
      this.menu_View_ToolbarsMain.Text = "Main";
      this.menu_View_ToolbarsMain.Click += new System.EventHandler(this.menu_View_ToolbarsMain_Click);
      // 
      // menu_View_ToolbarsEdit
      // 
      this.menu_View_ToolbarsEdit.Checked = true;
      this.menu_View_ToolbarsEdit.CheckState = System.Windows.Forms.CheckState.Checked;
      this.menu_View_ToolbarsEdit.Name = "menu_View_ToolbarsEdit";
      this.menu_View_ToolbarsEdit.Size = new System.Drawing.Size(163, 22);
      this.menu_View_ToolbarsEdit.Text = "Edit";
      this.menu_View_ToolbarsEdit.Click += new System.EventHandler(this.menu_View_ToolbarsEdit_Click);
      // 
      // menu_View_ToolbarsSelect
      // 
      this.menu_View_ToolbarsSelect.Checked = true;
      this.menu_View_ToolbarsSelect.CheckState = System.Windows.Forms.CheckState.Checked;
      this.menu_View_ToolbarsSelect.Name = "menu_View_ToolbarsSelect";
      this.menu_View_ToolbarsSelect.Size = new System.Drawing.Size(163, 22);
      this.menu_View_ToolbarsSelect.Text = "Select";
      this.menu_View_ToolbarsSelect.Click += new System.EventHandler(this.menu_View_ToolbarsSelect_Click);
      // 
      // menu_View_ToolbarsTools
      // 
      this.menu_View_ToolbarsTools.Checked = true;
      this.menu_View_ToolbarsTools.CheckState = System.Windows.Forms.CheckState.Checked;
      this.menu_View_ToolbarsTools.Name = "menu_View_ToolbarsTools";
      this.menu_View_ToolbarsTools.Size = new System.Drawing.Size(163, 22);
      this.menu_View_ToolbarsTools.Text = "Tools";
      this.menu_View_ToolbarsTools.Click += new System.EventHandler(this.menu_View_ToolbarsTools_Click);
      // 
      // menu_View_ToolbarsConstraints
      // 
      this.menu_View_ToolbarsConstraints.Checked = true;
      this.menu_View_ToolbarsConstraints.CheckState = System.Windows.Forms.CheckState.Checked;
      this.menu_View_ToolbarsConstraints.Name = "menu_View_ToolbarsConstraints";
      this.menu_View_ToolbarsConstraints.Size = new System.Drawing.Size(163, 22);
      this.menu_View_ToolbarsConstraints.Text = "Constraints";
      this.menu_View_ToolbarsConstraints.Click += new System.EventHandler(this.menu_View_ToolbarsConstraints_Click);
      // 
      // menu_View_ToolbarsPrimitives
      // 
      this.menu_View_ToolbarsPrimitives.Checked = true;
      this.menu_View_ToolbarsPrimitives.CheckState = System.Windows.Forms.CheckState.Checked;
      this.menu_View_ToolbarsPrimitives.Name = "menu_View_ToolbarsPrimitives";
      this.menu_View_ToolbarsPrimitives.Size = new System.Drawing.Size(163, 22);
      this.menu_View_ToolbarsPrimitives.Text = "Primitives";
      this.menu_View_ToolbarsPrimitives.Click += new System.EventHandler(this.menu_View_ToolbarsPrimitives_Click);
      // 
      // menu_View_ToolbarsTransforms
      // 
      this.menu_View_ToolbarsTransforms.Checked = true;
      this.menu_View_ToolbarsTransforms.CheckState = System.Windows.Forms.CheckState.Checked;
      this.menu_View_ToolbarsTransforms.Name = "menu_View_ToolbarsTransforms";
      this.menu_View_ToolbarsTransforms.Size = new System.Drawing.Size(163, 22);
      this.menu_View_ToolbarsTransforms.Text = "Transforms";
      this.menu_View_ToolbarsTransforms.Click += new System.EventHandler(this.menu_View_ToolbarsTransforms_Click);
      // 
      // menu_View_ToolbarsPointOperations
      // 
      this.menu_View_ToolbarsPointOperations.Checked = true;
      this.menu_View_ToolbarsPointOperations.CheckState = System.Windows.Forms.CheckState.Checked;
      this.menu_View_ToolbarsPointOperations.Name = "menu_View_ToolbarsPointOperations";
      this.menu_View_ToolbarsPointOperations.Size = new System.Drawing.Size(163, 22);
      this.menu_View_ToolbarsPointOperations.Text = "Point Operations";
      this.menu_View_ToolbarsPointOperations.Click += new System.EventHandler(this.menu_View_ToolbarsPointOperations_Click);
      // 
      // selectMenu
      // 
      this.menu_Select.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menu_Select_All,
            this.menu_Select_ByRectangle,
            this.menu_Select_ByPolygon});
      this.menu_Select.Name = "selectMenu";
      this.menu_Select.Size = new System.Drawing.Size(50, 20);
      this.menu_Select.Text = "Select";
      // 
      // menu_Select_All
      // 
      this.menu_Select_All.Image = global::CSharpAPP6ASample.Properties.Resources.Select_All;
      this.menu_Select_All.Name = "menu_Select_All";
      this.menu_Select_All.Size = new System.Drawing.Size(176, 22);
      this.menu_Select_All.Text = "Select All";
      this.menu_Select_All.Click += new System.EventHandler(this.menu_Select_All_Click);
      // 
      // menu_Select_ByRectangle
      // 
      this.menu_Select_ByRectangle.Image = global::CSharpAPP6ASample.Properties.Resources.Select_by_Rectangle;
      this.menu_Select_ByRectangle.Name = "menu_Select_ByRectangle";
      this.menu_Select_ByRectangle.Size = new System.Drawing.Size(176, 22);
      this.menu_Select_ByRectangle.Text = "Select By Rectangle";
      this.menu_Select_ByRectangle.Click += new System.EventHandler(this.menu_Select_ByRectangle_Click);
      // 
      // menu_Select_ByPolygon
      // 
      this.menu_Select_ByPolygon.Image = global::CSharpAPP6ASample.Properties.Resources.Select_by_Polygon;
      this.menu_Select_ByPolygon.Name = "menu_Select_ByPolygon";
      this.menu_Select_ByPolygon.Size = new System.Drawing.Size(176, 22);
      this.menu_Select_ByPolygon.Text = "Select By Polygon";
      this.menu_Select_ByPolygon.Click += new System.EventHandler(this.menu_Select_ByPolygon_Click);
      // 
      // toolsMenu
      // 
      this.menu_Tools.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menu_Tools_ZoomIn,
            this.menu_Tools_ZoomOut,
            this.menu_Tools_Reset,
            this.separator_tools,
            this.menu_Tools_Zoommode,
            this.menu_Tools_Panmode,
            this.menu_Tools_Grabmode,
            this.menu_Tools_Zoompanmode,
            this.menu_Tools_Editmode});
      this.menu_Tools.Name = "toolsMenu";
      this.menu_Tools.Size = new System.Drawing.Size(46, 20);
      this.menu_Tools.Text = "&Tools";
      // 
      // menu_Tools_ZoomIn
      // 
      this.menu_Tools_ZoomIn.Image = global::CSharpAPP6ASample.Properties.Resources.zoom_in;
      this.menu_Tools_ZoomIn.Name = "menu_Tools_ZoomIn";
      this.menu_Tools_ZoomIn.Size = new System.Drawing.Size(137, 22);
      this.menu_Tools_ZoomIn.Text = "Zoom In";
      this.menu_Tools_ZoomIn.Click += new System.EventHandler(this.Imode_ButtonClick);
      // 
      // menu_Tools_ZoomOut
      // 
      this.menu_Tools_ZoomOut.Image = global::CSharpAPP6ASample.Properties.Resources.zoom_out;
      this.menu_Tools_ZoomOut.Name = "menu_Tools_ZoomOut";
      this.menu_Tools_ZoomOut.Size = new System.Drawing.Size(137, 22);
      this.menu_Tools_ZoomOut.Text = "Zoom Out";
      this.menu_Tools_ZoomOut.Click += new System.EventHandler(this.Imode_ButtonClick);
      // 
      // menu_Tools_Reset
      // 
      this.menu_Tools_Reset.Image = global::CSharpAPP6ASample.Properties.Resources.Reset;
      this.menu_Tools_Reset.Name = "menu_Tools_Reset";
      this.menu_Tools_Reset.Size = new System.Drawing.Size(137, 22);
      this.menu_Tools_Reset.Text = "Reset";
      this.menu_Tools_Reset.Click += new System.EventHandler(this.Imode_ButtonClick);
      // 
      // menu_Tools_Zoommode
      // 
      this.menu_Tools_Zoommode.Image = global::CSharpAPP6ASample.Properties.Resources.Zoom;
      this.menu_Tools_Zoommode.Name = "menu_Tools_Zoommode";
      this.menu_Tools_Zoommode.Size = new System.Drawing.Size(137, 22);
      this.menu_Tools_Zoommode.Text = "Zoom";
      this.menu_Tools_Zoommode.Click += new System.EventHandler(this.Imode_ButtonClick);
      // 
      // menu_Tools_Panmode
      // 
      this.menu_Tools_Panmode.Image = global::CSharpAPP6ASample.Properties.Resources.Pan;
      this.menu_Tools_Panmode.Name = "menu_Tools_Panmode";
      this.menu_Tools_Panmode.Size = new System.Drawing.Size(137, 22);
      this.menu_Tools_Panmode.Text = "Pan";
      this.menu_Tools_Panmode.Click += new System.EventHandler(this.Imode_ButtonClick);
      // 
      // menu_Tools_Grabmode
      // 
      this.menu_Tools_Grabmode.Image = global::CSharpAPP6ASample.Properties.Resources.Grab;
      this.menu_Tools_Grabmode.Name = "menu_Tools_Grabmode";
      this.menu_Tools_Grabmode.Size = new System.Drawing.Size(137, 22);
      this.menu_Tools_Grabmode.Text = "Grab";
      this.menu_Tools_Grabmode.Click += new System.EventHandler(this.Imode_ButtonClick);
      // 
      // menu_Tools_Zoompanmode
      // 
      this.menu_Tools_Zoompanmode.Image = global::CSharpAPP6ASample.Properties.Resources.Zoom___Pan;
      this.menu_Tools_Zoompanmode.Name = "menu_Tools_Zoompanmode";
      this.menu_Tools_Zoompanmode.Size = new System.Drawing.Size(137, 22);
      this.menu_Tools_Zoompanmode.Text = "Zoom / Pan";
      this.menu_Tools_Zoompanmode.Click += new System.EventHandler(this.Imode_ButtonClick);
      // 
      // menu_Tools_Editmode
      // 
      this.menu_Tools_Editmode.Checked = true;
      this.menu_Tools_Editmode.CheckState = System.Windows.Forms.CheckState.Checked;
      this.menu_Tools_Editmode.Image = global::CSharpAPP6ASample.Properties.Resources.Edit_Mode;
      this.menu_Tools_Editmode.Name = "menu_Tools_Editmode";
      this.menu_Tools_Editmode.Size = new System.Drawing.Size(137, 22);
      this.menu_Tools_Editmode.Text = "Edit";
      this.menu_Tools_Editmode.Click += new System.EventHandler(this.Imode_ButtonClick);
      // 
      // menu_aPP6ATool
      // 
      this.menu_aPP6ATool.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menu_aPP6ATool_palette,
            this.menu_aPP6ATool_properties,
            this.menu_aPP6ATool_selector,
            toolStripSeparator18,
            this.menu_aPP6ATool_declutterOptions});
      this.menu_aPP6ATool.Name = "aPP6AToolMenu";
      this.menu_aPP6ATool.Size = new System.Drawing.Size(139, 22);
      this.menu_aPP6ATool.Text = "APP6A";
      // 
      // menu_aPP6ATool_palette 
      // 
      this.menu_aPP6ATool_palette.Name = "menu_aPP6ATool_palette ";
      this.menu_aPP6ATool_palette.Size = new System.Drawing.Size(169, 22);
      this.menu_aPP6ATool_palette.Text = "Palette";
      this.menu_aPP6ATool_palette.Click += new System.EventHandler(this.menu_aPP6ATool_Palette_Click);
      // 
      // menu_aPP6ATool_properties
      // 
      this.menu_aPP6ATool_properties.Name = "menu_aPP6ATool_properties";
      this.menu_aPP6ATool_properties.Size = new System.Drawing.Size(169, 22);
      this.menu_aPP6ATool_properties.Text = "Properties";
      this.menu_aPP6ATool_properties.Click += new System.EventHandler(this.menu_aPP6ATool_Properties_Click);
      // 
      // menu_aPP6ATool_selector
      // 
      this.menu_aPP6ATool_selector.Name = "menu_aPP6ATool_selector";
      this.menu_aPP6ATool_selector.Size = new System.Drawing.Size(169, 22);
      this.menu_aPP6ATool_selector.Text = "Selector";
      this.menu_aPP6ATool_selector.Click += new System.EventHandler(this.menu_aPP6ATool_Selector_Click);
      // 
      // menu_aPP6ATool_declutterOptions
      // 
      this.menu_aPP6ATool_declutterOptions.Name = "menu_aPP6ATool_declutterOptions";
      this.menu_aPP6ATool_declutterOptions.Size = new System.Drawing.Size(169, 22);
      this.menu_aPP6ATool_declutterOptions.Text = "Declutter Options";
      this.menu_aPP6ATool_declutterOptions.Click += new System.EventHandler(this.menu_aPP6ATool_DeclutterOptions_Click);
      // 
      // constraintsMenu
      // 
      this.menu_Constraints.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menu_Constraints_Horizontal,
            this.menu_Constraints_Vertical,
            this.menu_Constraints_Equal,
            this.menu_Constraints_Unequal,
            this.menu_Constraints_Angular,
            this.menu_Constraints_Distance});
      this.menu_Constraints.Name = "constraintsMenu";
      this.menu_Constraints.Size = new System.Drawing.Size(79, 20);
      this.menu_Constraints.Text = "Constraints";
      // 
      // menu_Constraints_Horizontal
      // 
      this.menu_Constraints_Horizontal.Image = global::CSharpAPP6ASample.Properties.Resources.Constraint_Horizontal;
      this.menu_Constraints_Horizontal.ImageTransparentColor = System.Drawing.Color.Silver;
      this.menu_Constraints_Horizontal.Name = "menu_Constraints_Horizontal";
      this.menu_Constraints_Horizontal.Size = new System.Drawing.Size(129, 22);
      this.menu_Constraints_Horizontal.Text = "Horizontal";
      this.menu_Constraints_Horizontal.Click += new System.EventHandler(this.menu_Constraints_Horizontal_Click);
      // 
      // menu_Constraints_Vertical
      // 
      this.menu_Constraints_Vertical.Image = global::CSharpAPP6ASample.Properties.Resources.Constraint_Vertical;
      this.menu_Constraints_Vertical.ImageTransparentColor = System.Drawing.Color.Silver;
      this.menu_Constraints_Vertical.Name = "menu_Constraints_Vertical";
      this.menu_Constraints_Vertical.Size = new System.Drawing.Size(129, 22);
      this.menu_Constraints_Vertical.Text = "Vertical";
      this.menu_Constraints_Vertical.Click += new System.EventHandler(this.menu_Constraints_Vertical_Click);
      // 
      // menu_Constraints_Equal
      // 
      this.menu_Constraints_Equal.Image = global::CSharpAPP6ASample.Properties.Resources.Constraint_Equal;
      this.menu_Constraints_Equal.ImageTransparentColor = System.Drawing.Color.Silver;
      this.menu_Constraints_Equal.Name = "menu_Constraints_Equal";
      this.menu_Constraints_Equal.Size = new System.Drawing.Size(129, 22);
      this.menu_Constraints_Equal.Text = "Equal";
      this.menu_Constraints_Equal.Click += new System.EventHandler(this.menu_Constraints_Equal_Click);
      // 
      // menu_Constraints_Unequal
      // 
      this.menu_Constraints_Unequal.Checked = true;
      this.menu_Constraints_Unequal.CheckState = System.Windows.Forms.CheckState.Checked;
      this.menu_Constraints_Unequal.Image = global::CSharpAPP6ASample.Properties.Resources.Constraint_Unequal;
      this.menu_Constraints_Unequal.ImageTransparentColor = System.Drawing.Color.Silver;
      this.menu_Constraints_Unequal.Name = "menu_Constraints_Unequal";
      this.menu_Constraints_Unequal.Size = new System.Drawing.Size(129, 22);
      this.menu_Constraints_Unequal.Text = "Unequal";
      this.menu_Constraints_Unequal.Click += new System.EventHandler(this.menu_Constraints_Unequal_Click);
      // 
      // menu_Constraints_Angular
      // 
      this.menu_Constraints_Angular.Image = global::CSharpAPP6ASample.Properties.Resources.Constraint_Angular;
      this.menu_Constraints_Angular.ImageTransparentColor = System.Drawing.Color.Silver;
      this.menu_Constraints_Angular.Name = "menu_Constraints_Angular";
      this.menu_Constraints_Angular.Size = new System.Drawing.Size(129, 22);
      this.menu_Constraints_Angular.Text = "Angular";
      this.menu_Constraints_Angular.Click += new System.EventHandler(this.menu_Constraints_Angular_Click);
      // 
      // menu_Constraints_Distance
      // 
      this.menu_Constraints_Distance.Image = global::CSharpAPP6ASample.Properties.Resources.Constraint_Distance;
      this.menu_Constraints_Distance.ImageTransparentColor = System.Drawing.Color.Silver;
      this.menu_Constraints_Distance.Name = "menu_Constraints_Distance";
      this.menu_Constraints_Distance.Size = new System.Drawing.Size(129, 22);
      this.menu_Constraints_Distance.Text = "Distance";
      this.menu_Constraints_Distance.Click += new System.EventHandler(this.menu_Constraints_Distance_Click);
      // 
      // primitivesMenu
      // 
      this.menu_Primitives.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menu_Primitives_Polygon,
            this.menu_Primitives_Polyline,
            this.menu_Primitives_Point,
            this.menu_Primitives_Circle,
            this.menu_Primitives_Text,
            this.separator_primitives1,
            this.menu_Primitives_ChangeText});
      this.menu_Primitives.Name = "primitivesMenu";
      this.menu_Primitives.Size = new System.Drawing.Size(71, 20);
      this.menu_Primitives.Text = "Primitives";
      // 
      // menu_Primitives_Polygon
      // 
      this.menu_Primitives_Polygon.Image = global::CSharpAPP6ASample.Properties.Resources.Polygon;
      this.menu_Primitives_Polygon.Name = "menu_Primitives_Polygon";
      this.menu_Primitives_Polygon.Size = new System.Drawing.Size(139, 22);
      this.menu_Primitives_Polygon.Text = "Polygon";
      this.menu_Primitives_Polygon.Click += new System.EventHandler(this.menu_Primitives_Polygon_Click);
      // 
      // menu_Primitives_Polyline
      // 
      this.menu_Primitives_Polyline.Image = global::CSharpAPP6ASample.Properties.Resources.Polyline;
      this.menu_Primitives_Polyline.Name = "menu_Primitives_Polyline";
      this.menu_Primitives_Polyline.Size = new System.Drawing.Size(139, 22);
      this.menu_Primitives_Polyline.Text = "Polyline";
      this.menu_Primitives_Polyline.Click += new System.EventHandler(this.menu_Primitives_Polyline_Click);
      // 
      // menu_Primitives_Point
      // 
      this.menu_Primitives_Point.Image = global::CSharpAPP6ASample.Properties.Resources.Point;
      this.menu_Primitives_Point.Name = "menu_Primitives_Point";
      this.menu_Primitives_Point.Size = new System.Drawing.Size(139, 22);
      this.menu_Primitives_Point.Text = "Marker";
      this.menu_Primitives_Point.Click += new System.EventHandler(this.menu_Primitives_Point_Click);
      //
      // menu_Primitives_Circle
      // 
      this.menu_Primitives_Circle.Name = "menu_Primitives_Circle";
      this.menu_Primitives_Circle.Size = new System.Drawing.Size(139, 22);
      this.menu_Primitives_Circle.Text = "Circle";
      this.menu_Primitives_Circle.Click += new System.EventHandler(this.menu_Primitives_Circle_Click);
      // 
      // menu_Primitives_Text
      // 
      this.menu_Primitives_Text.Image = global::CSharpAPP6ASample.Properties.Resources.Text;
      this.menu_Primitives_Text.Name = "menu_Primitives_Text";
      this.menu_Primitives_Text.Size = new System.Drawing.Size(139, 22);
      this.menu_Primitives_Text.Text = "Text";
      this.menu_Primitives_Text.Click += new System.EventHandler(this.menu_Primitives_Text_Click);
      // 
      // menu_Primitives_ChangeText
      // 
      this.menu_Primitives_ChangeText.Image = global::CSharpAPP6ASample.Properties.Resources.Edit_Text;
      this.menu_Primitives_ChangeText.Name = "menu_Primitives_ChangeText";
      this.menu_Primitives_ChangeText.Size = new System.Drawing.Size(139, 22);
      this.menu_Primitives_ChangeText.Text = "Change Text";
      this.menu_Primitives_ChangeText.Click += new System.EventHandler(this.menu_Primitives_ChangeText_Click);
      // 
      // transformationMenu
      // 
      this.menu_Transformation.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menu_Transformation_Move,
            this.menu_Transformation_Scale,
            this.menu_Transformation_Rotate,
            this.separator_transformation1,
            this.menu_Transformation_AddPoint,
            this.menu_Transformation_DeletePoint,
            this.menu_Transformation_MovePoint,
            this.separator_transformation2,
            this.menu_Transformation_Group,
            this.menu_Transformation_Ungroup,
            this.separator_transformation3,
            this.menu_Transformation_Front,
            this.menu_Transformation_Back});
      this.menu_Transformation.Name = "transformationMenu";
      this.menu_Transformation.Size = new System.Drawing.Size(99, 20);
      this.menu_Transformation.Text = "Transformation";
      // 
      // menu_Transformation_Move
      // 
      this.menu_Transformation_Move.Image = global::CSharpAPP6ASample.Properties.Resources.Move;
      this.menu_Transformation_Move.Name = "menu_Transformation_Move";
      this.menu_Transformation_Move.Size = new System.Drawing.Size(138, 22);
      this.menu_Transformation_Move.Text = "Move";
      this.menu_Transformation_Move.Click += new System.EventHandler(this.menu_Transformation_Move_Click);
      // 
      // menu_Transformation_Scale
      // 
      this.menu_Transformation_Scale.Image = global::CSharpAPP6ASample.Properties.Resources.Scale;
      this.menu_Transformation_Scale.Name = "menu_Transformation_Scale";
      this.menu_Transformation_Scale.Size = new System.Drawing.Size(138, 22);
      this.menu_Transformation_Scale.Text = "Scale";
      this.menu_Transformation_Scale.Click += new System.EventHandler(this.menu_Transformation_Scale_Click);
      // 
      // menu_Transformation_Rotate
      // 
      this.menu_Transformation_Rotate.Image = global::CSharpAPP6ASample.Properties.Resources.Rotate;
      this.menu_Transformation_Rotate.ImageTransparentColor = System.Drawing.Color.White;
      this.menu_Transformation_Rotate.Name = "menu_Transformation_Rotate";
      this.menu_Transformation_Rotate.Size = new System.Drawing.Size(138, 22);
      this.menu_Transformation_Rotate.Text = "Rotate";
      this.menu_Transformation_Rotate.Click += new System.EventHandler(this.menu_Transformation_Rotate_Click);
      // 
      // menu_Transformation_AddPoint
      // 
      this.menu_Transformation_AddPoint.Image = global::CSharpAPP6ASample.Properties.Resources.Add_Point;
      this.menu_Transformation_AddPoint.Name = "menu_Transformation_AddPoint";
      this.menu_Transformation_AddPoint.Size = new System.Drawing.Size(138, 22);
      this.menu_Transformation_AddPoint.Text = "Add Point";
      this.menu_Transformation_AddPoint.Click += new System.EventHandler(this.menu_Transformation_AddPoint_Click);
      // 
      // menu_Transformation_DeletePoint
      // 
      this.menu_Transformation_DeletePoint.Image = global::CSharpAPP6ASample.Properties.Resources.Delete_Point;
      this.menu_Transformation_DeletePoint.Name = "menu_Transformation_DeletePoint";
      this.menu_Transformation_DeletePoint.Size = new System.Drawing.Size(138, 22);
      this.menu_Transformation_DeletePoint.Text = "Delete Point";
      this.menu_Transformation_DeletePoint.Click += new System.EventHandler(this.menu_Transformation_DeletePoint_Click);
      // 
      // menu_Transformation_MovePoint
      // 
      this.menu_Transformation_MovePoint.Image = global::CSharpAPP6ASample.Properties.Resources.Move_Point;
      this.menu_Transformation_MovePoint.Name = "menu_Transformation_MovePoint";
      this.menu_Transformation_MovePoint.Size = new System.Drawing.Size(138, 22);
      this.menu_Transformation_MovePoint.Text = "Move Point";
      this.menu_Transformation_MovePoint.Click += new System.EventHandler(this.menu_Transformation_MovePoint_Click);
      // 
      // menu_Transformation_Group
      // 
      this.menu_Transformation_Group.Image = global::CSharpAPP6ASample.Properties.Resources.Group;
      this.menu_Transformation_Group.Name = "menu_Transformation_Group";
      this.menu_Transformation_Group.Size = new System.Drawing.Size(138, 22);
      this.menu_Transformation_Group.Text = "Group";
      this.menu_Transformation_Group.ToolTipText = "Group a set of entities";
      this.menu_Transformation_Group.Click += new System.EventHandler(this.menu_Transformation_Group_Click);
      // 
      // menu_Transformation_Ungroup
      // 
      this.menu_Transformation_Ungroup.Image = global::CSharpAPP6ASample.Properties.Resources.Ungroup;
      this.menu_Transformation_Ungroup.Name = "menu_Transformation_Ungroup";
      this.menu_Transformation_Ungroup.Size = new System.Drawing.Size(138, 22);
      this.menu_Transformation_Ungroup.Text = "Ungroup";
      this.menu_Transformation_Ungroup.ToolTipText = "Ungroup a set of entities";
      this.menu_Transformation_Ungroup.Click += new System.EventHandler(this.menu_Transformation_Ungroup_Click);
      // 
      // menu_Transformation_Front
      // 
      this.menu_Transformation_Front.Image = global::CSharpAPP6ASample.Properties.Resources.Bring_to_Front;
      this.menu_Transformation_Front.Name = "menu_Transformation_Front";
      this.menu_Transformation_Front.Size = new System.Drawing.Size(138, 22);
      this.menu_Transformation_Front.Text = "Front";
      this.menu_Transformation_Front.ToolTipText = "Bring an entity to the front";
      this.menu_Transformation_Front.Click += new System.EventHandler(this.menu_Transformation_Front_Click);
      // 
      // menu_Transformation_Back
      // 
      this.menu_Transformation_Back.Image = global::CSharpAPP6ASample.Properties.Resources.Send_to_Back;
      this.menu_Transformation_Back.Name = "menu_Transformation_Back";
      this.menu_Transformation_Back.Size = new System.Drawing.Size(138, 22);
      this.menu_Transformation_Back.Text = "Back";
      this.menu_Transformation_Back.ToolTipText = "Send an entity to the back";
      this.menu_Transformation_Back.Click += new System.EventHandler(this.menu_Transformation_Back_Click);
      // 
      // helpMenu
      // 
      this.menu_Help.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menu_Help_About});
      this.menu_Help.Name = "helpMenu";
      this.menu_Help.Size = new System.Drawing.Size(44, 20);
      this.menu_Help.Text = "&Help";
      // 
      // menu_Help_About
      // 
      this.menu_Help_About.Image = ((System.Drawing.Image)(resources.GetObject("helpAboutMenuItem.Image")));
      this.menu_Help_About.Name = "menu_Help_About";
      this.menu_Help_About.Size = new System.Drawing.Size(152, 22);
      this.menu_Help_About.Text = "&About...";
      this.menu_Help_About.Click += new System.EventHandler(this.OnAppAbout);
      // 
      // FileOpenTMFDialog
      // 
      this.FileOpenTMFDialog.DefaultExt = "tmf";
      this.FileOpenTMFDialog.Filter = "MapLink Geometry Files|*.tmf|All files|*.*";
      this.FileOpenTMFDialog.ReadOnlyChecked = true;
      this.FileOpenTMFDialog.Title = "Open a MapLink Geometry File";
      // 
      // FileSaveTMFDialog
      // 
      this.FileSaveTMFDialog.DefaultExt = "tmf";
      this.FileSaveTMFDialog.Filter = "MapLink Geometry Files|*.tmf|All files|*.*";
      this.FileSaveTMFDialog.Title = "Save As MapLink Geometry File";
      // 
      // MapViewerForm
      // 
      this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
      this.ClientSize = new System.Drawing.Size(953, 727);
      this.Controls.Add(this.toolStripContainer1);
      this.Controls.Add(this.TheMenu);
      this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
      this.IsMdiContainer = true;
      this.MainMenuStrip = this.TheMenu;
      this.Name = "MapViewerForm";
      this.Text = "C# .NET APP6A Sample";
      this.KeyUp += new System.Windows.Forms.KeyEventHandler(this.OnKeyUp);
      this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.OnKeyDown);
      this.toolStripContainer1.BottomToolStripPanel.ResumeLayout(false);
      this.toolStripContainer1.BottomToolStripPanel.PerformLayout();
      this.toolStripContainer1.ContentPanel.ResumeLayout(false);
      this.toolStripContainer1.LeftToolStripPanel.ResumeLayout(false);
      this.toolStripContainer1.LeftToolStripPanel.PerformLayout();
      this.toolStripContainer1.TopToolStripPanel.ResumeLayout(false);
      this.toolStripContainer1.TopToolStripPanel.PerformLayout();
      this.toolStripContainer1.ResumeLayout(false);
      this.toolStripContainer1.PerformLayout();
      this.TheStatusStrip.ResumeLayout(false);
      this.TheStatusStrip.PerformLayout();
      this.AttributesToolStrip.ResumeLayout(false);
      this.AttributesToolStrip.PerformLayout();
      this.PreferencesToolStrip.ResumeLayout(false);
      this.PreferencesToolStrip.PerformLayout();
      this.PrimitiveToolStrip.ResumeLayout(false);
      this.PrimitiveToolStrip.PerformLayout();
      this.PointToolStrip.ResumeLayout(false);
      this.PointToolStrip.PerformLayout();
      this.TransformsToolStrip.ResumeLayout(false);
      this.TransformsToolStrip.PerformLayout();
      this.ConstraintsToolStrip.ResumeLayout(false);
      this.ConstraintsToolStrip.PerformLayout();
      this.ToolsToolStrip.ResumeLayout(false);
      this.ToolsToolStrip.PerformLayout();
      this.SelectToolStrip.ResumeLayout(false);
      this.SelectToolStrip.PerformLayout();
      this.MainToolStrip.ResumeLayout(false);
      this.MainToolStrip.PerformLayout();
      this.EditToolStrip.ResumeLayout(false);
      this.EditToolStrip.PerformLayout();
      this.TheMenu.ResumeLayout(false);
      this.TheMenu.PerformLayout();
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    
    #endregion

    private System.Windows.Forms.OpenFileDialog FileOpenMapDialog;
    private MapViewerPanel ViewerPanel;
    private System.Windows.Forms.ToolStripContainer toolStripContainer1;
    private DockableAttributePanel AttributePanel;
    private System.Windows.Forms.ToolStrip PreferencesToolStrip;
    private DockablePreferencesPanel PreferencesPanel;
    private System.Windows.Forms.ToolStripButton btn_new;
    private System.Windows.Forms.ToolStripButton btn_save;
    private System.Windows.Forms.ToolStripButton btn_print;
    private System.Windows.Forms.ToolStripButton btn_about;
    private System.Windows.Forms.MenuStrip TheMenu;
    private System.Windows.Forms.ToolStripMenuItem menu_File;
    private System.Windows.Forms.ToolStripMenuItem menu_File_New;
    private System.Windows.Forms.ToolStripMenuItem menu_File_Open;
    private System.Windows.Forms.ToolStripSeparator separator_file1;
    private System.Windows.Forms.ToolStripMenuItem menu_File_Save;
    private System.Windows.Forms.ToolStripMenuItem menu_File_SaveAs;
    private System.Windows.Forms.ToolStripSeparator separator_file2;
    private System.Windows.Forms.ToolStripMenuItem menu_File_Print;
    private System.Windows.Forms.ToolStripMenuItem menu_File_PrintPreview;
    private System.Windows.Forms.ToolStripSeparator separator_file3;
    private System.Windows.Forms.ToolStripMenuItem menu_File_Exit;
    private System.Windows.Forms.ToolStripMenuItem menu_Edit;
    private System.Windows.Forms.ToolStripMenuItem menu_Edit_Undo;
    private System.Windows.Forms.ToolStripSeparator separator_edit1;
    private System.Windows.Forms.ToolStripMenuItem menu_Edit_Copy;
    private System.Windows.Forms.ToolStripMenuItem menu_Tools;
    private System.Windows.Forms.ToolStripMenuItem menu_Tools_ZoomIn;
    private System.Windows.Forms.ToolStripMenuItem menu_Tools_ZoomOut;
    private System.Windows.Forms.ToolStripMenuItem menu_Tools_Reset;
    private System.Windows.Forms.ToolStripMenuItem menu_Tools_Zoommode;
    private System.Windows.Forms.ToolStripMenuItem menu_Tools_Panmode;
    private System.Windows.Forms.ToolStripMenuItem menu_Tools_Grabmode;
    private System.Windows.Forms.ToolStripMenuItem menu_Tools_Zoompanmode;
    private System.Windows.Forms.ToolStripMenuItem menu_Help;
    private System.Windows.Forms.ToolStripMenuItem menu_Help_About;
    private System.Windows.Forms.ToolStrip ToolsToolStrip;
    private System.Windows.Forms.ToolStripButton btn_zoomIn;
    private System.Windows.Forms.ToolStripButton btn_zoomOut;
    private System.Windows.Forms.ToolStripButton btn_reset;
    private System.Windows.Forms.ToolStripButton btn_zoom;
    private System.Windows.Forms.ToolStripButton btn_pan;
    private System.Windows.Forms.ToolStripButton btn_zoomPan;
    private System.Windows.Forms.ToolStripMenuItem menu_Edit_Delete;
    private System.Windows.Forms.ToolStripMenuItem menu_Edit_Clear;
    private System.Windows.Forms.StatusStrip TheStatusStrip;
    private System.Windows.Forms.ToolStripMenuItem menu_Edit_Backup;
    private System.Windows.Forms.ToolStripMenuItem menu_View;
    private System.Windows.Forms.ToolStripMenuItem menu_View_AttributesPanel;
    private System.Windows.Forms.ToolStripMenuItem menu_View_PreferencesPanel;
    private System.Windows.Forms.ToolStripMenuItem menu_View_Statusbar;
    private System.Windows.Forms.ToolStripSeparator separator_view1;
    private System.Windows.Forms.ToolStripMenuItem menu_View_GridLayer;
    private System.Windows.Forms.ToolStripSeparator separator_view2;
    private System.Windows.Forms.ToolStripMenuItem menu_View_Toolbars;
    private System.Windows.Forms.ToolStripMenuItem menu_View_ToolbarsMain;
    private System.Windows.Forms.ToolStripMenuItem menu_View_ToolbarsEdit;
    private System.Windows.Forms.ToolStripMenuItem menu_View_ToolbarsSelect;
    private System.Windows.Forms.ToolStripMenuItem menu_View_ToolbarsTools;
    private System.Windows.Forms.ToolStripMenuItem menu_View_ToolbarsConstraints;
    private System.Windows.Forms.ToolStripMenuItem menu_View_ToolbarsPrimitives;
    private System.Windows.Forms.ToolStripMenuItem menu_View_ToolbarsTransforms;
    private System.Windows.Forms.ToolStripMenuItem menu_View_ToolbarsPointOperations;
    private System.Windows.Forms.ToolStrip EditToolStrip;
    private System.Windows.Forms.ToolStripButton btn_undo;
    private System.Windows.Forms.ToolStripButton btn_backup;
    private System.Windows.Forms.ToolStripSeparator toolStripSeparator7;
    private System.Windows.Forms.ToolStripButton btn_copy;
    private System.Windows.Forms.ToolStripButton btn_delete;
    private System.Windows.Forms.ToolStripButton btn_edit;
    private System.Windows.Forms.ToolStripButton btn_grab;
    private System.Windows.Forms.ToolStrip SelectToolStrip;
    private System.Windows.Forms.ToolStripMenuItem menu_Select;
    private System.Windows.Forms.ToolStripMenuItem menu_Select_All;
    private System.Windows.Forms.ToolStripMenuItem menu_Select_ByRectangle;
    private System.Windows.Forms.ToolStripMenuItem menu_Select_ByPolygon;
    private System.Windows.Forms.ToolStripButton btn_selectAll;
    private System.Windows.Forms.ToolStripButton btn_selectByRectangle;
    private System.Windows.Forms.ToolStripButton btn_selectByPolygon;
    private System.Windows.Forms.ToolStrip TransformsToolStrip;
    private System.Windows.Forms.ToolStripButton btn_move;
    private System.Windows.Forms.ToolStripButton btn_scale;
    private System.Windows.Forms.ToolStripButton btn_rotate;
    private System.Windows.Forms.ToolStripButton btn_group;
    private System.Windows.Forms.ToolStripButton btn_ungroup;
    private System.Windows.Forms.ToolStripButton btn_bringToFront;
    private System.Windows.Forms.ToolStripButton btn_sendToBack;
    private System.Windows.Forms.ToolStripMenuItem menu_Transformation;
    private System.Windows.Forms.ToolStripMenuItem menu_Transformation_Move;
    private System.Windows.Forms.ToolStripMenuItem menu_Transformation_Scale;
    private System.Windows.Forms.ToolStripMenuItem menu_Transformation_Rotate;
    private System.Windows.Forms.ToolStripMenuItem menu_Transformation_Group;
    private System.Windows.Forms.ToolStripMenuItem menu_Transformation_Ungroup;
    private System.Windows.Forms.ToolStripMenuItem menu_Transformation_Front;
    private System.Windows.Forms.ToolStripMenuItem menu_Transformation_Back;
    private System.Windows.Forms.ToolStrip PrimitiveToolStrip;
    private System.Windows.Forms.ToolStripButton btn_polygon;
    private System.Windows.Forms.ToolStripButton btn_polyline;
    private System.Windows.Forms.ToolStripButton btn_point;
    private System.Windows.Forms.ToolStripButton btn_text;
    private System.Windows.Forms.ToolStripButton btn_changeText;
    private System.Windows.Forms.ToolStrip PointToolStrip;
    private System.Windows.Forms.ToolStripButton btn_addPoint;
    private System.Windows.Forms.ToolStripButton btn_deletePoint;
    private System.Windows.Forms.ToolStripButton btn_movePoint;
    private System.Windows.Forms.ToolStripStatusLabel StatusString;
    private System.Windows.Forms.ToolStripStatusLabel MouseReadout;
    private System.Windows.Forms.ToolStripStatusLabel SelectedEntityCountString;
    private System.Windows.Forms.ToolStripMenuItem menu_Primitives;
    private System.Windows.Forms.ToolStripMenuItem menu_Primitives_Polygon;
    private System.Windows.Forms.ToolStripMenuItem menu_Primitives_Polyline;
    private System.Windows.Forms.ToolStripMenuItem menu_Primitives_Point;
    private System.Windows.Forms.ToolStripMenuItem menu_Primitives_Text;
    private System.Windows.Forms.ToolStripMenuItem menu_Primitives_ChangeText;
    private System.Windows.Forms.ToolStripMenuItem menu_File_OpenGeometry;
    private System.Windows.Forms.ToolStripMenuItem menu_File_OpenMap;
    private System.Windows.Forms.ToolStrip AttributesToolStrip;
    private System.Windows.Forms.ToolStrip MainToolStrip;
    private System.Windows.Forms.ToolStripSplitButton btn_open;
    private System.Windows.Forms.ToolStripMenuItem openbuttonGeometryMenuItem;
    private System.Windows.Forms.ToolStripMenuItem openbuttonMapMenuItem;
    private System.Windows.Forms.ToolStripMenuItem menu_Transformation_AddPoint;
    private System.Windows.Forms.ToolStripMenuItem menu_Transformation_DeletePoint;
    private System.Windows.Forms.ToolStripMenuItem menu_Transformation_MovePoint;
    private System.Windows.Forms.ToolStripMenuItem menu_Tools_Editmode;
    private System.Windows.Forms.ToolStrip ConstraintsToolStrip;
    private System.Windows.Forms.ToolStripButton btn_horizontal;
    private System.Windows.Forms.ToolStripButton btn_vertical;
    private System.Windows.Forms.ToolStripButton btn_equal;
    private System.Windows.Forms.ToolStripButton btn_unequal;
    private System.Windows.Forms.ToolStripButton btn_angular;
    private System.Windows.Forms.ToolStripButton btn_distance;
    private System.Windows.Forms.ToolStripMenuItem menu_Constraints;
    private System.Windows.Forms.ToolStripMenuItem menu_Constraints_Horizontal;
    private System.Windows.Forms.ToolStripMenuItem menu_Constraints_Vertical;
    private System.Windows.Forms.ToolStripMenuItem menu_Constraints_Equal;
    private System.Windows.Forms.ToolStripMenuItem menu_Constraints_Unequal;
    private System.Windows.Forms.ToolStripMenuItem menu_Constraints_Angular;
    private System.Windows.Forms.ToolStripMenuItem menu_Constraints_Distance;
    private System.Windows.Forms.OpenFileDialog FileOpenTMFDialog;
    private System.Windows.Forms.SaveFileDialog FileSaveTMFDialog;
    private System.Windows.Forms.ToolStripButton btn_symbolPalette;
    private System.Windows.Forms.ToolStripButton btn_symbolProperties;
    private System.Windows.Forms.ToolStripButton btn_symbolSelector;
    private System.Windows.Forms.ToolStripButton btn_declutterOptions;
    private System.Windows.Forms.ToolStripMenuItem menu_aPP6ATool;
    private System.Windows.Forms.ToolStripMenuItem menu_aPP6ATool_palette;
    private System.Windows.Forms.ToolStripMenuItem menu_aPP6ATool_properties;
    private System.Windows.Forms.ToolStripMenuItem menu_aPP6ATool_selector;
    private System.Windows.Forms.ToolStripMenuItem menu_aPP6ATool_declutterOptions;
    private System.Windows.Forms.ToolStripMenuItem menu_Primitives_Circle;
    private System.Windows.Forms.ToolStripSeparator separator_tools;
    private System.Windows.Forms.ToolStripSeparator separator_primitives1;
    private System.Windows.Forms.ToolStripSeparator separator_transformation2;
    private System.Windows.Forms.ToolStripSeparator separator_transformation3;
    private System.Windows.Forms.ToolStripSeparator separator_transformation1;
  }
}
