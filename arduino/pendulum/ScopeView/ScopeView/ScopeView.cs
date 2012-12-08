using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace ScopeView {
    public partial class ScopeView : UserControl {
        private UInt32 sampleCount;

        private Color bgColor;

        private struct LineData {
            public UInt32 sample;
            public UInt32 prevSample;
            public Color color;
        }

        private LineData [] samples;
        private Int32 linesNum = 1;
        
        // Read-Write Property for Number of Data Lines
        /*[DefaultValue(1)]
        public Int32 linesCount {
            get {
                return linesNum;
            }
            set {
                linesNum = value;

                //samples = new LineData[linesNum];
                //for (int i = 0; i < linesNum; i++) {
                    //samples[i] = new LineData();
                    //samples[i].sample = 0;
                    //samples[i].prevSample = 0;

                //    sampleCount = 0;
                //}
            }
        }*/

        // grid related data definitions
        private Color gridColor;
        private Int32 gridStepVertical;
        private Int32 gridStepHorizontal;

        private UInt32 drawAreaMaxX;
        private UInt32 drawAreaMaxY;

        public ScopeView() {
            InitializeComponent();
        }

        private void ScopeView_onLoad(object sender, EventArgs e) {
            sampleCount = 0;

            bgColor = Color.LightGray;

            gridColor = Color.Black;
            gridStepHorizontal = ClientSize.Height / 8;
            gridStepVertical = ClientSize.Width / 10;

            samples = new LineData[linesNum];
            samples[0] = new LineData();

            samples[0].color = Color.Blue;
        }

        private void DrawGrid(Graphics graphics, Pen pen) {
            // update pen with grid properties
            pen.Color = gridColor;
            pen.Width = 1;

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

            // boundaries
            graphics.DrawLine(pen, 0, 0, ClientSize.Width - 1, 0);
            graphics.DrawLine(pen, 0, ClientSize.Height - 1, ClientSize.Width - 1, ClientSize.Height - 1);
            graphics.DrawLine(pen, ClientSize.Width - 1, 0, ClientSize.Width - 1, ClientSize.Height - 1);
        }

        private void UpdateGrid(Graphics graphics, Pen pen, int xPos) {
            // update pen with grid properties
            pen.Color = gridColor;
            pen.Width = 1;

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

            // boundaries
            graphics.DrawLine(pen, xPos, 0, xPos + 1, 0);
            graphics.DrawLine(pen, xPos, ClientSize.Height - 1, xPos + 1, ClientSize.Height - 1);
            if (xPos == ClientSize.Width - 1) {
                graphics.DrawLine(pen, ClientSize.Width - 1, 0, ClientSize.Width - 1, ClientSize.Height - 1);
            }
        }

        public void submitNewSample(UInt32[] samplesList) {
            for (int i = 0; i < samples.Length; i++) {
                samples[i].prevSample = samples[i].sample;
                samples[i].sample = samplesList[i];
            }

            sampleCount++;
            
            Refresh();
        }

        protected override void OnPaint(PaintEventArgs e) {
            Graphics graphics = e.Graphics;
            Pen pen = new Pen(bgColor, 1);

            if (sampleCount > 1) {
                // clear previous line
                int xPos = (int)(sampleCount % ClientSize.Width);

                graphics.DrawLine(pen, xPos, 0, xPos, ClientSize.Height - 1);

                // draw grid
                UpdateGrid(graphics, pen, xPos);

                // draw data lines
                foreach (LineData sample in samples) {
                    pen.Color = sample.color;
                    graphics.DrawLine(pen, xPos - 1, sample.prevSample, xPos, sample.sample);
                }

                // draw cursor line
                pen.Color = Color.Red;
                graphics.DrawLine(pen, xPos + 2, 0, xPos + 2, ClientSize.Height - 1);
            } else {
                graphics.Clear(bgColor);
                DrawGrid(graphics, pen);
            }
        }

        protected override void OnPaintBackground(PaintEventArgs e) {
            // just do nothing here in order to have incremental paint
            // i.e. always paint only new element instead of all picture
        }
    }
}