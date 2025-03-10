using System;

namespace DrawingSurfacePanel
{
  //create this small extended class so that we can override the
  //paintbackground method of the panel
  //This allows us to use MapLink double buffering
  [System.Runtime.Versioning.SupportedOSPlatform("windows")]
  [System.ComponentModel.DesignerCategory("Code")]
  [System.ComponentModel.Designer(typeof(MapViewerPanelDesigner))]
  internal class MapViewerParentPanel : System.Windows.Forms.Panel
  {
    protected override void OnPaintBackground(System.Windows.Forms.PaintEventArgs pevent)
    {
      // do nothing...
      // we don't want the background to flash over the map
    }

    protected override void OnMouseEnter(EventArgs e)
    {
      this.Focus();
    }
  }

  /// <summary>
  /// This designer class forces the background of the MapViewerPanel class to be
  /// drawn when viewing instances of it through the designer.
  /// </summary>
  [System.Runtime.Versioning.SupportedOSPlatform("windows")]
  internal class MapViewerPanelDesigner : System.Windows.Forms.Design.ControlDesigner
  {
    protected override void OnPaintAdornments(System.Windows.Forms.PaintEventArgs pevent)
    {
      pevent.Graphics.FillRectangle(new System.Drawing.SolidBrush(this.Control.BackColor), pevent.ClipRectangle);
    }
  }
}
