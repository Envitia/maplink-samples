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

class FPSLayer : public TSLClientCustomDataLayer
{
public:
  FPSLayer();
  virtual ~FPSLayer();

  virtual bool drawLayer( TSLRenderingInterface *renderingInterface, const TSLEnvelope* extent, TSLCustomDataLayerHandler& layerHandler );

  void update( double secsSinceLastFrame )
  {
    m_cumulativeTime += secsSinceLastFrame;
  }

private:
  TSLText *m_framerateStr;
  TSLStyleID m_fpsCounterTextColour;
  TSLStyleID m_fpsCounterTextStyle;
  double m_fpsCounterTextSize;

  double m_cumulativeTime;
  uint32_t m_numFrames;
};

#endif // FRAMERATELAYER_H