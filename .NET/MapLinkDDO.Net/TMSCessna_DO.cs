using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

using Envitia.MapLink;
using Envitia.MapLink.DDO;

namespace CSharpDDOSample
{
  class TMSCessna_DO : AircraftDO
  {
    private static int m_symbolID = 6002;

#if GDI_PLUS_DRAWING
    protected override Color foregroundColour
    {
      get
      {
        return Color.Black;
      }
    }

    protected override Color backgroundColour
    {
      get
      {
        return Color.Black;
      }
    }

    protected override Point[] points
    {
      get
      {
        return null;
      }
    }
#else
    protected override int foregroundColour
    {
      get
      {
        return 0;
      }
    }

    protected override int backgroundColour
    {
      get
      {
        return 0;
      }
    }

    protected override int[] points
    {
      get
      {
        return null;
      }
    }
#endif

    public override bool draw(TSLNRenderingInterface d_surface, TSLNEnvelope d_extent)
    {
      TSLNCoord pos = ddo.position;

      int wndX, wndY;

      d_surface.TMCToDU(pos.x, pos.y, out wndX, out wndY);

      double resolutionX = 0.0, resolutionY = 0.0;
      d_surface.screenResolution(ref resolutionX, ref resolutionY); ;  // TMC units / pixel.
      double size = 20.0 * resolutionX;

      double hdg = ((Aircraft)ddo).heading;

      hdg *= -1;

      d_surface.setupSymbolAttributes(m_symbolID, Color.FromArgb(0, 0, 153), size, TSLNDimensionUnits.TSLNDimensionUnitsUndefined, 0, int.MaxValue, hdg);
      d_surface.drawSymbol(pos, (int)size, null);

      return true;
    }
  }
}
