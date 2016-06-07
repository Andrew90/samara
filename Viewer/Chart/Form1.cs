using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using DLL;
using System.Runtime.InteropServices;


namespace Chart
{
    public partial class Charts : Form
    {
        //[DllImport("user32.dll", CharSet = CharSet.Auto)]
        //static extern IntPtr SendMessage(IntPtr hWnd, UInt32 Msg, IntPtr wParam, IntPtr lParam);
        //[DllImport("user32.dll")]
        //static extern bool InvalidateRect(IntPtr hWnd, IntPtr lpRect, bool bErase);
        //
        //[DllImport("user32.dll")]
        //static extern bool RedrawWindow(IntPtr hWnd, IntPtr lprcUpdate, IntPtr hrgnUpdate, uint flags); 

        private const int WM_GETMINMAXINFO = 0x0024;
        private const int WM_SIZE          = 0x0005;
        private const int WM_PAINT         = 0x000F;

        private const int countZones = 240;
        private const int countSensors = 8;

        public interface XInterface
        {
            //GCHandle data { get; }
            void Create();
            void Destroy();
            void Size(uint resizing, int left, int top, int width, int height);
            unsafe void SetData(int c, double* x);
            unsafe void SetStatus(int c, sbyte* x);
            void SetCountZones(int x);
        };

        class XChart<T> : XInterface where T: IChart, new()
        {
            public T t = new T();
            Panel panel = new Panel();           
            Charts owner;
            public XChart(Charts o)
            {
                Create();
                owner = o;
                o.Controls.Add(panel);
            }
            //public GCHandle data { get { return GCHandle.Alloc(t); } }
            public void Create() { t.Create(); }
            public void Destroy() { t.Destroy(); }
            public void Size(uint resizing, int left, int top, int width, int height) 
            {
                panel.Left = left;
                panel.Top = top;
                panel.Width = width;
                panel.Height = height;
                t.Size(panel.Handle, resizing, width, height); 
            }
            public unsafe void SetData(int c, double* x) { t.SetData(c, x); }
            public unsafe  void SetStatus(int c, sbyte* x) { t.SetStatus(c, x); }
            public  void SetCountZones(int x) { t.SetCountZones(x); }
        };

        public struct POINTAPI
        {
            public int x;
            public int y;
        }

        public struct MINMAXINFO
        {
            public POINTAPI ptReserved;
            public POINTAPI ptMaxSize;
            public POINTAPI ptMaxPosition;
            public POINTAPI ptMinTrackSize;
            public POINTAPI ptMaxTrackSize;
        }

        public struct PointShort
        {           
            public short Width;
            public short Height;
        };

        IList<XInterface> charts = new List<XInterface>();
        public Charts()
        {
            InitializeComponent();           
        }

        protected override void WndProc(ref System.Windows.Forms.Message m)
        {
            switch (m.Msg)
            {
                case WM_GETMINMAXINFO:
                    {
                        MINMAXINFO mmi = (MINMAXINFO)m.GetLParam(typeof(MINMAXINFO));
                        mmi.ptMinTrackSize.y = 400;
                        System.Runtime.InteropServices.Marshal.StructureToPtr(mmi, m.LParam, true);
                    }
                    break;
                case WM_SIZE:
                    {
                        int width = m.LParam.ToInt32();
                        int height = width >> 16;
                        width &= 0xffff;
                        int dY = height - 50;
                        dY /= 4;
                        int Y = 0;                       
                        uint resizing = (uint)m.WParam.ToInt32();
                        foreach (var i in charts)
                        {
                            i.Size(resizing, 0, Y, width, dY);
                            Y += dY;
                        }
                    }
                    return;                
            }
            base.WndProc(ref m);
        }

        void SetDataChart<T>(T t) where T : XInterface
        {
            Random rand = new Random();
            unsafe
            {
                fixed (double* data = new double[countZones])
                {
                    for (int sensor = 0; sensor < countSensors; ++sensor)
                    {
                        for (int i = 0; i < countZones; ++i)
                        {
                            data[i] = i % 200;
                        }
                        t.SetData(sensor, data);
                    }
                }

                fixed (sbyte* status = new sbyte[countZones])
                {
                    for (int sensor = 0; sensor < countSensors; ++sensor)
                    {
                        for (int i = 0; i < countZones; ++i)
                        {
                            status[i] = (sbyte)(rand.Next(10));
                        }
                        t.SetStatus(sensor, status);
                    }
                }
            }
            t.SetCountZones(240);
        }
        void SetDataChart(XChart<ThicknessChart> t)
        {
            Random rand = new Random();
            unsafe
            {
                fixed (double* data = new double[countZones])
                {
                    for (int sensor = 0; sensor < 2; ++sensor)
                    {
                        for (int i = 0; i < countZones; ++i)
                        {
                            data[i] = 0.05 * (1 + i);
                        }
                        t.SetData(sensor, data);
                    }
                }

                fixed (sbyte* status = new sbyte[countZones])
                {
                    for (int i = 0; i < countZones; ++i)
                    {
                        status[i] = (sbyte)(rand.Next(10));
                    }
                    t.SetStatus(0, status);
                }
            }
            t.SetCountZones(240);
        }

        private void Charts_Load(object sender, EventArgs e)
        {
            Additional.ChartsInit();
            charts.Add(new XChart<CrossChart>(this));
            charts.Add(new XChart<LongChart>(this));
            charts.Add(new XChart<ThicknessChart>(this));
        }

        private void Charts_FormClosing(object sender, FormClosingEventArgs e)
        {
            charts.ToList().ForEach(x => x.Destroy());
        }

        private void button1_Click(object sender, EventArgs e)
        {
            SetDataChart(charts.Where(x => x is XChart<CrossChart>).First());
            Additional.RepaintWindow(Handle);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            SetDataChart(charts.Where(x => x is XChart<LongChart>).First());
            Additional.RepaintWindow(Handle);
        }

        private void button3_Click(object sender, EventArgs e)
        {
            XChart<ThicknessChart> z = charts.Where(x => x is XChart<ThicknessChart>).First() as XChart<ThicknessChart>;
            z.t.SetMinMaxYAxes(0, 12);
            SetDataChart(z);
            Additional.RepaintWindow(Handle);
        }  
    }
}
