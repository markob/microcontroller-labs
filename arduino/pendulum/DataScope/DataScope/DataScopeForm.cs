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
        private Int32 count = 0;
        private Random rnd = new Random();
        private System.Windows.Forms.DataVisualization.Charting.Axis axisX;

        public DataScopeForm()
        {
            InitializeComponent();
        }

        private void DataScopeForm_Load(object sender, EventArgs e)
        {
            var chartArea = this.scope.ChartAreas["chartArea"];
            
            chartArea.AxisX.Minimum = 0;
            chartArea.AxisX.Maximum = 300;
            chartArea.AxisY.Minimum = -600;
            chartArea.AxisY.Maximum = 600;

            chartArea.AxisX.MajorGrid.Interval = 50;
            chartArea.AxisX.MinorGrid.Interval = 50;
            chartArea.AxisX.Interval = 50;

            chartArea.AxisY.MajorGrid.Interval = 200;
            chartArea.AxisY.MinorGrid.Interval = 200;
            chartArea.AxisY.Interval = 200;

            axisX = chartArea.AxisX;

            Timer timer = new Timer();
            timer.Tick += new EventHandler(timer_Tick);
            timer.Interval = 10;
            timer.Enabled = true;
            timer.Start();
        }

        private void timer_Tick(object sender, EventArgs e)
        {
            if (count > axisX.Maximum)
            {
                axisX.Maximum++;
                axisX.Minimum++;
            }

            this.scope.Series["series_x"].Points.AddXY(count, rnd.Next() % 1024 - 512);
            this.scope.Series["series_y"].Points.AddXY(count, rnd.Next() % 1024 - 512);
            this.scope.Series["series_z"].Points.AddXY(count, rnd.Next() % 1024 - 512);

            count++;            
        }
    }
}
