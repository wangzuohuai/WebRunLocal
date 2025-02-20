using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace PluginExeDemo
{
    static class Program
    {
        /// <summary>
        /// 应用程序的主入口点。
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {
            if (args.Length <= 0)
            {
                MessageBox.Show("Please enter the startup parameters");
                Application.Exit();
                return;
            }

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1(args[0]));
        }
    }
}
