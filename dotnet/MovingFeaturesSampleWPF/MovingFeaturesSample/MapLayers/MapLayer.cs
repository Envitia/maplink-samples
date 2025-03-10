using System;

namespace ContourApp.MapLayers
{
  /// <summary>
  /// Base class for all map layer visualisations.
  /// </summary>
  public abstract class MapLayer : DrawingSurfacePanel.IMapLayer
  {
    public string Property { get; set; } = "";
    public string DataLocation { get; set; } = "";
    public bool IsFoundationLayer { get; set; } = false;
    public string Name { get; set; } = "";
    public int Opacity { get; set; } = 220;

    public DrawingSurfacePanel.IPanel Panel { get; set; }

    public virtual string Identifier()
    {
      const string delim = ".";
      return Property + delim + System.IO.Path.GetFileName(DataLocation);
    }

    public abstract Envitia.MapLink.TSLNDataLayer GetDataLayer();

    public virtual void Initialise()
    {

    }

    /// <summary>
    /// Configure the map layer.
    /// </summary>
    /// <param name="surface">The surface that the layer has been added to.</param>
    /// <param name="visible">The layer visibility in the surface.</param>
    public abstract void ConfigureMapLayer(Envitia.MapLink.TSLN2DDrawingSurface surface, bool visible);

    void DrawingSurfacePanel.IMapLayer.ConfigureMapLayer(Envitia.MapLink.TSLNDrawingSurface surface)
    {
      bool got = surface.getDataLayerProps(Identifier(), Envitia.MapLink.TSLNPropertyEnum.TSLNPropertyVisible, out int visible);
      ConfigureMapLayer(surface, got && visible != 0);
    }

    Envitia.MapLink.TSLNDataLayer DrawingSurfacePanel.IMapLayer.GetDataLayer()
    {
      return GetDataLayer();
    }

    public virtual System.Collections.Generic.List<DrawingSurfacePanel.IUiHandler> GetUiHandlers(Envitia.MapLink.TSLNDrawingSurface drawingSurface)
    {
      return null;
    }

    string DrawingSurfacePanel.IMapLayer.Identifier()
    {
      return Identifier();
    }
  }
}
