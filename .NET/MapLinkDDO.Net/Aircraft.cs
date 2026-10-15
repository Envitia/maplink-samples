using System;
using System.Collections.Generic;
using System.Text;

using Envitia.MapLink;
using Envitia.MapLink.DDO;

namespace CSharpDDOSample
{
  public class Aircraft : TSLNDynamicDataObject 
  {
    public enum AircraftClass
    {
      TMS_747    = 0,
      TMS_Airbus = 1,
      TMS_Cessna = 2
    };

    public Aircraft(AircraftClass cls)
    {
      m_aircraftClass = cls;
      m_velocity = 0.0;
      heading = 0.0;
      m_destination = new TSLNCoord();
    }

    public override TSLNDisplayObject instantiateDO(TSLNDisplayType key, int dsID)
    {
      // Instantiate and return the appropriate DO, or return 0
      // Could use the dsID to identify a particular darwingSurface and use that
      // to instantiate a specific type of Display Object (eg. for main and overview screens)

      switch (m_aircraftClass)
      {
        case AircraftClass.TMS_747:
          return new TMS747_DO( ) ;

        case AircraftClass.TMS_Airbus:
          return new TMSAirbus_DO( ) ;

        case AircraftClass.TMS_Cessna:
          return new TMSCessna_DO( ) ;
      }

      return null ;
    }

    public double velocity
    {
      get
      {
        return m_velocity*MS_to_KH;
      }
      set
      {
        m_velocity = value * KH_to_MS;
      }
    }

    public static int random(int min, int max)
    {
      double rval = m_randomNumGenerator.NextDouble();
      return (int)( rval * ((max - min) + 0.99999) + min );
    }

    public void destination(int dest_x, int dest_y)
    {
      m_destination.x = dest_x;
      m_destination.y = dest_y;
    }

    public void update(double rate)
    {
      double dxToGo = m_destination.x - position.x;
      double dyToGo = m_destination.y - position.y;

      bool notAtDestination = (dxToGo != 0) || (dyToGo != 0);

      if (notAtDestination)
      {
        double speed = (rate > 0) ? m_velocity * rate : m_velocity;
        double travelled = speed  * tmcPerMeter;

        if (dxToGo != 0)
        {
          heading = Math.Atan2(dxToGo, dyToGo);
        }
        else
        {
          heading = (dyToGo > 0) ? 0.0 : Math.PI;
        }

        while (heading < 0.0)
        {
          heading += Math.PI * 2;
        }

        while (heading > 2 * Math.PI)
        {
          heading -= Math.PI * 2;
        }

        double dx = Math.Sin(heading) * travelled;
        double dy = Math.Cos(heading) * travelled;

        if (Math.Abs(dx) > Math.Abs(dxToGo)) dx = dxToGo;
        if (Math.Abs(dy) > Math.Abs(dyToGo)) dy = dyToGo;

        translate((int)dx, (int)dy, true);
      }
    }

    private static double KH_to_MS = 1.0 / 3.6;
    private static double MS_to_KH = 3.6;
    private static Random m_randomNumGenerator = new Random();
    private AircraftClass m_aircraftClass;		// The class of aircraft.
    private double	      m_velocity;
    
    private TSLNCoord	    m_destination;	// Destination in TMC units.


    //Public variables
    public double heading;
    public static double tmcPerMeter;
  }
}
