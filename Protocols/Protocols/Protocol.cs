using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Protocols.Requests;

namespace Protocols
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
        }

        private void UltrasonicControlProductsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DuringPeriodForm duringPeriodForm = new DuringPeriodForm();
            duringPeriodForm.ShowDialog();
            if (duringPeriodForm.DialogResult == DialogResult.OK)
            {
                MainGridUpdate(duringPeriodForm);
            }
        }

        private void MainGridUpdate(DuringPeriodForm duringPeriodForm)
        {
            var l = UltrasonicControlProducts.IDProtocolsTable(duringPeriodForm.from, duringPeriodForm.to);
            if (null != l)
            {
                DataTable table = new DataTable();

                table.Columns.Add().DataType = typeof(int);
                table.Columns.Add().DataType = typeof(DateTime);
                table.Columns.Add().DataType = typeof(int);
                table.Columns.Add().DataType = typeof(string);

                table.Columns[0].ColumnName = "№ Протокола";
                table.Columns[1].ColumnName = "Время";
                table.Columns[2].ColumnName = "Количество труб";
                table.Columns[3].ColumnName = "Оператор";

                MainGridView.DataSource = table;

                for (int i = 0; i < MainGridView.Columns.Count; ++i)
                    MainGridView.Columns[i].ReadOnly = true;

                foreach (var i in l)
                {
                    table.Rows.Add(i.ID, i.TteTme, i.Count, i.Operator);
                }
                MainGridView.Update();
            }
        }

        private void MainGridView_CellClick(object sender, DataGridViewCellEventArgs e)
        {
            ReportForm f = new ReportForm();

            var row = (sender as DataGridView).Rows[e.RowIndex];

            try
            {

                f.ShowReport(
                    (int)row.Cells[0].Value
                    , (DateTime)row.Cells[1].Value
                    , (int)row.Cells[2].Value
                    , (string)row.Cells[3].Value
                    );
            }
            catch (System.InvalidCastException err)
            { }
        }
    }
}
