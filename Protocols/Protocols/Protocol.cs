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
                var l = UltrasonicControlProducts.IDProtocolsTable(duringPeriodForm.from, duringPeriodForm.to);
                if (null != l)
                {
                    DataTable table = new DataTable();

                    table.Columns.Add().DataType = typeof(int);
                    table.Columns.Add().DataType = typeof(DateTime);
                    table.Columns.Add().DataType = typeof(int);

                    table.Columns[0].ColumnName = "№ Протокола";
                    table.Columns[1].ColumnName = "Время";
                    table.Columns[2].ColumnName = "Количество";

                    MainGridView.DataSource = table;

                    for (int i = 0; i < 3; ++i)
                        MainGridView.Columns[i].ReadOnly = true;
                    foreach (var i in l)
                    {
                        table.Rows.Add(i.ID, i.TteTme, i.Count);
                    }
                    MainGridView.Update();
                }
            }
        }

        private void MainGridView_CellClick(object sender, DataGridViewCellEventArgs e)
        {
            Text = (sender as DataGridView).Rows[e.RowIndex].Cells[0].Value.ToString();
            int t = (int)(sender as DataGridView).Rows[e.RowIndex].Cells[0].Value;
        }
    }
}
