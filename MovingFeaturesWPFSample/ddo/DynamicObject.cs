using Envitia.MapLink;
using System;

namespace MovingFeaturesWPFSample.ddo
{
    // The DynamicObject defines the properties of a Dynamic Data Object, and instantiates a display object that defines how the DDO is displayed on the map.
    public class DynamicObject : Envitia.MapLink.DDO.TSLNDynamicDataObject
    {
        // Custom DDO attributes.
        public Tuple<double, double>? From { get => field; internal set => field = value; }
        public Tuple<double, double>? Location { get => field; internal set => field = value; }
        public Tuple<double, double>? To { get => field; internal set => field = value; }
        public int Velocity { get => field; internal set => field = value; }
        public double Heading { get => field; set => field = value; }

        public override Envitia.MapLink.DDO.TSLNDisplayObject instantiateDO(Envitia.MapLink.DDO.TSLNDisplayType key, int dsID)
        {
            // Instantiate and return the appropriate DO, or return null.
            // Could use the dsID to identify a particular darwingSurface and use that
            // to instantiate a specific type of Display Object (eg. for main and overview screens)
            return new DisplayObject();
        }
    }
}
