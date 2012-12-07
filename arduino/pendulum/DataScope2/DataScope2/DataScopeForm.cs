using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace DataScope2 {
    public partial class DataScopeForm : Form {

        public DataScopeForm() {
            InitializeComponent();
        }

        private void DataScopeForm_onLoad(object sender, EventArgs e) {
            this.timer.Enabled = true;
            this.timer.Start();
        }

        private void DataScopeForm_onPaint(object sender, PaintEventArgs e) {
            Graphics graphics = this.CreateGraphics();
            Pen pen = new Pen(Color.Red, 7);
            graphics.DrawLine(pen, 1, 1, 100, 100);
        }

        private void Timer_onTick(object sender, EventArgs e) {

        }
    }
}