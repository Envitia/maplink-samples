#pragma once

namespace ModeInteractions
{
  /// <summary>
  /// Interaction modes.
  /// </summary>
  public enum class InteractionModesEnum
  {
    TOOLS_ZOOM = 1,
    TOOLS_PAN = 2,
    TOOLS_GRAB = 3,
    TOOLS_ZOOM_PAN = 4,
  };

  /// <summary>
  /// Interface class to handle mouse right click call back function [optional].
  /// </summary>
  public ref class RightClickContextMenuInterface abstract
  {
  public:
    /// <summary>
    /// To show context menu with mouse right click. If not set, interaction modes will reset to zoom mode.
    /// </summary>
    virtual void handleMouseRightClick() = 0;
  };

}