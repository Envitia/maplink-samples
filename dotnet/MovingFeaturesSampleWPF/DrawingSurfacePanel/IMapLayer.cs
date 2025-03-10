using System;

namespace DrawingSurfacePanel
{
  // Interface to provide all the objects that the Drawing Surface Panel needs to add a layer to the surface.
  public interface IMapLayer
  {
    // Get the data layer for this map layer.
    Envitia.MapLink.TSLNDataLayer GetDataLayer();

    // Get the map layer's user interaction handlers.
    System.Collections.Generic.List<IUiHandler> GetUiHandlers(Envitia.MapLink.TSLNDrawingSurface surface);

    // Get a unique identifier for this map layer.
    string Identifier();

    // Perform any bespoke configuration required for this layer.
    void ConfigureMapLayer(Envitia.MapLink.TSLNDrawingSurface surface);
  }
}
