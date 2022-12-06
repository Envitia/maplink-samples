/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "frameratelayer.h"

#ifdef _MSC_VER
# define snprintf _snprintf
#endif

FPSLayer::FPSLayer()
  : m_fpsCounterTextColour( TSLComposeRGB( 255, 255, 255 ) )
  , m_fpsCounterTextStyle( 1 )
  , m_fpsCounterTextSize( 32.0 )
  , m_cumulativeTime( 0.0 )
  , m_numFrames( 0 )
  , m_framerateStr( TSLText::create( 0, 0, 0, "Calculating..." ) )
{
  m_framerateStr->setRendering( TSLRenderingAttributeTextFont, m_fpsCounterTextStyle );
  m_framerateStr->setRendering( TSLRenderingAttributeTextColour, m_fpsCounterTextColour );
  m_framerateStr->setRendering( TSLRenderingAttributeTextSizeFactor, m_fpsCounterTextSize );
  m_framerateStr->setRendering( TSLRenderingAttributeTextSizeFactorUnits, TSLDimensionUnitsPixels );
  m_framerateStr->setRendering( TSLRenderingAttributeTextOffsetX, -10.0 );
  m_framerateStr->setRendering( TSLRenderingAttributeTextOffsetY, 10.0 );
  m_framerateStr->setRendering( TSLRenderingAttributeTextOffsetUnits, TSLDimensionUnitsPixels );
  m_framerateStr->setRendering( TSLRenderingAttributeTextVerticalAlignment, TSLVerticalAlignmentFullBottom );
  m_framerateStr->setRendering( TSLRenderingAttributeTextHorizontalAlignment, TSLHorizontalAlignmentRight );
  m_framerateStr->setRendering( TSLRenderingAttributeTextBackgroundMode, TSLTextBackgroundModeHalo );
  m_framerateStr->setRendering( TSLRenderingAttributeTextBackgroundStyle, 1 );
  m_framerateStr->setRendering( TSLRenderingAttributeTextBackgroundColour, TSLComposeRGB( 0, 0, 0 ) );
  m_framerateStr->setRendering( TSLRenderingAttributeTextRotatable, TSLTextRotationDisabled );
}

FPSLayer::~FPSLayer()
{
  if( m_framerateStr )
    m_framerateStr->destroy();
}

bool FPSLayer::drawLayer( TSLRenderingInterface *renderingInterface, const TSLEnvelope* extent, TSLCustomDataLayerHandler& layerHandler)
{
  ++m_numFrames;
  if (m_cumulativeTime >= 1.0)
  {
    if (m_framerateStr)
    {
      char displayString[64] = { '\0' };
      snprintf(displayString, sizeof(displayString), "%.2lf FPS", m_numFrames / m_cumulativeTime);
      m_framerateStr->value(displayString);
    }
    m_cumulativeTime = 0.0;
    m_numFrames = 0;
  }

  if (m_framerateStr && extent)
  {
    // Position the text at the bottom right of the window
    TSLDeviceUnits screenX1, screenY1, screenX2, screenY2;
    layerHandler.drawingSurface()->getDUExtent(&screenX1, &screenY1, &screenX2, &screenY2);

    // Bottom right position offsets hardcoded
    TSLCoord screenPosition;
    renderingInterface->DUToTMC((screenX2 - 50), (screenY2 - 20), &screenPosition.m_x, &screenPosition.m_y);
    m_framerateStr->position(screenPosition);
  }
  renderingInterface->drawEntity(m_framerateStr);

  return true;
}
