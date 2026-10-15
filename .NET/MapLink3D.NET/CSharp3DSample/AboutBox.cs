using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

namespace CSharp3DSample
{
	/// <summary>
	/// Summary description for AboutBox.
	/// </summary>
	public class AboutBox : System.Windows.Forms.Form
	{
    private Button OKButton;
		private Label label1;
		private Label label2;
		private Label label3;
    private PictureBox pictureBox1;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private Container components = null;

		public AboutBox()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

      Bitmap bitmap = (Bitmap)pictureBox1.Image;
      bitmap.MakeTransparent(pictureBox1.Image.Palette.Entries[110]);
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
      System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(AboutBox));
      this.OKButton = new System.Windows.Forms.Button();
      this.label1 = new System.Windows.Forms.Label();
      this.label2 = new System.Windows.Forms.Label();
      this.label3 = new System.Windows.Forms.Label();
      this.pictureBox1 = new System.Windows.Forms.PictureBox();
      ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
      this.SuspendLayout();
      // 
      // OKButton
      // 
      this.OKButton.Location = new System.Drawing.Point(333, 12);
      this.OKButton.Name = "OKButton";
      this.OKButton.Size = new System.Drawing.Size(64, 24);
      this.OKButton.TabIndex = 0;
      this.OKButton.Text = "OK";
      this.OKButton.Click += new System.EventHandler(this.OKButton_Click);
      // 
      // label1
      // 
      this.label1.Location = new System.Drawing.Point(12, 62);
      this.label1.Name = "label1";
      this.label1.Size = new System.Drawing.Size(248, 16);
      this.label1.TabIndex = 2;
      this.label1.Text = "C# 3D .NET Sample Version 1.0";
      // 
      // label2
      // 
      this.label2.Location = new System.Drawing.Point(12, 87);
      this.label2.Name = "label2";
      this.label2.Size = new System.Drawing.Size(336, 16);
      this.label2.TabIndex = 3;
      this.label2.Text = "Copyright © 1998-2025 Envitia Group PLC. All rights reserved.";
      // 
      // label3
      // 
      this.label3.ForeColor = System.Drawing.SystemColors.ButtonShadow;
      this.label3.Location = new System.Drawing.Point(12, 113);
      this.label3.Name = "label3";
      this.label3.Size = new System.Drawing.Size(385, 32);
      this.label3.TabIndex = 4;
      this.label3.Text = "Portions developed using LEADTOOLS,\r\nCopyright © 1991-2025 LEAD Technologies, Inc" +
    ". ALL RIGHTS RESERVED\r\n";
      this.label3.TextAlign = System.Drawing.ContentAlignment.TopCenter;
      // 
      // pictureBox1
      // 
      this.pictureBox1.Image = ((System.Drawing.Image)(resources.GetObject("pictureBox1.Image")));
      this.pictureBox1.Location = new System.Drawing.Point(12, 8);
      this.pictureBox1.Name = "pictureBox1";
      this.pictureBox1.Size = new System.Drawing.Size(175, 41);
      this.pictureBox1.TabIndex = 5;
      this.pictureBox1.TabStop = false;
      // 
      // AboutBox
      // 
      this.AcceptButton = this.OKButton;
      this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
      this.ClientSize = new System.Drawing.Size(409, 155);
      this.Controls.Add(this.pictureBox1);
      this.Controls.Add(this.label3);
      this.Controls.Add(this.label2);
      this.Controls.Add(this.label1);
      this.Controls.Add(this.OKButton);
      this.Cursor = System.Windows.Forms.Cursors.Default;
      this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
      this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
      this.MaximizeBox = false;
      this.MinimizeBox = false;
      this.Name = "AboutBox";
      this.ShowInTaskbar = false;
      this.Text = "About C# 3D .NET Sample";
      ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
      this.ResumeLayout(false);

		}
		#endregion

		private void OKButton_Click(object sender, System.EventArgs e)
		{
			this.Hide();
		}
	}
}
