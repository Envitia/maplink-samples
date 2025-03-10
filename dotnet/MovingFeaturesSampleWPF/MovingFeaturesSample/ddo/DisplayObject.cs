using System;

namespace ContourApp.MapLayers
{
  public class DisplayObject : Envitia.MapLink.DDO.TSLNDisplayObject
  {

    public DisplayObject() : base(true)
    {
      setSize(0, 0, 0, 0);
    }

    protected System.Drawing.Color foregroundColour
    {
      get
      {
        return System.Drawing.Color.OrangeRed;
      }
    }

    public override bool draw(Envitia.MapLink.TSLNRenderingInterface renderer, Envitia.MapLink.TSLNEnvelope extent) 
    {
      var dynamicObject = (DynamicObject)ddo;
      renderer.setupSymbolAttributes(5, foregroundColour, 15, Envitia.MapLink.TSLNDimensionUnits.TSLNDimensionUnitsPixels, 15, 25, (-dynamicObject.Heading) * (System.MathF.PI / 180));
      return renderer.drawSymbol(ddo.position, 15);
    }

  }
}
