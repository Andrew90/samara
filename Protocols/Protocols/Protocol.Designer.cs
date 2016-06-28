﻿namespace Protocols
{
    partial class MainForm
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
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.FileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ExitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ProtocolToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.UltrasonicControlProductsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.MainGridView = new System.Windows.Forms.DataGridView();
            this.menuStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.MainGridView)).BeginInit();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.FileToolStripMenuItem,
            this.ProtocolToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(744, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // FileToolStripMenuItem
            // 
            this.FileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ExitToolStripMenuItem});
            this.FileToolStripMenuItem.Name = "FileToolStripMenuItem";
            this.FileToolStripMenuItem.Size = new System.Drawing.Size(48, 20);
            this.FileToolStripMenuItem.Text = "Файл";
            // 
            // ExitToolStripMenuItem
            // 
            this.ExitToolStripMenuItem.Name = "ExitToolStripMenuItem";
            this.ExitToolStripMenuItem.Size = new System.Drawing.Size(108, 22);
            this.ExitToolStripMenuItem.Text = "Выход";
            // 
            // ProtocolToolStripMenuItem
            // 
            this.ProtocolToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.UltrasonicControlProductsToolStripMenuItem});
            this.ProtocolToolStripMenuItem.Name = "ProtocolToolStripMenuItem";
            this.ProtocolToolStripMenuItem.Size = new System.Drawing.Size(74, 20);
            this.ProtocolToolStripMenuItem.Text = "Протокол";
            // 
            // UltrasonicControlProductsToolStripMenuItem
            // 
            this.UltrasonicControlProductsToolStripMenuItem.Name = "UltrasonicControlProductsToolStripMenuItem";
            this.UltrasonicControlProductsToolStripMenuItem.Size = new System.Drawing.Size(280, 22);
            this.UltrasonicControlProductsToolStripMenuItem.Text = "Ультразвуковой контроль продукции";
            this.UltrasonicControlProductsToolStripMenuItem.Click += new System.EventHandler(this.UltrasonicControlProductsToolStripMenuItem_Click);
            // 
            // MainGridView
            // 
            this.MainGridView.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.MainGridView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.MainGridView.Location = new System.Drawing.Point(0, 24);
            this.MainGridView.Name = "MainGridView";
            this.MainGridView.Size = new System.Drawing.Size(744, 361);
            this.MainGridView.TabIndex = 1;
            this.MainGridView.CellClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.MainGridView_CellClick);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(744, 385);
            this.Controls.Add(this.MainGridView);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "MainForm";
            this.Text = "Протокол";
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.MainGridView)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem FileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem ExitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem ProtocolToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem UltrasonicControlProductsToolStripMenuItem;
        private System.Windows.Forms.DataGridView MainGridView;
    }
}
