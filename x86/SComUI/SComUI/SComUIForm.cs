using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using System.Threading;

namespace SComUI
{

    public delegate void UpdateTextCallback(String text);

    public partial class SComUIForm : Form
    {
        private volatile Boolean isRunning;
        private volatile Boolean isReleased;

        private readonly BackgroundWorker backWorker;

        public SComUIForm()
        {
            InitializeComponent();

            backWorker = new BackgroundWorker();
            backWorker.WorkerReportsProgress = true;
            backWorker.DoWork += ListenSerialPort;
            backWorker.ProgressChanged += backWorker_ProgressChanged;
            backWorker.RunWorkerCompleted += backWorker_RunWorkerCompleted;

            sPort = new System.IO.Ports.SerialPort();
            sPort.PortName = "COM7";
            sPort.Parity = 0;
            sPort.BaudRate = 2400;
        }

        private void ListenSerialPort(object sender, System.EventArgs e)
        {
            BackgroundWorker worker = (BackgroundWorker) sender;

            do {
                Byte[] sPortData = new Byte[1];
                this.sPort.Read(sPortData, 0, 1);

                Int32 temp = (sPortData[0] - 32) * 5 / 9;

                worker.ReportProgress(temp);
                Thread.Sleep(200);
            } while (isRunning);
        }

        private void backWorker_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            lblTemp.Text = "Temp is " + e.ProgressPercentage.ToString() + " C.";
        }

        private void backWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            isReleased = true;
        }

        private void SComUIFrom_Load(object sender, EventArgs e)
        {
            sPort.Open();

            isRunning = true;
            isReleased = false;
            backWorker.RunWorkerAsync();
        }

        private void SComUIForm_Closing(object sender, FormClosingEventArgs e)
        {
            isRunning = false;
        }
    }
}
