/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/
#include <QWidget>

#include "layermanager.h"

#include <QOpenGLFunctions>

#include "MapLink.h"
#include "MapLinkOpenGLSurface.h"

#include "frameratelayer.h"
#include "tracklayer.h"
#include "decluttermodel.h"

LayerManager::LayerManager()
  : m_baseMapLayer( new TSLStaticMapDataLayer() )
  , m_baseMapName( "Map" )
  , m_framerateLayer( new FramerateLayer() )
  , m_framerateCL( new TSLCustomDataLayer() )
  , m_trackLayer( NULL )
  , m_trackCL( NULL )
  , m_tracksLayerName( "Tracks" )
{
  m_framerateCL->setClientCustomDataLayer( m_framerateLayer, false );
  // Give the layer a 128Mb cache. When tiled buffering is enabled this can be much smaller,
  // but otherwise larger values give better drawing performance.
  m_baseMapLayer->drawCacheSize( 128 * 1024 );
}

LayerManager::~LayerManager()
{
  m_baseMapLayer->destroy();
}

bool LayerManager::loadMap( const char *mapFile )
{
  bool success = m_baseMapLayer->loadData( mapFile );
  m_declutterModel.addLayerFeatures( QString::fromUtf8( m_baseMapName.c_str() ), m_baseMapLayer );

  TSLEnvelope extent;
  m_baseMapLayer->getTMCExtent( &extent.m_bottomLeft.m_x, &extent.m_bottomLeft.m_y,
                                &extent.m_topRight.m_x, &extent.m_topRight.m_y );

  // Create a copy of the map's coordinate system for use in the track update thread
  TSLCoordinateSystem *trackThreadCS = m_baseMapLayer->queryCoordinateSystem()->clone( 1000 );

  TrackManager::instance().setCoordinateAttributes( extent.bottomLeft().x(), extent.bottomLeft().y(),
                                                    extent.topRight().x(), extent.topRight().y(), trackThreadCS );
  return success;
}

void LayerManager::attachLayersToSurface( TSLOpenGLSurface *surface )
{
  surface->addDataLayer( m_baseMapLayer, m_baseMapName.c_str() );
  surface->setDataLayerProps( m_baseMapName.c_str(), TSLPropertyBuffered, 1 );

  // The tracks custom data layer is only created once the drawing surface is valid as
  // it uses QOpenGLFunctions which requires an active OpenGL context.
  m_trackLayer = new TrackLayer();
  m_trackCL = new TSLCustomDataLayer();
  m_trackCL->setClientCustomDataLayer( m_trackLayer, false );

  surface->addDataLayer( m_trackCL, m_tracksLayerName.c_str() );
  if( !m_trackLayer->initialise( surface ) )
  {
    // Something went wrong when creating the data layer, tracks will not be visible
    delete m_trackLayer;
    m_trackCL->destroy();
    m_trackLayer = NULL;
    m_trackCL = NULL;
  }
  else
  {
    m_declutterModel.addLayerFeatures( QString::fromUtf8( m_tracksLayerName.c_str() ), m_trackCL );
  }

  surface->addDataLayer( m_framerateCL, "framerate" );
  // Make the framerate layer hidden by default, this only needs to be displayed
  // while the track simulation is running as otherwise the application does not
  // continuously update.
  surface->setDataLayerProps( "framerate", TSLPropertyVisible, 0 );

  // Tell the declutter model about the drawing surface so that it can apply decluttering through it
  m_declutterModel.setDrawingSurface( surface );
}

void LayerManager::detachLayersFromSurface( TSLOpenGLSurface *surface )
{
  m_declutterModel.removeLayerFeatures( QString::fromUtf8( m_tracksLayerName.c_str() ) );

  surface->removeDataLayer( m_baseMapName.c_str() );

  // The custom data layers use 
  m_framerateCL->destroy();
  m_framerateCL = NULL;
  delete m_framerateLayer;
  m_framerateLayer = NULL;

  // The tracks custom data layer uses QOpenGLFunctions which is tied to the
  // GL surface widget's OpenGL context. This needs to be destroyed when the
  // widget is destroyed in order to avoid an application crash.
  m_trackCL->destroy();
  m_trackCL = NULL;
  delete m_trackLayer;
  m_trackLayer = NULL;

  m_declutterModel.setDrawingSurface( NULL );
}

void LayerManager::setFramerateLayerVisibility( TSLOpenGLSurface *surface, bool isVisible )
{
  m_framerateLayer->resetTimer();
  surface->setDataLayerProps( "framerate", TSLPropertyVisible, isVisible );
}

void LayerManager::resetLayers( TSLOpenGLSurface *surface )
{
  if( m_trackLayer )
  {
    m_trackLayer->reset( surface );
  }
}

LayerManager& LayerManager::instance()
{
  static LayerManager singleton;
  return singleton;
}
