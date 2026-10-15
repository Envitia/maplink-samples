using System;
using System.Collections.Generic;
using System.Text;

using Envitia.MapLink;
using Envitia.MapLink.InteractionModes;
using Envitia.MapLink.Terrain;

namespace CSharpTerrainSample
{
  public class TerrainCrossSectionInteractionMode : TSLNInteractionMode
  {
    private CrossSectionPanel m_panel;
    private MapPanel m_mappanel;
    private int m_startX;
    private int m_startY;
    private double m_start_mux;
    private double m_start_muy;
    private bool m_pressed;

    static int NUMPOINTS = 100;
    private double [] terr_data = new double[NUMPOINTS] ;

    public TerrainCrossSectionInteractionMode(int id, bool middleButtonPansToPoint, string prompt, CrossSectionPanel panel, MapPanel mapPanel)
      : base(id, middleButtonPansToPoint)
    {
      m_panel = panel;
      m_mappanel = mapPanel;
      m_startX = 0;
      m_startY = 0;
      m_start_mux = 0.0;
      m_start_muy = 0.0;
      m_pressed = false;
    }

    public override bool onLButtonDown(int x, int y, bool shift, bool control)
    {
      m_startX = x;
      m_startY = y;
      m_pressed = true;

      if (m_mappanel.isConversionRequired)
      {
        double lat, lng;
        display.drawingSurface.DUToLatLong(x, y, out lat, out lng, false, true);
        m_panel.terrainDB.latLongToMU(lat, lng, out m_start_mux, out m_start_muy, false);
      }
      else
      {
        // Convert as normal.
        display.drawingSurface.DUToMU(x, y, out m_start_mux, out m_start_muy);
      }

      return true;
    }
    public override bool onMouseMove(TSLNButtonType button, int x, int y, bool shift, bool control)
    {
      double end_mux ;
      double end_muy ;

      if (!m_pressed) 
        return true ;

      if (m_mappanel.isConversionRequired)
      {
        double lat, lng ;
        display.drawingSurface.DUToLatLong( x, y, out lat, out lng, false, true ) ;
        m_panel.terrainDB.latLongToMU(lat, lng, out end_mux, out end_muy, false);
        
      }
      else
      {   
        // Convert as normal.
        display.drawingSurface.DUToMU(x, y, out end_mux, out end_muy);
      }

      bool highestRes = shift;
      TSLNTerrainDataItem[] di = null;
      if (m_panel.terrainDB.queryLine(m_start_mux, m_start_muy, end_mux, end_muy, NUMPOINTS, out di, TSLNTerrainData_LEVEL.TSLNTerrainData_All, highestRes, TSLNTerrainInterpolate.TSLNTerrainInterpolate_LINEAR, false) == TSLNTerrainReturn.TSLNTerrain_OK)
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
      double mux2 = end_mux;
      double muy2 = end_muy;
      display.drawingSurface.DUToMU(m_startX, m_startY, out mux1, out muy1);
      display.drawingSurface.DUToMU(x, y, out mux2, out muy2);
      double range = Math.Sqrt( ( mux2 - mux1 ) * ( mux2 - mux1 ) + ( muy2 - muy1 ) * ( muy2 - muy1 ) );
      
      m_panel.SetData(NUMPOINTS, terr_data, range) ;

      display.setEchoLine((int)m_startX, (int)m_startY, (int)x, (int)y); 
      
      return false ;    // No redraw required
    }

    public override bool onLButtonUp(int x, int y, bool shift, bool control)
    {
      display.clearEchoRectangle();
      m_pressed = false;

      return true;
    }

    public override void deactivate()
    {
      display.clearEchoLine();
    }
  }
}
