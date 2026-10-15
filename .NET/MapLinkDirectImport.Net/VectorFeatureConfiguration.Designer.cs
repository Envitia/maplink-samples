namespace CSharpDirectImportSample
{
  partial class VectorFeatureConfiguration
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
      System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(VectorFeatureConfiguration));
      this.flowLayoutPanel1 = new System.Windows.Forms.FlowLayoutPanel();
      this.label1 = new System.Windows.Forms.Label();
      this.GenerateButton = new System.Windows.Forms.Button();
      this.CustomButton = new System.Windows.Forms.Button();
      this.LoadButton = new System.Windows.Forms.Button();
      this.flowLayoutPanel1.SuspendLayout();
      this.SuspendLayout();
      // 
      // flowLayoutPanel1
      // 
      this.flowLayoutPanel1.Controls.Add(this.label1);
      this.flowLayoutPanel1.Controls.Add(this.GenerateButton);
      this.flowLayoutPanel1.Controls.Add(this.CustomButton);
      this.flowLayoutPanel1.Controls.Add(this.LoadButton);
      this.flowLayoutPanel1.Location = new System.Drawing.Point(12, 12);
      this.flowLayoutPanel1.Name = "flowLayoutPanel1";
      this.flowLayoutPanel1.Size = new System.Drawing.Size(510, 203);
      this.flowLayoutPanel1.TabIndex = 1;
      // 
      // label1
      // 
      this.label1.AutoSize = true;
      this.label1.Location = new System.Drawing.Point(3, 0);
      this.label1.Name = "label1";
      this.label1.Size = new System.Drawing.Size(502, 130);
      this.label1.TabIndex = 0;
      this.label1.Text = resources.GetString("label1.Text");
      // 
      // GenerateButton
      // 
      this.GenerateButton.Location = new System.Drawing.Point(3, 133);
      this.GenerateButton.Name = "GenerateButton";
      this.GenerateButton.Size = new System.Drawing.Size(75, 23);
      this.GenerateButton.TabIndex = 1;
      this.GenerateButton.Text = "Generate";
      this.GenerateButton.UseVisualStyleBackColor = true;
      this.GenerateButton.Click += new System.EventHandler(this.GenerateButton_Click);
      // 
      // CustomButton
      // 
      this.CustomButton.Enabled = false;
      this.CustomButton.Location = new System.Drawing.Point(84, 133);
      this.CustomButton.Name = "CustomButton";
      this.CustomButton.Size = new System.Drawing.Size(75, 23);
      this.CustomButton.TabIndex = 2;
      this.CustomButton.Text = "Custom";
      this.CustomButton.UseVisualStyleBackColor = true;
      this.CustomButton.Click += new System.EventHandler(this.CustomButton_Click);
      // 
      // LoadButton
      // 
      this.LoadButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
      this.LoadButton.Location = new System.Drawing.Point(165, 133);
      this.LoadButton.Name = "LoadButton";
      this.LoadButton.Size = new System.Drawing.Size(75, 23);
      this.LoadButton.TabIndex = 3;
      this.LoadButton.Text = "Load...";
      this.LoadButton.UseVisualStyleBackColor = true;
      this.LoadButton.Click += new System.EventHandler(this.LoadButton_Click);
      // 
      // VectorFeatureConfiguration
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(534, 217);
      this.Controls.Add(this.flowLayoutPanel1);
      this.Name = "VectorFeatureConfiguration";
      this.Text = "VectorFeatureConfiguration";
      this.flowLayoutPanel1.ResumeLayout(false);
      this.flowLayoutPanel1.PerformLayout();
      this.ResumeLayout(false);

    }

    #endregion

    private System.Windows.Forms.FlowLayoutPanel flowLayoutPanel1;
    private System.Windows.Forms.Label label1;
    private System.Windows.Forms.Button GenerateButton;
    private System.Windows.Forms.Button CustomButton;
    private System.Windows.Forms.Button LoadButton;
  }
}