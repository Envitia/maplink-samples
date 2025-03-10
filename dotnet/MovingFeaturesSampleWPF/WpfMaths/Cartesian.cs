namespace WpfMaths
{
  public class Cartesian
  {
    /// <summary>
    /// Calculates a new point at an azimuth and distance from origin.
    /// </summary>
    /// <param name="origin">Starting point</param>
    /// <param name="distance"></param>
    /// <param name="azimuthDegrees"></param>
    /// <param name="rotation">A rotation to add.</param>
    /// <returns></returns>
    public static System.Windows.Point CalcPoint(System.Windows.Point origin, double distance, double azimuthDegrees, int rotation)
    {
      var rads = (azimuthDegrees + rotation) / (180.0 / Math.PI);
      var pt = new System.Windows.Point(origin.X + Math.Sin(rads) * distance, origin.Y - Math.Cos(rads) * distance);
      return pt;
    }

    /// <summary>
    /// Returns the distance betweem points 1 and 2
    /// </summary>
    /// <param name="point1"></param>
    /// <param name="point2"></param>
    /// <returns>The distance in Windows coordinates</returns>
    public static double Distance(System.Windows.Point point1, System.Windows.Point point2)
    {
      return Math.Sqrt(Math.Pow((point2.X - point1.X), 2) + Math.Pow((point2.Y - point1.Y), 2));
    }

    /// <summary>
    /// Returns the azimuth (bearing) of two points.
    /// </summary>
    /// <param name="point1"></param>
    /// <param name="point2"></param>
    /// <param name="rotation"></param>
    /// <returns>The azimith in degrees</returns>
    public static double Azimuth(System.Windows.Point point1, System.Windows.Point point2, double rotation)
    {
      var theta = Math.Atan2(
          point2.X - point1.X,  // pt2 - pt1, because x increases left to right
          point1.Y - point2.Y); // pt1 - pt2, because Y increases top to bottom (most Cartesian calcs assume bottom to top)

      if (theta < 0.0) { theta += Math.PI * 2; }
      var degs = theta * 180.0 / Math.PI - rotation;

      degs = degs % 360.0;
      if (degs < 0)
      {
        degs += 360;
      }

      return degs;
    }
  }
}
