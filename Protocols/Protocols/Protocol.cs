using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Protocols.Requests;
using Protocols.Models;
using System.Diagnostics;

namespace Protocols
{
    public partial class MainForm : Form
    {
        protected int currentRow;
        protected delegate void TDo(Point p);
        protected TDo doProc;

        protected void SelectProtocolListTubes(Point p)
        {
            contextMenuStrip1.Show(p);
        }
       
        protected void SelectTube(Point p)
        {
            try
            {
                var row = MainGridView.Rows[currentRow];
                string param = ((DateTime)row.Cells[3].Value).ToString("yyMMddHHmmss");
                param += " " + row.Cells[0].Value.ToString();
                param += " " + row.Cells[1].Value.ToString();
                param += " " + row.Cells[2].Value.ToString();
                Process.Start(".\\ViewerData.exe", param);
            }
            catch { }
        }

        public MainForm()
        {
            InitializeComponent();
            saveFileDialog1.Filter = "xml|*.xml|*|*.*";
            saveFileDialog1.Title = "Сохранить Excel";           
        }

        private void UltrasonicControlProductsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DuringPeriodForm duringPeriodForm = new DuringPeriodForm();
            duringPeriodForm.ShowDialog();
            if (duringPeriodForm.DialogResult == DialogResult.OK)
            {
                MainGridUpdate(duringPeriodForm);
                doProc = SelectProtocolListTubes;
            }
        }

        private void MainGridUpdate(DuringPeriodForm duringPeriodForm)
        {
            var l = UltrasonicControlProducts.IDProtocolsTable(duringPeriodForm.from, duringPeriodForm.to);
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
            table.Columns[1].ColumnName = "№ протокола";
            table.Columns[2].ColumnName = "Время";
            table.Columns[3].ColumnName = "№ партии";
            table.Columns[4].ColumnName = "Количество труб";
            table.Columns[5].ColumnName = "Оператор";

            table.Columns[6].ColumnName = "Сплав";
            table.Columns[7].ColumnName = "Состояние поставки";
            table.Columns[8].ColumnName = "Нормативный документ";
            table.Columns[9].ColumnName = "Смена";
            table.Columns[10].ColumnName = "Шифр изделия";
            table.Columns[11].ColumnName = "№СОП";
            MainGridView.DataSource = null;
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

        private void MainGridView_CellMouseDown(object sender, DataGridViewCellMouseEventArgs e)
        {
            if (MouseButtons.Right == e.Button)
            {
                currentRow = e.RowIndex;
                doProc(Cursor.Position);
            }
        }

        private void protocol_ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ReportForm f = new ReportForm();
            try
            {
                var row = MainGridView.Rows[currentRow];

                f.ShowReport(
                    (int)row.Cells[0].Value
                      , (long)row.Cells[1].Value
                    , (DateTime)row.Cells[2].Value
                    , (int)row.Cells[4].Value
                    , (string)row.Cells[5].Value
                    );
            }
            catch
            { }
        }

        private void ListTubes(long protocol, string packet)
        {
            IEnumerable<TubesView> l = RequestListTubes.Get(protocol, packet);
            DataTable table = new DataTable();

            table.Columns.Add().DataType = typeof(long);
            table.Columns.Add().DataType = typeof(string);
            table.Columns.Add().DataType = typeof(string);
            table.Columns.Add().DataType = typeof(DateTime);

            table.Columns[0].ColumnName = "№ протокола";
            table.Columns[1].ColumnName = "№ партии";
            table.Columns[2].ColumnName = "№ трубы";
            table.Columns[3].ColumnName = "Время";

           

            MainGridView.DataSource = null;
            MainGridView.DataSource = table;

            MainGridView.Columns[3].DefaultCellStyle.Format = "dd.MM.yyyy HH:mm:ss";

            for (int i = 0; i < MainGridView.Columns.Count; ++i)
                MainGridView.Columns[i].ReadOnly = true;

            foreach (var i in l)
            {
                table.Rows.Add(protocol, packet, i.NumTube, i.TteTme);
            }
            MainGridView.Update();
        }

        private void listToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                doProc = SelectTube;
                var row = MainGridView.Rows[currentRow];
                ListTubes((long)row.Cells[1].Value, (string)row.Cells[3].Value);
            }
            catch{}
        }

        private void сохранитьExelToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var row = MainGridView.Rows[currentRow];
            var tubeParty = (string)row.Cells[3].Value;
            Text = tubeParty;
            saveFileDialog1.ShowDialog();
            if (saveFileDialog1.FileName != "")
            {
                SaveToXML.Save((int)row.Cells[1].Value, tubeParty, saveFileDialog1.FileName);
            }
        }
    }
}
