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

        private struct DrawAreaInfo {
            public Int32 minX;
            public Int32 minY;
            public Int32 maxX;
            public Int32 maxY;
            public Int32 Width;
            public Int32 Height;
        }
        
        private DrawAreaInfo drawArea;

        public ScopeView() {
            InitializeComponent();
        }

        private void ScopeView_onLoad(object sender, EventArgs e) {
            sampleCount = 0;

            bgColor = Color.LightGray;

            gridColor = Color.Black;

            samples = new LineData[linesNum];
            samples[0] = new LineData();

            samples[0].color = Color.Blue;
        }

        private void InitDrawingArea(Graphics graphics) {
            Pen pen = new Pen(Color.Black, 1);
            graphics.Clear(bgColor);

            drawArea.minX = 1;
            drawArea.minY = 1;
            drawArea.maxX = ClientSize.Width - 2;
            drawArea.maxY = ClientSize.Height - 2;
            drawArea.Width = ClientSize.Width - 2;
            drawArea.Height = ClientSize.Height - 2;

            int maxX = ClientSize.Width - 1;
            int maxY = ClientSize.Height - 1;
            
            graphics.DrawLine(pen, 0, 0, maxX, 0);
            graphics.DrawLine(pen, 0, maxY, maxX, maxY);
            graphics.DrawLine(pen, 0, 0, 0, maxY);
            graphics.DrawLine(pen, maxX, 0, maxX, maxY);

            gridStepHorizontal = drawArea.Height / 8;
            gridStepVertical = drawArea.Width / 10;
        }

        private void DrawGrid(Graphics graphics, Pen pen) {
            // update pen with grid properties
            pen.Color = gridColor;
            pen.Width = 1;

            // horizontal grid
            for (int i = 0; i <= drawArea.Height / 2; i += gridStepHorizontal) {
                int yPos = drawArea.Height / 2 + i + drawArea.minY;
                graphics.DrawLine(pen, drawArea.minY, yPos, drawArea.maxX, yPos);
                yPos = drawArea.Height / 2 - i + drawArea.minY;
                graphics.DrawLine(pen, drawArea.minX, yPos, drawArea.maxX, yPos);
            }
            // vertical grid
            for (int i = 0; i < drawArea.Width; i += gridStepVertical) {
                graphics.DrawLine(pen, i, drawArea.minY, i, drawArea.maxY);
            }
        }

        private void UpdateGrid(Graphics graphics, Pen pen, int xPos) {
            // update pen with grid properties
            pen.Color = gridColor;
            pen.Width = 1;

            // horizontal grid
            for (int i = 0; i <= drawArea.Height / 2; i += gridStepHorizontal) {
                int yPos = drawArea.Height / 2 + i + drawArea.minY;
                graphics.DrawLine(pen, xPos, yPos, xPos + 1, yPos);
                yPos = drawArea.Height / 2 - i + drawArea.minY;
                graphics.DrawLine(pen, xPos, yPos, xPos + 1, yPos);
            }
            // vertical grid
            if (xPos % gridStepVertical == 0) {
                graphics.DrawLine(pen, xPos, drawArea.minY, xPos, drawArea.maxY);
            }
        }

        public void submitNewSample(UInt32[] dataIn) {
            for (int i = 0; i < samples.Length; i++) {
                samples[i].prevSample = samples[i].sample;
                // todo: check sample value range
                if (dataIn[i] > drawArea.Height) samples[i].sample = (UInt32) drawArea.Height;
                else samples[i].sample = dataIn[i];
            }

            sampleCount++;
            
            Refresh();
        }

        protected override void OnPaint(PaintEventArgs e) {
            Graphics graphics = e.Graphics;
            Pen pen = new Pen(bgColor, 1);

            if (sampleCount > 1) {
                // clear previous line
                int xPos = (int)(sampleCount % drawArea.Width) + drawArea.minX;

                graphics.DrawLine(pen, xPos, drawArea.minY, xPos, drawArea.maxY);

                // draw grid
                UpdateGrid(graphics, pen, xPos);

                // draw data lines
                foreach (LineData sample in samples) {
                    pen.Color = sample.color;
                    graphics.DrawLine(pen, xPos - 1, sample.prevSample, xPos, sample.sample);
                }

                // draw cursor line
                pen.Color = Color.Red;
                xPos = (xPos - drawArea.minX + 1) % drawArea.Width + drawArea.minX;
                graphics.DrawLine(pen, xPos, drawArea.minY, xPos, drawArea.maxY);
            } else {
                InitDrawingArea(graphics);
                DrawGrid(graphics, pen);
            }
        }

        protected override void OnPaintBackground(PaintEventArgs e) {
            // just do nothing here in order to have incremental paint
            // i.e. always paint only new element instead of all picture
        }
    }
}