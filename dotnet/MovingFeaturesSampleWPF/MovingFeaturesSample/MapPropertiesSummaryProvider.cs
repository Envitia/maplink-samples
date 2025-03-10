using System;

namespace MovingFeaturesSample
{
  /// <summary>
  /// Provides a text summary of the properties at a map location, for display in a tooltip
  /// </summary>
  internal class MapPropertiesSummaryProvider : DrawingSurfacePanel.TooltipTextProvider
  {
    public Envitia.MapLink.TSLNDrawingSurface DrawingSurface { get; set; }

    public override string ProvideText(int x, int y)
    {
      if (DrawingSurface is null)
      {
        throw new Exception("MapPropertiesSummaryProvider not properly configured");
      }

      DrawingSurface.DUToMU(x, y, out double mux, out double muy);
      DrawingSurface.MUToLatLong(mux, muy, out double latitude, out double longitude);

      return GetPropertiesSummary(mux, muy, latitude, longitude);
    }

    private string GetPropertiesSummary(double mux, double muy, double latitude, double longitude)
    {
      return $"Lat: {latitude:0.00}°\nLon: {longitude:0.00}°\n";
    }
  }
}
