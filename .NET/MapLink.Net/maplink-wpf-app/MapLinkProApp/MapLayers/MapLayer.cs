using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MapLinkProApp.MapLayers
{
  /// <summary>
  /// Base class for all map layer visualisations.
  /// </summary>
  public abstract class MapLayer : DrawingSurfacePanel.IMapLayer
  {
    public const double ALL_DEPTHS = Double.MaxValue;

    public double Depth { get; set; } = ALL_DEPTHS;
    public string Property { get; set; } = "";
    public string FeatureType { get; set; } = "";
    public string DataLocation { get; set; } = "";
    public bool IsFoundationLayer { get; set; } = false;

    public int Opacity { get; set; } = 100;

    public DrawingSurfacePanel.IPanel Panel { get; set; }

    public virtual string Identifier()
    {
      const string delim = ".";
      return Depth.ToString() + delim + Property + delim + FeatureType + delim + System.IO.Path.GetFileName(DataLocation);
    }

    /// <summary>
    /// Suggested Z order position of this layer.
    /// </summary>
    public int Z { get; set; }

    public abstract Envitia.MapLink.TSLNDataLayer GetDataLayer();

    /// <summary>
    /// Configure the map layer.
    /// </summary>
    /// <param name="surface">The surface that the layer has been added to.</param>
    /// <param name="visible">The layer visibility in the surface.</param>
    /// <param name="depth">The selected depth to display.</param>
    public abstract void ConfigureMapLayer(Envitia.MapLink.TSLN2DDrawingSurface surface, bool visible, double depth);

    void DrawingSurfacePanel.IMapLayer.ConfigureMapLayer(Envitia.MapLink.TSLN2DDrawingSurface surface)
    {
      int visible = 0;
      bool got = surface.getDataLayerProps(Identifier(), Envitia.MapLink.TSLNPropertyEnum.TSLNPropertyVisible, out visible);

      ConfigureMapLayer(surface, got && visible != 0, Depth);
    }

    Envitia.MapLink.TSLNDataLayer DrawingSurfacePanel.IMapLayer.GetDataLayer()
    {
      return GetDataLayer();
    }

    string DrawingSurfacePanel.IMapLayer.Identifier()
    {
      return Identifier();
    }
  }
}
