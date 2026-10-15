using System;

namespace MovingFeaturesWPFSample.ddo
{
    // The DisplayObject defines how the Dynamic Data Object is rendered on the map. It is instantiated by the DynamicObject class.
    public class DisplayObject : Envitia.MapLink.DDO.TSLNDisplayObject
    {

        public DisplayObject() : base(true)
        {
            setSize(0, 0, 0, 0);
        }

        static private System.Drawing.Color DdoColour
        {
            get
            {
                return System.Drawing.Color.OrangeRed;
            }
        }

        // Called by the DDO SDK to render the Dynamic Data Object on the map.
        // The DDO SDK will call this method for each Dynamic Data Object that is added to the DDO layer.
        public override bool draw(Envitia.MapLink.TSLNRenderingInterface renderer, Envitia.MapLink.TSLNEnvelope extent)
        {
            var dynamicObject = (DynamicObject)ddo;
            renderer.setupSymbolAttributes(5, DdoColour, 15, Envitia.MapLink.TSLNDimensionUnits.TSLNDimensionUnitsPixels, 15, 25, (-dynamicObject.Heading) * (System.MathF.PI / 180));
            return renderer.drawSymbol(ddo.position, 15);
        }

    }
}
