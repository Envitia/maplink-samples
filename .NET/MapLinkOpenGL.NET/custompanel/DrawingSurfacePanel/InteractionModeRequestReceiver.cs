using System;
using Envitia.MapLink;
using Envitia.MapLink.InteractionModes;

namespace DrawingSurfacePanel
{
    /// <summary>
    /// Summary description for InteractionModeRequestReceiver.
    /// </summary>
    internal class InteractionModeRequestReceiver : TSLNInteractionModeRequest
    {
        public InteractionModeRequestReceiver()
          : base()
        {
        }

        public override void resetMode(TSLNInteractionMode newMode, TSLNButtonType button, int xDU, int yDU)
        {
            // Ignore the override
        }

        public override void viewChanged(TSLNDrawingSurface drawingSurface)
        {
            // Ignore the override
        }
    }
}
