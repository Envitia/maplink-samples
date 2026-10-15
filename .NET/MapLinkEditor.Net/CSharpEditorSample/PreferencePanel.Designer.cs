namespace CSharpEditorSample
{
  partial class PreferencePanel
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

    #region Component Designer generated code

    /// <summary> 
    /// Required method for Designer support - do not modify 
    /// the contents of this method with the code editor.
    /// </summary>
    private void InitializeComponent()
    {
      this.DistanceEdit = new System.Windows.Forms.TextBox();
      this.label2 = new System.Windows.Forms.Label();
      this.AngleEdit = new System.Windows.Forms.TextBox();
      this.label3 = new System.Windows.Forms.Label();
      this.groupBox5 = new System.Windows.Forms.GroupBox();
      this.CursorReadoutCombo = new System.Windows.Forms.ComboBox();
      this.groupBox4 = new System.Windows.Forms.GroupBox();
      this.groupBox5.SuspendLayout();
      this.groupBox4.SuspendLayout();
      this.SuspendLayout();
      // 
      // DistanceEdit
      // 
      this.DistanceEdit.Location = new System.Drawing.Point(64, 45);
      this.DistanceEdit.Name = "DistanceEdit";
      this.DistanceEdit.Size = new System.Drawing.Size(76, 20);
      this.DistanceEdit.TabIndex = 2;
      this.DistanceEdit.TextChanged += new System.EventHandler(this.OnDistanceChanged);
      // 
      // label2
      // 
      this.label2.AutoSize = true;
      this.label2.Location = new System.Drawing.Point(5, 23);
      this.label2.Name = "label2";
      this.label2.Size = new System.Drawing.Size(37, 13);
      this.label2.TabIndex = 1;
      this.label2.Text = "Angle:";
      // 
      // AngleEdit
      // 
      this.AngleEdit.Location = new System.Drawing.Point(64, 19);
      this.AngleEdit.Name = "AngleEdit";
      this.AngleEdit.Size = new System.Drawing.Size(75, 20);
      this.AngleEdit.TabIndex = 0;
      this.AngleEdit.TextChanged += new System.EventHandler(this.OnAngleChanged);
      // 
      // label3
      // 
      this.label3.AutoSize = true;
      this.label3.Location = new System.Drawing.Point(6, 49);
      this.label3.Name = "label3";
      this.label3.Size = new System.Drawing.Size(52, 13);
      this.label3.TabIndex = 3;
      this.label3.Text = "Distance:";
      // 
      // groupBox5
      // 
      this.groupBox5.Controls.Add(this.CursorReadoutCombo);
      this.groupBox5.Location = new System.Drawing.Point(9, 88);
      this.groupBox5.Name = "groupBox5";
      this.groupBox5.Size = new System.Drawing.Size(149, 53);
      this.groupBox5.TabIndex = 12;
      this.groupBox5.TabStop = false;
      this.groupBox5.Text = "Cursor Readout";
      // 
      // CursorReadoutCombo
      // 
      this.CursorReadoutCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
      this.CursorReadoutCombo.FormattingEnabled = true;
      this.CursorReadoutCombo.Items.AddRange(new object[] {
            "TMC Absolute",
            "TMC Relative",
            "MU Absolute",
            "MU Relative",
            "UU Absolute",
            "UU Relative",
            "Lat/Long Decimal",
            "Lat/Long Angular",
            "Angle/TMC Distance",
            "Bearing/TMC Distance",
            "Angle/MU Distance",
            "Bearing/MU Distance",
            "Angle/UU Distance",
            "Bearing/UU Distance"});
      this.CursorReadoutCombo.Location = new System.Drawing.Point(6, 19);
      this.CursorReadoutCombo.Name = "CursorReadoutCombo";
      this.CursorReadoutCombo.Size = new System.Drawing.Size(134, 21);
      this.CursorReadoutCombo.TabIndex = 3;
      this.CursorReadoutCombo.SelectedIndexChanged += new System.EventHandler(this.OnCursorReadoutChanged);
      // 
      // groupBox4
      // 
      this.groupBox4.Controls.Add(this.label3);
      this.groupBox4.Controls.Add(this.DistanceEdit);
      this.groupBox4.Controls.Add(this.label2);
      this.groupBox4.Controls.Add(this.AngleEdit);
      this.groupBox4.Location = new System.Drawing.Point(9, 6);
      this.groupBox4.Name = "groupBox4";
      this.groupBox4.Size = new System.Drawing.Size(149, 76);
      this.groupBox4.TabIndex = 11;
      this.groupBox4.TabStop = false;
      this.groupBox4.Text = "Constraint Settings";
      // 
      // PreferencePanel
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.Controls.Add(this.groupBox5);
      this.Controls.Add(this.groupBox4);
      this.MaximumSize = new System.Drawing.Size(168, 150);
      this.MinimumSize = new System.Drawing.Size(168, 150);
      this.Name = "PreferencePanel";
      this.Padding = new System.Windows.Forms.Padding(4);
      this.Size = new System.Drawing.Size(168, 150);
      this.Load += new System.EventHandler(this.OnLoad);
      this.groupBox5.ResumeLayout(false);
      this.groupBox4.ResumeLayout(false);
      this.groupBox4.PerformLayout();
      this.ResumeLayout(false);

    }

    #endregion

    private System.Windows.Forms.TextBox DistanceEdit;
    private System.Windows.Forms.Label label2;
    private System.Windows.Forms.TextBox AngleEdit;
    private System.Windows.Forms.Label label3;
    private System.Windows.Forms.GroupBox groupBox5;
    private System.Windows.Forms.ComboBox CursorReadoutCombo;
    private System.Windows.Forms.GroupBox groupBox4;
  }
}
