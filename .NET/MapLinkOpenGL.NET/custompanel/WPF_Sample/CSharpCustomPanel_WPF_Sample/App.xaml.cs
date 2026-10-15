using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;

namespace CSharpCustomPanel_WPF_Sample
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
  {
    private void Application_Startup(object sender, StartupEventArgs e)
    {
      MainWindow wnd = new MainWindow();
      if (e.Args.Length == 1)
        wnd.openInitialMap(e.Args[0]);
      wnd.Show();
    }
  }
}
