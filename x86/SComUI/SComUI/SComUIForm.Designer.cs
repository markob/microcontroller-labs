namespace SComUI
{
    partial class SComUIForm
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
            this.lblTemp = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // lblTemp
            // 
            this.lblTemp.Font = new System.Drawing.Font("Verdana", 27.75F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.lblTemp.Location = new System.Drawing.Point(12, 12);
            this.lblTemp.Name = "lblTemp";
            this.lblTemp.Size = new System.Drawing.Size(268, 52);
            this.lblTemp.TabIndex = 0;
            this.lblTemp.Text = "Connecting...";
            // 
            // SComUIForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(292, 73);
            this.Controls.Add(this.lblTemp);
            this.Name = "SComUIForm";
            this.Text = "MSP430 Termometer";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.SComUIForm_Closing);
            this.Load += new System.EventHandler(this.SComUIFrom_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label lblTemp;
        private System.IO.Ports.SerialPort sPort;
    }
}

