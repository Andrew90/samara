using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Protocols.Models;
using Protocols.Requests;

namespace Protocols
{
    public partial class ReportForm : Form
    {
        public ReportForm()
        {
            InitializeComponent();
        }
        
        public void ShowReport(int id, DateTime tdeTme, int count, string Operator)
        {
            this.reportViewer1.LocalReport.DataSources.Clear();
            this.reportViewer1.LocalReport.ReportPath = "ProtocolReport.rdlc";

            var header = UltrasonicControlProducts.HeaderProtocol(id, tdeTme, count, Operator);

            Microsoft.Reporting.WinForms.ReportDataSource dataset1 = new Microsoft.Reporting.WinForms.ReportDataSource("DataSet1", header); // set the datasource
            this.reportViewer1.LocalReport.DataSources.Add(dataset1);

            var body = UltrasonicControlProducts.BodyProtocol(id);

            Microsoft.Reporting.WinForms.ReportDataSource dataset2 = new Microsoft.Reporting.WinForms.ReportDataSource("DataSet2", body); // set the datasource
            this.reportViewer1.LocalReport.DataSources.Add(dataset2);

            this.reportViewer1.LocalReport.Refresh();
            this.reportViewer1.RefreshReport();

            Show();
        }
    }
}
