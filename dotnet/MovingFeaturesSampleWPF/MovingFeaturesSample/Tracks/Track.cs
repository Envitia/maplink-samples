using System;
using System.Drawing;

namespace ContourApp.Tracks
{
  public class Track
  {
    public uint Id { get; set; }

    public Tuple<double, double> From { get; set; }
    public Tuple<double, double> To { get; set; }
    public int Velocity { get; set; }
    public Tuple<double, double> Location { get; internal set; }

    public bool Hostile { get; set; } = false;

    private Envitia.MapLink.TrackManager.TSLNTrack MapLinkTrack { get; set; }

    private Envitia.MapLink.TSLNSymbol CreatePointSymbol(int symbolId, int size, Color colour)
    {
      var symbol = new Envitia.MapLink.TSLNSymbol(0, 0, 0, 0);
      symbol.setRendering(new Envitia.MapLink.TSLNRenderingAttributes
      {
        symbolStyle = symbolId,
        symbolColour = colour.ToArgb(),
        symbolSizeFactor = size,
        symbolSizeFactorUnits = Envitia.MapLink.TSLNDimensionUnits.TSLNDimensionUnitsPixels,
        symbolRotatable = Envitia.MapLink.TSLNSymbolRotation.TSLNSymbolRotationEnabled,
      });
      return symbol;
    }

    public Envitia.MapLink.TrackManager.TSLNTrack GetTrack()
    {
      if (MapLinkTrack == null)
      {
        var pointSymbol = Envitia.MapLink.TrackManager.TSLNTrackPointSymbol.create();
        
        pointSymbol.addSymbolEntity(CreatePointSymbol(5, 20, Hostile ? System.Drawing.Color.OrangeRed : System.Drawing.Color.DodgerBlue), true);
        pointSymbol.rotateEntityToTrackHeading(0);

        MapLinkTrack = Envitia.MapLink.TrackManager.TSLNTrack.create(pointSymbol);
      }
      return MapLinkTrack;
    }

  }
}
