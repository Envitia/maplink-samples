using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Configuration;

namespace CSharpTerrainSample
{
  static class Program
  {
    /// <summary>
    /// The main entry point for the application.
    /// </summary>
    [STAThread]
    static void Main(string[] args)
    {
      string mapFile = "";
      string terrainDB = "";
      for (int i = 0; i < args.Length; i++)
      {
        if (args[i].ToLower().CompareTo("/map") == 0 ||
            args[i].ToLower().CompareTo("-map") == 0)
        {
          i++;
          mapFile = args[i];
        }
        else if (args[i].ToLower().CompareTo("/tdb") == 0 ||
                 args[i].ToLower().CompareTo("-tdb") == 0)
        {
          i++;
          terrainDB = args[i];
        }
      }

      Configuration config = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
      MRUSection mruSectionMap = (MRUSection)config.Sections["MRUMap"];
      if (mruSectionMap == null)
      {
        mruSectionMap = new MRUSection();
        config.Sections.Add("MRUMap", mruSectionMap);
      }

      MRUSection mruSectionTerrain = (MRUSection)config.Sections["MRUTerrain"];
      if (mruSectionTerrain == null)
      {
        mruSectionTerrain = new MRUSection();
        config.Sections.Add("MRUTerrain", mruSectionTerrain);
      }

      TerrainOptionsSection optsSection = (TerrainOptionsSection)config.Sections["TerrainOptions"];
      if (optsSection == null)
      {
        optsSection = new TerrainOptionsSection();
        config.Sections.Add("TerrainOptions", optsSection);
      }

      Application.EnableVisualStyles();
      Application.SetCompatibleTextRenderingDefault(false);
      Application.Run(new TerrainViewer(mruSectionMap, mruSectionTerrain, optsSection, mapFile, terrainDB));
      try
      {
        config.Save();
      }
      catch (Exception)
      {
      }
    }
  }
}