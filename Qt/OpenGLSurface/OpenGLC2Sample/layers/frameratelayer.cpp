#include "tracks/trackmanager.h"
#include "frameratelayer.h"

#include "MapLinkDrawing.h"


#ifdef _MSC_VER
# define snprintf _snprintf
#endif

FramerateLayer::FramerateLayer()
  : m_fpsCounterTextColour( TSLComposeRGB( 255, 255, 255 ) )
  , m_fpsCounterHaloColour( TSLComposeRGB( 0, 0, 0 ) )
  , m_fpsCounterTextStyle( 1 )
  , m_fpsCounterTextSize( 32.0 )
  , m_cumulativeTime( 0.0 )
  , m_numFrames( 0 )
  , m_totalNumFrames( 0 )
  , m_framerateStr( TSLText::create( 0, 0, 0, "Measuring framerate" ) )
{
#ifndef WIN32
# if _POSIX_TIMERS > 0
  m_clockType = sysconf(_POSIX_MONOTONIC_CLOCK) >= 0 ? CLOCK_MONOTONIC : CLOCK_REALTIME;
# else
  m_clockType = CLOCK_REALTIME;
# endif
#else
  QueryPerformanceFrequency( &m_counterFrequency );
#endif

  resetTimer();

  // Set up the text rendering attributes for the framerate display string
  m_framerateStr->setRendering( TSLRenderingAttributeTextFont, m_fpsCounterTextStyle );
  m_framerateStr->setRendering( TSLRenderingAttributeTextColour, m_fpsCounterTextColour );
  m_framerateStr->setRendering( TSLRenderingAttributeTextSizeFactor, m_fpsCounterTextSize );
  m_framerateStr->setRendering( TSLRenderingAttributeTextSizeFactorUnits, TSLDimensionUnitsPixels );
  m_framerateStr->setRendering( TSLRenderingAttributeTextOffsetX, 0.0 );
  m_framerateStr->setRendering( TSLRenderingAttributeTextOffsetY, -5.0 );
  m_framerateStr->setRendering( TSLRenderingAttributeTextOffsetUnits, TSLDimensionUnitsPixels );
  m_framerateStr->setRendering( TSLRenderingAttributeTextVerticalAlignment, TSLVerticalAlignmentFullTop );
  m_framerateStr->setRendering( TSLRenderingAttributeTextHorizontalAlignment, TSLHorizontalAlignmentCentre );
  m_framerateStr->setRendering( TSLRenderingAttributeTextBackgroundMode, TSLTextBackgroundModeHalo );
  m_framerateStr->setRendering( TSLRenderingAttributeTextBackgroundMode, TSLTextBackgroundModeHalo );
  m_framerateStr->setRendering( TSLRenderingAttributeTextBackgroundStyle, 1 );
  m_framerateStr->setRendering( TSLRenderingAttributeTextBackgroundColour, m_fpsCounterHaloColour );
  m_framerateStr->setRendering( TSLRenderingAttributeTextRotatable, TSLTextRotationDisabled );
}

FramerateLayer::~FramerateLayer()
{
  m_framerateStr->destroy();
}

void FramerateLayer::resetTimer ()
{
#ifdef _MSC_VER
  QueryPerformanceCounter( &m_lastFrameTime );
  QueryPerformanceCounter( &m_startTime );
#else
  clock_gettime( m_clockType, &m_lastFrameTime );
  clock_gettime( m_clockType, &m_startTime );
#endif

  m_cumulativeTime = 0.0;
  m_numFrames = 0;
  m_totalNumFrames = 0;

  m_framerateStr->value( "Measuring framerate" );
}

bool FramerateLayer::drawLayer (TSLRenderingInterface *renderingInterface, const TSLEnvelope*, TSLCustomDataLayerHandler& layerHandler)
{
  // Calculate the time since the last frame
#ifdef _MSC_VER
  LARGE_INTEGER currentTime;
  QueryPerformanceCounter( &currentTime );

  double secsSinceLastFrame = (currentTime.QuadPart - m_lastFrameTime.QuadPart) / (double)m_counterFrequency.QuadPart;
  double secsSinceStart = (currentTime.QuadPart - m_startTime.QuadPart) / (double)m_counterFrequency.QuadPart;
#else
  timespec currentTime;
  clock_gettime( m_clockType, &currentTime );
 
  // Work out the time difference
  timespec frameTime;
  frameTime.tv_sec = currentTime.tv_sec - m_lastFrameTime.tv_sec;
  frameTime.tv_nsec = currentTime.tv_nsec - m_lastFrameTime.tv_nsec;
  if( frameTime.tv_nsec < 0 )
  {
    --frameTime.tv_sec;
    frameTime.tv_nsec += 1000000000;
  }

  timespec totalTime;
  totalTime.tv_sec = currentTime.tv_sec - m_startTime.tv_sec;
  totalTime.tv_nsec = currentTime.tv_nsec - m_startTime.tv_nsec;
  if( totalTime.tv_nsec < 0 )
  {
    --totalTime.tv_sec;
    totalTime.tv_nsec += 1000000000;
  }

  double secsSinceLastFrame = frameTime.tv_sec + (frameTime.tv_nsec / 1000000000.0);
  double secsSinceStart = totalTime.tv_sec + (totalTime.tv_nsec / 1000000000.0);
#endif

  m_cumulativeTime += secsSinceLastFrame;
  ++m_numFrames;
  ++m_totalNumFrames;
  // Update the displayed text once per second
  if( m_cumulativeTime >= 1.0 )
  {
    char displayString[256];
    snprintf( displayString, sizeof( displayString ), "Current/Average\nDisplay rate: %.2lf/%.2lf FPS\nTrack update rate: %.2lf/%.2lf Hz",
              m_numFrames / m_cumulativeTime, m_totalNumFrames / secsSinceStart,
              TrackManager::instance().currentUpdateRate(), TrackManager::instance().averageUpdateRate() );
    m_framerateStr->value( displayString );
    m_cumulativeTime = 0.0;
    m_numFrames = 0;
  }

  // Position the text at the top of the window
  TSLDeviceUnits screenX1, screenY1, screenX2, screenY2;
  layerHandler.drawingSurface()->getDUExtent( &screenX1, &screenY1, &screenX2, &screenY2 );

  TSLCoord screenPosition;
  renderingInterface->DUToTMC( (screenX2-screenX1) / 2.0, 0, &screenPosition.m_x, &screenPosition.m_y );
  m_framerateStr->position( screenPosition );
  renderingInterface->drawEntity( m_framerateStr );

  m_lastFrameTime = currentTime;

  return true;
}

