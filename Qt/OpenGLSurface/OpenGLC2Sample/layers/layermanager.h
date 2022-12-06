/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef LAYERMANAGER_H
#define LAYERMANAGER_H

// This class acts as a central repository for all MapLink data layers used by the sample.

class TSLStaticMapDataLayer;
class TSLCustomDataLayer;
class TSLOpenGLSurface;
class FramerateLayer;
class TrackLayer;

#include "tslenvelope.h"
#include "decluttermodel.h"
#include <string>

using std::string;

class LayerManager
{
public:
  LayerManager();
  ~LayerManager();

  bool loadMap( const char *mapFile );

  void attachLayersToSurface( TSLOpenGLSurface *surface );
  void detachLayersFromSurface( TSLOpenGLSurface *surface );
  void setFramerateLayerVisibility( TSLOpenGLSurface *surface, bool isVisible );
  void resetLayers( TSLOpenGLSurface *surface );

  DeclutterModel& declutterModel();

  static LayerManager& instance();

private:
  // The data layer that displays the map that all other layers sit on top of
  TSLStaticMapDataLayer *m_baseMapLayer;
  string m_baseMapName;

  // A custom data layer that displays the current redraw rate of the application
  FramerateLayer *m_framerateLayer;
  TSLCustomDataLayer *m_framerateCL;

  // A custom data layer that displays the tracks from the track manager
  TrackLayer *m_trackLayer;
  TSLCustomDataLayer *m_trackCL;
  string m_tracksLayerName;

  // Qt model implementation for mapping layer features into a tree view
  DeclutterModel m_declutterModel;
};

inline DeclutterModel& LayerManager::declutterModel()
{
  return m_declutterModel;
}

#endif // LAYERMANAGER_H
