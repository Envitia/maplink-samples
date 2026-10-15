using System;
using System.Drawing;
using System.Collections;
using System.Windows.Forms;
using System.ComponentModel;
using System.Windows.Forms.Design;

namespace CSharpAPP6ASample
{
  [System.ComponentModel.DesignerCategory("Code")]
  [Designer(typeof(MapViewerPanelDesigner))]
  public class MapViewerPanel : Panel
  {
    protected override void OnPaintBackground( PaintEventArgs pevent )
    {
      // do nothing...
      // we don't want the background to flash over the map
    }
  }

  /// <summary>
  /// This designer class forces the background of the MapViewerPanel class to be
  /// drawn when viewing instances of it through the designer.
  /// </summary>
  internal class MapViewerPanelDesigner : ControlDesigner
  {
    protected override void OnPaintAdornments(PaintEventArgs pevent)
    {
      pevent.Graphics.FillRectangle(new SolidBrush(this.Control.BackColor), pevent.ClipRectangle);
    }
  }
}
