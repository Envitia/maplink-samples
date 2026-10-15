namespace CSharpEditorSample
{
  partial class AttributesPanel
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
      this.groupBox3 = new System.Windows.Forms.GroupBox();
      this.groupBox6 = new System.Windows.Forms.GroupBox();
      this.label1 = new System.Windows.Forms.Label();
      this.TextBGEdgeColourCombo = new CSharpEditorSample.NonDropDownComboBox();
      this.TextBGStyleCombo = new System.Windows.Forms.ComboBox();
      this.TextBGColourCombo = new CSharpEditorSample.NonDropDownComboBox();
      this.TextBGTypeCombo = new System.Windows.Forms.ComboBox();
      this.TextVAlignmentCombo = new System.Windows.Forms.ComboBox();
      this.TextHAlignmentCombo = new System.Windows.Forms.ComboBox();
      this.TextStyleCombo = new System.Windows.Forms.ComboBox();
      this.TextColourCombo = new CSharpEditorSample.NonDropDownComboBox();
      this.TextSizeVal = new System.Windows.Forms.NumericUpDown();
      this.groupBox1 = new System.Windows.Forms.GroupBox();
      this.EdgeStyleCombo = new System.Windows.Forms.ComboBox();
      this.EdgeColourCombo = new CSharpEditorSample.NonDropDownComboBox();
      this.EdgeWidthVal = new System.Windows.Forms.NumericUpDown();
      this.groupBox2 = new System.Windows.Forms.GroupBox();
      this.FillStyleCombo = new System.Windows.Forms.ComboBox();
      this.FillColourCombo = new CSharpEditorSample.NonDropDownComboBox();
      this.colourPicker = new System.Windows.Forms.ColorDialog();
      this.groupBox3.SuspendLayout();
      this.groupBox6.SuspendLayout();
      ((System.ComponentModel.ISupportInitialize)(this.TextSizeVal)).BeginInit();
      this.groupBox1.SuspendLayout();
      ((System.ComponentModel.ISupportInitialize)(this.EdgeWidthVal)).BeginInit();
      this.groupBox2.SuspendLayout();
      this.SuspendLayout();
      // 
      // groupBox3
      // 
      this.groupBox3.Controls.Add(this.groupBox6);
      this.groupBox3.Controls.Add(this.TextVAlignmentCombo);
      this.groupBox3.Controls.Add(this.TextHAlignmentCombo);
      this.groupBox3.Controls.Add(this.TextStyleCombo);
      this.groupBox3.Controls.Add(this.TextColourCombo);
      this.groupBox3.Controls.Add(this.TextSizeVal);
      this.groupBox3.Location = new System.Drawing.Point(9, 171);
      this.groupBox3.Name = "groupBox3";
      this.groupBox3.Size = new System.Drawing.Size(149, 244);
      this.groupBox3.TabIndex = 9;
      this.groupBox3.TabStop = false;
      this.groupBox3.Text = "Text";
      // 
      // groupBox6
      // 
      this.groupBox6.Controls.Add(this.label1);
      this.groupBox6.Controls.Add(this.TextBGEdgeColourCombo);
      this.groupBox6.Controls.Add(this.TextBGStyleCombo);
      this.groupBox6.Controls.Add(this.TextBGColourCombo);
      this.groupBox6.Controls.Add(this.TextBGTypeCombo);
      this.groupBox6.Location = new System.Drawing.Point(9, 100);
      this.groupBox6.Name = "groupBox6";
      this.groupBox6.Size = new System.Drawing.Size(132, 132);
      this.groupBox6.TabIndex = 8;
      this.groupBox6.TabStop = false;
      this.groupBox6.Text = "Background";
      // 
      // label1
      // 
      this.label1.AutoSize = true;
      this.label1.Location = new System.Drawing.Point(11, 104);
      this.label1.Name = "label1";
      this.label1.Size = new System.Drawing.Size(35, 13);
      this.label1.TabIndex = 10;
      this.label1.Text = "Edge:";
      // 
      // TextBGEdgeColourCombo
      // 
      this.TextBGEdgeColourCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
      this.TextBGEdgeColourCombo.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
      this.TextBGEdgeColourCombo.FormattingEnabled = true;
      this.TextBGEdgeColourCombo.Location = new System.Drawing.Point(63, 100);
      this.TextBGEdgeColourCombo.Name = "TextBGEdgeColourCombo";
      this.TextBGEdgeColourCombo.Size = new System.Drawing.Size(63, 21);
      this.TextBGEdgeColourCombo.TabIndex = 9;
      this.TextBGEdgeColourCombo.BackColorChanged += new System.EventHandler(this.TextBGEdgeColourComboBackColourChanged);
      this.TextBGEdgeColourCombo.DropDown += new System.EventHandler(this.OnDroppingColourItem);
      // 
      // TextBGStyleCombo
      // 
      this.TextBGStyleCombo.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
      this.TextBGStyleCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
      this.TextBGStyleCombo.FormattingEnabled = true;
      this.TextBGStyleCombo.Location = new System.Drawing.Point(6, 73);
      this.TextBGStyleCombo.Name = "TextBGStyleCombo";
      this.TextBGStyleCombo.Size = new System.Drawing.Size(120, 21);
      this.TextBGStyleCombo.TabIndex = 8;
      this.TextBGStyleCombo.DrawItem += new System.Windows.Forms.DrawItemEventHandler(this.OnDrawFillStyle);
      this.TextBGStyleCombo.SelectedIndexChanged += new System.EventHandler(this.TextBGStyleCombo_SelectedIndexChanged);
      // 
      // TextBGColourCombo
      // 
      this.TextBGColourCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
      this.TextBGColourCombo.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
      this.TextBGColourCombo.FormattingEnabled = true;
      this.TextBGColourCombo.Location = new System.Drawing.Point(6, 46);
      this.TextBGColourCombo.Name = "TextBGColourCombo";
      this.TextBGColourCombo.Size = new System.Drawing.Size(120, 21);
      this.TextBGColourCombo.TabIndex = 7;
      this.TextBGColourCombo.BackColorChanged += new System.EventHandler(this.TextBGColourComboColourChanged);
      this.TextBGColourCombo.DropDown += new System.EventHandler(this.OnDroppingColourItem);
      // 
      // TextBGTypeCombo
      // 
      this.TextBGTypeCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
      this.TextBGTypeCombo.FormattingEnabled = true;
      this.TextBGTypeCombo.Items.AddRange(new object[] {
            "None",
            "Halo",
            "Rectangle"});
      this.TextBGTypeCombo.Location = new System.Drawing.Point(6, 19);
      this.TextBGTypeCombo.Name = "TextBGTypeCombo";
      this.TextBGTypeCombo.Size = new System.Drawing.Size(120, 21);
      this.TextBGTypeCombo.TabIndex = 6;
      this.TextBGTypeCombo.SelectedIndexChanged += new System.EventHandler(this.TextBGTypeCombo_SelectedIndexChanged);
      // 
      // TextVAlignmentCombo
      // 
      this.TextVAlignmentCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
      this.TextVAlignmentCombo.FormattingEnabled = true;
      this.TextVAlignmentCombo.Items.AddRange(new object[] {
            "Full Bottom",
            "Baseline",
            "Middle",
            "Capital Top",
            "Full Top"});
      this.TextVAlignmentCombo.Location = new System.Drawing.Point(79, 73);
      this.TextVAlignmentCombo.Name = "TextVAlignmentCombo";
      this.TextVAlignmentCombo.Size = new System.Drawing.Size(63, 21);
      this.TextVAlignmentCombo.TabIndex = 7;
      this.TextVAlignmentCombo.SelectedIndexChanged += new System.EventHandler(this.TextVAlignmentCombo_SelectedIndexChanged);
      // 
      // TextHAlignmentCombo
      // 
      this.TextHAlignmentCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
      this.TextHAlignmentCombo.FormattingEnabled = true;
      this.TextHAlignmentCombo.Items.AddRange(new object[] {
            "Left",
            "Centre",
            "Right"});
      this.TextHAlignmentCombo.Location = new System.Drawing.Point(9, 73);
      this.TextHAlignmentCombo.Name = "TextHAlignmentCombo";
      this.TextHAlignmentCombo.Size = new System.Drawing.Size(63, 21);
      this.TextHAlignmentCombo.TabIndex = 6;
      this.TextHAlignmentCombo.SelectedIndexChanged += new System.EventHandler(this.TextHAlignmentCombo_SelectedIndexChanged);
      // 
      // TextStyleCombo
      // 
      this.TextStyleCombo.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
      this.TextStyleCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
      this.TextStyleCombo.FormattingEnabled = true;
      this.TextStyleCombo.Location = new System.Drawing.Point(9, 46);
      this.TextStyleCombo.Name = "TextStyleCombo";
      this.TextStyleCombo.Size = new System.Drawing.Size(134, 21);
      this.TextStyleCombo.TabIndex = 5;
      this.TextStyleCombo.DrawItem += new System.Windows.Forms.DrawItemEventHandler(this.OnDrawTextStyle);
      this.TextStyleCombo.SelectedIndexChanged += new System.EventHandler(this.TextStyleCombo_SelectedIndexChanged);
      // 
      // TextColourCombo
      // 
      this.TextColourCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
      this.TextColourCombo.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
      this.TextColourCombo.FormattingEnabled = true;
      this.TextColourCombo.Location = new System.Drawing.Point(9, 19);
      this.TextColourCombo.Name = "TextColourCombo";
      this.TextColourCombo.Size = new System.Drawing.Size(77, 21);
      this.TextColourCombo.TabIndex = 4;
      this.TextColourCombo.BackColorChanged += new System.EventHandler(this.TextColourComboBackColourChanged);
      this.TextColourCombo.DropDown += new System.EventHandler(this.OnDroppingColourItem);
      // 
      // TextSizeVal
      // 
      this.TextSizeVal.Location = new System.Drawing.Point(92, 19);
      this.TextSizeVal.Name = "TextSizeVal";
      this.TextSizeVal.Size = new System.Drawing.Size(51, 20);
      this.TextSizeVal.TabIndex = 3;
      this.TextSizeVal.ValueChanged += new System.EventHandler(this.TextSizeVal_ValueChanged);
      // 
      // groupBox1
      // 
      this.groupBox1.Controls.Add(this.EdgeStyleCombo);
      this.groupBox1.Controls.Add(this.EdgeColourCombo);
      this.groupBox1.Controls.Add(this.EdgeWidthVal);
      this.groupBox1.Location = new System.Drawing.Point(9, 3);
      this.groupBox1.Name = "groupBox1";
      this.groupBox1.Size = new System.Drawing.Size(149, 77);
      this.groupBox1.TabIndex = 7;
      this.groupBox1.TabStop = false;
      this.groupBox1.Text = "Edge";
      // 
      // EdgeStyleCombo
      // 
      this.EdgeStyleCombo.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
      this.EdgeStyleCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
      this.EdgeStyleCombo.FormattingEnabled = true;
      this.EdgeStyleCombo.Location = new System.Drawing.Point(9, 46);
      this.EdgeStyleCombo.Name = "EdgeStyleCombo";
      this.EdgeStyleCombo.Size = new System.Drawing.Size(134, 21);
      this.EdgeStyleCombo.TabIndex = 2;
      this.EdgeStyleCombo.DrawItem += new System.Windows.Forms.DrawItemEventHandler(this.OnDrawEdgeStyle);
      this.EdgeStyleCombo.SelectedIndexChanged += new System.EventHandler(this.EdgeStyleCombo_SelectedIndexChanged);
      // 
      // EdgeColourCombo
      // 
      this.EdgeColourCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
      this.EdgeColourCombo.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
      this.EdgeColourCombo.DropDownWidth = 1;
      this.EdgeColourCombo.FormattingEnabled = true;
      this.EdgeColourCombo.Location = new System.Drawing.Point(9, 19);
      this.EdgeColourCombo.Name = "EdgeColourCombo";
      this.EdgeColourCombo.Size = new System.Drawing.Size(77, 21);
      this.EdgeColourCombo.TabIndex = 1;
      this.EdgeColourCombo.BackColorChanged += new System.EventHandler(this.EdgeColourComboBackColourChanged);
      this.EdgeColourCombo.DropDown += new System.EventHandler(this.OnDroppingColourItem);
      // 
      // EdgeWidthVal
      // 
      this.EdgeWidthVal.Location = new System.Drawing.Point(92, 19);
      this.EdgeWidthVal.Name = "EdgeWidthVal";
      this.EdgeWidthVal.Size = new System.Drawing.Size(51, 20);
      this.EdgeWidthVal.TabIndex = 0;
      this.EdgeWidthVal.ValueChanged += new System.EventHandler(this.EdgeWidthVal_ValueChanged);
      // 
      // groupBox2
      // 
      this.groupBox2.Controls.Add(this.FillStyleCombo);
      this.groupBox2.Controls.Add(this.FillColourCombo);
      this.groupBox2.Location = new System.Drawing.Point(9, 86);
      this.groupBox2.Name = "groupBox2";
      this.groupBox2.Size = new System.Drawing.Size(149, 79);
      this.groupBox2.TabIndex = 8;
      this.groupBox2.TabStop = false;
      this.groupBox2.Text = "Fill";
      // 
      // FillStyleCombo
      // 
      this.FillStyleCombo.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
      this.FillStyleCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
      this.FillStyleCombo.FormattingEnabled = true;
      this.FillStyleCombo.Location = new System.Drawing.Point(9, 46);
      this.FillStyleCombo.Name = "FillStyleCombo";
      this.FillStyleCombo.Size = new System.Drawing.Size(134, 21);
      this.FillStyleCombo.TabIndex = 4;
      this.FillStyleCombo.DrawItem += new System.Windows.Forms.DrawItemEventHandler(this.OnDrawFillStyle);
      this.FillStyleCombo.SelectedIndexChanged += new System.EventHandler(this.FillStyleCombo_SelectedIndexChanged);
      // 
      // FillColourCombo
      // 
      this.FillColourCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
      this.FillColourCombo.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
      this.FillColourCombo.FormattingEnabled = true;
      this.FillColourCombo.Location = new System.Drawing.Point(9, 19);
      this.FillColourCombo.Name = "FillColourCombo";
      this.FillColourCombo.Size = new System.Drawing.Size(134, 21);
      this.FillColourCombo.TabIndex = 3;
      this.FillColourCombo.BackColorChanged += new System.EventHandler(this.FillColourComboBackColourChanged);
      this.FillColourCombo.DropDown += new System.EventHandler(this.OnDroppingColourItem);
      // 
      // AttributesPanel
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.Controls.Add(this.groupBox3);
      this.Controls.Add(this.groupBox1);
      this.Controls.Add(this.groupBox2);
      this.MaximumSize = new System.Drawing.Size(168, 423);
      this.MinimumSize = new System.Drawing.Size(168, 423);
      this.Name = "AttributesPanel";
      this.Padding = new System.Windows.Forms.Padding(4);
      this.Size = new System.Drawing.Size(168, 423);
      this.Load += new System.EventHandler(this.OnLoad);
      this.groupBox3.ResumeLayout(false);
      this.groupBox6.ResumeLayout(false);
      this.groupBox6.PerformLayout();
      ((System.ComponentModel.ISupportInitialize)(this.TextSizeVal)).EndInit();
      this.groupBox1.ResumeLayout(false);
      ((System.ComponentModel.ISupportInitialize)(this.EdgeWidthVal)).EndInit();
      this.groupBox2.ResumeLayout(false);
      this.ResumeLayout(false);

    }

    #endregion

    private System.Windows.Forms.GroupBox groupBox3;
    private System.Windows.Forms.GroupBox groupBox6;
    private System.Windows.Forms.Label label1;
    private CSharpEditorSample.NonDropDownComboBox TextBGEdgeColourCombo;
    private System.Windows.Forms.ComboBox TextBGStyleCombo;
    private CSharpEditorSample.NonDropDownComboBox TextBGColourCombo;
    private System.Windows.Forms.ComboBox TextBGTypeCombo;
    private System.Windows.Forms.ComboBox TextVAlignmentCombo;
    private System.Windows.Forms.ComboBox TextHAlignmentCombo;
    private System.Windows.Forms.ComboBox TextStyleCombo;
    private CSharpEditorSample.NonDropDownComboBox TextColourCombo;
    private System.Windows.Forms.NumericUpDown TextSizeVal;
    private System.Windows.Forms.GroupBox groupBox1;
    private System.Windows.Forms.ComboBox EdgeStyleCombo;
    private CSharpEditorSample.NonDropDownComboBox EdgeColourCombo;
    private System.Windows.Forms.NumericUpDown EdgeWidthVal;
    private System.Windows.Forms.GroupBox groupBox2;
    private System.Windows.Forms.ComboBox FillStyleCombo;
    private CSharpEditorSample.NonDropDownComboBox FillColourCombo;
    private System.Windows.Forms.ColorDialog colourPicker;
  }
}
