using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace ScopeViewTest {
    public partial class ScopeViewTest : Form {

        private Random rnd = new Random();
        private UInt32 sampleCount;

        public ScopeViewTest() {
            InitializeComponent();
        }

        private void ScopeViewTest_onLoad(object sender, EventArgs e) {
            sampleCount = 0;

            timer.Enabled = true;
            timer.Start();
        }

        private void ScopeViewTest_timer_onTick(object sender, EventArgs e) {
            UInt32 sample = (UInt32)ClientSize.Height / 2 - (UInt32)((Math.Sin((sampleCount / 10.0 / Math.PI)) + rnd.NextDouble() - 0.5) * ClientSize.Height / 3);
            sampleCount++;

            // todo: update view here
            UInt32[] samples = new UInt32[1];
            samples[0] = sample;
            scopeView.submitNewSample(samples);
        }
    }
}