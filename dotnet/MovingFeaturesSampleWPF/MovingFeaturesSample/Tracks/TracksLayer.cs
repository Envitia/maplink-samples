using System;
using MovingFeaturesSample.MapLayers;

namespace MovingFeaturesSample.Tracks
{
    /// <summary>
    ///  TracksLayer demonstrates the fundamentals of creating tracks using the Envitia.MapLink.TrackManager SDK.
    /// </summary>
    public class TracksLayer : MapLayer
    {
        public Envitia.MapLink.TrackManager.TSLNTrackDisplayManager TrackDisplayManager { get; } = Envitia.MapLink.TrackManager.TSLNTrackDisplayManager.create();

        private System.Collections.Generic.List<Track> Tracks { get; } = new System.Collections.Generic.List<Tracks.Track>();

        private static System.Windows.Threading.DispatcherTimer DispatcherTimer = new System.Windows.Threading.DispatcherTimer(System.Windows.Threading.DispatcherPriority.Render);

        private Envitia.MapLink.TSLN2DDrawingSurface DrawingSurface { get; set; }

        public Envitia.MapLink.TSLNDataLayer FoundationLayer { get; set; }

        public override string Identifier()
        {
            return "TracksLayer";
        }

        /// <summary>
        /// Creates a number of tracks moving between randomly assigned departure and destination airports.
        /// </summary>
        /// <param name="surface"></param>
        /// <param name="visible"></param>
        /// <exception cref="Exception"></exception>
        public override void ConfigureMapLayer(Envitia.MapLink.TSLN2DDrawingSurface surface, bool visible)
        {
            DrawingSurface = surface;
            TrackDisplayManager.addDrawingSurface(surface, Identifier());

            if (FoundationLayer == null)
            {
                throw new Exception("No foundation layer");
            }

            surface.setDataLayerProps(Identifier(), Envitia.MapLink.TSLNPropertyEnum.TSLNPropertyVisible, 1);
            surface.setDataLayerProps(Identifier(), Envitia.MapLink.TSLNPropertyEnum.TSLNPropertyDetect, 1);
            surface.setDataLayerProps(Identifier(), Envitia.MapLink.TSLNPropertyEnum.TSLNPropertyTransparency, IsFoundationLayer ? 255 : Opacity);

            // Always draw this layer on top of other layers, otherwise it is useless
            surface.bringToFront(Identifier());

            // Lat/Long coordinates (approx.) of various destinations.
            double[,] airportLocations = {
          { 37.62, -122.38 }, // SF
          { 32.74, -117.20 }, // San Diego
          { 33.94, -118.41 }, // LAX
          { 38.69, -121.59 }, // Sacramento
          { 38.95, -77.46  }, // Dulles
          { 41.98, -87.91  }, // Chicago O'hare
          { 21.33, -157.92 }, // Honolulu
          { 37.72, -122.22 }, // Oakland
          { 33.82, -118.15 }, // Long Beach
          { 36.59, -121.85 }, // Monterey
          { 37.36, -121.93 }, // San Jose
          { 36.78, -119.71 }, // Fresno Yosemite
          { 33.83, -116.51 }, // Palm Springs
          { 38.51, -122.81 }, // Santa Rosa
          { 35.44, -119.05 }, // Meadows Field
          { 40.97, -124.11 }, // Redwood Coast
          { 33.13, -117.28 }, // McClellan-Palomor
          { 37.29, -120.52 }, // Merced
          { 34.10, -117.25 }, // SBD
          { 40.49, -122.32 }, // Redding
          { 39.80, -121.86 }, // Chico
      };

            // Play with this number to see how it performs. This is all done on the UI thread so you will start to see performance issues around 100,000 tracks.
            // With large numbers of tracks, move the management of track objects to a background thread to alleviate performance issues in the UI thread.
            int numTracks = 1000;
            int numAirports = airportLocations.GetLength(0);

            // We'll do a bulk move operation for optimisation
            var trackIds = new uint[numTracks];
            var lats = new double[numTracks];
            var lons = new double[numTracks];

            Random randomizer = new();

            for (int i = 0; i < numTracks; ++i)
            {
                // Create a track
                trackIds[i] = TrackDisplayManager.numberOfTracks();
                var track = new Tracks.Track { Id = trackIds[i] };
                TrackDisplayManager.addTrack(track.Id, track.GetTrack());
                Tracks.Add(track);

                // Randomly assign departure and destination airports.
                long departure = randomizer.Next(numAirports - 1);
                long destination = randomizer.Next(numAirports - 1);
                track.From = new Tuple<double, double>(airportLocations[departure, 0], airportLocations[departure, 1]);
                track.Location = track.From;
                lats[i] = track.From.Item1;
                lons[i] = track.From.Item2;
                track.To = new Tuple<double, double>(airportLocations[destination, 0], airportLocations[destination, 1]);

                // Randomly assign a velocity
                track.Velocity = randomizer.Next(2000, 10000);

                // Set the track's heading.
                Envitia.MapLink.TSLNCoordinateConverter.greatCircleDistance(track.From.Item1, track.From.Item2, track.To.Item1, track.To.Item2, out double range, out double bearing);
                track.GetTrack().heading(bearing);
            }

            // Do the bulk move.
            TrackDisplayManager.moveTracks(trackIds, lats, lons);

            // Update every 100 ms. Play with this number to see how it performs. Again, track management should be done on a background thread.
            DispatcherTimer.Tick += new EventHandler(DispatcherTimer_Tick);
            DispatcherTimer.Interval = TimeSpan.FromMilliseconds(100);
            DispatcherTimer.Start();
        }

        /// <summary>
        /// Move the tracks towards their destination.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void DispatcherTimer_Tick(object sender, EventArgs e)
        {
            var numTracks = Tracks.Count;

            // We'll do a bulk move again.
            var trackIds = new uint[numTracks];
            var lats = new double[numTracks];
            var lons = new double[numTracks];

            int i = 0;
            foreach (var track in Tracks)
            {
                trackIds[i] = track.Id;
                lats[i] = track.To.Item1;
                lons[i] = track.To.Item2;

                double latToGo = track.To.Item1 - track.Location.Item1;
                double lonToGo = track.To.Item2 - track.Location.Item2;

                bool notAtDestination = latToGo != 0 || lonToGo != 0;
                if (notAtDestination)
                {
                    // Move the track along the same path towards it's destination
                    Envitia.MapLink.TSLNCoordinateConverter.greatCircleDistancePoint(track.Location.Item1, track.Location.Item2, track.GetTrack().heading(), track.Velocity, out double endLat, out double endLon);
                    lats[i] = endLat;
                    lons[i] = endLon;
                    track.Location = new Tuple<double, double>(lats[i], lons[i]);
                }
                ++i;
            }

            // Do the bulk move.
            TrackDisplayManager.moveTracks(trackIds, lats, lons);

            DrawingSurface.redraw();
        }

        public override Envitia.MapLink.TSLNDataLayer GetDataLayer()
        {
            return null;
        }
    }
}
