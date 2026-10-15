using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using Envitia.MapLink;
using Envitia.MapLink.InteractionModes;
using Envitia.MapLink.Terrain;


namespace CSharpTerrainSample
{
  public class TerrainLosInteractionMode : TSLNInteractionMode
  {
    private CrossSectionPanel m_panel;
    private MapPanel m_mappanel;
    private int m_startX;
    private int m_startY;
    private double m_start_mux;
    private double m_start_muy;
    private double m_end_mux;
    private double m_end_muy;
    private bool m_pressed;
    bool m_isLatLon;
    double m_central_meridian;

    static int NUMPOINTS = 100;
    private double[] terr_data = new double[NUMPOINTS];

    public TerrainLosInteractionMode(int id, bool middleButtonPansToPoint, string prompt, CrossSectionPanel panel, MapPanel mappanel)
      : base(id, middleButtonPansToPoint)
    {
      m_mappanel = mappanel;
      m_panel = panel;
      m_startX = 0;
      m_startY = 0;
      m_start_mux = 0.0;
      m_start_muy = 0.0;
      m_central_meridian = 0.0;
      m_pressed = false;
      m_isLatLon = false;
    }

    public override bool onLButtonDown(int x, int y, bool shift, bool control)
    {
      m_startX = x ;
      m_startY = y ;
      m_pressed = true ;

      if (m_mappanel.isConversionRequired)
      {
        double lat, lng;
        display.drawingSurface.DUToLatLong(x, y, out lat, out lng, false, true);
        m_panel.terrainDB.latLongToMU(lat, lng, out m_start_mux, out m_start_muy, false);

        if (lat == m_start_muy && lng == m_start_mux)
        {
          m_isLatLon = true;
        }
      }
      else
      {
        // Convert as normal.
        display.drawingSurface.DUToMU(x, y, out m_start_mux, out m_start_muy);
      }

      return true ;
    }

    public override bool onMouseMove(TSLNButtonType button, int x, int y, bool shift, bool control)
    {
      if ( !m_pressed ) 
        return true ;

      if (m_mappanel.isConversionRequired)
      {
        double lat, lng;
        display.drawingSurface.DUToLatLong(x, y, out lat, out lng, false, true);
        m_panel.terrainDB.latLongToMU(lat, lng, out m_end_mux, out m_end_muy, false);
      }
      else
      {   
        // Convert as normal.
        display.drawingSurface.DUToMU(x, y, out m_end_mux, out m_end_muy);
      }

      TSLNTerrainDataItem[] di = null;

      bool highestRes = shift;

      bool crossesDateLine = false;
      // Work out if we're crossing the dateline or not
      if( m_isLatLon && m_central_meridian != 0.0 && ( m_start_mux >=0 != m_end_mux >= 0 ) &&
          ( ( m_end_mux >= m_start_mux && x <= m_startX ) || ( m_end_mux <= m_start_mux && x >= m_startX ) ) )
      {
        crossesDateLine = true;
      }
      if (m_panel.terrainDB.queryLine(m_start_mux, m_start_muy, m_end_mux, m_end_muy, NUMPOINTS, out di, TSLNTerrainData_LEVEL.TSLNTerrainData_Min, highestRes, TSLNTerrainInterpolate.TSLNTerrainInterpolate_NONE, crossesDateLine) == TSLNTerrainReturn.TSLNTerrain_OK)
      {
        for ( int i = 0; i < NUMPOINTS; i++ )
        {
          terr_data[i] = di[i].m_z;
        }
      }
      else
      {
        Array.Clear(terr_data, 0, terr_data.Length);
      }

      // Find the range
      double mux1 = m_start_mux;
      double muy1 = m_start_muy;
      double mux2 = m_end_mux;
      double muy2 = m_end_muy;
      display.drawingSurface.DUToMU(m_startX, m_startY, out mux1, out muy1);
      display.drawingSurface.DUToMU(x, y, out mux2, out muy2);

      double range = Math.Sqrt( ( mux2 - mux1 ) * ( mux2 - mux1 ) + ( muy2 - muy1 ) * ( muy2 - muy1 ) );

      m_panel.SetData(NUMPOINTS, terr_data, range);

      display.setEchoLine(m_startX, m_startY, x, y);

      return false ;    // No redraw required
    }

    public override bool onLButtonUp(int x, int y, bool shift, bool control)
    {
      TSLNTerrainDataItem[] di = null;

      // Query the terrain database passing the Map Unit values of the
      // start and end of the line.
      bool crossesDateLine = false;
      if( m_isLatLon && m_central_meridian != 0.0 && ( m_start_mux >=0 != m_end_mux >= 0 ) &&
          ( ( m_end_mux > m_start_mux && x <= m_startX ) || ( m_end_mux < m_start_mux && x >= m_startX ) ) )
      {
        crossesDateLine = true;
      }
      if (m_panel.terrainDB.queryLine(m_start_mux, m_start_muy, m_end_mux, m_end_muy, NUMPOINTS, out di, TSLNTerrainData_LEVEL.TSLNTerrainData_Min, shift, TSLNTerrainInterpolate.TSLNTerrainInterpolate_NONE, false) == TSLNTerrainReturn.TSLNTerrain_OK)
      {

        m_mappanel.visDataLayer.entitySet.clear();

        // To work out if we can see any point on the map, from a given location
        // we need to work out the formula y = mx + c from the start to the 
        // current point. We can then look at each point before the current
        // on and check if it is in the way.

        //***********************************************************************
        // adjust_for_earth_curvature() does not seem to be working correctly
        // so has been commented out for now... TC 29/08/2003
        //***********************************************************************

        // Flag to say whether the current point can be seen.
        bool bHidden = false ;

        double prev_x, prev_y, prev_z ;
        double current_x, current_y, current_z ;
        double start_x, start_y, start_z ;

        // Set up the start location. Note the startHeight which
        // is declared within the config file.
        start_x = di[0].m_x ;
        start_y = di[0].m_y ;
        start_z = di[0].m_z + m_panel.m_startheight;

        // The end height can be set in the config, so update. 
        di[NUMPOINTS - 1].m_z += m_panel.m_endheight;

        // Loop through all the points on the line.
        for (int i = 1; i < NUMPOINTS - 1; ++i)
        {
          prev_x = di[i-1].m_x ;
          prev_y = di[i-1].m_y ;
          prev_z = di[i-1].m_z ;

          current_x = di[i].m_x ;
          current_y = di[i].m_y ;
          current_z = di[i].m_z ;

          // distance to current_z from start.
          double dist = distance( start_x, start_y, current_x, current_y ) ;
          double length = current_z - start_z;
          if( crossesDateLine && start_x >=0 != current_x >=0 && dist > 180.0 )
          {
            adjust_for_dateline( start_x, start_y, current_x, current_y, out length, out dist );
          }
          //adjust_for_earthcurvature( dist, current_z ) ;

          // We want mx+c for this line.
          double m = length / dist ;

          // We can now check that no points are in the way.
          for ( int j = 1 ; j < i; ++j )
          {
            // Work out the distance to each point that could be in the way.
            double p_d = distance ( start_x, start_y, di[j].m_x, di[j].m_y );

            if( crossesDateLine && start_x >=0 != di[j].m_x >=0 && p_d > 180.0 )
            {
              adjust_for_dateline( start_x, start_y, di[j].m_x, di[j].m_y, out length, out dist );
            }

            // The actual height of this point.
            double actual_height = di[j].m_z ;
            //adjust_for_earthcurvature ( p_d, actual_height ) ;

            // The theoretical height if this point falls exactly on the LOS.
            double theo_height = ((m*p_d) + start_z) ;
            //adjust_for_earthcurvature ( p_d, theo_height ) ;

            // If the actual height is above the theo height then it is blocking
            // our view.
            if ( actual_height > theo_height )
            {
              // Higher than the line, so in the way.
              bHidden = true ;
              // As something is in the way we can exit.
              break ;
            }
            else
            {
              //Below the line. So not in the way!
              bHidden = false ;
            } 
          } 

          int StartTmcX, StartTmcY, EndTmcX, EndTmcY;

          if (m_mappanel.isConversionRequired)
          {
            // Convert the points accounting for different coordinate systems.
            double lat1, lat2, lng1, lng2 ;
            m_panel.terrainDB.MUToLatLong( prev_x, prev_y, out lat1, out lng1, false, true ) ;
            m_panel.terrainDB.MUToLatLong(current_x, current_y, out lat2, out lng2, false, true);
            display.drawingSurface.latLongToTMC(lat1, lng1, out StartTmcX, out StartTmcY, false);
            display.drawingSurface.latLongToTMC(lat2, lng2, out EndTmcX, out EndTmcY, false);

          }
          else
          {   
            // Coordinate systems are the same, so convert as normal.
            display.drawingSurface.MUToTMC(prev_x, prev_y, out StartTmcX, out StartTmcY);
            display.drawingSurface.MUToTMC(current_x, current_y, out EndTmcX, out EndTmcY);
          }

          // Create a new coordset and add points.
          TSLNCoordSet coords = new TSLNCoordSet();
          coords.add(StartTmcX, StartTmcY);
          coords.add(EndTmcX, EndTmcY);

          // Create a polyline.
          TSLNPolyline polyline = m_mappanel.visDataLayer.entitySet.createPolyline(1, coords);
          m_mappanel.visDataLayer.entitySet.createSymbol(1, EndTmcX, EndTmcY, 1000, 0.0);
          if (polyline != null)
          {
            // Draw red or green depending on whether the line can been seen.
            if ( bHidden )
            {
              polyline.setRendering(TSLNRenderingAttributeDouble.TSLNRenderingAttributeEdgeThickness, 3.0);
              polyline.setRendering(TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeColour, 181);
            }
            else
            {
              polyline.setRendering(TSLNRenderingAttributeDouble.TSLNRenderingAttributeEdgeThickness, 3.0);
              polyline.setRendering(TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeColour, Color.FromArgb(124, 252, 0).ToArgb());
            }

            polyline.setRendering(TSLNRenderingAttributeInt.TSLNRenderingAttributeEdgeStyle, 1);
          }
        } // Continue looping through all points. 

      } // If

      m_mappanel.visDataLayer.notifyChanged();

      // clear the flags and any rectangle drawn
      display.clearEchoLine( ) ;
      m_pressed = false ;

      return true ;
    }

    public override void deactivate()
    {
      m_mappanel.visDataLayer.entitySet.clear();
      m_mappanel.visDataLayer.notifyChanged();
      m_mappanel.Invalidate();
    }

    void adjust_for_earthcurvature (double distance, ref double height)
    {
      // Radius of the earth in metres.
      double radius = 6378137.0 ;

      // Correct the height.
      height -= Math.Sqrt ( radius*radius + distance*distance ) - radius ;
    }

    // Works out the straight line distance between two coordinates.
    double distance (double x1, double y1, double x2, double y2)
    {
      double a_side = ( x2 - x1 ) ;
      double b_side = ( y2 - y1 ) ;
      //c side...
      return Math.Sqrt( (a_side*a_side) + (b_side*b_side) ) ;
    }

    // Adjusts for line of sight calculations when the line crosses the dateline
    void adjust_for_dateline (double start_x, double start_y, double end_x, double end_y, out double length, out double dist)
    {
      double dateline = 180.0;
      double minusDateline = -180.0;
      length = dateline - start_x - ( -dateline - end_x );
      double height = end_y - start_y;
      double gradient = height / length;
      double datelineY = ( gradient * ( dateline - start_x ) ) + start_y;
      dist = distance( start_x, start_y, dateline, datelineY ) +
             distance( minusDateline, datelineY, end_x, end_y );
    }

    void setCentralMeridian ( double meridian )
    {
      m_central_meridian = meridian;
    }
  }
}
