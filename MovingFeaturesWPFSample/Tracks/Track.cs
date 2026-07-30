using System;
using System.Drawing;

namespace MovingFeaturesWPFSample.Tracks
{
    // Wrapper class for TSLNTrack.
    public class Track
    {
        public uint Id { get; set; }

        public Tuple<double, double>? From { get; set; }
        public Tuple<double, double>? To { get; set; }
        public int Velocity { get; set; }
        public Tuple<double, double>? Location { get; internal set; }

        private Envitia.MapLink.TrackManager.TSLNTrack? MapLinkTrack { get; set; }

        static private System.Drawing.Color TrackColour
        {
            get
            {
                return System.Drawing.Color.DodgerBlue;
            }
        }

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
                // Lazy-create a new track object.

                // Define how the track will be displayed on the map.
                var pointSymbol = Envitia.MapLink.TrackManager.TSLNTrackPointSymbol.create();
                pointSymbol.addSymbolEntity(CreatePointSymbol(5, 20, TrackColour), true);
                pointSymbol.rotateEntityToTrackHeading(0);

                MapLinkTrack = Envitia.MapLink.TrackManager.TSLNTrack.create(pointSymbol);
            }
            return MapLinkTrack;
        }

    }
}
