using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;


using System.Drawing.Imaging;
using System.Runtime.InteropServices;

namespace DataScope
{
    public partial class DataScopeForm : Form
    {
        private Random rnd = new Random();

        private UInt32 window_size;
        private UInt32 sample_count;

        private UInt32 prevSample;

        private Bitmap image;
        private Point start_pos;

        public DataScopeForm()
        {
            InitializeComponent();

            window_size = (UInt32) ClientSize.Width;
            sample_count = 0;

            start_pos = new Point(0, 0);
            
            image = new Bitmap(ClientSize.Width, ClientSize.Height);
        }

        private void DataScopeForm_Load(object sender, EventArgs e)
        {
            timer.Interval = 10;
            timer.Enabled = true;
            timer.Start();

            Graphics gBitmap = Graphics.FromImage(image);
            gBitmap.Clear(Color.White);
        }

        private void DataScopeForm_timer_Tick(object sender, EventArgs e)
        {
            //UInt32 sample = (UInt32) ClientSize.Height - (UInt32) ((1.0 - rnd.NextDouble()) * ClientSize.Height);
            UInt32 sample = (UInt32)ClientSize.Height / 2 - (UInt32)(Math.Sin((sample_count * (1.0 + 0.3*rnd.NextDouble()))/ 10.0 / Math.PI) * ClientSize.Height / 2);

            if (sample_count > 1)
            {
                Graphics gBitmap = Graphics.FromImage(image);
                Pen pen = new Pen(Color.White, 1);
                int x_pos = (int)(sample_count % window_size);

                if (sample_count >= window_size)
                {
                    gBitmap.DrawLine(pen, x_pos, 0, x_pos, image.Height);
                }

                gBitmap.DrawLine(pen, x_pos, 0, x_pos, image.Height);

                pen.Width = 1;
                pen.Color = Color.Blue;
                gBitmap.DrawLine(pen, x_pos - 1, prevSample, x_pos, sample);

                pen.Color = Color.Red;
                gBitmap.DrawLine(pen, x_pos + 2, 0, x_pos + 2, image.Height);

                pen.Width = 1;
                pen.Color = Color.Black;
                gBitmap.DrawLine(pen, 0, image.Height/2, image.Width - 1, image.Height/2);
            }
            
            prevSample = sample;
            sample_count++;

            Refresh();
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            Graphics gForm = e.Graphics;
            gForm.DrawImageUnscaled(image, start_pos);
        }

        protected override void OnPaintBackground(PaintEventArgs e)
        {
            // do nothing here in order to avoid background clean
        }
         
    }
}
