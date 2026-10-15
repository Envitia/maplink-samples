namespace CSharpTerrainSample
{
  partial class TerrainOptions
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
      this.okButton = new System.Windows.Forms.Button();
      this.cancelButton = new System.Windows.Forms.Button();
      this.groupBox1 = new System.Windows.Forms.GroupBox();
      this.ManualScaleEdit = new System.Windows.Forms.TextBox();
      this.label1 = new System.Windows.Forms.Label();
      this.BathymetryCheck = new System.Windows.Forms.CheckBox();
      this.AutoScaleCheck = new System.Windows.Forms.CheckBox();
      this.groupBox2 = new System.Windows.Forms.GroupBox();
      this.BathymetryScaleEdit = new System.Windows.Forms.TextBox();
      this.label3 = new System.Windows.Forms.Label();
      this.TerrainScaleEdit = new System.Windows.Forms.TextBox();
      this.label2 = new System.Windows.Forms.Label();
      this.groupBox3 = new System.Windows.Forms.GroupBox();
      this.EndPointHeightEdit = new System.Windows.Forms.TextBox();
      this.label4 = new System.Windows.Forms.Label();
      this.StartPointHeightEdit = new System.Windows.Forms.TextBox();
      this.label5 = new System.Windows.Forms.Label();
      this.groupBox4 = new System.Windows.Forms.GroupBox();
      this.ApplyCurvatureOfEarthCheck = new System.Windows.Forms.CheckBox();
      this.StartHeightAboveGroundCheck = new System.Windows.Forms.CheckBox();
      this.AccurateLOSCheck = new System.Windows.Forms.CheckBox();
      this.GridCellsInLOSCalcEdit = new System.Windows.Forms.TextBox();
      this.DarkShadedCheck = new System.Windows.Forms.CheckBox();
      this.label6 = new System.Windows.Forms.Label();
      this.MaxRangeMUEdit = new System.Windows.Forms.TextBox();
      this.label7 = new System.Windows.Forms.Label();
      this.groupBox1.SuspendLayout();
      this.groupBox2.SuspendLayout();
      this.groupBox3.SuspendLayout();
      this.groupBox4.SuspendLayout();
      this.SuspendLayout();
      // 
      // okButton
      // 
      this.okButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
      this.okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
      this.okButton.Location = new System.Drawing.Point(68, 413);
      this.okButton.Name = "okButton";
      this.okButton.Size = new System.Drawing.Size(75, 23);
      this.okButton.TabIndex = 25;
      this.okButton.Text = "&OK";
      this.okButton.Click += new System.EventHandler(this.okButton_Click);
      // 
      // cancelButton
      // 
      this.cancelButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
      this.cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
      this.cancelButton.Location = new System.Drawing.Point(149, 413);
      this.cancelButton.Name = "cancelButton";
      this.cancelButton.Size = new System.Drawing.Size(75, 23);
      this.cancelButton.TabIndex = 26;
      this.cancelButton.Text = "&Cancel";
      // 
      // groupBox1
      // 
      this.groupBox1.Controls.Add(this.ManualScaleEdit);
      this.groupBox1.Controls.Add(this.label1);
      this.groupBox1.Controls.Add(this.BathymetryCheck);
      this.groupBox1.Controls.Add(this.AutoScaleCheck);
      this.groupBox1.Location = new System.Drawing.Point(12, 12);
      this.groupBox1.Name = "groupBox1";
      this.groupBox1.Size = new System.Drawing.Size(268, 75);
      this.groupBox1.TabIndex = 27;
      this.groupBox1.TabStop = false;
      this.groupBox1.Text = "Terrain Window Options";
      // 
      // ManualScaleEdit
      // 
      this.ManualScaleEdit.Location = new System.Drawing.Point(124, 45);
      this.ManualScaleEdit.Name = "ManualScaleEdit";
      this.ManualScaleEdit.Size = new System.Drawing.Size(137, 20);
      this.ManualScaleEdit.TabIndex = 3;
      this.ManualScaleEdit.TextChanged += new System.EventHandler(this.OnDoubleEditChanged);
      // 
      // label1
      // 
      this.label1.AutoSize = true;
      this.label1.Location = new System.Drawing.Point(3, 48);
      this.label1.Name = "label1";
      this.label1.Size = new System.Drawing.Size(72, 13);
      this.label1.TabIndex = 2;
      this.label1.Text = "Manual Scale";
      // 
      // BathymetryCheck
      // 
      this.BathymetryCheck.AutoSize = true;
      this.BathymetryCheck.Location = new System.Drawing.Point(167, 19);
      this.BathymetryCheck.Name = "BathymetryCheck";
      this.BathymetryCheck.Size = new System.Drawing.Size(95, 17);
      this.BathymetryCheck.TabIndex = 1;
      this.BathymetryCheck.Text = "Bathymetry On";
      this.BathymetryCheck.UseVisualStyleBackColor = true;
      // 
      // AutoScaleCheck
      // 
      this.AutoScaleCheck.AutoSize = true;
      this.AutoScaleCheck.Location = new System.Drawing.Point(6, 19);
      this.AutoScaleCheck.Name = "AutoScaleCheck";
      this.AutoScaleCheck.Size = new System.Drawing.Size(73, 17);
      this.AutoScaleCheck.TabIndex = 0;
      this.AutoScaleCheck.Text = "Autoscale";
      this.AutoScaleCheck.UseVisualStyleBackColor = true;
      // 
      // groupBox2
      // 
      this.groupBox2.Controls.Add(this.BathymetryScaleEdit);
      this.groupBox2.Controls.Add(this.label3);
      this.groupBox2.Controls.Add(this.TerrainScaleEdit);
      this.groupBox2.Controls.Add(this.label2);
      this.groupBox2.Location = new System.Drawing.Point(12, 93);
      this.groupBox2.Name = "groupBox2";
      this.groupBox2.Size = new System.Drawing.Size(268, 75);
      this.groupBox2.TabIndex = 28;
      this.groupBox2.TabStop = false;
      this.groupBox2.Text = "Height Display Scales";
      // 
      // BathymetryScaleEdit
      // 
      this.BathymetryScaleEdit.Location = new System.Drawing.Point(124, 45);
      this.BathymetryScaleEdit.Name = "BathymetryScaleEdit";
      this.BathymetryScaleEdit.Size = new System.Drawing.Size(137, 20);
      this.BathymetryScaleEdit.TabIndex = 5;
      this.BathymetryScaleEdit.TextChanged += new System.EventHandler(this.OnDoubleEditChanged);
      // 
      // label3
      // 
      this.label3.AutoSize = true;
      this.label3.Location = new System.Drawing.Point(3, 48);
      this.label3.Name = "label3";
      this.label3.Size = new System.Drawing.Size(89, 13);
      this.label3.TabIndex = 4;
      this.label3.Text = "Bathymetry Scale";
      // 
      // TerrainScaleEdit
      // 
      this.TerrainScaleEdit.Location = new System.Drawing.Point(124, 19);
      this.TerrainScaleEdit.Name = "TerrainScaleEdit";
      this.TerrainScaleEdit.Size = new System.Drawing.Size(137, 20);
      this.TerrainScaleEdit.TabIndex = 3;
      this.TerrainScaleEdit.TextChanged += new System.EventHandler(this.OnDoubleEditChanged);
      // 
      // label2
      // 
      this.label2.AutoSize = true;
      this.label2.Location = new System.Drawing.Point(3, 22);
      this.label2.Name = "label2";
      this.label2.Size = new System.Drawing.Size(70, 13);
      this.label2.TabIndex = 2;
      this.label2.Text = "Terrain Scale";
      // 
      // groupBox3
      // 
      this.groupBox3.Controls.Add(this.EndPointHeightEdit);
      this.groupBox3.Controls.Add(this.label4);
      this.groupBox3.Controls.Add(this.StartPointHeightEdit);
      this.groupBox3.Controls.Add(this.label5);
      this.groupBox3.Location = new System.Drawing.Point(12, 174);
      this.groupBox3.Name = "groupBox3";
      this.groupBox3.Size = new System.Drawing.Size(268, 75);
      this.groupBox3.TabIndex = 29;
      this.groupBox3.TabStop = false;
      this.groupBox3.Text = "Line Of Sight";
      // 
      // EndPointHeightEdit
      // 
      this.EndPointHeightEdit.Location = new System.Drawing.Point(124, 45);
      this.EndPointHeightEdit.Name = "EndPointHeightEdit";
      this.EndPointHeightEdit.Size = new System.Drawing.Size(137, 20);
      this.EndPointHeightEdit.TabIndex = 5;
      this.EndPointHeightEdit.TextChanged += new System.EventHandler(this.OnDoubleEditChanged);
      // 
      // label4
      // 
      this.label4.AutoSize = true;
      this.label4.Location = new System.Drawing.Point(3, 48);
      this.label4.Name = "label4";
      this.label4.Size = new System.Drawing.Size(87, 13);
      this.label4.TabIndex = 4;
      this.label4.Text = "End Point Height";
      // 
      // StartPointHeightEdit
      // 
      this.StartPointHeightEdit.Location = new System.Drawing.Point(124, 19);
      this.StartPointHeightEdit.Name = "StartPointHeightEdit";
      this.StartPointHeightEdit.Size = new System.Drawing.Size(137, 20);
      this.StartPointHeightEdit.TabIndex = 3;
      this.StartPointHeightEdit.TextChanged += new System.EventHandler(this.OnDoubleEditChanged);
      // 
      // label5
      // 
      this.label5.AutoSize = true;
      this.label5.Location = new System.Drawing.Point(3, 22);
      this.label5.Name = "label5";
      this.label5.Size = new System.Drawing.Size(90, 13);
      this.label5.TabIndex = 2;
      this.label5.Text = "Start Point Height";
      // 
      // groupBox4
      // 
      this.groupBox4.Controls.Add(this.ApplyCurvatureOfEarthCheck);
      this.groupBox4.Controls.Add(this.StartHeightAboveGroundCheck);
      this.groupBox4.Controls.Add(this.AccurateLOSCheck);
      this.groupBox4.Controls.Add(this.GridCellsInLOSCalcEdit);
      this.groupBox4.Controls.Add(this.DarkShadedCheck);
      this.groupBox4.Controls.Add(this.label6);
      this.groupBox4.Controls.Add(this.MaxRangeMUEdit);
      this.groupBox4.Controls.Add(this.label7);
      this.groupBox4.Location = new System.Drawing.Point(12, 255);
      this.groupBox4.Name = "groupBox4";
      this.groupBox4.Size = new System.Drawing.Size(268, 152);
      this.groupBox4.TabIndex = 30;
      this.groupBox4.TabStop = false;
      this.groupBox4.Text = "Line Of Sight";
      // 
      // ApplyCurvatureOfEarthCheck
      // 
      this.ApplyCurvatureOfEarthCheck.AutoSize = true;
      this.ApplyCurvatureOfEarthCheck.Location = new System.Drawing.Point(6, 122);
      this.ApplyCurvatureOfEarthCheck.Name = "ApplyCurvatureOfEarthCheck";
      this.ApplyCurvatureOfEarthCheck.Size = new System.Drawing.Size(141, 17);
      this.ApplyCurvatureOfEarthCheck.TabIndex = 7;
      this.ApplyCurvatureOfEarthCheck.Text = "Apply Curvature of Earth";
      this.ApplyCurvatureOfEarthCheck.UseVisualStyleBackColor = true;
      // 
      // StartHeightAboveGroundCheck
      // 
      this.StartHeightAboveGroundCheck.AutoSize = true;
      this.StartHeightAboveGroundCheck.Location = new System.Drawing.Point(6, 99);
      this.StartHeightAboveGroundCheck.Name = "StartHeightAboveGroundCheck";
      this.StartHeightAboveGroundCheck.Size = new System.Drawing.Size(154, 17);
      this.StartHeightAboveGroundCheck.TabIndex = 6;
      this.StartHeightAboveGroundCheck.Text = "Start Height Above Ground";
      this.StartHeightAboveGroundCheck.UseVisualStyleBackColor = true;
      // 
      // AccurateLOSCheck
      // 
      this.AccurateLOSCheck.AutoSize = true;
      this.AccurateLOSCheck.Location = new System.Drawing.Point(167, 76);
      this.AccurateLOSCheck.Name = "AccurateLOSCheck";
      this.AccurateLOSCheck.Size = new System.Drawing.Size(93, 17);
      this.AccurateLOSCheck.TabIndex = 5;
      this.AccurateLOSCheck.Text = "Accurate LOS";
      this.AccurateLOSCheck.UseVisualStyleBackColor = true;
      // 
      // GridCellsInLOSCalcEdit
      // 
      this.GridCellsInLOSCalcEdit.Location = new System.Drawing.Point(124, 45);
      this.GridCellsInLOSCalcEdit.Name = "GridCellsInLOSCalcEdit";
      this.GridCellsInLOSCalcEdit.Size = new System.Drawing.Size(137, 20);
      this.GridCellsInLOSCalcEdit.TabIndex = 5;
      this.GridCellsInLOSCalcEdit.TextChanged += new System.EventHandler(this.OnIntEditChanged);
      // 
      // DarkShadedCheck
      // 
      this.DarkShadedCheck.AutoSize = true;
      this.DarkShadedCheck.Location = new System.Drawing.Point(6, 76);
      this.DarkShadedCheck.Name = "DarkShadedCheck";
      this.DarkShadedCheck.Size = new System.Drawing.Size(89, 17);
      this.DarkShadedCheck.TabIndex = 4;
      this.DarkShadedCheck.Text = "Dark Shaded";
      this.DarkShadedCheck.UseVisualStyleBackColor = true;
      // 
      // label6
      // 
      this.label6.AutoSize = true;
      this.label6.Location = new System.Drawing.Point(3, 48);
      this.label6.Name = "label6";
      this.label6.Size = new System.Drawing.Size(114, 13);
      this.label6.TabIndex = 4;
      this.label6.Text = "Grid Cells In LOS Calc.";
      // 
      // MaxRangeMUEdit
      // 
      this.MaxRangeMUEdit.Location = new System.Drawing.Point(124, 19);
      this.MaxRangeMUEdit.Name = "MaxRangeMUEdit";
      this.MaxRangeMUEdit.Size = new System.Drawing.Size(137, 20);
      this.MaxRangeMUEdit.TabIndex = 3;
      this.MaxRangeMUEdit.TextChanged += new System.EventHandler(this.OnDoubleEditChanged);
      // 
      // label7
      // 
      this.label7.AutoSize = true;
      this.label7.Location = new System.Drawing.Point(3, 22);
      this.label7.Name = "label7";
      this.label7.Size = new System.Drawing.Size(122, 13);
      this.label7.TabIndex = 2;
      this.label7.Text = "Max. Range (Map Units)";
      // 
      // TerrainOptions
      // 
      this.AcceptButton = this.okButton;
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.CancelButton = this.cancelButton;
      this.ClientSize = new System.Drawing.Size(292, 448);
      this.ControlBox = false;
      this.Controls.Add(this.groupBox4);
      this.Controls.Add(this.groupBox3);
      this.Controls.Add(this.groupBox2);
      this.Controls.Add(this.groupBox1);
      this.Controls.Add(this.cancelButton);
      this.Controls.Add(this.okButton);
      this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
      this.MaximizeBox = false;
      this.MinimizeBox = false;
      this.Name = "TerrainOptions";
      this.ShowIcon = false;
      this.ShowInTaskbar = false;
      this.Text = "Options Dialog";
      this.groupBox1.ResumeLayout(false);
      this.groupBox1.PerformLayout();
      this.groupBox2.ResumeLayout(false);
      this.groupBox2.PerformLayout();
      this.groupBox3.ResumeLayout(false);
      this.groupBox3.PerformLayout();
      this.groupBox4.ResumeLayout(false);
      this.groupBox4.PerformLayout();
      this.ResumeLayout(false);

    }

    #endregion

    private System.Windows.Forms.Button okButton;
    private System.Windows.Forms.Button cancelButton;
    private System.Windows.Forms.GroupBox groupBox1;
    private System.Windows.Forms.TextBox ManualScaleEdit;
    private System.Windows.Forms.Label label1;
    private System.Windows.Forms.CheckBox BathymetryCheck;
    private System.Windows.Forms.CheckBox AutoScaleCheck;
    private System.Windows.Forms.GroupBox groupBox2;
    private System.Windows.Forms.TextBox BathymetryScaleEdit;
    private System.Windows.Forms.Label label3;
    private System.Windows.Forms.TextBox TerrainScaleEdit;
    private System.Windows.Forms.Label label2;
    private System.Windows.Forms.GroupBox groupBox3;
    private System.Windows.Forms.TextBox EndPointHeightEdit;
    private System.Windows.Forms.Label label4;
    private System.Windows.Forms.TextBox StartPointHeightEdit;
    private System.Windows.Forms.Label label5;
    private System.Windows.Forms.GroupBox groupBox4;
    private System.Windows.Forms.CheckBox ApplyCurvatureOfEarthCheck;
    private System.Windows.Forms.CheckBox StartHeightAboveGroundCheck;
    private System.Windows.Forms.CheckBox AccurateLOSCheck;
    private System.Windows.Forms.TextBox GridCellsInLOSCalcEdit;
    private System.Windows.Forms.CheckBox DarkShadedCheck;
    private System.Windows.Forms.Label label6;
    private System.Windows.Forms.TextBox MaxRangeMUEdit;
    private System.Windows.Forms.Label label7;
  }
}