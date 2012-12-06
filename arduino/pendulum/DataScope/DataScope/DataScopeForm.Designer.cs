namespace DataScope
{
    partial class DataScopeForm
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
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Series series1 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.Series series2 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.Series series3 = new System.Windows.Forms.DataVisualization.Charting.Series();
            this.scope = new System.Windows.Forms.DataVisualization.Charting.Chart();
            ((System.ComponentModel.ISupportInitialize)(this.scope)).BeginInit();
            this.SuspendLayout();
            // 
            // scope
            // 
            chartArea1.Name = "chartArea";
            this.scope.ChartAreas.Add(chartArea1);
            this.scope.Location = new System.Drawing.Point(-1, 0);
            this.scope.Name = "scope";
            series1.ChartArea = "chartArea";
            series1.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.FastLine;
            series1.Name = "series_x";
            series1.XValueMember = "data_t";
            series1.XValueType = System.Windows.Forms.DataVisualization.Charting.ChartValueType.UInt32;
            series1.YValueMembers = "data_x";
            series1.YValueType = System.Windows.Forms.DataVisualization.Charting.ChartValueType.Int32;
            series2.ChartArea = "chartArea";
            series2.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.FastLine;
            series2.Name = "series_y";
            series2.XValueMember = "data_t";
            series2.XValueType = System.Windows.Forms.DataVisualization.Charting.ChartValueType.UInt32;
            series2.YValueMembers = "data_y";
            series2.YValueType = System.Windows.Forms.DataVisualization.Charting.ChartValueType.Int32;
            series3.ChartArea = "chartArea";
            series3.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.FastLine;
            series3.Name = "series_z";
            series3.XValueMember = "data_t";
            series3.XValueType = System.Windows.Forms.DataVisualization.Charting.ChartValueType.UInt32;
            series3.YValueMembers = "data_y";
            series3.YValueType = System.Windows.Forms.DataVisualization.Charting.ChartValueType.Int32;
            this.scope.Series.Add(series1);
            this.scope.Series.Add(series2);
            this.scope.Series.Add(series3);
            this.scope.Size = new System.Drawing.Size(640, 480);
            this.scope.TabIndex = 0;
            this.scope.Text = "Scope";
            
            // 
            // DataScopeForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(639, 481);
            this.Controls.Add(this.scope);
            this.Name = "DataScopeForm";
            this.Text = "Data Scope";
            this.Load += new System.EventHandler(this.DataScopeForm_Load);
            ((System.ComponentModel.ISupportInitialize)(this.scope)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.DataVisualization.Charting.Chart scope;
    }
}

