using System;

namespace DrawingSurfacePanel
{
  /// <summary>
  /// Summary description for InteractionModeRequestReceiver.
  /// </summary>
  internal class InteractionModeRequestReceiver : Envitia.MapLink.InteractionModes.TSLNInteractionModeRequest
  {
    public InteractionModeRequestReceiver()
      : base()
    {
    }

    public override void resetMode(Envitia.MapLink.InteractionModes.TSLNInteractionMode newMode, Envitia.MapLink.TSLNButtonType button, int xDU, int yDU)
    {
      // Ignore the override
    }

    public override void viewChanged(Envitia.MapLink.TSLNDrawingSurface drawingSurface)
    {
      // Ignore the override
    }
  }
}
