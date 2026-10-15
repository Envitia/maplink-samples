using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DrawingSurfacePanel
{
  // Interface to provide all the objects that the Drawing Surface Panel needs to add a layer to the surface.
  public interface IMapLayer
  {
    // Get the data layer for this map layer.
    Envitia.MapLink.TSLNDataLayer GetDataLayer();

    // Get a unique identifier for this map layer.
    string Identifier();

    // Perform any bespoke configuration required for this layer.
    void ConfigureMapLayer(Envitia.MapLink.TSLN2DDrawingSurface surface);
  }
}
