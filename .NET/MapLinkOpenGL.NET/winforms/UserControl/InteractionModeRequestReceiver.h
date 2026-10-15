#pragma once
using namespace System;
using namespace Envitia::MapLink;
using namespace Envitia::MapLink::InteractionModes;

namespace MaplinkWinformsGLOverlay
{
  /// <summary>
  /// Summary description for InteractionModeRequestReceiver.
  /// </summary>
  ref class InteractionModeRequestReceiver : public TSLNInteractionModeRequest
  {
  public:
    InteractionModeRequestReceiver()
      : TSLNInteractionModeRequest()
    {
    }

    virtual void resetMode(TSLNInteractionMode^ newMode, TSLNButtonType button, int xDU, int yDU) override
    {
      // Ignore the override
    }

    virtual void viewChanged(TSLNDrawingSurface^ drawingSurface) override
    {
      // Ignore the override
    }
  };
}
