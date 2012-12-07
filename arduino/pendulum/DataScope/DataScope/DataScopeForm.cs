using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace DataScope
{
    public partial class DataScopeForm : Form
    {
        private Random rnd = new Random();

        private UInt32[] data_y;

        private UInt32 window_size;
        private UInt32 sample_count;

        private Bitmap image;
        private Point start_pos;

        public DataScopeForm()
        {
            InitializeComponent();

            window_size = (UInt32) Size.Width;
            data_y = new UInt32[window_size];

            sample_count = 0;

            start_pos = new Point(0, 0);
            image = new Bitmap(Size.Width, Size.Height);
        }

        private void DataScopeForm_Load(object sender, EventArgs e)
        {
            timer.Interval = 100;
            timer.Enabled = true;
            timer.Start();
        }

        private void DataScopeForm_timer_Tick(object sender, EventArgs e)
        {
            if (sample_count < window_size)
            {
                data_y[sample_count] = (UInt32)Size.Height - (UInt32)rnd.Next() % (UInt32)Size.Height;
            }
            //else
            //{

            //}

            sample_count++;

            Graphics gBitmap = Graphics.FromImage(image);
            Pen pen = new Pen(Color.Red, 1);
            gBitmap.DrawLine(pen, (int)sample_count - 1, (int)data_y[sample_count - 1], (int)sample_count, (int)data_y[sample_count]);

            Refresh();
        }

        private void DataScopeForm_onPaint(object sender, PaintEventArgs e)
        {
            Graphics gForm = e.Graphics;
            gForm.DrawImageUnscaled(image, start_pos);
        }
         
    }
}
