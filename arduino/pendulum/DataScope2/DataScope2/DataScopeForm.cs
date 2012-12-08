using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace DataScope2 {
    public partial class DataScopeForm : Form {

        private Random rnd = new Random();

        private UInt32 windowSize;
        private UInt32 sampleCount;
        private UInt32 sample;
        private UInt32 prevSample;

        private Int32 gridStepVertical;
        private Int32 gridStepHorizontal;

        public DataScopeForm() {
            InitializeComponent();

            windowSize = (UInt32)ClientSize.Width;
            sampleCount = 0;
        }

        private void DataScopeForm_onLoad(object sender, EventArgs e) {
            sample = 0;

            gridStepVertical = ClientSize.Width / 10;
            gridStepHorizontal = ClientSize.Height / 8;

            timer.Interval = 20;
            timer.Enabled = true;
            timer.Start();
        }

        private void DataScopeForm_timer_onTick(object sender, EventArgs e) {
            prevSample = sample;
            sample = (UInt32)ClientSize.Height / 2 - (UInt32)((Math.Sin((sampleCount / 10.0 / Math.PI)) + rnd.NextDouble() - 0.5) * ClientSize.Height / 3);
            sampleCount++;

            Refresh();
        }

        private void DrawGrid(Graphics graphics, Pen pen) {
            // horizontal grid
            for (int i = 0; i < ClientSize.Height / 2; i += gridStepHorizontal) {
                int yPos = ClientSize.Height / 2 + i;
                graphics.DrawLine(pen, 0, yPos, ClientSize.Width - 1, yPos);
                yPos = ClientSize.Height / 2 - i;
                graphics.DrawLine(pen, 0, yPos, ClientSize.Width - 1, yPos);
            }
            // vertical grid
            for (int i = 0; i < ClientSize.Width; i += gridStepVertical) {
                graphics.DrawLine(pen, i, 0, i, ClientSize.Height);
            }
        }

        private void UpdateGrid(Graphics graphics, Pen pen, int xPos) {
            // horizontal grid
            for (int i = 0; i < ClientSize.Height / 2; i += gridStepHorizontal) {
                int yPos = ClientSize.Height / 2 + i;
                graphics.DrawLine(pen, xPos, yPos, xPos + 1, yPos);
                yPos = ClientSize.Height / 2 - i;
                graphics.DrawLine(pen, xPos, yPos, xPos + 1, yPos);
            }
            // vertical grid
            if (xPos % gridStepVertical == 0) {
                graphics.DrawLine(pen, xPos, 0, xPos, ClientSize.Height - 1);
            }
        }

        protected override void OnPaint(PaintEventArgs e) {
            Graphics gForm = e.Graphics;
            Pen pen = new Pen(Color.LightGray, 1);

            if (sampleCount > 1) {
                // clear previous line
                int xPos = (int)(sampleCount % windowSize);

                gForm.DrawLine(pen, xPos, 0, xPos, ClientSize.Height - 1);

                // draw grid
                pen.Color = Color.Black;
                UpdateGrid(gForm, pen, xPos);

                pen.Color = Color.Blue;
                gForm.DrawLine(pen, xPos - 1, prevSample, xPos, sample);

                pen.Color = Color.Red;
                gForm.DrawLine(pen, xPos + 2, 0, xPos + 2, ClientSize.Height - 1);
            }
            else {
                gForm.Clear(Color.LightGray);
                
                pen.Color = Color.Black;
                DrawGrid(gForm, pen);
            }
        }

        protected override void OnPaintBackground(PaintEventArgs e) {
            // do nothing here in order to avoid background clean
        }

    }
}