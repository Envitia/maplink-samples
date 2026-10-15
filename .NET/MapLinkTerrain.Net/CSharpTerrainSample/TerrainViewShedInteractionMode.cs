using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using Envitia.MapLink;
using Envitia.MapLink.InteractionModes;
using Envitia.MapLink.Terrain;


namespace CSharpTerrainSample
{
  class TerrainViewShedInteractionMode : TSLNInteractionMode
  {
    private bool m_midButtonPansToPoint;
    private int m_myID;
    private string m_myPrompt;
    private TerrainCustomClientDataLayer m_cl;
    private TerrainOptionsSection m_options;
    private Control m_parent;

    public TerrainViewShedInteractionMode(int id, bool middleButtonPansToPoint, string prompt, TerrainCustomClientDataLayer cl, TerrainOptionsSection options, Control parent)
      : base(id, middleButtonPansToPoint)
    {
      m_myID = id;
      m_options = options;
      this.m_midButtonPansToPoint = middleButtonPansToPoint;
      m_myPrompt = prompt;
      this.m_cl = cl;
      this.m_parent = parent;
    }

    public override bool onMouseMove(TSLNButtonType button, int x, int y, bool shift, bool control)
    {
      if (display != null && display.drawingSurface != null)
      {
        double mux, muy;
        TSLNDrawingSurface ds = display.drawingSurface;
        ds.DUToMU(x, y, out mux, out muy);
        m_cl.viewShed.setEyeCoords(mux, muy);
        m_cl.viewShed.calculateLineOfSight(-1, -1, 
                                            m_options.StartHeight, 
                                            m_options.StartHeightAboveGround, 
                                            m_options.LOSAreaRangeMU,
                                            m_options.CurveOfEarth,
                                            m_options.UseAccurateLos,
                                            20);
        TSLNDataLayer datalayer = ds.getDataLayer("custom");
        datalayer.notifyChanged();
        ds.redraw();
      }
      return false;
    }

    public override void deactivate()
    {
      TSLNDataLayer datalayer = display.drawingSurface.getDataLayer("custom");
      if (datalayer != null)
      {
        datalayer.notifyChanged();
        m_parent.Invalidate();
      }
    }

    public override TSLNCursorStyle queryCursor()
    {
      return TSLNCursorStyle.TSLNCursorStyleNone;
    }

    public override string queryPrompt()
    {
      return m_myPrompt;
    }
  }
}
