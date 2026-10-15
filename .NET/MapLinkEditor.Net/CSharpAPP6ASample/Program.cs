using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Configuration;
using Envitia.MapLink;

namespace CSharpAPP6ASample
{
  static class Program
  {
    /// <summary>
    /// The main entry point for the application.
    /// </summary>
    [STAThread]
    static void Main(string[] args)
    {
      if ( args.Length == 2 && ( args[0] == "-i" || args[0] == "-I" ) )
      {
        string editorINIFile = args[1];
        TSLNProfileHelper.setDefaultFilename( editorINIFile ) ;
      }

      //Initialise MapLink
      TSLNDrawingSurface.loadStandardConfig();
      Application.EnableVisualStyles();
      Application.SetCompatibleTextRenderingDefault(false);
      Application.Run(new MapViewerForm());

      TSLNDrawingSurface.cleanup();
    }
  }
}