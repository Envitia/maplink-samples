using System;
using System.IO;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Configuration;
using Envitia.MapLink;

namespace CSharpMapViewer
{
  static class Program
  {
    /// <summary>
    /// The main entry point for the application.
    /// </summary>
    [STAThread]
    static void Main(string[] args)
    {
      //Initialise MapLink
      Application.EnableVisualStyles();
      Application.SetCompatibleTextRenderingDefault(false);

      string mapFile = "";
      bool accelerated = false ;
      if (args.Length > 0)
      {
          foreach ( string arg in args )
          {
              if (arg == "-dx")
                  accelerated = true;
              else
                  mapFile = arg;
          }
      }
      Application.Run(new MapViewerForm(mapFile, accelerated));

      TSLNDrawingSurface.cleanup();
    }
  }
}