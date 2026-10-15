namespace CSharpAPP6ASample
{
  partial class RequestTextDialog
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
      this.label1 = new System.Windows.Forms.Label();
      this.textBox1 = new System.Windows.Forms.TextBox();
      this.OkButton = new System.Windows.Forms.Button();
      this.CANCELButton = new System.Windows.Forms.Button();
      this.SuspendLayout();
      // 
      // label1
      // 
      this.label1.AutoSize = true;
      this.label1.Location = new System.Drawing.Point(7, 10);
      this.label1.Name = "label1";
      this.label1.Size = new System.Drawing.Size(220, 13);
      this.label1.TabIndex = 0;
      this.label1.Text = "Please enter the text that you wish to appear:";
      // 
      // textBox1
      // 
      this.textBox1.Location = new System.Drawing.Point(10, 26);
      this.textBox1.Name = "textBox1";
      this.textBox1.Size = new System.Drawing.Size(421, 20);
      this.textBox1.TabIndex = 1;
      // 
      // OkButton
      // 
      this.OkButton.DialogResult = System.Windows.Forms.DialogResult.OK;
      this.OkButton.Location = new System.Drawing.Point(144, 52);
      this.OkButton.Name = "OkButton";
      this.OkButton.Size = new System.Drawing.Size(75, 23);
      this.OkButton.TabIndex = 2;
      this.OkButton.Text = "OK";
      this.OkButton.UseVisualStyleBackColor = true;
      this.OkButton.Click += new System.EventHandler(this.OkButton_Click);
      // 
      // CANCELButton
      // 
      this.CANCELButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
      this.CANCELButton.Location = new System.Drawing.Point(225, 52);
      this.CANCELButton.Name = "CANCELButton";
      this.CANCELButton.Size = new System.Drawing.Size(75, 23);
      this.CANCELButton.TabIndex = 3;
      this.CANCELButton.Text = "Cancel";
      this.CANCELButton.UseVisualStyleBackColor = true;
      // 
      // RequestTextDialog
      // 
      this.AcceptButton = this.OkButton;
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.CancelButton = this.CANCELButton;
      this.ClientSize = new System.Drawing.Size(444, 85);
      this.ControlBox = false;
      this.Controls.Add(this.CANCELButton);
      this.Controls.Add(this.OkButton);
      this.Controls.Add(this.textBox1);
      this.Controls.Add(this.label1);
      this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
      this.MaximizeBox = false;
      this.MinimizeBox = false;
      this.Name = "RequestTextDialog";
      this.ShowIcon = false;
      this.ShowInTaskbar = false;
      this.Text = "C# .NET APP6A Sample";
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.Label label1;
    private System.Windows.Forms.TextBox textBox1;
    private System.Windows.Forms.Button OkButton;
    private System.Windows.Forms.Button CANCELButton;
  }
}