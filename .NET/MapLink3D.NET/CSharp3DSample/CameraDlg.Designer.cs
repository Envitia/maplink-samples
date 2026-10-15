namespace CSharp3DSample
{
  partial class CameraDlg
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
      this.groupBox1 = new System.Windows.Forms.GroupBox();
      this.label3 = new System.Windows.Forms.Label();
      this.label2 = new System.Windows.Forms.Label();
      this.zBox = new System.Windows.Forms.TextBox();
      this.label1 = new System.Windows.Forms.Label();
      this.yBox = new System.Windows.Forms.TextBox();
      this.xBox = new System.Windows.Forms.TextBox();
      this.label4 = new System.Windows.Forms.Label();
      this.raBox = new System.Windows.Forms.TextBox();
      this.label5 = new System.Windows.Forms.Label();
      this.fovBox = new System.Windows.Forms.TextBox();
      this.ok_button = new System.Windows.Forms.Button();
      this.cancel_button = new System.Windows.Forms.Button();
      this.groupBox1.SuspendLayout();
      this.SuspendLayout();
      // 
      // groupBox1
      // 
      this.groupBox1.Controls.Add(this.label3);
      this.groupBox1.Controls.Add(this.label2);
      this.groupBox1.Controls.Add(this.zBox);
      this.groupBox1.Controls.Add(this.label1);
      this.groupBox1.Controls.Add(this.yBox);
      this.groupBox1.Controls.Add(this.xBox);
      this.groupBox1.Location = new System.Drawing.Point(12, 12);
      this.groupBox1.Name = "groupBox1";
      this.groupBox1.Size = new System.Drawing.Size(201, 104);
      this.groupBox1.TabIndex = 0;
      this.groupBox1.TabStop = false;
      this.groupBox1.Text = "Rotation Axis";
      // 
      // label3
      // 
      this.label3.AutoSize = true;
      this.label3.Location = new System.Drawing.Point(6, 74);
      this.label3.Name = "label3";
      this.label3.Size = new System.Drawing.Size(74, 13);
      this.label3.TabIndex = 3;
      this.label3.Text = "Z-Component:";
      this.label3.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
      // 
      // label2
      // 
      this.label2.AutoSize = true;
      this.label2.Location = new System.Drawing.Point(6, 48);
      this.label2.Name = "label2";
      this.label2.Size = new System.Drawing.Size(74, 13);
      this.label2.TabIndex = 3;
      this.label2.Text = "Y-Component:";
      this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
      // 
      // zBox
      // 
      this.zBox.Location = new System.Drawing.Point(95, 71);
      this.zBox.Name = "zBox";
      this.zBox.Size = new System.Drawing.Size(100, 20);
      this.zBox.TabIndex = 2;
      this.zBox.Text = "0";
      this.zBox.TextChanged += new System.EventHandler(this.OnNumericTextChanged);
      // 
      // label1
      // 
      this.label1.AutoSize = true;
      this.label1.Location = new System.Drawing.Point(6, 22);
      this.label1.Name = "label1";
      this.label1.Size = new System.Drawing.Size(74, 13);
      this.label1.TabIndex = 1;
      this.label1.Text = "X-Component:";
      this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
      // 
      // yBox
      // 
      this.yBox.Location = new System.Drawing.Point(95, 45);
      this.yBox.Name = "yBox";
      this.yBox.Size = new System.Drawing.Size(100, 20);
      this.yBox.TabIndex = 1;
      this.yBox.Text = "0";
      this.yBox.TextChanged += new System.EventHandler(this.OnNumericTextChanged);
      // 
      // xBox
      // 
      this.xBox.Location = new System.Drawing.Point(95, 19);
      this.xBox.Name = "xBox";
      this.xBox.Size = new System.Drawing.Size(100, 20);
      this.xBox.TabIndex = 0;
      this.xBox.Text = "0";
      this.xBox.TextChanged += new System.EventHandler(this.OnNumericTextChanged);
      // 
      // label4
      // 
      this.label4.AutoSize = true;
      this.label4.Location = new System.Drawing.Point(18, 134);
      this.label4.Name = "label4";
      this.label4.Size = new System.Drawing.Size(80, 26);
      this.label4.TabIndex = 5;
      this.label4.Text = "Rotation Angle:\r\n(degrees)\r\n";
      this.label4.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
      // 
      // raBox
      // 
      this.raBox.Location = new System.Drawing.Point(107, 131);
      this.raBox.Name = "raBox";
      this.raBox.Size = new System.Drawing.Size(100, 20);
      this.raBox.TabIndex = 3;
      this.raBox.Text = "0";
      this.raBox.TextChanged += new System.EventHandler(this.OnNumericTextChanged);
      // 
      // label5
      // 
      this.label5.AutoSize = true;
      this.label5.Location = new System.Drawing.Point(18, 174);
      this.label5.Name = "label5";
      this.label5.Size = new System.Drawing.Size(72, 26);
      this.label5.TabIndex = 7;
      this.label5.Text = "Field Of View:\r\n(degrees)\r\n";
      this.label5.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
      // 
      // fovBox
      // 
      this.fovBox.Location = new System.Drawing.Point(107, 171);
      this.fovBox.Name = "fovBox";
      this.fovBox.Size = new System.Drawing.Size(100, 20);
      this.fovBox.TabIndex = 4;
      this.fovBox.Text = "45";
      this.fovBox.TextChanged += new System.EventHandler(this.OnNumericTextChanged);
      // 
      // ok_button
      // 
      this.ok_button.Location = new System.Drawing.Point(36, 210);
      this.ok_button.Name = "ok_button";
      this.ok_button.Size = new System.Drawing.Size(75, 23);
      this.ok_button.TabIndex = 8;
      this.ok_button.Text = "Ok";
      this.ok_button.UseVisualStyleBackColor = true;
      this.ok_button.Click += new System.EventHandler(this.ok_button_Click);
      // 
      // cancel_button
      // 
      this.cancel_button.DialogResult = System.Windows.Forms.DialogResult.Cancel;
      this.cancel_button.Location = new System.Drawing.Point(117, 210);
      this.cancel_button.Name = "cancel_button";
      this.cancel_button.Size = new System.Drawing.Size(75, 23);
      this.cancel_button.TabIndex = 9;
      this.cancel_button.Text = "Cancel";
      this.cancel_button.UseVisualStyleBackColor = true;
      // 
      // CameraDlg
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.CancelButton = this.cancel_button;
      this.ClientSize = new System.Drawing.Size(225, 245);
      this.Controls.Add(this.cancel_button);
      this.Controls.Add(this.ok_button);
      this.Controls.Add(this.label5);
      this.Controls.Add(this.fovBox);
      this.Controls.Add(this.label4);
      this.Controls.Add(this.raBox);
      this.Controls.Add(this.groupBox1);
      this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
      this.MaximizeBox = false;
      this.MinimizeBox = false;
      this.Name = "CameraDlg";
      this.ShowInTaskbar = false;
      this.Text = "Define Camera Orientation";
      this.groupBox1.ResumeLayout(false);
      this.groupBox1.PerformLayout();
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.GroupBox groupBox1;
    private System.Windows.Forms.Label label3;
    private System.Windows.Forms.Label label2;
    private System.Windows.Forms.TextBox zBox;
    private System.Windows.Forms.Label label1;
    private System.Windows.Forms.TextBox yBox;
    private System.Windows.Forms.TextBox xBox;
    private System.Windows.Forms.Label label4;
    private System.Windows.Forms.TextBox raBox;
    private System.Windows.Forms.Label label5;
    private System.Windows.Forms.TextBox fovBox;
    private System.Windows.Forms.Button ok_button;
    private System.Windows.Forms.Button cancel_button;
  }
}