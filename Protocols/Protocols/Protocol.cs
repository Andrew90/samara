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
                table.Columns.Add().DataType = typeof(long);
                table.Columns.Add().DataType = typeof(DateTime);

                table.Columns.Add().DataType = typeof(string);
       
                table.Columns.Add().DataType = typeof(int);
                table.Columns.Add().DataType = typeof(string);

               
                table.Columns.Add().DataType = typeof(string);
                table.Columns.Add().DataType = typeof(string);
                table.Columns.Add().DataType = typeof(string);
                table.Columns.Add().DataType = typeof(string);
                table.Columns.Add().DataType = typeof(string);
                table.Columns.Add().DataType = typeof(string);

                table.Columns[0].ColumnName = "ID";
                table.Columns[1].ColumnName = "№ Протокола";
                table.Columns[2].ColumnName = "Время";
                table.Columns[3].ColumnName = "Номер пакета";
                table.Columns[4].ColumnName = "Количество труб";
                table.Columns[5].ColumnName = "Оператор";

                table.Columns[6].ColumnName = "Сплав";
                table.Columns[7].ColumnName = "Состояние поставки";
                table.Columns[8].ColumnName = "Нормативный документ";
                table.Columns[9].ColumnName = "Смена";
                table.Columns[10].ColumnName = "Шифр изделия";
                table.Columns[11].ColumnName = "№СОП";


                MainGridView.DataSource = table;
                MainGridView.Columns[0].Width = 0;

                for (int i = 0; i < MainGridView.Columns.Count; ++i)
                    MainGridView.Columns[i].ReadOnly = true;

                foreach (var i in l)
                {
                    table.Rows.Add(i.ID, i.NumberProtocol, i.TteTme
                         , i.NumberPacket   
                        , i.Count, i.Operator
                        , i.Alloy            
                        , i.DeliveryStatus   
                        , i.NormativeDocument
                        , i.Gang             
                        , i.ProductCodeNumber                         
                        , i.Standart         
                        );
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
                      , (long)row.Cells[1].Value
                    , (DateTime)row.Cells[2].Value
                    , (int)row.Cells[4].Value
                    , (string)row.Cells[5].Value
                    );
            }
            catch (System.InvalidCastException )
            { }
        }
    }
}
