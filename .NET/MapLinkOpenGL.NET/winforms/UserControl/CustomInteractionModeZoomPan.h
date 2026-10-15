#pragma once

using namespace System;
using namespace Envitia::MapLink;
using namespace Envitia::MapLink::InteractionModes;
using namespace Envitia::MapLink::OpenGLSurface;

namespace MaplinkWinformsGLOverlay
{
#if 1
  typedef TSLNOpenGLSurface DrawingSurfaceClass;//< to use opengl drawing surface.
#else
  typedef TSLNDrawingSurface DrawingSurfaceClass;//< to use normal drawing surface.
#endif

  /// <summary>
  /// This class demonstrates how to implement a custom interaction mode
  /// </summary>
  ref class CustomInteractionModeZoomPan : public TSLNInteractionMode
  {
  private:
    int m_myID;
    int m_myZoomPercentage;
    bool m_midButtonPansToPoint;
    String^ m_myPrompt;

  public:
    CustomInteractionModeZoomPan(int id, int zoomPercentage, bool middleButtonPansToPoint, String^ prompt)
      : TSLNInteractionMode(id, middleButtonPansToPoint)
    {
      m_myID = id;
      m_myZoomPercentage = zoomPercentage;
      m_midButtonPansToPoint = middleButtonPansToPoint;
      m_myPrompt = prompt;
    }

    virtual bool onLButtonDown(int x, int y, bool shift, bool control) override
    {
      if (display != nullptr && display->drawingSurfaceBase != nullptr)
      {
        DrawingSurfaceClass^ ds = (DrawingSurfaceClass^)display->drawingSurfaceBase;
        double uux, uuy;
        ds->DUToUU(x, y, uux, uuy);

        if (ds->zoom(m_myZoomPercentage, true, false))
          return ds->pan(uux, uuy, false);
      }
      return false;
    }

    virtual bool onMButtonDown(int x, int y, bool shift, bool control) override
    {
      if (m_midButtonPansToPoint && display != nullptr && display->drawingSurfaceBase != nullptr)
      {
        DrawingSurfaceClass^ ds = (DrawingSurfaceClass^)display->drawingSurfaceBase;
        double uux, uuy;
        if (ds->DUToUU(x, y, uux, uuy))
        {
          if (ds->pan(uux, uuy, false))
          {
            display->viewChanged(true);
            return true;
          }
        }
      }
      return false;
    }

    virtual  bool onMouseWheel(short delta, int x, int y, int zoomPercentage) override
    {
      if (m_myZoomPercentage != 0 && display != nullptr && display->drawingSurfaceBase != nullptr)
      {
        DrawingSurfaceClass^ ds = (DrawingSurfaceClass^)display->drawingSurfaceBase;
        bool sts = ds->zoom(m_myZoomPercentage, delta > 0, false);
        if (sts)
          display->viewChanged(true);
        return sts;
      }
      return false;
    }

    virtual  bool onRButtonDown(int x, int y, bool shift, bool control) override
    {
      if (display != nullptr && display->drawingSurfaceBase != nullptr)
      {
        DrawingSurfaceClass^ ds = (DrawingSurfaceClass^)display->drawingSurfaceBase;
        double uux, uuy;
        ds->DUToUU(x, y, uux, uuy);

        if (ds->zoom(m_myZoomPercentage, false, false))
          return ds->pan(uux, uuy, false);
      }
      return false;
    }

    virtual  Envitia::MapLink::TSLNCursorStyle queryCursor() override
    {
      return TSLNCursorStyle::TSLNCursorStyleMagnify;
    }

    virtual String^ queryPrompt() override
    {
      return m_myPrompt;
    }

  };
}
