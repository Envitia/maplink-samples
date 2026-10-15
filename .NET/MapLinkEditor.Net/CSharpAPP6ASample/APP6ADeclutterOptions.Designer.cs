namespace CSharpAPP6ASample
{
  partial class APP6ADeclutterOptions
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
      System.Windows.Forms.GroupBox groupBox1;
      this.textCheckBox = new System.Windows.Forms.CheckBox();
      this.graphicsCheckBox = new System.Windows.Forms.CheckBox();
      this.frameCheckBox = new System.Windows.Forms.CheckBox();
      this.okButton = new System.Windows.Forms.Button();
      groupBox1 = new System.Windows.Forms.GroupBox();
      groupBox1.SuspendLayout();
      this.SuspendLayout();
      // 
      // groupBox1
      // 
      groupBox1.Controls.Add(this.frameCheckBox);
      groupBox1.Controls.Add(this.graphicsCheckBox);
      groupBox1.Controls.Add(this.textCheckBox);
      groupBox1.Location = new System.Drawing.Point(12, 12);
      groupBox1.Name = "groupBox1";
      groupBox1.Size = new System.Drawing.Size(324, 69);
      groupBox1.TabIndex = 0;
      groupBox1.TabStop = false;
      groupBox1.Text = "Options";
      // 
      // textCheckBox
      // 
      this.textCheckBox.AutoSize = true;
      this.textCheckBox.Location = new System.Drawing.Point(23, 29);
      this.textCheckBox.Name = "textCheckBox";
      this.textCheckBox.Size = new System.Drawing.Size(72, 17);
      this.textCheckBox.TabIndex = 0;
      this.textCheckBox.Text = "Hide Text";
      this.textCheckBox.UseVisualStyleBackColor = true;
      // 
      // graphicsCheckBox
      // 
      this.graphicsCheckBox.AutoSize = true;
      this.graphicsCheckBox.Location = new System.Drawing.Point(113, 29);
      this.graphicsCheckBox.Name = "graphicsCheckBox";
      this.graphicsCheckBox.Size = new System.Drawing.Size(93, 17);
      this.graphicsCheckBox.TabIndex = 1;
      this.graphicsCheckBox.Text = "Hide Graphics";
      this.graphicsCheckBox.UseVisualStyleBackColor = true;
      // 
      // frameCheckBox
      // 
      this.frameCheckBox.AutoSize = true;
      this.frameCheckBox.Location = new System.Drawing.Point(222, 29);
      this.frameCheckBox.Name = "frameCheckBox";
      this.frameCheckBox.Size = new System.Drawing.Size(80, 17);
      this.frameCheckBox.TabIndex = 2;
      this.frameCheckBox.Text = "Hide Frame";
      this.frameCheckBox.UseVisualStyleBackColor = true;
      // 
      // okButton
      // 
      this.okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
      this.okButton.Location = new System.Drawing.Point(137, 95);
      this.okButton.Name = "okButton";
      this.okButton.Size = new System.Drawing.Size(75, 23);
      this.okButton.TabIndex = 1;
      this.okButton.Text = "OK";
      this.okButton.UseVisualStyleBackColor = true;
      // 
      // APP6ADeclutterOptions
      // 
      this.AcceptButton = this.okButton;
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(348, 130);
      this.Controls.Add(this.okButton);
      this.Controls.Add(groupBox1);
      this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
      this.MaximizeBox = false;
      this.MinimizeBox = false;
      this.Name = "APP6ADeclutterOptions";
      this.ShowIcon = false;
      this.ShowInTaskbar = false;
      this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
      this.Text = "APP6A Declutter Options";
      groupBox1.ResumeLayout(false);
      groupBox1.PerformLayout();
      this.ResumeLayout(false);

    }

    #endregion

    private System.Windows.Forms.CheckBox frameCheckBox;
    private System.Windows.Forms.CheckBox graphicsCheckBox;
    private System.Windows.Forms.CheckBox textCheckBox;
    private System.Windows.Forms.Button okButton;
  }
}