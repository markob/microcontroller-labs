namespace ScopeViewTest {
    partial class ScopeViewTest {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing) {
            if (disposing && (components != null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
            this.components = new System.ComponentModel.Container();
            this.timer = new System.Windows.Forms.Timer(this.components);
            this.scopeView = new ScopeView.ScopeView();
            this.SuspendLayout();
            // 
            // timer
            // 
            this.timer.Interval = 20;
            this.timer.Tick += new System.EventHandler(this.ScopeViewTest_timer_onTick);
            // 
            // scopeView
            // 
            this.scopeView.Location = new System.Drawing.Point(12, 12);
            this.scopeView.Name = "scopeView";
            this.scopeView.Size = new System.Drawing.Size(545, 400);
            this.scopeView.TabIndex = 0;
            // 
            // ScopeViewTest
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(569, 424);
            this.Controls.Add(this.scopeView);
            this.Name = "ScopeViewTest";
            this.Text = "Scope View Test";
            this.Load += new System.EventHandler(this.ScopeViewTest_onLoad);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Timer timer;
        private ScopeView.ScopeView scopeView;
    }
}

