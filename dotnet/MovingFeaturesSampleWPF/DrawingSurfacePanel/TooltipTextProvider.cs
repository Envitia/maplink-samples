using System;

namespace DrawingSurfacePanel
{
  [System.Runtime.Versioning.SupportedOSPlatform("windows")]
  public abstract class TooltipTextProvider
  {
    internal System.Windows.Forms.ToolTip ToolTip { get; set; }
    internal DrawingSurfacePanel.MapViewerPanel Panel { get; set; }

    public int XPosOffset { get; set; } = 5;
    public int YPosOffset { get; set; } = 25;

    public abstract string ProvideText(int x, int y);

    internal void Start(int x, int y)
    {
      var threadParameters = new System.Threading.ThreadStart(delegate { ShowToolTip(ProvideText(x, y), x, y); });
      var thread2 = new System.Threading.Thread(threadParameters);
      thread2.Start();
    }

    private void ShowToolTip(string text, int x, int y)
    {
      // Are we in a background thread?
      if (Panel.InvokeRequired)
      {
        // Invoke the UI thread
        Action safeWrite = delegate { ShowToolTip(text, x, y); };
        Panel.Invoke(safeWrite);
      }
      else
      {
        // UI thread - show the tooltip
        ToolTip.Show(text, Panel, x + XPosOffset, y + YPosOffset);
        Panel.DrawingSurface.redraw();
      }
    }
  }
}
