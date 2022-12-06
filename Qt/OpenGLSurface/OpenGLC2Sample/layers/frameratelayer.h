/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

// This is a simple custom data layer that displays the current redraw rate as text
// on the drawing surface. It can be used to easily measure application drawing performance.

#ifndef FRAMERATELAYER_H
#define FRAMERATELAYER_H

#ifdef WIN32
# include <Windows.h>
#endif

#include "MapLink.h"

class FramerateLayer : public TSLClientCustomDataLayer
{
public:
  FramerateLayer();
  virtual ~FramerateLayer();

  virtual bool drawLayer (TSLRenderingInterface *renderingInterface, const TSLEnvelope* extent, TSLCustomDataLayerHandler& layerHandler);

  void resetTimer();

private:
  TSLText *m_framerateStr;
  TSLStyleID m_fpsCounterTextColour;
  TSLStyleID m_fpsCounterTextStyle;
  TSLStyleID m_fpsCounterHaloColour;
  double m_fpsCounterTextSize;

#ifdef WIN32
  LARGE_INTEGER m_counterFrequency;
  LARGE_INTEGER m_lastFrameTime;
  LARGE_INTEGER m_startTime;
#else
  timespec m_lastFrameTime;
  timespec m_startTime;
  clockid_t m_clockType;
#endif

  double m_cumulativeTime;
  uint32_t m_numFrames; // Number of frames rendered in the last second
  uint32_t m_totalNumFrames; // Total number of frames rendered since the last call to resetTimer()
};

#endif // FRAMERATELAYER_H