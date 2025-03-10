using System;
using System.Windows.Threading;

namespace ContourApp.MapLayers
{
  /// <summary>
  ///  DdoLayer demonstrates the fundamentals of creating DDOs using the Envitia.MapLink.DDO SDK.
  /// </summary>
  public class DdoLayer : MapLayer
  {
    public Envitia.MapLink.DDO.TSLNObjectDataLayer DataLayer { get; set; } = new Envitia.MapLink.DDO.TSLNObjectDataLayer();

    public Envitia.MapLink.TSLNDataLayer FoundationLayer { get; set; }
    private Envitia.MapLink.TSLN2DDrawingSurface DrawingSurface { get; set; }
    public static DispatcherTimer DispatcherTimer { get; } = new System.Windows.Threading.DispatcherTimer(System.Windows.Threading.DispatcherPriority.Render);

    public override string Identifier()
    {
      return "DdoLayer";
    }

    /// <summary>
    /// Creates a number of Dynamic Data Objects moving between randomly assigned departure and destination airports.
    /// </summary>
    /// <param name="surface"></param>
    /// <param name="visible"></param>
    /// <exception cref="Exception"></exception>
    public override void ConfigureMapLayer(Envitia.MapLink.TSLN2DDrawingSurface surface, bool visible)
    {
      DrawingSurface = surface;

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
      // With large numbers of ddos, move the management of ddos to a background thread to alleviate performance issues in the UI thread.
      uint numDdos = 1000;
      int numAirports = airportLocations.GetLength(0);

      Random randomizer = new();

      for (uint i = (uint)0; i < numDdos; ++i)
      {
        // Create a ddo
        var dynamicObject = new DynamicObject();
        DataLayer.addDDO(dynamicObject);

        // Randomly assign departure and destination airports.
        long dep = randomizer.Next(numAirports - 1);
        long arr = randomizer.Next(numAirports - 1);
        dynamicObject.From = new Tuple<double, double>(airportLocations[dep, 0], airportLocations[dep, 1]);
        dynamicObject.Location = dynamicObject.From;
        dynamicObject.To = new Tuple<double, double>(airportLocations[arr, 0], airportLocations[arr, 1]);

        // Randomly assign a velocity
        dynamicObject.Velocity = randomizer.Next(2000, 10000);

        // Move the DDO
        DrawingSurface.latLongToTMC(dynamicObject.From.Item1, dynamicObject.From.Item2, out int x, out int y);
        dynamicObject.move(new Envitia.MapLink.TSLNCoord(x, y), true);

        // Set the DDO's heading
        Envitia.MapLink.TSLNCoordinateConverter.greatCircleDistance(dynamicObject.From.Item1, dynamicObject.From.Item2, dynamicObject.To.Item1, dynamicObject.To.Item2, out double range, out double bearing);
        dynamicObject.Heading = bearing;
      }

      // Update every 100 ms. Play with this number to see how it performs. Again, DDO management should be done on a background thread.
      DispatcherTimer.Tick += new EventHandler(DispatcherTimer_Tick);
      DispatcherTimer.Interval = TimeSpan.FromMilliseconds(100);
      DispatcherTimer.Start();
    }

    /// <summary>
    /// Move the DDOs towards their destination.
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void DispatcherTimer_Tick(object sender, EventArgs e)
    {
      var ddos = DataLayer.getDDOList();
      foreach (var ddo in ddos) 
      {
        DynamicObject dynamicObject = (DynamicObject)ddo;

        double latToGo = dynamicObject.To.Item1 - dynamicObject.Location.Item1;
        double lonToGo = dynamicObject.To.Item2 - dynamicObject.Location.Item2;

        bool notAtDestination = (latToGo != 0) || (lonToGo != 0);
        if (notAtDestination)
        {
          // Move the DDO along the same path towards it's destination
          Envitia.MapLink.TSLNCoordinateConverter.greatCircleDistancePoint(dynamicObject.Location.Item1, dynamicObject.Location.Item2, dynamicObject.Heading, dynamicObject.Velocity, out double endLat, out double endLon);
          DrawingSurface.latLongToTMC(endLat, endLon, out int x, out int y);
          dynamicObject.move(new Envitia.MapLink.TSLNCoord(x, y), true);
          dynamicObject.Location = new Tuple<double, double>(endLat, endLon);
        }
      }

      DataLayer.notifyChanged(true);
      DrawingSurface.redraw();
    }

    public override Envitia.MapLink.TSLNDataLayer GetDataLayer()
    {
      return DataLayer;
    }
  }
}
