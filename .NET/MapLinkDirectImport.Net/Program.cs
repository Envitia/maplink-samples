using System;
using System.IO;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Configuration;
using Envitia.MapLink;

namespace CSharpDirectImportSample
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

      if (args.Length > 0)
      {
        for( uint i = 0; i < args.Length; ++i )
        {
          string arg = args[i];
          if (arg == "-home" ||
              arg == "/home" )
          {
            if( i != args.Length - 1 )
            {
              ++i;
              string newMaplHome = args[i];
              if( newMaplHome.Length == 0 )
              {
                continue;
              }
              if( newMaplHome[0] == '.' )
              {
                newMaplHome = AppDomain.CurrentDomain.BaseDirectory + "/" + args[i];
              }
              TSLNUtilityFunctions.setMapLinkHome(newMaplHome);
            }
          }
          else
          {
            mapFile = arg;
          }
        }
      }
      Application.Run(new MapViewerForm(mapFile));

      TSLNDrawingSurface.cleanup();
    }
  }
}