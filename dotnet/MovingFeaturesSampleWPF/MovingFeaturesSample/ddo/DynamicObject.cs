using Envitia.MapLink;
using System;

namespace MovingFeaturesSample.MapLayers
{
  public class DynamicObject : Envitia.MapLink.DDO.TSLNDynamicDataObject
  {
    public Tuple<double, double> From { get; internal set; }
    public Tuple<double, double> Location { get; internal set; }
    public Tuple<double, double> To { get; internal set; }
    public int Velocity { get; internal set; }
    public double Heading { get; set; }

    public override Envitia.MapLink.DDO.TSLNDisplayObject instantiateDO(Envitia.MapLink.DDO.TSLNDisplayType key, int dsID)
    {
      // Instantiate and return the appropriate DO, or return 0
      // Could use the dsID to identify a particular darwingSurface and use that
      // to instantiate a specific type of Display Object (eg. for main and overview screens)

      return new DisplayObject();
    }
  }
}
