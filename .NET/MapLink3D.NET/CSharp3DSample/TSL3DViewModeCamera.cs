using System;
using System.Collections.Generic;
using System.Text;
using Envitia.MapLink.ML3D;

namespace CSharp3DSample
{
  class TSL3DViewModeCamera : TSL3DViewMode
  {
    const double MAX_ALTITUDE = 5500000.0;
    const double SCALING_FACTOR = 1.0 / 900000.0;
    const double MAX_FACTOR = 0.2;

    public TSL3DViewModeCamera(TSL3DViewModeInterface iViewMode, int modeID) 
      : base( iViewMode, modeID )
    {
      m_originX = 0;
      m_originY = 0;
      m_active = false;
    }

    public override bool activate(System.Windows.Forms.IWin32Window parentWindow)
    {
      CameraDlg camDlg = new CameraDlg(m_drawingSurface.camera);
      if (camDlg.ShowDialog(parentWindow) == System.Windows.Forms.DialogResult.OK)
      {
        return true;
      }
      return false;
    }

    public override bool OnLButtonDown(int x, int y, bool shift, bool control)
    {
      m_originX = x;
      m_originY = y;

      m_active = true;

      return false;
    }

    public override bool OnLButtonUp(int x, int y, bool shift, bool control)
    {
      m_active = false;
      return false;
    }

    public override bool OnMouseMove(int x, int y, bool shift, bool control)
    {
      int dx = m_originX - x;
      int dy = m_originY - y;

      if ( !m_active || (dx == 0 && dy == 0) )
      {
        return false;
      }

      // Get the current lat/long/alt position
      double latitude, longitude, altitude ;
      m_drawingSurface.camera.position( out latitude, out longitude, out altitude ) ;

      // Calculate an increment based on the altitude but don't allow it to go too fast
      double distance = altitude;

      if ( distance > MAX_ALTITUDE )
      {
        distance = MAX_ALTITUDE;
      }

      double xFactor = (distance * SCALING_FACTOR) / (60.0 * Math.Cos(degreesToRadians(latitude)));
      double yFactor = -( distance * SCALING_FACTOR ) / 60.0;


      if (Math.Abs(yFactor) > MAX_FACTOR)
      {
        yFactor = -MAX_FACTOR;
      }

      if ( xFactor > MAX_FACTOR )
      {
        xFactor = MAX_FACTOR;
      }
      
      latitude += (double)dy * yFactor;
      double newLongitude = longitude;
      newLongitude += (double)dx * xFactor;
     
      // Deal with going over the north or south pole - flip the view
      // and mark the fact that we're now upside down
      if (Math.Abs(latitude) > 90.0)
      {
        double sgn = latitude > 90.0 ? -1.0 : +1.0;

        latitude = sgn*( Math.Abs( latitude ) - 180.0 );

        if (Math.Abs(newLongitude - longitude) < 1.0)
        {
          // We are now travelling on the opposite meridian.
          newLongitude = longitude + 180.0;
        }
      }

      longitude = newLongitude;

      // Remember the new origin
      m_originX = x ;
      m_originY = y ;

      // Update the screen
      m_drawingSurface.camera.moveTo(latitude, longitude, altitude, TSLN3DCameraMoveAction.TSLN3DCameraMoveActionNone);

      return true ;
    }

    public void fov(double angle)
    {
      m_drawingSurface.camera.setAngleOfView(angle);
    }

    private int m_originX;
    private int m_originY;
    private bool m_active;
  }
}
