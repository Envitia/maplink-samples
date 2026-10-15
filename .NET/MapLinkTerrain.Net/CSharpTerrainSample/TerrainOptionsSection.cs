using System;
using System.Collections.Generic;
using System.Text;
using System.Configuration;

namespace CSharpTerrainSample
{
  public class TerrainOptionsSection : ConfigurationSection
  {
    private ConfigurationPropertyCollection m_properties;

    private ConfigurationProperty m_manualScale = 
                new ConfigurationProperty("manual_scale", typeof(double), 2000.0);

    private ConfigurationProperty m_autoScale = 
                new ConfigurationProperty("auto_scale", typeof(bool), false); 

    private ConfigurationProperty m_terrainScale = 
                new ConfigurationProperty("terrain_scale", typeof(double), 3.0); 

    private ConfigurationProperty m_bathymetryScale = 
                new ConfigurationProperty("bathymetry_scale", typeof(double), 3.0); 

    private ConfigurationProperty m_bathymetry = 
                new ConfigurationProperty("bathymetry", typeof(bool), true); 

    private ConfigurationProperty m_startHeight = 
                new ConfigurationProperty("start_height", typeof(double), 6.0); 

    private ConfigurationProperty m_endHeight = 
                new ConfigurationProperty("end_height", typeof(double), 0.0);

    private ConfigurationProperty m_losAreaRangeMU =
                new ConfigurationProperty("los_area_range_mu", typeof(double), 999999.0);

    private ConfigurationProperty m_losGridCells =
                new ConfigurationProperty("los_area_grid_cells", typeof(int), 2);

    private ConfigurationProperty m_useAccurateLos =
                new ConfigurationProperty("los_area_accurate", typeof(bool), false);

    private ConfigurationProperty m_darkShaded =
                new ConfigurationProperty("los_area_dark_shaded", typeof(bool), false);

    private ConfigurationProperty m_startHeightAboveGround =
                new ConfigurationProperty("los_area_start_height_above_ground", typeof(bool), false);

    private ConfigurationProperty m_curveOfEarth =
                new ConfigurationProperty("los_area_curve_of_earth", typeof(bool), false);

    public TerrainOptionsSection()
    {
      // Property initialization
      m_properties = new ConfigurationPropertyCollection();

      m_properties.Add(m_manualScale);
      m_properties.Add(m_autoScale);
      m_properties.Add(m_terrainScale);
      m_properties.Add(m_bathymetryScale);
      m_properties.Add(m_bathymetry);
      m_properties.Add(m_startHeight);
      m_properties.Add(m_endHeight);
      m_properties.Add(m_losAreaRangeMU);
      m_properties.Add(m_losGridCells);
      m_properties.Add(m_useAccurateLos);
      m_properties.Add(m_darkShaded);
      m_properties.Add(m_startHeightAboveGround);
      m_properties.Add(m_curveOfEarth);
    }

    protected override ConfigurationPropertyCollection Properties
    {
      get
      {
        return m_properties;
      }
    }

    public double ManualScale
    {
      get
      {
        return (double)this["manual_scale"];
      }
      set
      {
        this["manual_scale"] = value;
      }
    }

    public bool AutoScale
    {
      get
      {
        return (bool)this["auto_scale"];
      }
      set
      {
        this["auto_scale"] = value;
      }
    }

    public double TerrainScale
    {
      get
      {
        return (double)this["bathymetry_scale"];
      }
      set
      {
        this["bathymetry_scale"] = value;
      }
    }

    public double BathymetryScale
    {
      get
      {
        return (double)this["terrain_scale"];
      }
      set
      {
        this["terrain_scale"] = value;
      }
    }

    public bool Bathymetry
    {
      get
      {
        return (bool)this["bathymetry"];
      }
      set
      {
        this["bathymetry"] = value;
      }
    }

    public double StartHeight
    {
      get
      {
        return (double)this["start_height"];
      }
      set
      {
        this["start_height"] = value;
      }
    }

    public double EndHeight
    {
      get
      {
        return (double)this["end_height"];
      }
      set
      {
        this["end_height"] = value;
      }
    }

    public double LOSAreaRangeMU
    {
      get
      {
        return (double)this["los_area_range_mu"];
      }
      set
      {
        this["los_area_range_mu"] = value;
      }
    }

    public int LOSAreaGridCells
    {
      get
      {
        return (int)this["los_area_grid_cells"];
      }
      set
      {
        this["los_area_grid_cells"] = value;
      }
    }

    public bool UseAccurateLos
    {
      get
      {
        return (bool)this["los_area_accurate"];
      }
      set
      {
        this["los_area_accurate"] = value;
      }
    }

    public bool DarkShaded
    {
      get
      {
        return (bool)this["los_area_dark_shaded"];
      }
      set
      {
        this["los_area_dark_shaded"] = value;
      }
    }

    public bool StartHeightAboveGround
    {
      get
      {
        return (bool)this["los_area_start_height_above_ground"];
      }
      set
      {
        this["los_area_start_height_above_ground"] = value;
      }
    }

    public bool CurveOfEarth
    {
      get
      {
        return (bool)this["los_area_curve_of_earth"];
      }
      set
      {
        this["los_area_curve_of_earth"] = value;
      }
    }
  }
}
